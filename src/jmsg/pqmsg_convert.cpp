
#include <stdexcept>
#include <cstring>

#include "jmsg/jmsg_types.hpp"
#include "jmsg/pqxxutils.hpp"

template<>
void pqxx::internal::params::add_field(const std::vector<uint8_t> & src)
{
	const pqxx::binarystring bs(src.data(), src.size());
        add_field(bs);
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
inline void pqxx::internal::params::add_field(const enum fs_pattern_t & src)
{ add_field(e2pqxx(src)); }
static inline void pqxx2c(enum fs_pattern_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, it->c_str()); }

/// @fiber_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_fiber_t[6] = {
	{ "AUTO", 5 },
	{ "DS", 1 },
	{ "FOLLOW", 4 },
	{ "MM", 3 },
	{ "NZ", 2 },
	{ "SM", 0 },
};

/// @fiber_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_fiber_t[6] = {
	{ 0, "SM" },
	{ 1, "DS" },
	{ 2, "NZ" },
	{ 3, "MM" },
	{ 4, "FOLLOW" },
	{ 5, "AUTO" },
};

static inline const char * e2pqxx(const enum fiber_t src)
{ return search_name_directly(e2str_fiber_t, (unsigned)src); }

template<>
inline void pqxx::internal::params::add_field(const enum fiber_t & src)
{ add_field(e2pqxx(src)); }
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
inline void pqxx::internal::params::add_field(const enum align_method_t & src)
{ add_field(e2pqxx(src)); }
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
inline void pqxx::internal::params::add_field(const enum loss_estimate_mode_t & src)
{ add_field(e2pqxx(src)); }
static inline void pqxx2c(enum loss_estimate_mode_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, it->c_str()); }

template<>
void pqxx::internal::params::add_field(const fs_param_cfg & src)
{
	add_field(src.seqn);
	add_field(src.name);
	add_field(src.ver);
	add_field(src.fusion_mode);
	add_field(src.lfti);
	add_field(src.rfti);
	add_field(src.align_mode);
	add_field(src.x_focus);
	add_field(src.y_focus);
	add_field(src.ecf_redress);
	add_field(src.auto_mag);
	add_field(src.vangle_limit);
	add_field(src.hangle_limit);
	add_field(src.push1_speed);
	add_field(src.clr_mag);
	add_field(src.clr_time);
	add_field(src.clr_pos);
	add_field(src.position);
	add_field(src.gap);
	add_field(src.overlap);
	add_field(src.pre_mag);
	add_field(src.pre_time);
	add_field(src.arc1_mag);
	add_field(src.arc1_time);
	add_field(src.arc2_mag);
	add_field(src.arc2_time);
	add_field(src.arc2_on_time);
	add_field(src.arc2_off_time);
	add_field(src.arc_man_time);
	add_field(src.lft_push_speed);
	add_field(src.rt_push_speed);
	add_field(src.taper_splice);
	add_field(src.taper_wait_time);
	add_field(src.taper_length);
	add_field(src.taper_speed);
	add_field(src.tense_test);
	add_field(src.tense_speed);
	add_field(src.tense_length);
	add_field(src.loss_mode);
	add_field(src.loss_limit);
	add_field(src.loss_min);
	add_field(src.lft_mfd);
	add_field(src.rt_mfd);
	add_field(src.syn_bend_co);
	add_field(src.opp_bend_co);
	add_field(src.mfd_mis_co);
}

pqxx::const_row_iterator pqxx2c(fs_param_cfg & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.seqn, it); ++it;
	pqxx2c(dst.name, it); ++it;
	pqxx2c(dst.ver, it); ++it;
	pqxx2c(dst.fusion_mode, it); ++it;
	pqxx2c(dst.lfti, it); ++it;
	pqxx2c(dst.rfti, it); ++it;
	pqxx2c(dst.align_mode, it); ++it;
	pqxx2c(dst.x_focus, it); ++it;
	pqxx2c(dst.y_focus, it); ++it;
	pqxx2c(dst.ecf_redress, it); ++it;
	pqxx2c(dst.auto_mag, it); ++it;
	pqxx2c(dst.vangle_limit, it); ++it;
	pqxx2c(dst.hangle_limit, it); ++it;
	pqxx2c(dst.push1_speed, it); ++it;
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
inline void pqxx::internal::params::add_field(const enum heat_material_t & src)
{ add_field(e2pqxx(src)); }
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
inline void pqxx::internal::params::add_field(const enum shrinktube_length_t & src)
{ add_field(e2pqxx(src)); }
static inline void pqxx2c(enum shrinktube_length_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum shrinktube_length_t)search_val_binary(str2e_shrinktube_length_t, it->c_str()); }

