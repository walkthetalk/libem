#pragma once

#include "msg/msg_cmm.hpp"

namespace msg {

/**
 * \brief specialize msg for \em mid_t::test1.
 */
template<>
class msg<mid_t::test1> {
public:
	msg() : a(0), b(0) {}
public:
	uint8_t a;
	uint32_t b;
public:
	MSGPACK_DEFINE(a, b)
};

}
