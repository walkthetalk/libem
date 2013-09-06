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

typedef ULONG BOOL;

typedef void VOID;

} /* namespace zmsg */

/**
 * \brief img fiber defect description
 */
typedef uint32_t ifd_t;

static constexpr ifd_t ifd_spot = 0x1;
static constexpr ifd_t ifd_break = 0x2;
static constexpr ifd_t ifd_bottom_sag = 0x4;
static constexpr ifd_t ifd_bottom_crest = 0x8;
static constexpr ifd_t ifd_bottom_corner = 0x10;
static constexpr ifd_t ifd_top_sag = 0x20;
static constexpr ifd_t ifd_top_crest = 0x40;
static constexpr ifd_t ifd_top_corner = 0x80;
static constexpr ifd_t ifd_end_crude = 0x100;
static constexpr ifd_t ifd_horizontal_angle = 0x200;
static constexpr ifd_t ifd_vertical_angle = 0x400;

static constexpr ifd_t ifd_cant_identify = 0x80000000;

typedef struct ifd_line final {
	ifd_t core;
	ifd_t wrap;

	double h_angle;
	double v_angle;		/// \note must greater than zero

	ifd_line()
	: core(0)
	, wrap(0)
	, h_angle(0)
	, v_angle(0)
	{
	}

	operator bool() const
	{
		return (core || wrap);
	}

	bool check(ifd_t msk) const
	{
		return ((core & msk)
			|| (wrap & msk));
	}
public:
	ZMSG_PU(core, wrap, h_angle, v_angle)
} ifd_line_t;

typedef struct img_defects final {
	ifd_line_t yzl;
	ifd_line_t yzr;
	ifd_line_t xzl;
	ifd_line_t xzr;

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
	ZMSG_PU(yzl, yzr, xzl, xzr)
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
	img_brightness,
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
		double   y;
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
enum fs_pattern_t : uint8_t {
	FS_PATTERN_AUTO = 0x0,
	FS_PATTERN_CALIBRATE,
	FS_PATTERN_NORMAL,
	FS_PATTERN_SPECIAL,
};
