#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::heat_start> {
public:
        UINT32 Material;
        UINT32 Fiberlen;
        BOOL Heatctl;
        UINT8 heat_time;	/// unit: second
        INT16 heat_temp;	/// unit: degree Celsius
        INT16 finish_temp;	/// unit: degree Celsius
public:
        ZMSG_PU(Material,Fiberlen,Heatctl,heat_time,heat_temp,finish_temp)
};

template<>
struct zmsg<mid_t::heat_result> {
	heat_err_t code;
public:
	ZMSG_PU(code)
};

}


