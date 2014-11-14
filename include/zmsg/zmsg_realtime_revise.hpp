#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"
#include "zmsg_tense_test_result.hpp"
#include "zmsg_manual_discharge_counts.hpp"
#include "zmsg_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::realtime_revise_start> {
public:
	fs_pattern_t FSPattern;
	fiber_t FiberType;
	align_method_t FiberAlignment;
	bool XImageFocus;
	bool YImageFocus;
	bool FiberShift;
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
	double Discharge1Strength;
	uint32_t Discharge1Time;
	double Discharge2Strength;
	uint32_t Discharge2LastTime;
	uint32_t Discharge2StartTime;
	uint32_t Discharge2StopTime;
	uint32_t ExtraManualDischargeTime;

	double LeftFSSpeed;	/// 0.0~1.0
	double RightFSSpeed;	/// 0.0~1.0

	bool ConeFS;
	uint32_t ConeFSWaitTime;
	double ConeFSSpeed;
	uint32_t ConeFSStretchLength;

	loss_estimate_mode_t LossEstimationMode;
	double LeftFiberMFD;			/// unit: um
	double RightFiberMFD;			/// unit: um
	double LeastLoss;			/// unit: db
	double RateOfSyntropyBending;
	double RateOfReverseBending;
	double MFDMismatchCoefficient;		/// 0.0 ~ 1.0

	bool AutoStart;
	bool Stop1;
	bool Stop2;
	/*data dispaly*/
	bool CutAngle;
	bool OffsetData;
	/*omitted choice*/
	bool Cut;
	bool Loss;
	bool FiberCoreAngle;
	bool Bubble;
	bool Thick;
	bool Thin;
	/*dischargesupplement*/
	bool AirPressure;
	bool Temperature;
	/*fiber_image_display*/
	fs_display_mode_t ImgGap;
	fs_display_mode_t ImgStop1;
	fs_display_mode_t ImgAlign;
	fs_display_mode_t ImgStop2;
	fs_display_mode_t ImgDischarge;
	fs_display_mode_t ImgLossEstimation;
	/*else*/
	bool FiberAutoFeed;
	bool BadCutSurface;
	bool AutoAlignAfterStop;
	bool CleanDischargeTwice;
	uint32_t ManualDischargeTimes;
public:
	ZMSG_PU(
		FSPattern,
		FiberType,
		FiberAlignment,
		XImageFocus,
		YImageFocus,
		FiberShift,
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
		Discharge1Strength,
		Discharge1Time,
		Discharge2Strength,
		Discharge2LastTime,
		Discharge2StartTime,
		Discharge2StopTime,
		ExtraManualDischargeTime,

		LeftFSSpeed,
		RightFSSpeed,

		ConeFS,
		ConeFSWaitTime,
		ConeFSSpeed,
		ConeFSStretchLength,
		LossEstimationMode,
		LeftFiberMFD,
		RightFiberMFD,
		LeastLoss,
		RateOfSyntropyBending,
		RateOfReverseBending,
		MFDMismatchCoefficient,

		AutoStart,
		Stop1,
		Stop2,
		CutAngle,
		OffsetData,
		Cut,
		Loss,
		FiberCoreAngle,
		Bubble,
		Thick,
		Thin,
		AirPressure,
		Temperature,
		ImgGap,
		ImgStop1,
		ImgAlign,
		ImgStop2,
		ImgDischarge,
		ImgLossEstimation,
		FiberAutoFeed,
		BadCutSurface,
		AutoAlignAfterStop,
		CleanDischargeTwice,
		ManualDischargeTimes)
};

template<>
struct zmsg<mid_t::realtime_revise_update> {
	fs_pattern_t FSPattern;
	fiber_t FiberType;
	double offset;
public:
	ZMSG_PU(
		FSPattern,
		FiberType,
		offset)
};

template<>
struct zmsg<mid_t::realtime_revise_result> {
	fs_err_t code;

	zmsg<mid_t::realtime_revise_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	zmsg<mid_t::record_off_set> z_record_off_set;

	double pattern_compensate;	/// 0.0~1.0
	double loss_db;		/// unit: db

	zmsg<mid_t::tense_test_result> z_tense_test_result;
	zmsg<mid_t::manual_discharge_counts> z_manual_discharge_counts;
	rt_revise_data_t RealtimeReviseData;
public:
	ZMSG_PU(
		code,
		z_cfg,
		rec_info,
		defect_data,
		z_record_off_set,
		pattern_compensate,
		loss_db,
		z_tense_test_result,
		z_manual_discharge_counts,
		RealtimeReviseData)
};

}
