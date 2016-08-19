#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg{

template<>
struct msg<mid_t::go_on> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::skip> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::stop> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::process_progress> {
	double progress;
public:
	ZMSG_PU(progress)
};

}

