#include "zlog/zlog.hpp"

#include "exemodel/wsserveree.hpp"
#include <cstring>
#include <system_error>

namespace exemodel {

class wsee : public pollee {
public:
	wsee(struct lws & wsi, struct lws_context & context, int fd, uint32_t evts, const char * info)
	: pollee(fd, evts, info)
	, m_wsi(wsi)
	, m_context(context)
	{
	}

	virtual void dispose(poller & /*mgr*/, uint32_t evts)
	{
		struct pollfd i = {
			this->fd(),
			(short)this->_evts(),
			(short)evts
		};
		lws_service_fd(&m_context, &i);
	}

	int sendTextMessage(void *buf, size_t len)
	{
		return lws_write(&m_wsi, (unsigned char *)buf, len, LWS_WRITE_TEXT);
	}

	int sendBinaryMessage(void *buf, size_t len)
	{
		return lws_write(&m_wsi, (unsigned char *)buf, len, LWS_WRITE_BINARY);
	}
private:
	struct lws & m_wsi;
	struct lws_context & m_context;
};

int wsserveree::callback_http(
	struct lws *wsi,
	enum lws_callback_reasons reason,
	void */*user*/,
	void *in,
	size_t len)
{
	struct lws_context & context = *lws_get_context(wsi);
	wsserveree & svr = *(wsserveree *)lws_context_user(&context);

	//printf("callback_http, reason: %d\n", (int)reason);
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED: {
			svr.__receiveConnectState(wsi, true);
		}
		break;

	case LWS_CALLBACK_CLOSED: {
			svr.__receiveConnectState(wsi, false);
		}
		break;

	case LWS_CALLBACK_ADD_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			//printf("wsi(%p) fd(%d)\n", (void*)wsi, pa.fd);
			svr.__addSp(wsi, context, pa.fd, pa.events);
		}
		break;

	case LWS_CALLBACK_DEL_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			svr.__delSp(wsi, pa.fd);
		}
		break;

	case LWS_CALLBACK_CHANGE_MODE_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			svr.__modSp(wsi, pa.fd, pa.events);
		}
		break;

	case LWS_CALLBACK_RECEIVE: {
			const size_t remaining = lws_remaining_packet_payload(wsi);
			const bool is_final = lws_is_final_fragment(wsi);
			if (remaining == 0 && is_final) {
				//char namebuf[200];
				//printf("receive from %s\n", lws_get_peer_simple(wsi, namebuf, sizeof(namebuf)));
				const bool is_binary = lws_frame_is_binary(wsi);
				if (is_binary) {
					svr.__receiveBinaryMessage(wsi, in, len);
				}
				else {
					svr.__receiveTextMessage(wsi, in, len);
				}
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

struct lws_protocols wsserveree::protocols[] = {
	{
		"http-only",		/* name */
		wsserveree::callback_http,		/* callback */
		0,	/* per_session_data_size */
		256*1024,			/* max frame size / rx buffer */
		0,
		nullptr
	},
	{ nullptr, nullptr, 0, 0, 0, nullptr} /* terminator */
};

wsserveree::wsserveree(uint16_t port)
: poller()
, m_buf_prepadding(LWS_PRE)
{
	auto & info = m_info;
	std::memset(&info, 0, sizeof(info));

	info.port = port;

	info.gid = -1;
	info.uid = -1;
	info.max_http_header_pool = 16;
	info.protocols = protocols;
	info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;
	info.timeout_secs = 5;
	info.user = this;

	m_pcontext = lws_create_context(&info);
	if (!m_pcontext) {
		throw std::runtime_error("init websocket server error!");
	}
}

wsserveree::~wsserveree()
{
	if (m_pcontext) {
		lws_context_destroy(m_pcontext);
		m_pcontext = nullptr;
	}
}

void wsserveree::sendTextMessage(cid wsi, void * buffer, size_t length)
{
	try {
		wsee * pD = m_sps.at(wsi);
		int ret = pD->sendTextMessage(buffer, length);
		if (ret < 0) {
			throw std::system_error(std::make_error_code(std::errc::network_unreachable));
		}
		else if ((size_t)ret < length) {
			throw std::system_error(std::make_error_code(std::errc::device_or_resource_busy));
		}
	}
	catch (const std::out_of_range & e) {
		throw std::system_error(std::make_error_code(std::errc::network_down));
	}
}

void wsserveree::sendBinaryMessage(cid wsi, void * buffer, size_t length)
{
	try {
		wsee * pD = m_sps.at(wsi);
		int ret = pD->sendBinaryMessage(buffer, length);
		if (ret < 0) {
			throw std::system_error(std::make_error_code(std::errc::network_unreachable));
		}
		else if ((size_t)ret < length) {
			throw std::system_error(std::make_error_code(std::errc::device_or_resource_busy));
		}
	}
	catch (const std::out_of_range & e) {
		throw std::system_error(std::make_error_code(std::errc::network_down));
	}
}

void wsserveree::setMessageCallback(msg_cb_t textMsgCb, msg_cb_t binaryMsgCb)
{
	m_rxTextCallback = textMsgCb;
	m_rxBinaryCallback = binaryMsgCb;
}

void wsserveree::setStateChangeCallback(std::function<void (cid, bool)> stateChangeCb)
{
	m_stateChangeCallback = stateChangeCb;
}

void wsserveree::__addSp(cid wsi, struct lws_context &context, int fd, uint32_t events)
{
	wsee * newSp = new wsee(*wsi, context, fd, events, "");
	m_sps.insert(std::make_pair(wsi, newSp));
	this->add(*newSp);
}

void wsserveree::__delSp(cid wsi, int fd)
{
	wsee * pD = m_sps.at(wsi);
	if (fd != pD->fd()) {
		zlog::zlog_err("wsserveree: error fd(%d) for delete", fd);
	}
	this->del(*pD);
	m_sps.erase(wsi);
	delete pD;
}

void wsserveree::__modSp(cid wsi, int fd, uint32_t events)
{
	wsee * pD = m_sps.at(wsi);
	if (fd != pD->fd()) {
		zlog::zlog_err("wsserveree: error fd(%d) for modify", fd);
	}
	pD->mod(*this, events);
}

void wsserveree::__receiveTextMessage(cid wsi, void * data, size_t len)
{
	//wsee * pD = m_sps.at(wsi);
	if (m_rxTextCallback) {
		m_rxTextCallback(wsi, data, len);
	}
}

void wsserveree::__receiveBinaryMessage(cid wsi, void * data, size_t len)
{
	//wsee * pD = m_sps.at(wsi);
	if (m_rxBinaryCallback) {
		m_rxBinaryCallback(wsi, data, len);
	}
}

void wsserveree::__receiveConnectState(cid wsi, bool isConnecting)
{
	if (m_stateChangeCallback) {
		m_stateChangeCallback(wsi, isConnecting);
	}
}

}
