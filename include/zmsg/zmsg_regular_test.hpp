#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::regular_test_start> {
public:
	ZMSG_PU()
};

} /* msg */
