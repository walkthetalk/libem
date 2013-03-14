#pragma once
/**
 * \file	exemodel/evt_cb.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <functional>

namespace exemodel {

class poller;

/**
 * \class evt_cb
 * \brief event callback, store the callback for specific type of \em pollee.
 */
template<typename... _ArgTypes>
class evt_cb {
	typedef std::function<void(_ArgTypes...)> _cb_t;
public:
	typedef evt_cb<_ArgTypes...> cb_t;
public:
	evt_cb() : m_cb(nullptr) {}
	virtual ~evt_cb() {}
public:
	/**
	 * \brief connect the callback user specified.
	 */
	void connect(_cb_t&& func)
	{
		m_cb = func;
	}

	void connect(const cb_t & other)
	{
		m_cb = other.m_cb;
	}

	/**
	 * \brief disconnect the callback stored.
	 */
	void disconnect(void)
	{
		m_cb = nullptr;
	}

	/**
	 * \brief execute the callback stored.
	 */
	void exe(_ArgTypes...args)
	{
		m_cb(args...);
	}
private:
	evt_cb(const evt_cb & rhs) = delete;
	evt_cb & operator=(const evt_cb & rhs) = delete;
private:
	_cb_t m_cb;
};

}

