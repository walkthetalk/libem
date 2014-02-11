#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct zmsg<mid_t::record_off_set> {
public:
	double core_diff_pre;		/// unit: um
	double cladding_diff_pre;	/// unit: um
	double vertex_intersect_angle;	/// unit: degree
public:
	ZMSG_PU(
		core_diff_pre,
		cladding_diff_pre,
		vertex_intersect_angle)
};

}
