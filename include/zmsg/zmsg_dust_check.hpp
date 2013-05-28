#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::dust_check_start> {
public:
	ZMSG_PU()
};

struct bool_img {
	uint16_t width;
	uint16_t height;
	std::vector<bool> data;
public:
	ZMSG_PU(width, height, data);
};

template<>
struct zmsg<mid_t::dust_check_result> {
	bool_img xz;
	bool_img yz;
public:
	ZMSG_PU(xz, yz)
};

} /* msg */
