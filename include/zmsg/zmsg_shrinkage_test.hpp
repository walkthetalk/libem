#pragma once

#include "zmsg_types.hpp"
#include "zmsg_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::shrinkage_test_start> : public fs_base_cfg_t {
public:
	///
public:
	ZMSG_PU(*(fs_base_cfg_t *)this)
};

template<>
struct zmsg<mid_t::shrinkage_test_result> {
	fs_err_t code;

	zmsg<mid_t::shrinkage_test_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	zmsg<mid_t::record_off_set> z_record_offset;

	double real_gap;		/// unit: um
	double shrinkage;		/// unit: um
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data, z_record_offset, real_gap, shrinkage)
};

}

