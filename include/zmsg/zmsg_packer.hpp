#pragma once

#include <functional>
#include <algorithm>
#include <stdexcept>

#include "zmsg/zmsg_cmm.hpp"
#include "zmsg/zmsg_header.hpp"

namespace zmsg {

class write_fail : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

struct packer_base final {
public:
	packer_base(uint8_t * buf, size_t l)
	: m_buf(buf)
	, m_buf_end(m_buf + l)
	, m_e(m_buf)
	{
	}

public:
	template< typename _T >
	void print(_T & o) const
	{
		const uint8_t * p = m_buf;
		while (p < m_e) {
			o << (uint16_t)(*p++) << " ";
		}
		o << "\n";
	}

	void reset(void)
	{
		m_e = m_buf;
	}
public:
	/**
	 * \note only for debug
	 */
	size_t dbg_read(uint8_t * buf, size_t l) const
	{
		size_t ret = std::min(l, this->size());
		std::copy_n(m_buf, l, buf);
		return ret;
	}
public:
	template< typename _T >
	void write_to(_T & o, size_t (_T::*f)(const void *, size_t)) const
	{
		const size_t s = this->size();
		const size_t ret = (o.*f)(m_buf, s);
		if (ret != s) {
			throw write_fail("write to fail");
		}
	}

	void write_to(std::function<size_t(const void *, size_t)>& f) const
	{
		const size_t s = this->size();
		f(m_buf, s);

	}

public:
	packer_base reserve(size_t l)
	{
		__check_err(l);
		uint8_t * const ret = m_e;
		m_e += l;
		return packer_base(ret, l);
	}
public:
	size_t size(void) const
	{
		return static_cast<size_t>(m_e - m_buf);
	}
public:
	template< bool _rev, typename _T >
	void fill(_T const & v)
	{
		this->__fill<_rev>((const uint8_t *)&v, sizeof(v));
	}
private:
	template< bool _rev, typename std::enable_if<
		!_rev, bool>::type = false >
	void __fill(const uint8_t * src, size_t l)
	{
		__check_err(l);
		std::copy_n(src, l, m_e);
		m_e += l;
	}

	template< bool _rev, typename std::enable_if<
		_rev, bool>::type = false >
	void __fill(const uint8_t * src, size_t l)
	{
		__check_err(l);
		std::reverse_copy(src, src+l, m_e);
		m_e += l;
	}

	void __check_err(size_t l)
	{
		if (l > static_cast<size_t>(m_buf_end - m_e)) {
			throw std::bad_alloc();
		}
	}
private:
	uint8_t * const m_buf;
	uint8_t * const m_buf_end;
	uint8_t * m_e;
};

template< typename _helper_t, bool _reverse >
class packer {
public:
	packer(_helper_t & helper, packer_base & b)
	: m_base(b)
	, m_helper(helper)
	{
	}
public:
	template< typename _T>
	void pack_d(_T const & v)
	{
		this->pack_data(v);
	}
	template< typename _T >
	void pack_t(_T const & v)
	{
		this->pack_type(v);
	}
	template< typename _T >
	void pack_td(_T const & v)
	{
		this->pack_type(v);
		this->pack_data(v);
	}

	template< typename _T, typename ... _args_t>
	void pack_d(_T const & v, _args_t const & ... args)
	{
		this->pack_d(v);
		this->pack_d(args...);
	}
	template< typename _T, typename ... _args_t >
	void pack_t(_T const & v, _args_t const & ... args)
	{
		this->pack_t(v);
		this->pack_t(args...);
	}
	template< typename _T, typename ... _args_t >
	void pack_td(_T const & v, _args_t const & ... args)
	{
		this->pack_td(v);
		this->pack_td(args...);
	}

	void pack_d()
	{
	}
	void pack_t()
	{
	}
	void pack_td()
	{
	}
public:
	/**
	 * signed/unsigned int8/int16/int32/int64 float double
	 */
	template< typename _T, typename std::enable_if<
		   std::is_arithmetic<_T>::value
		&& !std::is_same<_T, bool>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		m_base.template fill<_reverse>(v);
	}

