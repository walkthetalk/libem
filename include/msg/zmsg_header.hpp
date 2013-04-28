#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {

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
