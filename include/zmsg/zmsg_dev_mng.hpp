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
	double pressure;	/// bar
	double humidity;
	double env_temp;	/// degree Celsius
	double int_temp;	/// internal temperature

	double heat_temp;

	double bat_voltage;
public:
	ZMSG_PU(pressure,
		humidity,
		env_temp,
		int_temp,
		heat_temp,
		bat_voltage)
};

} /* msg */

