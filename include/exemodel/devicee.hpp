#pragma once
/**
 * \file	exemodel/devicee.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */
#include <sys/stat.h>

#include <stdint.h>
#include <string>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

/**
 *\class devicee
 *\brief device pollee,used by \em poller
 */
namespace exemodel{
class poller;

class devicee
: public pollee
, public evt_cb< poller &, uint32_t > {
public:
	/**
	 * \brief ctor of devicee
	 * \param path	point to a pathname to open.
	 */
	explicit devicee(const char* path)
	: pollee(::open(path, O_RDWR), uint32_t(::EPOLLIN), path)
	, m_path(path)
	{
	}

	virtual ~devicee() {};
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller & p, uint32_t evts)
	{
		this->exe(p, evts);
	}

	const char * path(void) const
	{
		return m_path.c_str();
	}
private:
	devicee(const devicee &rhs ) = delete;
	devicee &operator = (devicee & rhs) = delete;
private:
	const std::string m_path;
};

}
