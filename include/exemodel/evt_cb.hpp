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
template<typename pollee_t>
class evt_cb {
public:
	/**
	 * \brief the argument of callback.
	 */
	struct args_t {
		poller & mgr;
		pollee_t & ctx;
		uint32_t & evts;
	};
public:
	evt_cb() : m_cbinfo(nullptr) {}
	~evt_cb() {}
public:
	/**
	 * \brief connect the callback user specified.
	 */
	/// \note deprecated
	template<typename obj_t>
	void connect(void (obj_t::*mf)(args_t & args), obj_t * pObj)
	{
		m_cbinfo = [mf, pObj](args_t & arg)	{
			(pObj->*mf)(arg);
		};
	}
	/// \note deprecated
	template<typename obj_t>
	void connect(void (obj_t::*mf)(void), obj_t * pObj)
	{
		m_cbinfo = [mf, pObj](args_t &)	{
			(pObj->*mf)();
		};
	}

	void connect(std::function<void(args_t &)>&& func)
	{
		m_cbinfo = func;
	}

	void connect(const evt_cb<pollee_t> & other)
	{
		m_cbinfo = other.m_cbinfo;
	}

	/**
	 * \brief disconnect the callback stored.
	 */
	void disconnect(void)
	{
		m_cbinfo = nullptr;
	}

	/**
	 * \brief execute the callback stored.
	 */
	void exe(args_t & args)
	{
		m_cbinfo(args);
	}

private:
	evt_cb(const evt_cb & rhs) = delete;
	evt_cb & operator=(const evt_cb & rhs) = delete;
private:
	std::function<void(args_t &)> m_cbinfo;
};

}

