#include <iostream>
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
	filebuf & outf = s_outf_pqxx_converter;

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
		outf.pf(lvl+1, "const char * name;\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl, "} str2e_%s[%d] = {\n", ename, val_num);
		for (auto & i : string2int) {
			outf.pf(lvl+1, "{ \"%s\", %s },\n", i.first.c_str(), i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "/// @%s : enum to string\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl+1, "const char * name;\n");
		outf.pf(lvl, "} e2str_%s[%d] = {\n", ename, val_num);
		for (auto & i : int2string) {
			outf.pf(lvl+1, "{ %s, \"%s\" },\n", i.first.c_str(), i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "static inline %s e2pqxx(const enum %s src)\n", etype, ename);
		outf.pf(lvl, "{ return (%s)src; }\n\n", ename, etype);
		outf.pf(lvl, "template<>\n");
		outf.pf(lvl, "inline void pqxx::internal::params::add_field(const enum %s & src)\n", ename);
		outf.pf(lvl, "{ add_field(e2pqxx(src)); }\n");

		outf.pf(lvl, "static inline void pqxx2c(enum %s & dst, const pqxx::const_row_iterator & it)\n", ename);
		outf.pf(lvl, "{ %s v; it->to(v); dst = (enum %s)v; }\n", etype, ename);
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
			if (itr->value.HasMember("alias")) {
				const auto & aliasv = itr->value.FindMember("alias")->value;
				if (aliasv.IsString()) {
					string2int[aliasv.GetString()] = rval;
				}
			}
		}

		const int min_val = int2string.begin()->first;
		const int max_val = int2string.rbegin()->first;
		const int val_num = (int)int2string.size();
		const char * etype = val.FindMember("type")->value.GetString();

		outf.pf(lvl, "/// @%s : string to enum\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "const char * name;\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl, "} str2e_%s[%d] = {\n", ename, (int)string2int.size());
		for (auto & i : string2int) {
			outf.pf(lvl+1, "{ \"%s\", %d },\n", i.first.c_str(), i.second);
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "/// @%s : enum to string\n", ename);
		outf.pf(lvl, "static const struct {\n");
		outf.pf(lvl+1, "%s val;\n", etype);
		outf.pf(lvl+1, "const char * name;\n");
		outf.pf(lvl, "} e2str_%s[%d] = {\n", ename, val_num);
		for (auto & i : int2string) {
			outf.pf(lvl+1, "{ %d, \"%s\" },\n", i.first, i.second.c_str());
		}
		outf.pf(lvl, "};\n\n");

		outf.pf(lvl, "static inline const char * e2pqxx(const enum %s src)\n", ename);
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
		outf.pf(lvl, "template<>\n");
		outf.pf(lvl, "inline void pqxx::internal::params::add_field(const enum %s & src)\n", ename);
		outf.pf(lvl, "{ add_field(e2pqxx(src)); }\n");

		outf.pf(lvl, "static inline void pqxx2c(enum %s & dst, const pqxx::const_row_iterator &it)\n", ename);
		outf.pf(lvl, "{ dst = (enum %s)search_val_binary(str2e_%s, it->c_str()); }\n", ename, ename);
	}

	outf.pf(0, "\n");
}

static const rapidjson::Value * find_dep_type(const rapidjson::Document & doc, const rapidjson::Value & tname)
{
	for (auto & pi : doc.GetArray()) {
		const rapidjson::Value & myname = pi.FindMember("name")->value;
		if (myname == tname) {
			return &pi;
		}
		if (pi.HasMember("alias")) {
			const auto & aliasName = pi.FindMember("alias")->value;
			if (aliasName.IsString()) {
				if (aliasName == tname) {
					return &pi;
				}
			}
			else if (aliasName.IsArray()) {
				for (const auto & ai : aliasName.GetArray()) {
					RAPIDJSON_ASSERT(ai.IsString());
					if (ai == tname) {
						return &pi;
					}
				}
			}
		}
	}

	return nullptr;
}

static void convert_nested_struct(rapidjson::Document & doc,
			const rapidjson::Value & val,
			std::vector<std::string> & pqkList,
			std::vector<std::string> & smnList,
			std::vector<bool> & isEnumList,
			const std::string pqkPrefix,
			const std::string smnPrefix,
			const std::size_t lvl)
{
	const rapidjson::Value & fields = val.FindMember("fields")->value;

	for (rapidjson::Value::ConstMemberIterator itr = fields.MemberBegin();
	     itr != fields.MemberEnd(); ++itr) {
		const char * mname = itr->name.GetString();
		const auto & mval = itr->value;
		const auto & memtype = mval.FindMember("type")->value;

		const std::string newpqkPrefix = (pqkPrefix == "" ? mname : (pqkPrefix + "_" + mname));
		const std::string newsmnPrefix = (smnPrefix == "" ? mname : (smnPrefix + "." + mname));

		const rapidjson::Value * deptype = nullptr;
		if (memtype.IsString()) {
			deptype = find_dep_type(doc, memtype);
		}
		else if (memtype.IsObject()) {
			const auto & memtypecate = memtype.FindMember("category")->value;
			if (std::string(memtypecate.GetString()) == "struct") {
				deptype = &memtype;
			}
		}
		else {
			RAPIDJSON_ASSERT("unkown struct member type!");
		}

		if (deptype) {
			RAPIDJSON_ASSERT(deptype->IsObject());
			const auto & depcat = deptype->FindMember("category")->value;
			const std::string catstr = std::string(depcat.GetString());
			if (catstr == "struct") {
				convert_nested_struct(doc, *deptype, pqkList, smnList, isEnumList, newpqkPrefix, newsmnPrefix, lvl);
			}
			else if (catstr == "enum") {
				pqkList.push_back(newpqkPrefix);
				smnList.push_back(newsmnPrefix);
				isEnumList.push_back(true);
			}
			else {
				RAPIDJSON_ASSERT("unkown struct member type!");
			}
		}
		else {
			/// atomic type
			pqkList.push_back(newpqkPrefix);
			smnList.push_back(newsmnPrefix);
			isEnumList.push_back(false);
		}
	}
}

static void convert_struct(rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl)
{
	RAPIDJSON_ASSERT(val.HasMember("name"));

	std::vector<std::string> pqkList;
	std::vector<std::string> smnList;
	std::vector<bool> isEnumList;
	convert_nested_struct(doc, val, pqkList, smnList, isEnumList, "", "", lvl);

	auto & outf = s_outf_pqxx_converter;
	auto & hppoutf = s_outf_pqxx_hpp;
	const char * sname =val.FindMember("name")->value.GetString();

	/// PQKL
	std::string pqkStr;
	for (auto & i : pqkList) {
		if (pqkStr != "") {
			pqkStr.append(",");
		}
		pqkStr.append(i);
	}
	hppoutf.pf(lvl, "/// pq key list for %s\n", sname);
	hppoutf.pf(lvl, "#define PQKL_%s \"%s\"\n", sname, pqkStr.c_str());

	/// PQOL
	std::string pqoStr;
	for (size_t i = 0; i < pqkList.size(); ++i) {
		if (pqoStr != "") {
			pqoStr.append(",");
		}
		pqoStr.append("$" + std::to_string(i+1));
	}
	hppoutf.pf(lvl, "/// pq occupy symbol list for %s\n", sname);
	hppoutf.pf(lvl, "#define PQOL_%s \"%s\"\n", sname, pqoStr.c_str());

	/// PQMS
	hppoutf.pf(lvl, "/// pq member size of %s\n", sname);
	hppoutf.pf(lvl, "#define PQMS_%s \"%d\"\n", sname, (int)smnList.size());

	/// SAVE
	hppoutf.pf(lvl, "template<>\n");
	hppoutf.pf(lvl, "void pqxx::internal::params::add_field(const %s & src);\n", sname);
	outf.pf(lvl, "template<>\n");
	outf.pf(lvl, "void pqxx::internal::params::add_field(const %s & src)\n", sname);
	outf.pf(lvl, "{\n");
	for (size_t i = 0; i < smnList.size(); ++i) {
		const char * mn = smnList[i].c_str();
		outf.pf(lvl+1, "add_field(src.%s);\n", mn);
	}
	outf.pf(lvl, "}\n\n");

	///LOAD
	hppoutf.pf(lvl, "pqxx::const_row_iterator pqxx2c(%s & dst, const pqxx::const_row_iterator & src);\n", sname);
	outf.pf(lvl, "pqxx::const_row_iterator pqxx2c(%s & dst, const pqxx::const_row_iterator & src)\n", sname);
	outf.pf(lvl, "{\n");
	outf.pf(lvl+1, "pqxx::const_row_iterator it = src;\n");
	for (size_t i = 0; i < smnList.size(); ++i) {
		const char * const mn = smnList[i].c_str();
		outf.pf(lvl+1, "pqxx2c(dst.%s, it); ++it;\n", mn);
	}
	outf.pf(lvl+1, "return it;\n");
	outf.pf(lvl, "}\n\n");

	for (std::size_t i = 0; i < pqkList.size(); ++i) {
		std::cout << "\t\t" << pqkList[i] << " : " << smnList[i] << std::endl;
	}
}

static std::map<
	const std::string,
	std::function<void (rapidjson::Document &, rapidjson::Value &, const std::size_t)>
> func_pqxx_generator = {
	{"enum", convert_enum},
	{"struct", convert_struct},
};

void generate_pqxx_converter(rapidjson::Document & d)
{
	for (auto & pi : s_pqxx_order) {
		rapidjson::Value & val = *pi;
		if (val.HasMember("category")) {
			rapidjson::Value & cat = val.FindMember("category")->value;
			func_pqxx_generator.at(cat.GetString())(d, val, 0);
		}
	}
}
