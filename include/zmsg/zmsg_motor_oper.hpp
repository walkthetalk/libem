#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::motor_start> {
	motorId_t id;
	bool m_forward_dir;
public:
	ZMSG_PU(id, m_forward_dir)
};

template<>
struct zmsg<mid_t::motor_stop> {
	motorId_t id;
public:
	ZMSG_PU(id)
};

} /* msg */

