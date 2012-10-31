#pragma once
#include <cstdint>

#include "exemodel/poller.hpp"
#include "exemodel/clientee.hpp"

class CClient : public exemodel::poller {
private:
	typedef CClient self_t;
public:
	CClient(uint32_t ip, uint16_t port);

	virtual ~CClient();
private:
	void __handler(exemodel::clientee::args_t & args);
private:
	exemodel::clientee m_client;
}; 
