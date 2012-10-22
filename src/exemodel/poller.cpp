//#include <errno.h>
#include <unistd.h>	// close
#include <sys/epoll.h>
//#include <sys/syscall.h>
#include "exemodel/poller.h"
#include "exemodel/pollee.h"
#include "exemodel/poll_tools.h"

namespace exemodel {

poller::poller()
: m_fd(::epoll_create1(EPOLL_CLOEXEC))
{
	validate_fd(m_fd);
}

poller::~poller()
{
	::close(m_fd);
}

void poller::add(pollee & obj) const
{
	struct epoll_event evt;
	evt.events = obj._evts_();
	evt.data.ptr = &obj;
	int ret = ::epoll_ctl(m_fd, EPOLL_CTL_ADD, obj._fd_(), &evt);
	validate_ret(ret, "epoll add fd");
}

void poller::del(pollee & obj) const
{
	int ret = ::epoll_ctl(m_fd, EPOLL_CTL_DEL, obj._fd_(), NULL);
	validate_ret(ret, "epoll del fd");
}

void poller::run()
{
	struct epoll_event evt;
	do {
		// wait for change
		int ret = epoll_wait(m_fd, &evt, 1, -1);
		if (ret <= 0)
		{
			// TODO: log
			continue;
		}
		// handle change
		((pollee *)evt.data.ptr)->dispose(*this, evt.events);
	} while(true);
}

}
