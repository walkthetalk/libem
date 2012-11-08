#pragma once

#include "msg/msg_cmm.hpp"

namespace msg {

/**
 * \brief specialize msg for \em mid_t::test2.
 */
template<>
class msg<mid_t::test2> {
public:
	msg() : a(0), b(0), c(0) {}
public:
	uint32_t a;
	uint8_t b;
	uint16_t c;
public:
	MSGPACK_DEFINE(a, b, c)
};

}
