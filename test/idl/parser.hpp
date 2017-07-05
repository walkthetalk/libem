#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "rapidjson/document.h"
#include "filebuf.hpp"

/**
 * for type convert
 */
extern std::map<
	const std::string,
	rapidjson::Value *
> s_type_lists;

extern std::map<
	const std::string,
	rapidjson::Value *
> s_alias_lists;

extern std::vector<rapidjson::Value *> s_type_order;

/**
 * for msgs enum
 */
extern std::map<
	const std::string,
	const rapidjson::Value *
> s_msg_lists;
extern std::vector<rapidjson::Value *> s_msg_order;

static const char * const s_mid_under_type = "uint16_t";

void ensure_has_object_mem(rapidjson::Value & _val, rapidjson::Value::StringRefType _name, rapidjson::Document & _doc);
void ensure_has_array_mem(rapidjson::Value & _val, rapidjson::Value::StringRefType _name, rapidjson::Document & _doc);

void preprocess_types(rapidjson::Document & d);
void generate_types(rapidjson::Document & d);
void generate_id(rapidjson::Document & d);
void generate_converter(rapidjson::Document & d);
void generate_wsmsgs(rapidjson::Document & d);
