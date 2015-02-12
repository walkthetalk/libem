#include <sys/socket.h>
#include <arpa/inet.h>

#include <assert.h>

#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/serveree.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

serveree::serveree(uint16_t port)
: pollee(::socket(AF_INET, ::SOCK_STREAM, 0),
	 (uint32_t)(::EPOLLIN),
	 "serveree")
, connectee::cb_obj_t()
, m_destroycb(std::bind(&serveree::destroy, this, std::placeholders::_1))
, m_connectee(nullptr)
{
	int ret = 0;

	// reuse address when TIME_WAIT
	int opt = 1;
	ret = ::setsockopt(this->fd(), SOL_SOCKET, SO_REUSEADDR,
			 (const void*)&opt, sizeof(opt));
	validate_ret(ret, "setsockopt,reuseaddr");

	// bind local port
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	ret = ::bind(this->fd(),  (struct sockaddr *)&addr,  sizeof(addr));
	validate_ret(ret, "bind");

	// listen
	ret = ::listen(this->fd(), 0);
	validate_ret(ret, "listen");
}

serveree::~serveree()
{
}

void serveree::destroy(poller & mgr)
{
	assert(m_connectee != nullptr);
	mgr.del(*m_connectee);
	m_connectee = nullptr;
}

void serveree::dispose(poller & mgr, uint32_t evts)
{
	if (evts & ~::EPOLLIN) {
		// TODO: need further process
		return;
	}

	int conn_sock = ::accept(this->fd(), NULL, NULL);
	validate_ret(conn_sock, "accept");

	if (m_connectee) {
		destroy(mgr);
	}

	m_connectee = std::unique_ptr<connectee>(
		new connectee(m_destroycb, conn_sock));
	m_connectee->connect(*this);
	mgr.add(*m_connectee);
}

size_t serveree::send(const void * buffer, size_t length)
{
	if (m_connectee) {
		return m_connectee->send(buffer, length);
	}
	else {
		zlog::zlog_warning("no client, it is needed for send msg!!");
		return 0;
	}
}

}
