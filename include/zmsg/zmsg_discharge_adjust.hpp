#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::discharge_adjust_start> {
public:
	ZMSG_PU()
};

}

