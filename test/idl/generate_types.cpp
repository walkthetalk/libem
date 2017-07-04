#include "parser.hpp"


void process_type_struct(rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl);
void process_type_enum(rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl);

std::map<
	const std::string,
	std::function<void (rapidjson::Document &, rapidjson::Value &, const std::size_t lvl)>
> type_processor = {
	{"struct", process_type_struct},
	{"enum", process_type_enum},
};

void process_var_rawarray(const char * name, rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl);
void process_var_struct(const char * name, rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl);
//static void process_var_enum(const char * name, const rapidjson::Value & val, const std::size_t lvl);
std::map<
	const std::string,
	std::function<void (const char *, rapidjson::Document & doc, rapidjson::Value &, const std::size_t lvl)>
> var_processor = {
	{"struct", process_var_struct},
	{"rawarray", process_var_rawarray},
};

void generate_types(rapidjson::Document & d)
{
	//for (auto & itr : d.GetArray()) {
	for (auto & pitr : s_type_order) {
		rapidjson::Value & itr = *pitr;
		RAPIDJSON_ASSERT(itr.HasMember("category"));
		const char * category = itr.FindMember("category")->value.GetString();
		type_processor.at(category)(d, itr, 0);

		if (itr.HasMember("val_size")) {
			s_outf_types.pf(0, ";\n");
			const char * const type_str = itr.FindMember("type")->value.GetString();
			s_outf_types.pf(0, "template<> struct enum_info<enum %s> {\n",
					itr.FindMember("name")->value.GetString());
			s_outf_types.pf(1, "static constexpr %s min = %d;\n",type_str,
					itr.FindMember("min")->value.GetInt());
			s_outf_types.pf(1, "static constexpr %s max = %d;\n",type_str,
					itr.FindMember("max")->value.GetInt());
			s_outf_types.pf(1, "static constexpr %s size = %d;\n",type_str,
					itr.FindMember("val_size")->value.GetInt());
			s_outf_types.pf(0, "}");
		}
		s_outf_types.pf_type_tail();
	}
}


#define ENSURE_IF(_cond, _val, _name, _doc) \
do { \
	if (!_cond) { \
		rapidjson::Value tmp; \
		tmp = _val; \
		_val.SetObject(); \
		_val.AddMember(_name, tmp, _doc.GetAllocator()); \
		RAPIDJSON_ASSERT(_val.HasMember(_name)); \
	} \
	RAPIDJSON_ASSERT(_cond); \
} while (0)

#define ENSURE_OBJECT(_val, _name, _doc) \
	ENSURE_IF(_val.IsObject(), _val, _name, _doc)

#define ENSURE_ARRAY(_val, _name, _doc) \
do { \
	if (!_val.IsArray()) { \
		rapidjson::Value tmp; \
		tmp = _val; \
		_val.SetArray(); \
		_val.PushBack(tmp, _doc.GetAllocator()); \
	} \
} while (0)


void process_var_struct(const char * name, rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl)
{
	process_type_struct(doc, val, lvl);
	s_outf_types.pf(0, " %s", name);
}

