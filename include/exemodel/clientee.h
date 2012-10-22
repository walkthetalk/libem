#pragma once

#include "exemodel/pollee.h"
#include "exemodel/evt_cb.h"

namespace exemodel {

class clientee : public pollee, public evt_cb<clientee> {
public:
	explicit clientee(uint32_t svrip, uint16_t svrport);
	virtual ~clientee() = default;
public:
	virtual void dispose(poller & mgr, uint32_t evts);
private:
	clientee(const clientee & rhs) = delete;
	clientee & operator = (const clientee & rhs) = delete;
private:
	const uint32_t m_svrip;
	const uint16_t m_svrport;
};

}
