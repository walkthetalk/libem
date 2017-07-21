
#include <stdexcept>
#include <cstring>

#include "jmsg/jmsg_types.hpp"
#include "jmsg/pqxxutils.hpp"

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

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		V cur = flist[mid].val;
		if (cur == val) {
			return flist[mid].name;
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
auto search_val_binary(const T & flist, const char * name) -> decltype(flist[0].val)
{
	std::size_t head = 0;
	std::size_t tail = std::extent<T>::value - 1;

	while (head < tail) {
		std::size_t mid = (head + tail) / 2;
		int cmp_result = std::strcmp(flist[mid].name, name);
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
auto search_val_binary(const T & flist, const std::string & name) -> decltype(flist[0].val)
{
	search_val_binary(flist, name.c_str());
}

/// @fs_pattern_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_fs_pattern_t[4] = {
	{ "automatic", 0 },
	{ "calibrate", 1 },
	{ "normal", 2 },
	{ "special", 3 },
};

/// @fs_pattern_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_fs_pattern_t[4] = {
	{ 0, "automatic" },
	{ 1, "calibrate" },
	{ 2, "normal" },
	{ 3, "special" },
};

static inline const char * e2pqxx(const enum fs_pattern_t src)
{ return search_name_directly(e2str_fs_pattern_t, (unsigned)src); }

static inline void pqxx2e(enum fs_pattern_t & dst, const char * src)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, src); }

/// @fiber_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_fiber_t[6] = {
	{ "automatic", 0 },
	{ "ds", 2 },
	{ "follow", 5 },
	{ "mm", 4 },
	{ "nz", 3 },
	{ "sm", 1 },
};

/// @fiber_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_fiber_t[6] = {
	{ 0, "automatic" },
	{ 1, "sm" },
	{ 2, "ds" },
	{ 3, "nz" },
	{ 4, "mm" },
	{ 5, "follow" },
};

static inline const char * e2pqxx(const enum fiber_t src)
{ return search_name_directly(e2str_fiber_t, (unsigned)src); }

static inline void pqxx2e(enum fiber_t & dst, const char * src)
{ dst = (enum fiber_t)search_val_binary(str2e_fiber_t, src); }

/// @align_method_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_align_method_t[5] = {
	{ "automatic", 0 },
	{ "clad", 2 },
	{ "core", 3 },
	{ "fine", 1 },
	{ "manual", 4 },
};

/// @align_method_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_align_method_t[5] = {
	{ 0, "automatic" },
	{ 1, "fine" },
	{ 2, "clad" },
	{ 3, "core" },
	{ 4, "manual" },
};

static inline const char * e2pqxx(const enum align_method_t src)
{ return search_name_directly(e2str_align_method_t, (unsigned)src); }

static inline void pqxx2e(enum align_method_t & dst, const char * src)
{ dst = (enum align_method_t)search_val_binary(str2e_align_method_t, src); }

/// @loss_estimate_mode_t : string to enum
static const struct {
	const char * name;
	unsigned val;
} str2e_loss_estimate_mode_t[4] = {
	{ "accurate", 1 },
	{ "cladding", 3 },
	{ "core", 2 },
	{ "off", 0 },
};

/// @loss_estimate_mode_t : enum to string
static const struct {
	unsigned val;
	const char * name;
} e2str_loss_estimate_mode_t[4] = {
	{ 0, "off" },
	{ 1, "accurate" },
	{ 2, "core" },
	{ 3, "cladding" },
};

static inline const char * e2pqxx(const enum loss_estimate_mode_t src)
{ return search_name_directly(e2str_loss_estimate_mode_t, (unsigned)src); }

static inline void pqxx2e(enum loss_estimate_mode_t & dst, const char * src)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, src); }

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const fs_param_cfg & src)
{
	return (*this)(e2pqxx(src.fusion_mode))(e2pqxx(src.lfti))(e2pqxx(src.rfti))(e2pqxx(src.align_mode))(src.x_focus)(src.y_focus)(src.ecf_redress)(src.auto_mag)(src.vangle_limit)(src.hangle_limit)(src.clr_mag)(src.clr_time)(src.clr_pos)(src.position)(src.gap)(src.overlap)(src.pre_mag)(src.pre_time)(src.arc1_mag)(src.arc1_time)(src.arc2_mag)(src.arc2_time)(src.arc2_on_time)(src.arc2_off_time)(src.arc_man_time)(src.lft_push_speed)(src.rt_push_speed)(src.taper_splice)(src.taper_wait_time)(src.taper_length)(src.taper_speed)(src.tense_test)(src.tense_speed)(src.tense_length)(e2pqxx(src.loss_mode))(src.loss_limit)(src.loss_min)(src.lft_mfd)(src.rt_mfd)(src.syn_bend_co)(src.opp_bend_co)(src.mfd_mis_co);
}

void pqxx2c(fs_param_cfg & dst, pqxx::const_tuple_iterator & src)
{
	pqxx2e(dst.fusion_mode, src->c_str()); ++src;
	pqxx2e(dst.lfti, src->c_str()); ++src;
	pqxx2e(dst.rfti, src->c_str()); ++src;
	pqxx2e(dst.align_mode, src->c_str()); ++src;
	src->to(dst.x_focus); ++src;
	src->to(dst.y_focus); ++src;
	src->to(dst.ecf_redress); ++src;
	src->to(dst.auto_mag); ++src;
	src->to(dst.vangle_limit); ++src;
	src->to(dst.hangle_limit); ++src;
	src->to(dst.clr_mag); ++src;
	src->to(dst.clr_time); ++src;
	src->to(dst.clr_pos); ++src;
	src->to(dst.position); ++src;
	src->to(dst.gap); ++src;
	src->to(dst.overlap); ++src;
	src->to(dst.pre_mag); ++src;
	src->to(dst.pre_time); ++src;
	src->to(dst.arc1_mag); ++src;
	src->to(dst.arc1_time); ++src;
	src->to(dst.arc2_mag); ++src;
	src->to(dst.arc2_time); ++src;
	src->to(dst.arc2_on_time); ++src;
	src->to(dst.arc2_off_time); ++src;
	src->to(dst.arc_man_time); ++src;
	src->to(dst.lft_push_speed); ++src;
	src->to(dst.rt_push_speed); ++src;
	src->to(dst.taper_splice); ++src;
	src->to(dst.taper_wait_time); ++src;
	src->to(dst.taper_length); ++src;
	src->to(dst.taper_speed); ++src;
	src->to(dst.tense_test); ++src;
	src->to(dst.tense_speed); ++src;
	src->to(dst.tense_length); ++src;
	pqxx2e(dst.loss_mode, src->c_str()); ++src;
	src->to(dst.loss_limit); ++src;
	src->to(dst.loss_min); ++src;
	src->to(dst.lft_mfd); ++src;
	src->to(dst.rt_mfd); ++src;
	src->to(dst.syn_bend_co); ++src;
	src->to(dst.opp_bend_co); ++src;
	src->to(dst.mfd_mis_co); ++src;
}

