#pragma once
/**
 * \file	exemodel/devicee.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 */
#include "exemodel/pollee.hpp"

/**
 *\class devicee
 *\brief device pollee,used by \em poller
 */
namespace exemodel{

class devicee : public pollee {
public:
	explicit devicee() = default;
	virtual ~devicee() = default;
public:
	int init(const char *path, int oflag = O_RDWR, uint32_t evts = EPOLLIN)
	{
		int ret;
		ret = this->open(path, oflag);
		if (ret == -1)
			return ret;

		saveevts(evts);
		return 0;
	}

private:
	devicee(const devicee &rhs ) = delete;
	devicee &operator = (devicee & rhs) = delete;
};

}
