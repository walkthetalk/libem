#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::set_fs_spec> {
public:
	int32_t window_x_row;			// unit: pixel
	int32_t window_x_col;			// unit: pixel
	int32_t window_y_row;			// unit: pixel
	int32_t window_y_col;			// unit: pixel

	uint32_t nm_per_pixel;			// unit: nm/pixel

	double zmotor_spec_nm_per_step;		// unit: nm/step

	double lz_nm_per_step;			// unit: nm/step
	double rz_nm_per_step;			// unit: nm/step

	uint16_t img_cap_delay;			// unit: ms

	double   clr_discharge_strength; 	// unit: volt
	uint32_t clr_discharge_gap;		// unit: nm
	uint16_t check_fiber_exist_time;	// unit: ms

	uint16_t motor_min_speed[motorId_t::NUM];
	uint16_t motor_max_speed[motorId_t::NUM];

	double entering_speed;
	double push1_speed;
	double push2_stage1_speed;
	double push2_stage2_speed;
	double manual_calibrate_speed;
	double motor_lzrz_fs_speed;
	double motor_xy_precise_calibrate_speed;
	double tensionSpeed;

	uint32_t tensionStretchLength;

	uint16_t motor_xy_steps_per_pixel;	// unit: step/pixel

	double   fiber_outline_blacklevel;		// 0.0 ~ 1.0
	double   calibrating_xy_dist_threshold;		// unit: pixel
	double   precise_calibrating_xy_dist_threshold;	// unit: pixel
	double   z_dist_threshold;			// unit: pixel

	discharge_data_t discharge_base;
	discharge_data_t discharge_revise;

	double   dust_check_threshold0;
	double   dust_check_threshold1;
	uint16_t img_denoise_threshold;

	double   led_brightness[ledId_t::LED_NUM];
	double   x_focal_distance;
	double   y_focal_distance;

	double   zmotor_speed_factor;
	double   zmotor_speed_pow;
	double   zmotor_speed_max;		/// 0.0 ~ 1.0

	double   xymotor_speed_factor;
	double   xymotor_speed_pow;
	double   xymotor_speed_max;		/// 0.0 ~ 1.0

	double   zmotor_forward_distance;	/// unit: nm
	double   zmotor_stroke;			/// unit: nm
public:
        ZMSG_PU(
		window_x_row,
		window_x_col,
		window_y_row,
		window_y_col,

		nm_per_pixel,
		zmotor_spec_nm_per_step,
		lz_nm_per_step,
		rz_nm_per_step,

		img_cap_delay,

		clr_discharge_strength,
		clr_discharge_gap,
		check_fiber_exist_time,

		motor_min_speed,
		motor_max_speed,

		entering_speed,
		push1_speed,
		push2_stage1_speed,
		push2_stage2_speed,
		manual_calibrate_speed,
		motor_lzrz_fs_speed,
		motor_xy_precise_calibrate_speed,
		tensionSpeed,

		tensionStretchLength,

		motor_xy_steps_per_pixel,

		fiber_outline_blacklevel,
		calibrating_xy_dist_threshold,
		precise_calibrating_xy_dist_threshold,
		z_dist_threshold,

		discharge_base,
		discharge_revise,

		dust_check_threshold0,
		dust_check_threshold1,
		img_denoise_threshold,

		led_brightness,

		x_focal_distance,
		y_focal_distance,

		zmotor_speed_factor,
		zmotor_speed_pow,
		zmotor_speed_max,

		xymotor_speed_factor,
		xymotor_speed_pow,
		xymotor_speed_max,

		zmotor_forward_distance,
		zmotor_stroke)
};

template<>
struct zmsg<mid_t::update_led_brightness> {
public:
	ledId_t  id;
	double   brightness;
public:
        ZMSG_PU(id,
		brightness)
};

template<>
struct zmsg<mid_t::update_window_position> {
	bool is_pos_x;
	int32_t row;
	int32_t column;
public:
	ZMSG_PU(is_pos_x,row,column)
};

template<>
struct zmsg<mid_t::update_cmos_focal_distance> {
public:
	double x_focal_distance;
	double y_focal_distance;
public:
	ZMSG_PU(x_focal_distance, y_focal_distance)
};

}
