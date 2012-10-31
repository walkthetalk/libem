#pragma once

#include <cstdint>
#include <msgpack.hpp>

namespace msg {

enum class mid_t : uint8_t {
	nil,
	test1,
	test2,
};

template<mid_t id>
class msg {
};

}