template<>
void pqxx::internal::params::add_field(const heat_param_cfg & src)
{
	add_field(src.seqn);
	add_field(src.name);
	add_field(src.material);
	add_field(src.length);
	add_field(src.auto_heat);
	add_field(src.heat_time);
	add_field(src.heat_temp);
	add_field(src.finish_temp);
	add_field(src.fast_heat);
	add_field(src.hold_temp);
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
} str2e_fs_err_t[30] = {
	{ "ABNORMAL_ARC", 11 },
	{ "ARC1_REVISE", 19 },
	{ "ARC2_REVISE", 20 },
	{ "ARC_OVERFLOW", 29 },
	{ "ARC_POS", 28 },
	{ "ARC_TIME_ZERO", 18 },
	{ "CALIBRATE_TIMEOUT", 16 },
	{ "CMOS_EXP", 26 },
	{ "COVER_OPENNED", 3 },
	{ "CROSS_OVER", 7 },
	{ "FIBER_BROKEN", 13 },
	{ "FIBER_DEFECT", 5 },
	{ "FIBER_OFFSIDE", 25 },
	{ "FIBER_RECO", 6 },
	{ "FOCUS_X", 21 },
	{ "FOCUS_Y", 22 },
	{ "IMG_DUST", 10 },
	{ "IMG_LUM", 9 },
	{ "IMG_PROCESS", 23 },
	{ "LOSS_EST", 27 },
	{ "MEDDIACY", 1 },
	{ "NO_FIBER", 4 },
	{ "OFF_CENTER", 8 },
	{ "PUSH_TIMEOUT", 15 },
	{ "QUIT_MIDWAY", 14 },
	{ "RESET_TIMEOUT", 17 },
	{ "SUCCESS", 0 },
	{ "SYSTEM_ERR", 24 },
	{ "TENSE_TEST_FAIL", 12 },
	{ "UNKOWN_ERR", 2 },
};

/// @fs_err_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_fs_err_t[30] = {
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
	{ 10, "IMG_DUST" },
	{ 11, "ABNORMAL_ARC" },
	{ 12, "TENSE_TEST_FAIL" },
	{ 13, "FIBER_BROKEN" },
	{ 14, "QUIT_MIDWAY" },
	{ 15, "PUSH_TIMEOUT" },
	{ 16, "CALIBRATE_TIMEOUT" },
	{ 17, "RESET_TIMEOUT" },
	{ 18, "ARC_TIME_ZERO" },
	{ 19, "ARC1_REVISE" },
	{ 20, "ARC2_REVISE" },
	{ 21, "FOCUS_X" },
	{ 22, "FOCUS_Y" },
	{ 23, "IMG_PROCESS" },
	{ 24, "SYSTEM_ERR" },
	{ 25, "FIBER_OFFSIDE" },
	{ 26, "CMOS_EXP" },
	{ 27, "LOSS_EST" },
	{ 28, "ARC_POS" },
	{ 29, "ARC_OVERFLOW" },
};

static inline const char * e2pqxx(const enum fs_err_t src)
{ return search_name_directly(e2str_fs_err_t, (unsigned)src); }

template<>
inline void pqxx::internal::params::add_field(const enum fs_err_t & src)
{ add_field(e2pqxx(src)); }
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
inline void pqxx::internal::params::add_field(const enum ifd_t & src)
{ add_field(e2pqxx(src)); }
static inline void pqxx2c(enum ifd_t & dst, const pqxx::const_row_iterator & it)
{ uint32_t v; it->to(v); dst = (enum ifd_t)v; }

