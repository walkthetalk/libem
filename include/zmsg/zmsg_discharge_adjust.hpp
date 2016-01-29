#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge_adjust_init> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::discharge_adjust_start> : public fs_base_cfg_t {
public:
public:
	ZMSG_PU(*(fs_base_cfg_t *)this)
};

template<>
struct zmsg<mid_t::discharge_adjust_result> {
	fs_err_t code;

	zmsg<mid_t::discharge_adjust_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	discharge_data_t base;
	discharge_data_t revise;
	/// suggest value for next
	double suggest1;
	double suggest2;
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data, base, revise, suggest1, suggest2)
};

}