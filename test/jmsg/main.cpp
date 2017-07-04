
#include <iostream>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "c2json.hpp"
#include "json2c.hpp"
#include "in_out_wrapper.hpp"

#include "jmsg/jmsg_sender.hpp"
#include "jmsg/jmsg_rcver.hpp"

struct XXX {
	int32_t a;
	std::string b;
	std::vector<int32_t> c;
};

struct YYY {
	struct XXX _x;
	std::array<struct XXX, 3> _v;
	int d[2][3][4];
	int8_t i8;
	bool b;
};


rapidjson::Value c2json(rapidjson::Document & jd, const XXX & src)
{
	printf("c2json XXX\n");
	rapidjson::Value obj(rapidjson::kObjectType);
	ENC_MEM(jd, "int32_t", obj, src.a);
	ENC_MEM(jd, "string", obj, src.b);
	ENC_MEM(jd, "vector", obj, src.c);
	return obj;
}

rapidjson::Value c2json(rapidjson::Document & jd, const YYY & src)
{
	printf("c2json YYY\n");
	rapidjson::Value obj(rapidjson::kObjectType);
	ENC_MEM(jd, "struct XXX", obj, src._x);
	ENC_MEM(jd, "vec_XXX", obj, src._v);
	ENC_MEM(jd, "multarray", obj, src.d);
	ENC_MEM(jd, "int8_t", obj, src.i8);
	ENC_MEM(jd, "bool", obj, src.b);
	return obj;
}



void json2c(XXX & dst,  const rapidjson::Value & src)
{
	printf("json2c XXX\n");
	RAPIDJSON_ASSERT(src.IsObject());
	DEC_MEM("int32_t", src, dst.a);
	DEC_MEM("string", src, dst.b);
	DEC_MEM("vector", src, dst.c);
}

void json2c(YYY & dst,  const rapidjson::Value & src)
{
	printf("json2c YYY\n");
	RAPIDJSON_ASSERT(src.IsObject());
	DEC_MEM("struct XXX", src, dst._x);
	DEC_MEM("vec_XXX", src, dst._v);
	DEC_MEM("multarray",  src,  dst.d);
	DEC_MEM("int8_t",  src,  dst.i8);
	DEC_MEM("bool", src, dst.b);
}

void tostring(rapidjson::StringBuffer & buf,  const struct YYY & data)
{
	rapidjson::Document doc2;
	rapidjson::Value val = c2json(doc2, data);

	rapidjson::Writer<rapidjson::StringBuffer> writer2(buf);
	val.Accept(writer2);
	auto out2 = buf.GetString();
	printf("out: %s\n", out2);
}

void fromstring(struct YYY & data,  const rapidjson::StringBuffer & buf)
{
	printf("from: %s\n",  buf.GetString());
	rapidjson::Document doc3;
	doc3.Parse(buf.GetString());
	json2c(data,  doc3);
}

int main()
{
	struct YYY testdata2 = {
		{3, "abc", {1,2,3}},
		{{
			{5, "acc", {1,3,3}},
			{5, "adc", {4,2,3}},
			{7, "aec", {1,10,3}},
		}},
		{
			{ {1, 2, 3, 4},  {5, 6, 7, 8},  {9, 10, 11, 12}, },
			{ {12, 11, 10, 9},  {8, 7, 6, 5},  {4, 3, 2, 1}, },
		},
		0x14,
		false,
	};

	rapidjson::StringBuffer buffer2;
	tostring(buffer2,  testdata2);

	struct YYY testdata3;
	fromstring(testdata3,  buffer2);

	rapidjson::StringBuffer buffer3;
	tostring(buffer3,  testdata3);

	struct discharge jmsg_data = {
		34.5, 27,
	};

	char * pmsg = new char[65536];
	in_wrapper * pinter = nullptr;
	sender s;
	s.fill_to<mid_t::discharge>(jmsg_data, [pmsg, &pinter](const void * pdata, size_t s) -> size_t {
		::memcpy(pmsg, pdata, s);
		pinter = new in_wrapper(pmsg, s);
		printf("header: ");
		for (int i = 0; i < 8; ++i) {
			printf(" %02x", (unsigned)((char*)pdata)[i]);
		}
		printf("\n");
		printf("%s\nsize: %lu\n", &((char *)pdata)[8], (unsigned long)s);
		return s;
	});

	rcver r;
	r.register_callback<mid_t::discharge>([](struct discharge & dst)->void {
		printf("mag: %f, time: %d\n", dst.magnitude, dst.time);
	});
	r.fill_from([pinter](void *pdst, size_t s) -> std::size_t {
		printf("read data size(%lu)\n", (unsigned long)s);
		char * p = (char *)pdst;
		for (auto i = s; i != 0; --i) {
			*p = pinter->Take();
			++p;
		}
		return s;
	});


	return 0;
}




