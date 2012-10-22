#pragma once

#include <functional>

namespace exemodel {

class poller;


template<typename pollee_t>
class evt_cb {
private:
	class callable {
	public:
		callable() = default;
		virtual ~callable() {}
	public:
		virtual void operator()(poller & mgr, pollee_t & obj, uint32_t evts) = 0;
		virtual callable * clone(void) const = 0;
	protected:
		callable(const callable & rhs) = default;
		callable & operator=(const callable & rhs) = delete;
	};

public:
	evt_cb() : m_cbinfo(nullptr) {}
	~evt_cb() {}
public:
	template<typename ctx_t>
	void connect(void (ctx_t::*mf)(poller &, pollee_t &, uint32_t), ctx_t * pctx)
	{
		m_cbinfo = std::bind(mf, pctx,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3);
	}

	void connect(const evt_cb<pollee_t> & other)
	{
		m_cbinfo = other.m_cbinfo;
	}

	void disconnect(void)
	{
		m_cbinfo = nullptr;
	}

	void exe(poller & mgr, pollee_t & obj, uint32_t evts)
	{
		if (m_cbinfo != nullptr) {
			m_cbinfo(mgr, obj, evts);
		}
	}

private:
	evt_cb(const evt_cb & rhs) = delete;
	evt_cb & operator=(const evt_cb & rhs) = delete;
private:
	std::function<void(poller &, pollee_t &, uint32_t)> m_cbinfo;
};

}

