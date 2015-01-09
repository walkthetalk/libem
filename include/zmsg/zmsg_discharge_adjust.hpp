#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge_adjust_init> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::discharge_adjust_start> {
public:
	fs_pattern_t FSPattern;
	uint32_t FiberType;
	uint32_t FiberAlignment;
	bool XImageFocus;
	bool YImageFocus;
	bool FiberShift;
	bool DischargeStrengthAdjustment;
	bool TensionSet;
	float CutAngleLimit;
	float LossLimit;
	float FiberAngleLimit;
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

	bool ConeFS;
	uint32_t ConeFSWaitTime;
	double ConeFSSpeed;
	uint32_t ConeFSStretchLength;
	loss_estimate_mode_t LossEstimationMode;
	float LeftFiberMFD;
	float RightFiberMFD;
	float LeastLoss;
	float RateOfSyntropyBending;
	float RateOfReverseBending;
	float RateOfMFDDeviation;

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
	uint32_t ImgGap;
	uint32_t ImgStop1;
	uint32_t ImgAlign;
	uint32_t ImgStop2;
	uint32_t ImgDischarge;
	uint32_t ImgLossEstimation;
	/*else*/
	bool FiberAutoFeed;
	bool BadCutSurface;
	bool AutoAlignAfterStop;
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
		RateOfMFDDeviation,

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
		ManualDischargeTimes)
};

template<>
struct zmsg<mid_t::discharge_adjust_result> {
	fs_err_t code;

	zmsg<mid_t::discharge_adjust_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	discharge_data_t base;
	discharge_data_t revise;
	/// suggest value for next
	double strength1;
	double strength2;
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data, base, revise, strength1, strength2)
};

}