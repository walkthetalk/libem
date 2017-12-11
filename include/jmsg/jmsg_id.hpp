#pragma once

#include "jmsg_types.hpp"

#define DCL_MSG(_x) msg_helper<mid_t::_x>::value_type

enum class mid_t : uint16_t {
	nil = 0,	/// occupy symbol, correspond to \em NULL.
	arcTestResult = 1,
	arc_revise = 2,
	bat_state = 3,
	beep = 4,
	countDown = 5,
	defect_detect_result = 6,
	discharge = 7,
	dischargeAdjustInit = 8,
	dischargeAdjustResult = 9,
	dischargeAdjustStart = 10,
	discharge_count = 11,
	dustCheckFullResult = 12,
	dustCheckFullStart = 13,
	dustCheckResult = 14,
	dustCheckStart = 15,
	fiberTrainInit = 16,
	fiberTrainResult = 17,
	fiberTrainStart = 18,
	fiber_reco_result = 19,
	fsCoverOpenned = 20,
	fs_cover_state = 21,
	fs_reset = 22,
	fs_state = 23,
	fspre_state = 24,
	fusionSpliceResult = 25,
	fusionSpliceStart = 26,
	getFiberDefectInfo = 27,
	goOn = 28,
	heatTestResult = 29,
	heat_result = 30,
	heat_start = 31,
	heat_state = 32,
	image_move = 33,
	lcd_power_ctl = 34,
	loss_estimating_result = 35,
	manual_arc_result = 36,
	motorTestResult = 37,
	motorTestStart = 38,
	motor_start = 39,
	motor_stop = 40,
	process_progress = 41,
	queryBatState = 42,
	queryDevState = 43,
	queryWaveForm = 44,
	realtimeReviseResult = 45,
	realtimeReviseStart = 46,
	regularTestResult = 47,
	regularTestStart = 48,
	report_dev_state = 49,
	report_wave_form = 50,
	setFsDisplayModeExt = 51,
	set_fs_display_mode = 52,
	set_fs_display_zoom_ext = 53,
	set_lcd_brightness = 54,
	set_led = 55,
	shutdown = 56,
	skip = 57,
	stabilizeElectrodeResult = 58,
	stabilizeElectrodeStart = 59,
	startDustCheckFull = 60,
	stop = 61,
	stopDischarge = 62,
	tenseTestResult = 63,
	update_led_brightness = 64,
	update_window_position = 65,
};
template<mid_t mid>
struct msg_helper final {};

template<>
struct msg_helper<mid_t::arcTestResult> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::arc_revise> final {
	typedef struct arc_revise value_type;
};

template<>
struct msg_helper<mid_t::bat_state> final {
	typedef struct bat_state value_type;
};

template<>
struct msg_helper<mid_t::beep> final {
	typedef struct beep value_type;
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
	typedef struct dustCheckFullStart value_type;
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
struct msg_helper<mid_t::fs_reset> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::fs_state> final {
	typedef struct fs_state value_type;
};

template<>
struct msg_helper<mid_t::fspre_state> final {
	typedef struct fspre_state value_type;
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
struct msg_helper<mid_t::heatTestResult> final {
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
struct msg_helper<mid_t::lcd_power_ctl> final {
	typedef struct lcd_power_ctl value_type;
};

template<>
struct msg_helper<mid_t::loss_estimating_result> final {
	typedef struct loss_estimating_result value_type;
};

template<>
struct msg_helper<mid_t::manual_arc_result> final {
	typedef struct manual_arc_result value_type;
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
struct msg_helper<mid_t::queryBatState> final {
	typedef struct simple_msg value_type;
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
struct msg_helper<mid_t::shutdown> final {
	typedef struct simple_msg value_type;
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
struct msg_helper<mid_t::startDustCheckFull> final {
	typedef struct simple_msg value_type;
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

