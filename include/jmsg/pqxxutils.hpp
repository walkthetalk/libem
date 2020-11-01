#pragma once

#include <pqxx/result>
#include <pqxx/prepared_statement>

#include "jmsg_types.hpp"

/// pq key list for fs_param_cfg
#define PQKL_fs_param_cfg "seqn,name,ver,fusion_mode,lfti,rfti,align_mode,x_focus,y_focus,ecf_redress,auto_mag,vangle_limit,hangle_limit,clr_mag,clr_time,clr_pos,position,gap,overlap,pre_mag,pre_time,arc1_mag,arc1_time,arc2_mag,arc2_time,arc2_on_time,arc2_off_time,arc_man_time,lft_push_speed,rt_push_speed,taper_splice,taper_wait_time,taper_length,taper_speed,tense_test,tense_speed,tense_length,loss_mode,loss_limit,loss_min,lft_mfd,rt_mfd,syn_bend_co,opp_bend_co,mfd_mis_co"
/// pq occupy symbol list for fs_param_cfg
#define PQOL_fs_param_cfg "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42,$43,$44,$45"
/// pq member size of fs_param_cfg
#define PQMS_fs_param_cfg "45"
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
#define PQKL_fusion_splice_result "name,fsp_seqn,fsp_ver,time_consume,code,loss,recinfo_lft_ft,recinfo_lft_clad_dm,recinfo_lft_core_dm,recinfo_rt_ft,recinfo_rt_clad_dm,recinfo_rt_core_dm,defect_yzl_dbmp,defect_yzl_hangle,defect_yzl_vangle,defect_yzl_clad_dm,defect_yzl_core_dm,defect_yzr_dbmp,defect_yzr_hangle,defect_yzr_vangle,defect_yzr_clad_dm,defect_yzr_core_dm,defect_xzl_dbmp,defect_xzl_hangle,defect_xzl_vangle,defect_xzl_clad_dm,defect_xzl_core_dm,defect_xzr_dbmp,defect_xzr_hangle,defect_xzr_vangle,defect_xzr_clad_dm,defect_xzr_core_dm,defect_yz_hangle,defect_xz_hangle,defect_lft_vangle,defect_rt_vangle,defect_lft_vertex,defect_rt_vertex,defect_yz_img,defect_xz_img,defect_yz_defect_img,defect_xz_defect_img,prestate_core_offset,prestate_clad_offset,prestate_endface_gap,prestate_vertex_angle,tense_test_exed,tense_test_pass,manual_arc_count,xz_final_img,yz_final_img"
/// pq occupy symbol list for fusion_splice_result
#define PQOL_fusion_splice_result "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42,$43,$44,$45,$46,$47,$48,$49,$50,$51"
/// pq member size of fusion_splice_result
#define PQMS_fusion_splice_result "51"
template<>
void pqxx::internal::params::add_field(const fusion_splice_result & src);
pqxx::const_row_iterator pqxx2c(fusion_splice_result & dst, const pqxx::const_row_iterator & src);
