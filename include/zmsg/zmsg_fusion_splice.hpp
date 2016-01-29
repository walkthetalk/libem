#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"
#include "zmsg_tense_test_result.hpp"
#include "zmsg_manual_discharge_counts.hpp"
#include "zmsg_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::fusion_splice_start> : public fs_base_cfg_t {
public:
	///
public:
	ZMSG_PU(*(fs_base_cfg_t *)this)
};

template<>
struct zmsg<mid_t::fusion_splice_result> {
	fs_err_t code;

	zmsg<mid_t::fusion_splice_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	zmsg<mid_t::record_off_set> z_record_off_set;

	double pattern_compensate;	/// 0.0~1.0
	double loss_db;		/// unit: db

	zmsg<mid_t::tense_test_result> z_tense_test_result;

	zmsg<mid_t::manual_discharge_counts> z_manual_discharge_counts;

	std::string fs_done_x_img;
	std::string fs_done_y_img;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		z_record_off_set,
		pattern_compensate,
		loss_db,
		z_tense_test_result,
		z_manual_discharge_counts,
		fs_done_x_img,
		fs_done_y_img)
};

}
