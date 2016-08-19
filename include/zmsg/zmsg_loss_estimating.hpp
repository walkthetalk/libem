#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::loss_estimating_result> {
	bool valid; //"valid == 1" means that "loss_data" is valid
	double loss_data;
public:
	ZMSG_PU(valid,loss_data)
};

} /* msg */
