#include <iostream>
#include <sys/epoll.h>

#include "test/client.hpp"
#include "msg/msg_tools.hpp"
#include "msg/test1.hpp"
#include "msg/test2.hpp"

CClient::CClient(uint32_t ip, uint16_t port)
: m_client(ip, port)
{
	m_client.connect(&self_t::__handler, this);
	this->add(m_client);
}

CClient::~CClient()
{
	this->del(m_client);
}

void CClient::__handler(exemodel::clientee::args_t & args)
{
	if (args.evts & EPOLLIN) {
		// 1. read
		msg::recver pac;
		pac.fill_from(args.ctx);
		switch (pac.id()) {
			case msg::mid_t::test1: {
				msg::msg<msg::mid_t::test1> msg;
				pac.convert(msg);
				std::cout << "client recieved test1: " << msg.a << " " << msg.b << std::endl; }
				break;
			case msg::mid_t::test2: {
				msg::msg<msg::mid_t::test2> msg;
				pac.convert(msg);
				std::cout << "client recieved test2: " << msg.a << " " << (uint16_t)msg.b << " " << msg.c << std::endl; }
				break;
			default:
				break;
		}
	}

	if (args.evts & EPOLLOUT) {
		msg::sender pac;

		msg::msg<msg::mid_t::test1> msg;
		msg.a = 10;
		msg.b = 130;

		std::cout << "client send: " << (uint16_t)msg.a << " " << msg.b << std::endl;
		//pac.fill_to(msg, m_client, &exemodel::clientee::send);
		pac.fill_to(msg, m_client);
	}
}
