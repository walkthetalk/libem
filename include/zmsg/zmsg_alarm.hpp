#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg{

template <>
struct msg<mid_t::alarm>{
public:
	ZMSG_PU()
};

}
