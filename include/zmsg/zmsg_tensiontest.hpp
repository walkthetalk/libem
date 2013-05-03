#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::tension_test_start> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::tension_test_stop> {
public:
	ZMSG_PU()
};


}

