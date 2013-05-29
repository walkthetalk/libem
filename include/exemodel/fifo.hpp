#pragma once
/**
 * \file	exemodel/fifo.hpp
 * \author	Yi Qingliang<niqingliang2003@gmail.com>
 */

#include <stdint.h>
#include <vector>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

/**
 *\class fifo
 *\brief device pollee,used by \em poller
 */
namespace exemodel{
class poller;

class fifo_readee
: public pollee
, public evt_cb< std::vector<uint8_t> & > {
public:
	/**
	 * \brief ctor of fifo_readee
	 * \param path	point to a pathname to open.
	 */
	explicit fifo_readee(const char* path);
	virtual ~fifo_readee();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &, uint32_t evts);

	const char * path(void) const
	{
		return m_path;
	}
private:
	fifo_readee(const fifo_readee &rhs ) = delete;
	fifo_readee &operator = (fifo_readee & rhs) = delete;
private:
	const char* const m_path;
	std::vector<uint8_t> m_data;
};

class fifo_writee
: public pollee
, public evt_cb< std::vector<uint8_t> & > {
public:
	/**
	 * \brief ctor of fifo_writee
	 * \param path	point to a pathname to open.
	 */
	explicit fifo_writee(const char* path);

	virtual ~fifo_writee();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &, uint32_t evts);

	const char * path(void) const
	{
		return m_path;
	}
private:
	fifo_writee(const fifo_writee &rhs ) = delete;
	fifo_writee &operator = (fifo_writee & rhs) = delete;
private:
	const char* const m_path;
	std::vector<uint8_t> m_data;
};

}
