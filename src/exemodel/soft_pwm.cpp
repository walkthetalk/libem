
#include "exemodel/soft_pwm.hpp"

namespace exemodel {

soft_pwm::~soft_pwm()
{
	this->del(m_t2);
	this->del(m_t1);
}

int soft_pwm::init()
{
	int ret = 0;
	ret = poller::init();
	if (ret == -1)
		return ret;

	ret = m_t1.init(CLOCK_MONOTONIC);
	if (ret == -1)
		return ret;

	ret = m_t2.init(CLOCK_MONOTONIC);
	if (ret == -1)
		return ret;

	ret = this->add(m_t1);
	if (ret == -1)
		return ret;
	ret = this->add(m_t2);
	if (ret == -1)
		return ret;

	return ret;
}

void soft_pwm::save_spec(const timespec_t & t1, const timespec_t & t2)
{
	auto const cur = m_t1.get_time();

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
	m_t1.save_spec({ cycle, t1_val });
	m_t2.save_spec({ cycle, t2_val });
}

void soft_pwm::start()
{
	m_t1.start_abs();
	m_t2.start_abs();
}

void soft_pwm::stop()
{
	m_t1.stop();
	m_t2.stop();
	if (m_f_stop) {
		m_f_stop();
	}
}

}
