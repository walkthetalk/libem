#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::fs_state> {
public:
	svc_fs_state_t sstate;
public:
	ZMSG_PU(sstate)
};

template<>
struct zmsg<mid_t::heat_state> {
public:
	svc_heat_state_t sstate;
public:
	ZMSG_PU(sstate)
};

}
