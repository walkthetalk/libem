#pragma once
/**
 * \file	exemodel/wswsserveree.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <libwebsockets.h>
#include "exemodel/poller.hpp"

namespace exemodel {

/**
 * \class wsserveree
 * \brief server pollee, used by \em poller.
 */
class wsserveree
: public poller {
public:
	/**
	 * \brief ctor of wsserveree
	 * \param port	the server will listen on.
	 */
	explicit wsserveree(uint16_t port);
	virtual ~wsserveree();
public:
	size_t send(const void * buffer, size_t length);
private:
	wsserveree(const wsserveree & rhs) = delete;
	wsserveree & operator = (const wsserveree & rhs) = delete;
private:
	struct lws_context_creation_info m_info;
	struct lws_context * m_pcontext;
};

}