template<>
void pqxx::internal::params::add_field(const fusion_splice_result & src)
{
	add_field(src.name);
	add_field(src.fsp_seqn);
	add_field(src.fsp_ver);
	add_field(src.time_consume);
	add_field(src.code);
	add_field(src.loss);
	add_field(src.recinfo.lft.ft);
	add_field(src.recinfo.lft.clad_dm);
	add_field(src.recinfo.lft.core_dm);
	add_field(src.recinfo.rt.ft);
	add_field(src.recinfo.rt.clad_dm);
	add_field(src.recinfo.rt.core_dm);
	add_field(src.defect.yzl.dbmp);
	add_field(src.defect.yzl.hangle);
	add_field(src.defect.yzl.vangle);
	add_field(src.defect.yzl.clad_dm);
	add_field(src.defect.yzl.core_dm);
	add_field(src.defect.yzr.dbmp);
	add_field(src.defect.yzr.hangle);
	add_field(src.defect.yzr.vangle);
	add_field(src.defect.yzr.clad_dm);
	add_field(src.defect.yzr.core_dm);
	add_field(src.defect.xzl.dbmp);
	add_field(src.defect.xzl.hangle);
	add_field(src.defect.xzl.vangle);
	add_field(src.defect.xzl.clad_dm);
	add_field(src.defect.xzl.core_dm);
	add_field(src.defect.xzr.dbmp);
	add_field(src.defect.xzr.hangle);
	add_field(src.defect.xzr.vangle);
	add_field(src.defect.xzr.clad_dm);
	add_field(src.defect.xzr.core_dm);
	add_field(src.defect.yzl_hangle);
	add_field(src.defect.yzr_hangle);
	add_field(src.defect.xzl_hangle);
	add_field(src.defect.xzr_hangle);
	add_field(src.defect.intersect_hangle);
	add_field(src.defect.lft_vangle);
	add_field(src.defect.rt_vangle);
	add_field(src.defect.yz_img);
	add_field(src.defect.xz_img);
	add_field(src.defect.yz_defect_img);
	add_field(src.defect.xz_defect_img);
	add_field(src.prestate.core_offset);
	add_field(src.prestate.clad_offset);
	add_field(src.prestate.endface_gap);
	add_field(src.prestate.vertex_angle);
	add_field(src.tense_test.exed);
	add_field(src.tense_test.pass);
	add_field(src.manual_arc.count);
	add_field(src.xz_final_img);
	add_field(src.yz_final_img);
}

pqxx::const_row_iterator pqxx2c(fusion_splice_result & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.name, it); ++it;
	pqxx2c(dst.fsp_seqn, it); ++it;
	pqxx2c(dst.fsp_ver, it); ++it;
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
	pqxx2c(dst.defect.yzl_hangle, it); ++it;
	pqxx2c(dst.defect.yzr_hangle, it); ++it;
	pqxx2c(dst.defect.xzl_hangle, it); ++it;
	pqxx2c(dst.defect.xzr_hangle, it); ++it;
	pqxx2c(dst.defect.intersect_hangle, it); ++it;
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

/// @fs_display_mode_t : string to enum
static const struct {
	const char * name;
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
	const char * name;
} e2str_fs_display_mode_t[5] = {
	{ 0, "X" },
	{ 1, "Y" },
	{ 2, "TB" },
	{ 3, "LR" },
	{ 4, "NO" },
};

static inline const char * e2pqxx(const enum fs_display_mode_t src)
{ return search_name_directly(e2str_fs_display_mode_t, (unsigned)src); }

template<>
inline void pqxx::internal::params::add_field(const enum fs_display_mode_t & src)
{ add_field(e2pqxx(src)); }
static inline void pqxx2c(enum fs_display_mode_t & dst, const pqxx::const_row_iterator &it)
{ dst = (enum fs_display_mode_t)search_val_binary(str2e_fs_display_mode_t, it->c_str()); }

