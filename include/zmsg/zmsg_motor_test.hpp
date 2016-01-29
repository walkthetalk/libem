#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::motor_test_start> : public fs_base_cfg_t {
public:
	/* motor test para */
	uint32_t MotorTestTimes;
	uint32_t ElectricArcTestTimes;
	uint32_t CleanArcRate;
public:
	ZMSG_PU(*(fs_base_cfg_t *)this,
		MotorTestTimes,
		ElectricArcTestTimes,
		CleanArcRate)
};

struct statistic_data_t {
	int32_t cnt;
	double ref_v;
	double min_v;
	double max_v;
	double mid_v;
	double avg_v;
	std::string data;
public:
	bool empty()
	{
		return (cnt == 0);
	}
public:
	ZMSG_PU(cnt, ref_v, min_v, max_v, mid_v, avg_v, data)
};

template<>
struct zmsg<mid_t::motor_test_result> {
	fs_err_t code;

	zmsg<mid_t::motor_test_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	uint32_t motor_tested_times;
	uint32_t ele_arc_tested_times;

	/// \note following are error times
	uint32_t reset;
	uint32_t push;
	uint32_t calibrate;
	uint32_t ele_arc;
	uint32_t img;

	statistic_data_t nm_per_pixel_xz;
	statistic_data_t nm_per_pixel_yz;
	statistic_data_t nm_per_step_lz;
	statistic_data_t nm_per_step_rz;
	statistic_data_t nm_push_lz;
	statistic_data_t nm_push_rz;
	statistic_data_t arc_mag;
	statistic_data_t img_process;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		motor_tested_times,
		ele_arc_tested_times,
		reset, push, calibrate, ele_arc, img,
		nm_per_pixel_xz,
		nm_per_pixel_yz,
		nm_per_step_lz,
		nm_per_step_rz,
		nm_push_lz,
		nm_push_rz,
		arc_mag,
		img_process)
};

} /* namespace zmsg */

