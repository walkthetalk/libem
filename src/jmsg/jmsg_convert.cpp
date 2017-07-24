
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

	while (head <= tail) {
		std::size_t mid = (head + tail) / 2;
		V cur = flist[mid].val;
		if (cur == val) {
			rapidjson::Value::StringRefType str(flist[mid].name);
			return  rapidjson::Value(str);
		}
		else if (cur < val) {
			head = mid + 1;
		}
		else {
			tail = mid - 1;
		}
	};

	throw std::invalid_argument("enum val error!");
}

template<typename T>
auto search_val_binary(const T & flist, const rapidjson::Value & name) -> decltype(flist[0].val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head <= tail) {
		std::size_t mid = (head + tail) / 2;
		const rapidjson::Value::StringRefType & cur = flist[mid].name;
		int cmp_result = std::strcmp(cur.s, name.GetString());
		if (cmp_result == 0) {
			return flist[mid].val;
		}
		else if (cmp_result < 0) {
			head = mid + 1;
		}
		else {
			tail = mid - 1;
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
	{ "ifd_all", 0x7FFFFFFF },
	{ "ifd_cant_identify", 0x40000000 },
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
	{ 0x40000000, "ifd_cant_identify" },
	{ 0x7FFFFFFF, "ifd_all" },
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
	ENC_MEM(jd, "hangle", v, src.hangle);
	ENC_MEM(jd, "vangle", v, src.vangle);
	ENC_MEM(jd, "clad_dm", v, src.clad_dm);

	return v;
}
static inline void json2c(struct ifd_line & dst, const rapidjson::Value & src)
{
	DEC_MEM("dbmp", src, dst.dbmp);
	DEC_MEM("hangle", src, dst.hangle);
	DEC_MEM("vangle", src, dst.vangle);
	DEC_MEM("clad_dm", src, dst.clad_dm);
}

/// @struct defect_detect_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct defect_detect_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "yzl", v, src.yzl);
	ENC_MEM(jd, "yzr", v, src.yzr);
	ENC_MEM(jd, "xzl", v, src.xzl);
	ENC_MEM(jd, "xzr", v, src.xzr);
	ENC_MEM(jd, "yz_hangle", v, src.yz_hangle);
	ENC_MEM(jd, "xz_hangle", v, src.xz_hangle);
	ENC_MEM(jd, "lft_vangle", v, src.lft_vangle);
	ENC_MEM(jd, "rt_vangle", v, src.rt_vangle);
	ENC_MEM(jd, "yz_img", v, src.yz_img);
	ENC_MEM(jd, "xz_img", v, src.xz_img);
	ENC_MEM(jd, "yz_defect_img", v, src.yz_defect_img);
	ENC_MEM(jd, "xz_defect_img", v, src.xz_defect_img);

	return v;
}
static inline void json2c(struct defect_detect_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("yzl", src, dst.yzl);
	DEC_MEM("yzr", src, dst.yzr);
	DEC_MEM("xzl", src, dst.xzl);
	DEC_MEM("xzr", src, dst.xzr);
	DEC_MEM("yz_hangle", src, dst.yz_hangle);
	DEC_MEM("xz_hangle", src, dst.xz_hangle);
	DEC_MEM("lft_vangle", src, dst.lft_vangle);
	DEC_MEM("rt_vangle", src, dst.rt_vangle);
	DEC_MEM("yz_img", src, dst.yz_img);
	DEC_MEM("xz_img", src, dst.xz_img);
	DEC_MEM("yz_defect_img", src, dst.yz_defect_img);
	DEC_MEM("xz_defect_img", src, dst.xz_defect_img);
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
	unsigned val;
} str2e_motorId_t[4] = {
	{ "LZ", 0 },
	{ "RZ", 1 },
	{ "X", 2 },
	{ "Y", 3 },
};

/// @motorId_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_motorId_t[4] = {
	{ 0, "LZ" },
	{ 1, "RZ" },
	{ 2, "X" },
	{ 3, "Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum motorId_t src)
{ return search_name_directly(e2str_motorId_t, (unsigned)src); }

static inline void json2c(enum motorId_t & dst, const rapidjson::Value & src)
{ dst = (enum motorId_t)search_val_binary(str2e_motorId_t, src); }

/// @fs_display_mode_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_fs_display_mode_t[5] = {
	{ "LR", 3 },
	{ "NO", 4 },
	{ "TB", 2 },
	{ "X", 0 },
	{ "Y", 1 },
};

