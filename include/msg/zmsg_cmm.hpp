#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <stdio.h>

namespace zmsg {


/**
 * \brief data types
 */
enum class id_t : uint8_t {
	BOOL = std::numeric_limits<bool>::digits,
	ARRAY = 2,
	INT8 = std::numeric_limits<int8_t>::digits,
	UINT8 = std::numeric_limits<uint8_t>::digits,
	INT16 = std::numeric_limits<int16_t>::digits,
	UINT16 = std::numeric_limits<uint16_t>::digits,
	INT32 = std::numeric_limits<int32_t>::digits,
	UINT32 = std::numeric_limits<uint32_t>::digits,
	INT64 = std::numeric_limits<int64_t>::digits,
	UINT64 = std::numeric_limits<uint64_t>::digits,
	FLOAT = std::numeric_limits<float>::digits,
	DOUBLE = std::numeric_limits<double>::digits,
};


/**
 * \brief id_of
 * \note the std::enable_if of the second parameter for some type specialization can't hold
 * the second arg.
 * \note if you pass id_of::value to some function, the type of arg can't be a reference.
 * 'constexpr' is a compile-time value.
 */
template< typename _T, typename _enable = void >
struct id_of;	/// undefined

template<typename _T>
struct id_of< _T, typename std::enable_if<
	std::is_arithmetic<_T>::value>::type > {

	static constexpr id_t value = static_cast<id_t>(std::numeric_limits<_T>::digits);
};

template<typename _T>
struct id_of< _T, typename std::enable_if<
	std::is_array<_T>::value>::type > {

	static constexpr id_t value = id_t::ARRAY;
};

typedef uint8_t array_rank_t;
typedef uint16_t array_extent_t;

typedef uint16_t msg_len_t;

static constexpr size_t s_def_buf_size = std::numeric_limits<msg_len_t>::max() + 1;


/*
* \brief message identifier
*/
enum class mid_t : uint16_t {
	nil,	/// occupy symbol, correspond to \em NULL.
	test1,
	test2,
	svc_state,
	alarm,
	heat_start,
	fs,
	fusion_splice_start,
	discharge_adjust_start,
	regular_test_start,
	go_on,
	stop,

	motor_start,
	motor_stop,

	discharge_test_start,
	discharge_test_stop,

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
