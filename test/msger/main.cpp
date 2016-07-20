#include <iostream>
#include <iomanip>

#include "zmsg/zmsg_alarm.hpp"
#include "zmsg/zmsg_camera_oper.hpp"
#include "zmsg/zmsg_cmm.hpp"
#include "zmsg/zmsg_discharge.hpp"
#include "zmsg/zmsg_discharge_adjust.hpp"
#include "zmsg/zmsg_display_oper.hpp"
#include "zmsg/zmsg_dust_check.hpp"
#include "zmsg/zmsg_fusion_splice.hpp"
#include "zmsg/zmsg_fusion_splice_reset.hpp"
#include "zmsg/zmsg_header.hpp"
#include "zmsg/zmsg_heat.hpp"
#include "zmsg/zmsg_motor_oper.hpp"
#include "zmsg/zmsg_motor_test.hpp"
#include "zmsg/zmsg_packer.hpp"
#include "zmsg/zmsg_process_control.hpp"
#include "zmsg/zmsg_regular_test.hpp"
#include "zmsg/zmsg_stabilize_electrode.hpp"
#include "zmsg/zmsg_svc_state.hpp"
#include "zmsg/zmsg_types.hpp"
#include "zmsg/zmsg_unpacker.hpp"


struct T1 {
	int a;
	int c;
	double d;
	std::array<int, 3> e;
public:
	ZMSG_PU(a, c, d, e)
};

struct T2 {
	uint16_t x;
	float y;
	std::vector<bool> b;
	int32_t z;
	std::vector<double> t;
public:
	ZMSG_PU(x, y, b, z, t)
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
		//m_o << " xxxxxxxxx multi " << std::endl;
		//m_o << typeid(v).name() << std::endl;
		this->template operator()(v);

		//m_o << " remain size is: " << sizeof...(args) << std::endl;
		this->template operator()(args...);
	}

	template< typename _1st_t, typename std::enable_if<
		std::is_arithmetic<_1st_t>::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx num " << std::endl;
		m_o << v << " ";
	}

	template< typename _1st_t, typename std::enable_if<
		std::is_enum<_1st_t>::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx enum " << std::endl;
		auto under_v = static_cast< typename std::underlying_type<_1st_t>::type >(v);
		m_o << (long)under_v << " ";
	}

	template< typename _1st_t, typename std::enable_if<
		zmsg::is_std_array<_1st_t>::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx std::array " << std::endl;
		m_o << "size: " << v.size() << " ";
		for (auto & i : v) {
			this->template operator()(i);
		}
	}

	template< typename _1st_t, typename std::enable_if<
		    zmsg::is_std_vector<_1st_t>::value
		&& !std::is_same< typename _1st_t::value_type, bool >::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx std::vector<!bool> " << std::endl;
		m_o << "size: " << v.size() << " ";
		for (auto & i : v) {
			this->template operator()(i);
		}
	}

	template< typename _1st_t, typename std::enable_if<
		   zmsg::is_std_vector<_1st_t>::value
		&& std::is_same< typename _1st_t::value_type, bool >::value, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx std::vector<bool> " << std::endl;
		m_o << "bool size: " << v.size() << " ";
		auto i = v.begin();
		while (i != v.end()) {
			bool tb = *i;
			this->template operator()(tb);
			++i;
		}
	}

	template< typename _1st_t, typename std::enable_if<
		zmsg::id_of<_1st_t>::value == zmsg::id_t::UDT, bool>::type = false  >
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxxx struct " << std::endl;
		v.serialize(*this);
	}

	template< typename _1st_t, typename std::enable_if<
		std::is_same< _1st_t, std::string >::value, bool>::type = false>
	void operator()(_1st_t & v)
	{
		//m_o << " xxxxxxxx string " << std::endl;
		m_o << "size: " << v.size() << " ";
		for (auto & i : v) {
			this->template operator()(i);
		}
	}

	void operator()(void)
	{
		//m_o << " xxxxxxxxx null " << std::endl;
	}
private:
	_T & m_o;
};

template< bool _wti, bool _rev, zmsg::mid_t mid >
void test(zmsg::sender & p, zmsg::rcver & u, zmsg::zmsg<mid> & msg)
{
	//std::cout << "test -> print_aux start: " << std::endl;
	print_aux<decltype(std::cout)> pa(std::cout);
	//std::cout << "test -> print_aux end! " << std::endl;
	msg.serialize(pa);
	//std::cout << "test -> 1 " << std::endl;
#if 1
	//pa.template operator()("\n");
	std::cout << std::endl;

	p.pack<_wti, _rev>(msg);
	//std::cout << "test -> 2 " << std::endl;
	p.b().print(std::cout);

	//std::cout << "test -> 3 " << std::endl;
	u.dbg_fill_from([&p](void * buf, size_t size) -> size_t {
		return p.b().dbg_read((uint8_t*)buf, size);
	}, p.b().size());
#if 1
	zmsg::zmsg<mid> umsg;
	switch (u.id()) {
		case mid: {
			u.convert_to(umsg);
			umsg.serialize(pa);
			//pa.template operator()("\n");
			std::cout << std::endl;
		} break;
		default:
			break;
	}
#endif
#endif
}

int main(void)
{
	std::cout << std::hex << std::setw(2) << std::setfill('0');

	zmsg::sender p;
	zmsg::rcver u;

	DCL_ZMSG(test1) msg = {
		{ 0x34, 0x777, 1.01, {{ 2, 3, 4, }} },
		{ 0x4568, 0.544f, { true, false, false, true, false, }, 0xabcdef, {{ 3.4, 2.5, }} },
		'c',
	};
#if 1
	//test<false, false>(p, u, msg);
	//test<true, false>(p, u, msg);
#else
	typedef std::vector<double> self_t;
	std::cout << (int)zmsg::id_of<self_t>::value << std::endl;
#endif
	//std::cout << "test done!" << std::endl;

	DCL_ZMSG(dust_check_result) msg2 = {
		fs_err_t::success,
		{},
		true,
		"fjdk", "kfdjafo",
		false,
		"ofjoi", "932iew",
	};
	test<false, false>(p, u, msg2);
	std::cout << "dust_check first done!" <<std::endl;
	test<true, false>(p, u, msg2);

	return 0;
}
