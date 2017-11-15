#include "parser.hpp"

static void gen_msg_glb(const std::size_t lvl)
{
	s_outf_converter.pf(lvl, "/// mid to string\n");
	s_outf_converter.pf(lvl, "static rapidjson::Value::StringRefType const s_mid_to_str[] = {\n");
	s_outf_converter.pf(lvl+1, "\"nil\",\n");
	for (const auto & itr: s_msg_lists) {
		const char * evt_name = itr.first.c_str();
		s_outf_converter.pf(lvl+1, "\"%s\",\n", evt_name);
	}
	s_outf_converter.pf(lvl, "};\n\n");

	s_outf_converter.pf(lvl, "static rapidjson::Value::StringRefType const s_id = \"msgid\";\n");
	s_outf_converter.pf(lvl, "static rapidjson::Value::StringRefType const s_data = \"data\";\n");
}

static void generate_msg_sender_header(std::size_t lvl)
{
	s_outf_sender.pf(lvl, R"senderheader(
/// @class sender : used to send messages
class sender final {
public:
	sender(size_t reserve_header_size = 0);
	~sender();
public:
	template<mid_t mid, typename _T >
	void fill_to(typename msg_helper<mid>::value_type const & msg,  _T & o, void (_T::*f)(void *, size_t))
	{
		__reset(mid);
		__pack(msg);
		__serialize();
		o.f(m_buf, __size());
	}

	template< mid_t mid >
	void fill_to(typename msg_helper<mid>::value_type const & msg, std::function<void(void *, size_t)>  f)
	{
		__reset(mid);
		__pack(msg);
		__serialize();
		f(m_buf, __size());
	}
)senderheader");

	for (const auto & itr: s_export_order) {
		/// @todo maybe not struct
		const rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_sender.pf(lvl+1, "void convert(std::string & dst, const struct %s & src);\n", type_name);
	}

	s_outf_sender.pf(lvl, R"senderheader(
private:
)senderheader");

	for (const auto & itr: s_msg_order) {
		/// @todo maybe not struct
		const rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_sender.pf(lvl+1, "void __pack(const struct %s & val);\n", type_name);
	}

	s_outf_sender.pf(lvl, R"senderheader(
private:
	void __reset(mid_t mid);
	void __serialize();
private:
	std::size_t __size() const
	{
		return (m_pcur - m_buf);
	}
private:
	void __fill_header();
private:
	uint8_t m_lbuf[1048576];
	uint8_t * const m_buf;
	uint8_t * m_pcur;
	const uint8_t * const m_pend;
	void * m_doc;
};

)senderheader");

	s_outf_sender.pf(lvl, "\n");
}

static void generate_msg_sender_implement(std::size_t lvl)
{
	s_outf_converter.pf(lvl, R"senderfillheader(
/// @sender constructor
sender::sender(size_t reserve_header_size)
: m_buf(&m_lbuf[reserve_header_size + LWS_PRE])
, m_pcur(m_buf)
, m_pend(m_lbuf + sizeof(m_lbuf))
, m_doc(new rapidjson::Document())
{}

/// @sender destructor
sender::~sender()
{
	delete (rapidjson::Document*)m_doc;
}

/// @sender reset
void sender::__reset(mid_t mid)
{
	m_pcur = m_buf;
	((rapidjson::Document*)m_doc)->SetObject();

	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_id, s_mid_to_str[(%s)mid], doc.GetAllocator());
}

/// @sender serialize
void sender::__serialize()
{
	out_wrapper buf((char *)m_pcur);
	rapidjson::Writer<out_wrapper> writer(buf);
	((rapidjson::Document *)m_doc)->Accept(writer);
	m_pcur = (uint8_t *)buf.pcur;
}

)senderfillheader", s_mid_under_type);
	s_outf_converter.pf(lvl, "\n");

	for (const auto & itr: s_export_order) {
		/// @todo maybe not struct
		const rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_converter.pf(lvl, "void sender::convert(std::string & dst, const struct %s & src)\n", type_name);
		s_outf_converter.pf(lvl, "{\n");
		s_outf_converter.pf(lvl+1, "rapidjson::Document & doc = *(rapidjson::Document*)m_doc;\n");
		s_outf_converter.pf(lvl+1, "out_string_wrapper buf(dst);\n");
		s_outf_converter.pf(lvl+1, "rapidjson::Writer<out_string_wrapper> writer(buf);\n");
		s_outf_converter.pf(lvl+1, "c2json(doc, src).Accept(writer);\n");
		s_outf_converter.pf(lvl, "}\n\n");
	}

	s_outf_converter.pf(lvl, "/// messages packer wrapper\n");
	for (const auto & itr: s_msg_order) {
		/// @todo maybe not struct
		const rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_converter.pf(lvl, "void sender::__pack(const struct %s & val)\n", type_name);
		s_outf_converter.pf(lvl, "{\n");
		s_outf_converter.pf(lvl+1, "rapidjson::Document & doc = *(rapidjson::Document *)m_doc;\n");
		s_outf_converter.pf(lvl+1, "doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());\n");
		s_outf_converter.pf(lvl, "}\n\n");
	}
}

