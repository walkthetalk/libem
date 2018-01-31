#include <time.h>

#include <cerrno>

#include "zlog/zlog.hpp"

#include "exemodel/clk_info.hpp"

namespace exemodel {

timespec_t get_clk_res(int clk_id)
{
	struct timespec param;
	int ret = ::clock_getres(clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: get_res %d", errno);
	}
	return { param.tv_sec, param.tv_nsec, };
}

timespec_t get_clk_time(int clk_id)
{
	struct timespec param;
	int ret = ::clock_gettime(clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: get_time %d", errno);
	}
	return { param.tv_sec, param.tv_nsec, };
}

void set_clk_time(int clk_id, const timespec_t & tp)
{
	struct timespec param = { tp.tv_sec, tp.tv_nsec, };
	int ret = ::clock_settime(clk_id, &param);
	if (ret == -1) {
		zlog_err("timer: set_time %d", errno);
	}
}

}
