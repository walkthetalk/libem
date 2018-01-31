//#include <time.h>

#include <sys/timerfd.h>

#include "zlog/zlog.hpp"

#include "exemodel/clk_info.hpp"

#include "exemodel/timeree.hpp"

namespace exemodel {

int timeree::__init(int clk_id)
{
	int ret = ::timerfd_create(clk_id, TFD_NONBLOCK | TFD_CLOEXEC);
	if (ret == -1)
		return ret;

	this->savefd(ret);
	this->saveevts(EPOLLIN);
	m_clk_id = clk_id;

	return 0;
}

int timeree::init_realtime()
{
	return __init(CLOCK_REALTIME);
}

int timeree::init_mono()
{
	return __init(CLOCK_MONOTONIC);
}

void timeree::start(void)
{
	const struct itimerspec param = {
		{ m_spec.it_interval.tv_sec, m_spec.it_interval.tv_nsec },
		{ m_spec.it_value.tv_sec, m_spec.it_value.tv_nsec },
	};
	int ret = ::timerfd_settime(this->fd(),
				    0, &param, nullptr);
	if (ret == -1) {
		zlog_err("timer: start error %d", errno);
	}
}

void timeree::start_abs(void)
{
	const struct itimerspec param = {
		{ m_spec.it_interval.tv_sec, m_spec.it_interval.tv_nsec },
		{ m_spec.it_value.tv_sec, m_spec.it_value.tv_nsec },
	};
	int ret = ::timerfd_settime(this->fd(),
				    TFD_TIMER_ABSTIME, &param, nullptr);
	if (ret == -1) {
		zlog_err("timer: start_abs error %d", errno);
	}
}

void timeree::stop(void)
{
	struct itimerspec param = { {0,0}, {0,0}, };
	int ret = ::timerfd_settime(this->fd(),
				    0, &param, nullptr);
	if (ret == -1) {
		zlog_err("timer: stop error %d", errno);
	}
}

int timeree::dispose(poller & mgr, uint32_t)
{
	uint64_t buf = 0;
	ssize_t ret = this->read(&buf, sizeof(buf));
	if(ret != sizeof(buf)){
		return 0;
	}

	return m_processor(mgr, buf);
}

void timeree::save_spec(const itimerspec_t & new_value)
{
	m_spec = new_value;
}

void timeree::save_spec(int laterms, int firstms)
{
	m_spec = {
		ms_to_timespec(laterms),
		ms_to_timespec(firstms),
	};
}

timespec_t timeree::get_res(void) const
{
	return get_clk_res(m_clk_id);
}
timespec_t timeree::get_time(void) const
{
	return get_clk_time(m_clk_id);
}
void timeree::set_time(const timespec_t & tp)
{
	set_clk_time(m_clk_id, tp);
}

}