/// @fs_display_mode_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_fs_display_mode_t[5] = {
	{ 0, "X" },
	{ 1, "Y" },
	{ 2, "TB" },
	{ 3, "LR" },
	{ 4, "NO" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_display_mode_t src)
{ return search_name_directly(e2str_fs_display_mode_t, (unsigned)src); }

static inline void json2c(enum fs_display_mode_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_display_mode_t)search_val_binary(str2e_fs_display_mode_t, src); }

/// @fs_err_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_fs_err_t[29] = {
	{ "ABNORMAL_ARC", 10 },
	{ "ARC1_REVISE", 18 },
	{ "ARC2_REVISE", 19 },
	{ "ARC_OVERFLOW", 28 },
	{ "ARC_POS", 27 },
	{ "ARC_TIME_ZERO", 17 },
	{ "CALIBRATE_TIMEOUT", 15 },
	{ "CMOS_EXP", 25 },
	{ "COVER_OPENNED", 3 },
	{ "CROSS_OVER", 7 },
	{ "FIBER_BROKEN", 12 },
	{ "FIBER_DEFECT", 5 },
	{ "FIBER_OFFSIDE", 24 },
	{ "FIBER_RECO", 6 },
	{ "FOCUS_X", 20 },
	{ "FOCUS_Y", 21 },
	{ "IMG_LUM", 9 },
	{ "IMG_PROCESS", 22 },
	{ "LOSS_EST", 26 },
	{ "MEDDIACY", 1 },
	{ "NO_FIBER", 4 },
	{ "OFF_CENTER", 8 },
	{ "PUSH_TIMEOUT", 14 },
	{ "QUIT_MIDWAY", 13 },
	{ "RESET_TIMEOUT", 16 },
	{ "SUCCESS", 0 },
	{ "SYSTEM_ERR", 23 },
	{ "TENSE_TEST_FAIL", 11 },
	{ "UNKOWN_ERR", 2 },
};

/// @fs_err_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_fs_err_t[29] = {
	{ 0, "SUCCESS" },
	{ 1, "MEDDIACY" },
	{ 2, "UNKOWN_ERR" },
	{ 3, "COVER_OPENNED" },
	{ 4, "NO_FIBER" },
	{ 5, "FIBER_DEFECT" },
	{ 6, "FIBER_RECO" },
	{ 7, "CROSS_OVER" },
	{ 8, "OFF_CENTER" },
	{ 9, "IMG_LUM" },
	{ 10, "ABNORMAL_ARC" },
	{ 11, "TENSE_TEST_FAIL" },
	{ 12, "FIBER_BROKEN" },
	{ 13, "QUIT_MIDWAY" },
	{ 14, "PUSH_TIMEOUT" },
	{ 15, "CALIBRATE_TIMEOUT" },
	{ 16, "RESET_TIMEOUT" },
	{ 17, "ARC_TIME_ZERO" },
	{ 18, "ARC1_REVISE" },
	{ 19, "ARC2_REVISE" },
	{ 20, "FOCUS_X" },
	{ 21, "FOCUS_Y" },
	{ 22, "IMG_PROCESS" },
	{ 23, "SYSTEM_ERR" },
	{ 24, "FIBER_OFFSIDE" },
	{ 25, "CMOS_EXP" },
	{ 26, "LOSS_EST" },
	{ 27, "ARC_POS" },
	{ 28, "ARC_OVERFLOW" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_err_t src)
{ return search_name_directly(e2str_fs_err_t, (unsigned)src); }

static inline void json2c(enum fs_err_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_err_t)search_val_binary(str2e_fs_err_t, src); }

/// @ledId_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_ledId_t[2] = {
	{ "CMOS_X", 0 },
	{ "CMOS_Y", 1 },
};

/// @ledId_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_ledId_t[2] = {
	{ 0, "CMOS_X" },
	{ 1, "CMOS_Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum ledId_t src)
{ return search_name_directly(e2str_ledId_t, (unsigned)src); }

static inline void json2c(enum ledId_t & dst, const rapidjson::Value & src)
{ dst = (enum ledId_t)search_val_binary(str2e_ledId_t, src); }

/// @cmosId_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_cmosId_t[2] = {
	{ "X", 0 },
	{ "Y", 1 },
};

