#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	// read/write

#include "exemodel/poll_tools.hpp"
#include "exemodel/clientee.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

clientee::clientee(uint32_t svrip, uint16_t svrport)
: pollee(::socket(AF_INET, ::SOCK_STREAM, 0),
	(uint32_t)(::EPOLLIN | ::EPOLLOUT | ::EPOLLRDHUP | ::EPOLLERR | ::EPOLLET))
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
		if (e.code() == std::error_code(EINPROGRESS, std::system_category())) {
			// wait it
		} else {
			throw;
		}
	}
}

void clientee::dispose(poller & mgr, uint32_t evts)
{
	if ((evts & ::EPOLLRDHUP) || (evts & ::EPOLLERR)) {
		emlog("clientee ERROR");
		return;
	}

	/// don't care EPOLLOUT any more.
	if (evts & ::EPOLLOUT) {
		m_evts &= ~EPOLLOUT;
		mgr.mod(*this);
	}

	clientee::args_t args = { mgr, *this, evts };
	this->exe(args);
}

size_t clientee::recv(void * buffer, size_t length)
{
	ssize_t ret = ::recv(_fd_(), buffer, length, 0);
	validate_ret(ret, "socket recv");

	return (size_t)ret;
}

size_t clientee::send(const void * buffer, size_t length)
{
	ssize_t ret = ::send(_fd_(), buffer, length, 0);
	validate_ret(ret, "socket send");

	return (size_t)ret;
}

}
