
#include <stdexcept>
#include <cstring>

#include "libwebsockets.h"
#include "rapidjson/writer.h"

#include "c2json.hpp"
#include "json2c.hpp"

#include "in_out_wrapper.hpp"

#include "jmsg/jmsg_types.hpp"
#include "jmsg/jmsg_sender.hpp"
#include "jmsg/jmsg_rcver.hpp"

template<typename T, typename V>
rapidjson::Value search_name_directly(const T &flist, const V val)
{
	if (0 <= val && val < std::extent<T>::value) {
		rapidjson::Value::StringRefType str(flist[val].name);
		return rapidjson::Value(str);
	}

	throw std::invalid_argument("enum val error!");
}

template<typename T, typename V>
rapidjson::Value search_name_binary(const T &flist, const V val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		V cur = flist[mid].val;
		if (cur == val) {
			rapidjson::Value::StringRefType str(flist[mid].name);
			return  rapidjson::Value(str);
		}
		else if (cur < val) {
			head = mid;
		}
		else {
			tail = mid;
		}
	};

	throw std::invalid_argument("enum val error!");
}

template<typename T>
auto search_val_binary(const T & flist, const rapidjson::Value & name) -> decltype(flist[0].val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		const rapidjson::Value::StringRefType & cur = flist[mid].name;
		int cmp_result = std::strcmp(cur.s, name.GetString());
		if (cmp_result == 0) {
			return flist[mid].val;
		}
		else if (cmp_result < 0) {
			head = mid;
		}
		else {
			tail = mid;
		}
	};

	throw std::invalid_argument("enum name error!");
}

template<typename T>
auto flag2e(const T & flist, const rapidjson::Value & name_array) -> decltype(flist[0].val)
{
	RAPIDJSON_ASSERT(name_array.IsArray());
	decltype(flist[0].val) ret = 0;

	for (const auto & name : name_array.GetArray()) {
		for (const auto & i : flist) {
			if (name == i.name) {
				ret |= i.val;
				break;
			}
		}
	}
	return ret;
}

template<typename T, typename V>
rapidjson::Value e2flag(rapidjson::Document & doc, const T &flist, const V val)
{
	const decltype(flist[0].val) dval = val;
	rapidjson::Value ret(rapidjson::kArrayType);

	for (const auto & i : flist) {
		if ((dval & i.val) == i.val) {
			ret.PushBack(i.name, doc.GetAllocator());
		}
	}
	return ret;
}
/// @ifd_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint32_t val;
} str2e_ifd_t[5] = {
	{ "ifd_all", 0xFFFFFFFF },
	{ "ifd_cant_identify", 0x80000000 },
	{ "ifd_end_crude", 0x00000001 },
	{ "ifd_horizontal_angle", 0x00000002 },
	{ "ifd_vertical_angle", 0x00000004 },
};

/// @ifd_t : enum to string
static const struct {
	uint32_t val;
	rapidjson::Value::StringRefType name;
} e2str_ifd_t[5] = {
	{ 0x00000001, "ifd_end_crude" },
	{ 0x00000002, "ifd_horizontal_angle" },
	{ 0x00000004, "ifd_vertical_angle" },
	{ 0x80000000, "ifd_cant_identify" },
	{ 0xFFFFFFFF, "ifd_all" },
};

static inline rapidjson::Value c2json(rapidjson::Document & jd, const enum ifd_t src)
{ return e2flag(jd, e2str_ifd_t, (uint32_t)src); }

static inline void json2c(enum ifd_t & dst, const rapidjson::Value & src)
{ dst = (enum ifd_t)flag2e(str2e_ifd_t, src); }

/// @struct ifd_line
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct ifd_line & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "dbmp", v, src.dbmp);
	ENC_MEM(jd, "h_angle", v, src.h_angle);
	ENC_MEM(jd, "v_angle", v, src.v_angle);
	ENC_MEM(jd, "wrap_diameter", v, src.wrap_diameter);

	return v;
}
static inline void json2c(struct ifd_line & dst, const rapidjson::Value & src)
{
	DEC_MEM("dbmp", src, dst.dbmp);
	DEC_MEM("h_angle", src, dst.h_angle);
	DEC_MEM("v_angle", src, dst.v_angle);
	DEC_MEM("wrap_diameter", src, dst.wrap_diameter);
}

/// @struct img_defects
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct img_defects & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "yzl", v, src.yzl);
	ENC_MEM(jd, "yzr", v, src.yzr);
	ENC_MEM(jd, "xzl", v, src.xzl);
	ENC_MEM(jd, "xzr", v, src.xzr);
	ENC_MEM(jd, "yz_hangle_intersect", v, src.yz_hangle_intersect);
	ENC_MEM(jd, "xz_hangle_intersect", v, src.xz_hangle_intersect);
	ENC_MEM(jd, "yz_ref_img", v, src.yz_ref_img);
	ENC_MEM(jd, "xz_ref_img", v, src.xz_ref_img);
	ENC_MEM(jd, "yz_img", v, src.yz_img);
	ENC_MEM(jd, "xz_img", v, src.xz_img);

	return v;
}
static inline void json2c(struct img_defects & dst, const rapidjson::Value & src)
{
	DEC_MEM("yzl", src, dst.yzl);
	DEC_MEM("yzr", src, dst.yzr);
	DEC_MEM("xzl", src, dst.xzl);
	DEC_MEM("xzr", src, dst.xzr);
	DEC_MEM("yz_hangle_intersect", src, dst.yz_hangle_intersect);
	DEC_MEM("xz_hangle_intersect", src, dst.xz_hangle_intersect);
	DEC_MEM("yz_ref_img", src, dst.yz_ref_img);
	DEC_MEM("xz_ref_img", src, dst.xz_ref_img);
	DEC_MEM("yz_img", src, dst.yz_img);
	DEC_MEM("xz_img", src, dst.xz_img);
}

/// @svc_fs_state_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint16_t val;
} str2e_svc_fs_state_t[23] = {
	{ "calibrating", 5 },
	{ "clring", 7 },
	{ "defect_detecting", 9 },
	{ "discharge1", 16 },
	{ "discharge2", 17 },
	{ "discharge_manual", 18 },
	{ "entering", 3 },
	{ "fiber_rec", 10 },
	{ "finished", 21 },
	{ "focusing", 8 },
	{ "idle", 1 },
	{ "loss_estimating", 19 },
	{ "pause1", 12 },
	{ "pause2", 14 },
	{ "pre_splice", 15 },
	{ "precise_calibrating", 13 },
	{ "push1", 4 },
	{ "push2", 11 },
	{ "ready", 2 },
	{ "reseting", 0 },
	{ "tension_testing", 20 },
	{ "wait_reset", 22 },
	{ "waiting", 6 },
};

/// @svc_fs_state_t : enum to string
static const struct {
	uint16_t val;
	rapidjson::Value::StringRefType name;
} e2str_svc_fs_state_t[23] = {
	{ 0, "reseting" },
	{ 1, "idle" },
	{ 2, "ready" },
	{ 3, "entering" },
	{ 4, "push1" },
	{ 5, "calibrating" },
	{ 6, "waiting" },
	{ 7, "clring" },
	{ 8, "focusing" },
	{ 9, "defect_detecting" },
	{ 10, "fiber_rec" },
	{ 11, "push2" },
	{ 12, "pause1" },
	{ 13, "precise_calibrating" },
	{ 14, "pause2" },
	{ 15, "pre_splice" },
	{ 16, "discharge1" },
	{ 17, "discharge2" },
	{ 18, "discharge_manual" },
	{ 19, "loss_estimating" },
	{ 20, "tension_testing" },
	{ 21, "finished" },
	{ 22, "wait_reset" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum svc_fs_state_t src)
{ return search_name_directly(e2str_svc_fs_state_t, (uint16_t)src); }

static inline void json2c(enum svc_fs_state_t & dst, const rapidjson::Value & src)
{ dst = (enum svc_fs_state_t)search_val_binary(str2e_svc_fs_state_t, src); }

/// @svc_heat_state_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint16_t val;
} str2e_svc_heat_state_t[5] = {
	{ "heat_ascending", 5 },
	{ "heat_descending", 7 },
	{ "heat_idle", 0 },
	{ "heat_ready", 4 },
	{ "heat_stabling", 6 },
};