/// @cmosId_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_cmosId_t[2] = {
	{ 0, "X" },
	{ 1, "Y" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum cmosId_t src)
{ return search_name_directly(e2str_cmosId_t, (unsigned)src); }

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
	unsigned val;
} str2e_fiber_t[6] = {
	{ "AUTO", 0 },
	{ "DS", 2 },
	{ "FOLLOW", 5 },
	{ "MM", 4 },
	{ "NZ", 3 },
	{ "SM", 1 },
};

/// @fiber_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_fiber_t[6] = {
	{ 0, "AUTO" },
	{ 1, "SM" },
	{ 2, "DS" },
	{ 3, "NZ" },
	{ 4, "MM" },
	{ 5, "FOLLOW" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fiber_t src)
{ return search_name_directly(e2str_fiber_t, (unsigned)src); }

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
	ENC_MEM(jd, "clad_dm", v, src.clad_dm);
	ENC_MEM(jd, "core_dm", v, src.core_dm);

	return v;
}
static inline void json2c(struct fiber_rec_info & dst, const rapidjson::Value & src)
{
	DEC_MEM("ft", src, dst.ft);
	DEC_MEM("clad_dm", src, dst.clad_dm);
	DEC_MEM("core_dm", src, dst.core_dm);
}

/// @fs_pattern_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_fs_pattern_t[4] = {
	{ "AUTO", 0 },
	{ "CALIBRATE", 1 },
	{ "NORMAL", 2 },
	{ "SPECIAL", 3 },
};

/// @fs_pattern_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_fs_pattern_t[4] = {
	{ 0, "AUTO" },
	{ 1, "CALIBRATE" },
	{ 2, "NORMAL" },
	{ 3, "SPECIAL" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum fs_pattern_t src)
{ return search_name_directly(e2str_fs_pattern_t, (unsigned)src); }

static inline void json2c(enum fs_pattern_t & dst, const rapidjson::Value & src)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, src); }

/// @loss_estimate_mode_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_loss_estimate_mode_t[4] = {
	{ "CLAD", 3 },
	{ "CORE", 2 },
	{ "FINE", 1 },
	{ "OFF", 0 },
};

/// @loss_estimate_mode_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_loss_estimate_mode_t[4] = {
	{ 0, "OFF" },
	{ 1, "FINE" },
	{ 2, "CORE" },
	{ 3, "CLAD" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum loss_estimate_mode_t src)
{ return search_name_directly(e2str_loss_estimate_mode_t, (unsigned)src); }

static inline void json2c(enum loss_estimate_mode_t & dst, const rapidjson::Value & src)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, src); }

/// @align_method_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_align_method_t[5] = {
	{ "AUTO", 0 },
	{ "CLAD", 1 },
	{ "CORE", 2 },
	{ "FINE", 3 },
	{ "MANUAL", 4 },
};

/// @align_method_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_align_method_t[5] = {
	{ 0, "AUTO" },
	{ 1, "CLAD" },
	{ 2, "CORE" },
	{ 3, "FINE" },
	{ 4, "MANUAL" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum align_method_t src)
{ return search_name_directly(e2str_align_method_t, (unsigned)src); }

static inline void json2c(enum align_method_t & dst, const rapidjson::Value & src)
{ dst = (enum align_method_t)search_val_binary(str2e_align_method_t, src); }

