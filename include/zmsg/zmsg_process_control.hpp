#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg{

template<>
struct zmsg<mid_t::go_on> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::skip> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::stop> {
public:
	ZMSG_PU()
};

}

