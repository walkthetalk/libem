//#include <time.h>

#include <sys/timerfd.h>

#include "zlog/zlog.hpp"

#include "exemodel/timeree.hpp"

namespace exemodel {

static constexpr int32_t NSEC_PER_SEC = 1000000000L;
static timespec_t set_normalized_timespec(
	long sec,
	long nsec)
{
	while (nsec >= NSEC_PER_SEC) {
		/*
		* The following asm() prevents the compiler from
		* optimising this loop into a modulo operation. See
		* also __iter_div_u64_rem() in include/linux/time.h
		*/
		asm("" : "+rm"(nsec));
		nsec -= NSEC_PER_SEC;
		++sec;
	}
	while (nsec < 0) {
		asm("" : "+rm"(nsec));
		nsec += NSEC_PER_SEC;
		--sec;
	}

	return { sec, nsec };
}

timespec_t ms_to_timespec(int ms)
{
	return {
		(ms/1000),
		(ms%1000) * 1000 * 1000
	};
}

bool timespec_equal(const timespec_t &a,
		    const timespec_t &b)
{
	return (a.tv_sec == b.tv_sec) && (a.tv_nsec == b.tv_nsec);
}

int timespec_compare(const timespec_t &lhs,
		     const timespec_t &rhs)
{
	if (lhs.tv_sec < rhs.tv_sec) {
		return -1;
	}
	if (lhs.tv_sec > rhs.tv_sec) {
		return 1;
	}
	if (lhs.tv_nsec < rhs.tv_nsec) {
		return -1;
	}
	if (lhs.tv_nsec > rhs.tv_nsec) {
		return 1;
	}
	return 0;
}

timespec_t timespec_add(const timespec_t lhs,
			const timespec_t rhs)
{
	return set_normalized_timespec(
			lhs.tv_sec + rhs.tv_sec,
			lhs.tv_nsec + rhs.tv_nsec);
}

timespec_t timespec_sub(const timespec_t lhs,
			const timespec_t rhs)
{
	return set_normalized_timespec(
			lhs.tv_sec - rhs.tv_sec,
			lhs.tv_nsec - rhs.tv_nsec);
}


int timeree::init(int clk_id)
{
	int ret = ::timerfd_create(clk_id, TFD_NONBLOCK | TFD_CLOEXEC);
	if (ret == -1)
		return ret;

	this->savefd(ret);
	this->saveevts(EPOLLIN);
	m_clk_id = clk_id;

	return 0;
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

timespec_t timeree::get_res(void)
{
	struct timespec param;
	int ret = ::clock_getres(m_clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: get_res %d", errno);
	}
	return { param.tv_sec, param.tv_nsec, };
}

timespec_t timeree::get_time(void)
{
	struct timespec param;
	int ret = ::clock_gettime(m_clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: get_time %d", errno);
	}
	return { param.tv_sec, param.tv_nsec, };
}

void timeree::set_time(const timespec_t & tp)
{
	struct timespec param = { tp.tv_sec, tp.tv_nsec, };
	int ret = ::clock_settime(m_clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: set_time %d", errno);
	}
}

}
