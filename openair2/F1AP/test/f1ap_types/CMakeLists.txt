set(f1ap_src_types
  activate_cell.c
  add_pdcp_dup_tnl.c
  alloc_retention_prio.c

  brdcst_PLMN_id_info.c
  cause_f1ap.c

  cells_to_be_barred.c

  criticallity_diagnostic_f1ap.c
  criticallity_diagnostic_ie_f1ap.c
  cp_trans_layer_info.c
  cu_to_du_rrc_info.c

  dl_up_tnl_info_tbs.c
  drx_cycle.c

  drb_info.c
  drb_setup_item.c
  drb_to_be_setup.c

  dyn_5qi_descriptor.c
  du_to_cu_rrc_information_f1ap.c

  e_utran_qos.c
  endpoint_ip_addr.c
  endpoint_ip_addr_port.c
  eutra_cell.c

  f1_setup.c
  f1_setup_response.c
  f1_setup_failure.c
  fdd_info.c
  freq_band.c
  freq_shift.c

  flows_mapped_to_drb.c

  gbr_qos_info.c
  gnb_rrc_ver.c
  gnb_du_srv_cell.c
  gnb_du_sys_info.c
  gnb_cu_sys_info.c
  gnb_cu_conf_update.c
  gnb_cu_tnl_assoc_to_add.c
  gnb_cu_tnl_assoc_to_rem.c
  gnb_cu_tnl_assoc_to_upd.c
  gbr_qos_flow_info.c
  iab_info_iab_du.c
  intented_tdd_dl_ul_conf.c

  neighbour_cell_info.c
  nr_carrier_lst.c
  nr_freq_info.c
  nr_cgi.c
  nr_prach_conf.c
  non_dyn_5qi_descriptor.c

  prot_eutra_resources.c
  plmn.c

  qos_flow_level_qos_parameter.c

  s_nssai.c
  scell_to_be_setup.c

  serv_eutra_cell_info.c
  srv_cell_info.c
  sib.c
  srv_plmn.c
  srb_to_be_setup.c
  srb_setup_item.c
  ssb_pos_burst.c
  sul_info.c

  tsc_traffic_char.c
  tsc_assistance_info.c
  time_to_wait_f1ap.c
  tdd_info.c
  trans_bw.c
  ue_ctx_setup_request.c

  ue_ctx_setup_response.c

  ul_up_tnl_info.c
  ul_up_tnl_info_lst.c

  )

add_library(f1ap_types_obj OBJECT ${f1ap_src_types}) 