/// @struct fs_param_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_param_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "seqn", v, src.seqn);
	ENC_MEM(jd, "name", v, src.name);
	ENC_MEM(jd, "fusion_mode", v, src.fusion_mode);
	ENC_MEM(jd, "lfti", v, src.lfti);
	ENC_MEM(jd, "rfti", v, src.rfti);
	ENC_MEM(jd, "align_mode", v, src.align_mode);
	ENC_MEM(jd, "x_focus", v, src.x_focus);
	ENC_MEM(jd, "y_focus", v, src.y_focus);
	ENC_MEM(jd, "ecf_redress", v, src.ecf_redress);
	ENC_MEM(jd, "auto_mag", v, src.auto_mag);
	ENC_MEM(jd, "vangle_limit", v, src.vangle_limit);
	ENC_MEM(jd, "hangle_limit", v, src.hangle_limit);
	ENC_MEM(jd, "clr_mag", v, src.clr_mag);
	ENC_MEM(jd, "clr_time", v, src.clr_time);
	ENC_MEM(jd, "clr_pos", v, src.clr_pos);
	ENC_MEM(jd, "position", v, src.position);
	ENC_MEM(jd, "gap", v, src.gap);
	ENC_MEM(jd, "overlap", v, src.overlap);
	ENC_MEM(jd, "pre_mag", v, src.pre_mag);
	ENC_MEM(jd, "pre_time", v, src.pre_time);
	ENC_MEM(jd, "arc1_mag", v, src.arc1_mag);
	ENC_MEM(jd, "arc1_time", v, src.arc1_time);
	ENC_MEM(jd, "arc2_mag", v, src.arc2_mag);
	ENC_MEM(jd, "arc2_time", v, src.arc2_time);
	ENC_MEM(jd, "arc2_on_time", v, src.arc2_on_time);
	ENC_MEM(jd, "arc2_off_time", v, src.arc2_off_time);
	ENC_MEM(jd, "arc_man_time", v, src.arc_man_time);
	ENC_MEM(jd, "lft_push_speed", v, src.lft_push_speed);
	ENC_MEM(jd, "rt_push_speed", v, src.rt_push_speed);
	ENC_MEM(jd, "taper_splice", v, src.taper_splice);
	ENC_MEM(jd, "taper_wait_time", v, src.taper_wait_time);
	ENC_MEM(jd, "taper_length", v, src.taper_length);
	ENC_MEM(jd, "taper_speed", v, src.taper_speed);
	ENC_MEM(jd, "tense_test", v, src.tense_test);
	ENC_MEM(jd, "tense_speed", v, src.tense_speed);
	ENC_MEM(jd, "tense_length", v, src.tense_length);
	ENC_MEM(jd, "loss_mode", v, src.loss_mode);
	ENC_MEM(jd, "loss_limit", v, src.loss_limit);
	ENC_MEM(jd, "loss_min", v, src.loss_min);
	ENC_MEM(jd, "lft_mfd", v, src.lft_mfd);
	ENC_MEM(jd, "rt_mfd", v, src.rt_mfd);
	ENC_MEM(jd, "syn_bend_co", v, src.syn_bend_co);
	ENC_MEM(jd, "opp_bend_co", v, src.opp_bend_co);
	ENC_MEM(jd, "mfd_mis_co", v, src.mfd_mis_co);

	return v;
}
static inline void json2c(struct fs_param_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("seqn", src, dst.seqn);
	DEC_MEM("name", src, dst.name);
	DEC_MEM("fusion_mode", src, dst.fusion_mode);
	DEC_MEM("lfti", src, dst.lfti);
	DEC_MEM("rfti", src, dst.rfti);
	DEC_MEM("align_mode", src, dst.align_mode);
	DEC_MEM("x_focus", src, dst.x_focus);
	DEC_MEM("y_focus", src, dst.y_focus);
	DEC_MEM("ecf_redress", src, dst.ecf_redress);
	DEC_MEM("auto_mag", src, dst.auto_mag);
	DEC_MEM("vangle_limit", src, dst.vangle_limit);
	DEC_MEM("hangle_limit", src, dst.hangle_limit);
	DEC_MEM("clr_mag", src, dst.clr_mag);
	DEC_MEM("clr_time", src, dst.clr_time);
	DEC_MEM("clr_pos", src, dst.clr_pos);
	DEC_MEM("position", src, dst.position);
	DEC_MEM("gap", src, dst.gap);
	DEC_MEM("overlap", src, dst.overlap);
	DEC_MEM("pre_mag", src, dst.pre_mag);
	DEC_MEM("pre_time", src, dst.pre_time);
	DEC_MEM("arc1_mag", src, dst.arc1_mag);
	DEC_MEM("arc1_time", src, dst.arc1_time);
	DEC_MEM("arc2_mag", src, dst.arc2_mag);
	DEC_MEM("arc2_time", src, dst.arc2_time);
	DEC_MEM("arc2_on_time", src, dst.arc2_on_time);
	DEC_MEM("arc2_off_time", src, dst.arc2_off_time);
	DEC_MEM("arc_man_time", src, dst.arc_man_time);
	DEC_MEM("lft_push_speed", src, dst.lft_push_speed);
	DEC_MEM("rt_push_speed", src, dst.rt_push_speed);
	DEC_MEM("taper_splice", src, dst.taper_splice);
	DEC_MEM("taper_wait_time", src, dst.taper_wait_time);
	DEC_MEM("taper_length", src, dst.taper_length);
	DEC_MEM("taper_speed", src, dst.taper_speed);
	DEC_MEM("tense_test", src, dst.tense_test);
	DEC_MEM("tense_speed", src, dst.tense_speed);
	DEC_MEM("tense_length", src, dst.tense_length);
	DEC_MEM("loss_mode", src, dst.loss_mode);
	DEC_MEM("loss_limit", src, dst.loss_limit);
	DEC_MEM("loss_min", src, dst.loss_min);
	DEC_MEM("lft_mfd", src, dst.lft_mfd);
	DEC_MEM("rt_mfd", src, dst.rt_mfd);
	DEC_MEM("syn_bend_co", src, dst.syn_bend_co);
	DEC_MEM("opp_bend_co", src, dst.opp_bend_co);
	DEC_MEM("mfd_mis_co", src, dst.mfd_mis_co);
}

