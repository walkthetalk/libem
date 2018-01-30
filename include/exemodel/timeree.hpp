#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 * 		linxiang<linxiang2019@163.com>
 */
#include <functional>

#include "exemodel/pollee.hpp"

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
bool ts_equal(const timespec_t &a, const timespec_t &b);
/*
 * lhs < rhs:  return <0
 * lhs == rhs: return 0
 * lhs > rhs:  return >0
 */
int ts_compare(const timespec_t &lhs, const timespec_t &rhs);
timespec_t ts_add(const timespec_t lhs, const timespec_t rhs);
timespec_t ts_sub(const timespec_t lhs, const timespec_t rhs);

class timeree : public pollee {
public:
	typedef std::function<int (poller & mgr, uint64_t cnt)> cb_t;
public:
	explicit timeree() = default;
	virtual ~timeree() = default;

public:
	int init(int clk_id);

	void save_spec(const itimerspec_t & new_value);
	void save_spec(int laterms, int firstms);

	void start(void);
	void start_abs(void);
	void stop(void);

	timespec_t get_res(void);
	timespec_t get_time(void);
	void set_time(const timespec_t & tp);

	void bind(cb_t & cb)
	{
		m_processor = cb;
	}
	void unbind()
	{
		m_processor = nullptr;
	}
public:
	virtual int dispose(poller & mgr, uint32_t);
private:
	timeree(const timeree & rhs) = delete;
	timeree & operator = (const timeree & rhs ) = delete;
private:
	itimerspec_t m_spec;
	int m_clk_id;
	cb_t m_processor;
};

}
