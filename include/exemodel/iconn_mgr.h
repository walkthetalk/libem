#pragma once

namespace exemodel {

class poller;

class IConnMgr {
public:
	virtual void destroy(poller &, int) = 0;
};

}

