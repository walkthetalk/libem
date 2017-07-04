#pragma once
/**
 * \file	exemodel/wswsserveree.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <map>
#include <functional>
#include <libwebsockets.h>
#include "exemodel/poller.hpp"

namespace exemodel {

/**
 * \class wsserveree
 * \brief server pollee, used by \em poller.
 */
class wsee;
class wsserveree
: public poller {
public:
	typedef struct lws * cid;
	typedef std::function<void (cid, void *, size_t)> msg_cb_t;
public:
	/**
	 * \brief ctor of wsserveree
	 * \param port	the server will listen on.
	 */
	explicit wsserveree(uint16_t port);
	virtual ~wsserveree();
public:
	void sendTextMessage(cid wsi, void * buffer, size_t length);
	void sendBinaryMessage(cid wsi, void * buffer, size_t length);

	void setMessageCallback(msg_cb_t textMsgCb, msg_cb_t binaryMsgCb);
private:
	wsserveree(const wsserveree & rhs) = delete;
	wsserveree & operator = (const wsserveree & rhs) = delete;

private:
	static struct lws_protocols protocols[];

	static int callback_http(
		struct lws *wsi,
		enum lws_callback_reasons reason,
		void *user,
		void *in,
		size_t len);
	void __addSp(cid wsi, struct lws_context &context, int fd, uint32_t events);
	void __delSp(cid wsi, int fd);
	void __modSp(cid wsi, int fd, uint32_t events);
	void __receiveTextMessage(cid wsi, void * data, size_t len);
	void __receiveBinaryMessage(cid wsi, void * data, size_t len);
private:
	struct lws_context_creation_info m_info;
	struct lws_context * m_pcontext;
	std::map<cid, wsee*> m_sps;
	msg_cb_t m_rxTextCallback;
	msg_cb_t m_rxBinaryCallback;
	size_t m_buf_prepadding;
};

}