/// @svc_heat_state_t : enum to string
static const struct {
	uint16_t val;
	rapidjson::Value::StringRefType name;
} e2str_svc_heat_state_t[5] = {
	{ 0, "heat_idle" },
	{ 4, "heat_ready" },
	{ 5, "heat_ascending" },
	{ 6, "heat_stabling" },
	{ 7, "heat_descending" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum svc_heat_state_t src)
{ return search_name_binary(e2str_svc_heat_state_t, (uint16_t)src); }

static inline void json2c(enum svc_heat_state_t & dst, const rapidjson::Value & src)
{ dst = (enum svc_heat_state_t)search_val_binary(str2e_svc_heat_state_t, src); }

/// @motorId_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_motorId_t[4] = {
	{ "LZ", 0 },
	{ "RZ", 1 },
	{ "X", 2 },
	{ "Y", 3 },
};

/// @motorId_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_motorId_t[4] = {
	{ 0, "LZ" },
	{ 1, "RZ" },
	{ 2, "X" },
	{ 3, "Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum motorId_t src)
{ return search_name_directly(e2str_motorId_t, (uint8_t)src); }

static inline void json2c(enum motorId_t & dst, const rapidjson::Value & src)
{ dst = (enum motorId_t)search_val_binary(str2e_motorId_t, src); }

/// @fs_display_mode_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_fs_display_mode_t[5] = {
	{ "LR", 3 },
	{ "NO", 4 },
	{ "TB", 2 },
	{ "X", 0 },
	{ "Y", 1 },
};

/// @fs_display_mode_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_fs_display_mode_t[5] = {
	{ 0, "X" },
	{ 1, "Y" },
	{ 2, "TB" },
	{ 3, "LR" },
	{ 4, "NO" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_display_mode_t src)
{ return search_name_directly(e2str_fs_display_mode_t, (uint8_t)src); }

static inline void json2c(enum fs_display_mode_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_display_mode_t)search_val_binary(str2e_fs_display_mode_t, src); }

/// @fs_err_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_fs_err_t[28] = {
	{ "abnormal_arc", 7 },
	{ "arc_mag_overflow", 27 },
	{ "arc_off_center", 25 },
	{ "arc_time_zero", 14 },
	{ "calibrate_timeout", 12 },
	{ "cmos_exposure", 23 },
	{ "cover_openned", 1 },
	{ "failed", 26 },
	{ "fiber_broken", 9 },
	{ "fiber_cross_over", 4 },
	{ "fiber_defect", 3 },
	{ "fiber_off_center", 5 },
	{ "fiber_offside", 22 },
	{ "focus_x", 18 },
	{ "focus_y", 19 },
	{ "ignore", 15 },
	{ "img_brightness", 6 },
	{ "img_process_error", 20 },
	{ "loss_estimate", 24 },
	{ "no_fiber", 2 },
	{ "push_timeout", 11 },
	{ "quit_midway", 10 },
	{ "reset_timeout", 13 },
	{ "revise1_mag", 16 },
	{ "revise2_mag", 17 },
	{ "success", 0 },
	{ "system_error", 21 },
	{ "tense_test_fail", 8 },
};

/// @fs_err_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_fs_err_t[28] = {
	{ 0, "success" },
	{ 1, "cover_openned" },
	{ 2, "no_fiber" },
	{ 3, "fiber_defect" },
	{ 4, "fiber_cross_over" },
	{ 5, "fiber_off_center" },
	{ 6, "img_brightness" },
	{ 7, "abnormal_arc" },
	{ 8, "tense_test_fail" },
	{ 9, "fiber_broken" },
	{ 10, "quit_midway" },
	{ 11, "push_timeout" },
	{ 12, "calibrate_timeout" },
	{ 13, "reset_timeout" },
	{ 14, "arc_time_zero" },
	{ 15, "ignore" },
	{ 16, "revise1_mag" },
	{ 17, "revise2_mag" },
	{ 18, "focus_x" },
	{ 19, "focus_y" },
	{ 20, "img_process_error" },
	{ 21, "system_error" },
	{ 22, "fiber_offside" },
	{ 23, "cmos_exposure" },
	{ 24, "loss_estimate" },
	{ 25, "arc_off_center" },
	{ 26, "failed" },
	{ 27, "arc_mag_overflow" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_err_t src)
{ return search_name_directly(e2str_fs_err_t, (uint8_t)src); }

static inline void json2c(enum fs_err_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_err_t)search_val_binary(str2e_fs_err_t, src); }

/// @ledId_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_ledId_t[2] = {
	{ "CMOS_X", 0 },
	{ "CMOS_Y", 1 },
};

/// @ledId_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_ledId_t[2] = {
	{ 0, "CMOS_X" },
	{ 1, "CMOS_Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum ledId_t src)
{ return search_name_directly(e2str_ledId_t, (uint8_t)src); }

static inline void json2c(enum ledId_t & dst, const rapidjson::Value & src)
{ dst = (enum ledId_t)search_val_binary(str2e_ledId_t, src); }

/// @cmosId_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_cmosId_t[2] = {
	{ "X", 0 },
	{ "Y", 1 },
};

/// @cmosId_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_cmosId_t[2] = {
	{ 0, "X" },
	{ 1, "Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum cmosId_t src)
{ return search_name_directly(e2str_cmosId_t, (uint8_t)src); }

static inline void json2c(enum cmosId_t & dst, const rapidjson::Value & src)
{ dst = (enum cmosId_t)search_val_binary(str2e_cmosId_t, src); }

/// @struct mag2shrink
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct mag2shrink & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "x", v, src.x);
	ENC_MEM(jd, "y", v, src.y);

	return v;
}
static inline void json2c(struct mag2shrink & dst, const rapidjson::Value & src)
{
	DEC_MEM("x", src, dst.x);
	DEC_MEM("y", src, dst.y);
}

/// @struct discharge_data
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct discharge_data & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "p", v, src.p);
	ENC_MEM(jd, "temp", v, src.temp);
	ENC_MEM(jd, "pressure", v, src.pressure);

	return v;
}
static inline void json2c(struct discharge_data & dst, const rapidjson::Value & src)
{
	DEC_MEM("p", src, dst.p);
	DEC_MEM("temp", src, dst.temp);
	DEC_MEM("pressure", src, dst.pressure);
}

/// @struct rt_revise_data
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct rt_revise_data & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "x_exposure", v, src.x_exposure);
	ENC_MEM(jd, "y_exposure", v, src.y_exposure);
	ENC_MEM(jd, "a3", v, src.a3);
	ENC_MEM(jd, "a2", v, src.a2);
	ENC_MEM(jd, "a1", v, src.a1);
	ENC_MEM(jd, "a0", v, src.a0);
	ENC_MEM(jd, "offset_auto", v, src.offset_auto);
	ENC_MEM(jd, "offset_cal", v, src.offset_cal);

	return v;
}
static inline void json2c(struct rt_revise_data & dst, const rapidjson::Value & src)
{
	DEC_MEM("x_exposure", src, dst.x_exposure);
	DEC_MEM("y_exposure", src, dst.y_exposure);
	DEC_MEM("a3", src, dst.a3);
	DEC_MEM("a2", src, dst.a2);
	DEC_MEM("a1", src, dst.a1);
	DEC_MEM("a0", src, dst.a0);
	DEC_MEM("offset_auto", src, dst.offset_auto);
	DEC_MEM("offset_cal", src, dst.offset_cal);
}

/// @fiber_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_fiber_t[4] = {
	{ "ds", 1 },
	{ "mm", 3 },
	{ "nz", 2 },
	{ "sm", 0 },
};

/// @fiber_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_fiber_t[4] = {
	{ 0, "sm" },
	{ 1, "ds" },
	{ 2, "nz" },
	{ 3, "mm" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fiber_t src)
{ return search_name_directly(e2str_fiber_t, (uint8_t)src); }

static inline void json2c(enum fiber_t & dst, const rapidjson::Value & src)
{ dst = (enum fiber_t)search_val_binary(str2e_fiber_t, src); }

/// @struct fiber_reco_data
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fiber_reco_data & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "data", v, src.data);

	return v;
}
static inline void json2c(struct fiber_reco_data & dst, const rapidjson::Value & src)
{
	DEC_MEM("data", src, dst.data);
}

