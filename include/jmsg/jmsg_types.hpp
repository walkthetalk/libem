#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

template<typename T>
struct enum_info { };

enum ifd_t : uint32_t {
	ifd_end_crude = 0x00000001,
	ifd_horizontal_angle = 0x00000002,
	ifd_vertical_angle = 0x00000004,
	ifd_cant_identify = 0x80000000,
	ifd_all = 0xFFFFFFFF,
};

struct ifd_line_t {
	enum ifd_t dbmp;
	double h_angle;	/// @unit: degree
	double v_angle;	/// @unit: degree
	int32_t wrap_diameter;	/// @unit: pixel
};

struct img_defects_t {
	struct ifd_line_t yzl;
	struct ifd_line_t yzr;
	struct ifd_line_t xzl;
	struct ifd_line_t xzr;
	double yz_hangle_intersect;
	double xz_hangle_intersect;
	std::string yz_ref_img;
	std::string xz_ref_img;
	std::string yz_img;
	std::string xz_img;
};

enum class svc_fs_state_t : uint16_t {
	reseting = 0,
	idle = 1,
	ready = 2,
	entering = 3,
	push1 = 4,
	calibrating = 5,
	waiting = 6,
	clring = 7,
	focusing = 8,
	defect_detecting = 9,
	fiber_rec = 10,
	push2 = 11,
	pause1 = 12,
	precise_calibrating = 13,
	pause2 = 14,
	pre_splice = 15,
	discharge1 = 16,
	discharge2 = 17,
	discharge_manual = 18,
	loss_estimating = 19,
	tension_testing = 20,
	finished = 21,
	wait_reset = 22,
	/// @min : 0, @max : 22
};
template<> struct enum_info<enum svc_fs_state_t> {
	static constexpr uint16_t min = 0;
	static constexpr uint16_t max = 22;
	static constexpr uint16_t size = 23;
};
static constexpr uint16_t min_svc_fs_state = 0;
static constexpr uint16_t max_svc_fs_state = 22;
static constexpr uint16_t rsize_svc_fs_state = 23;

enum class svc_heat_state_t : uint16_t {
	heat_idle = 0,
	heat_ready = 4,
	heat_ascending = 5,
	heat_stabling = 6,
	heat_descending = 7,
	/// @min : 0, @max : 7
};
template<> struct enum_info<enum svc_heat_state_t> {
	static constexpr uint16_t min = 0;
	static constexpr uint16_t max = 7;
	static constexpr uint16_t size = 8;
};
static constexpr uint16_t min_svc_heat_state = 0;
static constexpr uint16_t max_svc_heat_state = 7;
static constexpr uint16_t rsize_svc_heat_state = 8;

enum motorId_t : uint8_t {
	LZ = 0,	/// left motor
	RZ = 1,	/// right motor
	X = 2,
	Y = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum motorId_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 3;
	static constexpr uint8_t size = 4;
};
static constexpr uint8_t min_motorId = 0;
static constexpr uint8_t max_motorId = 3;
static constexpr uint8_t rsize_motorId = 4;

enum class fs_display_mode_t : uint8_t {
	X = 0,
	Y = 1,
	TB = 2,
	LR = 3,
	NO = 4,
	/// @min : 0, @max : 4
};
template<> struct enum_info<enum fs_display_mode_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 4;
	static constexpr uint8_t size = 5;
};
static constexpr uint8_t min_fs_display_mode = 0;
static constexpr uint8_t max_fs_display_mode = 4;
static constexpr uint8_t rsize_fs_display_mode = 5;

enum class fs_err_t : uint8_t {
	success = 0,
	cover_openned = 1,
	no_fiber = 2,
	fiber_defect = 3,
	fiber_cross_over = 4,
	fiber_off_center = 5,
	img_brightness = 6,
	abnormal_arc = 7,
	tense_test_fail = 8,
	fiber_broken = 9,
	quit_midway = 10,
	push_timeout = 11,
	calibrate_timeout = 12,
	reset_timeout = 13,
	arc_time_zero = 14,
	ignore = 15,
	revise1_mag = 16,
	revise2_mag = 17,
	focus_x = 18,
	focus_y = 19,
	img_process_error = 20,
	system_error = 21,
	fiber_offside = 22,	/// user should replace fiber
	cmos_exposure = 23,
	loss_estimate = 24,
	arc_off_center = 25,
	failed = 26,
	arc_mag_overflow = 27,
	/// @min : 0, @max : 27
};
template<> struct enum_info<enum fs_err_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 27;
	static constexpr uint8_t size = 28;
};
static constexpr uint8_t min_fs_err = 0;
static constexpr uint8_t max_fs_err = 27;
static constexpr uint8_t rsize_fs_err = 28;

