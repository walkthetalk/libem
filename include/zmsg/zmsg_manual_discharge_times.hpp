#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct zmsg<mid_t::manual_discharge_times> {
public:
	int times;
public:
	ZMSG_PU(
		times)
};

}
