#pragma once

#include <algorithm>
#include <stdexcept>

#include "zmsg_cmm.hpp"
#include "zmsg_header.hpp"

namespace zmsg {

class bad_type : public std::runtime_error {
public:
#if 0	/// \note it is only implemented in gcc >=4.8
	using std::runtime_error::runtime_error;
#else
	explicit bad_type( const std::string& what_arg )
	: runtime_error(what_arg)
	{
	}

	explicit bad_type( const char* what_arg )
	: runtime_error(what_arg)
	{
	}
#endif
};

class read_fail : public std::runtime_error {
public:
#if 0	/// \note it is only implemented in gcc >=4.8
	using std::runtime_error::runtime_error;
#else
	explicit read_fail( const std::string& what_arg )
	: runtime_error(what_arg)
	{
	}

	explicit read_fail( const char* what_arg )
	: runtime_error(what_arg)
	{
	}
#endif
};

class unpacker_base final {
public:
	unpacker_base()
	: m_buf_end(m_buf + sizeof(m_buf))
	, m_d(m_buf)
	, m_e(m_buf)
	{
	}

public:
	template< typename _T >
	void print(_T & o) const
	{
		o << (uint64_t)m_d << ":  ";
		const uint8_t * p = m_d;
		while (p < m_e) {
			o << (uint16_t)(*p++) << " ";
		}
		o << "\n";
	}

	void reset(void)
	{
		m_d = m_buf;
		m_e = m_buf;
	}

	template< typename _T >
	void read_from(_T  & o, size_t (_T::*f)(void *, size_t), size_t l)
	{
		size_t idle_size = static_cast<size_t>(m_buf_end - m_e);
		if (idle_size < l) {
			throw std::bad_alloc();
		}

		size_t r = (o.*f)(m_e, l);
		if (r != l) {
			throw read_fail("read fail");
		}

		m_e += l;
	}

	void read_from(std::function<size_t (void *, size_t)> & f, size_t l)
	{
		size_t idle_size = static_cast<size_t>(m_buf_end - m_e);
		if (idle_size < l) {
			throw std::bad_alloc();
		}

		size_t r = f(m_e, l);
		if (r != l) {
			throw read_fail("read fail");
		}

		m_e += l;
	}
public:
	template< bool _rev, typename _T >
	void fill(_T & v)
	{
		this->__fill<_rev>((uint8_t *)&v, sizeof(v));
	}
private:
	template< bool _rev, typename std::enable_if<
		!_rev, bool>::type = false >
	void __fill(uint8_t * dst, size_t l)
	{
		__check_err(l);
		std::copy_n(m_d, l, dst);
		m_d += l;
	}

	template< bool _rev, typename std::enable_if<
		_rev, bool>::type = false >
	void __fill(uint8_t * dst, size_t l)
	{
		__check_err(l);
		std::reverse_copy(m_d, m_d+l, dst);
		m_d += l;
	}
private:
	void __check_err(size_t l)
	{
		if (m_d + l > m_e) {
			throw std::bad_alloc();
		}
	}
private:
	uint8_t m_buf[s_def_buf_size];
	uint8_t * const m_buf_end;
	uint8_t * m_d;
	uint8_t * m_e;
};

template< typename _helper_t, bool _reverse >
class unpacker {
public:
	unpacker(_helper_t & helper, unpacker_base & base)
	: m_base(base)
	, m_helper(helper)
	{
	}
public:
	template< typename _T>
	void unpack_d(_T & v)
	{
		this->unpack_data(v);
	}
	template< typename _T >
	void unpack_t(_T & v)
	{
		this->unpack_type(v);
	}
	template< typename _T >
	void unpack_td(_T & v)
	{
		this->unpack_type(v);
		this->unpack_data(v);
	}

	template< typename _T, typename ... _args_t>
	void unpack_d(_T & v, _args_t & ... args)
	{
		this->unpack_d(v);
		this->unpack_d(args...);
	}
	template< typename _T, typename ... _args_t >
	void unpack_t(_T & v, _args_t & ... args)
	{
		this->unpack_t(v);
		this->unpack_t(args...);
	}
	template< typename _T, typename ... _args_t >
	void unpack_td(_T & v, _args_t & ... args)
	{
		this->unpack_td(v);
		this->unpack_td(args...);
	}

