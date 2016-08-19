#pragma once

#include "zmsg_types.hpp"

namespace zmsg {

template<>
struct msg<mid_t::dust_check_start> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::dust_check1_start> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::dust_check_result> {
	fs_err_t code;

	msg<mid_t::dust_check_start> z_cfg;

	/// \note following member are valid only if 'code' is 'SUCCESS'!
	bool xz_ok;
	std::string xz_ori;
	std::string xz_dust;
	bool yz_ok;
	std::string yz_ori;
	std::string yz_dust;
public:
	ZMSG_PU(code, z_cfg,
		xz_ok, xz_ori, xz_dust,
		yz_ok, yz_ori, yz_dust)
};

template<>
struct msg<mid_t::full_dust_check_start> {
public:
	ZMSG_PU()
};

template<>
struct msg<mid_t::full_dust_check_result> {
	fs_err_t code;

	msg<mid_t::full_dust_check_start> z_cfg;

	/// \note following member are valid only if 'code' is 'SUCCESS'!
	bool xz_ok;
	std::string xz_ori;
	std::string xz_dust;
	bool yz_ok;
	std::string yz_ori;
	std::string yz_dust;
public:
	ZMSG_PU(code, z_cfg,
		xz_ok, xz_ori, xz_dust,
		yz_ok, yz_ori, yz_dust)
};

} /* msg */
