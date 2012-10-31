#pragma once

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

namespace exemodel {

class clientee : public pollee, public evt_cb<clientee> {
public:
	explicit clientee(uint32_t svrip, uint16_t svrport);
	virtual ~clientee() = default;
public:
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	size_t recv(void * buffer, size_t length);
	size_t send(const void * buffer, size_t length);
private:
	clientee(const clientee & rhs) = delete;
	clientee & operator = (const clientee & rhs) = delete;
private:
	const uint32_t m_svrip;
	const uint16_t m_svrport;
};

}
