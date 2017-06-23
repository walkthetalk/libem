#include "exemodel/wsserveree.hpp"
#include <cstring>
#include <stdexcept>

namespace exemodel {

class wsee : public pollee {
public:
	wsee(struct lws_context & context, int fd, uint32_t evts, const char * info)
	: pollee(fd, evts, info)
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
private:
	struct lws_context & m_context;
};

static int callback_http(
	struct lws *wsi,
	enum lws_callback_reasons reason,
	void *user,
	void *in,
	size_t /*len*/)
{
	struct lws_context & context = *lws_get_context(wsi);
	wsserveree & svr = *(wsserveree *)lws_context_user(&context);
	wsee * & ppollee = *(wsee **)user;

	switch (reason) {
	case LWS_CALLBACK_ADD_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			ppollee = new wsee(context, pa.fd, pa.events, "");
			svr.add(*ppollee);
		}
		break;

	case LWS_CALLBACK_DEL_POLL_FD: {
			svr.del(*ppollee);
			delete ppollee;
			ppollee = nullptr;
		}
		break;

	case LWS_CALLBACK_CHANGE_MODE_POLL_FD: {
			struct lws_pollargs & pa = *(struct lws_pollargs *)in;
			ppollee->mod(svr, pa.events);
		}
		break;
	default:
		break;
	}

	return 0;
}

static struct lws_protocols protocols[] = {
	{
		"http-only",		/* name */
		callback_http,		/* callback */
		sizeof(wsee *),	/* per_session_data_size */
		64*1024,			/* max frame size / rx buffer */
		0,
		nullptr
	},
	{ nullptr, nullptr, 0, 0, 0, nullptr } /* terminator */
};

wsserveree::wsserveree(uint16_t port)
: poller()
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

}
