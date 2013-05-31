#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::stabilize_electrode_start> {
public:
	uint16_t magnitude;
	uint32_t time;	/// unit: ms
	uint32_t number;
public:
	ZMSG_PU(magnitude, time, number)
};

template<>
struct zmsg<mid_t::stabilize_electrode_result> {
	stabilize_electrode_err_t code;
public:
	ZMSG_PU(code)
};

}

