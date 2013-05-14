#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge>{
	uint16_t magnitude;
	uint16_t time;	/// unit: ms
public:
	ZMSG_PU(magnitude, time)
};

} /* msg */
