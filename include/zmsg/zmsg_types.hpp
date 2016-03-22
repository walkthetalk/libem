#pragma once

#include <limits>
#include <cmath>
#include <array>
#include <vector>

#include "zmsg_cmm.hpp"

namespace zmsg {

/**
 * \brief typedefs
 */
#if 0
int{8|16|32}_t
uint{8|16|32}_t
float
double
#endif

} /* namespace zmsg */

/**
 * \brief img fiber defect description
 */
typedef uint32_t ifd_t;

static constexpr ifd_t ifd_end_crude = 0x1;
static constexpr ifd_t ifd_horizontal_angle = 0x2;
static constexpr ifd_t ifd_vertical_angle = 0x4;

static constexpr ifd_t ifd_cant_identify = 0x80000000;

static constexpr ifd_t ifd_all = std::numeric_limits<ifd_t>::max();

inline void ifd_clr(ifd_t & dst, const ifd_t src)
{
	dst &= ~src;
}

inline void ifd_set(ifd_t & dst, const ifd_t src)
{
	dst |= src;
}

struct ifd_line_t final {
	ifd_t  dbmp;

	/// \note all angles' unit are degree
	double h_angle;
	double v_angle;

	int32_t wrap_diameter;	/// unit: pixel

	ifd_line_t()
	: dbmp(0)
	, h_angle(0)
	, v_angle(0)
	, wrap_diameter(0)
	{
	}

	void init(void)
	{
		this->dbmp = 0;
		this->h_angle = 0;
		this->v_angle = 0;
		this->wrap_diameter = 0;
	}

	operator bool() const
	{
		return dbmp;
	}

	bool check(ifd_t msk) const
	{
		return (dbmp & msk);
	}
public:
	ZMSG_PU(dbmp, h_angle, v_angle, wrap_diameter)
};

struct img_defects_t final {
	ifd_line_t yzl;
	ifd_line_t yzr;
	ifd_line_t xzl;
	ifd_line_t xzr;

	double yz_hangle_intersect;
	double xz_hangle_intersect;

	/// the image contain missed corner info
	std::string yz_img;
	std::string xz_img;

	img_defects_t()
	: yzl(), yzr(), xzl(), xzr()
	, yz_hangle_intersect(0)
	, xz_hangle_intersect(0)
	, yz_img(), xz_img()
	{
	}

	void init(void)
	{
		this->yzl.init();
		this->yzr.init();
		this->xzl.init();
		this->xzr.init();
		this->yz_hangle_intersect = 0;
		this->xz_hangle_intersect = 0;
		this->yz_img = "";
		this->xz_img = "";
	}

	operator bool() const
	{
		return (yzl || yzr || xzl || xzr);
	}

	bool check(ifd_t msk) const
	{
		return (yzl.check(msk)
			|| yzr.check(msk)
			|| xzl.check(msk)
			|| xzr.check(msk));
	}

	double left_cut_angle(void) const
	{
		return std::max(yzl.v_angle, xzl.v_angle);
	}

	double right_cut_angle(void) const
	{
		return std::max(yzr.v_angle, xzr.v_angle);
	}
public:
	ZMSG_PU(yzl, yzr, xzl, xzr, yz_hangle_intersect, xz_hangle_intersect, yz_img, xz_img)
};

/**
 * \biref fiber recognition infomation
 */
struct fiber_rec_info_t final {
	uint32_t wrap_diameter;	/// unit: nm
	uint32_t core_diameter; /// unit: nm
public:
	ZMSG_PU(wrap_diameter, core_diameter)
};

/**
 * \brief service fs state
 * \note all states mean enter this state.
 */
enum class svc_fs_state_t : uint16_t {
	fs_reseting,
	fs_idle,
	fs_ready,

	fs_entering,
	fs_push1,
	fs_calibrating,
	fs_waiting,
	fs_clring,
	fs_focusing,
	fs_defect_detecting,
	fs_push2,
	fs_pause1,
	fs_precise_calibrating,
	fs_pause2,
	fs_pre_splice,
	fs_discharge1,
	fs_discharge2,
	fs_discharge_manual,
	fs_loss_estimating,
	fs_tension_testing,
	fs_finished,

	fs_wait_reset,
};

/**
 * \brief service heat state
 */
