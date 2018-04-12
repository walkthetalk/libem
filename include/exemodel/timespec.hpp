#pragma once

namespace exemodel {

typedef struct {
	long tv_sec;
	long tv_nsec;
} timespec_t;

typedef struct {
	timespec_t it_interval;
	timespec_t it_value;
} itimerspec_t;

timespec_t ms_to_timespec(int ms);
bool timespec_equal(const timespec_t &a, const timespec_t &b);
/*
 * lhs < rhs:  return <0
 * lhs == rhs: return 0
 * lhs > rhs:  return >0
 */
int timespec_compare(const timespec_t &lhs, const timespec_t &rhs);
timespec_t timespec_add(const timespec_t lhs, const timespec_t rhs);
timespec_t timespec_sub(const timespec_t lhs, const timespec_t rhs);

}
