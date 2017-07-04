#pragma once

#include "jmsg_cmm.hpp"

// 1. bool / arithmetic
template<typename _T, typename std::enable_if<
			   std::is_same<_T, bool>::value
			|| std::is_same<_T, float>::value
			|| std::is_same<_T, double>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	dst = src.Get<_T>();
}

template<typename _T, typename std::enable_if<
			   std::is_same<_T, int8_t>::value
			|| std::is_same<_T, int16_t>::value
			|| std::is_same<_T, int32_t>::value
			|| std::is_same<_T, int64_t>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	dst = (_T)src.Get<int64_t>();
}

template<typename _T, typename std::enable_if<
			   std::is_same<_T, uint8_t>::value
			|| std::is_same<_T, uint16_t>::value
			|| std::is_same<_T, uint32_t>::value
			|| std::is_same<_T, uint64_t>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	dst = (_T)src.Get<uint64_t>();
}

// 2. std::string
inline void json2c(std::string & dst, const rapidjson::Value & src)
{
	dst = src.GetString();
}

/// 2. raw array
template<typename _T, typename std::enable_if<
			std::is_array<_T>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	RAPIDJSON_ASSERT(src.Size() == (std::extent<_T, 0>::value));
	for (rapidjson::SizeType i = 0; i < std::extent<_T, 0>::value; ++i) {
		json2c(dst[i], src[i]);
	}
}

/// 2. std array
template<typename _T, typename std::enable_if<
			is_std_array<_T>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	RAPIDJSON_ASSERT(src.Size() == dst.size());
	for (rapidjson::SizeType i = 0; i < src.Size(); ++i) {
		json2c(dst[i], src[i]);
	}
}

/// 3. vector
template<typename _T, typename std::enable_if<
			is_std_vector<_T>::value
, bool>::type = false>
inline void json2c(_T & dst, const rapidjson::Value & src)
{
	dst.resize(src.Size());
	for (rapidjson::SizeType i = 0; i < src.Size(); ++i) {
		json2c(dst[i], src[i]);
	}
}

/**
 * encode member
 */
#define DEC_MEM(m_name, src, dst) json2c(dst, src[m_name]);
