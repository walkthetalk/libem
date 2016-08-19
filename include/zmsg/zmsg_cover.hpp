#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct msg<mid_t::fs_cover_openned> {
public:
	ZMSG_PU()
};

template <>
struct msg<mid_t::fs_cover_state> {
	bool is_openned;
public:
	ZMSG_PU(is_openned)
};

}
