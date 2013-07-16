#pragma once

#include <cstdint>

namespace zmsg {

/*
* \brief message identifier
*/
enum class mid_t : uint16_t {
	nil,	/// occupy symbol, correspond to \em NULL.
	test1,
	test2,

	fs_state,
	heat_state,

	alarm,

	fusion_splice_reset,

	heat_start,
	fusion_splice_start,
	discharge_adjust_start,
	regular_test_start,
	motor_test_start,
	dust_check_start,
	stabilize_electrode_start,
	go_on,
	stop,

	motor_start,
	motor_stop,
	discharge,
	image_move,
	set_fs_display_mode,
	set_led,

	defect_detect_result,
	fusion_splice_result,
	dust_check_result,
	motor_test_result,
	regular_test_result,
	discharge_adjust_result,
	stabilize_electrode_result,


	set_fs_spec,
	discharge_count,

	heat_result,

	query_dev_state,
	report_dev_state,

	query_wave_form,
	report_wave_form,
	/// \todo add new message id
	max,
};

template< mid_t mid >
struct zmsg;	/// not implemented.

/**
 * \brief zmsg pack/unpack macro
 */
#define ZMSG_PU(...) \
	template< typename _T > \
	void serialize(_T & o) const \
	{ \
		o.template operator()(__VA_ARGS__); \
	} \
	template< typename _T > \
	void serialize(_T & o) \
	{ \
		o.template operator()(__VA_ARGS__); \
	}

}/* namespace zmsg */

#define DCL_ZMSG(mid) zmsg::zmsg<zmsg::mid_t::mid>
