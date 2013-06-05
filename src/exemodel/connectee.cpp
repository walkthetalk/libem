#include <sys/socket.h>

#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/connectee.hpp"

namespace exemodel {

connectee::connectee(const destroy_cb_t & destroycb, int fd)
: pollee(fd, (uint32_t)(::EPOLLIN | ::EPOLLRDHUP | ::EPOLLERR), "connectee")
, m_destroycb(destroycb)
{
}

void connectee::dispose(poller & mgr, uint32_t evts)
{
	if ((evts & ::EPOLLRDHUP) || (evts & ::EPOLLERR)) {
		zlog::zlog_warning("connectee ERROR");
		m_destroycb(mgr);
		return;
	}

	this->exe(mgr, evts, *this);
}

size_t connectee::recv(void * buffer, size_t length)
{
	ssize_t ret = ::recv(_fd(), buffer, length, 0);
	validate_ret(ret, "socket recv");

	return (size_t)ret;
}

size_t connectee::send(const void * buffer, size_t length)
{
	ssize_t ret = ::send(_fd(), buffer, length, 0);
	validate_ret(ret, "socket send");

	return (size_t)ret;
}

}
