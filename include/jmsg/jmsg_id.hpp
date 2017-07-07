#pragma once

#include "jmsg_types.hpp"

#define DCL_MSG(_x) msg_helper<mid_t::_x>::value_type

enum class mid_t : uint16_t {
	nil = 0,	/// occupy symbol, correspond to \em NULL.
	arc_revise = 1,
	countDown = 2,
	defect_detect_result = 3,
	discharge = 4,
	dischargeAdjustInit = 5,
	dischargeAdjustResult = 6,
	dischargeAdjustStart = 7,
	discharge_count = 8,
	dustCheckFullResult = 9,
	dustCheckFullStart = 10,
	dustCheckResult = 11,
	dustCheckStart = 12,
	fiberTrainInit = 13,
	fiberTrainResult = 14,
	fiberTrainStart = 15,
	fiber_reco_result = 16,
	fsCoverOpenned = 17,
	fs_cover_state = 18,
	fs_state = 19,
	fusionSpliceReset = 20,
	fusionSpliceResult = 21,
	fusionSpliceStart = 22,
	getFiberDefectInfo = 23,
	goOn = 24,
	heat_result = 25,
	heat_start = 26,
	heat_state = 27,
	image_move = 28,
	loss_estimating_result = 29,
	manual_discharge_counts = 30,
	motorTestResult = 31,
	motorTestStart = 32,
	motor_start = 33,
	motor_stop = 34,
	process_progress = 35,
	queryDevState = 36,
	queryWaveForm = 37,
	realtimeReviseResult = 38,
	realtimeReviseStart = 39,
	realtime_revise_result = 40,
	record_offset = 41,
	regularTestResult = 42,
	regularTestStart = 43,
	report_dev_state = 44,
	report_wave_form = 45,
	setFsDisplayModeExt = 46,
	set_fs_display_mode = 47,
	set_fs_display_mode_ext = 48,
	set_fs_display_zoom_ext = 49,
	set_lcd_brightness = 50,
	set_led = 51,
	set_window = 52,
	skip = 53,
	stabilizeElectrodeResult = 54,
	stabilizeElectrodeStart = 55,
	stop = 56,
	stopDischarge = 57,
	tenseTestResult = 58,
	update_led_brightness = 59,
	update_window_position = 60,
};
template<mid_t mid>
struct msg_helper final {};

template<>
struct msg_helper<mid_t::arc_revise> final {
	typedef struct arc_revise value_type;
};

template<>
struct msg_helper<mid_t::countDown> final {
	typedef struct count_down value_type;
};

template<>
struct msg_helper<mid_t::defect_detect_result> final {
	typedef struct defect_detect_result value_type;
};

template<>
struct msg_helper<mid_t::discharge> final {
	typedef struct discharge value_type;
};

template<>
struct msg_helper<mid_t::dischargeAdjustInit> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::dischargeAdjustResult> final {
	typedef struct discharge_adjust_result value_type;
};

template<>
struct msg_helper<mid_t::dischargeAdjustStart> final {
	typedef struct fs_da_cfg value_type;
};

template<>
struct msg_helper<mid_t::discharge_count> final {
	typedef struct discharge_count value_type;
};

template<>
struct msg_helper<mid_t::dustCheckFullResult> final {
	typedef struct dust_check_result value_type;
};

template<>
struct msg_helper<mid_t::dustCheckFullStart> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::dustCheckResult> final {
	typedef struct dust_check_result value_type;
};

template<>
struct msg_helper<mid_t::dustCheckStart> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::fiberTrainInit> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::fiberTrainResult> final {
	typedef struct fiber_train_result value_type;
};

template<>
struct msg_helper<mid_t::fiberTrainStart> final {
	typedef struct fs_ft_cfg value_type;
};

template<>
struct msg_helper<mid_t::fiber_reco_result> final {
	typedef struct fiber_reco_result value_type;
};

template<>
struct msg_helper<mid_t::fsCoverOpenned> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::fs_cover_state> final {
	typedef struct fs_cover_state value_type;
};