/// @struct fiber_rec_info
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fiber_rec_info & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "ft", v, src.ft);
	ENC_MEM(jd, "wrap_diameter", v, src.wrap_diameter);
	ENC_MEM(jd, "core_diameter", v, src.core_diameter);

	return v;
}
static inline void json2c(struct fiber_rec_info & dst, const rapidjson::Value & src)
{
	DEC_MEM("ft", src, dst.ft);
	DEC_MEM("wrap_diameter", src, dst.wrap_diameter);
	DEC_MEM("core_diameter", src, dst.core_diameter);
}

/// @fs_pattern_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_fs_pattern_t[4] = {
	{ "automatic", 0 },
	{ "calibrate", 1 },
	{ "normal", 2 },
	{ "special", 3 },
};

/// @fs_pattern_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_fs_pattern_t[4] = {
	{ 0, "automatic" },
	{ 1, "calibrate" },
	{ 2, "normal" },
	{ 3, "special" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_pattern_t src)
{ return search_name_directly(e2str_fs_pattern_t, (uint8_t)src); }

static inline void json2c(enum fs_pattern_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, src); }

/// @loss_estimate_mode_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_loss_estimate_mode_t[4] = {
	{ "accurate", 1 },
	{ "cladding", 3 },
	{ "core", 2 },
	{ "off", 0 },
};

/// @loss_estimate_mode_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_loss_estimate_mode_t[4] = {
	{ 0, "off" },
	{ 1, "accurate" },
	{ 2, "core" },
	{ 3, "cladding" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum loss_estimate_mode_t src)
{ return search_name_directly(e2str_loss_estimate_mode_t, (uint8_t)src); }

static inline void json2c(enum loss_estimate_mode_t & dst, const rapidjson::Value & src)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, src); }

/// @shrink_tube_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_shrink_tube_t[3] = {
	{ "len_20mm", 0 },
	{ "len_40mm", 1 },
	{ "len_60mm", 2 },
};

/// @shrink_tube_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_shrink_tube_t[3] = {
	{ 0, "len_20mm" },
	{ 1, "len_40mm" },
	{ 2, "len_60mm" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum shrink_tube_t src)
{ return search_name_directly(e2str_shrink_tube_t, (uint8_t)src); }

static inline void json2c(enum shrink_tube_t & dst, const rapidjson::Value & src)
{ dst = (enum shrink_tube_t)search_val_binary(str2e_shrink_tube_t, src); }

/// @align_method_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	uint8_t val;
} str2e_align_method_t[4] = {
	{ "clad", 1 },
	{ "core", 2 },
	{ "fine", 0 },
	{ "manual", 3 },
};

/// @align_method_t : enum to string
static const struct {
	uint8_t val;
	rapidjson::Value::StringRefType name;
} e2str_align_method_t[4] = {
	{ 0, "fine" },
	{ 1, "clad" },
	{ 2, "core" },
	{ 3, "manual" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum align_method_t src)
{ return search_name_directly(e2str_align_method_t, (uint8_t)src); }

static inline void json2c(enum align_method_t & dst, const rapidjson::Value & src)
{ dst = (enum align_method_t)search_val_binary(str2e_align_method_t, src); }

/// @struct fs_base_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_base_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "FSPattern", v, src.FSPattern);
	ENC_MEM(jd, "FiberType", v, src.FiberType);
	ENC_MEM(jd, "FiberAlignment", v, src.FiberAlignment);
	ENC_MEM(jd, "XImageFocus", v, src.XImageFocus);
	ENC_MEM(jd, "YImageFocus", v, src.YImageFocus);
	ENC_MEM(jd, "FiberShift", v, src.FiberShift);
	ENC_MEM(jd, "DischargeStrengthAdjustment", v, src.DischargeStrengthAdjustment);
	ENC_MEM(jd, "TensionSet", v, src.TensionSet);
	ENC_MEM(jd, "CutAngleLimit", v, src.CutAngleLimit);
	ENC_MEM(jd, "LossLimit", v, src.LossLimit);
	ENC_MEM(jd, "FiberAngleLimit", v, src.FiberAngleLimit);
	ENC_MEM(jd, "CleanDischargeStrength", v, src.CleanDischargeStrength);
	ENC_MEM(jd, "CleanDischargeTime", v, src.CleanDischargeTime);
	ENC_MEM(jd, "FiberIntervalSetup", v, src.FiberIntervalSetup);
	ENC_MEM(jd, "FSPosSetup", v, src.FSPosSetup);
	ENC_MEM(jd, "FiberPreFSStrength", v, src.FiberPreFSStrength);
	ENC_MEM(jd, "FiberPreFSTime", v, src.FiberPreFSTime);
	ENC_MEM(jd, "FiberOverlapSetup", v, src.FiberOverlapSetup);
	ENC_MEM(jd, "Discharge1Strength", v, src.Discharge1Strength);
	ENC_MEM(jd, "Discharge1Time", v, src.Discharge1Time);
	ENC_MEM(jd, "Discharge2Strength", v, src.Discharge2Strength);
	ENC_MEM(jd, "Discharge2LastTime", v, src.Discharge2LastTime);
	ENC_MEM(jd, "Discharge2StartTime", v, src.Discharge2StartTime);
	ENC_MEM(jd, "Discharge2StopTime", v, src.Discharge2StopTime);
	ENC_MEM(jd, "ExtraManualDischargeTime", v, src.ExtraManualDischargeTime);
	ENC_MEM(jd, "LeftFSSpeed", v, src.LeftFSSpeed);
	ENC_MEM(jd, "RightFSSpeed", v, src.RightFSSpeed);
	ENC_MEM(jd, "ConeFS", v, src.ConeFS);
	ENC_MEM(jd, "ConeFSWaitTime", v, src.ConeFSWaitTime);
	ENC_MEM(jd, "ConeFSSpeed", v, src.ConeFSSpeed);
	ENC_MEM(jd, "ConeFSStretchLength", v, src.ConeFSStretchLength);
	ENC_MEM(jd, "LossEstimationMode", v, src.LossEstimationMode);
	ENC_MEM(jd, "LeftFiberMFD", v, src.LeftFiberMFD);
	ENC_MEM(jd, "RightFiberMFD", v, src.RightFiberMFD);
	ENC_MEM(jd, "LeastLoss", v, src.LeastLoss);
	ENC_MEM(jd, "RateOfSyntropyBending", v, src.RateOfSyntropyBending);
	ENC_MEM(jd, "RateOfReverseBending", v, src.RateOfReverseBending);
	ENC_MEM(jd, "MFDMismatchCoefficient", v, src.MFDMismatchCoefficient);
	ENC_MEM(jd, "AutoStart", v, src.AutoStart);
	ENC_MEM(jd, "Stop1", v, src.Stop1);
	ENC_MEM(jd, "Stop2", v, src.Stop2);
	ENC_MEM(jd, "CutAngle", v, src.CutAngle);
	ENC_MEM(jd, "OffsetData", v, src.OffsetData);
	ENC_MEM(jd, "ArcCorrectedValue", v, src.ArcCorrectedValue);
	ENC_MEM(jd, "Cut", v, src.Cut);
	ENC_MEM(jd, "Loss", v, src.Loss);
	ENC_MEM(jd, "FiberCoreAngle", v, src.FiberCoreAngle);
	ENC_MEM(jd, "Bubble", v, src.Bubble);
	ENC_MEM(jd, "Thick", v, src.Thick);
	ENC_MEM(jd, "Thin", v, src.Thin);
	ENC_MEM(jd, "AirPressure", v, src.AirPressure);
	ENC_MEM(jd, "Temperature", v, src.Temperature);
	ENC_MEM(jd, "RealTimeRevise", v, src.RealTimeRevise);
	ENC_MEM(jd, "ImgGap", v, src.ImgGap);
	ENC_MEM(jd, "ImgStop1", v, src.ImgStop1);
	ENC_MEM(jd, "ImgAlign", v, src.ImgAlign);
	ENC_MEM(jd, "ImgStop2", v, src.ImgStop2);
	ENC_MEM(jd, "ImgDischarge", v, src.ImgDischarge);
	ENC_MEM(jd, "ImgLossEstimation", v, src.ImgLossEstimation);
	ENC_MEM(jd, "FiberAutoFeed", v, src.FiberAutoFeed);
	ENC_MEM(jd, "BadCutSurface", v, src.BadCutSurface);
	ENC_MEM(jd, "AutoReset", v, src.AutoReset);
	ENC_MEM(jd, "CleanDischargeTwice", v, src.CleanDischargeTwice);
	ENC_MEM(jd, "ManualDischargeTimes", v, src.ManualDischargeTimes);

	return v;
}
static inline void json2c(struct fs_base_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("FSPattern", src, dst.FSPattern);
	DEC_MEM("FiberType", src, dst.FiberType);
	DEC_MEM("FiberAlignment", src, dst.FiberAlignment);
	DEC_MEM("XImageFocus", src, dst.XImageFocus);
	DEC_MEM("YImageFocus", src, dst.YImageFocus);
	DEC_MEM("FiberShift", src, dst.FiberShift);
	DEC_MEM("DischargeStrengthAdjustment", src, dst.DischargeStrengthAdjustment);
	DEC_MEM("TensionSet", src, dst.TensionSet);
	DEC_MEM("CutAngleLimit", src, dst.CutAngleLimit);
	DEC_MEM("LossLimit", src, dst.LossLimit);
	DEC_MEM("FiberAngleLimit", src, dst.FiberAngleLimit);
	DEC_MEM("CleanDischargeStrength", src, dst.CleanDischargeStrength);
	DEC_MEM("CleanDischargeTime", src, dst.CleanDischargeTime);
	DEC_MEM("FiberIntervalSetup", src, dst.FiberIntervalSetup);
	DEC_MEM("FSPosSetup", src, dst.FSPosSetup);
	DEC_MEM("FiberPreFSStrength", src, dst.FiberPreFSStrength);
	DEC_MEM("FiberPreFSTime", src, dst.FiberPreFSTime);
	DEC_MEM("FiberOverlapSetup", src, dst.FiberOverlapSetup);
	DEC_MEM("Discharge1Strength", src, dst.Discharge1Strength);
	DEC_MEM("Discharge1Time", src, dst.Discharge1Time);
	DEC_MEM("Discharge2Strength", src, dst.Discharge2Strength);
	DEC_MEM("Discharge2LastTime", src, dst.Discharge2LastTime);
	DEC_MEM("Discharge2StartTime", src, dst.Discharge2StartTime);
	DEC_MEM("Discharge2StopTime", src, dst.Discharge2StopTime);
	DEC_MEM("ExtraManualDischargeTime", src, dst.ExtraManualDischargeTime);
	DEC_MEM("LeftFSSpeed", src, dst.LeftFSSpeed);
	DEC_MEM("RightFSSpeed", src, dst.RightFSSpeed);
	DEC_MEM("ConeFS", src, dst.ConeFS);
	DEC_MEM("ConeFSWaitTime", src, dst.ConeFSWaitTime);
	DEC_MEM("ConeFSSpeed", src, dst.ConeFSSpeed);
	DEC_MEM("ConeFSStretchLength", src, dst.ConeFSStretchLength);
	DEC_MEM("LossEstimationMode", src, dst.LossEstimationMode);
	DEC_MEM("LeftFiberMFD", src, dst.LeftFiberMFD);
	DEC_MEM("RightFiberMFD", src, dst.RightFiberMFD);
	DEC_MEM("LeastLoss", src, dst.LeastLoss);
	DEC_MEM("RateOfSyntropyBending", src, dst.RateOfSyntropyBending);
	DEC_MEM("RateOfReverseBending", src, dst.RateOfReverseBending);
	DEC_MEM("MFDMismatchCoefficient", src, dst.MFDMismatchCoefficient);
	DEC_MEM("AutoStart", src, dst.AutoStart);
	DEC_MEM("Stop1", src, dst.Stop1);
	DEC_MEM("Stop2", src, dst.Stop2);
	DEC_MEM("CutAngle", src, dst.CutAngle);
	DEC_MEM("OffsetData", src, dst.OffsetData);
	DEC_MEM("ArcCorrectedValue", src, dst.ArcCorrectedValue);
	DEC_MEM("Cut", src, dst.Cut);
	DEC_MEM("Loss", src, dst.Loss);
	DEC_MEM("FiberCoreAngle", src, dst.FiberCoreAngle);
	DEC_MEM("Bubble", src, dst.Bubble);
	DEC_MEM("Thick", src, dst.Thick);
	DEC_MEM("Thin", src, dst.Thin);
	DEC_MEM("AirPressure", src, dst.AirPressure);
	DEC_MEM("Temperature", src, dst.Temperature);
	DEC_MEM("RealTimeRevise", src, dst.RealTimeRevise);
	DEC_MEM("ImgGap", src, dst.ImgGap);
	DEC_MEM("ImgStop1", src, dst.ImgStop1);
	DEC_MEM("ImgAlign", src, dst.ImgAlign);
	DEC_MEM("ImgStop2", src, dst.ImgStop2);
	DEC_MEM("ImgDischarge", src, dst.ImgDischarge);
	DEC_MEM("ImgLossEstimation", src, dst.ImgLossEstimation);
	DEC_MEM("FiberAutoFeed", src, dst.FiberAutoFeed);
	DEC_MEM("BadCutSurface", src, dst.BadCutSurface);
	DEC_MEM("AutoReset", src, dst.AutoReset);
	DEC_MEM("CleanDischargeTwice", src, dst.CleanDischargeTwice);
	DEC_MEM("ManualDischargeTimes", src, dst.ManualDischargeTimes);
}

