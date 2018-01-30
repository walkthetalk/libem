#pragma once

#include "exemodel/pollee.hpp"

/**
 * \file	exemodel/poller.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
namespace exemodel {

class poller : public pollee {
public:
	explicit poller();
	virtual ~poller();
public:
	int init(void);
public:
	int add(pollee & obj) const;
	int del(pollee & obj) const;
	int mod(pollee & obj) const;
public:
	/**
	 * \brief the \em run will poll all fd(s) in it continuously.
	 * \note  it won't return.
	 */
	void run(void);
	int dispose(poller & mgr, uint32_t evts);
private:
	poller(const poller & rhs) = delete;
	poller & operator = (const poller & rhs) = delete;
};

}
