#pragma once
/**
 * \file	exemodel/signalee.hpp
 * \author	Yi Qingliang<niqingliang2003@gmail.com>
 */

#include <signal.h>

#include <stdint.h>
#include <vector>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

/**
 *\class fifo
 *\brief device pollee,used by \em poller
 */
namespace exemodel{
class poller;

class signalee
: public pollee
, public evt_cb< void > {
public:
	/**
	 * \brief ctor of signalee
	 * \param path	point to a pathname to open.
	 */
	explicit signalee(int signum);
	virtual ~signalee();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &, uint32_t evts);
private:
	signalee(const signalee &rhs ) = delete;
	signalee &operator = (signalee & rhs) = delete;
};

void mask_signal(int signum);

}
