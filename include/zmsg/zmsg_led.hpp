#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::set_led> {
public:
        ledId_t	id;
        DOUBLE	brightness;	/// 0~1		0: black	1: white
public:
        ZMSG_PU(id, brightness)
};

}
