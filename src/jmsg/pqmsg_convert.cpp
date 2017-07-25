
#include <stdexcept>
#include <cstring>

#include "jmsg/jmsg_types.hpp"
#include "jmsg/pqxxutils.hpp"

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const std::vector<uint8_t> & src)
{
	const pqxx::binarystring bs(src.data(), src.size());
	return this->operator()(bs);
}

static inline void pqxx2c(bool & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(int16_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(uint16_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(int32_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(uint32_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(int64_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(uint64_t & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(float & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(double & dst, const pqxx::const_row_iterator & it) { it->to(dst); }
static inline void pqxx2c(std::string & dst, const pqxx::const_row_iterator & it) { it->to(dst); }

static inline void pqxx2c(std::vector<uint8_t> & dst, const pqxx::field & f)
{
	dst.clear();
	pqxx::binarystring bs(f);
	dst.insert(dst.end(), &(bs.data()[0]), &(bs.data()[bs.size()]));
}

template<typename T, typename V>
const char * search_name_directly(const T &flist, const V val)
{
	if (0 <= val && val < std::extent<T>::value) {
		return flist[val].name;
	}

	throw std::invalid_argument("enum val error!");
}

template<typename T, typename V>
const char * search_name_binary(const T &flist, const V val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head <= tail) {
		std::size_t mid = (head + tail) / 2;
		V cur = flist[mid].val;
		if (cur == val) {
			return flist[mid].name;
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
auto search_val_binary(const T & flist, const char * name) -> decltype(flist[0].val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head <= tail) {
		std::size_t mid = (head + tail) / 2;
		int cmp_result = std::strcmp(flist[mid].name, name);
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
auto search_val_binary(const T & flist, const std::string & name) -> decltype(flist[0].val)
{
	search_val_binary(flist, name.c_str());
}

/// @fs_pattern_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_fs_pattern_t[4] = {
	{ 0, "AUTO" },
	{ 1, "CALIBRATE" },
	{ 2, "NORMAL" },
	{ 3, "SPECIAL" },
};

static inline const char * e2pqxx(const enum fs_pattern_t src)
{ return search_name_directly(e2str_fs_pattern_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum fs_pattern_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum fs_pattern_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, it->c_str()); }

/// @fiber_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_fiber_t[6] = {
	{ 0, "AUTO" },
	{ 1, "SM" },
	{ 2, "DS" },
	{ 3, "NZ" },
	{ 4, "MM" },
	{ 5, "FOLLOW" },
};

static inline const char * e2pqxx(const enum fiber_t src)
{ return search_name_directly(e2str_fiber_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum fiber_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum fiber_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum fiber_t)search_val_binary(str2e_fiber_t, it->c_str()); }

/// @align_method_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_align_method_t[5] = {
	{ 0, "AUTO" },
	{ 1, "CLAD" },
	{ 2, "CORE" },
	{ 3, "FINE" },
	{ 4, "MANUAL" },
};

static inline const char * e2pqxx(const enum align_method_t src)
{ return search_name_directly(e2str_align_method_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum align_method_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum align_method_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum align_method_t)search_val_binary(str2e_align_method_t, it->c_str()); }

/// @loss_estimate_mode_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_loss_estimate_mode_t[4] = {
	{ 0, "OFF" },
	{ 1, "FINE" },
	{ 2, "CORE" },
	{ 3, "CLAD" },
};

static inline const char * e2pqxx(const enum loss_estimate_mode_t src)
{ return search_name_directly(e2str_loss_estimate_mode_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum loss_estimate_mode_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum loss_estimate_mode_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, it->c_str()); }

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const fs_param_cfg & src)
{
	return (*this)(src.seqn)(src.name)(src.fusion_mode)(src.lfti)(src.rfti)(src.align_mode)(src.x_focus)(src.y_focus)(src.ecf_redress)(src.fiber_shift)(src.auto_mag)(src.vangle_limit)(src.hangle_limit)(src.clr_mag)(src.clr_time)(src.clr_pos)(src.position)(src.gap)(src.overlap)(src.pre_mag)(src.pre_time)(src.arc1_mag)(src.arc1_time)(src.arc2_mag)(src.arc2_time)(src.arc2_on_time)(src.arc2_off_time)(src.arc_man_time)(src.lft_push_speed)(src.rt_push_speed)(src.taper_splice)(src.taper_wait_time)(src.taper_length)(src.taper_speed)(src.tense_test)(src.tense_speed)(src.tense_length)(src.loss_mode)(src.loss_limit)(src.loss_min)(src.lft_mfd)(src.rt_mfd)(src.syn_bend_co)(src.opp_bend_co)(src.mfd_mis_co);
}

pqxx::const_row_iterator pqxx2c(fs_param_cfg & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.seqn, it); ++it;
	pqxx2c(dst.name, it); ++it;
	pqxx2c(dst.fusion_mode, it); ++it;
	pqxx2c(dst.lfti, it); ++it;
	pqxx2c(dst.rfti, it); ++it;
	pqxx2c(dst.align_mode, it); ++it;
	pqxx2c(dst.x_focus, it); ++it;
	pqxx2c(dst.y_focus, it); ++it;
	pqxx2c(dst.ecf_redress, it); ++it;
	pqxx2c(dst.fiber_shift, it); ++it;
	pqxx2c(dst.auto_mag, it); ++it;
	pqxx2c(dst.vangle_limit, it); ++it;
	pqxx2c(dst.hangle_limit, it); ++it;
	pqxx2c(dst.clr_mag, it); ++it;
	pqxx2c(dst.clr_time, it); ++it;
	pqxx2c(dst.clr_pos, it); ++it;
	pqxx2c(dst.position, it); ++it;
	pqxx2c(dst.gap, it); ++it;
	pqxx2c(dst.overlap, it); ++it;
	pqxx2c(dst.pre_mag, it); ++it;
	pqxx2c(dst.pre_time, it); ++it;
	pqxx2c(dst.arc1_mag, it); ++it;
	pqxx2c(dst.arc1_time, it); ++it;
	pqxx2c(dst.arc2_mag, it); ++it;
	pqxx2c(dst.arc2_time, it); ++it;
	pqxx2c(dst.arc2_on_time, it); ++it;
	pqxx2c(dst.arc2_off_time, it); ++it;
	pqxx2c(dst.arc_man_time, it); ++it;
	pqxx2c(dst.lft_push_speed, it); ++it;
	pqxx2c(dst.rt_push_speed, it); ++it;
	pqxx2c(dst.taper_splice, it); ++it;
	pqxx2c(dst.taper_wait_time, it); ++it;
	pqxx2c(dst.taper_length, it); ++it;
	pqxx2c(dst.taper_speed, it); ++it;
	pqxx2c(dst.tense_test, it); ++it;
	pqxx2c(dst.tense_speed, it); ++it;
	pqxx2c(dst.tense_length, it); ++it;
	pqxx2c(dst.loss_mode, it); ++it;
	pqxx2c(dst.loss_limit, it); ++it;
	pqxx2c(dst.loss_min, it); ++it;
	pqxx2c(dst.lft_mfd, it); ++it;
	pqxx2c(dst.rt_mfd, it); ++it;
	pqxx2c(dst.syn_bend_co, it); ++it;
	pqxx2c(dst.opp_bend_co, it); ++it;
	pqxx2c(dst.mfd_mis_co, it); ++it;
	return it;
}

/// @heat_material_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_heat_material_t[10] = {
	{ "CONNECTOR", 4 },
	{ "Connector", 4 },
	{ "MICRO250", 1 },
	{ "MICRO400", 2 },
	{ "MICRO900", 3 },
	{ "Micro250", 1 },
	{ "Micro400", 2 },
	{ "Micro900", 3 },
	{ "STANDARD", 0 },
	{ "Standard", 0 },
};

/// @heat_material_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_heat_material_t[5] = {
	{ 0, "STANDARD" },
	{ 1, "MICRO250" },
	{ 2, "MICRO400" },
	{ 3, "MICRO900" },
	{ 4, "CONNECTOR" },
};

static inline const char * e2pqxx(const enum heat_material_t src)
{ return search_name_directly(e2str_heat_material_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum heat_material_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum heat_material_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum heat_material_t)search_val_binary(str2e_heat_material_t, it->c_str()); }

/// @shrinktube_length_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_shrinktube_length_t[6] = {
	{ "20mm", 0 },
	{ "40mm", 1 },
	{ "60mm", 2 },
	{ "L20MM", 0 },
	{ "L40MM", 1 },
	{ "L60MM", 2 },
};

/// @shrinktube_length_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_shrinktube_length_t[3] = {
	{ 0, "L20MM" },
	{ 1, "L40MM" },
	{ 2, "L60MM" },
};

static inline const char * e2pqxx(const enum shrinktube_length_t src)
{ return search_name_directly(e2str_shrinktube_length_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum shrinktube_length_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum shrinktube_length_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum shrinktube_length_t)search_val_binary(str2e_shrinktube_length_t, it->c_str()); }

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const heat_param_cfg & src)
{
	return (*this)(src.seqn)(src.name)(src.material)(src.length)(src.auto_heat)(src.heat_time)(src.heat_temp)(src.finish_temp)(src.fast_heat)(src.hold_temp);
}

