#pragma once

#include <type_traits>

namespace zmsg {

/**
 * \brief mainly used for converting enum class value to underlying type
 */
template< typename T >
constexpr auto to_val(const T mid) -> typename std::underlying_type<T>::type
{
	return static_cast<typename std::underlying_type<T>::type>(mid);
}

}
