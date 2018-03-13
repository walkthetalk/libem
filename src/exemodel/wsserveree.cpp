#include <cstring>	/// for memset

#include "zlog/zlog.hpp"

#include "exemodel/wsserveree.hpp"

namespace exemodel {

class wsee final : public pollee {
public:
	wsee() = default;
	~wsee() = default;

	int init(struct lws * wsi, struct lws_context * context, int fd, uint32_t evts)
	{
		savefd(fd);
		saveevts(evts);
		m_wsi = wsi;
		m_context = context;
		return 0;
	}

	virtual int dispose(poller & /*mgr*/, uint32_t evts)
	{
		struct pollfd i = {
			this->fd(),
			(short)this->evts(),
			(short)evts
		};
		lws_service_fd(m_context, &i);
		return 0;
	}

	int sendTextMessage(void *buf, size_t len)
	{
		return lws_write(m_wsi, (unsigned char *)buf, len, LWS_WRITE_TEXT);
	}

	int sendBinaryMessage(void *buf, size_t len)
	{
		return lws_write(m_wsi, (unsigned char *)buf, len, LWS_WRITE_BINARY);
	}
private:
	struct lws * m_wsi;
	struct lws_context * m_context;
};

int wsserveree::callback_http(
	struct lws *wsi,
	enum lws_callback_reasons reason,
	void */*user*/,
	void *in,
	size_t len)
{
	struct lws_context * context = lws_get_context(wsi);
	wsserveree * svr = (wsserveree *)lws_context_user(context);

	int ret = 0;
	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		ret = svr->__receiveConnectState(wsi, true);
		break;

	case LWS_CALLBACK_CLOSED:
		ret = svr->__receiveConnectState(wsi, false);
		break;

	case LWS_CALLBACK_ADD_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			//printf("wsi(%p) fd(%d)\n", (void*)wsi, pa.fd);
			zlog_debug("add poll fd: %#lx %d", (unsigned long)wsi, pa.fd);
			svr->__addSp(wsi, context, pa.fd, pa.events);
		}
		break;

	case LWS_CALLBACK_DEL_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			zlog_debug("del poll fd: %#lx %d", (unsigned long)wsi, pa.fd);
			svr->__delSp(wsi, pa.fd);
		}
		break;

	case LWS_CALLBACK_CHANGE_MODE_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			svr->__modSp(wsi, pa.fd, pa.events);
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
					svr->__receiveBinaryMessage(wsi, in, len);
				}
				else {
					svr->__receiveTextMessage(wsi, in, len);
				}
			}
		}
		break;
	default:
		break;
	}

	return ret;
}

struct lws_protocols wsserveree::protocols[] = {
	{
		"http-only",		/* name */
		wsserveree::callback_http,		/* callback */
		0,	/* per_session_data_size */
		256*1024,			/* max frame size / rx buffer */
		0,
		nullptr,
		0
	},
	{ nullptr, nullptr, 0, 0, 0, nullptr, 0 } /* terminator */
};

int wsserveree::init(uint16_t port)
{
	int ret = 0;
	ret = poller::init();
	if (ret == -1)
		return ret;

	struct lws_context_creation_info info;
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
		return -1;
	}

	return 0;
}

wsserveree::~wsserveree()
{
	if (m_pcontext) {
		lws_context_destroy(m_pcontext);
		m_pcontext = nullptr;
	}
}

int wsserveree::sendTextMessage(cid wsi, void * buffer, size_t length)
{
	wsee * pD = m_sps.at(wsi);
	return pD->sendTextMessage(buffer, length);
}

int wsserveree::sendBinaryMessage(cid wsi, void * buffer, size_t length)
{
	wsee * pD = m_sps.at(wsi);
	return pD->sendBinaryMessage(buffer, length);
}

void wsserveree::bind4TextMessage(msg_cb_t cb)
{
	m_rxTextCallback = cb;
}

void wsserveree::bind4BinaryMessage(msg_cb_t cb)
{
	m_rxBinaryCallback = cb;
}

void wsserveree::bind4StateChange(state_cb_t cb)
{
	m_stateChangeCallback = cb;
}

void wsserveree::__addSp(cid wsi, struct lws_context *context, int fd, uint32_t events)
{
	wsee * newSp = new wsee();
	int ret = newSp->init(wsi, context, fd, events);
	if (ret < 0) {
		delete newSp;
		return;
	}

	m_sps.insert(std::make_pair(wsi, newSp));
	this->add(*newSp);
}

void wsserveree::__delSp(cid wsi, int fd)
{
	wsee * pD = m_sps.at(wsi);
	if (fd != pD->fd()) {
		zlog_err("wsserveree: error fd(%d) for delete", fd);
	}
	this->del(*pD);
	m_sps.erase(wsi);
	delete pD;
}

void wsserveree::__modSp(cid wsi, int fd, uint32_t events)
{
	wsee * pD = m_sps.at(wsi);
	if (fd != pD->fd()) {
		zlog_err("wsserveree: error fd(%d) for modify", fd);
	}

	pD->saveevts(events);
	this->mod(*pD);
}

int wsserveree::__receiveTextMessage(cid wsi, void * data, size_t len)
{
	//wsee * pD = m_sps.at(wsi);
	if (m_rxTextCallback) {
		return m_rxTextCallback(wsi, data, len);
	}
	return 0;
}

int wsserveree::__receiveBinaryMessage(cid wsi, void * data, size_t len)
{
	if (m_rxBinaryCallback) {
		return m_rxBinaryCallback(wsi, data, len);
	}
	return 0;
}

int wsserveree::__receiveConnectState(cid wsi, bool isConnecting)
{
	if (m_stateChangeCallback) {
		return m_stateChangeCallback(wsi, isConnecting);
	}
	return 0;
}

void wsserveree::unbind()
{
	m_rxTextCallback = nullptr;
	m_rxBinaryCallback = nullptr;
	m_stateChangeCallback = nullptr;
}

}
