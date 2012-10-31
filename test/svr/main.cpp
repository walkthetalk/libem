#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>

#include "exemodel/serveree.hpp"
#include "exemodel/poller.hpp"
#include "exemodel/poll_tools.hpp"

#include "msg/msg_tools.hpp"
#include "msg/test1.hpp"
#include "msg/test2.hpp"

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
		m_svr.connect(&self_t::__handler, this);
		this->add(m_svr);
	}

	virtual ~CSvr()
	{
		this->del(m_svr);
	}
private:
	void __handler(exemodel::serveree::args_t & args)
	{
		m_recver.fill_from(args.ctx);
		switch (m_recver.id()) {
			case msg::mid_t::test1: {
				msg::msg<msg::mid_t::test1> recved_msg;
				m_recver.convert(recved_msg);
				std::cout << "server recved test1: "
					<< (uint16_t)recved_msg.a << " "
					<< recved_msg.b << std::endl;

				msg::msg<msg::mid_t::test2> send_msg;
				send_msg.a = recved_msg.a * 20;
				send_msg.b = recved_msg.b * 2;

				std::cout << "server send test2: "
					<< send_msg.a << ":"
					<< (uint16_t)send_msg.b << std::endl;
				m_sender.fill_to(send_msg, args.ctx);
				} break;
			default:
				break;
		}
	}
private:
	exemodel::serveree m_svr;
	msg::recver m_recver;
	msg::sender m_sender;
};

int main(void)
{
	//signal(SIGPIPE,SIG_IGN);

	CSvr svr(65533);
	svr.run();

	return 0;
}
