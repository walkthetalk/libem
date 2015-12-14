#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::motor_test_start> {
public:
	fs_pattern_t FSPattern;
	uint32_t FiberType;
	uint32_t FiberAlignment;
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
	double LeftFiberMFD;
	double RightFiberMFD;
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

	/* motor test para */
	uint32_t MotorTestTimes;
	uint32_t ElectricArcTestTimes;
	uint32_t CleanArcRate;
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

		ManualDischargeTimes,

		MotorTestTimes,
		ElectricArcTestTimes,
		CleanArcRate)
};

struct statistic_data_t {
	int32_t cnt;
	double ref_v;
	double min_v;
	double max_v;
	double mid_v;
	double avg_v;
	std::string data;
public:
	bool empty()
	{
		return (cnt == 0);
	}
public:
	ZMSG_PU(cnt, ref_v, min_v, max_v, mid_v, avg_v, data)
};

template<>
struct zmsg<mid_t::motor_test_result> {
	fs_err_t code;

	zmsg<mid_t::motor_test_start> z_cfg;

	fiber_rec_info_t rec_info;

	img_defects_t defect_data;

	uint32_t motor_tested_times;
	uint32_t ele_arc_tested_times;

	/// \note following are error times
	uint32_t reset;
	uint32_t push;
	uint32_t calibrate;
	uint32_t ele_arc;
	uint32_t img;

	statistic_data_t nm_per_pixel_xz;
	statistic_data_t nm_per_pixel_yz;
	statistic_data_t nm_per_step_lz;
	statistic_data_t nm_per_step_rz;
	statistic_data_t nm_push_lz;
	statistic_data_t nm_push_rz;
	statistic_data_t arc_mag;
	statistic_data_t img_process;
public:
	ZMSG_PU(code,
		z_cfg,
		rec_info,
		defect_data,
		motor_tested_times,
		ele_arc_tested_times,
		reset, push, calibrate, ele_arc, img,
		nm_per_pixel_xz,
		nm_per_pixel_yz,
		nm_per_step_lz,
		nm_per_step_rz,
		nm_push_lz,
		nm_push_rz,
		arc_mag,
		img_process)
};

} /* namespace zmsg */

