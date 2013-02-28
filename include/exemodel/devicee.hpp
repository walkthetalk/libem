#pragma once
/**
 * \file	exemodel/devicee.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */

#include <cstdint>

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
, public evt_cb<devicee>{
public:
	/**
	 * \brief ctor of devicee
	 * \param path	point to a pathname to open.
	 */
	explicit devicee(const char* path);
	virtual ~devicee();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &mgr, uint32_t evts);
private:
	devicee(const devicee &rhs ) = delete;
	devicee &operator = (devicee & rhs) = delete;
private:
	const char* const m_path;
};

}
