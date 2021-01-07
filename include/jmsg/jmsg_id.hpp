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
	dbg_start_motor = 6,
	dbg_stop_motor = 7,
	defect_detect_result = 8,
	discharge = 9,
	dischargeAdjustInit = 10,
	dischargeAdjustResult = 11,
	dischargeAdjustStart = 12,
	discharge_count = 13,
	discharge_v2 = 14,
	dustCheckFullResult = 15,
	dustCheckFullStart = 16,
	dustCheckResult = 17,
	dustCheckStart = 18,
	fiberTrainInit = 19,
	fiberTrainResult = 20,
	fiberTrainStart = 21,
	fiber_reco_result = 22,
	fsCoverOpenned = 23,
	fs_cover_state = 24,
	fs_reset = 25,
	fs_state = 26,
	fspre_state = 27,
	fusionSpliceResult = 28,
	getFiberDefectInfo = 29,
	goOn = 30,
	heatTestResult = 31,
	heat_result = 32,
	heat_start = 33,
	heat_state = 34,
	lcd_power_ctl = 35,
	llvl_request = 36,
	load_cfg = 37,
	loss_estimating_result = 38,
	manual_arc_result = 39,
	motorTestResult = 40,
	motorTestStart = 41,
	move_image = 42,
	process_progress = 43,
	queryBatState = 44,
	queryDevState = 45,
	queryWaveForm = 46,
	realtimeReviseResult = 47,
	realtimeReviseStart = 48,
	regularTestResult = 49,
	report_dev_state = 50,
	report_wave_form = 51,
	save_cfg = 52,
	set_exposure = 53,
	set_lcd_brightness = 54,
	set_led = 55,
	set_motor_speed = 56,
	set_multi_stream = 57,
	set_single_stream = 58,
	shutdown = 59,
	skip = 60,
	stabilizeElectrodeResult = 61,
	stabilizeElectrodeStart = 62,
	startDustCheckFull = 63,
	startFusionSplice = 64,
	startRegularTest = 65,
	start_motor = 66,
	stop = 67,
	stopDischarge = 68,
	stop_motor = 69,
	sys_cfg = 70,
	tenseTestResult = 71,
	update_led_brightness = 72,
	update_window_position = 73,
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
struct msg_helper<mid_t::dbg_start_motor> final {
	typedef struct motor_start_info value_type;
};

template<>
struct msg_helper<mid_t::dbg_stop_motor> final {
	typedef struct motor_stop_info value_type;
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
struct msg_helper<mid_t::discharge_v2> final {
	typedef struct discharge_v2 value_type;
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
struct msg_helper<mid_t::lcd_power_ctl> final {
	typedef struct lcd_power_ctl value_type;
};

template<>
struct msg_helper<mid_t::llvl_request> final {
	typedef struct llvl_request value_type;
};

template<>
struct msg_helper<mid_t::load_cfg> final {
	typedef struct simple_msg value_type;
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
struct msg_helper<mid_t::move_image> final {
	typedef struct move_image value_type;
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
struct msg_helper<mid_t::report_dev_state> final {
	typedef struct report_dev_state value_type;
};

template<>
struct msg_helper<mid_t::report_wave_form> final {
	typedef struct report_wave_form value_type;
};

template<>
struct msg_helper<mid_t::save_cfg> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::set_exposure> final {
	typedef struct set_exposure value_type;
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
struct msg_helper<mid_t::set_motor_speed> final {
	typedef struct motor_speed_info value_type;
};

template<>
struct msg_helper<mid_t::set_multi_stream> final {
	typedef struct mstream_display_info value_type;
};

template<>
struct msg_helper<mid_t::set_single_stream> final {
	typedef struct sstream_display_info value_type;
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
struct msg_helper<mid_t::startFusionSplice> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::startRegularTest> final {
	typedef struct simple_msg value_type;
};

template<>
struct msg_helper<mid_t::start_motor> final {
	typedef struct motor_start_info value_type;
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
struct msg_helper<mid_t::stop_motor> final {
	typedef struct motor_stop_info value_type;
};

template<>
struct msg_helper<mid_t::sys_cfg> final {
	typedef struct sys_cfg value_type;
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

