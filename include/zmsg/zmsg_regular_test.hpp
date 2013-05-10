#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::regular_test_start> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::discharge>{
	uint16_t magnitude;
public:
	ZMSG_PU(magnitude)
};

} /* msg */