	void unpack_d()
	{
	}
	void unpack_t()
	{
	}
	void unpack_td()
	{
	}
public:
	/**
	 * signed/unsigned int8/int16/int32/int64 float double
	 */
	template< typename _T, typename std::enable_if<
		   std::is_arithmetic<_T>::value
		&& !std::is_same<_T, bool>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		m_base.template fill<_reverse>(v);
	}

	template< typename _T, typename std::enable_if<
		   std::is_arithmetic<_T>::value
		&& !std::is_same<_T, bool>::value, bool>::type = false >
	void unpack_type(_T const &)
	{
		this->__unpack_type_base(id_of<_T>::value);
	}

	/**
	 * bool
	 */
	template< typename _T, typename std::enable_if<
		   std::is_same<_T, bool>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		uint8_t d;
		this->unpack_data(d);
		v = (d ? true : false);
	}

	template< typename _T, typename std::enable_if<
		   std::is_same<_T, bool>::value, bool>::type = false >
	void unpack_type(_T const &)
	{
		this->__unpack_type_base(id_of<_T>::value);
	}

	/**
	 * enum
	 */
	template< typename _T, typename std::enable_if<
		std::is_enum<_T>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		this->unpack_data(
			reinterpret_cast< typename std::underlying_type<_T>::type &>(v));
	}

	template< typename _T, typename std::enable_if<
		std::is_enum<_T>::value, bool>::type = false >
	void unpack_type(_T const &)
	{
		this->__unpack_type_base(
			id_of< typename std::underlying_type<_T>::type >::value);
	}

	/**
	 * array
	 */
	template< typename _T, typename std::enable_if<
		   std::is_array<_T>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		for (size_t i = 0; i < std::extent<_T>::value; ++i) {
			this->unpack_data <typename std::remove_extent<_T>::type>(v[i]);
		}
	}

	template< typename _T, typename std::enable_if<
		   std::is_array<_T>::value, bool>::type = false >
	void unpack_type(_T const &)
	{
		this->__unpack_type_base(id_of<_T>::value);
		array_rank_t constexpr r = std::rank<_T>::value;
		this->__unpack_type_base(r);
		this->__xx_array_extent<_T>();

		this->__unpack_type_base(
			id_of<typename std::remove_all_extents<_T>::type>::value);
	}

	/**
	 * std array
	 */
	template< typename _T, typename std::enable_if<
		is_std_array<_T>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		for (auto & i : v) {
			this->unpack_data(i);
		}
	}

	template< typename _T, typename std::enable_if<
		is_std_array<_T>::value, bool>::type = false >
	void unpack_type(_T const & v)
	{
		if (v.size() > std::numeric_limits<ele_num_t>::max()) {
			throw;
		}
		this->__unpack_type_base(id_of<_T>::value);
		this->__unpack_type_base(static_cast<ele_num_t>(v.size()));

		this->__unpack_type_base(id_of<typename _T::value_type>::value);
	}

	/**
	 * std vector
	 */
	template< typename _T, typename std::enable_if<
		is_std_vector<_T>::value, bool>::type = false >
	void unpack_data(_T & v)
	{
		if (v.size() > std::numeric_limits<ele_num_t>::max()) {
			throw;
		}

		ele_num_t size;
		this->unpack_data(size);

		for (int i = 0; i < size; ++i) {
			typename _T::value_type tmp;
			this->unpack_data(tmp);
			v.push_back(tmp);
		}
	}

	template< typename _T, typename std::enable_if<
		is_std_vector<_T>::value, bool>::type = false >
	void unpack_type(_T const & v)
	{
		this->__unpack_type_base(id_of<_T>::value);
		this->__unpack_type_base(id_of<typename _T::value_type>::value);
	}

	/**
	 * struct / class
	 */
	template< typename _T, typename std::enable_if<
		id_of<_T>::value == id_t::UDT, bool>::type = false >
	void unpack_data(_T & v)
	{
		v.template serialize(m_helper);
	}

	template< typename _T, typename std::enable_if<
		id_of<_T>::value == id_t::UDT, bool>::type = false >
	void unpack_type(_T const &)
	{
		this->__unpack_type_base(
			id_of< _T >::value);
	}