void process_type_struct(rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl)
{
	const char * name = "";
	if (val.HasMember("name")) {
		name = val.FindMember("name")->value.GetString();
	}

	const rapidjson::Value & alias_v = val.FindMember("alias")->value;

	bool has_attr_final = false;
	if (val.HasMember("attr")) {
		const rapidjson::Value & attrs = val.FindMember("attr")->value;
		if (attrs == "final") {
			has_attr_final = true;
		}
		else if (attrs.IsArray()) {
			for (auto & attr : attrs.GetArray()) {
				if (attr == "final") {
					has_attr_final = true;
				}
			}
		}
	}

	s_outf_types.pf(lvl, "%sstruct%s%s%s {",
		      alias_v.GetArray().Size() > 0 ? "typedef " : "",
		      name[0] == '\0' ? "" : " ",
		      name[0] == '\0' ? "" : name,
		      has_attr_final ? " final" : ""
	);

	RAPIDJSON_ASSERT(val.HasMember("fields"));
	rapidjson::Value & fields = val.FindMember("fields")->value;
	RAPIDJSON_ASSERT(fields.IsObject());
	if (fields.MemberCount() > 0) {
		s_outf_types.pf(0,"\n");
	}

	for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
		const char * mname = itr->name.GetString();
		auto & mval = itr->value;
		ENSURE_OBJECT(mval, "type", doc);
		ENSURE_IF(!mval.HasMember("category"), mval, "type", doc);

		RAPIDJSON_ASSERT(mval.HasMember("type"));
		rapidjson::Value & type_val = mval.FindMember("type")->value;
		if (type_val.IsString()) {
			const char * type_prefix = "";
			if (s_type_lists.find(type_val.GetString()) != s_type_lists.end()) {
				const rapidjson::Value & cat_val = s_type_lists[type_val.GetString()]->FindMember("category")->value;
				if (cat_val == "struct") {
					type_prefix = "struct ";
				}
				else if (cat_val == "enum") {
					type_prefix = "enum ";
				}
			}
			s_outf_types.pf(lvl+1, "%s%s %s", type_prefix, type_val.GetString(), mname);
		}
		else if (type_val.IsObject()) {
			RAPIDJSON_ASSERT(type_val.HasMember("category"));
			const char * category = type_val.FindMember("category")->value.GetString();
			var_processor.at(category)(mname, doc, type_val, lvl+1);
		}
		s_outf_types.pf(0, ";");

		bool has_unit = mval.HasMember("unit");
		bool has_range = mval.HasMember("range");
		if (has_unit || has_range) {
			s_outf_types.pf(0, "\t///");
		}
		if (mval.HasMember("unit")) {
			s_outf_types.pf(0, " @unit: %s", mval.FindMember("unit")->value.GetString());
		}
		if (mval.HasMember("range")) {
			s_outf_types.pf(0, " @range: %s", mval.FindMember("range")->value.GetString());
		}

		s_outf_types.pf(0, "\n");
	}

	s_outf_types.pf(lvl, "}");
	bool is_first_alias = true;
	for (auto & itr : alias_v.GetArray()) {
		if (is_first_alias) {
			s_outf_types.pf(0, " %s", itr.GetString());
			is_first_alias = false;
		}
		else {
			s_outf_types.pf(0, ", %s", itr.GetString());
		}
	}
}

void process_var_rawarray(const char * name, rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl)
{
	RAPIDJSON_ASSERT(val.HasMember("type"));
	rapidjson::Value & type_val = val.FindMember("type")->value;
	if (type_val.IsString()) {
		const char * type_prefix = "";
		if (s_type_lists.find(type_val.GetString()) != s_type_lists.end()) {
			const rapidjson::Value & cat_val = s_type_lists[type_val.GetString()]->FindMember("category")->value;
			if (cat_val == "struct") {
				type_prefix = "struct ";
			}
			else if (cat_val == "enum") {
				type_prefix = "enum ";
			}
		}
		s_outf_types.pf(lvl, "%s%s", type_prefix, type_val.GetString());
	}
	else if (type_val.IsObject()) {
		RAPIDJSON_ASSERT(type_val.HasMember("category"));
		type_processor[type_val.FindMember("category")->value.GetString()](doc, type_val, lvl);
	}
	s_outf_types.pf(0, " %s", name);

	RAPIDJSON_ASSERT(val.HasMember("rank"));
	const rapidjson::Value & ranks = val.FindMember("rank")->value;
	RAPIDJSON_ASSERT(ranks.IsNumber() || ranks.IsArray());
	if (ranks.IsNumber()) {
		s_outf_types.pf(0, "[%u]", ranks.GetUint());
	}
	else if (ranks.IsArray()) {
		for (auto& r : ranks.GetArray()) {
			s_outf_types.pf(0, "[%u]", r.GetUint());
		}
	}
}