template<>
struct msg_helper<mid_t::fs_state> final {
	typedef struct fs_state value_type;
};

template<>
struct msg_helper<mid_t::fusionSpliceReset> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::fusionSpliceResult> final {
	typedef struct fusion_splice_result value_type;
};

template<>
struct msg_helper<mid_t::fusionSpliceStart> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::getFiberDefectInfo> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::goOn> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::heat_result> final {
	typedef struct heat_result value_type;
};

template<>
struct msg_helper<mid_t::heat_start> final {
	typedef struct heat_start value_type;
};

template<>
struct msg_helper<mid_t::heat_state> final {
	typedef struct heat_state value_type;
};

template<>
struct msg_helper<mid_t::image_move> final {
	typedef struct image_move value_type;
};

template<>
struct msg_helper<mid_t::loss_estimating_result> final {
	typedef struct loss_estimating_result value_type;
};

template<>
struct msg_helper<mid_t::manual_discharge_counts> final {
	typedef struct manual_discharge_counts value_type;
};

template<>
struct msg_helper<mid_t::motorTestResult> final {
	typedef struct motor_test_result value_type;
};

template<>
struct msg_helper<mid_t::motorTestStart> final {
	typedef struct fs_mt_cfg value_type;
};

template<>
struct msg_helper<mid_t::motor_start> final {
	typedef struct motor_start value_type;
};

template<>
struct msg_helper<mid_t::motor_stop> final {
	typedef struct motor_stop value_type;
};

template<>
struct msg_helper<mid_t::process_progress> final {
	typedef struct process_progress value_type;
};

template<>
struct msg_helper<mid_t::queryDevState> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::queryWaveForm> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::realtimeReviseResult> final {
	typedef struct realtime_revise_result value_type;
};

template<>
struct msg_helper<mid_t::realtimeReviseStart> final {
	typedef struct fs_rr_cfg value_type;
};

template<>
struct msg_helper<mid_t::realtime_revise_result> final {
	typedef struct realtime_revise_result value_type;
};

template<>
struct msg_helper<mid_t::record_offset> final {
	typedef struct record_offset value_type;
};

template<>
struct msg_helper<mid_t::regularTestResult> final {
	typedef struct regular_test_result value_type;
};

template<>
struct msg_helper<mid_t::regularTestStart> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::report_dev_state> final {
	typedef struct report_dev_state value_type;
};

template<>
struct msg_helper<mid_t::report_wave_form> final {
	typedef struct report_wave_form value_type;
};

template<>
struct msg_helper<mid_t::setFsDisplayModeExt> final {
	typedef struct set_fs_display_mode_ext value_type;
};

template<>
struct msg_helper<mid_t::set_fs_display_mode> final {
	typedef struct set_fs_display_mode value_type;
};

template<>
struct msg_helper<mid_t::set_fs_display_mode_ext> final {
	typedef struct set_fs_display_mode_ext value_type;
};

template<>
struct msg_helper<mid_t::set_fs_display_zoom_ext> final {
	typedef struct set_fs_display_zoom_ext value_type;
};

template<>
struct msg_helper<mid_t::set_lcd_brightness> final {
	typedef struct set_lcd_brightness value_type;
};

template<>
struct msg_helper<mid_t::set_led> final {
	typedef struct set_led value_type;
};

template<>
struct msg_helper<mid_t::set_window> final {
	typedef struct set_window value_type;
};

template<>
struct msg_helper<mid_t::skip> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::stabilizeElectrodeResult> final {
	typedef struct stabilize_electrode_result value_type;
};

template<>
struct msg_helper<mid_t::stabilizeElectrodeStart> final {
	typedef struct fs_se_cfg value_type;
};

template<>
struct msg_helper<mid_t::stop> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::stopDischarge> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::tenseTestResult> final {
	typedef struct tense_test_result value_type;
};

template<>
struct msg_helper<mid_t::update_led_brightness> final {
	typedef struct update_led_brightness value_type;
};

template<>
struct msg_helper<mid_t::update_window_position> final {
	typedef struct update_window_position value_type;
};

