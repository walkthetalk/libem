#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge_adjust_start> {
public:
	UINT32 FSPattern;
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
	UINT32 LossEstimationMode;
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
		ManualDischargeTimes)
};

template<>
struct zmsg<mid_t::discharge_adjust_result> {
	discharge_adjust_err_t code;

	uint16_t i[2];
	double   t[2];
public:
	ZMSG_PU(code, i, t)
};

}

