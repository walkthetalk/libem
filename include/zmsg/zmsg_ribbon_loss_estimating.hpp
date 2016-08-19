#pragma once

#include "zmsg_types"

namespace zmsg {

template<>
struct msg<mid_t::ribbon_loss_estimating_result> {
	std::vector<bool> valid;
	std::vector<double> loss_data;
public:
	ZMSG_PU(valid, loss_data)
};

}