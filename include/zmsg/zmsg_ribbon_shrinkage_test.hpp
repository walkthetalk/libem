#pragma once

#include "zmsg_types.hpp"
#include "zmsg_ribbon_record_offset.hpp"

namespace zmsg {

template<>
struct msg<mid_t::ribbon_shrinkage_test_start> {
};

template<>
struct msg<mid_t::ribbon_shrinkage_test_result> {
};

}