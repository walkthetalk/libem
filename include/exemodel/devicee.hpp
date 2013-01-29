#pragma once
/**
 * \file	exemodel/devee.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */

#include <cstdint>

#include "exemodel/pollee.hpp"
#include "exemodel/evt_cb.hpp"

/**
 *\class devee
 *\brief device pollee,used by \em poller
 */
namespace exemodel{
class poller;
    
class devee 
: public pollee   
, public evt_cb<devee>{
public:
	/**
	 * \brief ctor of devee
	 * \param path	point to a pathname to open.
	 */
	explicit devee(char* path);
	virtual ~devee();
public:
	/**
	 * \brief used for disposing the event caught by the \em poller attached.
	 */
	virtual void dispose(poller &mgr, uint32_t evts);
private:
	devee(const devee &rhs ) = delete;
	devee &operator = (devee & rhs) = delete;
private:
	char* m_path;
};

}
