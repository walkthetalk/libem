#pragma once

#include "exemodel/timeree.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

class soft_pwm : public poller {
	typedef monotonic_timeree __tmr_t;
public:
	soft_pwm()
	: m_t1()
	, m_t2()
	, m_f_stop(nullptr)
	{
		this->add(m_t1);
		this->add(m_t2);
	}

	void connect(__tmr_t::cb_func_t const & f1,
		__tmr_t::cb_func_t const & f2,
		std::function<void(void)> const & f_stop = nullptr)
	{
		m_t1.connect(f1);
		m_t2.connect(f2);
		m_f_stop = f_stop;
	}

	void start(const timespec_t & t1, const timespec_t & t2)
	{
		auto const cur = __tmr_t::info::get_time();

		timespec_t cycle = {
			t1.tv_sec + t2.tv_sec,
			t1.tv_nsec + t2.tv_nsec,
		};
		if (cycle.tv_nsec >= 1000*1000*1000) {
			++cycle.tv_sec;
			cycle.tv_nsec -= 1000*1000*1000;
		}

		timespec_t t1_val = {cur.tv_sec, cur.tv_nsec};
		timespec_t t2_val = {
			cur.tv_sec + t1.tv_sec,
			cur.tv_nsec + t1.tv_nsec,
		};
		if (t2_val.tv_nsec >= 1000*1000*1000) {
			++t2_val.tv_sec;
			t2_val.tv_nsec -= 1000*1000*1000;
		}
		m_t1.start_abs({ cycle, t1_val });
		m_t2.start_abs({ cycle, t2_val });
	}

	void stop()
	{
		m_t1.stop();
		m_t2.stop();
		if (m_f_stop) {
			m_f_stop();
		}
	}
private:
	__tmr_t m_t1;
	__tmr_t m_t2;
	std::function<void(void)> m_f_stop;
};

} /* namespace exemodel */
