#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::discharge>{
	double   magnitude;	/// unit: volt
	uint16_t time;	/// unit: ms
public:
	ZMSG_PU(magnitude, time)
};

template<>
struct msg<mid_t::stop_discharge> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::discharge_count>{
	uint32_t discharge_count;
public:
	ZMSG_PU(discharge_count)
};

} /* msg */
