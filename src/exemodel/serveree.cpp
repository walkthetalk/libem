#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <assert.h>
#include <iostream>

#include "exemodel/poll_tools.h"
#include "exemodel/serveree.h"
#include "exemodel/poller.h"

namespace exemodel {

serveree::serveree(uint16_t port)
: pollee(::socket(AF_INET, ::SOCK_STREAM, 0),
	(uint32_t)(::EPOLLIN | ::EPOLLET))
, evt_cb<connectee>()
, m_destroycb(std::bind(&serveree::destroy, this, std::placeholders::_1, std::placeholders::_2))
, m_connectee(nullptr)
{
	int ret = 0;

	// reuse address when TIME_WAIT
	int opt = 1;
	ret = ::setsockopt(_fd_(), SOL_SOCKET, SO_REUSEADDR,
			 (const void*)&opt, sizeof(opt));
	validate_ret(ret, "setsockopt,reuseaddr");

	// bind local port
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	ret = ::bind(_fd_(),  (struct sockaddr *)&addr,  sizeof(addr));
	validate_ret(ret, "bind");

	// listen
	ret = ::listen(_fd_(), 1);
	validate_ret(ret, "listen");
}

serveree::~serveree()
{
	delete m_connectee;
}

void serveree::destroy(poller & mgr, int idx)
{
	assert(idx == 0);

	mgr.del(*m_connectee);
	delete m_connectee;
	m_connectee = nullptr;
}

void serveree::dispose(poller & mgr, uint32_t evts)
{
	try {
		int conn_sock = ::accept(_fd_(), NULL, NULL);
		validate_ret(conn_sock, "accept");

		if (m_connectee != nullptr) {
			// NOTE: only support single client
			delete m_connectee;
		}

		m_connectee = new connectee(m_destroycb, 0, conn_sock);
		m_connectee->connect(*this);
		mgr.add(*m_connectee);
	}
	catch (std::system_error & e) {
		std::cout << e.code() << "-" << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "serveree dispose: unkown exception" << std::endl;
	}
}

}
