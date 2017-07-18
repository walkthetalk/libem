#include "parser.hpp"

static bool is_enum_flag(const rapidjson::Value & val)
{
	RAPIDJSON_ASSERT(val.HasMember("category"));
	RAPIDJSON_ASSERT(val.FindMember("category")->value.IsString());
	RAPIDJSON_ASSERT(val.FindMember("category")->value == "enum");
	RAPIDJSON_ASSERT(val.HasMember("attr"));
	const rapidjson::Value & attrs = val.FindMember("attr")->value;
	for (const auto & itr : attrs.GetArray()) {
		if (itr == "flag") {
			return true;
		}
	}
	return false;
}

struct cmp_str_as_n {
	bool operator()(const std::string & lhs, const std::string rhs)
	{
		unsigned long long lhs_n = std::stoull(lhs.c_str(),0,0);
		unsigned long long rhs_n = std::stoull(rhs.c_str(),0,0);
		return (lhs_n < rhs_n);
	}
};


static void convert_enum(rapidjson::Document & /*doc*/, rapidjson::Value & val, const std::size_t lvl)
{
	filebuf & outf = s_outf_converter;

	RAPIDJSON_ASSERT(val.HasMember("name"));
	const char * ename = val.FindMember("name")->value.GetString();

	bool is_flag = is_enum_flag(val);
	//bool is_msg = is_as_msg(val);
	//const char * fn_prefix = is_msg ? "" : "static inline ";

	RAPIDJSON_ASSERT(val.HasMember("fields"));
	auto & fields = val.FindMember("fields")->value;
	RAPIDJSON_ASSERT(fields.IsObject());

	if (is_flag) {
		std::map<std::string, std::string, cmp_str_as_n> int2string;
		std::map<std::string, std::string> string2int;
		for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
			const char * name = itr->name.GetString();
			RAPIDJSON_ASSERT(itr->value.IsObject());
			RAPIDJSON_ASSERT(itr->value.HasMember("value"));
			const rapidjson::Value & rval = itr->value.FindMember("value")->value;
			RAPIDJSON_ASSERT(rval.IsString());
			int2string[rval.GetString()] = name;
			string2int[name] = rval.GetString();
		}

		int val_num = (int)string2int.size();
#if 0
		/// @note dec order
		std::vector<std::pair<std::string, std::string>> reorder_pairs;
		for (auto & i : string2int)	reorder_pairs.push_back(i);
		std::sort(reorder_pairs.begin(), reorder_pairs.end(), [=](std::pair<std::string, std::string>& lhs, std::pair<std::string, std::string>& rhs) {
			unsigned long long lhs_n = std::stoull(lhs.second,0,0);
			unsigned long long rhs_n = std::stoull(rhs.second,0,0);
			bool ret = false;
			if (lhs_n < rhs_n) {
				ret = true;
			}
			else if (lhs_n == rhs_n) {
				ret = (lhs.first.compare(rhs.first) < 0);
			}
			else {
				ret = false;
			}
			return !ret;
		});
#endif
		const char * const etype = val.FindMember("type")->value.GetString();

		outf.pf(lvl, "/// @%s : string to enum\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "rapidjson::Value::StringRefType name;\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl, "} str2e_%s[%d] = {\n", ename, val_num);
		for (auto & i : string2int) {
			outf.pf(lvl+1, "{ \"%s\", %s },\n", i.first.c_str(), i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "/// @%s : enum to string\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl+1, "rapidjson::Value::StringRefType name;\n");
		outf.pf(lvl, "} e2str_%s[%d] = {\n", ename, val_num);
		for (auto & i : int2string) {
			outf.pf(lvl+1, "{ %s, \"%s\" },\n", i.first.c_str(), i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "static inline rapidjson::Value c2json(rapidjson::Document & jd, const enum %s src)\n", ename);
		outf.pf(lvl, "{ return e2flag(jd, e2str_%s, (%s)src); }\n\n", ename, etype);

		outf.pf(lvl, "static inline void json2c(enum %s & dst, const rapidjson::Value & src)\n", ename);
		outf.pf(lvl, "{ dst = (enum %s)flag2e(str2e_%s, src); }\n", ename, ename);
	}
	else {
		std::map<int, std::string> int2string;
		std::map<std::string, int> string2int;
		for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
			const char * name = itr->name.GetString();
			RAPIDJSON_ASSERT(itr->value.IsObject());
			RAPIDJSON_ASSERT(itr->value.HasMember("value"));
			int rval = itr->value.FindMember("value")->value.GetInt();
			int2string[rval] = name;
			string2int[name] = rval;
		}

		const int min_val = int2string.begin()->first;
		const int max_val = int2string.rbegin()->first;
		const int val_num = (int)int2string.size();
		const char * etype = val.FindMember("type")->value.GetString();

		outf.pf(lvl, "/// @%s : string to enum\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "rapidjson::Value::StringRefType name;\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl, "} str2e_%s[%d] = {\n", ename, (int)string2int.size());
		for (auto & i : string2int) {
			outf.pf(lvl+1, "{ \"%s\", %d },\n", i.first.c_str(), i.second);
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "/// @%s : enum to string\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl+1, "rapidjson::Value::StringRefType name;\n");
		outf.pf(lvl, "} e2str_%s[%d] = {\n", ename, val_num);
		for (auto & i : int2string) {
			outf.pf(lvl+1, "{ %d, \"%s\" },\n", i.first, i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum %s src)\n", ename);
		if (val_num >= (max_val - min_val + 1)) {
			if (min_val == 0) {
				outf.pf(lvl, "{ return search_name_directly(e2str_%s, (%s)src); }\n\n", ename, etype);
			}
			else {
				outf.pf(lvl, "{ return search_name_directly(e2str_%s, (%s)src - %d); }\n\n", ename, etype, min_val);
			}
		}
		else {
			outf.pf(lvl, "{ return search_name_binary(e2str_%s, (%s)src); }\n\n", ename, etype);
		}

		outf.pf(lvl, "static inline void json2c(enum %s & dst, const rapidjson::Value & src)\n", ename);
		outf.pf(lvl, "{ dst = (enum %s)search_val_binary(str2e_%s, src); }\n", ename, ename);
	}

	outf.pf(0, "\n");
}

void convert_struct(rapidjson::Document & /*doc*/, rapidjson::Value & val, const std::size_t lvl)
{
	RAPIDJSON_ASSERT(val.HasMember("name"));
	const char * ename = val.FindMember("name")->value.GetString();
	//bool is_msg = is_as_msg(val);
	//const char * fn_prefix = is_msg ? "" : "static inline ";
	const rapidjson::Value & fields = val.FindMember("fields")->value;

	filebuf & outf = s_outf_converter;
	if (fields.MemberCount() > 0) {
		outf.pf(lvl, "/// @struct %s\n", ename);
		outf.pf(lvl, "static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct %s & src)\n", ename);
		outf.pf(lvl, "{\n");

		outf.pf(lvl+1, "rapidjson::Value v(rapidjson::kObjectType);\n");
		for (rapidjson::Value::ConstMemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
			const char * mname = itr->name.GetString();
			auto & mval = itr->value;
			if (mval.FindMember("maybenull")->value.GetBool()) {
				outf.pf(lvl+1, "ENC_MEM_IF(jd, \"%s\", v, src.%s, src.has_%s);\n", mname, mname, mname);
			}
			else {
				outf.pf(lvl+1, "ENC_MEM(jd, \"%s\", v, src.%s);\n", mname, mname);
			}
		}
		outf.pf(0, "\n");
		outf.pf(lvl+1, "return v;\n");

		outf.pf(lvl, "}\n");

		outf.pf(lvl, "static inline void json2c(struct %s & dst, const rapidjson::Value & src)\n", ename);
		outf.pf(lvl, "{\n");
		for (rapidjson::Value::ConstMemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
			const char * mname = itr->name.GetString();
			auto & mval = itr->value;
			if (mval.FindMember("maybenull")->value.GetBool()) {
				outf.pf(lvl+1, "DEC_MEM_IF(\"%s\", src, dst.%s, dst.has_%s);\n", mname, mname, mname);
			}
			else {
				outf.pf(lvl+1, "DEC_MEM(\"%s\", src, dst.%s);\n", mname, mname);
			}
		}
		outf.pf(lvl, "}\n");

		outf.pf(0, "\n");
	}
	else {
		outf.pf(lvl, "static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const struct %s & /*src*/) {\n", ename);
		outf.pf(lvl+1, "return rapidjson::Value(rapidjson::kObjectType);\n");
		outf.pf(lvl, "}\n");
		outf.pf(lvl, "static inline void json2c(struct %s & /*dst*/, const rapidjson::Value & /*src*/) {}\n", ename);
		outf.pf(0, "\n");
	}
}

std::map<
	const std::string,
	std::function<void (rapidjson::Document &, rapidjson::Value &, const std::size_t)>
> func_generator = {
	{"enum", convert_enum},
	{"struct", convert_struct},
};

void generate_converter(rapidjson::Document & d)
{
	//for (auto & i : s_type_lists) {
	for (auto & pi : s_type_order) {
		//rapidjson::Value &val = *i.second;
		rapidjson::Value & val = *pi;
		if (val.HasMember("category")) {
			rapidjson::Value & cat = val.FindMember("category")->value;
			func_generator.at(cat.GetString())(d, val, 0);
		}
	}
}