enum class ledId_t : uint8_t {
	CMOS_X = 0,
	CMOS_Y = 1,
	/// @min : 0, @max : 1
};
template<> struct enum_info<enum ledId_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 1;
	static constexpr uint8_t size = 2;
};
static constexpr uint8_t min_ledId = 0;
static constexpr uint8_t max_ledId = 1;
static constexpr uint8_t rsize_ledId = 2;

enum class cmosId_t : uint8_t {
	X = 0,
	Y = 1,
	/// @min : 0, @max : 1
};
template<> struct enum_info<enum cmosId_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 1;
	static constexpr uint8_t size = 2;
};
static constexpr uint8_t min_cmosId = 0;
static constexpr uint8_t max_cmosId = 1;
static constexpr uint8_t rsize_cmosId = 2;

struct mag2shrink_t {
	double x;	/// @unit: volt
	double y;	/// @unit: um
};

struct discharge_data_t {
	struct mag2shrink_t p[2];
	double temp;	/// @unit: degree centigrade
	double pressure;	/// @unit: bar
};

struct rt_revise_data_t {
	int32_t rt_x_exposure;
	int32_t rt_y_exposure;
	double rt_revise_a3;
	double rt_revise_a2;
	double rt_revise_a1;
	double rt_revise_a0;
	double rt_offset_auto;
	double rt_offset_cal;
};

enum class fiber_t : uint8_t {
	sm = 0,
	ds = 1,
	nz = 2,
	mm = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum fiber_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 3;
	static constexpr uint8_t size = 4;
};
static constexpr uint8_t min_fiber = 0;
static constexpr uint8_t max_fiber = 3;
static constexpr uint8_t rsize_fiber = 4;

struct fiber_reco_data_t {
	double data[4][5][3];
};

struct fiber_rec_info_t {
	enum fiber_t ft;
	uint32_t wrap_diameter;	/// @unit: nm
	uint32_t core_diameter;	/// @unit: nm
};

enum class fs_pattern_t : uint8_t {
	automatic = 0,
	calibrate = 1,
	normal = 2,
	special = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum fs_pattern_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 3;
	static constexpr uint8_t size = 4;
};
static constexpr uint8_t min_fs_pattern = 0;
static constexpr uint8_t max_fs_pattern = 3;
static constexpr uint8_t rsize_fs_pattern = 4;

enum class loss_estimate_mode_t : uint8_t {
	off = 0,
	accurate = 1,
	core = 2,
	cladding = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum loss_estimate_mode_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 3;
	static constexpr uint8_t size = 4;
};
static constexpr uint8_t min_loss_estimate_mode = 0;
static constexpr uint8_t max_loss_estimate_mode = 3;
static constexpr uint8_t rsize_loss_estimate_mode = 4;

enum class shrink_tube_t : uint8_t {
	len_20mm = 0,
	len_40mm = 1,
	len_60mm = 2,
	/// @min : 0, @max : 2
};
template<> struct enum_info<enum shrink_tube_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 2;
	static constexpr uint8_t size = 3;
};
static constexpr uint8_t min_shrink_tube = 0;
static constexpr uint8_t max_shrink_tube = 2;
static constexpr uint8_t rsize_shrink_tube = 3;

