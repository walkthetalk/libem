#pragma once

#include "jmsg_cmm.hpp"

// 1. bool / arithmetic
template<typename _T, typename std::enable_if<
			   std::is_same<_T, bool>::value
			|| std::is_same<_T, float>::value
			|| std::is_same<_T, double>::value
, bool>::type = false>
inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const _T src)
{
	return rapidjson::Value(src);
}

template<typename _T, typename std::enable_if<
			   std::is_same<_T, int8_t>::value
			|| std::is_same<_T, int16_t>::value
			|| std::is_same<_T, int32_t>::value
			|| std::is_same<_T, int64_t>::value
, bool>::type = false>
inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const _T src)
{
	return rapidjson::Value((int64_t)src);
}

template<typename _T, typename std::enable_if<
			   std::is_same<_T, uint8_t>::value
			|| std::is_same<_T, uint16_t>::value
			|| std::is_same<_T, uint32_t>::value
			|| std::is_same<_T, uint64_t>::value
, bool>::type = false>
inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const _T src)
{
	return rapidjson::Value((uint64_t)src);
}

/// 2. std::string
inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const std::string & src)
{
	return rapidjson::Value(rapidjson::StringRef(src.c_str()));
}

/// 3. vector/array/[]
template<typename _T, typename std::enable_if<
			   is_std_vector<_T>::value
			|| is_std_array<_T>::value
			|| std::is_array<_T>::value
, bool>::type = false>
inline rapidjson::Value c2json(rapidjson::Document & jd, const _T & src)
{
	rapidjson::Value obj(rapidjson::kArrayType);
	for (auto & ele : src) {
		obj.PushBack(c2json(jd, ele).Move(), jd.GetAllocator());
	}
	return obj;
}


/**
 * encode member
 */
#define ENC_MEM(jd, m_name, jobj, src) jobj.AddMember(rapidjson::Value::StringRefType(m_name), c2json(jd, src).Move(), jd.GetAllocator());
#define ENC_MEM_IF(jd, m_name, jobj, src, dep) if (dep) ENC_MEM(jd, m_name, jobj, src)
