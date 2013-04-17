#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 * 		linxiang<linxiang2019@163.com>
 */
#include <sys/timerfd.h>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"
#include "exemodel/poll_tools.hpp"

namespace exemodel {

typedef ::timespec timespec_t;
typedef ::itimerspec itimerspec_t;

inline timespec_t ms_to_timespec(uint32_t ms)
{
	return { ms/1000, static_cast<uint16_t>(ms%1000) * 1000 * 1000 };
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
		, uint32_t(::EPOLLIN))
	, m_spec(spec)
	{
	}

	virtual ~timeree()
	{
	}

public:
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
		/// store new value
		m_spec = new_value;
		this->start();
	}

	void start_abs(const itimerspec_t & new_value)
	{
		/// store new value
		m_spec = new_value;

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
	virtual void dispose(poller & mgr, uint32_t evts)
	{
		uint64_t buf = 0;
		ssize_t ret = read(&buf, sizeof(buf));
		if(ret != sizeof(buf)){
			std::cout << "read error!" << std::endl;
			return;
		}

		this->exe(mgr, buf);
	}
private:
	int __set_spec(int flags,
		const itimerspec_t *new_value,
		itimerspec_t *old_value)
	{
		return ::timerfd_settime(_fd(), flags, new_value, old_value);
	}

	int get_spec(itimerspec_t *curr_value) const
	{
		return ::timerfd_gettime(_fd(), curr_value);
	}
private:
	timeree(const timeree & rhs) = delete;
	timeree & operator = (const timeree & rhs ) = delete;
private:
	itimerspec_t m_spec;
};

typedef timeree<CLOCK_MONOTONIC> monotonic_timeree;
typedef timeree<CLOCK_REALTIME> realtime_timeree;

}
