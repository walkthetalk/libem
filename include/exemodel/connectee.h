#pragma once

#include "exemodel/pollee.h"
#include "exemodel/evt_cb.h"
#include "exemodel/iconn_mgr.h"

namespace exemodel {

class IConnMgr;

class connectee : public pollee, public evt_cb<connectee> {
public:
	explicit connectee(IConnMgr & master, int idx, int fd);
	virtual ~connectee() = default;
public:
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	int _idx_(void) const { return m_idx; }
private:
	connectee(const connectee & rhs) = delete;
	connectee & operator = (const connectee & rhs) = delete;
private:
	IConnMgr & m_master;
	const int m_idx;
};

}
