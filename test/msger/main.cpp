#include <iostream>
#include <iomanip>

#include "zmsg/zmsg_packer.hpp"
#include "zmsg/zmsg_unpacker.hpp"

#include "zmsg/zmsg_header.hpp"

struct T1 {
	int a;
	int c;
	double d;
public:
	ZMSG_PU(a, c, d)
};

struct T2 {
	uint16_t x;
	float y;
	int32_t z;
public:
	ZMSG_PU(x, y, z)
};


namespace zmsg {

template<>
struct zmsg<mid_t::test1> {
public:
	T1 a;
	T2 b;
	char c;
public:
	ZMSG_PU(a, b, c)
};

}

template< typename _T >
struct print_aux {
	print_aux(_T & o)
	: m_o(o)
	{
	}
	template< typename _1st_t, typename ... _args_t >
	void operator()(_1st_t & v, _args_t & ... args)
	{
		this->template operator()(v);

		this->operator()(args...);
	}

	template< typename _1st_t, typename std::enable_if<
		std::is_class<_1st_t>::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		v.serialize(*this);
	}

	template< typename _1st_t, typename std::enable_if<
		!std::is_class<_1st_t>::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		m_o << v << " ";
	}

	void operator()(void)
	{
	}
private:
	_T & m_o;
};

template< bool _wti, bool _rev, zmsg::mid_t mid >
void test(zmsg::sender & p, zmsg::rcver & u, zmsg::zmsg<mid> & msg)
{
	print_aux<decltype(std::cout)> pa(std::cout);
	msg.serialize(pa);

	pa.template operator()("\n");

	p.pack<_wti, _rev>(msg);
	p.b().print(std::cout);

	u.dbg_fill_from([&p](void * buf, size_t size) -> size_t {
		return p.b().dbg_read((uint8_t*)buf, size);
	}, p.b().size());
#if 1
	zmsg::zmsg<mid> umsg;
	switch (u.id()) {
		case mid: {
			u.convert_to(umsg);
			umsg.serialize(pa);
			pa.template operator()("\n");
		} break;
		default:
			break;
	}
#endif
}

int main(void)
{
	std::cout << std::hex << std::setw(2) << std::setfill('0');

	zmsg::sender p;
	zmsg::rcver u;

	DCL_ZMSG(test1) msg = {
		{ 0x34, 0x777, 1.01, },
		{ 0x4568, 0.544f, 0xabcdef, },
		'c',
	};

	test<false, false>(p, u, msg);

	return 0;
}
