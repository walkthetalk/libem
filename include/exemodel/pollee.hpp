#pragma once
/**
 * \file	exemodel/pollee.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <sys/epoll.h>

#include "exemodel/file.hpp"

namespace exemodel {

class poller;

class pollee : public file {
public:
	explicit pollee() = default;
	virtual ~pollee() = default;
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 * \note all subclasses of \em pollee must implement it.
	 * return: 0 - success
	 *         or  terminate thread
	 */
	virtual int dispose(poller & mgr, uint32_t evts) = 0;
public:
	int setfl_nonblock();
	int setfd_cloexec();

	void saveevts(uint32_t evts) {
		m_evts = evts;
	}
	uint32_t evts(void) const {
		return m_evts;
	}
private:
	pollee(const pollee & rhs) = delete;
	pollee & operator = (const pollee & rhs) = delete;
private:
	uint32_t  m_evts;
};

}
