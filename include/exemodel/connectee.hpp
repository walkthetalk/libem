#pragma once
/**
 * \file	exemodel/connectee.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

namespace exemodel {
/**
 * \class connectee
 * \brief connection pollee, used by \em poller.
 */
class connectee
: public pollee
, public evt_cb<poller&, uint32_t, connectee&> {
public:
	typedef std::function<void (poller &)> destroy_cb_t;
public:
	/**
	 * \brief ctor of connectee
	 * \param destroycb	the callback used when destoryed.
	 * \param fd		file descriptor representing the connection.
	 */
	explicit connectee(const destroy_cb_t & destroycb, int fd);
	virtual ~connectee() = default;
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
	connectee(const connectee & rhs) = delete;
	connectee & operator = (const connectee & rhs) = delete;
private:
	const destroy_cb_t m_destroycb;
};

}
