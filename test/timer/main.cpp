
#include "exemodel/poller.hpp"
#include "exemodel/timeree.hpp"
#include "exemodel/poll_tools.hpp"

#include <time.h>

using namespace exemodel;

class swpwm : public poller {
public:
	swpwm(void)
	: poller()
	, m_t1({ {0, 100000000}, {0,100000000} })
	, m_t2({ {0, 100000000}, {0,100000000} })
	, m_time_res(decltype(m_t1)::info::get_res())
	, m_idx(0)
	{
		std::cout << "timer res:  " << m_time_res.tv_sec << ":" << m_time_res.tv_nsec << std::endl;

		m_t1.connect([this](exemodel::poller&, uint64_t) {
			this->get_time();
		});
		m_t2.connect([this](exemodel::poller&, uint64_t) {
			this->get_time();
		});

		this->add(m_t1);
		this->add(m_t2);

		m_t1.start();
		//m_t2.start();
	}
	void get_time(void)
	{
		if (m_idx < 100) {
			std::cout << m_idx << std::endl;
				struct timespec & ts = m_time[m_idx];
				ts = decltype(m_t1)::info::get_time();
				std::cout << "timer 1:  " << ts.tv_sec << ":" << ts.tv_nsec << std::endl;
			++m_idx;
		}
		else {
			for (int i = 0; i < 100; ++i) {
				struct timespec & ts = m_time[i];
				std::cout << "timer 1:  " << ts.tv_sec << ":" << ts.tv_nsec << std::endl;
			}
			exit(1);
		}
	}
private:
	monotonic_timeree m_t1;
	monotonic_timeree m_t2;
	timespec_t m_time_res;
	int m_idx;
	timespec_t m_time[100];
};

int main(void)
{
	swpwm a;
	poller b;
	b.add(a);
	b.run();

	return 0;
}
