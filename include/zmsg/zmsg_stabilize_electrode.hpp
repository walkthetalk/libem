#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct msg<mid_t::stabilize_electrode_start> {
public:
	double   magnitude; /// unit: volt
	uint32_t time;	/// unit: ms
	uint32_t interval;	/// unit: ms
	uint32_t number;
public:
	ZMSG_PU(magnitude, time, interval, number)
};

template<>
struct msg<mid_t::stabilize_electrode_result> {
	fs_err_t code;

	msg<mid_t::stabilize_electrode_start> z_cfg;

	uint32_t number;
public:
	ZMSG_PU(code, z_cfg, number)
};

}