/// @struct fs_state
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_state & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "sstate", v, src.sstate);

	return v;
}
static inline void json2c(struct fs_state & dst, const rapidjson::Value & src)
{
	DEC_MEM("sstate", src, dst.sstate);
}

/// @struct heat_state
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct heat_state & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "sstate", v, src.sstate);

	return v;
}
static inline void json2c(struct heat_state & dst, const rapidjson::Value & src)
{
	DEC_MEM("sstate", src, dst.sstate);
}

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const struct simple_msg & /*src*/) {
	return rapidjson::Value(rapidjson::kObjectType);
}
static inline void json2c(struct simple_msg & /*dst*/, const rapidjson::Value & /*src*/) {}

/// @struct record_offset
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct record_offset & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "core_diff_pre", v, src.core_diff_pre);
	ENC_MEM(jd, "cladding_diff_pre", v, src.cladding_diff_pre);
	ENC_MEM(jd, "vertex_intersect_angle", v, src.vertex_intersect_angle);

	return v;
}
static inline void json2c(struct record_offset & dst, const rapidjson::Value & src)
{
	DEC_MEM("core_diff_pre", src, dst.core_diff_pre);
	DEC_MEM("cladding_diff_pre", src, dst.cladding_diff_pre);
	DEC_MEM("vertex_intersect_angle", src, dst.vertex_intersect_angle);
}

/// @struct tense_test_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct tense_test_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "is_tense_test", v, src.is_tense_test);
	ENC_MEM(jd, "is_success", v, src.is_success);

	return v;
}
static inline void json2c(struct tense_test_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("is_tense_test", src, dst.is_tense_test);
	DEC_MEM("is_success", src, dst.is_success);
}

/// @struct loss_estimating_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct loss_estimating_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "valid", v, src.valid);
	ENC_MEM(jd, "loss_data", v, src.loss_data);

	return v;
}
static inline void json2c(struct loss_estimating_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("valid", src, dst.valid);
	DEC_MEM("loss_data", src, dst.loss_data);
}

/// @struct defect_detect_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct defect_detect_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "data", v, src.data);

	return v;
}
static inline void json2c(struct defect_detect_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("data", src, dst.data);
}

/// @struct fiber_reco_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fiber_reco_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "lft_rec_info", v, src.lft_rec_info);
	ENC_MEM(jd, "rt_rec_info", v, src.rt_rec_info);

	return v;
}
static inline void json2c(struct fiber_reco_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("lft_rec_info", src, dst.lft_rec_info);
	DEC_MEM("rt_rec_info", src, dst.rt_rec_info);
}

/// @struct manual_discharge_counts
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct manual_discharge_counts & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "counts", v, src.counts);

	return v;
}
static inline void json2c(struct manual_discharge_counts & dst, const rapidjson::Value & src)
{
	DEC_MEM("counts", src, dst.counts);
}

