#pragma once

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
		class callablemf : public callable {
		public:
			callablemf(decltype(mf) mf, ctx_t * pctx)
			: m_mf(mf)
			, m_ctx(pctx)
			{
			}
		public:
			virtual void operator()(poller & mgr, pollee_t & obj, uint32_t evts)
			{
				(m_ctx->*m_mf)(mgr, obj, evts);
			}
			virtual callable * clone(void) const
			{
				return new callablemf(*this);
			}
		private:
			callablemf(const callablemf & rhs) = default;
			callablemf & operator=(const callablemf & rhs) = delete;
		private:
			decltype(mf) m_mf;
			ctx_t * m_ctx;
		};
		m_cbinfo = new callablemf(mf, pctx);
	}

	void connect(const evt_cb<pollee_t> & other)
	{
		m_cbinfo = other.m_cbinfo->clone();
	}

	void disconnect(void)
	{
		delete m_cbinfo;
	}

	void exe(poller & mgr, pollee_t & obj, uint32_t evts)
	{
		if (m_cbinfo != nullptr) {
			(*m_cbinfo)(mgr, obj, evts);
		}
	}

private:
	evt_cb(const evt_cb & rhs) = delete;
	evt_cb & operator=(const evt_cb & rhs) = delete;
private:
	callable * m_cbinfo;
};

}