template<>
void pqxx::internal::params::add_field(const sys_cfg & src)
{
	add_field(src.iaparam.bg_lum);
	add_field(src.iaparam.lens_mag);
	add_field(src.iaparam.cap_delay);
	add_field(src.iaparam.cover_delay);
	add_field(src.iaparam.fullx.left);
	add_field(src.iaparam.fullx.top);
	add_field(src.iaparam.fullx.width);
	add_field(src.iaparam.fullx.height);
	add_field(src.iaparam.winx.left);
	add_field(src.iaparam.winx.top);
	add_field(src.iaparam.winx.width);
	add_field(src.iaparam.winx.height);
	add_field(src.iaparam.fully.left);
	add_field(src.iaparam.fully.top);
	add_field(src.iaparam.fully.width);
	add_field(src.iaparam.fully.height);
	add_field(src.iaparam.winy.left);
	add_field(src.iaparam.winy.top);
	add_field(src.iaparam.winy.width);
	add_field(src.iaparam.winy.height);
	add_field(src.iaparam.ledx_lum);
	add_field(src.iaparam.ledy_lum);
	add_field(src.iaparam.cmosx_exposure);
	add_field(src.iaparam.cmosy_exposure);
	add_field(src.iaparam.dc_th0);
	add_field(src.iaparam.dc_th1);
	add_field(src.iaparam.denoise_th);
	add_field(src.iaparam.loss_est_factor);
	add_field(src.iaparam.vdist_th0);
	add_field(src.iaparam.vdist_th1);
	add_field(src.iaparam.hdist_th0);
	add_field(src.iaparam.hdist_th1);
	add_field(src.iaparam.hangle_th);
	add_field(src.iaparam.vangle_th);
	add_field(src.fsparam.seqn);
	add_field(src.fsparam.name);
	add_field(src.fsparam.ver);
	add_field(src.fsparam.fusion_mode);
	add_field(src.fsparam.lfti);
	add_field(src.fsparam.rfti);
	add_field(src.fsparam.align_mode);
	add_field(src.fsparam.x_focus);
	add_field(src.fsparam.y_focus);
	add_field(src.fsparam.ecf_redress);
	add_field(src.fsparam.auto_mag);
	add_field(src.fsparam.vangle_limit);
	add_field(src.fsparam.hangle_limit);
	add_field(src.fsparam.push1_speed);
	add_field(src.fsparam.clr_mag);
	add_field(src.fsparam.clr_time);
	add_field(src.fsparam.clr_pos);
	add_field(src.fsparam.position);
	add_field(src.fsparam.gap);
	add_field(src.fsparam.overlap);
	add_field(src.fsparam.pre_mag);
	add_field(src.fsparam.pre_time);
	add_field(src.fsparam.arc1_mag);
	add_field(src.fsparam.arc1_time);
	add_field(src.fsparam.arc2_mag);
	add_field(src.fsparam.arc2_time);
	add_field(src.fsparam.arc2_on_time);
	add_field(src.fsparam.arc2_off_time);
	add_field(src.fsparam.arc_man_time);
	add_field(src.fsparam.lft_push_speed);
	add_field(src.fsparam.rt_push_speed);
	add_field(src.fsparam.taper_splice);
	add_field(src.fsparam.taper_wait_time);
	add_field(src.fsparam.taper_length);
	add_field(src.fsparam.taper_speed);
	add_field(src.fsparam.tense_test);
	add_field(src.fsparam.tense_speed);
	add_field(src.fsparam.tense_length);
	add_field(src.fsparam.loss_mode);
	add_field(src.fsparam.loss_limit);
	add_field(src.fsparam.loss_min);
	add_field(src.fsparam.lft_mfd);
	add_field(src.fsparam.rt_mfd);
	add_field(src.fsparam.syn_bend_co);
	add_field(src.fsparam.opp_bend_co);
	add_field(src.fsparam.mfd_mis_co);
	add_field(src.fsoption.operation.autoStart);
	add_field(src.fsoption.operation.pause1);
	add_field(src.fsoption.operation.pause2);
	add_field(src.fsoption.dataDisplay.cleaveAngle);
	add_field(src.fsoption.dataDisplay.axisOffset);
	add_field(src.fsoption.dataDisplay.arcCorrectedValue);
	add_field(src.fsoption.ignoreOptions.cleave);
	add_field(src.fsoption.ignoreOptions.loss);
	add_field(src.fsoption.ignoreOptions.fiberCoreAngle);
	add_field(src.fsoption.ignoreOptions.bubble);
	add_field(src.fsoption.ignoreOptions.thick);
	add_field(src.fsoption.ignoreOptions.thin);
	add_field(src.fsoption.arcCompensation.pressure);
	add_field(src.fsoption.arcCompensation.temperature);
	add_field(src.fsoption.arcCompensation.realTimeRevise);
	add_field(src.fsoption.fiberDisplay.gapSet);
	add_field(src.fsoption.fiberDisplay.pause1);
	add_field(src.fsoption.fiberDisplay.align);
	add_field(src.fsoption.fiberDisplay.pause2);
	add_field(src.fsoption.fiberDisplay.arc);
	add_field(src.fsoption.fiberDisplay.estLoss);
	add_field(src.fsoption.others.autoFeedFiber);
	add_field(src.fsoption.others.badCleavedEndface);
	add_field(src.fsoption.others.resetAfterSplice);
	add_field(src.fsoption.others.cleanAgain);
	add_field(src.fsoption.others.imageZoomIn);
	add_field(src.fsoption.others.manualArcLimit);
	add_field(src.heatparam.seqn);
	add_field(src.heatparam.name);
	add_field(src.heatparam.material);
	add_field(src.heatparam.length);
	add_field(src.heatparam.auto_heat);
	add_field(src.heatparam.heat_time);
	add_field(src.heatparam.heat_temp);
	add_field(src.heatparam.finish_temp);
	add_field(src.heatparam.fast_heat);
	add_field(src.heatparam.hold_temp);
}

