#pragma once

#include <cstdint>

namespace zmsg {

/*
* \brief message identifier
*/
enum class mid_t : uint16_t {
	nil,	/// occupy symbol, correspond to \em NULL.
	test1,
	test2,

	fs_state,
	heat_state,

	alarm,

	fusion_splice_reset,

	heat_start,
	fusion_splice_start,
	discharge_adjust_start,
	regular_test_start,
	go_on,
	stop,

	motor_start,
	motor_stop,

	discharge,

	image_move,
	/// \todo add new message id
	max,

};

template< mid_t mid >
struct zmsg;	/// not implemented.

/**
 * \brief zmsg pack/unpack macro
 */
#define ZMSG_PU(...) \
	template< typename _T > \
	void serialize(_T & o) const \
	{ \
		o.template operator()(__VA_ARGS__); \
	} \
	template< typename _T > \
	void serialize(_T & o) \
	{ \
		o.template operator()(__VA_ARGS__); \
	}

}/* namespace zmsg */

#define DCL_ZMSG(mid) zmsg::zmsg<zmsg::mid_t::mid>
