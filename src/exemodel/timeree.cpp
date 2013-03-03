#include <sys/epoll.h>
#include <unistd.h>

#include "exemodel/timeree.hpp"
#include "exemodel/poll_tools.hpp"

namespace exemodel{
timeree::timeree(struct timespec interval, bool reuse)
: pollee(::timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC),uint32_t(::EPOLLIN | ::EPOLLERR))
, m_interval(interval)
, m_reuse(reuse)
{
}

timeree::~timeree()
{
}

void timeree::start(void)
{
	//start timer
	struct itimerspec tmpTime = {
		m_interval,
		m_interval,
	};
	int ret = timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"start timer error!\n");
}

void timeree::stop(void)
{
	//stop timer
	struct itimerspec tmpTime = {
		 m_interval,
		 {0,0},
	};
	int ret = timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"stop timer error!\n");
}

void timeree::dispose(poller & mgr, uint32_t evts)
{
	if(evts & EPOLLERR){
	      printf("timer error occurd fd %d\n",_fd_());
	      return;
	}
	//read the timeout information
	uint64_t buf = 0;
	ssize_t ret = ::read(_fd_(), &buf, sizeof(buf));
	if(ret != sizeof(buf)){
		printf("read error!\n");
		return;
		}

	if(evts & EPOLLIN){
		timeree::args_t args = { mgr, *this, evts };
		this->exe(args);

		if(!m_reuse){
			stop();
		}
	}
}

}
