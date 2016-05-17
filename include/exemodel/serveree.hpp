#pragma once
/**
 * \file	exemodel/serveree.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <memory>

#include "exemodel/evt_cb.hpp"
#include "exemodel/pollee.hpp"

#include "exemodel/connectee.hpp"

namespace exemodel {

class poller;
/**
 * \class serveree
 * \brief server pollee, used by \em poller.
 */
class serveree
: public pollee
, public connectee::cb_obj_t {
public:
	/**
	 * \brief ctor of serveree
	 * \param port	the server will listen on.
	 */
	explicit serveree(uint16_t port, int max_conn = 1);
	virtual ~serveree();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller & mgr, uint32_t evts);
public:
	/**
	 * \brief hack: for send msg though server
	 * \todo maybe we need reconsider the arch of server/client
	 */
	size_t send(const void * buffer, size_t length);
private:
	/**
	 * \brief used for destorying connection.
	 */
	void destroy(poller & mgr);
private:
	serveree(const serveree & rhs) = delete;
	serveree & operator = (const serveree & rhs) = delete;
private:
	const connectee::destroy_cb_t m_destroycb;
	/**
	 * \brief connection(s) connected.
	 * \note only support single connection currently.
	 */
	std::unique_ptr<connectee> m_connectee;
};

}
