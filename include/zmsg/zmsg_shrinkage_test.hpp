#pragma once

#include "zmsg_types.hpp"
#include "zmsg_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::shrinkage_test_start> {
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
struct zmsg<mid_t::shrinkage_test_result> {
	fs_err_t code;

	zmsg<mid_t::shrinkage_test_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	zmsg<mid_t::record_off_set> z_record_offset;

	double real_gap;		/// unit: um
	double shrinkage;		/// unit: um
public:
	ZMSG_PU(code, z_cfg, rec_info, defect_data, z_record_offset, real_gap, shrinkage)
};

}
