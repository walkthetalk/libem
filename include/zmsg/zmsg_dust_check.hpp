#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct zmsg<mid_t::dust_check_start> {
public:
	ZMSG_PU()
};

template<>
struct zmsg<mid_t::dust_check_result> {
	fs_err_t code;

	zmsg<mid_t::dust_check_start> z_cfg;

	/// \note following member are valid only if 'code' is 'SUCCESS'!
	bool xz_ok;
	bool_img xz;
	bool yz_ok;
	bool_img yz;
public:
	ZMSG_PU(code, z_cfg, xz_ok, xz, yz_ok, yz)
};

} /* msg */
