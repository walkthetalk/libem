#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"
#include "zmsg_tense_test_result.hpp"
#include "zmsg_manual_discharge_times.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::fusion_splice_start> {
public:
	fs_pattern_t FSPattern;
	UINT32 FibreType;
	UINT32 FibreAlignment;
	BOOL XImageFocus;
	BOOL YImageFocus;
	UINT32 FibreShift;
	BOOL DischargeStrengthAdjustment;
	BOOL TensionSet;
	FLOAT CutAngleLimit;
	FLOAT LossLimit;
	FLOAT FibreAngleLimit;
	ULONG CleanDischargeTime;
	ULONG FibreIntervalSetup;
	LONG  FSPosSetup;
	UINT16 FibrePreFSStrength;
	ULONG FibrePreFSTime;
	ULONG FibreOverlapSetup;
	UINT16 Discharge1Strength;
	ULONG Discharge1Time;
	UINT16 Discharge2Strength;
	ULONG Discharge2LastTime;
	ULONG Discharge2StartTime;
	ULONG Discharge2StopTime;
	ULONG ExtraManualDischargeTime;

	BOOL ConeFS;
	ULONG ConeFSWaitTime;
	ULONG ConeFSSpeed;
	ULONG ConeFSStretchLength;

	loss_estimate_mode_t LossEstimationMode;
	FLOAT LeftFibreMFD;			/// unit: um
	FLOAT RightFibreMFD;			/// unit: um
	FLOAT LeastLoss;			/// unit: db
	FLOAT RateOfSyntropyBending;
	FLOAT RateOfReverseBending;
	FLOAT MFDMismatchCoefficient;		/// 0.0 ~ 1.0

	BOOL AutoStart;
	BOOL Stop1;
	BOOL Stop2;
	/*data dispaly*/
	BOOL CutAngle;
	BOOL OffsetData;
	/*omitted choice*/
	BOOL Cut;
	BOOL Loss;
	BOOL FibreCoreAngle;
	BOOL Bubble;
	BOOL Thick;
	BOOL Thin;
	/*dischargesupplement*/
	BOOL AirPressure;
	BOOL Temperature;
	/*fiber_image_display*/
	fs_display_mode_t ImgGap;
	fs_display_mode_t ImgStop1;
	fs_display_mode_t ImgAlign;
	fs_display_mode_t ImgStop2;
	fs_display_mode_t ImgDischarge;
	fs_display_mode_t ImgLossEstimation;
	/*else*/
	BOOL FibreAutoFeed;
	BOOL BadCutSurface;
	BOOL AutoAlignAfterStop;
	ULONG ManualDischargeTimes;
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
		ManualDischargeTimes)
};

template<>
struct zmsg<mid_t::fusion_splice_result> {
	fs_err_t code;

	zmsg<mid_t::fusion_splice_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	double core_diff_pre;		/// unit: um
	double cladding_diff_pre;	/// unit: um
	double vertex_intersect_angle;	/// unit: degree

	double pattern_compensate;	/// 0.0~1.0
	double loss_db;		/// unit: db

	bool is_tense_test;
	zmsg<mid_t::tense_test_result> z_tense_test_result;

	zmsg<mid_t::manual_discharge_times> z_manual_discharge_times;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		core_diff_pre,
		cladding_diff_pre,
		vertex_intersect_angle,
		pattern_compensate,
		loss_db,
		is_tense_test,
		z_tense_test_result,
		z_manual_discharge_times)
};

}
