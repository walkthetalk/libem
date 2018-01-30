#pragma once

#include "exemodel/timeree.hpp"
#include "exemodel/poller.hpp"

namespace exemodel {

class soft_pwm final : public poller {
public:
	soft_pwm() = default;
	~soft_pwm();

	int init();

	void bind(timeree::cb_t & f1, timeree::cb_t & f2)
	{
		m_t1.bind(f1);
		m_t2.bind(f2);
	}

	void bind4stop(std::function<void(void)> & cb)
	{
		m_f_stop = cb;
	}

	void save_spec(const timespec_t & t1, const timespec_t & t2);
	void start();
	void stop();
private:
	timeree m_t1;
	timeree m_t2;
	std::function<void(void)> m_f_stop;
};

} /* namespace exemodel */
