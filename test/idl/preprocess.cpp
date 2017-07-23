#include "parser.hpp"
#include <iostream>

std::map<
	const std::string,
	rapidjson::Value *
> s_type_lists;

std::map<
	const std::string,
	rapidjson::Value *
> s_alias_lists;

std::vector<rapidjson::Value *> s_type_order;
std::vector<rapidjson::Value *> s_pqxx_order;

std::map<
	const std::string,
	const rapidjson::Value *
> s_msg_lists;

std::vector<rapidjson::Value *> s_msg_order;
std::vector<rapidjson::Value *> s_export_order;

#if 0
bool is_as_msg(const rapidjson::Value & val)
{
	RAPIDJSON_ASSERT(val.HasMember("category"));
	RAPIDJSON_ASSERT(val.HasMember("msgid"));
	RAPIDJSON_ASSERT(val.FindMember("msgid")->value.IsArray());
	if (val.FindMember("msgid")->value.Size() > 0) {
		return true;
	}
	return false;
}
#endif

void ensure_has_object_mem(rapidjson::Value & _val, rapidjson::Value::StringRefType _name, rapidjson::Document & _doc)
{
	if (!_val.HasMember(_name)) {
		rapidjson::Value v(rapidjson::kObjectType);
		_val.AddMember(_name, v, _doc.GetAllocator());
	}
	RAPIDJSON_ASSERT(_val.FindMember(_name)->value.IsObject());
}

void ensure_has_array_mem(rapidjson::Value & _val, rapidjson::Value::StringRefType _name, rapidjson::Document & _doc)
{
	if (!_val.HasMember(_name)) {
		rapidjson::Value v(rapidjson::kArrayType);
		_val.AddMember(_name, v, _doc.GetAllocator());
	}
	else {
		rapidjson::Value & _mem_val = _val.FindMember(_name)->value;
		if (!_mem_val.IsArray()) {
			rapidjson::Value tmp;
			tmp = _mem_val;
			_mem_val.SetArray();
			_mem_val.PushBack(tmp, _doc.GetAllocator());
		}
	}
	RAPIDJSON_ASSERT(_val.FindMember(_name)->value.IsArray());
}

void ensure_has_bool_mem(rapidjson::Value & _val, rapidjson::Value::StringRefType _name, rapidjson::Document & _doc)
{
	if (!_val.HasMember(_name)) {
		rapidjson::Value v(rapidjson::kFalseType);
		_val.AddMember(_name, v, _doc.GetAllocator());
	}

	RAPIDJSON_ASSERT(_val.FindMember(_name)->value.IsBool());
}

rapidjson::Value * find_stype(rapidjson::Document & d, rapidjson::Value & tname)
{
	for (auto & pi : d.GetArray()) {
		auto & sname = pi.FindMember("name")->value;
		if (sname == tname) {
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

static void add_pqxx(rapidjson::Document & d, rapidjson::Value & s, bool force_add)
{
	for (auto i : s_pqxx_order) {
		if (i == &s) {
			return;
		}
	}

	/// @note: if 'force_add', we will add it into list,
	///        or not, which is used for nested struct member.
	///        for nesting case, we will expand every struct directly
	///        when generating related serializing code.
	if (std::string(s.FindMember("category")->value.GetString()) == "struct"
	    && s.HasMember("fields")) {
		rapidjson::Value & fields = s.FindMember("fields")->value;
		RAPIDJSON_ASSERT(fields.IsObject());
		for (rapidjson::Value::MemberIterator itr = fields.MemberBegin(); itr != fields.MemberEnd(); ++itr) {
			//auto & mname = itr->name;
			auto & mval = itr->value;
			rapidjson::Value * pFieldType = nullptr;
			if (mval.IsString()) {
				pFieldType = find_stype(d, mval);
			}
			else if (mval.IsObject()) {
				if (mval.HasMember("category")) {
					pFieldType = &mval;
				}
				else {
					auto & tv = mval.FindMember("type")->value;
					if (tv.IsString()) {
						pFieldType = find_stype(d, tv);
					}
					else if (tv.IsObject()) {
						pFieldType = &tv;
					}
				}
			}
			if (pFieldType) {
				RAPIDJSON_ASSERT(pFieldType->IsObject());
				RAPIDJSON_ASSERT(pFieldType->HasMember("category"));
				const auto & cate = pFieldType->FindMember("category")->value;
				if (cate == rapidjson::Value("struct")) {
					add_pqxx(d, *pFieldType, false);
				}
				else if (cate == rapidjson::Value("enum")) {
					add_pqxx(d, *pFieldType, true);
				}
			}
		}
	}

	if (force_add) {
		s_pqxx_order.push_back(&s);
	}
}

void preprocess_types(rapidjson::Document & d)
{
	/// pre record types
	for (auto & itr : d.GetArray()) {
		RAPIDJSON_ASSERT(itr.HasMember("category"));
		RAPIDJSON_ASSERT(itr.HasMember("name"));
		ensure_has_array_mem(itr, "attr", d);
		ensure_has_array_mem(itr, "msgid", d);
		ensure_has_object_mem(itr, "fields", d);
		ensure_has_array_mem(itr, "alias", d);
		s_type_lists[itr.FindMember("name")->value.GetString()] = &itr;
		for (auto & alias : itr.FindMember("alias")->value.GetArray()) {
			s_alias_lists[alias.GetString()] = & itr;
		}
		s_type_order.push_back(&itr);

	}

	/// messages enum
	for (auto & itr : d.GetArray()) {
		bool has_attr_msg = false;
		rapidjson::Value & attr_val = itr.FindMember("attr")->value;
		for (auto & sattr : attr_val.GetArray()) {
			RAPIDJSON_ASSERT(sattr.IsString());
			if (sattr == "msg") {
				has_attr_msg = true;
				attr_val.GetArray().Erase(&sattr);
			}
			else if (sattr == "export") {
				s_export_order.push_back(&itr);
			}
			else if (sattr == "pqxx") {
				add_pqxx(d, itr, true);
			}
		}

		rapidjson::Value & msgid_val = itr.FindMember("msgid")->value;
		if (has_attr_msg) {
			RAPIDJSON_ASSERT(itr.HasMember("name"));
			rapidjson::Value new_name(itr.FindMember("name")->value, d.GetAllocator());
			msgid_val.PushBack(new_name, d.GetAllocator());
		}

		for (const auto & smsgid : msgid_val.GetArray()) {
			RAPIDJSON_ASSERT(smsgid.IsString());
			s_msg_lists[smsgid.GetString()] = &itr;
		}

		if (msgid_val.GetArray().Size() > 0) {
			s_msg_order.push_back(&itr);
		}

		if (msgid_val.GetArray().Size() == 0) {
			itr.RemoveMember("msgid");
		}
	}

	for (auto i : s_pqxx_order) {
		std::cout << "\t" << i->FindMember("name")->value.GetString() << std::endl;;
	}
}
