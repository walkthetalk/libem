#pragma once
/**
 * \file	exemodel/timeree.hpp
 * \author	justgaoyuan<gao_yuangy@126.com>
 * 		linxiang<linxiang2019@163.com>
 */
#include <functional>

#include "exemodel/pollee.hpp"
#include "exemodel/timespec.hpp"

namespace exemodel {

class timeree : public pollee {
public:
	typedef std::function<int (poller & mgr, uint64_t cnt)> cb_t;
public:
	explicit timeree() = default;
	virtual ~timeree() = default;

public:
	int init_realtime();
	int init_mono();

	void save_spec(const itimerspec_t & new_value);
	void save_spec(int laterms, int firstms);

	void start(void);
	void start_abs(void);
	void stop(void);

	timespec_t get_res(void) const;
	timespec_t get_time(void) const;
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
	int __init(int clk_id);
private:
	itimerspec_t m_spec;
	int m_clk_id;
	cb_t m_processor;
};

}
