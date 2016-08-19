#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::get_fiber_defect_info> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::defect_detect_result> {
	img_defects_t data;
public:
	ZMSG_PU(data)
};

} /* msg */