static void generate_msg_rcver_header(std::size_t lvl)
{
	s_outf_rcver.pf(lvl, R"rcverheader(
/// @class rcver : used to receive messages
class rcver final {
public:
	rcver();
	~rcver();
public:
	void process(void *, size_t);

	template<mid_t mid, template<mid_t> class T>
	void register_callback(std::function<void(T<mid> &)> cb)
	{
		m_cbs[__mid_to_str(mid)] = [this, cb](void) -> void {
			T<mid> tmp;
			__unpack((typename msg_helper<mid>::value_type &)tmp);
			cb(tmp);
		};
	}

	template<mid_t mid>
	void register_callback(std::function<void(typename msg_helper<mid>::value_type &)> cb)
	{
		m_cbs[__mid_to_str(mid)] = [this, cb](void) -> void {
			typename msg_helper<mid>::value_type msg;
			__unpack(msg);
			cb(msg);
		};
	}

	template<typename _T>
	void convert(_T & dst, const std::string & src)
	{
		convert(dst, src.c_str());
	}
)rcverheader");

	for (const auto & itr: s_export_order) {
		rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_rcver.pf(lvl+1, "void convert(struct %s & dst, const char * src);\n", type_name);
	}

	s_outf_rcver.pf(lvl, R"rcverheader(
private:
	const char * __mid_to_str(mid_t mid);
)rcverheader");

	for (const auto & itr: s_msg_order) {
		rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_rcver.pf(lvl+1, "void __unpack(struct %s & dst);\n", type_name);
	}

	s_outf_rcver.pf(lvl, R"rcverheader(
private:
	void __reset();
private:
	void * m_doc;
	std::map<std::string, std::function<void(void)>> m_cbs;
};

)rcverheader");
#if 0
	for (const auto & itr: s_msg_lists) {
		const char * evt_name = itr.first.c_str();
		const rapidjson::Value & val = *(itr.second);
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_rcver.pf(lvl, "template<> void rcver::register_callback<mid_t::%s>(std::function< void(std::function<void(struct %s&)>)> f);\n", evt_name, type_name);
	}
#endif

	s_outf_rcver.pf(lvl, "\n");
}

static void generate_msg_rcver_implement(std::size_t lvl)
{
	s_outf_converter.pf(lvl, R"rcverimplementer(
/// @class rcver : used to receive messages
rcver::rcver()
: m_doc(new rapidjson::Document())
{}

rcver::~rcver()
{
	delete (rapidjson::Document*)m_doc;
}

void rcver::__reset()
{
	((rapidjson::Document*)m_doc)->SetNull();
}

void rcver::process(void * buf, size_t /*len*/)
{
	/// process
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu((char *)buf);
	auto & cb = m_cbs[doc.FindMember(s_id)->value.GetString()];
	if (cb) { cb(); }

	__reset();
}

)rcverimplementer");

	for (const auto & itr: s_export_order) {
		rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_converter.pf(lvl, "void rcver::convert(struct %s & dst, const char * src)\n", type_name);
		s_outf_converter.pf(lvl, "{\n");
		s_outf_converter.pf(lvl+1, "rapidjson::Document & doc = *((rapidjson::Document*)m_doc);\n");
		s_outf_converter.pf(lvl+1, "doc.Parse(src);\n");
		s_outf_converter.pf(lvl+1, "json2c(dst, doc);\n");
		s_outf_converter.pf(lvl+1, "__reset();\n");
		s_outf_converter.pf(lvl, "}\n\n", type_name);
	}

#if 0
	/// register callback
	s_outf_converter.pf(lvl, "/// messages register\n");
	for (const auto & itr: s_msg_lists) {
		const char * evt_name = itr.first.c_str();
		const rapidjson::Value & val = *(itr.second);
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_converter.pf(lvl, "template<>\n");
		s_outf_converter.pf(lvl, "void rcver::register_callback<mid_t::%s>(std::function< void(std::function<void(struct %s&)>)> f)\n", evt_name, type_name);
		s_outf_converter.pf(lvl, "{\n");
		s_outf_converter.pf(lvl+1, "auto getter = [this](struct %s & dst) -> void {\n", type_name);
		s_outf_converter.pf(lvl+2, "json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(\"data\")->value);\n");
		s_outf_converter.pf(lvl+1, "};\n");
		s_outf_converter.pf(lvl+1, "m_cbs[\"%s\"] = [getter, f]()->void {\n", evt_name);
		s_outf_converter.pf(lvl+2, "f(getter);\n");
		s_outf_converter.pf(lvl+1, "};\n");
		s_outf_converter.pf(lvl, "}\n\n");
	}
#endif
	/// mid to string
	s_outf_converter.pf(lvl, "/// mid to string\n");
	s_outf_converter.pf(lvl, "const char * rcver::__mid_to_str(mid_t mid)\n");
	s_outf_converter.pf(lvl, "{\n");
	s_outf_converter.pf(lvl+1, "return s_mid_to_str[(%s)mid];\n", s_mid_under_type);
	s_outf_converter.pf(lvl, "}\n\n");

	/// unpack
	s_outf_converter.pf(lvl, "/// messages unpack\n");
	for (const auto & itr: s_msg_order) {
		rapidjson::Value & val = *itr;
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_converter.pf(lvl, "void rcver::__unpack(struct %s & dst)\n", type_name);
		s_outf_converter.pf(lvl, "{\n");
		s_outf_converter.pf(lvl+1, "json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);\n");
		s_outf_converter.pf(lvl, "}\n\n");
	}
}

void generate_wsmsgs(rapidjson::Document & /*d*/)
{
	const std::size_t lvl = 0;

	gen_msg_glb(lvl);

	generate_msg_sender_header(lvl);
	generate_msg_sender_implement(lvl);

	generate_msg_rcver_header(lvl);
	generate_msg_rcver_implement(lvl);
}
