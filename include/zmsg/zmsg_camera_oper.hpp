#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::image_move>{
	bool is_pos_x;
	int16_t row;
	int16_t column;
public:
	ZMSG_PU(is_pos_x,row,column)
};

} /* msg */
