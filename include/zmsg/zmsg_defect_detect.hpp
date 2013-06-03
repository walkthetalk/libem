#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::defect_detect_result> {
	img_defects_t data;
public:
	ZMSG_PU(data)
};

} /* msg */
