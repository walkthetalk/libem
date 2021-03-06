#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

template<typename T>
struct enum_info { };

typedef struct region {
	int left;
	int top;
	int width;
	int height;
} region_t;

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
	double core_dm;	/// @unit: um
} ifd_line_t;

typedef struct defect_detect_result {
	ifd_line_t yzl;
	ifd_line_t yzr;
	ifd_line_t xzl;
	ifd_line_t xzr;
	double yzl_hangle;	/// @unit: degree
	double yzr_hangle;	/// @unit: degree
	double xzl_hangle;	/// @unit: degree
	double xzr_hangle;	/// @unit: degree
	double intersect_hangle;	/// @unit: degree
	double lft_vangle;	/// @unit: degree
	double rt_vangle;	/// @unit: degree
	std::string yz_img;
	std::string xz_img;
	std::string yz_defect_img;
	std::string xz_defect_img;
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
	regular_test = 23,
	/// @min : 0, @max : 23
};
template<> struct enum_info<enum svc_fs_state_t> {
	static constexpr uint16_t min = 0;
	static constexpr uint16_t max = 23;
	static constexpr uint16_t size = 24;
};
static constexpr uint16_t min_svc_fs_state = 0;
static constexpr uint16_t max_svc_fs_state = 23;
static constexpr uint16_t rsize_svc_fs_state = 24;

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
	lp = 0,	/// left push motor
	rp = 1,	/// right push motor
	ax = 2,	/// cmos x align motor
	ay = 3,	/// cmos y align motor
	lr = 4,	/// left rotate motor
	rr = 5,	/// right rotate motor
	/// @min : 0, @max : 5
};
template<> struct enum_info<enum motorId_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 5;
	static constexpr unsigned size = 6;
};
static constexpr unsigned min_motorId = 0;
static constexpr unsigned max_motorId = 5;
static constexpr unsigned rsize_motorId = 6;

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
	IMG_DUST = 10,
	ABNORMAL_ARC = 11,
	TENSE_TEST_FAIL = 12,
	FIBER_BROKEN = 13,
	QUIT_MIDWAY = 14,
	PUSH_TIMEOUT = 15,
	CALIBRATE_TIMEOUT = 16,
	RESET_TIMEOUT = 17,
	ARC_TIME_ZERO = 18,
	ARC1_REVISE = 19,
	ARC2_REVISE = 20,
	FOCUS_X = 21,
	FOCUS_Y = 22,
	IMG_PROCESS = 23,
	SYSTEM_ERR = 24,
	FIBER_OFFSIDE = 25,	/// user should replace fiber
	CMOS_EXP = 26,
	LOSS_EST = 27,
	ARC_POS = 28,
	ARC_OVERFLOW = 29,
	/// @min : 0, @max : 29
};
template<> struct enum_info<enum fs_err_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 29;
	static constexpr unsigned size = 30;
};
static constexpr unsigned min_fs_err = 0;
static constexpr unsigned max_fs_err = 29;
static constexpr unsigned rsize_fs_err = 30;

enum class ledId_t : unsigned {
	CMOS_X = 0,
	CMOS_Y = 1,
	LCD = 2,
	/// @min : 0, @max : 2
};
template<> struct enum_info<enum ledId_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 2;
	static constexpr unsigned size = 3;
};
static constexpr unsigned min_ledId = 0;
static constexpr unsigned max_ledId = 2;
static constexpr unsigned rsize_ledId = 3;

enum class cmosId_t : unsigned {
	x = 0,
	y = 1,
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

typedef struct arcpinfo {
	double mag;	/// @unit: volt
	int time;	/// @unit: ms
	double len;	/// @unit: um
} arcpinfo_t;

typedef struct arcpenvinfo {
	arcpinfo_t arc1;
	arcpinfo_t arc2;
	double temp;	/// @unit: degree centigrade
	double pressure;	/// @unit: bar
} arcpenvinfo_t;

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
	SM = 0,
	DS = 1,
	NZ = 2,
	MM = 3,
	FOLLOW = 4,
	AUTO = 5,
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
	double data[4][3][3];
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
	int ver;
	enum fs_pattern_t fusion_mode;
	enum fiber_t lfti;
	enum fiber_t rfti;
	enum align_method_t align_mode;
	bool x_focus;
	bool y_focus;
	bool ecf_redress;
	bool auto_mag;
	double vangle_limit;	/// @unit: degree
	double hangle_limit;	/// @unit: degree
	double push1_speed;	/// @unit: um/s
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
	double lft_push_speed;	/// @unit: um/s
	double rt_push_speed;	/// @unit: um/s
	bool taper_splice;
	int taper_wait_time;	/// @unit: ms
	double taper_length;	/// @unit: um
	double taper_speed;	/// @unit: um/s
	bool tense_test;
	double tense_speed;	/// @unit: um/s
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
	int hold_temp;	/// @unit: degree Celsius
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
	} operation;
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
		bool realTimeRevise;
	} arcCompensation;
	struct {
		enum fs_display_mode_t gapSet;
		enum fs_display_mode_t pause1;
		enum fs_display_mode_t align;
		enum fs_display_mode_t pause2;
		enum fs_display_mode_t arc;
		enum fs_display_mode_t estLoss;
	} fiberDisplay;
	struct {
		bool autoFeedFiber;
		bool badCleavedEndface;
		int resetAfterSplice;	/// @unit: s
		bool cleanAgain;
		bool imageZoomIn;
		uint32_t manualArcLimit;
	} others;
} fs_option_cfg_t;

