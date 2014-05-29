#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"
#include "zmsg_tense_test_result"
#include "zmsg_manual_discharge_counts.hpp"
#include "zmsg_ribbon_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::ribbon_fusion_splicer_start> {
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
struct zmsg<mid_t::ribbon_fusion_splicer_result> {
	fs_err_t code;
	
	zmsg<mid_t::ribbon_fusion_splicer_start> z_cfg;
	
	std::vector<fiber_rec_info_t> rec_info;
	
	std::vector<img_defects_t> defect_data;
	
	zmsg<mid_t::ribbon_record_off_set> z_record_set;
	
	std::vector<double> pattern_cpmpensate;
	std::vector<double> loss_db;
	
	zmsg<mid_t::tense_test_result> z_tense_test_result;
	
	zmsg<mid_t::manual_discharge_counts> z_manual_test_discharge_counts;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		z_record_set,
		pattern_cpmpensate,
		loss_db,
		z_tense_test_result,
		z_manual_test_discharge_counts)
};

}