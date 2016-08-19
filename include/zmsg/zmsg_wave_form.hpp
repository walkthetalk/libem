#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::query_wave_form> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::report_wave_form> {
	std::string x_wave;
	std::string y_wave;
public:
	ZMSG_PU(x_wave, y_wave)
};

} /* msg */
