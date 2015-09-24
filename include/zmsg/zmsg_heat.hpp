#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::heat_start> {
public:
        uint32_t Material;
        shrink_tube_t Fiberlen;
        bool Heatctl;
        uint16_t heat_time;	/// unit: second
        int16_t heat_temp;	/// unit: degree Celsius
        int16_t finish_temp;	/// unit: degree Celsius
        int16_t stable_temp;	/// unit: degree Celsius
        bool fast_heat;
public:
        ZMSG_PU(Material,Fiberlen,Heatctl,heat_time,heat_temp,finish_temp,stable_temp,fast_heat)
};

template<>
struct zmsg<mid_t::heat_result> {
	fs_err_t code;
public:
	ZMSG_PU(code)
};

}