pqxx::const_row_iterator pqxx2c(heat_param_cfg & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.seqn, it); ++it;
	pqxx2c(dst.name, it); ++it;
	pqxx2c(dst.material, it); ++it;
	pqxx2c(dst.length, it); ++it;
	pqxx2c(dst.auto_heat, it); ++it;
	pqxx2c(dst.heat_time, it); ++it;
	pqxx2c(dst.heat_temp, it); ++it;
	pqxx2c(dst.finish_temp, it); ++it;
	pqxx2c(dst.fast_heat, it); ++it;
	pqxx2c(dst.hold_temp, it); ++it;
	return it;
}

/// @fs_err_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
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

static inline const char * e2pqxx(const enum fs_err_t src)
{ return search_name_directly(e2str_fs_err_t, (unsigned)src); }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum fs_err_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum fs_err_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum fs_err_t)search_val_binary(str2e_fs_err_t, it->c_str()); }

/// @ifd_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_ifd_t[5] = {
	{ 0x00000001, "ifd_end_crude" },
	{ 0x00000002, "ifd_horizontal_angle" },
	{ 0x00000004, "ifd_vertical_angle" },
	{ 0x40000000, "ifd_cant_identify" },
	{ 0x7FFFFFFF, "ifd_all" },
};

static inline uint32_t e2pqxx(const enum ifd_t src)
{ return (ifd_t)src; }

