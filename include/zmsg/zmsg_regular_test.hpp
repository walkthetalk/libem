#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::regular_test_start> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::motor_start> {
	motorId_t id;
	bool m_forward_dir;
public:
	ZMSG_PU(id, m_forward_dir)
};

template<>
struct zmsg<mid_t::motor_stop> {
	motorId_t id;
public:
	ZMSG_PU(id)
};

template<>
struct zmsg<mid_t::discharge_test_start>{
	uint16_t magnitude;
public:
	ZMSG_PU(magnitude)
};

template<>
struct zmsg<mid_t::discharge_test_stop>{
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::image_move>{
	bool is_pos_x;
	int16_t row;
	int16_t column;
public:
	ZMSG_PU(is_pos_x,row,column)
};

} /* msg */