/// @struct fusion_splice_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fusion_splice_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "lft_rec_info", v, src.lft_rec_info);
	ENC_MEM(jd, "rt_rec_info", v, src.rt_rec_info);
	ENC_MEM(jd, "defect_data", v, src.defect_data);
	ENC_MEM(jd, "z_record_off_set", v, src.z_record_off_set);
	ENC_MEM(jd, "pattern_compensate", v, src.pattern_compensate);
	ENC_MEM(jd, "loss_db", v, src.loss_db);
	ENC_MEM(jd, "z_tense_test_result", v, src.z_tense_test_result);
	ENC_MEM(jd, "z_manual_discharge_counts", v, src.z_manual_discharge_counts);
	ENC_MEM(jd, "fs_done_x_img", v, src.fs_done_x_img);
	ENC_MEM(jd, "fs_done_y_img", v, src.fs_done_y_img);

	return v;
}
static inline void json2c(struct fusion_splice_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("lft_rec_info", src, dst.lft_rec_info);
	DEC_MEM("rt_rec_info", src, dst.rt_rec_info);
	DEC_MEM("defect_data", src, dst.defect_data);
	DEC_MEM("z_record_off_set", src, dst.z_record_off_set);
	DEC_MEM("pattern_compensate", src, dst.pattern_compensate);
	DEC_MEM("loss_db", src, dst.loss_db);
	DEC_MEM("z_tense_test_result", src, dst.z_tense_test_result);
	DEC_MEM("z_manual_discharge_counts", src, dst.z_manual_discharge_counts);
	DEC_MEM("fs_done_x_img", src, dst.fs_done_x_img);
	DEC_MEM("fs_done_y_img", src, dst.fs_done_y_img);
}

/// @struct arc_revise
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct arc_revise & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "revise", v, src.revise);

	return v;
}
static inline void json2c(struct arc_revise & dst, const rapidjson::Value & src)
{
	DEC_MEM("revise", src, dst.revise);
}

/// @struct fs_da_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_da_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "arc1_mag", v, src.arc1_mag);
	ENC_MEM(jd, "arc2_mag", v, src.arc2_mag);

	return v;
}
static inline void json2c(struct fs_da_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("arc1_mag", src, dst.arc1_mag);
	DEC_MEM("arc2_mag", src, dst.arc2_mag);
}

/// @struct discharge_adjust_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct discharge_adjust_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "z_cfg", v, src.z_cfg);
	ENC_MEM(jd, "rec_info", v, src.rec_info);
	ENC_MEM(jd, "defect_data", v, src.defect_data);
	ENC_MEM(jd, "base", v, src.base);
	ENC_MEM(jd, "revise", v, src.revise);
	ENC_MEM(jd, "suggest1", v, src.suggest1);
	ENC_MEM(jd, "suggest2", v, src.suggest2);

	return v;
}
static inline void json2c(struct discharge_adjust_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("z_cfg", src, dst.z_cfg);
	DEC_MEM("rec_info", src, dst.rec_info);
	DEC_MEM("defect_data", src, dst.defect_data);
	DEC_MEM("base", src, dst.base);
	DEC_MEM("revise", src, dst.revise);
	DEC_MEM("suggest1", src, dst.suggest1);
	DEC_MEM("suggest2", src, dst.suggest2);
}

/// @struct discharge
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct discharge & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "magnitude", v, src.magnitude);
	ENC_MEM(jd, "time", v, src.time);

	return v;
}
static inline void json2c(struct discharge & dst, const rapidjson::Value & src)
{
	DEC_MEM("magnitude", src, dst.magnitude);
	DEC_MEM("time", src, dst.time);
}

/// @struct discharge_count
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct discharge_count & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "discharge_count", v, src.discharge_count);

	return v;
}
static inline void json2c(struct discharge_count & dst, const rapidjson::Value & src)
{
	DEC_MEM("discharge_count", src, dst.discharge_count);
}

/// @struct set_fs_display_mode_ext
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_fs_display_mode_ext & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "xovery", v, src.xovery);
	ENC_MEM(jd, "x_left", v, src.x_left);
	ENC_MEM(jd, "x_top", v, src.x_top);
	ENC_MEM(jd, "x_width", v, src.x_width);
	ENC_MEM(jd, "x_height", v, src.x_height);
	ENC_MEM(jd, "y_left", v, src.y_left);
	ENC_MEM(jd, "y_top", v, src.y_top);
	ENC_MEM(jd, "y_width", v, src.y_width);
	ENC_MEM(jd, "y_height", v, src.y_height);

	return v;
}
static inline void json2c(struct set_fs_display_mode_ext & dst, const rapidjson::Value & src)
{
	DEC_MEM("xovery", src, dst.xovery);
	DEC_MEM("x_left", src, dst.x_left);
	DEC_MEM("x_top", src, dst.x_top);
	DEC_MEM("x_width", src, dst.x_width);
	DEC_MEM("x_height", src, dst.x_height);
	DEC_MEM("y_left", src, dst.y_left);
	DEC_MEM("y_top", src, dst.y_top);
	DEC_MEM("y_width", src, dst.y_width);
	DEC_MEM("y_height", src, dst.y_height);
}

/// @struct set_fs_display_zoom_ext
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_fs_display_zoom_ext & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "x_left", v, src.x_left);
	ENC_MEM(jd, "x_up", v, src.x_up);
	ENC_MEM(jd, "x_width", v, src.x_width);
	ENC_MEM(jd, "x_height", v, src.x_height);
	ENC_MEM(jd, "y_left", v, src.y_left);
	ENC_MEM(jd, "y_up", v, src.y_up);
	ENC_MEM(jd, "y_width", v, src.y_width);
	ENC_MEM(jd, "y_height", v, src.y_height);

	return v;
}
static inline void json2c(struct set_fs_display_zoom_ext & dst, const rapidjson::Value & src)
{
	DEC_MEM("x_left", src, dst.x_left);
	DEC_MEM("x_up", src, dst.x_up);
	DEC_MEM("x_width", src, dst.x_width);
	DEC_MEM("x_height", src, dst.x_height);
	DEC_MEM("y_left", src, dst.y_left);
	DEC_MEM("y_up", src, dst.y_up);
	DEC_MEM("y_width", src, dst.y_width);
	DEC_MEM("y_height", src, dst.y_height);
}

/// @struct set_fs_display_mode
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_fs_display_mode & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "mode", v, src.mode);

	return v;
}
static inline void json2c(struct set_fs_display_mode & dst, const rapidjson::Value & src)
{
	DEC_MEM("mode", src, dst.mode);
}

/// @struct dust_check_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct dust_check_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "cmosid", v, src.cmosid);
	ENC_MEM(jd, "ori_img", v, src.ori_img);
	ENC_MEM(jd, "dust_img", v, src.dust_img);

	return v;
}
static inline void json2c(struct dust_check_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("cmosid", src, dst.cmosid);
	DEC_MEM("ori_img", src, dst.ori_img);
	DEC_MEM("dust_img", src, dst.dust_img);
}

/// @struct heat_start
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct heat_start & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "Material", v, src.Material);
	ENC_MEM(jd, "Fiberlen", v, src.Fiberlen);
	ENC_MEM(jd, "Heatctl", v, src.Heatctl);
	ENC_MEM(jd, "heat_time", v, src.heat_time);
	ENC_MEM(jd, "heat_temp", v, src.heat_temp);
	ENC_MEM(jd, "finish_temp", v, src.finish_temp);
	ENC_MEM(jd, "stable_temp", v, src.stable_temp);
	ENC_MEM(jd, "fast_heat", v, src.fast_heat);

	return v;
}
static inline void json2c(struct heat_start & dst, const rapidjson::Value & src)
{
	DEC_MEM("Material", src, dst.Material);
	DEC_MEM("Fiberlen", src, dst.Fiberlen);
	DEC_MEM("Heatctl", src, dst.Heatctl);
	DEC_MEM("heat_time", src, dst.heat_time);
	DEC_MEM("heat_temp", src, dst.heat_temp);
	DEC_MEM("finish_temp", src, dst.finish_temp);
	DEC_MEM("stable_temp", src, dst.stable_temp);
	DEC_MEM("fast_heat", src, dst.fast_heat);
}

/// @struct heat_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct heat_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);

	return v;
}
static inline void json2c(struct heat_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
}

