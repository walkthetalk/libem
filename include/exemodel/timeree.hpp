#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */
#include <sys/timerfd.h>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

namespace exemodel {

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
	explicit timeree(struct timespec interval = { 0, 0 }, bool reuse = true);
	virtual ~timeree();
public:
	/**
	 * \brief used for starting/stoping the timer
	 */
	void start(void);
	void stop(void);
public:
	/**
	 * \brief used for disposing the event caughted by \em poller attached
	 */
	virtual void dispose(poller & mgr, uint32_t evts);
private:
	timeree(const timeree & rhs) = delete;
	timeree & operator = (const timeree & rhs ) = delete;
private:
	struct timespec m_interval;
	bool m_reuse;
};

}
