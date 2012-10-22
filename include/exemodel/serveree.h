#pragma once

#include "exemodel/evt_cb.h"
#include "exemodel/pollee.h"

#include "exemodel/connectee.h"

namespace exemodel {

class poller;

class serveree
: public pollee
, public evt_cb<connectee> {
public:
	explicit serveree(uint16_t port);
	virtual ~serveree();
public:
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	void destroy(poller & mgr, int idx);
private:
	serveree(const serveree & rhs) = delete;
	serveree & operator = (const serveree & rhs) = delete;
private:
	const connectee::destroy_cb_t m_destroycb;
	/*
	 * connection(s)
	 * TODO: support multi connections
	 */
	connectee * m_connectee;
};

}
