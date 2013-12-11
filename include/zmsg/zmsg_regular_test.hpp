#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::regular_test_start> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::regular_test_result> {
	fs_err_t code;

	zmsg<mid_t::regular_test_start> z_cfg;
public:
	ZMSG_PU(code, z_cfg)
};

} /* msg */
