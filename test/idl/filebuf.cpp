#include "filebuf.hpp"

static const char * s_converter_header = R"convertheader(
#include <stdexcept>
#include <cstring>

#include "rapidjson/writer.h"

#include "c2json.hpp"
#include "json2c.hpp"

#include "in_out_wrapper.hpp"

static constexpr std::size_t msg_header_size = 8;

#include "jmsg/jmsg_types.hpp"
#include "jmsg/jmsg_sender.hpp"
#include "jmsg/jmsg_rcver.hpp"

template<typename T, typename V>
rapidjson::Value search_name_directly(const T &flist, const V val)
{
	if (0 <= val && val < std::extent<T>::value) {
		rapidjson::Value::StringRefType str(flist[val].name);
		return rapidjson::Value(str);
	}

	throw std::invalid_argument("enum val error!");
}

template<typename T, typename V>
rapidjson::Value search_name_binary(const T &flist, const V val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		V cur = flist[mid].val;
		if (cur == val) {
			rapidjson::Value::StringRefType str(flist[mid].name);
			return  rapidjson::Value(str);
		}
		else if (cur < val) {
			head = mid;
		}
		else {
			tail = mid;
		}
	};

	throw std::invalid_argument("enum val error!");
}

template<typename T>
auto search_val_binary(const T & flist, const rapidjson::Value & name) -> decltype(flist[0].val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		const rapidjson::Value::StringRefType & cur = flist[mid].name;
		int cmp_result = std::strcmp(cur.s, name.GetString());
		if (cmp_result == 0) {
			return flist[mid].val;
		}
		else if (cmp_result < 0) {
			head = mid;
		}
		else {
			tail = mid;
		}
	};

	throw std::invalid_argument("enum name error!");
}

template<typename T>
auto flag2e(const T & flist, const rapidjson::Value & name_array) -> decltype(flist[0].val)
{
	RAPIDJSON_ASSERT(name_array.IsArray());
	decltype(flist[0].val) ret = 0;

	for (const auto & name : name_array.GetArray()) {
		for (const auto & i : flist) {
			if (name == i.name) {
				ret |= i.val;
				break;
			}
		}
	}
	return ret;
}

template<typename T, typename V>
rapidjson::Value e2flag(rapidjson::Document & doc, const T &flist, const V val)
{
	const decltype(flist[0].val) dval = val;
	rapidjson::Value ret(rapidjson::kArrayType);

	for (const auto & i : flist) {
		if ((dval & i.val) == i.val) {
			ret.PushBack(i.name, doc.GetAllocator());
		}
	}
	return ret;
}
)convertheader";

static const char * s_jmsg_types_hpp_header = R"jmsgtypes(#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

template<typename T>
struct enum_info { };

)jmsgtypes";

static const char * s_msg_id_header = R"msgidheader(#pragma once

#include "jmsg_types.hpp"

#define DCL_MSG(_x) msg_helper<mid_t::_x>::value_type

)msgidheader";

static const char * s_utils_hpp_header = R"msgheader(#pragma once
#include <map>
#include <string>
#include <functional>

#include "jmsg_id.hpp"

)msgheader";

filebuf s_outf_types(0x100000, IDLINC "jmsg_types.hpp", s_jmsg_types_hpp_header);
filebuf s_outf_converter(0x100000, IDLSRC "jmsg_convert.cpp", s_converter_header);
filebuf s_outf_id(0x100000, IDLINC "jmsg_id.hpp", s_msg_id_header);
filebuf s_outf_sender(0x100000, IDLINC "jmsg_sender.hpp", s_utils_hpp_header);
filebuf s_outf_rcver(0x100000, IDLINC "jmsg_rcver.hpp", s_utils_hpp_header);
