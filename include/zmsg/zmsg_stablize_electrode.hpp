#pragma once

#include <zmsg_types.hpp>

namespace zmsg{

template <>
struct zmsg<mid_t::stablize_electrode_start>{
public:
	ZMSG_PU()
};

template <>
struct zmsg<mid_t::stablize_electrode_stop>{
public:
	ZMSG_PU()
};

}
