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
	ifd_cant_identify = 0x40000000,
	ifd_all = 0x7FFFFFFF,
};

typedef struct ifd_line {
	enum ifd_t dbmp;
	double hangle;	/// @unit: degree
	double vangle;	/// @unit: degree
	double clad_dm;	/// @unit: um
} ifd_line_t;

typedef struct defect_detect_result {
	ifd_line_t yzl;
	ifd_line_t yzr;
	ifd_line_t xzl;
	ifd_line_t xzr;
	double yz_hangle;	/// @unit: degree
	double xz_hangle;
	double lft_vangle;
	double rt_vangle;
	std::vector<uint8_t> yz_img;
	std::vector<uint8_t> xz_img;
	std::vector<uint8_t> yz_defect_img;
	std::vector<uint8_t> xz_defect_img;
} defect_detect_result_t;

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

enum motorId_t : unsigned {
	LZ = 0,	/// left motor
	RZ = 1,	/// right motor
	X = 2,
	Y = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum motorId_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 3;
	static constexpr unsigned size = 4;
};
static constexpr unsigned min_motorId = 0;
static constexpr unsigned max_motorId = 3;
static constexpr unsigned rsize_motorId = 4;

enum class fs_display_mode_t : unsigned {
	X = 0,
	Y = 1,
	TB = 2,
	LR = 3,
	NO = 4,
	/// @min : 0, @max : 4
};
template<> struct enum_info<enum fs_display_mode_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 4;
	static constexpr unsigned size = 5;
};
static constexpr unsigned min_fs_display_mode = 0;
static constexpr unsigned max_fs_display_mode = 4;
static constexpr unsigned rsize_fs_display_mode = 5;

enum class fs_err_t : unsigned {
	SUCCESS = 0,
	MEDDIACY = 1,	/// not final result
	UNKOWN_ERR = 2,
	COVER_OPENNED = 3,
	NO_FIBER = 4,
	FIBER_DEFECT = 5,
	FIBER_RECO = 6,
	CROSS_OVER = 7,
	OFF_CENTER = 8,
	IMG_LUM = 9,
	ABNORMAL_ARC = 10,
	TENSE_TEST_FAIL = 11,
	FIBER_BROKEN = 12,
	QUIT_MIDWAY = 13,
	PUSH_TIMEOUT = 14,
	CALIBRATE_TIMEOUT = 15,
	RESET_TIMEOUT = 16,
	ARC_TIME_ZERO = 17,
	ARC1_REVISE = 18,
	ARC2_REVISE = 19,
	FOCUS_X = 20,
	FOCUS_Y = 21,
	IMG_PROCESS = 22,
	SYSTEM_ERR = 23,
	FIBER_OFFSIDE = 24,	/// user should replace fiber
	CMOS_EXP = 25,
	LOSS_EST = 26,
	ARC_POS = 27,
	ARC_OVERFLOW = 28,
	/// @min : 0, @max : 28
};
template<> struct enum_info<enum fs_err_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 28;
	static constexpr unsigned size = 29;
};
static constexpr unsigned min_fs_err = 0;
static constexpr unsigned max_fs_err = 28;
static constexpr unsigned rsize_fs_err = 29;

enum class ledId_t : unsigned {
	CMOS_X = 0,
	CMOS_Y = 1,
	/// @min : 0, @max : 1
};
template<> struct enum_info<enum ledId_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 1;
	static constexpr unsigned size = 2;
};
static constexpr unsigned min_ledId = 0;
static constexpr unsigned max_ledId = 1;
static constexpr unsigned rsize_ledId = 2;

enum class cmosId_t : unsigned {
	X = 0,
	Y = 1,
	/// @min : 0, @max : 1
};
template<> struct enum_info<enum cmosId_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 1;
	static constexpr unsigned size = 2;
};
static constexpr unsigned min_cmosId = 0;
static constexpr unsigned max_cmosId = 1;
static constexpr unsigned rsize_cmosId = 2;

typedef struct mag2shrink {
	double x;	/// @unit: volt
	double y;	/// @unit: um
} mag2shrink_t;

typedef struct discharge_data {
	mag2shrink_t p[2];
	double temp;	/// @unit: degree centigrade
	double pressure;	/// @unit: bar
} discharge_data_t;

typedef struct rt_revise_data {
	int32_t x_exposure;
	int32_t y_exposure;
	double a3;
	double a2;
	double a1;
	double a0;
	double offset_auto;
	double offset_cal;
} rt_revise_data_t;

enum class fiber_t : unsigned {
	AUTO = 0,
	SM = 1,
	DS = 2,
	NZ = 3,
	MM = 4,
	FOLLOW = 5,
	/// @min : 0, @max : 5
};
template<> struct enum_info<enum fiber_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 5;
	static constexpr unsigned size = 6;
};
static constexpr unsigned min_fiber = 0;
static constexpr unsigned max_fiber = 5;
static constexpr unsigned rsize_fiber = 6;

typedef struct fiber_reco_data {
	double data[4][5][3];
} fiber_reco_data_t;

typedef struct fiber_rec_info {
	enum fiber_t ft;
	double clad_dm;	/// @unit: um
	double core_dm;	/// @unit: um
} fiber_rec_info_t;

