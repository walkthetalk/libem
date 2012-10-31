#include <sys/epoll.h>
#include <sys/socket.h>

#include "exemodel/poll_tools.hpp"
#include "exemodel/connectee.hpp"

namespace exemodel {

connectee::connectee(const destroy_cb_t & destroycb, int idx, int fd)
: pollee(fd, (uint32_t)(::EPOLLIN | ::EPOLLRDHUP | ::EPOLLERR | ::EPOLLET))
, m_destroycb(destroycb)
, m_idx(idx)
{
}

void connectee::dispose(poller & mgr, uint32_t evts)
{
	if ((evts & ::EPOLLRDHUP) || (evts & ::EPOLLERR)) {
		emlog("connectee ERROR");
		m_destroycb(mgr, m_idx);
		return;
	}

	connectee::args_t args = { mgr, *this, evts };
	this->exe(args);
}

size_t connectee::recv(void * buffer, size_t length)
{
	ssize_t ret = ::recv(_fd_(), buffer, length, 0);
	validate_ret(ret, "socket recv");

	return (size_t)ret;
}

size_t connectee::send(const void * buffer, size_t length)
{
	ssize_t ret = ::send(_fd_(), buffer, length, 0);
	validate_ret(ret, "socket send");

	return (size_t)ret;
}

}
