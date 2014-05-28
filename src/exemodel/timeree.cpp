#include "zlog/zlog.hpp"

#include "exemodel/poll_tools.hpp"
#include "exemodel/timeree.hpp"

namespace exemodel {

static constexpr int32_t NSEC_PER_SEC = 1000000000L;
void set_normalized_timespec(timespec_t & ts,
						decltype(ts.tv_sec) sec,
						decltype(ts.tv_nsec) nsec)
{
	while (nsec >= NSEC_PER_SEC) {
		/*
		* The following asm() prevents the compiler from
		* optimising this loop into a modulo operation. See
		* also __iter_div_u64_rem() in include/linux/time.h
		*/
		asm("" : "+rm"(nsec));
		nsec -= NSEC_PER_SEC;
		++sec;
	}
	while (nsec < 0) {
		asm("" : "+rm"(nsec));
		nsec += NSEC_PER_SEC;
		--sec;
	}
	ts.tv_sec = sec;
	ts.tv_nsec = nsec;
}

}
