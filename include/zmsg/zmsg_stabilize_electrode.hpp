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
	se_err_t code;

	zmsg<mid_t::stabilize_electrode_start> z_cfg;

	uint32_t number;
public:
	ZMSG_PU(code, z_cfg, number)
};

}

