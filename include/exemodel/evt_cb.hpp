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
public:
	typedef std::function<void(_ArgTypes...)> cb_func_t;
	typedef evt_cb<_ArgTypes...> cb_obj_t;
public:
	evt_cb() : m_cb(nullptr) {}
	virtual ~evt_cb() {}
public:
	/**
	 * \brief connect the callback user specified.
	 */
	void connect(const cb_func_t & func)
	{
		m_cb = func;
	}

	void connect(const cb_obj_t & other)
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
	cb_func_t m_cb;
};

}

