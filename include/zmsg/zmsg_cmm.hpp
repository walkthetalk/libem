#pragma once

#include <cstdint>

namespace zmsg {

/**
 * \brief message identifier
 */
enum class mid_t : uint16_t {
	nil,	/// occupy symbol, correspond to \em NULL.
	test1,
	test2,

	fs_state,
	heat_state,			/// 5

	alarm,

	fusion_splice_reset,

	heat_start,
	fusion_splice_start,
	discharge_adjust_start,		/// 10
	regular_test_start,
	motor_test_start,
	dust_check_start,
	stabilize_electrode_start,
	go_on,				/// 15
	stop,

	motor_start,
	motor_stop,
	discharge,
	image_move,			/// 20
	set_fs_display_mode,
	set_led,

	defect_detect_result,
	fusion_splice_result,
	dust_check_result,		/// 25
	motor_test_result,
	regular_test_result,
	discharge_adjust_result,
	stabilize_electrode_result,


	set_fs_spec,			/// 30
	discharge_count,

	heat_result,

	query_dev_state,
	report_dev_state,

	query_wave_form,		/// 35
	report_wave_form,

	update_led_brightness,
	update_window_position,

	set_lcd_brightness,
	loss_estimating_result,		/// 40
	manual_focal_distance,
	update_cmos_focal_distance,

	get_fiber_defect_info,
	record_off_set,
	manual_discharge_counts,	/// 45
	tense_test_result,

	skip,
	fs_cover_openned,
	stop_discharge,

	full_dust_check_start,		/// 50
	full_dust_check_result,

	shrinkage_test_start,
	shrinkage_test_result,

	process_progress,

	/// \todo ribbon message
	ribbon_get_defect_info,
	ribbon_defect_detect_result,

	ribbon_fusion_splicer_start,
	ribbon_fusion_splicer_result,

	ribbon_discharge_adjust_start,
	ribbon_discharge_adjust_result,

	ribbon_set_fs_spec,

	ribbon_loss_estimating_result,

	ribbon_motor_test_start,
	ribbon_motor_test_result,

	ribbon_record_off_set,

	ribbon_shrinkage_test_start,
	ribbon_shrinkage_test_result,

	realtime_revise_start,
	realtime_revise_update,
	realtime_revise_result,

	fs_cover_state,
	arc_revise,

	discharge_adjust_init,
	dust_check1_start,

	/// \todo add new message id above this line
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