pqxx::const_row_iterator pqxx2c(sys_cfg & dst, const pqxx::const_row_iterator & src)
{
	pqxx::const_row_iterator it = src;
	pqxx2c(dst.iaparam.bg_lum, it); ++it;
	pqxx2c(dst.iaparam.lens_mag, it); ++it;
	pqxx2c(dst.iaparam.cap_delay, it); ++it;
	pqxx2c(dst.iaparam.cover_delay, it); ++it;
	pqxx2c(dst.iaparam.fullx.left, it); ++it;
	pqxx2c(dst.iaparam.fullx.top, it); ++it;
	pqxx2c(dst.iaparam.fullx.width, it); ++it;
	pqxx2c(dst.iaparam.fullx.height, it); ++it;
	pqxx2c(dst.iaparam.winx.left, it); ++it;
	pqxx2c(dst.iaparam.winx.top, it); ++it;
	pqxx2c(dst.iaparam.winx.width, it); ++it;
	pqxx2c(dst.iaparam.winx.height, it); ++it;
	pqxx2c(dst.iaparam.fully.left, it); ++it;
	pqxx2c(dst.iaparam.fully.top, it); ++it;
	pqxx2c(dst.iaparam.fully.width, it); ++it;
	pqxx2c(dst.iaparam.fully.height, it); ++it;
	pqxx2c(dst.iaparam.winy.left, it); ++it;
	pqxx2c(dst.iaparam.winy.top, it); ++it;
	pqxx2c(dst.iaparam.winy.width, it); ++it;
	pqxx2c(dst.iaparam.winy.height, it); ++it;
	pqxx2c(dst.iaparam.ledx_lum, it); ++it;
	pqxx2c(dst.iaparam.ledy_lum, it); ++it;
	pqxx2c(dst.iaparam.cmosx_exposure, it); ++it;
	pqxx2c(dst.iaparam.cmosy_exposure, it); ++it;
	pqxx2c(dst.iaparam.dc_th0, it); ++it;
	pqxx2c(dst.iaparam.dc_th1, it); ++it;
	pqxx2c(dst.iaparam.denoise_th, it); ++it;
	pqxx2c(dst.iaparam.loss_est_factor, it); ++it;
	pqxx2c(dst.iaparam.vdist_th0, it); ++it;
	pqxx2c(dst.iaparam.vdist_th1, it); ++it;
	pqxx2c(dst.iaparam.hdist_th0, it); ++it;
	pqxx2c(dst.iaparam.hdist_th1, it); ++it;
	pqxx2c(dst.iaparam.hangle_th, it); ++it;
	pqxx2c(dst.iaparam.vangle_th, it); ++it;
	pqxx2c(dst.fsparam.seqn, it); ++it;
	pqxx2c(dst.fsparam.name, it); ++it;
	pqxx2c(dst.fsparam.ver, it); ++it;
	pqxx2c(dst.fsparam.fusion_mode, it); ++it;
	pqxx2c(dst.fsparam.lfti, it); ++it;
	pqxx2c(dst.fsparam.rfti, it); ++it;
	pqxx2c(dst.fsparam.align_mode, it); ++it;
	pqxx2c(dst.fsparam.x_focus, it); ++it;
	pqxx2c(dst.fsparam.y_focus, it); ++it;
	pqxx2c(dst.fsparam.ecf_redress, it); ++it;
	pqxx2c(dst.fsparam.auto_mag, it); ++it;
	pqxx2c(dst.fsparam.vangle_limit, it); ++it;
	pqxx2c(dst.fsparam.hangle_limit, it); ++it;
	pqxx2c(dst.fsparam.push1_speed, it); ++it;
	pqxx2c(dst.fsparam.clr_mag, it); ++it;
	pqxx2c(dst.fsparam.clr_time, it); ++it;
	pqxx2c(dst.fsparam.clr_pos, it); ++it;
	pqxx2c(dst.fsparam.position, it); ++it;
	pqxx2c(dst.fsparam.gap, it); ++it;
	pqxx2c(dst.fsparam.overlap, it); ++it;
	pqxx2c(dst.fsparam.pre_mag, it); ++it;
	pqxx2c(dst.fsparam.pre_time, it); ++it;
	pqxx2c(dst.fsparam.arc1_mag, it); ++it;
	pqxx2c(dst.fsparam.arc1_time, it); ++it;
	pqxx2c(dst.fsparam.arc2_mag, it); ++it;
	pqxx2c(dst.fsparam.arc2_time, it); ++it;
	pqxx2c(dst.fsparam.arc2_on_time, it); ++it;
	pqxx2c(dst.fsparam.arc2_off_time, it); ++it;
	pqxx2c(dst.fsparam.arc_man_time, it); ++it;
	pqxx2c(dst.fsparam.lft_push_speed, it); ++it;
	pqxx2c(dst.fsparam.rt_push_speed, it); ++it;
	pqxx2c(dst.fsparam.taper_splice, it); ++it;
	pqxx2c(dst.fsparam.taper_wait_time, it); ++it;
	pqxx2c(dst.fsparam.taper_length, it); ++it;
	pqxx2c(dst.fsparam.taper_speed, it); ++it;
	pqxx2c(dst.fsparam.tense_test, it); ++it;
	pqxx2c(dst.fsparam.tense_speed, it); ++it;
	pqxx2c(dst.fsparam.tense_length, it); ++it;
	pqxx2c(dst.fsparam.loss_mode, it); ++it;
	pqxx2c(dst.fsparam.loss_limit, it); ++it;
	pqxx2c(dst.fsparam.loss_min, it); ++it;
	pqxx2c(dst.fsparam.lft_mfd, it); ++it;
	pqxx2c(dst.fsparam.rt_mfd, it); ++it;
	pqxx2c(dst.fsparam.syn_bend_co, it); ++it;
	pqxx2c(dst.fsparam.opp_bend_co, it); ++it;
	pqxx2c(dst.fsparam.mfd_mis_co, it); ++it;
	pqxx2c(dst.fsoption.operation.autoStart, it); ++it;
	pqxx2c(dst.fsoption.operation.pause1, it); ++it;
	pqxx2c(dst.fsoption.operation.pause2, it); ++it;
	pqxx2c(dst.fsoption.dataDisplay.cleaveAngle, it); ++it;
	pqxx2c(dst.fsoption.dataDisplay.axisOffset, it); ++it;
	pqxx2c(dst.fsoption.dataDisplay.arcCorrectedValue, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.cleave, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.loss, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.fiberCoreAngle, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.bubble, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.thick, it); ++it;
	pqxx2c(dst.fsoption.ignoreOptions.thin, it); ++it;
	pqxx2c(dst.fsoption.arcCompensation.pressure, it); ++it;
	pqxx2c(dst.fsoption.arcCompensation.temperature, it); ++it;
	pqxx2c(dst.fsoption.arcCompensation.realTimeRevise, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.gapSet, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.pause1, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.align, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.pause2, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.arc, it); ++it;
	pqxx2c(dst.fsoption.fiberDisplay.estLoss, it); ++it;
	pqxx2c(dst.fsoption.others.autoFeedFiber, it); ++it;
	pqxx2c(dst.fsoption.others.badCleavedEndface, it); ++it;
	pqxx2c(dst.fsoption.others.resetAfterSplice, it); ++it;
	pqxx2c(dst.fsoption.others.cleanAgain, it); ++it;
	pqxx2c(dst.fsoption.others.imageZoomIn, it); ++it;
	pqxx2c(dst.fsoption.others.manualArcLimit, it); ++it;
	pqxx2c(dst.heatparam.seqn, it); ++it;
	pqxx2c(dst.heatparam.name, it); ++it;
	pqxx2c(dst.heatparam.material, it); ++it;
	pqxx2c(dst.heatparam.length, it); ++it;
	pqxx2c(dst.heatparam.auto_heat, it); ++it;
	pqxx2c(dst.heatparam.heat_time, it); ++it;
	pqxx2c(dst.heatparam.heat_temp, it); ++it;
	pqxx2c(dst.heatparam.finish_temp, it); ++it;
	pqxx2c(dst.heatparam.fast_heat, it); ++it;
	pqxx2c(dst.heatparam.hold_temp, it); ++it;
	return it;
}

