#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::set_lcd_brightness> {
public:
	/// \brief the brightness of lcd, valid range: [0.0 ~ 1.0]
	double   brightness;
public:
        ZMSG_PU(brightness)
};

}
