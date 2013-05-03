#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg{

template <>
struct zmsg<mid_t::alarm>{
public:
	ZMSG_PU()
};

}
