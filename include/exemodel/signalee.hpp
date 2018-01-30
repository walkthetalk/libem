#pragma once
/**
 * \file	exemodel/signalee.hpp
 * \author	Yi Qingliang<niqingliang2003@gmail.com>
 */

#include <signal.h>

#include <functional>

#include "exemodel/pollee.hpp"

namespace exemodel{

class signalee : public pollee {
public:
	explicit signalee() = default;
	virtual ~signalee() = default;
public:
	int init(int signum);

	void bind(std::function<int (void)> cb)
	{
		m_processor = cb;
	}
	void unbind()
	{
		m_processor = nullptr;
	}
public:
	virtual int dispose(poller &, uint32_t evts);
private:
	signalee(const signalee &rhs ) = delete;
	signalee &operator = (signalee & rhs) = delete;

	std::function<int (void)> m_processor;
};

int mask_signal(int signum);

}
