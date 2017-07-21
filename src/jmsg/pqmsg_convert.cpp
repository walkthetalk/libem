
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

static inline void pqxx2e(enum fs_pattern_t & dst, const char * src)
{ dst = (enum fs_pattern_t)search_val_binary(str2e_fs_pattern_t, src); }

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

static inline void pqxx2e(enum fiber_t & dst, const char * src)
{ dst = (enum fiber_t)search_val_binary(str2e_fiber_t, src); }

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

static inline void pqxx2e(enum align_method_t & dst, const char * src)
{ dst = (enum align_method_t)search_val_binary(str2e_align_method_t, src); }

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

static inline void pqxx2e(enum loss_estimate_mode_t & dst, const char * src)
{ dst = (enum loss_estimate_mode_t)search_val_binary(str2e_loss_estimate_mode_t, src); }

template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const fs_param_cfg & src)
{
	return (*this)(e2pqxx(src.fusion_mode))(e2pqxx(src.lfti))(e2pqxx(src.rfti))(e2pqxx(src.align_mode))(src.x_focus)(src.y_focus)(src.ecf_redress)(src.auto_mag)(src.vangle_limit)(src.hangle_limit)(src.clr_mag)(src.clr_time)(src.clr_pos)(src.position)(src.gap)(src.overlap)(src.pre_mag)(src.pre_time)(src.arc1_mag)(src.arc1_time)(src.arc2_mag)(src.arc2_time)(src.arc2_on_time)(src.arc2_off_time)(src.arc_man_time)(src.lft_push_speed)(src.rt_push_speed)(src.taper_splice)(src.taper_wait_time)(src.taper_length)(src.taper_speed)(src.tense_test)(src.tense_speed)(src.tense_length)(e2pqxx(src.loss_mode))(src.loss_limit)(src.loss_min)(src.lft_mfd)(src.rt_mfd)(src.syn_bend_co)(src.opp_bend_co)(src.mfd_mis_co);
}

pqxx::const_tuple_iterator pqxx2c(fs_param_cfg & dst, const pqxx::const_tuple_iterator & src)
{
	pqxx::const_tuple_iterator it = src;
	pqxx2e(dst.fusion_mode, it->c_str()); ++it;
	pqxx2e(dst.lfti, it->c_str()); ++it;
	pqxx2e(dst.rfti, it->c_str()); ++it;
	pqxx2e(dst.align_mode, it->c_str()); ++it;
	it->to(dst.x_focus); ++it;
	it->to(dst.y_focus); ++it;
	it->to(dst.ecf_redress); ++it;
	it->to(dst.auto_mag); ++it;
	it->to(dst.vangle_limit); ++it;
	it->to(dst.hangle_limit); ++it;
	it->to(dst.clr_mag); ++it;
	it->to(dst.clr_time); ++it;
	it->to(dst.clr_pos); ++it;
	it->to(dst.position); ++it;
	it->to(dst.gap); ++it;
	it->to(dst.overlap); ++it;
	it->to(dst.pre_mag); ++it;
	it->to(dst.pre_time); ++it;
	it->to(dst.arc1_mag); ++it;
	it->to(dst.arc1_time); ++it;
	it->to(dst.arc2_mag); ++it;
	it->to(dst.arc2_time); ++it;
	it->to(dst.arc2_on_time); ++it;
	it->to(dst.arc2_off_time); ++it;
	it->to(dst.arc_man_time); ++it;
	it->to(dst.lft_push_speed); ++it;
	it->to(dst.rt_push_speed); ++it;
	it->to(dst.taper_splice); ++it;
	it->to(dst.taper_wait_time); ++it;
	it->to(dst.taper_length); ++it;
	it->to(dst.taper_speed); ++it;
	it->to(dst.tense_test); ++it;
	it->to(dst.tense_speed); ++it;
	it->to(dst.tense_length); ++it;
	pqxx2e(dst.loss_mode, it->c_str()); ++it;
	it->to(dst.loss_limit); ++it;
	it->to(dst.loss_min); ++it;
	it->to(dst.lft_mfd); ++it;
	it->to(dst.rt_mfd); ++it;
	it->to(dst.syn_bend_co); ++it;
	it->to(dst.opp_bend_co); ++it;
	it->to(dst.mfd_mis_co); ++it;
	return it;
}