/// @heat_material_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_heat_material_t[5] = {
	{ "CONNECTOR", 4 },
	{ "MICRO250", 1 },
	{ "MICRO400", 2 },
	{ "MICRO900", 3 },
	{ "STANDARD", 0 },
};

/// @heat_material_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_heat_material_t[5] = {
	{ 0, "STANDARD" },
	{ 1, "MICRO250" },
	{ 2, "MICRO400" },
	{ 3, "MICRO900" },
	{ 4, "CONNECTOR" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum heat_material_t src)
{ return search_name_directly(e2str_heat_material_t, (unsigned)src); }

static inline void json2c(enum heat_material_t & dst, const rapidjson::Value & src)
{ dst = (enum heat_material_t)search_val_binary(str2e_heat_material_t, src); }

/// @shrinktube_length_t : string to enum
static const struct {
	rapidjson::Value::StringRefType name;
	unsigned val;
} str2e_shrinktube_length_t[3] = {
	{ "L20MM", 0 },
	{ "L40MM", 1 },
	{ "L60MM", 2 },
};

/// @shrinktube_length_t : enum to string
static const struct {
	unsigned val;
	rapidjson::Value::StringRefType name;
} e2str_shrinktube_length_t[3] = {
	{ 0, "L20MM" },
	{ 1, "L40MM" },
	{ 2, "L60MM" },
};

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const enum shrinktube_length_t src)
{ return search_name_directly(e2str_shrinktube_length_t, (unsigned)src); }

static inline void json2c(enum shrinktube_length_t & dst, const rapidjson::Value & src)
{ dst = (enum shrinktube_length_t)search_val_binary(str2e_shrinktube_length_t, src); }

/// @struct heat_param_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct heat_param_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "seqn", v, src.seqn);
	ENC_MEM(jd, "name", v, src.name);
	ENC_MEM(jd, "material", v, src.material);
	ENC_MEM(jd, "length", v, src.length);
	ENC_MEM(jd, "auto_heat", v, src.auto_heat);
	ENC_MEM(jd, "heat_time", v, src.heat_time);
	ENC_MEM(jd, "heat_temp", v, src.heat_temp);
	ENC_MEM(jd, "finish_temp", v, src.finish_temp);
	ENC_MEM(jd, "fast_heat", v, src.fast_heat);
	ENC_MEM(jd, "temp_stay", v, src.temp_stay);

	return v;
}
static inline void json2c(struct heat_param_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("seqn", src, dst.seqn);
	DEC_MEM("name", src, dst.name);
	DEC_MEM("material", src, dst.material);
	DEC_MEM("length", src, dst.length);
	DEC_MEM("auto_heat", src, dst.auto_heat);
	DEC_MEM("heat_time", src, dst.heat_time);
	DEC_MEM("heat_temp", src, dst.heat_temp);
	DEC_MEM("finish_temp", src, dst.finish_temp);
	DEC_MEM("fast_heat", src, dst.fast_heat);
	DEC_MEM("temp_stay", src, dst.temp_stay);
}

/// @struct misc_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct misc_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "fsParamIdx", v, src.fsParamIdx);
	ENC_MEM(jd, "heatParamIdx", v, src.heatParamIdx);

	return v;
}
static inline void json2c(struct misc_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("fsParamIdx", src, dst.fsParamIdx);
	DEC_MEM("heatParamIdx", src, dst.heatParamIdx);
}

