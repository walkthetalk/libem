#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <iomanip>

#include "exemodel/serveree.hpp"
#include "exemodel/poller.hpp"
#include "exemodel/poll_tools.hpp"

#include "msg/zmsg_cmm.hpp"
#include "msg/zmsg_packer.hpp"
#include "msg/zmsg_unpacker.hpp"
#include "msg/zmsg_msg.hpp"




class CSvr : public exemodel::poller {
private:
	typedef CSvr self_t;
public:
	CSvr(uint16_t port)
	: exemodel::poller()
	, m_svr(port)
	, m_recver()
	, m_sender()
	{
		m_svr.connect([this](exemodel::poller&, uint32_t evts, exemodel::connectee& conn)
		{
			zmsg::print_aux<decltype(std::cout)> pa(std::cout);
			if (evts & EPOLLIN) {
				// 1. read
				zmsg::rcver m_recver;
				m_recver.fill_from(conn);
				switch (m_recver.id()) {
					case zmsg::mid_t::test1: {
						zmsg::zmsg<zmsg::mid_t::test1> recved_msg;
						m_recver.convert_to(recved_msg);
						m_recver.b().print(std::cout);
						recved_msg.serialize(pa);
						pa. operator()("\n");

						
						DCL_ZMSG(test2) send_msg;

						send_msg.serialize(pa);
						pa. operator()("\n");
						
						m_sender.fill_to<false,true>(send_msg, m_svr, &exemodel::serveree::send);
						m_sender.b().print(std::cout);
						} break;
					default:
						break;
				}
			}
		});
		this->add(m_svr);
	}

	virtual ~CSvr()
	{
		this->del(m_svr);
	}

private:
	exemodel::serveree m_svr;
	zmsg::rcver m_recver;
	zmsg::sender m_sender;
};

int main(void)
{
	//signal(SIGPIPE,SIG_IGN);

	CSvr svr(65533);
	svr.run();

	return 0;
}
