#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::svc_state> {
	zmsg(svc_state_t s)
	: sstate(s)
	{
	}
public:
	svc_state_t sstate;
public:
	ZMSG_PU(sstate)
};

}
