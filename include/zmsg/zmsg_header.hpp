#pragma once

#include <limits>
#include <type_traits>
#include <array>
#include <vector>

#include "zmsg_cmm.hpp"

namespace zmsg {

/**
 * \brief data types
 */
enum class id_t : uint8_t {
	BOOL = std::numeric_limits<bool>::digits,
	ARRAY = 2,
	FIX_SIZE_CONTAINER = 3,
	DYNAMIC_SIZE_CONTAINER = 4,
	UDT = 5,
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
 * \brief helper class
 */
template <typename T> struct is_std_vector { static constexpr bool value = false; };
template <typename T>
struct is_std_vector< std::vector<T> > { static constexpr bool value = true; };

template <typename T> struct is_std_array { static constexpr bool value = false; };
template <typename T, std::size_t size>
struct is_std_array< std::array<T, size> > { static constexpr bool value = true; };

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

template<typename _T>
struct id_of< _T, typename std::enable_if<
	is_std_array<_T>::value>::type > {

	static constexpr id_t value = id_t::FIX_SIZE_CONTAINER;
};

template<typename _T>
struct id_of< _T, typename std::enable_if<
	is_std_vector<_T>::value>::type > {

	static constexpr id_t value = id_t::DYNAMIC_SIZE_CONTAINER;
};

template<typename _T>
struct id_of< _T, typename std::enable_if<
	    std::is_class<_T>::value
	&& !is_std_vector<_T>::value
	&& !is_std_array<_T>::value>::type > {

	static constexpr id_t value = id_t::UDT;
};

typedef uint8_t array_rank_t;
typedef uint16_t array_extent_t;

typedef uint16_t ele_num_t;

typedef uint16_t msg_len_t;

static constexpr msg_len_t ZMSG_MAX_LEN = std::numeric_limits<msg_len_t>::max();
static constexpr size_t s_def_buf_size = ZMSG_MAX_LEN + 1;


/// \todo
static constexpr bool IS_LE = true;

struct zmsg_header {
	uint8_t flag;
	uint8_t version;
	uint16_t magic;
	msg_len_t len;
	uint16_t crc;
	mid_t mid;
public:
	/**
	 * \note no 'flag'
	 */
	ZMSG_PU(version, magic, len, crc, mid)
};

constexpr std::underlying_type<mid_t>::type to_val(const mid_t mid)
{
	return (std::underlying_type<mid_t>::type)(mid);
}

constexpr size_t HDR_SIZE = sizeof(zmsg_header);

} /* zmsg */
