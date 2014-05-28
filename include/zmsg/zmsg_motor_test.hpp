#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::motor_test_start> {
public:
	fs_pattern_t FSPattern;
	uint32_t FibreType;
	uint32_t FibreAlignment;
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

	bool ConeFS;
	uint32_t ConeFSWaitTime;
	double ConeFSSpeed;
	uint32_t ConeFSStretchLength;
	loss_estimate_mode_t LossEstimationMode;
	double LeftFibreMFD;
	double RightFibreMFD;
	double LeastLoss;
	double RateOfSyntropyBending;
	double RateOfReverseBending;
	double RateOfMFDDeviation;

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
	uint32_t ImgGap;
	uint32_t ImgStop1;
	uint32_t ImgAlign;
	uint32_t ImgStop2;
	uint32_t ImgDischarge;
	uint32_t ImgLossEstimation;
	/*else*/
	bool FibreAutoFeed;
	bool BadCutSurface;
	bool AutoAlignAfterStop;
	uint32_t ManualDischargeTimes;

	/* motor test para */
	uint32_t MotorTestTimes;
	uint32_t ElectricArcTestTimes;
	uint32_t CleanArcRate;
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
	fs_err_t code;

	zmsg<mid_t::motor_test_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	uint32_t motor_test_times;
	uint32_t ele_arc_test_times;

	/// \note following are error times
	uint32_t reset;
	uint32_t push;
	uint32_t calibrate;
	uint32_t ele_arc;
	uint32_t img;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		motor_test_times,
		ele_arc_test_times,
		reset, push, calibrate, ele_arc, img)
};

} /* namespace zmsg */

