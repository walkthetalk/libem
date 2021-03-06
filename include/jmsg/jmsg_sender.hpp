#pragma once
#include <map>
#include <string>
#include <functional>

#include "jmsg_id.hpp"


/// @class sender : used to send messages
class sender final {
public:
	sender(size_t reserve_header_size = 0);
	~sender();
public:
	template<mid_t mid, typename _T >
	void fill_to(typename msg_helper<mid>::value_type const & msg,  _T & o, void (_T::*f)(void *, size_t))
	{
		__reset(mid);
		__pack(msg);
		__serialize();
		o.f(m_buf, __size());
	}

	template< mid_t mid >
	void fill_to(typename msg_helper<mid>::value_type const & msg, std::function<void(void *, size_t)>  f)
	{
		__reset(mid);
		__pack(msg);
		__serialize();
		f(m_buf, __size());
	}
	void convert(std::string & dst, const struct fs_param_cfg & src);
	void convert(std::string & dst, const struct heat_param_cfg & src);
	void convert(std::string & dst, const struct misc_cfg & src);
	void convert(std::string & dst, const struct fs_option_cfg & src);
	void convert(std::string & dst, const struct fusion_splice_result & src);
	void convert(std::string & dst, const struct motor_spec & src);
	void convert(std::string & dst, const struct cmos_spec & src);
	void convert(std::string & dst, const struct hvb_spec & src);
	void convert(std::string & dst, const struct ia_spec & src);
	void convert(std::string & dst, const struct mc_spec & src);
	void convert(std::string & dst, const struct ar_spec & src);
	void convert(std::string & dst, const struct rr_spec & src);
	void convert(std::string & dst, const struct fr_spec & src);
	void convert(std::string & dst, const struct sys_cfg & src);

private:
	void __pack(const struct defect_detect_result & val);
	void __pack(const struct fs_state & val);
	void __pack(const struct heat_state & val);
	void __pack(const struct simple_msg & val);
	void __pack(const struct fspre_state & val);
	void __pack(const struct tense_test_result & val);
	void __pack(const struct loss_estimating_result & val);
	void __pack(const struct fiber_reco_result & val);
	void __pack(const struct manual_arc_result & val);
	void __pack(const struct fusion_splice_result & val);
	void __pack(const struct arc_revise & val);
	void __pack(const struct fs_da_cfg & val);
	void __pack(const struct discharge_adjust_result & val);
	void __pack(const struct discharge & val);
	void __pack(const struct discharge_v2 & val);
	void __pack(const struct discharge_count & val);
	void __pack(const struct sstream_display_info & val);
	void __pack(const struct mstream_display_info & val);
	void __pack(const struct dust_check_result & val);
	void __pack(const struct heat_start & val);
	void __pack(const struct heat_result & val);
	void __pack(const struct move_image & val);
	void __pack(const struct fs_cover_state & val);
	void __pack(const struct set_lcd_brightness & val);
	void __pack(const struct lcd_power_ctl & val);
	void __pack(const struct set_led & val);
	void __pack(const struct set_exposure & val);
	void __pack(const struct motor_start_info & val);
	void __pack(const struct motor_stop_info & val);
	void __pack(const struct fs_mt_cfg & val);
	void __pack(const struct motor_test_result & val);
	void __pack(const struct process_progress & val);
	void __pack(const struct fs_rr_cfg & val);
	void __pack(const struct realtime_revise_result & val);
	void __pack(const struct regular_test_result & val);
	void __pack(const struct fs_se_cfg & val);
	void __pack(const struct stabilize_electrode_result & val);
	void __pack(const struct report_dev_state & val);
	void __pack(const struct report_wave_form & val);
	void __pack(const struct fs_ft_cfg & val);
	void __pack(const struct fiber_train_result & val);
	void __pack(const struct count_down & val);
	void __pack(const struct dustCheckFullStart & val);
	void __pack(const struct update_window_position & val);
	void __pack(const struct update_led_brightness & val);
	void __pack(const struct bat_state & val);
	void __pack(const struct beep & val);
	void __pack(const struct motor_speed_info & val);
	void __pack(const struct llvl_request & val);
	void __pack(const struct sys_cfg & val);

private:
	void __reset(mid_t mid);
	void __serialize();
private:
	std::size_t __size() const
	{
		return (m_pcur - m_buf);
	}
private:
	void __fill_header();
private:
	uint8_t m_lbuf[1048576];
	uint8_t * const m_buf;
	uint8_t * m_pcur;
	const uint8_t * const m_pend;
	void * m_doc;
};


