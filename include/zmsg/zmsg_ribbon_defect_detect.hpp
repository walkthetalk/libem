#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::ribbon_get_defect_info> {
public:
	ZMSG_PU()
};

template<.
struct zmsg<mid_t::ribbon_defect_detect_result> {
	std::vector<img_defects_t> data;
public:
	ZMSG_PU(data)
};

}
  