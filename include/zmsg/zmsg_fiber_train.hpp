#pragma once

#include "zmsg_types.hpp"
#include "zmsg_utils.hpp"

namespace zmsg {

template<>
struct msg<mid_t::fiber_train_init> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::fiber_train_start> : public fs_base_cfg_t {
public:
	fiber_t lft_type;
	fiber_t rt_type;
public:
	ZMSG_PU(*(fs_base_cfg_t *)this, lft_type, rt_type)
};

template<>
struct msg<mid_t::fiber_train_result> {
	fs_err_t code;

	msg<mid_t::fiber_train_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	uint32_t cnt;
	uint32_t cnt_limit;

	double lft_attr[2];
	double rt_attr[2];
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data,
		cnt, cnt_limit,
		lft_attr, rt_attr)
};

}
