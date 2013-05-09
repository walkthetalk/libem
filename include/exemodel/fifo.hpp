#pragma once
/**
 * \file	exemodel/fifo.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */
#include <sys/stat.h>

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
	explicit fifo_readee(const char* path)
	: pollee(::open(path, (O_RDONLY | O_NONBLOCK)),
		 (uint32_t)(::EPOLLIN))
	, m_path(path)
	{
	}

	virtual ~fifo_readee() {};
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &, uint32_t evts)
	{
		if (evts & ::EPOLLIN) {
			m_data.clear();

			uint8_t buf;
			do {
				ssize_t ret = this->read(&buf, sizeof(buf));
				if (ret <= 0) {
					break;
				}

				m_data.push_back(buf);
			} while (true);

			if (m_data.size()) {
				this->exe(m_data);
			}
		}
	}
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
	explicit fifo_writee(const char* path)
	: pollee(::open(path, (O_WRONLY | O_NONBLOCK)),
		 (uint32_t)(::EPOLLOUT))
	, m_path(path)
	{
	}

	virtual ~fifo_writee() {};
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &, uint32_t evts)
	{
		if (evts & ::EPOLLOUT) {
			m_data.clear();
			this->exe(m_data);

			ssize_t ret = this->write(m_data.data(), m_data.size());
			if (ret < 0
				|| (size_t)ret != m_data.size()) {
				return;
			}
		}
	}
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