	template< typename _T, typename std::enable_if<
		   std::is_arithmetic<_T>::value
		&& !std::is_same<_T, bool>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(id_of<_T>::value);
	}

	/**
	 * bool
	 */
	template< typename _T, typename std::enable_if<
		   std::is_same<_T, bool>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		uint8_t const d = (v ? 1 : 0);
		this->pack_data(d);
	}

	template< typename _T, typename std::enable_if<
		   std::is_same<_T, bool>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(id_of<_T>::value);
	}

	/**
	 * enum
	 */
	template< typename _T, typename std::enable_if<
		std::is_enum<_T>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		this->pack_data(
			static_cast< typename std::underlying_type<_T>::type >(v));
	}

	template< typename _T, typename std::enable_if<
		std::is_enum<_T>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(
			id_of< typename std::underlying_type<_T>::type >::value);
	}

	/**
	 * array
	 */
	template< typename _T, typename std::enable_if<
		   std::is_array<_T>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		for (size_t i = 0; i < std::extent<_T>::value; ++i) {
			this->pack_data <typename std::remove_extent<_T>::type>(v[i]);
		}
	}

	template< typename _T, typename std::enable_if<
		   std::is_array<_T>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(id_of<_T>::value);
		array_rank_t constexpr r = std::rank<_T>::value;
		this->__pack_type_base(r);
		this->__xx_array_extent<_T>();

		this->__pack_type_base(
			id_of<typename std::remove_all_extents<_T>::type>::value);
	}

	/**
	 * std array
	 */
	template< typename _T, typename std::enable_if<
		is_std_array<_T>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		for (auto & i : v) {
			this->pack_data(i);
		}
	}

	template< typename _T, typename std::enable_if<
		is_std_array<_T>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		if (v.size() > std::numeric_limits<ele_num_t>::max()) {
			throw std::overflow_error("std::array size overflow!");
		}
		this->__pack_type_base(id_of<_T>::value);
		this->__pack_type_base(static_cast<ele_num_t>(v.size()));

		this->__pack_type_base(id_of<typename _T::value_type>::value);
	}

	/**
	 * std vector
	 */
	template< typename _T, typename std::enable_if<
		   is_std_vector<_T>::value
		&& !std::is_same< typename _T::value_type, bool >::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		if (v.size() > std::numeric_limits<ele_num_t>::max()) {
			throw std::overflow_error("std::vector size overflow!");
		}

		this->pack_data(static_cast<ele_num_t>(v.size()));

		for (auto & i : v) {
			this->pack_data(i);
		}
	}

	template< typename _T, typename std::enable_if<
		   is_std_vector<_T>::value
		&& std::is_same< typename _T::value_type, bool >::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		auto const size = v.size();
		if (size > std::numeric_limits<ele_num_t>::max()) {
			throw std::overflow_error("std::vector size overflow!");
		}
		this->pack_data(static_cast<ele_num_t>(v.size()));

		uint8_t sv = 0;
		std::vector<bool>::const_iterator it = v.begin();
		for (auto i = size / 8; i > 0; --i) {
			sv = 0;
			for (auto j = 0; j < 8; ++j) {
				if (*it++) {
					sv |= (1 << j);
				}
			}
			this->pack_data(sv);
		}

		auto remainder = size % 8;
		if (remainder > 0) {
			sv = 0;
			for (decltype(remainder) j = 0; j < remainder; ++j) {
				if (*it++) {
					sv |= (1 << j);
				}
			}
			this->pack_data(sv);
		}
	}

	template< typename _T, typename std::enable_if<
		is_std_vector<_T>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(id_of<_T>::value);
		this->__pack_type_base(id_of<typename _T::value_type>::value);
	}

	/**
	 * std string
	 */
	template< typename _T, typename std::enable_if<
		   is_std_string<_T>::value, bool>::type = false >
	void pack_data(_T const & v)
	{
		if (v.size() > std::numeric_limits<ele_num_t>::max()) {
			throw std::overflow_error("std::string size overflow!");
		}

		this->pack_data(static_cast<ele_num_t>(v.size()));

		for (auto & i : v) {
			this->pack_data(i);
		}
	}

	template< typename _T, typename std::enable_if<
		is_std_string<_T>::value, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(id_of<_T>::value);
		this->__pack_type_base(id_of<typename _T::value_type>::value);
	}

	/**
	 * struc / class
	 */
	template< typename _T, typename std::enable_if<
		id_of<_T>::value == id_t::UDT, bool>::type = false >
	void pack_data(_T const & v)
	{
		v.template serialize(m_helper);
	}

	template< typename _T, typename std::enable_if<
		id_of<_T>::value == id_t::UDT, bool>::type = false >
	void pack_type(_T const & v)
	{
		this->__pack_type_base(
			id_of< _T >::value);
	}
