#include "zlog/zlog.hpp"

#include "exemodel/poller.hpp"
#include "exemodel/pollee.hpp"

namespace exemodel {

poller::poller()
: pollee()
{
}

poller::~poller()
{
}

int poller::init(void)
{
	int ret = ::epoll_create1(EPOLL_CLOEXEC);
	if (ret == -1) {
		return ret;
	}

	savefd(ret);

	const uint32_t evts = (EPOLLIN | EPOLLOUT | EPOLLPRI);
	saveevts(evts);

	return 0;
}

int poller::add(pollee & obj) const
{
	struct epoll_event evt;
	evt.events = obj.evts();
	evt.data.ptr = &obj;
	return ::epoll_ctl(this->fd(), EPOLL_CTL_ADD, obj.fd(), &evt);
}

int poller::del(pollee & obj) const
{
	return ::epoll_ctl(this->fd(), EPOLL_CTL_DEL, obj.fd(), NULL);
}

int poller::mod(pollee & obj) const
{
	struct epoll_event evt;
	evt.events = obj.evts();
	evt.data.ptr = &obj;
	return ::epoll_ctl(this->fd(), EPOLL_CTL_MOD, obj.fd(), &evt);
}

void poller::run()
{
	struct epoll_event evt;
	do {
		int ret = epoll_wait(this->fd(), &evt, 1, -1);
		if (ret <= 0) {
			// TODO: log
			continue;
		}

		pollee * p = (pollee *)evt.data.ptr;
		ret = p->dispose(*this, evt.events);
		if (ret) {
			return;
		}
	} while(true);
}

int poller::dispose(poller & /*mgr*/, uint32_t /*evts*/)
{
	struct epoll_event evt;
	do {
		int ret = epoll_wait(this->fd(), &evt, 1, 0);
		if (ret <= 0) {
			// TODO: log
			break;
		}

		ret = ((pollee *)evt.data.ptr)->dispose(*this, evt.events);
		if (ret) {
			return ret;
		}
	} while(true);

	return 0;
}

}