/// @decltype(fs_option_cfg::operationOptions)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::operationOptions) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "autoStart", v, src.autoStart);
	ENC_MEM(jd, "pause1", v, src.pause1);
	ENC_MEM(jd, "pause2", v, src.pause2);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::operationOptions) & dst, const rapidjson::Value & src)
{
	DEC_MEM("autoStart", src, dst.autoStart);
	DEC_MEM("pause1", src, dst.pause1);
	DEC_MEM("pause2", src, dst.pause2);
}

/// @decltype(fs_option_cfg::dataDisplay)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::dataDisplay) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "cleaveAngle", v, src.cleaveAngle);
	ENC_MEM(jd, "axisOffset", v, src.axisOffset);
	ENC_MEM(jd, "arcCorrectedValue", v, src.arcCorrectedValue);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::dataDisplay) & dst, const rapidjson::Value & src)
{
	DEC_MEM("cleaveAngle", src, dst.cleaveAngle);
	DEC_MEM("axisOffset", src, dst.axisOffset);
	DEC_MEM("arcCorrectedValue", src, dst.arcCorrectedValue);
}

/// @decltype(fs_option_cfg::ignoreOptions)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::ignoreOptions) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "cleave", v, src.cleave);
	ENC_MEM(jd, "loss", v, src.loss);
	ENC_MEM(jd, "fiberCoreAngle", v, src.fiberCoreAngle);
	ENC_MEM(jd, "bubble", v, src.bubble);
	ENC_MEM(jd, "thick", v, src.thick);
	ENC_MEM(jd, "thin", v, src.thin);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::ignoreOptions) & dst, const rapidjson::Value & src)
{
	DEC_MEM("cleave", src, dst.cleave);
	DEC_MEM("loss", src, dst.loss);
	DEC_MEM("fiberCoreAngle", src, dst.fiberCoreAngle);
	DEC_MEM("bubble", src, dst.bubble);
	DEC_MEM("thick", src, dst.thick);
	DEC_MEM("thin", src, dst.thin);
}

/// @decltype(fs_option_cfg::arcCompensation)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::arcCompensation) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "pressure", v, src.pressure);
	ENC_MEM(jd, "temperature", v, src.temperature);
	ENC_MEM(jd, "RealTimeRevise", v, src.RealTimeRevise);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::arcCompensation) & dst, const rapidjson::Value & src)
{
	DEC_MEM("pressure", src, dst.pressure);
	DEC_MEM("temperature", src, dst.temperature);
	DEC_MEM("RealTimeRevise", src, dst.RealTimeRevise);
}

/// @decltype(fs_option_cfg::fiberImageDisplay)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::fiberImageDisplay) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "gapSet", v, src.gapSet);
	ENC_MEM(jd, "pause1", v, src.pause1);
	ENC_MEM(jd, "alignOption", v, src.alignOption);
	ENC_MEM(jd, "pause2", v, src.pause2);
	ENC_MEM(jd, "arc", v, src.arc);
	ENC_MEM(jd, "estimateLoss", v, src.estimateLoss);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::fiberImageDisplay) & dst, const rapidjson::Value & src)
{
	DEC_MEM("gapSet", src, dst.gapSet);
	DEC_MEM("pause1", src, dst.pause1);
	DEC_MEM("alignOption", src, dst.alignOption);
	DEC_MEM("pause2", src, dst.pause2);
	DEC_MEM("arc", src, dst.arc);
	DEC_MEM("estimateLoss", src, dst.estimateLoss);
}

/// @decltype(fs_option_cfg::others)
static inline rapidjson::Value c2json(rapidjson::Document & jd, const decltype(fs_option_cfg::others) & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "autoFeedFiber", v, src.autoFeedFiber);
	ENC_MEM(jd, "badCleavedEndface", v, src.badCleavedEndface);
	ENC_MEM(jd, "resetAfterSplice", v, src.resetAfterSplice);
	ENC_MEM(jd, "cleanAgain", v, src.cleanAgain);
	ENC_MEM(jd, "imageZoomIn", v, src.imageZoomIn);
	ENC_MEM(jd, "manualArcLimit", v, src.manualArcLimit);

	return v;
}
static inline void json2c(decltype(fs_option_cfg::others) & dst, const rapidjson::Value & src)
{
	DEC_MEM("autoFeedFiber", src, dst.autoFeedFiber);
	DEC_MEM("badCleavedEndface", src, dst.badCleavedEndface);
	DEC_MEM("resetAfterSplice", src, dst.resetAfterSplice);
	DEC_MEM("cleanAgain", src, dst.cleanAgain);
	DEC_MEM("imageZoomIn", src, dst.imageZoomIn);
	DEC_MEM("manualArcLimit", src, dst.manualArcLimit);
}