private:
	template< typename _T, typename std::enable_if<
		std::is_array<_T>::value, bool>::type = false >
	void __xx_array_extent(void)
	{
		this->__pack_type_base <array_extent_t>(std::extent<_T>::value);
		this->__xx_array_extent <typename std::remove_extent<_T>::type>();
	}
	template< typename _T, typename std::enable_if<
		!std::is_array<_T>::value, bool>::type = false >
	void __xx_array_extent(void)
	{
	}

	template< typename _T >
	void __pack_type_base(_T const id)
	{
		this->pack_data(id);
	}
private:
	packer_base & m_base;
	_helper_t & m_helper;
};

/**
 * \brief native pure
 */
class packer_np final : private packer<packer_np, false> {
	typedef packer<packer_np, false> base_t;
public:
	packer_np(packer_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t const & ... args)
	{
		this->template pack_d(args...);
	}
};

/**
 * \brief native full
 */
class packer_nf final : private packer<packer_nf, false> {
	typedef packer<packer_nf, false> base_t;
public:
	packer_nf(packer_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t const & ... args)
	{
		this->template pack_td(args...);
	}
};

/**
 * \brief native pure
 */
class packer_rp final : private packer<packer_rp, true> {
	typedef packer<packer_rp, true> base_t;
public:
	packer_rp(packer_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t const & ... args)
	{
		this->template pack_d(args...);
	}
};

/**
 * \brief native full
 */
class packer_rf final : private packer<packer_rf, true> {
	typedef packer<packer_rf, true> base_t;
public:
	packer_rf(packer_base & b)
	: base_t(*this, b)
	{
	}
public:
	template< typename ... _args_t >
	void operator()(_args_t const & ... args)
	{
		this->template pack_td(args...);
	}
};

/**
 * \brief sender
 */
class sender final {
public:
	sender()
	: m_base(m_buf, sizeof(m_buf))
	{
	}
public:
	packer_base const & b(void) const
	{
		return m_base;
	}
public:
	template< bool _wti, bool _rev, mid_t mid, typename _T >
	void fill_to(zmsg<mid> const & msg,  _T & o, size_t (_T::*f)(const void *, size_t))
	{
		this->pack<_wti, _rev>(msg);
		m_base.template write_to(o, f);
	}

	template< bool _wti, bool _rev, mid_t mid >
	void fill_to(zmsg<mid> const & msg, std::function<size_t(const void *, size_t)>  f)
	{
		this->pack<_wti, _rev>(msg);
		m_base.template write_to(f);
	}
public:
	template< bool _wti, bool _rev, mid_t mid >
	void pack(zmsg<mid> const & v)
	{
		typedef typename std::conditional<_rev, packer_rf, packer_nf>::type packer_full;
		typedef typename std::conditional<_rev, packer_rp, packer_np>::type packer_pure;
		typedef typename std::conditional<_wti, packer_full, packer_pure>::type packer_specific;

		m_base.reset();
		packer_base hdr_buf = m_base.reserve(HDR_SIZE);

		/// fill msg body
		packer_specific payload_packer(m_base);
		v.template serialize(payload_packer);
		/// fill hdr
		zmsg_header h = { 0xF0, 0xBB, 0xABCD, 0, 0x1357, mid, };
		if (_wti) h.flag |= 0x1;
		if (_rev != IS_LE) h.flag |= 0x2;
		hdr_buf.fill<false>(h.flag);

		if (ZMSG_MAX_LEN < m_base.size()) {
			throw std::overflow_error("zmsg size overflow!");
		}

		h.len = static_cast<msg_len_t>((m_base.size() - HDR_SIZE));
		packer_pure hdr_packer(hdr_buf);
		h.template serialize(hdr_packer);
	}

private:
	uint8_t m_buf[s_def_buf_size];
	packer_base m_base;
};

} /* namespace zmsg */
