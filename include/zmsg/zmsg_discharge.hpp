#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge>{
	uint16_t magnitude;
public:
	ZMSG_PU(magnitude)
};

} /* msg */
