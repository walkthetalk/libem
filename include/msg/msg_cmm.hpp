#pragma once
/**
 * \file	msg/msg_cmm.hpp
 * \author	Yi Qingliang <niqingliang2003@tom.com>
 */
#include <cstdint>
#include <msgpack.hpp>

namespace msg {
/**
 * \brief message identifier
 */
enum class mid_t : uint8_t {
	nil,	/// occupy symbol, correspond to \em NULL.
	test1,
	test2,
	/// \todo add new message id
};

/**
 * \class msg
 * \brief message body
 * \param id	message id
 * \note you should specialize it for specific message id.
 */
template<mid_t id = mid_t::nil>
class msg {
};

}
