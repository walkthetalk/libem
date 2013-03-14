#pragma once
/**
 * \file	exemodel/devicee.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdint>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

/**
 *\class devicee
 *\brief device pollee,used by \em poller
 */
namespace exemodel{
class poller;

template< typename T >
class devicee
: public pollee
, public evt_cb< T& > {
public:
	/**
	 * \brief ctor of devicee
	 * \param path	point to a pathname to open.
	 */
	explicit devicee(const char* path)
	: pollee(::open(path, O_RDWR), uint32_t(::EPOLLIN))
	, m_path(path)
	{
	}

	virtual ~devicee() {};
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &mgr, uint32_t evts)
	{
		ssize_t ret = this->read(&m_data, sizeof(m_data));
		if (ret == -1) {
			return;
		}

		this->exe(m_data);
	}
private:
	devicee(const devicee &rhs ) = delete;
	devicee &operator = (devicee & rhs) = delete;
private:
	const char* const m_path;
	T m_data;
};

}
