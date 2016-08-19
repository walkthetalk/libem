#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

/**
 * \brief arc strength revise value
 * 
 */
template <>
struct msg<mid_t::arc_revise> {
	double revise;
public:
	ZMSG_PU(revise)
};

} /* namespace zmsg*/