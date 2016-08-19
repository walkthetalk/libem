#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template <>
struct msg<mid_t::fiber_reco_result> {
public:
	fiber_rec_info_t lft_rec_info;
	fiber_rec_info_t rt_rec_info;
public:
	ZMSG_PU(lft_rec_info, rt_rec_info)
};

}