template<>
inline pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const enum ifd_t & src)
{ return this->operator()(e2pqxx(src)); }
static inline void pqxx2c(enum ifd_t & dst, const pqxx::const_row_iterator & it)
{ uint32_t v; it->to(v); dst = (enum ifd_t)v; }

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const fusion_splice_result & src)
{
	return (*this)(src.time_consume)(src.code)(src.loss)(src.recinfo.lft.ft)(src.recinfo.lft.clad_dm)(src.recinfo.lft.core_dm)(src.recinfo.rt.ft)(src.recinfo.rt.clad_dm)(src.recinfo.rt.core_dm)(src.defect.yzl.dbmp)(src.defect.yzl.hangle)(src.defect.yzl.vangle)(src.defect.yzl.clad_dm)(src.defect.yzl.core_dm)(src.defect.yzr.dbmp)(src.defect.yzr.hangle)(src.defect.yzr.vangle)(src.defect.yzr.clad_dm)(src.defect.yzr.core_dm)(src.defect.xzl.dbmp)(src.defect.xzl.hangle)(src.defect.xzl.vangle)(src.defect.xzl.clad_dm)(src.defect.xzl.core_dm)(src.defect.xzr.dbmp)(src.defect.xzr.hangle)(src.defect.xzr.vangle)(src.defect.xzr.clad_dm)(src.defect.xzr.core_dm)(src.defect.yz_hangle)(src.defect.xz_hangle)(src.defect.lft_vangle)(src.defect.rt_vangle)(src.defect.yz_img)(src.defect.xz_img)(src.defect.yz_defect_img)(src.defect.xz_defect_img)(src.prestate.core_offset)(src.prestate.clad_offset)(src.prestate.endface_gap)(src.prestate.vertex_angle)(src.tense_test.exed)(src.tense_test.pass)(src.manual_arc.count)(src.xz_final_img)(src.yz_final_img);
}

