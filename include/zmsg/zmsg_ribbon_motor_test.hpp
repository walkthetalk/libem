#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::ribbon_motor_test_start> {
	public:
	fs_pattern_t FSPattern;
	uint32_t FiberType;
	bool XImageFocus;
	bool YImageFocus;
	bool DischargeStrengthAdjustment;
	bool TensionSet;
	double CutAngleLimit;
	double LossLimit;
	double FiberAngleLimit;
	uint32_t CleanDischargeTime;
	uint32_t FiberIntervalSetup;
	int32_t  FSPosSetup;
	double FiberPreFSStrength;
	uint32_t FiberPreFSTime;
	uint32_t FiberOverlapSetup;
	double DischargeStrength;
	uint32_t DischargeTime;
	uint32_t ExtraManualDischargeTime;
	
	bool ConeFS;
	uint32_t ConeFSWaitTime;
	double ConeFSSpeed;
	uint32_t ConeFSStretchLength;
	
	double LeftFSSpeed;
	double RightFSSpeed;
	
public:
	ZMSG_PU(
		FSPattern,
		FiberType,
		XImageFocus,
		YImageFocus,
		DischargeStrengthAdjustment,
		TensionSet,
		CutAngleLimit,
		LossLimit,
		FiberAngleLimit,
		CleanDischargeTime,
		FiberIntervalSetup,
		FSPosSetup,
		FiberPreFSStrength,
		FiberPreFSTime,
		FiberOverlapSetup,
		DischargeStrength,
		DischargeTime,
		ExtraManualDischargeTime,
		ConeFS,
		ConeFSWaitTime,
		ConeFSSpeed,
		ConeFSStretchLength,
		LeftFSSpeed,
		RightFSSpeed)
};

template<>
struct msg<mid_t::ribbon_motor_test_result> {
public:
	fs_err_t code;
	
	msg<mid_t::ribbon_motor_test_start> z_cfg;
	
	std::vector<fiber_rec_info_t> rec_info;
	
	std::vector<img_defects_t> defect_data;
	
	uint32_t motor_test_times;
	uint32_t ele_arc_test_times;
	
	uint32_t reset;
	uint32_t push;
	uint32_t ele_arc;
	uint32_t img;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		motor_test_times
		ele_arc_test_times,
		reset, push, ele_arc, img)
};

}