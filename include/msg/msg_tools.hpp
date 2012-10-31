#pragma once 

#include <type_traits>

#include "msg/msg_cmm.hpp"

namespace msg {

class recver {
public:
	recver(size_t init_buf_size = MSGPACK_UNPACKER_INIT_BUFFER_SIZE)
	: m_unpacker(init_buf_size)
	{
	}

public:
	template<typename T>
	void fill_from(T & o, size_t (T::*f)(void *, size_t))
	{
		m_unpacker.reset();
		size_t r = (o.*f)(m_unpacker.buffer(), m_unpacker.buffer_capacity());
		m_unpacker.buffer_consumed(r);
	}

	template<typename T>
	void fill_from(T & o)
	{
		fill_from(o, &T::recv);
	}

	mid_t id(void)
	{
		mid_t mid = mid_t::nil;
		m_unpacker.next(&m_result);
		m_result.get().convert((std::underlying_type<mid_t>::type*)(&mid));
		return mid;
	}

	template<mid_t id>
	void convert(msg<id> & m)
	{
		m_unpacker.next(&m_result);
		m_result.get().convert(&m);
	}	
private:
	msgpack::unpacker m_unpacker;
	msgpack::unpacked m_result;
};

class sender {
public:
	sender() : m_buf(), m_packer(m_buf) {}
public:
	template<mid_t id, typename T>
	void fill_to(const msg<id> & m, T & o, size_t (T::*f)(const void *, size_t))
	{
		m_buf.clear();
		m_packer.pack((std::underlying_type<mid_t>::type)id);
		m_packer.pack(m);

		(o.*f)(m_buf.data(), m_buf.size());
	}

	template<mid_t id, typename T>
	void fill_to(const msg<id> & m, T & o)
	{
		fill_to(m, o, &T::send);
	}
private:
	msgpack::sbuffer m_buf;
	msgpack::packer<msgpack::sbuffer> m_packer;
};

}
