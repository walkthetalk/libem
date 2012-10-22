#pragma once

#include "exemodel/evt_cb.h"
#include "exemodel/iconn_mgr.h"
#include "exemodel/pollee.h"

#include "exemodel/poller.h"
#include "exemodel/connectee.h"

namespace exemodel {

class serveree
: public pollee
, public evt_cb<connectee>
, public IConnMgr {
public:
	explicit serveree(uint16_t port);
	virtual ~serveree();
public:
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	virtual void destroy(int idx);
private:
	serveree(const serveree & rhs) = delete;
	serveree & operator = (const serveree & rhs) = delete;
private:
	/*
	 * connection(s)
	 * TODO: support multi connections
	 */
	connectee * m_connectee;
};

}
