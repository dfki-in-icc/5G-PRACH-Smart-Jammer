set(f1ap_src_types

  activate_cell.c
  brdcst_PLMN_id_info.c
  criticallity_diagnostic_f1ap.c
  criticallity_diagnostic_ie_f1ap.c
  cause_f1ap.c
  f1_setup.c
  f1_setup_response.c
  f1_setup_failure.c
  fdd_info.c
  freq_band.c
  freq_shift.c
  gnb_rrc_ver.c
  gnb_du_srv_cell.c
  gnb_du_sys_info.c
  gnb_cu_sys_info.c
  iab_info_iab_du.c
  intented_tdd_dl_ul_conf.c
  nr_carrier_lst.c
  nr_freq_info.c
  nr_cgi.c
  nr_prach_conf.c
  plmn.c
  s_nssai.c
  srv_cell_info.c
  sib.c
  srv_plmn.c
  ssb_pos_burst.c
  sul_info.c
  time_to_wait_f1ap.c
  tdd_info.c
  trans_bw.c
  )

add_library(f1ap_types_obj OBJECT ${f1ap_src_types}) 



