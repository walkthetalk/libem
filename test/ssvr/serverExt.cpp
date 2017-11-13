#include <exemodel/poll_tools.hpp>

#include "serverExt.hpp"
#include <iostream>

serverExt::serverExt(uint16_t port)
: exemodel::wsserveree(port)
, m_sigterm(SIGUSR1)
, m_tmr({ {1,0}, {1,0} })
{
	this->setMessageCallback([this](cid client_id, void *buf, size_t len) {
		std::cout << "receive text message" << std::endl;
		m_cid = client_id;
		m_rcver.process(buf, len);
	},[this](cid client_id, void *buf, size_t len) {
		std::cout << "receive binary message" << std::endl;
		m_cid = client_id;
		m_brcver.process(buf, len);
	});
	this->setStateChangeCallback([this](cid client_id, bool connectState) {
		m_cid = client_id;
		if (connectState) {
			printf("Connect success!\n");
			m_tmr.start();
		}
		else {
			printf("Connect broken!\n");
			m_tmr.stop();
		}
	});
	/// for shutdown
	m_sigterm.connect([this]() {
		throw exemodel::exec_stop("service dev mng exit");
	});
	/// for battery
	m_tmr.connect([this](exemodel::poller&, uint64_t) -> void {
		bat_state_t bs;
		if(std::rand() % 100 > 49) {
			int bm = std::rand() % 3;
			switch (bm) {
				case 0:
					bs.power_mode = power_t::ADAPTER;
					break;
				case 1:
					bs.power_mode = power_t::CHARGING;
					break;
				case 2:
					bs.power_mode = power_t::UNKNOWN;
					break;
			}
		}
		else {
			bs.power_mode = power_t::BATTERY;
		}
		bs.percent = (std::rand() % 101);

		this->send<mid_t::bat_state>(bs);
	});
	this->add(m_sigterm);
	this->add(m_tmr);
}

serverExt::~serverExt()
{
	this->del(m_sigterm);
	this->del(m_tmr);
}