/// @struct fs_option_cfg
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fs_option_cfg & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "operationOptions", v, src.operationOptions);
	ENC_MEM(jd, "dataDisplay", v, src.dataDisplay);
	ENC_MEM(jd, "ignoreOptions", v, src.ignoreOptions);
	ENC_MEM(jd, "arcCompensation", v, src.arcCompensation);
	ENC_MEM(jd, "fiberImageDisplay", v, src.fiberImageDisplay);
	ENC_MEM(jd, "others", v, src.others);

	return v;
}
static inline void json2c(struct fs_option_cfg & dst, const rapidjson::Value & src)
{
	DEC_MEM("operationOptions", src, dst.operationOptions);
	DEC_MEM("dataDisplay", src, dst.dataDisplay);
	DEC_MEM("ignoreOptions", src, dst.ignoreOptions);
	DEC_MEM("arcCompensation", src, dst.arcCompensation);
	DEC_MEM("fiberImageDisplay", src, dst.fiberImageDisplay);
	DEC_MEM("others", src, dst.others);
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

/// @struct fspre_state
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fspre_state & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "core_offset", v, src.core_offset);
	ENC_MEM(jd, "clad_offset", v, src.clad_offset);
	ENC_MEM(jd, "endface_gap", v, src.endface_gap);
	ENC_MEM(jd, "vertex_angle", v, src.vertex_angle);

	return v;
}
static inline void json2c(struct fspre_state & dst, const rapidjson::Value & src)
{
	DEC_MEM("core_offset", src, dst.core_offset);
	DEC_MEM("clad_offset", src, dst.clad_offset);
	DEC_MEM("endface_gap", src, dst.endface_gap);
	DEC_MEM("vertex_angle", src, dst.vertex_angle);
}

/// @struct tense_test_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct tense_test_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "exed", v, src.exed);
	ENC_MEM(jd, "pass", v, src.pass);

	return v;
}
static inline void json2c(struct tense_test_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("exed", src, dst.exed);
	DEC_MEM("pass", src, dst.pass);
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

/// @struct fiber_reco_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fiber_reco_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "lft", v, src.lft);
	ENC_MEM(jd, "rt", v, src.rt);

	return v;
}
static inline void json2c(struct fiber_reco_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("lft", src, dst.lft);
	DEC_MEM("rt", src, dst.rt);
}

/// @struct manual_arc_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct manual_arc_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "count", v, src.count);

	return v;
}
static inline void json2c(struct manual_arc_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("count", src, dst.count);
}

