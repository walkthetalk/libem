
#include "soft_pwm.hpp"

int main(void)
{
	exemodel::soft_pwm a;
	exemodel::poller b;
	b.add(a);

	a.connect([](exemodel::poller &, uint64_t){
		auto const cur = exemodel::monotonic_timeree::info::get_time();
		std::cout << "0: " << cur.tv_sec << "  " << cur.tv_nsec << std::endl;
	}, [](exemodel::poller &, uint64_t) {
		auto const cur = exemodel::monotonic_timeree::info::get_time();
		std::cout << "1: " << cur.tv_sec << "  " << cur.tv_nsec << std::endl;
	});
	a.start({0, 500*1000*1000}, {0, 750*1000*1000});

	b.run();

	return 0;
}