pqxx::const_row_iterator pqxx2c(fusion_splice_result & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.time_consume, it); ++it;
	pqxx2c(dst.code, it); ++it;
	pqxx2c(dst.loss, it); ++it;
	pqxx2c(dst.recinfo.lft.ft, it); ++it;
	pqxx2c(dst.recinfo.lft.clad_dm, it); ++it;
	pqxx2c(dst.recinfo.lft.core_dm, it); ++it;
	pqxx2c(dst.recinfo.rt.ft, it); ++it;
	pqxx2c(dst.recinfo.rt.clad_dm, it); ++it;
	pqxx2c(dst.recinfo.rt.core_dm, it); ++it;
	pqxx2c(dst.defect.yzl.dbmp, it); ++it;
	pqxx2c(dst.defect.yzl.hangle, it); ++it;
	pqxx2c(dst.defect.yzl.vangle, it); ++it;
	pqxx2c(dst.defect.yzl.clad_dm, it); ++it;
	pqxx2c(dst.defect.yzl.core_dm, it); ++it;
	pqxx2c(dst.defect.yzr.dbmp, it); ++it;
	pqxx2c(dst.defect.yzr.hangle, it); ++it;
	pqxx2c(dst.defect.yzr.vangle, it); ++it;
	pqxx2c(dst.defect.yzr.clad_dm, it); ++it;
	pqxx2c(dst.defect.yzr.core_dm, it); ++it;
	pqxx2c(dst.defect.xzl.dbmp, it); ++it;
	pqxx2c(dst.defect.xzl.hangle, it); ++it;
	pqxx2c(dst.defect.xzl.vangle, it); ++it;
	pqxx2c(dst.defect.xzl.clad_dm, it); ++it;
	pqxx2c(dst.defect.xzl.core_dm, it); ++it;
	pqxx2c(dst.defect.xzr.dbmp, it); ++it;
	pqxx2c(dst.defect.xzr.hangle, it); ++it;
	pqxx2c(dst.defect.xzr.vangle, it); ++it;
	pqxx2c(dst.defect.xzr.clad_dm, it); ++it;
	pqxx2c(dst.defect.xzr.core_dm, it); ++it;
	pqxx2c(dst.defect.yz_hangle, it); ++it;
	pqxx2c(dst.defect.xz_hangle, it); ++it;
	pqxx2c(dst.defect.lft_vangle, it); ++it;
	pqxx2c(dst.defect.rt_vangle, it); ++it;
	pqxx2c(dst.defect.yz_img, it); ++it;
	pqxx2c(dst.defect.xz_img, it); ++it;
	pqxx2c(dst.defect.yz_defect_img, it); ++it;
	pqxx2c(dst.defect.xz_defect_img, it); ++it;
	pqxx2c(dst.prestate.core_offset, it); ++it;
	pqxx2c(dst.prestate.clad_offset, it); ++it;
	pqxx2c(dst.prestate.endface_gap, it); ++it;
	pqxx2c(dst.prestate.vertex_angle, it); ++it;
	pqxx2c(dst.tense_test.exed, it); ++it;
	pqxx2c(dst.tense_test.pass, it); ++it;
	pqxx2c(dst.manual_arc.count, it); ++it;
	pqxx2c(dst.xz_final_img, it); ++it;
	pqxx2c(dst.yz_final_img, it); ++it;
	return it;
}

