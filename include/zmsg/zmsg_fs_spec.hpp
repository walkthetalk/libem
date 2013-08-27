#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::set_fs_spec> {
public:
	uint16_t window_x_row;			// unit: pixel
	uint16_t window_x_col;			// unit: pixel
	uint16_t window_y_row;			// unit: pixel
	uint16_t window_y_col;			// unit: pixel

	uint32_t nm_per_pixel;			// unit: nm/pixel
	uint32_t nm_per_step;			// unit: nm/step

	uint16_t img_cap_delay;			// unit: ms

	uint16_t clr_discharge_strength; 	// unit:  bit
	uint32_t clr_discharge_gap;		// unit: nm
	uint16_t check_fiber_exist_time;	// unit: ms

	uint16_t motor_min_speed[motorId_t::NUM];
	uint16_t motor_max_speed[motorId_t::NUM];

	uint16_t motor_lzrz_fs_speed;
	uint32_t motor_xy_precise_calibrate_speed;

	uint16_t motor_xy_steps_per_pixel;	// unit: step/pixel

	double   fiber_outline_blacklevel;		// 0.0 ~ 1.0
	double   calibrating_xy_dist_threshold;		// unit: pixel
	double   precise_calibrating_xy_dist_threshold;	// unit: pixel
	double   z_dist_threshold;			// unit: pixel

	discharge_data_t discharge_base;
	discharge_data_t discharge_revise;

	double   dust_check_threshold0;
	double   dust_check_threshold1;

	double   led_brightness[ledId_t::LED_NUM];
public:
        ZMSG_PU(
		window_x_row,
		window_x_col,
		window_y_row,
		window_y_col,

		nm_per_pixel,
		nm_per_step,

		img_cap_delay,

		clr_discharge_strength,
		clr_discharge_gap,
		check_fiber_exist_time,

		motor_min_speed,
		motor_max_speed,

		motor_lzrz_fs_speed,
		motor_xy_precise_calibrate_speed,

		motor_xy_steps_per_pixel,

		fiber_outline_blacklevel,
		calibrating_xy_dist_threshold,
		precise_calibrating_xy_dist_threshold,
		z_dist_threshold,

		discharge_base,
		discharge_revise,

		dust_check_threshold0,
		dust_check_threshold1,

		led_brightness)
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
	uint16_t row;
	uint16_t column;
public:
	ZMSG_PU(is_pos_x,row,column)
};

}
