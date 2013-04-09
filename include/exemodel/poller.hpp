#pragma once

#include "exemodel/pollee.hpp"

/**
 * \file	exemodel/poller.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
namespace exemodel {

/**
 * \class poller
 * \brief it can wait on some \em pollee(s) for events.
 */
class poller : public pollee {
public:
	explicit poller();
	virtual ~poller();
public:
	/**
	 * \brief the \em add / \em del / \em mod can be used to add/delete
	 * /modify pollee to/from/in this poller.
	 */
	void add(pollee & obj) const;
	void del(pollee & obj) const;
	void mod(pollee & obj) const;
public:
	/**
	 * \brief the \em run will poll all fd(s) in it continuously.
	 * \note  it won't return.
	 */
	void run(void);
	void dispose(poller & mgr, uint32_t evts) final;
private:
	poller(const poller & rhs) = delete;
	poller & operator = (const poller & rhs) = delete;
};

}
