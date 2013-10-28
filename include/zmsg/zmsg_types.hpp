#pragma once

#include <cmath>
#include <array>
#include <vector>

#include "zmsg_cmm.hpp"

namespace zmsg {

/**
 * \brief typedefs
 */
typedef signed char CHAR;
typedef unsigned char UCHAR;

typedef signed short SHORT;
typedef unsigned short USHORT;

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;

typedef signed long LONG;
typedef unsigned long ULONG;

typedef float FLOAT;

typedef double DOUBLE;

typedef bool BOOL;

typedef void VOID;

} /* namespace zmsg */

/**
 * \brief bool image
 */
struct bool_img {
	uint16_t width;
	uint16_t height;
	std::vector<bool> data;
public:
	ZMSG_PU(width, height, data)
};

/**
 * \brief img fiber defect description
 */
typedef uint32_t ifd_t;

static constexpr ifd_t ifd_end_crude = 0x1;
static constexpr ifd_t ifd_horizontal_angle = 0x2;
static constexpr ifd_t ifd_vertical_angle = 0x4;

static constexpr ifd_t ifd_cant_identify = 0x80000000;

typedef struct ifd_line final {
	ifd_t  dbmp;

	/// \note all angles' unit are degree
	double h_angle;
	double v_angle;

	int32_t wrap_diameter;	/// unit: pixel

	ifd_line()
	: dbmp(0)
	, h_angle(0)
	, v_angle(0)
	, wrap_diameter(0)
	{
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
} ifd_line_t;

typedef struct img_defects final {
	ifd_line_t yzl;
	ifd_line_t yzr;
	ifd_line_t xzl;
	ifd_line_t xzr;

	/// the image contain missed corner info
	bool_img yz_img;
	bool_img xz_img;

	img_defects()
	: yzl(), yzr(), xzl(), xzr()
	{
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
	ZMSG_PU(yzl, yzr, xzl, xzr, yz_img, xz_img)
} img_defects_t;

/**
 * \biref fiber recognition infomation
 */
typedef struct final {
	uint32_t wrap_diameter;	/// unit: nm
	uint32_t core_diameter; /// unit: nm
public:
	ZMSG_PU(wrap_diameter, core_diameter)
} fiber_rec_info_t;

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
};

/**
 * \brief service heat state
 */
enum class svc_heat_state_t : uint16_t {
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
 * \brief fusion splice related error code
 */
enum class fs_err_t : uint8_t {
	success,
	cover_openned,
	no_fiber,
	fiber_defect,
	fiber_cross_over,
	fiber_off_center,
	img_brightness,
	abnormal_arc,
	tense_test_fail,
	fiber_broken,

	arc_time_zero,
};

/**
 * \brief regular test related error code
 */
enum class rt_err_t : uint8_t {
	success,
};

/**
 * \brief dust check related error code
 */
enum class dc_err_t : uint8_t {
	success,
	cover_openned,
	img_brightness,
};

/**
 * \brief motor test related error code
 */
enum class mt_err_t : uint8_t {
	success,
	cover_openned,
	no_fiber,
	fiber_defect,
	fiber_cross_over,
	fiber_off_center,
	img_brightness,
	abnormal_arc,
	push_timeout,
	calibrate_timeout,
	reset_timeout,

	arc_time_zero,
};

/**
 * \brief stablize electrode error code
 */
enum class se_err_t : uint8_t {
	success,
	cover_openned,
	no_fiber,
	fiber_defect,
	fiber_cross_over,
	fiber_off_center,
	img_brightness,
	abnormal_arc,

	arc_time_zero,
};

/**
 * \brief discharg adjust error code
 */
enum class da_err_t : uint8_t {
	success,
	cover_openned,
	no_fiber,
	fiber_defect,
	fiber_cross_over,
	fiber_off_center,
	img_brightness,
	abnormal_arc,

	revise1_mag,
	revise2_mag,

	arc_time_zero,
};

/**
 * \brief heat related error code
 */
enum class heat_err_t : uint8_t {
	success,
};

/**
 * \brief led id
 */
enum ledId_t : uint8_t {
	CMOS_X = 0x0,
	CMOS_Y,

	LED_NUM,
};

/**
 * \brief discharge_data_t, used for discharge revising
 */
typedef struct {
	struct {
		uint16_t x;
		double   y;		/// unit: um
	public:
		ZMSG_PU(x, y)
	} p[2];

	double temp;	/// unit: degree centigrade

	bool empty() const
	{
		return (p[0].x == p[1].x);
	}
public:
	ZMSG_PU(p, temp)
} discharge_data_t;

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