struct fs_state {
	enum svc_fs_state_t sstate;
};

struct heat_state {
	enum svc_heat_state_t sstate;
};

typedef struct simple_msg {} simple_msg_t;

typedef struct fspre_state {
	double core_offset;	/// @unit: um
	double clad_offset;	/// @unit: um
	double endface_gap;	/// @unit: um
	double vertex_angle;	/// @unit: degree
} fspre_state_t;

typedef struct tense_test_result {
	bool exed;
	bool pass;
} tense_test_result_t;

struct loss_estimating_result {
	bool valid;
	double loss_data;
};

typedef struct fiber_reco_result {
	fiber_rec_info_t lft;
	fiber_rec_info_t rt;
} fiber_reco_result_t;

typedef struct manual_arc_result {
	int count;
} manual_arc_result_t;

typedef struct fusion_splice_result {
	std::string name;
	int fsp_seqn;
	int fsp_ver;
	int time_consume;	/// @unit: ms
	enum fs_err_t code;
	double loss;	/// @unit: db
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	fspre_state_t prestate;
	tense_test_result_t tense_test;
	manual_arc_result_t manual_arc;
	std::string xz_final_img;
	std::string yz_final_img;
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
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	arcpenvinfo_t base;
	arcpenvinfo_t revise;
	double suggest1;	/// @unit: volt
	double suggest2;	/// @unit: volt
} discharge_adjust_result_t;

typedef struct ar_run_data {
	int cnt;
	int continuous_success_cnt;
	arcpinfo_t arc1_min;
	arcpinfo_t arc1_max;
	arcpinfo_t arc2_min;
	arcpinfo_t arc2_max;
	std::vector< arcpinfo_t > d1_seq;
	std::vector< arcpinfo_t > d2_seq;
} ar_run_data_t;

struct discharge {
	double magnitude;	/// @unit: volt
	uint16_t time;	/// @unit: ms
};

typedef struct discharge_v2 {
	double mag0;	/// @unit: volt
	double time0;	/// @unit: ms
	double mag1;	/// @unit: volt
	double time1;	/// @unit: ms
	double inc_time;	/// @unit: ms
} discharge_v2_t;

struct discharge_count {
	uint32_t discharge_count;
};

typedef struct sstream_display_info {
	enum cmosId_t sid;
	int layerid;
	uint16_t width;
	uint16_t height;
	region_t src;
	region_t dst;
} sstream_display_info_t;

typedef struct mstream_display_info {
	std::vector<sstream_display_info_t> data;
} mstream_display_info_t;

typedef struct dust_check_result {
	enum fs_err_t code;
	enum cmosId_t cmosid;
	std::string ori_img;
	std::string dust_img;
} dust_check_result_t;

struct heat_start {};

struct heat_result {
	enum fs_err_t code;
};

typedef struct move_image {
	enum cmosId_t cmosId;
	int16_t row;
	int16_t column;
} move_image_t;

struct fs_cover_state {
	bool is_openned;
};

struct set_lcd_brightness {
	double brightness;	/// @range: 0.0~1.0
};

struct lcd_power_ctl {
	bool on;
};

struct set_led {
	enum ledId_t id;
	double brightness;	/// @range: 0.0~1.0
};

typedef struct set_exposure {
	enum cmosId_t cmosId;
	uint32_t exposure;
} set_exposure_t;

typedef struct motor_start_info {
	enum motorId_t id;
	bool forward;
	double speed;	/// @unit: um/s
	int distance;	/// @unit: nm
} motor_start_info_t;

typedef struct motor_stop_info {
	enum motorId_t id;
} motor_stop_info_t;

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
	fiber_reco_result_t recinfo;
	defect_detect_result_t defect;
	uint32_t base_count;
	uint32_t arc_count;
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

typedef struct mt_run_data {
	std::vector<double> statistic_yz_nm_per_pixel;
	std::vector<double> statistic_xz_nm_per_pixel;
	std::vector<double> statistic_lz_nm_per_step;
	std::vector<double> statistic_rz_nm_per_step;
	std::vector<double> statistic_lz_push_nm;
	std::vector<double> statistic_rz_push_nm;
	std::vector<double> statistic_arc_mag;
	std::vector<double> statistic_img_process;
} mt_run_data_t;

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
	double bat_percent;	/// @unit: %
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
	double lft_attr[3];
	double rt_attr[3];
} fiber_train_result_t;

