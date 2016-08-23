
#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/poller.hpp"
#include "exemodel/pollee.hpp"

namespace exemodel {

poller::poller()
: pollee(::epoll_create1(EPOLL_CLOEXEC), uint32_t(::EPOLLIN | ::EPOLLOUT | ::EPOLLPRI), "poller")
{
}

poller::~poller()
{
}

void poller::add(pollee & obj) const
{
	struct epoll_event evt;
	evt.events = obj._evts();
	evt.data.ptr = &obj;
	int ret = ::epoll_ctl(this->fd(), EPOLL_CTL_ADD, obj.fd(), &evt);
	validate_ret(ret, "epoll add fd");
}

void poller::del(pollee & obj) const
{
	int ret = ::epoll_ctl(this->fd(), EPOLL_CTL_DEL, obj.fd(), NULL);
	validate_ret(ret, "epoll del fd");
}

void poller::mod(pollee & obj) const
{
	struct epoll_event evt;
	evt.events = obj._evts();
	evt.data.ptr = &obj;
	int ret = ::epoll_ctl(this->fd(), EPOLL_CTL_MOD, obj.fd(), &evt);
	validate_ret(ret, "epoll mod fd");
}

void poller::run()
{
	try {
		struct epoll_event evt;
		do {
			int ret = epoll_wait(this->fd(), &evt, 1, -1);
			if (ret <= 0) {
				// TODO: log
				continue;
			}

			((pollee *)evt.data.ptr)->dispose(*this, evt.events);
		} while(true);
	}
	catch (const exec_stop & e) {
		zlog::zlog_info(e.what());
	}
}

void poller::dispose(poller & mgr, uint32_t evts)
{
	(void)mgr;
	(void)evts;

	struct epoll_event evt;
	do {
		int ret = epoll_wait(this->fd(), &evt, 1, 0);
		if (ret <= 0) {
			// TODO: log
			break;
		}

		((pollee *)evt.data.ptr)->dispose(*this, evt.events);
	} while(true);
}

}
