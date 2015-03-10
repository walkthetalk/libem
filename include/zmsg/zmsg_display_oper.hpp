#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::set_fs_display_mode>{
	fs_display_mode_t mode;
public:
	ZMSG_PU(mode)
};

template<>
struct zmsg<mid_t::set_fs_display_mode_ext>{
	bool order_xtoy;

	uint16_t x_left;
	uint16_t x_up;
	uint16_t x_width;
	uint16_t x_height;

	uint16_t y_left;
	uint16_t y_up;
	uint16_t y_width;
	uint16_t y_height;
public:
	ZMSG_PU(order_xtoy,
		x_left, x_up, x_width, x_height,
		y_left, y_up, y_width, y_height)
};

} /* msg */

