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
, public evt_cb<timeree> {
public:
	/**
	 * \brief ctor of timeree
	 * \param interval	interval for the periodic timeree
	 * \param reuse		reuse the timer
	 */
	explicit timeree(bool once, struct timercycle value={0,0});
	virtual ~timeree();
public:
	/**
	 * \brief used for starting/stoping the timer
	 */
	void start(void);
	void stop(void);
	bool setcycle(struct timercycle cycle);
	void setmodel(bool once);       //false:loop   true:once
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
