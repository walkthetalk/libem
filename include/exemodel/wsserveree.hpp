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

class wsee;
class wsserveree : public poller {
public:
	typedef struct lws * cid;
	typedef std::function<int (cid, void *, size_t)> msg_cb_t;
	typedef std::function<int (cid, bool)> state_cb_t;
public:
	explicit wsserveree() = default;
	virtual ~wsserveree();
public:
	int init(uint16_t port);
public:
	int sendTextMessage(cid wsi, void * buffer, size_t length);
	int sendBinaryMessage(cid wsi, void * buffer, size_t length);

	void bind4TextMessage(msg_cb_t cb);
	void bind4BinaryMessage(msg_cb_t cb);
	void bind4StateChange(state_cb_t cb);
	void unbind();
private:
	wsserveree(const wsserveree & rhs) = delete;
	wsserveree & operator = (const wsserveree & rhs) = delete;

private:
	void __addSp(cid wsi, struct lws_context *context, int fd, uint32_t events);
	void __delSp(cid wsi, int fd);
	void __modSp(cid wsi, int fd, uint32_t events);
	int __receiveTextMessage(cid wsi, void * data, size_t len);
	int __receiveBinaryMessage(cid wsi, void * data, size_t len);
	int __receiveConnectState(cid wsi, bool connectState);
private:
	static struct lws_protocols protocols[];

	static int callback_http(
		struct lws *wsi,
		enum lws_callback_reasons reason,
		void *user,
		void *in,
		size_t len);
private:
	struct lws_context * m_pcontext;
	std::map<cid, wsee*> m_sps;
	msg_cb_t m_rxTextCallback;
	msg_cb_t m_rxBinaryCallback;
	state_cb_t m_stateChangeCallback;
};

}
