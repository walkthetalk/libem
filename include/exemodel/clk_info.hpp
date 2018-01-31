#pragma once

#include <time.h>

#include "exemodel/timespec.hpp"

namespace exemodel {

timespec_t get_clk_res(int clk_id);
timespec_t get_clk_time(int clk_id);
void set_clk_time(int clk_id, const timespec_t & tp);

template<int clk_id>
class clk_info {
public:
	static timespec_t get_res()
	{
		return get_clk_res(clk_id);
	}
	static timespec_t get_time()
	{
		return get_clk_time(clk_id);
	}
	static void set_time(const timespec_t & tp)
	{
		set_clk_time(clk_id, tp);
	}
};

typedef clk_info<CLOCK_REALTIME> rt_clk_info;
typedef clk_info<CLOCK_MONOTONIC> mono_clk_info;

}
