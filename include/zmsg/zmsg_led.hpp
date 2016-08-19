#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::set_led> {
public:
        ledId_t	id;
        double	brightness;	/// 0~1		0: black	1: white
public:
        ZMSG_PU(id, brightness)
};

}
