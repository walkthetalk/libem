#pragma once
#include <map>
#include <string>
#include <functional>

#include "jmsg_id.hpp"


/// @class rcver : used to receive messages
class rcver final {
public:
	rcver();
	~rcver();
public:
	void process(void *, size_t);

	template<mid_t mid, template<mid_t> class T>
	void register_callback(std::function<void(T<mid> &)> cb)
	{
		m_cbs[__mid_to_str(mid)] = [this, cb](void) -> void {
			T<mid> tmp;
			__unpack((typename msg_helper<mid>::value_type &)tmp);
			cb(tmp);
		};
	}

	template<mid_t mid>
	void register_callback(std::function<void(typename msg_helper<mid>::value_type &)> cb)
	{
		m_cbs[__mid_to_str(mid)] = [this, cb](void) -> void {
			typename msg_helper<mid>::value_type msg;
			__unpack(msg);
			cb(msg);
		};
	}

	template<typename _T>
	void convert(_T & dst, const std::string & src)
	{
		convert(dst, src.c_str());
	}
	void convert(struct fs_param_cfg & dst, const char * src);
	void convert(struct heat_param_cfg & dst, const char * src);
	void convert(struct misc_cfg & dst, const char * src);
	void convert(struct fs_option_cfg & dst, const char * src);
	void convert(struct motor_spec & dst, const char * src);
	void convert(struct cmos_spec & dst, const char * src);
	void convert(struct hvb_spec & dst, const char * src);
	void convert(struct ia_spec & dst, const char * src);
	void convert(struct mc_spec & dst, const char * src);
	void convert(struct ar_spec & dst, const char * src);
	void convert(struct rr_spec & dst, const char * src);
	void convert(struct fr_spec & dst, const char * src);

private:
	const char * __mid_to_str(mid_t mid);
	void __unpack(struct defect_detect_result & dst);
	void __unpack(struct fs_state & dst);
	void __unpack(struct heat_state & dst);
	void __unpack(struct simple_msg & dst);
	void __unpack(struct fspre_state & dst);
	void __unpack(struct tense_test_result & dst);
	void __unpack(struct loss_estimating_result & dst);
	void __unpack(struct fiber_reco_result & dst);
	void __unpack(struct manual_arc_result & dst);
	void __unpack(struct fusion_splice_result & dst);
	void __unpack(struct arc_revise & dst);
	void __unpack(struct fs_da_cfg & dst);
	void __unpack(struct discharge_adjust_result & dst);
	void __unpack(struct discharge & dst);
	void __unpack(struct discharge_count & dst);
	void __unpack(struct set_fs_display_mode_ext & dst);
	void __unpack(struct set_fs_display_zoom_ext & dst);
	void __unpack(struct set_fs_display_mode & dst);
	void __unpack(struct dust_check_result & dst);
	void __unpack(struct heat_start & dst);
	void __unpack(struct heat_result & dst);
	void __unpack(struct image_move & dst);
	void __unpack(struct fs_cover_state & dst);
	void __unpack(struct set_lcd_brightness & dst);
	void __unpack(struct lcd_power_ctl & dst);
	void __unpack(struct set_led & dst);
	void __unpack(struct motor_start & dst);
	void __unpack(struct motor_stop & dst);
	void __unpack(struct fs_mt_cfg & dst);
	void __unpack(struct motor_test_result & dst);
	void __unpack(struct process_progress & dst);
	void __unpack(struct fs_rr_cfg & dst);
	void __unpack(struct realtime_revise_result & dst);
	void __unpack(struct regular_test_result & dst);
	void __unpack(struct fs_se_cfg & dst);
	void __unpack(struct stabilize_electrode_result & dst);
	void __unpack(struct report_dev_state & dst);
	void __unpack(struct report_wave_form & dst);
	void __unpack(struct fs_ft_cfg & dst);
	void __unpack(struct fiber_train_result & dst);
	void __unpack(struct count_down & dst);
	void __unpack(struct dustCheckFullStart & dst);
	void __unpack(struct update_window_position & dst);
	void __unpack(struct update_led_brightness & dst);
	void __unpack(struct bat_state & dst);
	void __unpack(struct beep & dst);

private:
	void __reset();
private:
	void * m_doc;
	std::map<std::string, std::function<void(void)>> m_cbs;
};


