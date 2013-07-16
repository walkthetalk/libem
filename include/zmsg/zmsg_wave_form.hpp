#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::query_wave_form> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::report_wave_form> {
	std::vector<uint8_t> x_wave;
	std::vector<uint8_t> y_wave;
public:
	ZMSG_PU(x_wave, y_wave)
};

} /* msg */
