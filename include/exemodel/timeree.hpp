#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 * 			linxiang<linxiang2019@163.com>
 */
#include <sys/timerfd.h>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

namespace exemodel {

/**
 * timercycle for setting period
 */
struct timercycle{
	uint32_t sec;
	uint32_t nsec;
};

/**
 *\class timeree
 *\brief timeree pollee,used by \em poller
 */
class timeree
: public pollee
, public evt_cb<poller&, uint64_t> {
public:
	/**
	 * \brief ctor of timeree
	 * \param interval	interval for the periodic timeree
	 * \param oneshot	if true, oneshot, or periodic
	 */
	explicit timeree(struct timercycle interval = {0,0}, bool oneshot = false);
	virtual ~timeree();
public:
	/**
	 * \brief used for starting/stoping the timer
	 */
	void start(void);
	void stop(void);
	void setcycle(const struct timercycle cycle);
	void setcycle_ms(uint32_t ms)
	{
		const struct timercycle tmp = {
			ms / 1000,
			ms % 1000 * 1000,
		};
		setcycle(tmp);
	}
	void setmodel(bool oneshot);       //false:loop   true:once
public:
	/**
	 * \brief used for disposing the event caughted by \em poller attached
	 */
	virtual void dispose(poller & mgr, uint32_t evts);
private:
	timeree(const timeree & rhs) = delete;
	timeree & operator = (const timeree & rhs ) = delete;
private:
	struct timespec m_value;                //it_value
	struct timespec m_interval;             //it_interval
};

}