/// @struct image_move
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct image_move & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "is_pos_x", v, src.is_pos_x);
	ENC_MEM(jd, "row", v, src.row);
	ENC_MEM(jd, "column", v, src.column);

	return v;
}
static inline void json2c(struct image_move & dst, const rapidjson::Value & src)
{
	DEC_MEM("is_pos_x", src, dst.is_pos_x);
	DEC_MEM("row", src, dst.row);
	DEC_MEM("column", src, dst.column);
}

/// @struct set_window
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_window & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "is_pos_x", v, src.is_pos_x);
	ENC_MEM(jd, "row", v, src.row);
	ENC_MEM(jd, "column", v, src.column);

	return v;
}
static inline void json2c(struct set_window & dst, const rapidjson::Value & src)
{
	DEC_MEM("is_pos_x", src, dst.is_pos_x);
	DEC_MEM("row", src, dst.row);
	DEC_MEM("column", src, dst.column);
}

/// @struct fs_cover_state
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_cover_state & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "is_openned", v, src.is_openned);

	return v;
}
static inline void json2c(struct fs_cover_state & dst, const rapidjson::Value & src)
{
	DEC_MEM("is_openned", src, dst.is_openned);
}

/// @struct set_lcd_brightness
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_lcd_brightness & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "brightness", v, src.brightness);

	return v;
}
static inline void json2c(struct set_lcd_brightness & dst, const rapidjson::Value & src)
{
	DEC_MEM("brightness", src, dst.brightness);
}

/// @struct set_led
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct set_led & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "id", v, src.id);
	ENC_MEM(jd, "brightness", v, src.brightness);

	return v;
}
static inline void json2c(struct set_led & dst, const rapidjson::Value & src)
{
	DEC_MEM("id", src, dst.id);
	DEC_MEM("brightness", src, dst.brightness);
}

/// @struct motor_start
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct motor_start & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "id", v, src.id);
	ENC_MEM(jd, "m_forward_dir", v, src.m_forward_dir);

	return v;
}
static inline void json2c(struct motor_start & dst, const rapidjson::Value & src)
{
	DEC_MEM("id", src, dst.id);
	DEC_MEM("m_forward_dir", src, dst.m_forward_dir);
}

/// @struct motor_stop
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct motor_stop & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "id", v, src.id);

	return v;
}
static inline void json2c(struct motor_stop & dst, const rapidjson::Value & src)
{
	DEC_MEM("id", src, dst.id);
}

/// @struct fs_mt_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_mt_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "MotorTestTimes", v, src.MotorTestTimes);
	ENC_MEM(jd, "ElectricArcTestTimes", v, src.ElectricArcTestTimes);
	ENC_MEM(jd, "CleanArcRate", v, src.CleanArcRate);

	return v;
}
static inline void json2c(struct fs_mt_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("MotorTestTimes", src, dst.MotorTestTimes);
	DEC_MEM("ElectricArcTestTimes", src, dst.ElectricArcTestTimes);
	DEC_MEM("CleanArcRate", src, dst.CleanArcRate);
}

/// @struct statistic_data_t
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct statistic_data_t & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "cnt", v, src.cnt);
	ENC_MEM(jd, "ref_v", v, src.ref_v);
	ENC_MEM(jd, "min_v", v, src.min_v);
	ENC_MEM(jd, "max_v", v, src.max_v);
	ENC_MEM(jd, "mid_v", v, src.mid_v);
	ENC_MEM(jd, "avg_v", v, src.avg_v);
	ENC_MEM(jd, "data", v, src.data);

	return v;
}
static inline void json2c(struct statistic_data_t & dst, const rapidjson::Value & src)
{
	DEC_MEM("cnt", src, dst.cnt);
	DEC_MEM("ref_v", src, dst.ref_v);
	DEC_MEM("min_v", src, dst.min_v);
	DEC_MEM("max_v", src, dst.max_v);
	DEC_MEM("mid_v", src, dst.mid_v);
	DEC_MEM("avg_v", src, dst.avg_v);
	DEC_MEM("data", src, dst.data);
}

/// @struct motor_test_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct motor_test_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "z_cfg", v, src.z_cfg);
	ENC_MEM(jd, "rec_info", v, src.rec_info);
	ENC_MEM(jd, "defect_data", v, src.defect_data);
	ENC_MEM(jd, "motor_tested_times", v, src.motor_tested_times);
	ENC_MEM(jd, "ele_arc_tested_times", v, src.ele_arc_tested_times);
	ENC_MEM(jd, "reset", v, src.reset);
	ENC_MEM(jd, "push", v, src.push);
	ENC_MEM(jd, "calibrate", v, src.calibrate);
	ENC_MEM(jd, "ele_arc", v, src.ele_arc);
	ENC_MEM(jd, "img", v, src.img);
	ENC_MEM(jd, "nm_per_pixel_xz", v, src.nm_per_pixel_xz);
	ENC_MEM(jd, "nm_per_pixel_yz", v, src.nm_per_pixel_yz);
	ENC_MEM(jd, "nm_per_step_lz", v, src.nm_per_step_lz);
	ENC_MEM(jd, "nm_per_step_rz", v, src.nm_per_step_rz);
	ENC_MEM(jd, "nm_push_lz", v, src.nm_push_lz);
	ENC_MEM(jd, "nm_push_rz", v, src.nm_push_rz);
	ENC_MEM(jd, "arc_mag", v, src.arc_mag);
	ENC_MEM(jd, "img_process", v, src.img_process);

	return v;
}
static inline void json2c(struct motor_test_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("z_cfg", src, dst.z_cfg);
	DEC_MEM("rec_info", src, dst.rec_info);
	DEC_MEM("defect_data", src, dst.defect_data);
	DEC_MEM("motor_tested_times", src, dst.motor_tested_times);
	DEC_MEM("ele_arc_tested_times", src, dst.ele_arc_tested_times);
	DEC_MEM("reset", src, dst.reset);
	DEC_MEM("push", src, dst.push);
	DEC_MEM("calibrate", src, dst.calibrate);
	DEC_MEM("ele_arc", src, dst.ele_arc);
	DEC_MEM("img", src, dst.img);
	DEC_MEM("nm_per_pixel_xz", src, dst.nm_per_pixel_xz);
	DEC_MEM("nm_per_pixel_yz", src, dst.nm_per_pixel_yz);
	DEC_MEM("nm_per_step_lz", src, dst.nm_per_step_lz);
	DEC_MEM("nm_per_step_rz", src, dst.nm_per_step_rz);
	DEC_MEM("nm_push_lz", src, dst.nm_push_lz);
	DEC_MEM("nm_push_rz", src, dst.nm_push_rz);
	DEC_MEM("arc_mag", src, dst.arc_mag);
	DEC_MEM("img_process", src, dst.img_process);
}

/// @struct process_progress
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct process_progress & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "progress", v, src.progress);

	return v;
}
static inline void json2c(struct process_progress & dst, const rapidjson::Value & src)
{
	DEC_MEM("progress", src, dst.progress);
}

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const struct fs_rr_cfg & /*src*/) {
	return rapidjson::Value(rapidjson::kObjectType);
}
static inline void json2c(struct fs_rr_cfg & /*dst*/, const rapidjson::Value & /*src*/) {}

/// @struct realtime_revise_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct realtime_revise_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "rec_info", v, src.rec_info);
	ENC_MEM(jd, "defect_data", v, src.defect_data);
	ENC_MEM(jd, "z_record_off_set", v, src.z_record_off_set);
	ENC_MEM(jd, "pattern_compensate", v, src.pattern_compensate);
	ENC_MEM(jd, "loss_db", v, src.loss_db);
	ENC_MEM(jd, "z_tense_test_result", v, src.z_tense_test_result);
	ENC_MEM(jd, "z_manual_discharge_counts", v, src.z_manual_discharge_counts);
	ENC_MEM(jd, "RealtimeReviseData", v, src.RealtimeReviseData);

	return v;
}
static inline void json2c(struct realtime_revise_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("rec_info", src, dst.rec_info);
	DEC_MEM("defect_data", src, dst.defect_data);
	DEC_MEM("z_record_off_set", src, dst.z_record_off_set);
	DEC_MEM("pattern_compensate", src, dst.pattern_compensate);
	DEC_MEM("loss_db", src, dst.loss_db);
	DEC_MEM("z_tense_test_result", src, dst.z_tense_test_result);
	DEC_MEM("z_manual_discharge_counts", src, dst.z_manual_discharge_counts);
	DEC_MEM("RealtimeReviseData", src, dst.RealtimeReviseData);
}

