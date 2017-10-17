#pragma once

#include "jmsg_types.hpp"

#define DCL_MSG(_x) msg_helper<mid_t::_x>::value_type

enum class mid_t : uint16_t {
	nil = 0,	/// occupy symbol, correspond to \em NULL.
	arcTestResult = 1,
	arc_revise = 2,
	bat_state = 3,
	countDown = 4,
	defect_detect_result = 5,
	discharge = 6,
	dischargeAdjustInit = 7,
	dischargeAdjustResult = 8,
	dischargeAdjustStart = 9,
	discharge_count = 10,
	dustCheckFullResult = 11,
	dustCheckFullStart = 12,
	dustCheckResult = 13,
	dustCheckStart = 14,
	fiberTrainInit = 15,
	fiberTrainResult = 16,
	fiberTrainStart = 17,
	fiber_reco_result = 18,
	fsCoverOpenned = 19,
	fs_cover_state = 20,
	fs_state = 21,
	fspre_state = 22,
	fusionSpliceReset = 23,
	fusionSpliceResult = 24,
	fusionSpliceStart = 25,
	getFiberDefectInfo = 26,
	goOn = 27,
	heatTestResult = 28,
	heat_result = 29,
	heat_start = 30,
	heat_state = 31,
	image_move = 32,
	lcd_power_ctl = 33,
	loss_estimating_result = 34,
	manual_arc_result = 35,
	motorTestResult = 36,
	motorTestStart = 37,
	motor_start = 38,
	motor_stop = 39,
	process_progress = 40,
	queryBatState = 41,
	queryDevState = 42,
	queryWaveForm = 43,
	realtimeReviseResult = 44,
	realtimeReviseStart = 45,
	regularTestResult = 46,
	regularTestStart = 47,
	report_dev_state = 48,
	report_wave_form = 49,
	setFsDisplayModeExt = 50,
	set_fs_display_mode = 51,
	set_fs_display_zoom_ext = 52,
	set_lcd_brightness = 53,
	set_led = 54,
	shutdown = 55,
	skip = 56,
	stabilizeElectrodeResult = 57,
	stabilizeElectrodeStart = 58,
	startDustCheckFull = 59,
	stop = 60,
	stopDischarge = 61,
	tenseTestResult = 62,
	update_led_brightness = 63,
	update_window_position = 64,
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
struct msg_helper<mid_t::fs_state> final {
	typedef struct fs_state value_type;
};

template<>
struct msg_helper<mid_t::fspre_state> final {
	typedef struct fspre_state value_type;
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