enum class svc_heat_state_t : uint16_t {
	heat_idle,
	heat_ready,
	heat_ascending,
	heat_stabling,
	heat_descending,
};

/**
 * \brief motor id
 */
enum motorId_t : uint8_t {
	LZ = 0,	// left z
	RZ,	// right z
	X,	// x
	Y,	// y

	NUM,	// total number
};

/**
 * \brief fusion splice display mode
 */
enum class fs_display_mode_t : uint8_t {
	X = 0x0,	/// only x
	Y,		/// only y
	TB,		/// top <-> bottom
	LR,		/// left <-> right
	NO,		/// no

	max,		/// number
};

/**
 * \brief fusion splicer related error code
 */
enum class fs_err_t : uint8_t {
	success,
	cover_openned,
	no_fiber,
	fiber_defect,
	fiber_cross_over,	/// sw or hw problem
	fiber_off_center,
	img_brightness,
	abnormal_arc,
	tense_test_fail,
	fiber_broken,
	quit_midway,
	push_timeout,
	calibrate_timeout,
	reset_timeout,

	arc_time_zero,
	ignore,

	revise1_mag,
	revise2_mag,

	focus_x,
	focus_y,

	img_process_error,
	system_error,

	fiber_offside,		/// user should replace fiber
	cmos_exposure,
	loss_estimate,
	arc_off_center,
	failed
};

/**
 * \brief led id
 */
enum ledId_t : uint8_t {
	CMOS_X = 0x0,
	CMOS_Y,

	LED_NUM,
};

struct mag2shrink_t {
	double x;	/// unit: volt
	double y;	/// unit: um
public:
	ZMSG_PU(x, y)
};

/**
 * \brief discharge_data_t, used for discharge revising
 */
struct discharge_data_t {
	std::array<mag2shrink_t, 2> p;

	double temp;		/// unit: degree centigrade
	double pressure;	/// unit: bar

	bool empty() const
	{
		return (p[0].x == p[1].x);
	}

	void clear()
	{
		p[0].x = p[1].x = 0;
		p[0].y = p[1].y = 0;
		temp = 0;
		pressure = 0;
	}
public:
	ZMSG_PU(p, temp, pressure)
};

/**
 * \brief rt_revise_data_t, used for real time discharge revising
 */
struct rt_revise_data_t {
	int32_t  rt_x_exposure;
	int32_t  rt_y_exposure;
	double   rt_revise_a3;
	double   rt_revise_a2;
	double   rt_revise_a1;
	double   rt_revise_a0;
	double   rt_offset_auto;
	double   rt_offset_cal;

	bool empty() const
	{
		return (rt_x_exposure <= 0 || rt_y_exposure <= 0);
	}

	void clear()
	{
		rt_x_exposure = 0;
		rt_y_exposure = 0;
		rt_revise_a3 = rt_revise_a2 = rt_revise_a1 = rt_revise_a0 = 0;
		rt_offset_auto = rt_offset_cal = 0;
	}
public:
	ZMSG_PU(rt_x_exposure,
		rt_y_exposure,
		rt_revise_a3,
		rt_revise_a2,
		rt_revise_a1,
		rt_revise_a0,
		rt_offset_auto,
		rt_offset_cal)
};

enum class fiber_t : uint8_t {
	sm = 0x0,
	ds,
	nz,
	mm,
	max,
};

/**
 * \brief fusion splice pattern
 */
enum class fs_pattern_t : uint8_t {
	automatic = 0x0,
	calibrate,
	normal,
	special,
};

/**
 * \brief loss estimate mode
 */
enum class loss_estimate_mode_t : uint8_t {
	off = 0x0,
	accurate,
	core,
	cladding,
};

/**
 * \brief shrinkabletube length
 */
enum class shrink_tube_t : uint8_t {
	len_20mm = 0x0,
	len_40mm,
	len_60mm,
};

/**
 * \brief fiber align method
 */
enum class align_method_t : uint8_t {
	fine = 0x0,
	clad,
	core,
	manual,
};

struct fs_base_cfg_t {
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
	bool ArcCorrectedValue;
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
	bool RealTimeRevise;
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
		ArcCorrectedValue,
		Cut,
		Loss,
		FiberCoreAngle,
		Bubble,
		Thick,
		Thin,
		AirPressure,
		Temperature,
		RealTimeRevise,
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
