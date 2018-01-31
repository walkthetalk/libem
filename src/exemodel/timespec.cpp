#include "exemodel/timespec.hpp"

namespace exemodel {

static constexpr long NSEC_PER_SEC = 1000000000L;
static timespec_t set_normalized_timespec(
	long sec,
	long nsec)
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

	return { sec, nsec };
}

timespec_t ms_to_timespec(int ms)
{
	return {
		(ms/1000),
		(ms%1000) * 1000 * 1000
	};
}

bool timespec_equal(const timespec_t &a,
		    const timespec_t &b)
{
	return (a.tv_sec == b.tv_sec) && (a.tv_nsec == b.tv_nsec);
}

int timespec_compare(const timespec_t &lhs,
		     const timespec_t &rhs)
{
	if (lhs.tv_sec < rhs.tv_sec) {
		return -1;
	}
	if (lhs.tv_sec > rhs.tv_sec) {
		return 1;
	}
	if (lhs.tv_nsec < rhs.tv_nsec) {
		return -1;
	}
	if (lhs.tv_nsec > rhs.tv_nsec) {
		return 1;
	}
	return 0;
}

timespec_t timespec_add(const timespec_t lhs,
			const timespec_t rhs)
{
	return set_normalized_timespec(
			lhs.tv_sec + rhs.tv_sec,
			lhs.tv_nsec + rhs.tv_nsec);
}

timespec_t timespec_sub(const timespec_t lhs,
			const timespec_t rhs)
{
	return set_normalized_timespec(
			lhs.tv_sec - rhs.tv_sec,
			lhs.tv_nsec - rhs.tv_nsec);
}

}
