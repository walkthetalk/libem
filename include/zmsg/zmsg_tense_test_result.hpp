#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct zmsg<mid_t::tense_test_result> {
public:
	bool is_success; // 1 is sucess;
public:
	ZMSG_PU(
		is_success)
};

}

