#pragma once

#include "zmsg_types.hpp"
#include "zmsg_ribbon_record_offset.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::ribbon_shrinkage_test_start> {
};

template<>
struct zmsg<mid_t::ribbon_shrinkage_test_result> {
};

}