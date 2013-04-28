#pragma once
#include <cstdint>

#include "exemodel/poller.hpp"
#include "exemodel/clientee.hpp"

#include "msg/zmsg_cmm.hpp"
#include "msg/zmsg_packer.hpp"
#include "msg/zmsg_unpacker.hpp"


class CClient : public exemodel::poller {
private:
	typedef CClient self_t;
public:
	CClient(uint32_t ip, uint16_t port);

	virtual ~CClient();
private:
	//void CClient::__handler(exemodel::poller&, uint32_t evts, exemodel::connectee& conn);
private:
	exemodel::clientee m_client;
	zmsg::sender m_sender;
	zmsg::rcver m_recver;
}; 
