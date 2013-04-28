#pragma once

#include "zmsg_cmm.hpp"

namespace zmsg {


template< typename _obj >
struct print_aux {
	print_aux(_obj & o)
	: m_o(o)
	{
	}
	
	template< typename _1st_t, typename ... _args_t >
	void operator()(_1st_t const & v, _args_t const & ... args)
	{
		__print(v);
		
		this->operator()(args...);
	}
	
	void operator()(void)
	{
	}
private:
	template< typename _T, typename std::enable_if<
		std::is_array<_T>::value, bool>::type = false >
	void __print(_T const & v)
	{
		for(size_t i = 0; i < std::extent< _T >::value; i++) {
			__print(v[i]);
		}
	}
	
	template< typename _T, typename std::enable_if<
		!std::is_array<_T>::value, bool>::type = false >
	void __print(_T const & v)
	{
		m_o << v << " ";
	}
	
	
private:
	_obj & m_o;
};
#if 0

template<>
struct zmsg< mid_t::TEST1 > {
	int a;
	int b;
	char c;

	ZMSG_PU(a, b, c)
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_INT8 > {
	int8_t a;
	int8_t b;
	int8_t c;

	ZMSG_PU(a, b, c);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_UINT8 > {
	uint8_t a;
	uint8_t b;

	ZMSG_PU(a, b);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_INT16 > {
	int16_t a;
	int16_t b;
	int16_t c;

	ZMSG_PU(a, b, c);
};
#endif

#if 1
template<>
struct zmsg< mid_t::test2 > {
	
#if 0
	int a[2];
	int b[2][2];
	char c[4];

	ZMSG_PU(a, b, c)
#endif

	ZMSG_PU()
};
#endif

#if 1
template<>
struct zmsg< mid_t::test1 > {
	int a;
	int b;


	ZMSG_PU(a, b)
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_UINT32 > {
	uint32_t a;
	uint32_t b;

	ZMSG_PU(a, b);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_INT64 > {
	int64_t a;
	int64_t b;
	int64_t c;

	ZMSG_PU(a, b, c);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST_INT64 > {
	int64_t a;
	int64_t b;

	ZMSG_PU(a, b);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST2 > {
	float a;
	float b;
	double c;
	double d;

	ZMSG_PU(a, b, c, d);
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST3 > {
	 int a[2];
	 int b[4];
	char c[4];

	ZMSG_PU(a, b, c)
};
#endif

#if 0
template<>
struct zmsg< mid_t::TEST3 > {
	 int a[2];
	 int b[2][2];
	char c[4];

	ZMSG_PU(a, b, c)
};
#endif

}

#define DCL_ZMSG(mid) zmsg::zmsg<zmsg::mid_t::mid>
