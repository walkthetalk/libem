#include "exemodel/timeree.hpp"
#include "exemodel/poll_tools.hpp"

namespace exemodel{

timeree::timeree(struct timercycle interval, bool oneshot)
: pollee(::timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC),uint32_t(::EPOLLIN))
, m_value{ (decltype(m_value.tv_sec))interval.sec, (decltype(m_value.tv_nsec))interval.nsec, }
, m_oneshot(oneshot)
{
}

timeree::~timeree()
{
}

void timeree::start(void)
{
	struct itimerspec tmpTime;
	if (m_oneshot) {
		tmpTime = {
			{0,0},
			m_value,
		};

	}
	else {
		tmpTime = {
			m_value,
			m_value,
		};
	}
	int ret = ::timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"start timer error!\n");
}

void timeree::stop(void)
{
	struct itimerspec tmpTime = {
		{0,0},
		{0,0},
	};
	int ret = ::timerfd_settime(_fd_(), 0, &tmpTime, NULL);
	validate_ret(ret,"stop timer error!\n");
}

void timeree::setcycle(struct timercycle cycle)
{
	if ((cycle.nsec != 0) || (cycle.sec != 0)) {
		m_value.tv_sec = cycle.sec;
		m_value.tv_nsec = cycle.nsec;
	}
	else {
		std::cout << "WARNING:: cycle cann't set zero!" << std::endl;
	}

}

void timeree::setmodel(bool oneshot)
{
	m_oneshot = oneshot;
}

void timeree::dispose(poller & mgr, uint32_t evts)
{
	uint64_t buf = 0;
	ssize_t ret = ::read(_fd_(), &buf, sizeof(buf));
	if(ret != sizeof(buf)){
		std::cout << "read error!" << std::endl;
		return;
	}

	this->exe(mgr, buf);
}

}
