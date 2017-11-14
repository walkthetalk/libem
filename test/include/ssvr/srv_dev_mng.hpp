#pragma once

#include "serverExt.hpp"

namespace svcDevMng {

class svcDeviceManager final : public serverExt {
private: typedef svcDeviceManager self_t;
public:
	svcDeviceManager(uint16_t port);
	virtual ~svcDeviceManager();
public:
	void fifo_processer(std::vector<uint8_t> & buf) override;
private:
	int m_pkgc;
};

}
