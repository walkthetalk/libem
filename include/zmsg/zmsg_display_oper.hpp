#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::set_fs_display_mode>{
	fs_display_mode_t mode;
public:
	ZMSG_PU(is_pos_x,row,column)
};

} /* msg */