/// @struct regular_test_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct regular_test_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);

	return v;
}
static inline void json2c(struct regular_test_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
}

/// @struct fs_se_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_se_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "magnitude", v, src.magnitude);
	ENC_MEM(jd, "time", v, src.time);
	ENC_MEM(jd, "interval", v, src.interval);
	ENC_MEM(jd, "number", v, src.number);

	return v;
}
static inline void json2c(struct fs_se_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("magnitude", src, dst.magnitude);
	DEC_MEM("time", src, dst.time);
	DEC_MEM("interval", src, dst.interval);
	DEC_MEM("number", src, dst.number);
}

/// @struct stabilize_electrode_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct stabilize_electrode_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "z_cfg", v, src.z_cfg);
	ENC_MEM(jd, "number", v, src.number);

	return v;
}
static inline void json2c(struct stabilize_electrode_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("z_cfg", src, dst.z_cfg);
	DEC_MEM("number", src, dst.number);
}

/// @struct report_dev_state
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct report_dev_state & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "pressure", v, src.pressure);
	ENC_MEM(jd, "humidity", v, src.humidity);
	ENC_MEM(jd, "env_temp", v, src.env_temp);
	ENC_MEM(jd, "int_temp", v, src.int_temp);
	ENC_MEM(jd, "heat_temp", v, src.heat_temp);
	ENC_MEM(jd, "bat_voltage", v, src.bat_voltage);

	return v;
}
static inline void json2c(struct report_dev_state & dst, const rapidjson::Value & src)
{
	DEC_MEM("pressure", src, dst.pressure);
	DEC_MEM("humidity", src, dst.humidity);
	DEC_MEM("env_temp", src, dst.env_temp);
	DEC_MEM("int_temp", src, dst.int_temp);
	DEC_MEM("heat_temp", src, dst.heat_temp);
	DEC_MEM("bat_voltage", src, dst.bat_voltage);
}

/// @struct report_wave_form
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct report_wave_form & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "x_wave", v, src.x_wave);
	ENC_MEM(jd, "y_wave", v, src.y_wave);

	return v;
}
static inline void json2c(struct report_wave_form & dst, const rapidjson::Value & src)
{
	DEC_MEM("x_wave", src, dst.x_wave);
	DEC_MEM("y_wave", src, dst.y_wave);
}

/// @struct fs_ft_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_ft_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "lft_type", v, src.lft_type);
	ENC_MEM(jd, "rt_type", v, src.rt_type);

	return v;
}
static inline void json2c(struct fs_ft_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("lft_type", src, dst.lft_type);
	DEC_MEM("rt_type", src, dst.rt_type);
}

/// @struct fiber_train_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fiber_train_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "z_cfg", v, src.z_cfg);
	ENC_MEM(jd, "rec_info", v, src.rec_info);
	ENC_MEM(jd, "defect_data", v, src.defect_data);
	ENC_MEM(jd, "cnt", v, src.cnt);
	ENC_MEM(jd, "cnt_limit", v, src.cnt_limit);
	ENC_MEM(jd, "lft_attr", v, src.lft_attr);
	ENC_MEM(jd, "rt_attr", v, src.rt_attr);

	return v;
}
static inline void json2c(struct fiber_train_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("z_cfg", src, dst.z_cfg);
	DEC_MEM("rec_info", src, dst.rec_info);
	DEC_MEM("defect_data", src, dst.defect_data);
	DEC_MEM("cnt", src, dst.cnt);
	DEC_MEM("cnt_limit", src, dst.cnt_limit);
	DEC_MEM("lft_attr", src, dst.lft_attr);
	DEC_MEM("rt_attr", src, dst.rt_attr);
}

/// @struct count_down
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct count_down & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "cnt", v, src.cnt);

	return v;
}
static inline void json2c(struct count_down & dst, const rapidjson::Value & src)
{
	DEC_MEM("cnt", src, dst.cnt);
}

/// @struct motor_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct motor_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "clock", v, src.clock);
	ENC_MEM(jd, "raw_max", v, src.raw_max);
	ENC_MEM(jd, "raw_min", v, src.raw_min);
	ENC_MEM(jd, "backlash", v, src.backlash);
	ENC_MEM(jd, "stroke", v, src.stroke);
	ENC_MEM(jd, "sfactor", v, src.sfactor);
	ENC_MEM(jd, "spow", v, src.spow);
	ENC_MEM(jd, "lps", v, src.lps);

	return v;
}
static inline void json2c(struct motor_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("clock", src, dst.clock);
	DEC_MEM("raw_max", src, dst.raw_max);
	DEC_MEM("raw_min", src, dst.raw_min);
	DEC_MEM("backlash", src, dst.backlash);
	DEC_MEM("stroke", src, dst.stroke);
	DEC_MEM("sfactor", src, dst.sfactor);
	DEC_MEM("spow", src, dst.spow);
	DEC_MEM("lps", src, dst.lps);
}

/// @struct cmos_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct cmos_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "model", v, src.model);
	ENC_MEM(jd, "full_width", v, src.full_width);
	ENC_MEM(jd, "full_height", v, src.full_height);
	ENC_MEM(jd, "win_width", v, src.win_width);
	ENC_MEM(jd, "win_height", v, src.win_height);
	ENC_MEM(jd, "win_left", v, src.win_left);
	ENC_MEM(jd, "win_top", v, src.win_top);
	ENC_MEM(jd, "min_exposure", v, src.min_exposure);
	ENC_MEM(jd, "max_exposure", v, src.max_exposure);
	ENC_MEM(jd, "pixel_width", v, src.pixel_width);
	ENC_MEM(jd, "pixel_height", v, src.pixel_height);

	return v;
}
static inline void json2c(struct cmos_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("model", src, dst.model);
	DEC_MEM("full_width", src, dst.full_width);
	DEC_MEM("full_height", src, dst.full_height);
	DEC_MEM("win_width", src, dst.win_width);
	DEC_MEM("win_height", src, dst.win_height);
	DEC_MEM("win_left", src, dst.win_left);
	DEC_MEM("win_top", src, dst.win_top);
	DEC_MEM("min_exposure", src, dst.min_exposure);
	DEC_MEM("max_exposure", src, dst.max_exposure);
	DEC_MEM("pixel_width", src, dst.pixel_width);
	DEC_MEM("pixel_height", src, dst.pixel_height);
}

/// @struct hvb_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct hvb_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "max_volt", v, src.max_volt);
	ENC_MEM(jd, "pressure_c0", v, src.pressure_c0);
	ENC_MEM(jd, "pressure_c1", v, src.pressure_c1);

	return v;
}
static inline void json2c(struct hvb_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("max_volt", src, dst.max_volt);
	DEC_MEM("pressure_c0", src, dst.pressure_c0);
	DEC_MEM("pressure_c1", src, dst.pressure_c1);
}

/// @struct ia_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct ia_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "bg_lum", v, src.bg_lum);
	ENC_MEM(jd, "cap_delay", v, src.cap_delay);
	ENC_MEM(jd, "cover_delay", v, src.cover_delay);
	ENC_MEM(jd, "led_lum", v, src.led_lum);
	ENC_MEM(jd, "dc_th0", v, src.dc_th0);
	ENC_MEM(jd, "dc_th1", v, src.dc_th1);
	ENC_MEM(jd, "denoise_th", v, src.denoise_th);
	ENC_MEM(jd, "loss_est_factor", v, src.loss_est_factor);
	ENC_MEM(jd, "vdist_th0", v, src.vdist_th0);
	ENC_MEM(jd, "vdist_th1", v, src.vdist_th1);
	ENC_MEM(jd, "hdist_th0", v, src.hdist_th0);
	ENC_MEM(jd, "hdist_th1", v, src.hdist_th1);
	ENC_MEM(jd, "hangle_th", v, src.hangle_th);
	ENC_MEM(jd, "vangle_th", v, src.vangle_th);

	return v;
}
static inline void json2c(struct ia_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("bg_lum", src, dst.bg_lum);
	DEC_MEM("cap_delay", src, dst.cap_delay);
	DEC_MEM("cover_delay", src, dst.cover_delay);
	DEC_MEM("led_lum", src, dst.led_lum);
	DEC_MEM("dc_th0", src, dst.dc_th0);
	DEC_MEM("dc_th1", src, dst.dc_th1);
	DEC_MEM("denoise_th", src, dst.denoise_th);
	DEC_MEM("loss_est_factor", src, dst.loss_est_factor);
	DEC_MEM("vdist_th0", src, dst.vdist_th0);
	DEC_MEM("vdist_th1", src, dst.vdist_th1);
	DEC_MEM("hdist_th0", src, dst.hdist_th0);
	DEC_MEM("hdist_th1", src, dst.hdist_th1);
	DEC_MEM("hangle_th", src, dst.hangle_th);
	DEC_MEM("vangle_th", src, dst.vangle_th);
}

