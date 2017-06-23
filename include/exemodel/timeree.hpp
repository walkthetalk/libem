#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 * 		linxiang<linxiang2019@163.com>
 */
#include <sys/timerfd.h>

#include "zlog/zlog.hpp"

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"
#include "exemodel/poll_tools.hpp"

namespace exemodel {

typedef ::timespec timespec_t;
typedef ::itimerspec itimerspec_t;

static inline timespec_t ms_to_timespec(uint32_t ms)
{
	return { (::__time_t)(ms/1000), static_cast<int32_t>(ms%1000) * 1000 * 1000 };
}

void set_normalized_timespec(timespec_t & ts,
			decltype(ts.tv_sec) sec,
			decltype(ts.tv_nsec) nsec);

static inline bool timespec_equal(const timespec_t &a,
					const timespec_t &b)
{
	return (a.tv_sec == b.tv_sec) && (a.tv_nsec == b.tv_nsec);
}

/*
* lhs < rhs:  return <0
* lhs == rhs: return 0
* lhs > rhs:  return >0
*/
static inline int timespec_compare(const timespec_t &lhs,
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

static inline timespec_t timespec_add(const timespec_t lhs,
					const timespec_t rhs)
{
	timespec_t ts_delta;
	set_normalized_timespec(ts_delta, lhs.tv_sec + rhs.tv_sec,
				lhs.tv_nsec + rhs.tv_nsec);
	return ts_delta;
}

/*
* sub = lhs - rhs, in normalized form
*/
static inline timespec_t timespec_sub(const timespec_t lhs,
						const timespec_t rhs)
{
	timespec_t ts_delta;
	set_normalized_timespec(ts_delta, lhs.tv_sec - rhs.tv_sec,
				lhs.tv_nsec - rhs.tv_nsec);
	return ts_delta;
}


template< clockid_t clk_id >
class clock_info {
public:
	static timespec_t get_res(void)
	{
		timespec_t res;
		int ret = ::clock_getres(clk_id, &res);
		validate_ret(ret, "get clock res!");
		return res;
	}

	static timespec_t get_time(void)
	{
		timespec_t tp;
		int ret = ::clock_gettime(clk_id, &tp);
		validate_ret(ret, "get clock time!");
		return tp;
	}

	static int set_time(const timespec_t & tp)
	{
		return clock_settime(clk_id, &tp);
	}
};

/**
 *\class timeree
 *\brief timeree pollee,used by \em poller
 */
template< clockid_t clk_id >
class timeree
: public pollee
, public evt_cb<poller&, uint64_t> {
public:
	typedef clock_info<clk_id> info;
public:
	explicit timeree(itimerspec_t spec = { {0,0}, {0,0} })
	: pollee(
		::timerfd_create(clk_id, TFD_NONBLOCK | TFD_CLOEXEC)
		, uint32_t(::EPOLLIN)
		, "timeree")
	, m_spec(spec)
	{
	}

	virtual ~timeree()
	{
	}

public:
	/**
	 * \brief set spec
	 */
	void set_spec(const itimerspec_t & new_value)
	{
		m_spec = new_value;
	}
	/**
	 * \brief used for starting/stoping the timer
	 */
	void start(void)
	{
		int ret = __set_spec(0, &m_spec, NULL);
		validate_ret(ret,"start timer error!\n");
	}

	void start(const itimerspec_t & new_value)
	{
		this->set_spec(new_value);
		this->start();
	}

	void start(const uint32_t laterms, const uint32_t firstms)
	{
		this->start({ ms_to_timespec(laterms), ms_to_timespec(firstms), });
	}

	void start_abs(const itimerspec_t & new_value)
	{
		this->set_spec(new_value);
		int ret = __set_spec(TFD_TIMER_ABSTIME, &new_value, NULL);
		validate_ret(ret,"start timer abs error!\n");
	}

	void stop(void)
	{
		struct itimerspec new_value = {	{0,0}, {0,0}, };
		int ret = __set_spec(0, &new_value, NULL);
		validate_ret(ret,"stop timer error!\n");
	}
public:
	/**
	 * \brief used for disposing the event caughted by \em poller attached
	 */
	virtual void dispose(poller & mgr, uint32_t)
	{
		uint64_t buf = 0;
		ssize_t ret = read(&buf, sizeof(buf));
		if(ret != sizeof(buf)){
			zlog::zlog_warning("exemodel: timeree read error!");
			return;
		}

		this->exe(mgr, buf);
	}
private:
	int __set_spec(int flags,
		const itimerspec_t *new_value,
		itimerspec_t *old_value)
	{
		return ::timerfd_settime(this->fd(), flags, new_value, old_value);
	}

	int get_spec(itimerspec_t *curr_value) const
	{
		return ::timerfd_gettime(this->fd(), curr_value);
	}
private:
	timeree(const timeree & rhs) = delete;
	timeree & operator = (const timeree & rhs ) = delete;
private:
	itimerspec_t m_spec;
};

typedef clock_info<CLOCK_MONOTONIC> monotonic_clock_info;
typedef clock_info<CLOCK_REALTIME> realtime_clock_info;

typedef timeree<CLOCK_MONOTONIC> monotonic_timeree;
typedef timeree<CLOCK_REALTIME> realtime_timeree;

}
