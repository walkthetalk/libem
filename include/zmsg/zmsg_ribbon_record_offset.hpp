#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::ribbon_record_off_set> {
public:
	std::vector<double> core_diff_pre;
	std::vector<double> cladding_diff_pre;
	std::vector<double> vertex_intersect_angle;
public:
	ZMSG_PU(
		core_diff_pre,
		cladding_diff_pre,
		vertex_intersect_angle)
};

}