/// @struct mc_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct mc_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "reset_speed", v, src.reset_speed);
	ENC_MEM(jd, "enter_speed", v, src.enter_speed);
	ENC_MEM(jd, "push1_speed", v, src.push1_speed);
	ENC_MEM(jd, "push2_speed", v, src.push2_speed);
	ENC_MEM(jd, "calib_speed", v, src.calib_speed);
	ENC_MEM(jd, "manual_speed", v, src.manual_speed);

	return v;
}
static inline void json2c(struct mc_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("reset_speed", src, dst.reset_speed);
	DEC_MEM("enter_speed", src, dst.enter_speed);
	DEC_MEM("push1_speed", src, dst.push1_speed);
	DEC_MEM("push2_speed", src, dst.push2_speed);
	DEC_MEM("calib_speed", src, dst.calib_speed);
	DEC_MEM("manual_speed", src, dst.manual_speed);
}

/// @struct ar_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct ar_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "base", v, src.base);
	ENC_MEM(jd, "revise", v, src.revise);

	return v;
}
static inline void json2c(struct ar_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("base", src, dst.base);
	DEC_MEM("revise", src, dst.revise);
}

/// @struct rr_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct rr_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "sm", v, src.sm);
	ENC_MEM(jd, "mm", v, src.mm);
	ENC_MEM(jd, "nz", v, src.nz);
	ENC_MEM(jd, "ds", v, src.ds);

	return v;
}
static inline void json2c(struct rr_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("sm", src, dst.sm);
	DEC_MEM("mm", src, dst.mm);
	DEC_MEM("nz", src, dst.nz);
	DEC_MEM("ds", src, dst.ds);
}

/// @struct fr_spec
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fr_spec & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "left", v, src.left);
	ENC_MEM(jd, "right", v, src.right);

	return v;
}
static inline void json2c(struct fr_spec & dst, const rapidjson::Value & src)
{
	DEC_MEM("left", src, dst.left);
	DEC_MEM("right", src, dst.right);
}

/// @struct update_window_position
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct update_window_position & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "is_pos_x", v, src.is_pos_x);
	ENC_MEM(jd, "row", v, src.row);
	ENC_MEM(jd, "column", v, src.column);

	return v;
}
static inline void json2c(struct update_window_position & dst, const rapidjson::Value & src)
{
	DEC_MEM("is_pos_x", src, dst.is_pos_x);
	DEC_MEM("row", src, dst.row);
	DEC_MEM("column", src, dst.column);
}

/// @struct update_led_brightness
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct update_led_brightness & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "id", v, src.id);
	ENC_MEM(jd, "brightness", v, src.brightness);

	return v;
}
static inline void json2c(struct update_led_brightness & dst, const rapidjson::Value & src)
{
	DEC_MEM("id", src, dst.id);
	DEC_MEM("brightness", src, dst.brightness);
}

/// mid to string
static rapidjson::Value::StringRefType const s_mid_to_str[] = {
	"nil",
	"arc_revise",
	"countDown",
	"defect_detect_result",
	"discharge",
	"dischargeAdjustInit",
	"dischargeAdjustResult",
	"dischargeAdjustStart",
	"discharge_count",
	"dustCheckFullResult",
	"dustCheckFullStart",
	"dustCheckResult",
	"dustCheckStart",
	"fiberTrainInit",
	"fiberTrainResult",
	"fiberTrainStart",
	"fiber_reco_result",
	"fsCoverOpenned",
	"fs_cover_state",
	"fs_state",
	"fusionSpliceReset",
	"fusionSpliceResult",
	"fusionSpliceStart",
	"getFiberDefectInfo",
	"goOn",
	"heat_result",
	"heat_start",
	"heat_state",
	"image_move",
	"loss_estimating_result",
	"manual_discharge_counts",
	"motorTestResult",
	"motorTestStart",
	"motor_start",
	"motor_stop",
	"process_progress",
	"queryDevState",
	"queryWaveForm",
	"realtimeReviseResult",
	"realtimeReviseStart",
	"realtime_revise_result",
	"record_offset",
	"regularTestResult",
	"regularTestStart",
	"report_dev_state",
	"report_wave_form",
	"setFsDisplayModeExt",
	"set_fs_display_mode",
	"set_fs_display_mode_ext",
	"set_fs_display_zoom_ext",
	"set_lcd_brightness",
	"set_led",
	"set_window",
	"skip",
	"stabilizeElectrodeResult",
	"stabilizeElectrodeStart",
	"stop",
	"stopDischarge",
	"tenseTestResult",
	"update_led_brightness",
	"update_window_position",
};

static rapidjson::Value::StringRefType const s_id = "msgid";
static rapidjson::Value::StringRefType const s_data = "data";

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
	doc.AddMember(s_id, s_mid_to_str[(uint16_t)mid], doc.GetAllocator());
}

/// @sender serialize
void sender::__serialize()
{
	out_wrapper buf((char *)m_pcur);
	rapidjson::Writer<out_wrapper> writer(buf);
	((rapidjson::Document *)m_doc)->Accept(writer);
	m_pcur = (uint8_t *)buf.pcur;
}


void sender::convert(std::string & dst, const struct cmos_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct hvb_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct ia_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct mc_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct ar_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct rr_spec & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

/// messages packer wrapper
void sender::__pack(const struct fs_state & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct heat_state & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct simple_msg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct record_offset & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct tense_test_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct loss_estimating_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct defect_detect_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fiber_reco_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct manual_discharge_counts & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fusion_splice_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct arc_revise & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_da_cfg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct discharge_adjust_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct discharge & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct discharge_count & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_fs_display_mode_ext & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_fs_display_zoom_ext & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_fs_display_mode & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct dust_check_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct heat_start & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct heat_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct image_move & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_window & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_cover_state & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_lcd_brightness & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct set_led & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct motor_start & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct motor_stop & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_mt_cfg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct motor_test_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct process_progress & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_rr_cfg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct realtime_revise_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct regular_test_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_se_cfg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct stabilize_electrode_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct report_dev_state & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct report_wave_form & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fs_ft_cfg & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct fiber_train_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct count_down & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct update_window_position & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct update_led_brightness & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}


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

void rcver::convert(struct cmos_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct hvb_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct ia_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct mc_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct ar_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct rr_spec & dst, char *buf)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.ParseInsitu(buf);
	json2c(dst, doc);
	__reset();
}

/// mid to string
const char * rcver::__mid_to_str(mid_t mid)
{
	return s_mid_to_str[(uint16_t)mid];
}

/// messages unpack
void rcver::__unpack(struct fs_state & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct heat_state & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct simple_msg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct record_offset & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct tense_test_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct loss_estimating_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct defect_detect_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fiber_reco_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct manual_discharge_counts & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fusion_splice_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct arc_revise & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_da_cfg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct discharge_adjust_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct discharge & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct discharge_count & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_fs_display_mode_ext & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_fs_display_zoom_ext & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_fs_display_mode & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct dust_check_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct heat_start & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct heat_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct image_move & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_window & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_cover_state & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_lcd_brightness & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct set_led & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct motor_start & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct motor_stop & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_mt_cfg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct motor_test_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct process_progress & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_rr_cfg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct realtime_revise_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct regular_test_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_se_cfg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct stabilize_electrode_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct report_dev_state & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct report_wave_form & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fs_ft_cfg & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct fiber_train_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct count_down & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct update_window_position & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct update_led_brightness & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

