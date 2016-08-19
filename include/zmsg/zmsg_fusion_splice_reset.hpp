#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::fusion_splice_reset> {
public:
	ZMSG_PU()
};

}
