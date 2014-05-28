#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"
#include "zmsg_tense_test_result.hpp"
#include "zmsg_manual_discharge_counts.hpp"
#include "zmsg_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::fusion_splice_start> {
public:
	fs_pattern_t FSPattern;
	uint32_t FibreType;
	align_method_t FibreAlignment;
	bool XImageFocus;
	bool YImageFocus;
	bool FibreShift;
	bool DischargeStrengthAdjustment;
	bool TensionSet;
	double CutAngleLimit;
	double LossLimit;
	double FibreAngleLimit;
	uint32_t CleanDischargeTime;
	uint32_t FibreIntervalSetup;
	int32_t  FSPosSetup;
	double FibrePreFSStrength;
	uint32_t FibrePreFSTime;
	uint32_t FibreOverlapSetup;
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
	double LeftFibreMFD;			/// unit: um
	double RightFibreMFD;			/// unit: um
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
	bool FibreCoreAngle;
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
	bool FibreAutoFeed;
	bool BadCutSurface;
	bool AutoAlignAfterStop;
	bool CleanDischargeTwice;
	uint32_t ManualDischargeTimes;
public:
	ZMSG_PU(
		FSPattern,
		FibreType,
		FibreAlignment,
		XImageFocus,
		YImageFocus,
		FibreShift,
		DischargeStrengthAdjustment,
		TensionSet,
		CutAngleLimit,
		LossLimit,
		FibreAngleLimit,
		CleanDischargeTime,
		FibreIntervalSetup,
		FSPosSetup,
		FibrePreFSStrength,
		FibrePreFSTime,
		FibreOverlapSetup,
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
		LeftFibreMFD,
		RightFibreMFD,
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
		FibreCoreAngle,
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
		FibreAutoFeed,
		BadCutSurface,
		AutoAlignAfterStop,
		CleanDischargeTwice,
		ManualDischargeTimes)
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
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		z_record_off_set,
		pattern_compensate,
		loss_db,
		z_tense_test_result,
		z_manual_discharge_counts)
};

}
