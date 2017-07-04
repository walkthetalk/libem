#include "parser.hpp"

static void gen_msg_enum(const std::size_t lvl)
{
	s_outf_id.pf(lvl, "enum class mid_t : %s {\n", s_mid_under_type);
	s_outf_id.pf(lvl+1, "nil = 0,	/// occupy symbol, correspond to \\em NULL.\n");

	std::size_t i = 0;
	for (const auto & itr : s_msg_lists) {
		++i;
		s_outf_id.pf(lvl+1, "%s = %d,\n", itr.first.c_str(), (int)i);
	}

	s_outf_id.pf(lvl, "};\n");
}

static void gen_msg_map(const std::size_t lvl)
{
	s_outf_id.pf(lvl, "template<mid_t mid>\n");
	s_outf_id.pf(lvl, "struct msg_helper final {};\n\n");
	for (const auto & itr: s_msg_lists) {
		const char * evt_name = itr.first.c_str();
		const rapidjson::Value & val = *(itr.second);
		const char * type_name = val.FindMember("name")->value.GetString();
		s_outf_id.pf(lvl, "template<>\n");
		s_outf_id.pf(lvl, "struct msg_helper<mid_t::%s> final {\n", evt_name);
		s_outf_id.pf(lvl+1, "typedef struct %s value_type;\n", type_name);
		s_outf_id.pf(lvl, "};\n\n");
	}

}

void generate_id(rapidjson::Document & /*d*/)
{
	const std::size_t lvl = 0;

	gen_msg_enum(lvl);
	gen_msg_map(lvl);
}