enum class fs_pattern_t : unsigned {
	AUTO = 0,
	CALIBRATE = 1,
	NORMAL = 2,
	SPECIAL = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum fs_pattern_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 3;
	static constexpr unsigned size = 4;
};
static constexpr unsigned min_fs_pattern = 0;
static constexpr unsigned max_fs_pattern = 3;
static constexpr unsigned rsize_fs_pattern = 4;

enum class loss_estimate_mode_t : unsigned {
	OFF = 0,
	FINE = 1,
	CORE = 2,
	CLAD = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum loss_estimate_mode_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 3;
	static constexpr unsigned size = 4;
};
static constexpr unsigned min_loss_estimate_mode = 0;
static constexpr unsigned max_loss_estimate_mode = 3;
static constexpr unsigned rsize_loss_estimate_mode = 4;

enum class align_method_t : unsigned {
	AUTO = 0,
	CLAD = 1,
	CORE = 2,
	FINE = 3,
	MANUAL = 4,
	/// @min : 0, @max : 4
};
template<> struct enum_info<enum align_method_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 4;
	static constexpr unsigned size = 5;
};
static constexpr unsigned min_align_method = 0;
static constexpr unsigned max_align_method = 4;
static constexpr unsigned rsize_align_method = 5;

typedef struct fs_param_cfg {
	int seqn;
	std::string name;
	enum fs_pattern_t fusion_mode;
	enum fiber_t lfti;
	enum fiber_t rfti;
	enum align_method_t align_mode;
	bool x_focus;
	bool y_focus;
	bool ecf_redress;
	bool fiber_shift;
	bool auto_mag;
	double vangle_limit;	/// @unit: degree
	double hangle_limit;	/// @unit: degree
	double clr_mag;	/// @unit: volt
	int clr_time;	/// @unit: ms
	double clr_pos;	/// @unit: um
	double position;	/// @unit: um
	double gap;	/// @unit: um
	double overlap;	/// @unit: um
	double pre_mag;	/// @unit: volt
	int pre_time;	/// @unit: ms
	double arc1_mag;	/// @unit: volt
	int arc1_time;	/// @unit: ms
	double arc2_mag;	/// @unit: volt
	int arc2_time;	/// @unit: ms
	int arc2_on_time;	/// @unit: ms
	int arc2_off_time;	/// @unit: ms
	int arc_man_time;	/// @unit: ms
	int lft_push_speed;	/// @unit: um/s
	int rt_push_speed;	/// @unit: um/s
	bool taper_splice;
	int taper_wait_time;	/// @unit: ms
	double taper_length;	/// @unit: um
	int taper_speed;	/// @unit: um/s
	bool tense_test;
	int tense_speed;	/// @unit: um/s
	double tense_length;	/// @unit: um
	enum loss_estimate_mode_t loss_mode;
	double loss_limit;	/// @unit: db
	double loss_min;	/// @unit: db
	double lft_mfd;	/// @unit: um
	double rt_mfd;	/// @unit: um
	double syn_bend_co;
	double opp_bend_co;
	double mfd_mis_co;	/// @range: 0.0~1.0
} fs_param_cfg_t;

enum heat_material_t : unsigned {
	STANDARD = 0,
	MICRO250 = 1,
	MICRO400 = 2,
	MICRO900 = 3,
	CONNECTOR = 4,
	/// @min : 0, @max : 4
};
template<> struct enum_info<enum heat_material_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 4;
	static constexpr unsigned size = 5;
};
static constexpr unsigned min_heat_material = 0;
static constexpr unsigned max_heat_material = 4;
static constexpr unsigned rsize_heat_material = 5;

enum shrinktube_length_t : unsigned {
	L20MM = 0,
	L40MM = 1,
	L60MM = 2,
	/// @min : 0, @max : 2
};
template<> struct enum_info<enum shrinktube_length_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 2;
	static constexpr unsigned size = 3;
};
static constexpr unsigned min_shrinktube_length = 0;
static constexpr unsigned max_shrinktube_length = 2;
static constexpr unsigned rsize_shrinktube_length = 3;

typedef struct heat_param_cfg {
	int seqn;
	std::string name;
	enum heat_material_t material;
	enum shrinktube_length_t length;
	bool auto_heat;
	int heat_time;	/// @unit: s
	int heat_temp;	/// @unit: degree Celsius
	int finish_temp;	/// @unit: degree Celsius
	bool fast_heat;
	int temp_stay;	/// @unit: degree Celsius
} heat_param_cfg_t;

typedef struct misc_cfg {
	int fsParamIdx;
	int heatParamIdx;
} misc_cfg_t;