enum class align_method_t : uint8_t {
	fine = 0,
	clad = 1,
	core = 2,
	manual = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum align_method_t> {
	static constexpr uint8_t min = 0;
	static constexpr uint8_t max = 3;
	static constexpr uint8_t size = 4;
};
static constexpr uint8_t min_align_method = 0;
static constexpr uint8_t max_align_method = 3;
static constexpr uint8_t rsize_align_method = 4;

struct fs_base_cfg_t {
	enum fs_pattern_t FSPattern;
	enum fiber_t FiberType;
	enum align_method_t FiberAlignment;
	bool XImageFocus;
	bool YImageFocus;
	bool FiberShift;
	bool DischargeStrengthAdjustment;
	bool TensionSet;
	double CutAngleLimit;
	double LossLimit;
	double FiberAngleLimit;
	double CleanDischargeStrength;
	uint32_t CleanDischargeTime;
	uint32_t FiberIntervalSetup;
	int32_t FSPosSetup;
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
	double LeftFSSpeed;	/// @range: 0.0~1.0
	double RightFSSpeed;	/// @range: 0.0~1.0
	bool ConeFS;
	uint32_t ConeFSWaitTime;
	double ConeFSSpeed;
	uint32_t ConeFSStretchLength;
	enum loss_estimate_mode_t LossEstimationMode;
	double LeftFiberMFD;	/// @unit: um
	double RightFiberMFD;	/// @unit: um
	double LeastLoss;	/// @unit: db
	double RateOfSyntropyBending;
	double RateOfReverseBending;
	double MFDMismatchCoefficient;	/// @range: 0.0~1.0
	bool AutoStart;
	bool Stop1;
	bool Stop2;
	bool CutAngle;
	bool OffsetData;
	bool ArcCorrectedValue;
	bool Cut;
	bool Loss;
	bool FiberCoreAngle;
	bool Bubble;
	bool Thick;
	bool Thin;
	bool AirPressure;
	bool Temperature;
	bool RealTimeRevise;
	enum fs_display_mode_t ImgGap;
	enum fs_display_mode_t ImgStop1;
	enum fs_display_mode_t ImgAlign;
	enum fs_display_mode_t ImgStop2;
	enum fs_display_mode_t ImgDischarge;
	enum fs_display_mode_t ImgLossEstimation;
	bool FiberAutoFeed;
	bool BadCutSurface;
	uint32_t AutoReset;
	bool CleanDischargeTwice;
	uint32_t ManualDischargeTimes;
};

struct fs_state {
	enum svc_fs_state_t sstate;
};

struct heat_state {
	enum svc_heat_state_t sstate;
};

struct simple_msg {};

typedef struct record_offset {
	double core_diff_pre;	/// @unit: um
	double cladding_diff_pre;	/// @unit: um
	double vertex_intersect_angle;	/// @unit: degree
} record_offset_t;

struct tense_test_result {
	bool is_tense_test;
	bool is_success;
};

struct loss_estimating_result {
	bool valid;
	double loss_data;
};

struct defect_detect_result {
	struct img_defects_t data;
};

struct fiber_reco_result {
	struct fiber_rec_info_t lft_rec_info;
	struct fiber_rec_info_t rt_rec_info;
};

struct manual_discharge_counts {
	int counts;
};

typedef struct fusion_splice_result {
	enum fs_err_t code;
	struct fiber_rec_info_t lft_rec_info;
	struct fiber_rec_info_t rt_rec_info;
	struct img_defects_t defect_data;
	record_offset_t z_record_off_set;
	double pattern_compensate;	/// @range: 0.0~1.0
	double loss_db;	/// @unit: db
	struct tense_test_result z_tense_test_result;
	struct manual_discharge_counts z_manual_discharge_counts;
	std::string fs_done_x_img;
	std::string fs_done_y_img;
} fusion_splice_result_t;

struct arc_revise {
	double revise;
};

typedef struct fs_da_cfg {
	double arc1_mag;
	double arc2_mag;
} fs_da_cfg_t;

typedef struct discharge_adjust_result {
	enum fs_err_t code;
	struct fs_da_cfg z_cfg;
	struct fiber_rec_info_t rec_info;
	struct img_defects_t defect_data;
	struct discharge_data_t base;
	struct discharge_data_t revise;
	double suggest1;
	double suggest2;
} discharge_adjust_result_t;

struct discharge {
	double magnitude;	/// @unit: volt
	uint16_t time;	/// @unit: ms
};

struct discharge_count {
	uint32_t discharge_count;
};

struct set_fs_display_mode_ext {
	bool xovery;
	uint16_t x_left;
	uint16_t x_top;
	uint16_t x_width;
	uint16_t x_height;
	uint16_t y_left;
	uint16_t y_top;
	uint16_t y_width;
	uint16_t y_height;
};

struct set_fs_display_zoom_ext {
	uint16_t x_left;
	uint16_t x_up;
	uint16_t x_width;
	uint16_t x_height;
	uint16_t y_left;
	uint16_t y_up;
	uint16_t y_width;
	uint16_t y_height;
};

struct set_fs_display_mode {
	enum fs_display_mode_t mode;
};

typedef struct dust_check_result {
	enum fs_err_t code;
	bool xz_ok;
	std::string xz_ori;
	std::string xz_dust;
	bool yz_ok;
	std::string yz_ori;
	std::string yz_dust;
} dust_check_result_t;

struct heat_start {
	uint32_t Material;
	enum shrink_tube_t Fiberlen;
	bool Heatctl;
	uint16_t heat_time;	/// @unit: second
	uint16_t heat_temp;	/// @unit: degree Celsius
	uint16_t finish_temp;	/// @unit: degree Celsius
	uint16_t stable_temp;	/// @unit: degree Celsius
	bool fast_heat;
};

struct heat_result {
	enum fs_err_t code;
};

struct image_move {
	bool is_pos_x;
	int16_t row;
	int16_t column;
};

struct set_window {
	bool is_pos_x;
	uint16_t row;
	uint16_t column;
};

struct fs_cover_state {
	bool is_openned;
};

struct set_lcd_brightness {
	double brightness;	/// @range: 0.0~1.0
};

struct set_led {
	enum ledId_t id;
	double brightness;	/// @range: 0.0~1.0
};

struct motor_start {
	enum motorId_t id;
	bool m_forward_dir;
};

struct motor_stop {
	enum motorId_t id;
};

typedef struct fs_mt_cfg {
	uint32_t MotorTestTimes;
	uint32_t ElectricArcTestTimes;
	uint32_t CleanArcRate;
} fs_mt_cfg_t;

struct statistic_data_t {
	int32_t cnt;
	double ref_v;
	double min_v;
	double max_v;
	double mid_v;
	double avg_v;
	std::string data;
};

typedef struct motor_test_result {
	enum fs_err_t code;
	struct fs_mt_cfg z_cfg;
	struct fiber_rec_info_t rec_info;
	struct img_defects_t defect_data;
	uint32_t motor_tested_times;
	uint32_t ele_arc_tested_times;
	uint32_t reset;
	uint32_t push;
	uint32_t calibrate;
	uint32_t ele_arc;
	uint32_t img;
	struct statistic_data_t nm_per_pixel_xz;
	struct statistic_data_t nm_per_pixel_yz;
	struct statistic_data_t nm_per_step_lz;
	struct statistic_data_t nm_per_step_rz;
	struct statistic_data_t nm_push_lz;
	struct statistic_data_t nm_push_rz;
	struct statistic_data_t arc_mag;
	struct statistic_data_t img_process;
} motor_test_result_t;

struct process_progress {
	double progress;
};

typedef struct fs_rr_cfg {} fs_rr_cfg_t;

typedef struct realtime_revise_result {
	enum fs_err_t code;
	struct fiber_rec_info_t rec_info;
	struct img_defects_t defect_data;
	record_offset_t z_record_off_set;
	double pattern_compensate;	/// @range: 0.0~1.0
	double loss_db;	/// @unit: db
	struct tense_test_result z_tense_test_result;
	struct manual_discharge_counts z_manual_discharge_counts;
	struct rt_revise_data_t RealtimeReviseData;
} realtime_revise_result_t;

typedef struct regular_test_result {
	enum fs_err_t code;
} regular_test_result_t;

typedef struct fs_se_cfg {
	double magnitude;
	uint32_t time;	/// @unit: ms
	uint32_t interval;	/// @unit: ms
	uint32_t number;
} fs_se_cfg_t;

typedef struct stabilize_electrode_result {
	enum fs_err_t code;
	struct fs_se_cfg z_cfg;
	uint32_t number;
} stabilize_electrode_result_t;

struct report_dev_state {
	double pressure;	/// @unit: bar
	double humidity;
	double env_temp;	/// @unit: degree Celsius
	double int_temp;
	double heat_temp;
	double bat_voltage;	/// @unit: volt
};

struct report_wave_form {
	std::string x_wave;
	std::string y_wave;
};

typedef struct fs_ft_cfg {
	enum fiber_t lft_type;
	enum fiber_t rt_type;
} fs_ft_cfg_t;

typedef struct fiber_train_result {
	enum fs_err_t code;
	struct fs_ft_cfg z_cfg;
	struct fiber_rec_info_t rec_info;
	struct img_defects_t defect_data;
	uint32_t cnt;
	uint32_t cnt_limit;
	double lft_attr[2];
	double rt_attr[2];
} fiber_train_result_t;

typedef struct count_down {
	uint32_t cnt;	/// @unit: second
} count_down_t;

typedef struct motor_spec {
	uint16_t raw_max;
	uint16_t raw_min;
	double sfactor;	/// @unit: nm
	double spow;
	double lps;	/// @unit: nm/step
} motor_spec_t;

typedef struct update_window_position {
	bool is_pos_x;
	uint32_t row;
	uint32_t column;
} update_window_position_t;

struct update_led_brightness {
	enum ledId_t id;
	double brightness;
};