private:
	template< typename _T, typename std::enable_if<
		std::is_array<_T>::value, bool>::type = false >
	void __xx_array_extent(void)
	{
		this->__unpack_type_base <array_extent_t>(std::extent<_T>::value);
		this->__xx_array_extent <typename std::remove_extent<_T>::type>();
	}
	template< typename _T, typename std::enable_if<
		!std::is_array<_T>::value, bool>::type = false >
	void __xx_array_extent(void)
	{
	}

	template< typename _T >
	void __unpack_type_base(_T const id)
	{
		_T tmp;
		this->unpack_data(tmp);
		if (tmp != id) {
			throw bad_type("TODO: id string mismatch");
		}
	}
private:
	unpacker_base & m_base;
	_helper_t & m_helper;
};

/**
 * \brief native pure
 */
class unpacker_np final : private unpacker<unpacker_np, false> {
	typedef unpacker<unpacker_np, false> base_t;
public:
	unpacker_np(unpacker_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t & ... args)
	{
		this->template unpack_d(args...);
	}
};

/**
 * \brief native full
 */
class unpacker_nf final : private unpacker<unpacker_nf, false> {
	typedef unpacker<unpacker_nf, false> base_t;
public:
	unpacker_nf(unpacker_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t & ... args)
	{
		this->template unpack_td(args...);
	}
};

/**
 * \brief native pure
 */
class unpacker_rp final : private unpacker<unpacker_rp, true> {
	typedef unpacker<unpacker_rp, true> base_t;
public:
	unpacker_rp(unpacker_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t & ... args)
	{
		this->template unpack_d(args...);
	}
};

/**
 * \brief native full
 */
class unpacker_rf final : private unpacker<unpacker_rf, true> {
	typedef unpacker<unpacker_rf, true> base_t;
public:
	unpacker_rf(unpacker_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t & ... args)
	{
		this->template unpack_td(args...);
	}
};

class rcver final {
public:
	rcver()
	: m_base()
	{
	}
public:
	unpacker_base const & b(void) const
	{
		return m_base;
	}
public:
	template< typename _T >
	void fill_from( _T & o, size_t (_T::*f)(void *, size_t))
	{
		m_base.reset();
		m_base.template read_from(o, f, HDR_SIZE);
		m_base.fill<false>(m_hdr.flag);
		this->__convert_to(m_hdr);

		if(m_hdr.len > 0 ) {
			m_base.template read_from(o, f, m_hdr.len);
		}

	}

	template< typename _T >
	void fill_from( _T & o)
	{
		fill_from(o, &_T::recv);
	}

	void fill_from(std::function<size_t (void *, size_t)> && f)
	{
		m_base.reset();
		m_base.read_from(f, HDR_SIZE);
		m_base.fill<false>(m_hdr.flag);
		this->__convert_to(m_hdr);

		m_base.read_from(f, m_hdr.len);
	}

	mid_t id(void) const
	{
		return m_hdr.mid;
	}
public:
	/**
	 * \note only for debug
	 */
	void dbg_fill_from(std::function<size_t (void *, size_t)> && f, size_t l)
	{
		m_base.read_from(f, l);
		m_base.fill<false>(m_hdr.flag);
		this->__convert_to(m_hdr);
	}
public:
	template< mid_t mid >
	void convert_to(zmsg<mid> & v)
	{
		if (this->id() != mid) {
			throw bad_type("msg id error");
		}

		const bool _wti = (m_hdr.flag & 0x1);
		const bool _rev  = ((bool)(m_hdr.flag & 0x2) != IS_LE);
		if (_wti) {	// with type info
			if (_rev) {
				unpacker_rf tmp(m_base);
				v.template serialize(tmp);
			}
			else {
				unpacker_nf tmp(m_base);
				v.template serialize(tmp);
			}
		}
		else {			// without type info
			if (_rev) {
				unpacker_rp tmp(m_base);
				v.template serialize(tmp);
			}
			else {
				unpacker_np tmp(m_base);
				v.template serialize(tmp);
			}
		}
	}
private:
	void __convert_to(zmsg_header & v)
	{
		const bool _rev  = ((bool)(m_hdr.flag & 0x2) != IS_LE);
		if (_rev) {
			unpacker_rp tmp(m_base);
			v.serialize(tmp);
		}
		else {
			unpacker_np tmp(m_base);
			v.serialize(tmp);
		}
	}
private:
	unpacker_base m_base;
	zmsg_header m_hdr;
};

} /* namespace zmsg */