typedef struct count_down {
	uint32_t cnt;	/// @unit: second
} count_down_t;

typedef struct motor_spec {
	int clock;	/// @unit: HZ
	int raw_hs;
	int raw_ls;
	int backlash;	/// @unit: step
	int stroke;	/// @unit: step
	double sfactor;	/// @unit: um
	double spow;
	double lps;	/// @unit: um/step
} motor_spec_t;

typedef struct cmos_spec {
	std::string model;
	int full_width;
	int full_height;
	int min_exposure;
	int max_exposure;
	double pixel_width;	/// @unit: um
	double pixel_height;	/// @unit: um
} cmos_spec_t;

typedef struct hvb_spec {
	double min_volt;	/// @unit: volt
	double max_volt;	/// @unit: volt
	double pressure_c0;
	double pressure_c1;
	double temp_c0;
	double temp_c1;
} hvb_spec_t;

typedef struct ia_spec {
	double bg_lum;	/// @range: 0.0~1.0
	double lens_mag;
	int cap_delay;	/// @unit: ms
	int cover_delay;	/// @unit: ms
	region_t fullx;
	region_t winx;
	region_t fully;
	region_t winy;
	double ledx_lum;	/// @range: 0.0~1.0
	double ledy_lum;	/// @range: 0.0~1.0
	uint32_t cmosx_exposure;
	uint32_t cmosy_exposure;
	double dc_th0;
	double dc_th1;
	uint32_t denoise_th;
	double loss_est_factor;
	double vdist_th0;	/// @unit: pixel
	double vdist_th1;	/// @unit: pixel
	double hdist_th0;	/// @unit: pixel
	double hdist_th1;	/// @unit: pixel
	double hangle_th;	/// @unit: degree
	double vangle_th;	/// @unit: degree
} ia_spec_t;

typedef struct mc_spec {
	double reset_speed;	/// @unit: um/s
	double enter_speed;	/// @unit: um/s
	double push1_speed;	/// @unit: um/s
	double push2_speed;	/// @unit: um/s
	double calib_speed;	/// @unit: um/s
	double manual_speed;	/// @unit: um/s
} mc_spec_t;

typedef struct ar_spec {
	arcpenvinfo_t base;
	arcpenvinfo_t revise;
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
	region_t winx;
} fr_spec_t;

struct dustCheckFullStart {
	enum cmosId_t cmosId;
};

typedef struct update_window_position {
	enum cmosId_t cmosId;
	uint32_t row;
	uint32_t column;
} update_window_position_t;

typedef struct update_led_brightness {
	enum ledId_t id;
	uint32_t denominator;
	uint32_t numerator;
} update_led_brightness_t;

enum power_t : unsigned {
	BATTERY = 0,	/// only battery
	ADAPTER = 1,	/// only adapter
	CHARGING = 2,	/// adapter conncted, charging
	UNKNOWN = 3,
	/// @min : 0, @max : 3
};
template<> struct enum_info<enum power_t> {
	static constexpr unsigned min = 0;
	static constexpr unsigned max = 3;
	static constexpr unsigned size = 4;
};
static constexpr unsigned min_power = 0;
static constexpr unsigned max_power = 3;
static constexpr unsigned rsize_power = 4;

typedef struct bat_state {
	enum power_t power_mode;
	int32_t percent;
} bat_state_t;

struct beep {
	uint32_t time;	/// @unit: second
};

typedef struct motor_speed_info {
	int brIdx;
	int stepn;
	double minspeed;	/// @unit: um/s
	double maxspeed;	/// @unit: um/s
} motor_speed_info_t;

typedef struct llvl_request {
	uint32_t cmd;
	uint32_t par0;
	uint32_t par1;
	uint32_t par2;
	uint32_t par3;
} llvl_request_t;

typedef struct sys_cfg {
	ia_spec_t iaparam;
	fs_param_cfg_t fsparam;
	fs_option_cfg_t fsoption;
	heat_param_cfg_t heatparam;
} sys_cfg_t;

enum svc_fs_type_t : uint32_t {
	fusion_splice = 0,
	regular_test = 1,
	/// @min : 0, @max : 1
};
template<> struct enum_info<enum svc_fs_type_t> {
	static constexpr uint32_t min = 0;
	static constexpr uint32_t max = 1;
	static constexpr uint32_t size = 2;
};
static constexpr uint32_t min_svc_fs_type = 0;
static constexpr uint32_t max_svc_fs_type = 1;
static constexpr uint32_t rsize_svc_fs_type = 2;