typedef struct fs_option_cfg {
	struct {
		bool autoStart;
		bool pause1;
		bool pause2;
	} operationOptions;
	struct {
		bool cleaveAngle;
		bool axisOffset;
		bool arcCorrectedValue;
	} dataDisplay;
	struct {
		bool cleave;
		bool loss;
		bool fiberCoreAngle;
		bool bubble;
		bool thick;
		bool thin;
	} ignoreOptions;
	struct {
		bool pressure;
		bool temperature;
		bool RealTimeRevise;
	} arcCompensation;
	struct {
		enum fs_display_mode_t gapSet;
		enum fs_display_mode_t pause1;
		enum fs_display_mode_t alignOption;
		enum fs_display_mode_t pause2;
		enum fs_display_mode_t arc;
		enum fs_display_mode_t estimateLoss;
	} fiberImageDisplay;
	struct {
		bool autoFeedFiber;
		bool badCleavedEndface;
		int resetAfterSplice;	/// @unit: s
		bool cleanAgain;
		bool imageZoomIn;
		int manualArcLimit;
	} others;
} fs_option_cfg_t;

struct fs_state {
	enum svc_fs_state_t sstate;
};

struct heat_state {
	enum svc_heat_state_t sstate;
};

struct simple_msg {};

typedef struct fspre_state {
	double core_offset;	/// @unit: um
	double clad_offset;	/// @unit: um
	double endface_gap;	/// @unit: um
	double vertex_angle;	/// @unit: degree
} fspre_state_t;

struct tense_test_result {
	bool exed;
	bool pass;
};

struct loss_estimating_result {
	bool valid;
	double loss_data;
};

typedef struct fiber_reco_result {
	fiber_rec_info_t lft;
	fiber_rec_info_t rt;
} fiber_reco_result_t;

struct manual_arc_result {
	int count;
};

typedef struct fusion_splice_result {
	int time_consume;	/// @unit: ms
	enum fs_err_t code;
	double loss;	/// @unit: db
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	fspre_state_t prestate;
	struct tense_test_result tense_test;
	struct manual_arc_result manual_arc;
	std::vector<uint8_t> xz_final_img;
	std::vector<uint8_t> yz_final_img;
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
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	fspre_state_t prestate;
	struct tense_test_result tense_test;
	discharge_data_t base;
	discharge_data_t revise;
	double suggest1;	/// @unit: volt
	double suggest2;	/// @unit: volt
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
	enum cmosId_t cmosid;
	std::vector<uint8_t> ori_img;
	std::vector<uint8_t> dust_img;
} dust_check_result_t;

struct heat_start {};

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
	std::vector<uint8_t> data;
};

typedef struct motor_test_result {
	enum fs_err_t code;
	struct fs_mt_cfg z_cfg;
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	uint32_t base_count;
	uint32_t arc_count;
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
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	fspre_state_t prestate;
	double loss;	/// @unit: db
	rt_revise_data_t RealtimeReviseData;
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
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	uint32_t cnt;
	uint32_t cnt_limit;
	double lft_attr[2];
	double rt_attr[2];
} fiber_train_result_t;

typedef struct count_down {
	uint32_t cnt;	/// @unit: second
} count_down_t;

typedef struct motor_spec {
	int clock;	/// @unit: HZ
	int raw_max;
	int raw_min;
	int backlash;	/// @unit: step
	int stroke;	/// @unit: step
	double sfactor;	/// @unit: um
	double spow;
	double lps;	/// @unit: nm/step
} motor_spec_t;

typedef struct cmos_spec {
	std::string model;
	int full_width;
	int full_height;
	int win_width;
	int win_height;
	int win_left;
	int win_top;
	int min_exposure;
	int max_exposure;
	double pixel_width;	/// @unit: um
	double pixel_height;	/// @unit: um
} cmos_spec_t;

typedef struct hvb_spec {
	double max_volt;	/// @unit: volt
	double pressure_c0;
	double pressure_c1;
} hvb_spec_t;

typedef struct ia_spec {
	double bg_lum;	/// @range: 0.0~1.0
	int cap_delay;	/// @unit: ms
	int cover_delay;	/// @unit: ms
	double led_lum[2];
	double dc_th0;
	double dc_th1;
	int denoise_th;
	double loss_est_factor;
	double vdist_th0;	/// @unit: pixel
	double vdist_th1;	/// @unit: pixel
	double hdist_th0;	/// @unit: pixel
	double hdist_th1;	/// @unit: pixel
	double hangle_th;	/// @unit: degree
	double vangle_th;	/// @unit: degree
} ia_spec_t;

typedef struct mc_spec {
	int reset_speed;	/// @unit: um/s
	int enter_speed;	/// @unit: um/s
	int push1_speed;	/// @unit: um/s
	int push2_speed;	/// @unit: um/s
	int calib_speed;	/// @unit: um/s
	int manual_speed;	/// @unit: um/s
} mc_spec_t;

typedef struct ar_spec {
	discharge_data_t base;
	discharge_data_t revise;
} ar_spec_t;

typedef struct rr_spec {
	rt_revise_data_t sm;
	rt_revise_data_t mm;
	rt_revise_data_t nz;
	rt_revise_data_t ds;
} rr_spec_t;

typedef struct fr_spec {
	fiber_reco_data_t left;
	fiber_reco_data_t right;
} fr_spec_t;

typedef struct update_window_position {
	bool is_pos_x;
	uint32_t row;
	uint32_t column;
} update_window_position_t;

struct update_led_brightness {
	enum ledId_t id;
	double brightness;
};

