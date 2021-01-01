#pragma once

#include <pqxx/result>
#include <pqxx/prepared_statement>

#include "jmsg_types.hpp"

/// pq key list for fs_param_cfg
#define PQKL_fs_param_cfg "seqn,name,ver,fusion_mode,lfti,rfti,align_mode,x_focus,y_focus,ecf_redress,auto_mag,vangle_limit,hangle_limit,push1_speed,clr_mag,clr_time,clr_pos,position,gap,overlap,pre_mag,pre_time,arc1_mag,arc1_time,arc2_mag,arc2_time,arc2_on_time,arc2_off_time,arc_man_time,lft_push_speed,rt_push_speed,taper_splice,taper_wait_time,taper_length,taper_speed,tense_test,tense_speed,tense_length,loss_mode,loss_limit,loss_min,lft_mfd,rt_mfd,syn_bend_co,opp_bend_co,mfd_mis_co"
/// pq occupy symbol list for fs_param_cfg
#define PQOL_fs_param_cfg "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42,$43,$44,$45,$46"
/// pq member size of fs_param_cfg
#define PQMS_fs_param_cfg "46"
template<>
void pqxx::internal::params::add_field(const fs_param_cfg & src);
pqxx::const_row_iterator pqxx2c(fs_param_cfg & dst, const pqxx::const_row_iterator & src);
/// pq key list for heat_param_cfg
#define PQKL_heat_param_cfg "seqn,name,material,length,auto_heat,heat_time,heat_temp,finish_temp,fast_heat,hold_temp"
/// pq occupy symbol list for heat_param_cfg
#define PQOL_heat_param_cfg "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10"
/// pq member size of heat_param_cfg
#define PQMS_heat_param_cfg "10"
template<>
void pqxx::internal::params::add_field(const heat_param_cfg & src);
pqxx::const_row_iterator pqxx2c(heat_param_cfg & dst, const pqxx::const_row_iterator & src);
/// pq key list for fusion_splice_result
#define PQKL_fusion_splice_result "name,fsp_seqn,fsp_ver,time_consume,code,loss,recinfo_lft_ft,recinfo_lft_clad_dm,recinfo_lft_core_dm,recinfo_rt_ft,recinfo_rt_clad_dm,recinfo_rt_core_dm,defect_yzl_dbmp,defect_yzl_hangle,defect_yzl_vangle,defect_yzl_clad_dm,defect_yzl_core_dm,defect_yzr_dbmp,defect_yzr_hangle,defect_yzr_vangle,defect_yzr_clad_dm,defect_yzr_core_dm,defect_xzl_dbmp,defect_xzl_hangle,defect_xzl_vangle,defect_xzl_clad_dm,defect_xzl_core_dm,defect_xzr_dbmp,defect_xzr_hangle,defect_xzr_vangle,defect_xzr_clad_dm,defect_xzr_core_dm,defect_yzl_hangle,defect_yzr_hangle,defect_xzl_hangle,defect_xzr_hangle,defect_intersect_hangle,defect_lft_vangle,defect_rt_vangle,defect_yz_img,defect_xz_img,defect_yz_defect_img,defect_xz_defect_img,prestate_core_offset,prestate_clad_offset,prestate_endface_gap,prestate_vertex_angle,tense_test_exed,tense_test_pass,manual_arc_count,xz_final_img,yz_final_img"
/// pq occupy symbol list for fusion_splice_result
#define PQOL_fusion_splice_result "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42,$43,$44,$45,$46,$47,$48,$49,$50,$51,$52"
/// pq member size of fusion_splice_result
#define PQMS_fusion_splice_result "52"
template<>
void pqxx::internal::params::add_field(const fusion_splice_result & src);
pqxx::const_row_iterator pqxx2c(fusion_splice_result & dst, const pqxx::const_row_iterator & src);
/// pq key list for sys_cfg
#define PQKL_sys_cfg "iaparam_bg_lum,iaparam_lens_mag,iaparam_cap_delay,iaparam_cover_delay,iaparam_winx_width,iaparam_winx_height,iaparam_winx_left,iaparam_winx_top,iaparam_winy_width,iaparam_winy_height,iaparam_winy_left,iaparam_winy_top,iaparam_ledx_lum,iaparam_ledy_lum,iaparam_cmosx_exposure,iaparam_cmosy_exposure,iaparam_dc_th0,iaparam_dc_th1,iaparam_denoise_th,iaparam_loss_est_factor,iaparam_vdist_th0,iaparam_vdist_th1,iaparam_hdist_th0,iaparam_hdist_th1,iaparam_hangle_th,iaparam_vangle_th,fsparam_seqn,fsparam_name,fsparam_ver,fsparam_fusion_mode,fsparam_lfti,fsparam_rfti,fsparam_align_mode,fsparam_x_focus,fsparam_y_focus,fsparam_ecf_redress,fsparam_auto_mag,fsparam_vangle_limit,fsparam_hangle_limit,fsparam_push1_speed,fsparam_clr_mag,fsparam_clr_time,fsparam_clr_pos,fsparam_position,fsparam_gap,fsparam_overlap,fsparam_pre_mag,fsparam_pre_time,fsparam_arc1_mag,fsparam_arc1_time,fsparam_arc2_mag,fsparam_arc2_time,fsparam_arc2_on_time,fsparam_arc2_off_time,fsparam_arc_man_time,fsparam_lft_push_speed,fsparam_rt_push_speed,fsparam_taper_splice,fsparam_taper_wait_time,fsparam_taper_length,fsparam_taper_speed,fsparam_tense_test,fsparam_tense_speed,fsparam_tense_length,fsparam_loss_mode,fsparam_loss_limit,fsparam_loss_min,fsparam_lft_mfd,fsparam_rt_mfd,fsparam_syn_bend_co,fsparam_opp_bend_co,fsparam_mfd_mis_co,fsoption_operation_autoStart,fsoption_operation_pause1,fsoption_operation_pause2,fsoption_dataDisplay_cleaveAngle,fsoption_dataDisplay_axisOffset,fsoption_dataDisplay_arcCorrectedValue,fsoption_ignoreOptions_cleave,fsoption_ignoreOptions_loss,fsoption_ignoreOptions_fiberCoreAngle,fsoption_ignoreOptions_bubble,fsoption_ignoreOptions_thick,fsoption_ignoreOptions_thin,fsoption_arcCompensation_pressure,fsoption_arcCompensation_temperature,fsoption_arcCompensation_realTimeRevise,fsoption_fiberDisplay_gapSet,fsoption_fiberDisplay_pause1,fsoption_fiberDisplay_align,fsoption_fiberDisplay_pause2,fsoption_fiberDisplay_arc,fsoption_fiberDisplay_estLoss,fsoption_others_autoFeedFiber,fsoption_others_badCleavedEndface,fsoption_others_resetAfterSplice,fsoption_others_cleanAgain,fsoption_others_imageZoomIn,fsoption_others_manualArcLimit,heatparam_seqn,heatparam_name,heatparam_material,heatparam_length,heatparam_auto_heat,heatparam_heat_time,heatparam_heat_temp,heatparam_finish_temp,heatparam_fast_heat,heatparam_hold_temp"
/// pq occupy symbol list for sys_cfg
#define PQOL_sys_cfg "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42,$43,$44,$45,$46,$47,$48,$49,$50,$51,$52,$53,$54,$55,$56,$57,$58,$59,$60,$61,$62,$63,$64,$65,$66,$67,$68,$69,$70,$71,$72,$73,$74,$75,$76,$77,$78,$79,$80,$81,$82,$83,$84,$85,$86,$87,$88,$89,$90,$91,$92,$93,$94,$95,$96,$97,$98,$99,$100,$101,$102,$103,$104,$105,$106,$107,$108,$109"
/// pq member size of sys_cfg
#define PQMS_sys_cfg "109"
template<>
void pqxx::internal::params::add_field(const sys_cfg & src);
pqxx::const_row_iterator pqxx2c(sys_cfg & dst, const pqxx::const_row_iterator & src);
