#pragma once

#include "zmsg_cmm.hpp"
#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::fusion_splice_reset> {
public:
	ZMSG_PU()
};

}