void process_type_enum(rapidjson::Document & doc, rapidjson::Value & val, const std::size_t lvl)
{
	const char * name = "";
	if (val.HasMember("name")) {
		name = val.FindMember("name")->value.GetString();
	}

	bool has_attr_class = false;
	bool is_flag = false;
	ensure_has_array_mem(val, "attr", doc);
	rapidjson::Value & attrs = val.FindMember("attr")->value;
	for (auto & attr : attrs.GetArray()) {
		RAPIDJSON_ASSERT(attr.IsString());
		if (attr == "class") {
			has_attr_class = true;
		}
		else if (attr == "flag") {
			is_flag = true;
		}
	}

	s_outf_types.pf(lvl, "enum%s%s%s", has_attr_class ? " class" : "",
		      name[0] == '\0' ? "" : " ",
		      name[0] == '\0' ? "" : name
	);
	if (val.HasMember("type")) {
		const char * basetype = val.FindMember("type")->value.GetString();
		s_outf_types.pf(0, " : %s", basetype);
	}
	else {
		val.AddMember("type", "int", doc.GetAllocator());
	}
	s_outf_types.pf(0, " {\n");

	if (val.HasMember("fields")) {
		rapidjson::Value & fields = val.FindMember("fields")->value;
		RAPIDJSON_ASSERT(fields.IsObject());

		int enum_val = 0;
		if (val.HasMember("start")) {
			RAPIDJSON_ASSERT(val.FindMember("start")->value.IsInt());
			enum_val = val.FindMember("start")->value.GetInt();
		}

		if (is_flag) {
			for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
				const char * mname = itr->name.GetString();
				auto & mval = itr->value;
				ENSURE_OBJECT(mval, "value", doc);

				auto & rVal = mval.FindMember("value")->value;
				RAPIDJSON_ASSERT(rVal.IsNumber() || rVal.IsString());
				if (rVal.IsNumber()) {
					/// @note must be deep-copy
					char buf[64] = "";
					int str_len = std::sprintf(buf, "%#x", rVal.GetUint());
					RAPIDJSON_ASSERT(0 < str_len && str_len < (int)sizeof(buf));
					rVal = rapidjson::Value(buf, doc.GetAllocator());
				}

				s_outf_types.pf(lvl+1, "%s = %s", mname, rVal.GetString());

				if (mval.HasMember("comment")) {
					s_outf_types.pf(0, ",\t/// %s\n", mval.FindMember("comment")->value.GetString());
				}
				else {
					s_outf_types.pf(0, ",\n");
				}
			}
		}
		else {
			int val_min = std::numeric_limits<int>::max();
			int val_max = std::numeric_limits<int>::min();
			std::map<int, std::string> int2string;
			for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
				const char * mname = itr->name.GetString();
				auto & mval = itr->value;
				ENSURE_OBJECT(mval, "value", doc);

				if (mval.HasMember("value")) {
					const auto & rval = mval.FindMember("value")->value;
					RAPIDJSON_ASSERT(rval.IsInt());
					enum_val = rval.GetInt();
				}
				else {
					mval.AddMember("value", enum_val, doc.GetAllocator());
				}

				if (enum_val < val_min) val_min = enum_val;
				if (val_max < enum_val) val_max = enum_val;

				int2string[enum_val] = mname;
				s_outf_types.pf(lvl+1, "%s = %d", mname, enum_val);
				++enum_val;

				if (mval.HasMember("comment")) {
					s_outf_types.pf(0, ",\t/// %s\n", mval.FindMember("comment")->value.GetString());
				}
				else {
					s_outf_types.pf(0, ",\n");
				}
			}
			if (val_min <= val_max) {
				val.AddMember("min", val_min, doc.GetAllocator());
				val.AddMember("max", val_max, doc.GetAllocator());
				s_outf_types.pf(lvl+1, "/// @min : %d, @max : %d\n", val_min, val_max);
				val.AddMember("val_size", int2string.size(), doc.GetAllocator());
			}
		}
	}

	s_outf_types.pf(lvl, "}");
}
