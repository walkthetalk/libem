#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "exemodel/poll_tools.h"
#include "exemodel/clientee.h"

namespace exemodel {

clientee::clientee(uint32_t svrip, uint16_t svrport)
: pollee(::socket(AF_INET, ::SOCK_STREAM, 0),
	(uint32_t)(::EPOLLIN | ::EPOLLET/* | ::EPOLLOUT*/))
, m_svrip(svrip)
, m_svrport(svrport)
{
	try {
		struct sockaddr_in svraddr;
		svraddr.sin_family = AF_INET;
		svraddr.sin_port = htons(svrport);
		svraddr.sin_addr.s_addr = htonl(svrip);

		int ret = ::connect(_fd_(),
			(struct sockaddr*)&svraddr, sizeof(svraddr));
		validate_ret(ret, "connect");
	} catch (std::system_error & e) {
		if (e.code() == std::error_code(EINPROGRESS, std::system_category())
			|| e.code() == std::error_code(EALREADY, std::system_category())) {
			// wait it
		} else {
			throw;
		}
	}
}

void clientee::dispose(poller & mgr, uint32_t evts)
{
	if (evts & EPOLLIN) {
		clientee::args_t args = { mgr, *this, evts };
		this->exe(args);
	}
}

}
