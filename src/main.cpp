#include <thread>
#include <iostream>
#include <unistd.h>

#include "exemodel/serveree.h"
#include "exemodel/clientee.h"
#include "exemodel/poller.h"
#include "exemodel/poll_tools.h"

using namespace exemodel;

class CSvr : public poller {
private:
	typedef CSvr self_t;
public:
	CSvr(uint16_t port)
	: poller()
	, m_svr(port)
	{
		m_svr.connect(&self_t::__handler, this);
		this->add(m_svr);
	}

	virtual ~CSvr()
	{
		this->del(m_svr);
	}
private:
	void __handler(serveree::args_t & args)
	{
		char buf[100] = { '\0' };
		args.ctx.read(buf, 100);
		std::cout << "server recved from" << args.ctx._idx_() << ": " << buf << std::endl;
	}
private:
	serveree m_svr;
};

class CClient : public poller {
private:
	typedef CClient self_t;
public:
	CClient(uint32_t ip, uint16_t port)
	: m_client(ip, port)
	{
		m_client.connect(&self_t::__handler, this);
		this->add(m_client);
	}

	virtual ~CClient()
	{
		this->del(m_client);
	}
private:
	void __handler(clientee::args_t & args)
	{
		char buf[100] = { '\0' };
		args.ctx.read(buf, 100);
		std::cout << "client recved: " << buf << std::endl;
		args.ctx.write("world", 6);
	}
private:
	clientee m_client;
};

int main(void)
{
	CSvr svr(5900);
	CClient client(0x7F000001, 5900);
	std::thread th1(&CSvr::run, std::ref(svr));
	std::thread th2(&CClient::run, std::ref(client));

	th1.join();
	th2.join();

	return 0;
}
