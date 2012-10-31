#pragma once

#include <functional>

namespace exemodel {

class poller;

template<typename pollee_t>
class evt_cb {
public:
	struct args_t {
		poller & mgr;
		pollee_t & ctx;
		uint32_t & evts;
	};
public:
	evt_cb() : m_cbinfo(nullptr) {}
	~evt_cb() {}
public:
	template<typename obj_t>
	void connect(void (obj_t::*mf)(args_t & args), obj_t * pObj)
	{
		m_cbinfo = std::bind(mf, pObj,
				std::placeholders::_1);
	}
	template<typename obj_t>
	void connect(void (obj_t::*mf)(void), obj_t * pObj)
	{
		m_cbinfo = std::bind([mf, pObj](args_t &)
				{
					(pObj->*mf)();
				},
				std::placeholders::_1);
	}

	void connect(const evt_cb<pollee_t> & other)
	{
		m_cbinfo = other.m_cbinfo;
	}

	void disconnect(void)
	{
		m_cbinfo = nullptr;
	}

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

