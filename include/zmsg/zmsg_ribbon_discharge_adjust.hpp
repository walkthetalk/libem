#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::ribbon_discharge_adjust_start> {
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
struct zmsg<mid_t::ribbon_discharge_adjust_result> {
public:
	fs_err_t code;
	
	zsmg<mid_t::ribbon_discharge_adjust_start> z_cfg;
	
	std::vector<fiber_rec_info_t> rec_info;
	
	std::vector<img_defects_t> defect_data;
	
	discharge_data_t base;
	discharge_data_t revise;
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data, base, revise)
};

}
