#pragma once

#include <pqxx/tuple>
#include <pqxx/prepared_statement>

#include "jmsg_types.hpp"

/// pq key list for fs_param_cfg
#define PQKL_fs_param_cfg "fusion_mode,lfti,rfti,align_mode,x_focus,y_focus,ecf_redress,auto_mag,vangle_limit,hangle_limit,clr_mag,clr_time,clr_pos,position,gap,overlap,pre_mag,pre_time,arc1_mag,arc1_time,arc2_mag,arc2_time,arc2_on_time,arc2_off_time,arc_man_time,lft_push_speed,rt_push_speed,taper_splice,taper_wait_time,taper_length,taper_speed,tense_test,tense_speed,tense_length,loss_mode,loss_limit,loss_min,lft_mfd,rt_mfd,syn_bend_co,opp_bend_co,mfd_mis_co"
/// pq occupy symbol list for fs_param_cfg
#define PQOL_fs_param_cfg "$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33,$34,$35,$36,$37,$38,$39,$40,$41,$42"
template<>
pqxx::prepare::invocation & pqxx::prepare::invocation::operator()(const fs_param_cfg & src);
void pqxx2c(fs_param_cfg & dst, pqxx::const_tuple_iterator & src);
