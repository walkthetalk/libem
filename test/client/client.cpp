#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <iomanip>

#include "exemodel/clientee.hpp"
#include "exemodel/connectee.hpp"
#include "exemodel/poller.hpp"
#include "exemodel/poll_tools.hpp"

#include "test/client.hpp"
#include "msg/zmsg_cmm.hpp"
#include "msg/zmsg_packer.hpp"
#include "msg/zmsg_unpacker.hpp"
#include "msg/zmsg_msg.hpp"


CClient::CClient(uint32_t ip, uint16_t port)
: exemodel::poller()
, m_client(ip, port)
{
	m_client.connect([this](exemodel::poller&, uint32_t evts, exemodel::clientee& conn)
			{
				zmsg::print_aux<decltype(std::cout)> pa(std::cout);
				if (evts & EPOLLIN) {
					// 1. read
					
					m_recver.fill_from(conn);
					
					switch (m_recver.id()) {
						case zmsg::mid_t::test1: {
							zmsg::zmsg<zmsg::mid_t::test1> msg1;
							m_recver.convert_to(msg1);
							m_recver.b().print(std::cout);
							msg1.serialize(pa);
							pa.operator()("\n"); 
							
						}
							break;
						case zmsg::mid_t::test2: {
							zmsg::zmsg<zmsg::mid_t::test2> msg1;
							m_recver.convert_to(msg1);
							m_recver.b().print(std::cout);
							msg1.serialize(pa);
							pa.operator()("\n"); 
							
						}
							break;
						default:
							break;
					}
				}

				if (evts & EPOLLOUT) {
					

					
					DCL_ZMSG(test1) msg2={
						10,
						20,
					};

					msg2.serialize(pa);
					pa.operator()("\n"); 
					m_sender.fill_to<false, true>(msg2, m_client, &exemodel::clientee::send);
					m_sender.b().print(std::cout);
				}
			});
	this->add(m_client);
}

CClient::~CClient()
{
	this->del(m_client);
}