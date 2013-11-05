#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct zmsg<mid_t::manual_focal_distance> {
public:
	bool is_pos_x; // 1 is x cmos, 0 is y cmos
	double   focal_distance;
public:
	ZMSG_PU(
		is_pos_x,
		focal_distance)
};

}
