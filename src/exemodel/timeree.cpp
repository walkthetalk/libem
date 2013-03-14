#include <sys/epoll.h>
#include <unistd.h>

#include "exemodel/timeree.hpp"
#include "exemodel/poll_tools.hpp"

namespace exemodel{
timeree::timeree(struct timercycle interval, bool oneshot)
:pollee(::timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC),uint32_t(::EPOLLIN))
{
	m_value.tv_sec = interval.sec;
	m_value.tv_nsec = interval.nsec;
	setmodel(oneshot);
}

timeree::~timeree()
{
}

void timeree::start(void)
{
	//start timer
	struct itimerspec tmpTime = {
		m_interval,
		m_value,
	};
	int ret = ::timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"start timer error!\n");
}

void timeree::stop(void)
{
	//stop timer
	struct itimerspec tmpTime = {
		 m_interval,
		 {0,0},
	};
	int ret = ::timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"stop timer error!\n");
}

void timeree::setcycle(struct timercycle cycle)
{
	//reset cycle
	if((cycle.nsec!=0)||(cycle.sec!=0))
	{
		m_value.tv_sec=cycle.sec;
		m_value.tv_nsec=cycle.nsec;
	}else
	{
		std::cout << "WARNING:: cycle cann't set zero!" << std::endl;
	}

}

void timeree::setmodel(bool oneshot)
{
	//set model
	if(oneshot)             //ONCE
	{
		m_interval.tv_nsec=0;
		m_interval.tv_sec=0;
	}
	else               //LOOP
	{
		m_interval.tv_nsec=1;
	}
}

void timeree::dispose(poller & mgr, uint32_t evts)
{
	uint64_t buf = 0;
	ssize_t ret = ::read(_fd_(), &buf, sizeof(buf));
	if(ret != sizeof(buf)){
		std::cout << "read error!" << std::endl;
		return;
	}

	timeree::args_t args = { mgr, *this, evts };
	this->exe(args);
}

}