/// @struct fusion_splice_result
static inline rapidjson::Value c2json(rapidjson::Document & jd, const struct fusion_splice_result & src)
{
	rapidjson::Value v(rapidjson::kObjectType);
	ENC_MEM(jd, "time_consume", v, src.time_consume);
	ENC_MEM(jd, "code", v, src.code);
	ENC_MEM(jd, "loss", v, src.loss);
	ENC_MEM(jd, "recinfo", v, src.recinfo);
	ENC_MEM(jd, "defect", v, src.defect);
	ENC_MEM(jd, "prestate", v, src.prestate);
	ENC_MEM(jd, "tense_test", v, src.tense_test);
	ENC_MEM(jd, "manual_arc", v, src.manual_arc);
	ENC_MEM(jd, "xz_final_img", v, src.xz_final_img);
	ENC_MEM(jd, "yz_final_img", v, src.yz_final_img);

	return v;
}
static inline void json2c(struct fusion_splice_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("time_consume", src, dst.time_consume);
	DEC_MEM("code", src, dst.code);
	DEC_MEM("loss", src, dst.loss);
	DEC_MEM("recinfo", src, dst.recinfo);
	DEC_MEM("defect", src, dst.defect);
	DEC_MEM("prestate", src, dst.prestate);
	DEC_MEM("tense_test", src, dst.tense_test);
	DEC_MEM("manual_arc", src, dst.manual_arc);
	DEC_MEM("xz_final_img", src, dst.xz_final_img);
	DEC_MEM("yz_final_img", src, dst.yz_final_img);
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
	ENC_MEM(jd, "recinfo", v, src.recinfo);
	ENC_MEM(jd, "defect", v, src.defect);
	ENC_MEM(jd, "prestate", v, src.prestate);
	ENC_MEM(jd, "tense_test", v, src.tense_test);
	ENC_MEM(jd, "base", v, src.base);
	ENC_MEM(jd, "revise", v, src.revise);
	ENC_MEM(jd, "suggest1", v, src.suggest1);
	ENC_MEM(jd, "suggest2", v, src.suggest2);

	return v;
}
static inline void json2c(struct discharge_adjust_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("recinfo", src, dst.recinfo);
	DEC_MEM("defect", src, dst.defect);
	DEC_MEM("prestate", src, dst.prestate);
	DEC_MEM("tense_test", src, dst.tense_test);
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

static inline rapidjson::Value c2json(rapidjson::Document & /*jd*/, const struct heat_start & /*src*/) {
	return rapidjson::Value(rapidjson::kObjectType);
}
static inline void json2c(struct heat_start & /*dst*/, const rapidjson::Value & /*src*/) {}

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
	ENC_MEM(jd, "recinfo", v, src.recinfo);
	ENC_MEM(jd, "defect", v, src.defect);
	ENC_MEM(jd, "base_count", v, src.base_count);
	ENC_MEM(jd, "arc_count", v, src.arc_count);
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
	DEC_MEM("recinfo", src, dst.recinfo);
	DEC_MEM("defect", src, dst.defect);
	DEC_MEM("base_count", src, dst.base_count);
	DEC_MEM("arc_count", src, dst.arc_count);
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
	ENC_MEM(jd, "recinfo", v, src.recinfo);
	ENC_MEM(jd, "defect", v, src.defect);
	ENC_MEM(jd, "prestate", v, src.prestate);
	ENC_MEM(jd, "loss", v, src.loss);
	ENC_MEM(jd, "RealtimeReviseData", v, src.RealtimeReviseData);

	return v;
}
static inline void json2c(struct realtime_revise_result & dst, const rapidjson::Value & src)
{
	DEC_MEM("code", src, dst.code);
	DEC_MEM("recinfo", src, dst.recinfo);
	DEC_MEM("defect", src, dst.defect);
	DEC_MEM("prestate", src, dst.prestate);
	DEC_MEM("loss", src, dst.loss);
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
	ENC_MEM(jd, "recinfo", v, src.recinfo);
	ENC_MEM(jd, "defect", v, src.defect);
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
	DEC_MEM("recinfo", src, dst.recinfo);
	DEC_MEM("defect", src, dst.defect);
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
	"fspre_state",
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
	"manual_arc_result",
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


void sender::convert(std::string & dst, const struct fs_param_cfg & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct heat_param_cfg & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct misc_cfg & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
}

void sender::convert(std::string & dst, const struct fs_option_cfg & src)
{
	rapidjson::Document & doc = *(rapidjson::Document*)m_doc;
	out_string_wrapper buf(dst);
	rapidjson::Writer<out_string_wrapper> writer(buf);
	c2json(doc, src).Accept(writer);
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
void sender::__pack(const struct defect_detect_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

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

void sender::__pack(const struct fspre_state & val)
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

void sender::__pack(const struct fiber_reco_result & val)
{
	rapidjson::Document & doc = *(rapidjson::Document *)m_doc;
	doc.AddMember(s_data, c2json(doc, val), doc.GetAllocator());
}

void sender::__pack(const struct manual_arc_result & val)
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

void rcver::convert(struct fs_param_cfg & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct heat_param_cfg & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct misc_cfg & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct fs_option_cfg & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct cmos_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct hvb_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct ia_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct mc_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct ar_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

void rcver::convert(struct rr_spec & dst, const char * src)
{
	rapidjson::Document & doc = *((rapidjson::Document*)m_doc);
	doc.Parse(src);
	json2c(dst, doc);
	__reset();
}

/// mid to string
const char * rcver::__mid_to_str(mid_t mid)
{
	return s_mid_to_str[(uint16_t)mid];
}

/// messages unpack
void rcver::__unpack(struct defect_detect_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

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

void rcver::__unpack(struct fspre_state & dst)
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

void rcver::__unpack(struct fiber_reco_result & dst)
{
	json2c(dst, ((rapidjson::Document*)m_doc)->FindMember(s_data)->value);
}

void rcver::__unpack(struct manual_arc_result & dst)
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

