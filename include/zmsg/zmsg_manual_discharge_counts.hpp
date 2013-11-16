#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct zmsg<mid_t::manual_discharge_counts> {
public:
	int counts;
public:
	ZMSG_PU(
		counts)
};

}
