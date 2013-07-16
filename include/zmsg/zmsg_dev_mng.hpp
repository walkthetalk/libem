#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::query_dev_state> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::report_dev_state> {
	DOUBLE pressure;	/// kpa
	DOUBLE humidity;
	DOUBLE env_temp;	/// degree Celsius
	DOUBLE int_temp;	/// internal temperature

	DOUBLE heat_temp;

	DOUBLE bat_voltage;
public:
	ZMSG_PU(pressure,
		humidity,
		env_temp,
		int_temp,
		heat_temp,
		bat_voltage)
};

} /* msg */

