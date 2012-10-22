#pragma once

#include "exemodel/pollee.h"
#include "exemodel/evt_cb.h"

namespace exemodel {

class connectee : public pollee, public evt_cb<connectee> {
public:
	typedef std::function<void (poller &, int)> destroy_cb_t;
public:
	explicit connectee(const destroy_cb_t & destroycb, int idx, int fd);
	virtual ~connectee() = default;
public:
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	int _idx_(void) const { return m_idx; }
private:
	connectee(const connectee & rhs) = delete;
	connectee & operator = (const connectee & rhs) = delete;
private:
	const destroy_cb_t & m_destroycb;
	const int m_idx;
};

}
