#pragma once
/**
 * \file	exemodel/fifo.hpp
 * \author	Yi Qingliang<niqingliang2003@gmail.com>
 */
#include <functional>

#include "exemodel/pollee.hpp"

namespace exemodel{

class fifo_readee : public pollee {
public:
	typedef std::function<int (char * buf, size_t len)> cb_t;
public:
	explicit fifo_readee() = default;
	virtual ~fifo_readee() = default;
public:
	int init(const char *path);
	void bind(cb_t cb)
	{
		m_processor = cb;
	}
	void unbind()
	{
		m_processor = nullptr;
	}
public:
	virtual int dispose(poller &, uint32_t evts);
private:
	fifo_readee(const fifo_readee &rhs ) = delete;
	fifo_readee &operator = (fifo_readee & rhs) = delete;
private:
	cb_t m_processor;
};

class fifo_writee : public pollee {
public:
	typedef std::function<int (void)> cb_t;
public:
	explicit fifo_writee() = default;
	virtual ~fifo_writee() = default;
public:
	int init(const char *path);
	void bind(cb_t cb)
	{
		m_processor = cb;
	}
	void unbind()
	{
		m_processor = nullptr;
	}
public:
	virtual int dispose(poller &, uint32_t evts);
private:
	fifo_writee(const fifo_writee &rhs ) = delete;
	fifo_writee &operator = (fifo_writee & rhs) = delete;
private:
	cb_t m_processor;
};

}
