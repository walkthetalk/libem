#pragma once
/**
 * \file	exemodel/clientee.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

namespace exemodel {

class poller;

/**
 * \class clientee
 * \brief client pollee, used by \em poller.
 */
class clientee
: public pollee
, public evt_cb<poller&, uint32_t, clientee&> {
public:
	/**
	 * \brief ctor of clientee
	 * \param svrip		server ip connected.
	 * \param svrport	server port connected.
	 */
	explicit clientee(uint32_t svrip, uint16_t svrport);
	virtual ~clientee() = default;
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	/**
	 * \brief used for recieving / sending data from / to the \em clientee.
	 */
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
