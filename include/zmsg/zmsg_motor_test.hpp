#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::motor_test_start> {
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
	FLOAT LeftFibreMFD;
	FLOAT RightFibreMFD;
	FLOAT LeastLoss;
	FLOAT RateOfSyntropyBending;
	FLOAT RateOfReverseBending;
	FLOAT RateOfMFDDeviation;

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
	UINT32 ImgGap;
	UINT32 ImgStop1;
	UINT32 ImgAlign;
	UINT32 ImgStop2;
	UINT32 ImgDischarge;
	UINT32 ImgLossEstimation;
	/*else*/
	BOOL FibreAutoFeed;
	BOOL BadCutSurface;
	BOOL AutoAlignAfterStop;
	ULONG ManualDischargeTimes;

	/* motor test para */
	UINT32 MotorTestTimes;
	UINT32 ElectricArcTestTimes;
	UINT32 CleanArcRate;
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
		RateOfMFDDeviation,

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

		ManualDischargeTimes,

		MotorTestTimes,
		ElectricArcTestTimes,
		CleanArcRate)
};

template<>
struct zmsg<mid_t::motor_test_result> {
	mt_err_t code;

	zmsg<mid_t::motor_test_start> cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	UINT32 motor_test_times;
	UINT32 ele_arc_test_times;

	/// \note following are error times
	UINT32 reset;
	UINT32 push;
	UINT32 calibrate;
	UINT32 ele_arc;
	UINT32 img;
public:
	ZMSG_PU(code,
		cfg,
		rec_info,
		defect_data,
		motor_test_times,
		ele_arc_test_times,
		reset, push, calibrate, ele_arc, img)
};

} /* namespace zmsg */

