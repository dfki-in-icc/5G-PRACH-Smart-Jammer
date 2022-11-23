#include "gen_rnd_data.h"
#include "f1ap_types/srv_plmn.h"
#include "f1ap_types/freq_band.h"
#include "f1ap_types/nr_freq_info.h"
#include "f1ap_types/srv_cell_info.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <poll.h>

static
void gen_freq_band(freq_band_f1ap_t* dst)
{
  assert(dst != NULL);

  dst->frq_band = rand() % 1024; 

  dst->sz_sul_band = (rand() % 32) + 1;
  dst->sul_band = calloc(dst->sz_sul_band, sizeof(uint16_t) );
  assert(dst->sul_band != NULL);

  for(size_t i = 0; i < dst->sz_sul_band ; ++i){
    dst->sul_band[i] = rand() % 1025; 
  }

}

static
void gen_freq_info(nr_freq_info_f1ap_t* dst)
{
  time_t t;
  srand((unsigned) time(&t));

  dst->nr_arfcn = rand() % 3279165; // max. 3279165

  //  sul_info_t* sul_info; // optional;


  // Frequency band list
  dst->sz_frq_band = (rand() % 32) + 1;
  dst->frq_band = calloc(dst->sz_frq_band, sizeof(freq_band_f1ap_t) );
  assert(dst->frq_band != NULL);

  for(size_t i = 0; i < dst->sz_frq_band ; ++i){
    gen_freq_band(&dst->frq_band[i]);
  }
  //  freq_shift_t* freq_shift; // optional 
}

static
void gen_srv_cell_info(srv_cell_info_t* dst)
{
  assert(dst!= NULL);

  // NR CGI
  dst->nr_cgi.nr_cell_id = rand()% (1UL << 36);
  dst ->nr_cgi.plmn_id.mcc = 505;
  dst->nr_cgi.plmn_id.mnc = 1;
  dst->nr_cgi.plmn_id.mnc_digit_len = 2;

  // 2 NR PCI
  dst->nr_pci = rand() % 1008; 

  // 3 5GS TAC
  dst->five_gs_tac = malloc(sizeof(uint32_t ) );
  assert(dst->five_gs_tac != NULL);
  *dst->five_gs_tac = rand() % (1UL << 24); 

  // 4 Configured EPS TAC
  dst->eps_tac = malloc(sizeof(uint16_t));
  assert(dst->eps_tac != NULL);
  *dst->eps_tac = rand(); 

  // 5 Served PLMN
  dst->sz_srv_plmn = (rand() % 6) + 1;

  dst->srv_plmn = calloc(dst->sz_srv_plmn, sizeof(srv_plmn_f1ap_t));

  for(size_t i = 0; i < dst->sz_srv_plmn; ++i ){
    dst->srv_plmn[i].plmn_id.mcc = 202;
    dst->srv_plmn[i].plmn_id.mnc = 11;
    dst->srv_plmn[i].plmn_id.mnc_digit_len = 2;

    dst->srv_plmn[i].sz_tai_slice_sup_lst = rand()%1024;
    if(dst->srv_plmn[i].sz_tai_slice_sup_lst > 0 ){
      dst->srv_plmn[i].tai_slice_sup_lst = calloc(dst->srv_plmn[i].sz_tai_slice_sup_lst, sizeof(s_nssai_t));
    }

    for(size_t j =0; j < dst->srv_plmn[i].sz_tai_slice_sup_lst; ++j){
      s_nssai_t* tai_slice_sup = &dst->srv_plmn[i].tai_slice_sup_lst[j]; 
      tai_slice_sup->sst = rand(); 
      tai_slice_sup->sd= calloc(1, sizeof(byte_array_t));
      assert( tai_slice_sup->sd != NULL); 
      tai_slice_sup->sd->len = 3;
      tai_slice_sup->sd->buf = malloc(3);
      uint8_t tmp[3] = {0xde, 0xad, 0xbe};
      memcpy(tai_slice_sup->sd->buf, &tmp, 3); 
    }

    //  uint64_t* npn_sup_info_nid; // SIZE(44) bits 
  }

  // 6 CHOICE NR-Mode-Info
  dst->nr_mode_info = rand() % NR_MODE_INFO_F1AP_END;
  if(dst->nr_mode_info == FDD_NR_MODE_INFO_F1AP){

    gen_freq_info(&dst->fdd.ul_frq_info); 
    gen_freq_info(&dst->fdd.dl_frq_info); 

    dst->fdd.ul_bw.nr_scs = rand () % (  F1AP_NR_SCS_scs120 + 1);	
    dst->fdd.ul_bw.n_rb = rand () % ( F1AP_NR_NRB_nrb273 + 1);	

    dst->fdd.dl_bw.nr_scs = rand () % (F1AP_NR_SCS_scs120 + 1);	
    dst->fdd.dl_bw.n_rb = rand () % (F1AP_NR_NRB_nrb273	+ 1);	

    dst->fdd.sz_ul_nr_carrier_lst = 0; // rand() % 6; // max. 5
    
    if(dst->fdd.sz_ul_nr_carrier_lst > 0){
      dst->fdd.ul_nr_carrier_lst = calloc( dst->fdd.sz_ul_nr_carrier_lst, sizeof(nr_carrier_lst_f1ap_t) );
      assert(dst->fdd.ul_nr_carrier_lst != NULL);
    }

    for(int i = 0; i < dst->fdd.sz_ul_nr_carrier_lst; ++i){
//      dst->fdd.ul_nr_carrier_lst[i]. 
    }

  } else if(dst->nr_mode_info == TDD_NR_MODE_INFO_F1AP){

    gen_freq_info(&dst->tdd.freq_info); 

    dst->tdd.bw.nr_scs = rand () % (F1AP_NR_SCS_scs120 + 1);	
    dst->tdd.bw.n_rb = rand () % (F1AP_NR_NRB_nrb273	+ 1);	

  // optional
  //intended_tdd_dl_ul_conf_t* intended_tdd_dl_ul_conf;

  // optional
  //byte_array_t* tdd_ul_dl_conf_common;

  // optional
  //size_t sz_nr_carrier_lst;
  //nr_carrier_lst_f1ap_t* carrier_lst;

  }  else {
    assert( 0!=0 && "Unknown type");
  }
 
  // 7 Measurement Timing Configuration
  dst->meas_timing_conf.len = 4;
  dst->meas_timing_conf.buf = calloc(4, sizeof(uint8_t));
  assert(dst->meas_timing_conf.buf != NULL);
  uint8_t tmp[4] = {0xde, 0xad, 0xbe, 0xef};
  memcpy(dst->meas_timing_conf.buf, &tmp, 4);

  // 8 RANAC 
  //uint8_t* ran_ac;

  // 9 Extended Served PLMN
//  dst->sz_ext_srv_plmn = (rand() % 6) + 1;

//  dst->ext_srv_plmn = calloc(dst->sz_ext_srv_plmn, sizeof(srv_plmn_f1ap_t));
//  assert(dst->srv_plmn != NULL );

//  for(size_t i = 0; i < dst->sz_ext_srv_plmn; ++i){
//    dst->ext_srv_plmn[i].plmn_id.mcc = 222;
//    dst->ext_srv_plmn[i].plmn_id.mnc = 3;
//    dst->ext_srv_plmn[i].plmn_id.mnc_digit_len = 1;

  //optional
  //size_t sz_tai_slice_sup_lst;
  //s_nssai_t* tai_slice_sup_lst;

  //optional
  //uint64_t* npn_sup_info_nid; // SIZE(44) bits 

  //optional
  //size_t sz_ext_slice_sup_lst;
  //s_nssai_t* ext_tai_slice_sup_lst;

//  }

  // 10 Cell Direction
//  e_F1AP_Cell_Direction* cell_dir;

  // 11 Cell Type
//  e_F1AP_CellSize* cell_type;

  // 12 Broadcast PLMN Identity Info
//  size_t sz_brdcst_PLMN_id_info;
//  brdcst_PLMN_id_info_f1ap_t* brdcst_PLMN_id_info; 

  // 13 Configured TAC Indication
  //e_F1AP_ConfiguredTACIndication* conf_tac_ind; // enum TRUE

  // 14 Aggressor gNB Set ID
  //byte_array_t* aggr_gnb_set_id;   

  // 15 Victim gNB Set ID
  //byte_array_t* victim_gnb_set_id;

  // 16 AB Info IAB-DU
  //iab_info_iab_du_f1ap_t* ab_info_iab_du;

  // 17 SSB Positions In Burst
  //ssb_pos_burst_t* ssb_pos_burst;

  // 18 NR PRACH Configuration
  //nr_prach_conf_t* nr_prach_conf; 
  
  // 19 SFN Offset
  //byte_array_t* sfn_offset;

}

f1_setup_t gen_rnd_f1_setup(void)
{
  f1_setup_t dst = {0}; 

  // Transaction ID. Mandatory
  dst.trans_id = rand();

  // gNB-DU ID
  dst.gnb_du_id = rand() % (1UL << 33);

  // Name. Optional
  const char* name = "F1 Setup msg !!!";
  dst.gnb_du_name = malloc(sizeof(byte_array_t));
  assert(dst.gnb_du_name != NULL && "Memory exhausted");
  dst.gnb_du_name->buf = malloc(sizeof(uint8_t)* strlen(name)  ); 
  memcpy(dst.gnb_du_name->buf, name, strlen(name));
  dst.gnb_du_name->len = strlen(name);

  dst.sz_srv_cell = (rand() % 512) + 1;

  dst.srv_cell = calloc(dst.sz_srv_cell, sizeof(gnb_du_srv_cell_f1ap_t) );
  assert(dst.srv_cell != NULL);

  for(size_t i = 0; i <dst.sz_srv_cell; ++i){
    gen_srv_cell_info(&dst.srv_cell[i].srv_cell_info);
    time_t t;
    srand((unsigned) time(&t));
    poll(NULL, 0, i%3);
  }

  // gNB-DU RRC version
  // This IE is not used in this release. pag. 197.   9.3.1.70
  dst.ver.latest_rrc_version.bits_unused = 0;
  const uint8_t version[1] = {0xa0};
  dst.ver.latest_rrc_version.buf = calloc(1, 1);
  assert(dst.ver.latest_rrc_version.buf  != NULL);
  memcpy(dst.ver.latest_rrc_version.buf, version, 1);
  dst.ver.latest_rrc_version.size = 1;
  dst.ver.latest_rrc_version.bits_unused = 8 - 3; // Only 3 bits used

  // Transport Layer Address Info. Optional
  dst.tlai = NULL;

  // BAP Address. Optional
  dst.bad_add = NULL;

  // Extended gNB-DU Name. Optional
  dst.egdn = NULL;
 
  return dst;
}

static
void gen_rnd_act_cell(activate_cell_t* dst)
{
  assert(dst != NULL);

  // NR CGI
  dst->nr_cgi.nr_cell_id = rand()% (1UL << 36);
  dst ->nr_cgi.plmn_id.mcc = 505;
  dst->nr_cgi.plmn_id.mnc = 1;
  dst->nr_cgi.plmn_id.mnc_digit_len = 2;

  // NR PCI. Optional
  dst->nr_pci = calloc(1, sizeof(uint16_t)); 
  assert(dst->nr_pci != NULL && "Memory exhausted");
  *dst->nr_pci = rand() % 1008; 

  // gNB-CU System Information. Optional
  dst->sys_info = calloc(1, sizeof( gnb_cu_sys_info_t ) );  
  assert(dst->sys_info != NULL && "Memory exhausted" );

  dst->sys_info->len = (rand() % 32) + 1; 
  if(dst->sys_info->len > 0){
    dst->sys_info->sib = calloc(dst->sys_info->len, sizeof(sib_t));
    assert(dst->sys_info->sib != NULL && "Memory exhausted");
  }

  for(size_t i = 0; i < dst->sys_info->len; ++i){

    // mandatory
    dst->sys_info->sib[i].type = (rand() % 3 ) + 2 ;

    // mandatory
    const char* dummy_str = "Dummy string";
    dst->sys_info->sib[i].msg.buf = calloc(1, strlen(dummy_str));
    memcpy(dst->sys_info->sib[i].msg.buf, dummy_str, strlen( dummy_str ) );
    dst->sys_info->sib[i].msg.len = strlen(dummy_str);

    // mandatory
    dst->sys_info->sib[i].tag = rand() % 31;

    // optional
    dst->sys_info->sib[i].area_scope = NULL;
  }

  dst->sys_info->id = NULL;

  // Available PLMN List. Optional
  dst->sz_avail_plmn_lst = 0;
  dst->avail_plmn = NULL;

  // Extended Available PLMN List. Optional
  dst->sz_ext_avail_plmn_lst = 0; 
  dst->ext_avail_plmn = NULL;

  // IAB Info IAB-donor-CU. Optional
  dst->ab_info = NULL;

  // Available SNPN ID List
  dst->sz_av_snpn_id_lst = 0;
  dst->av_snpn = NULL;

}

f1_setup_response_t gen_rnd_f1_setup_response(void)
{
  f1_setup_response_t dst = {0}; 

  // Transaction ID. Mandatory
  dst.trans_id = rand() % 256;

  // CU name. Optional
  dst.gnb_cu_name = calloc(1, sizeof(byte_array_t));
  assert(dst.gnb_cu_name != NULL && "Memory exhasuted");
  const char* cu_name = "I can't wait for the weekend to begin";

  dst.gnb_cu_name->buf = malloc(strlen(cu_name)); 
  memcpy(dst.gnb_cu_name->buf, cu_name, strlen(cu_name) );
  dst.gnb_cu_name->len = strlen(cu_name);

  // Cells to be Activated List. Optional
  dst.sz_act_cell = rand() % 512; 

  if(dst.sz_act_cell > 0){
    dst.act_cell = calloc(dst.sz_act_cell, sizeof( activate_cell_t) );
    assert(dst.act_cell != NULL && "Memory exhausted");
  }

  for(size_t i = 0; i < dst.sz_act_cell; ++i){
    gen_rnd_act_cell(&dst.act_cell[i]);
  }

  // gNB-CU RRC version. Mandatory
  // This IE is not used in this release. pag. 197. 9.3.1.70
  dst.ver.latest_rrc_version.bits_unused = 0;
  const uint8_t version[1] = {0xa0};
  dst.ver.latest_rrc_version.buf = calloc(1, 1);
  assert(dst.ver.latest_rrc_version.buf  != NULL);
  memcpy(dst.ver.latest_rrc_version.buf, version, 1);
  dst.ver.latest_rrc_version.size = 1;
  dst.ver.latest_rrc_version.bits_unused = 8 - 3; // Only 3 bits used

  // Transport Layer Address Info. Optional
  dst.tlai = NULL;

  // Uplink BH Non-UP Traffic Mapping. Optional
  dst.ubh_nup_traff = NULL;

  // BAP Address. Optional
  dst.bad_add = NULL;

  // Extended gNB-CU Name
  dst.egcn = NULL;

  return dst;
}

static
void gen_rnd_criticallity_diagnostic( criticallity_diagnostic_f1ap_t* crit)
{
  assert(crit != NULL);

  //optional
  //Procedure Code
  crit->proc_code = malloc(sizeof(uint8_t));
  assert(crit->proc_code != NULL && "Memory exhausted" );
  *crit->proc_code = rand(); 

 //optional
  //Triggering Message
  crit->trig_msg = malloc(sizeof(trig_msg_e));
  assert(crit->trig_msg != NULL && "Memory exhausted" );
  *crit->trig_msg = rand() % END_TRIGGERING_MESSAGE_F1AP ; 

  //optional
  //Procedure Criticality
  crit->proc_crit = malloc(sizeof(proc_crit_e));
  assert(crit->proc_crit != NULL && "Memory exhausted" );
  *crit->proc_crit = rand() % END_PROCEDURE_CRITICALLITY_F1AP; 

  //optional
  //Transaction ID
  crit->trans_id = malloc(sizeof(uint8_t) );
  assert(crit->trans_id != NULL && "Memory exhausted");
  *crit->trans_id = rand(); 

  crit->sz_crit_diagn_ie = rand() % 257;
  if(crit->sz_crit_diagn_ie > 0){
    crit->crit_diagn_ie = calloc(crit->sz_crit_diagn_ie, sizeof(crit_diagn_ie_t));
    assert(crit->crit_diagn_ie != NULL && "Memory exhausted");
  }

  for(size_t i =0; i < crit->sz_crit_diagn_ie; ++i){
    crit_diagn_ie_t* ie = &crit->crit_diagn_ie[i];
    ie->proc_crit = rand() % END_PROCEDURE_CRITICALLITY_F1AP ;
    ie->id = rand();
    ie->type_of_error = rand() % END_CRITICALLITY_DIAGNOSTIC_IE_F1AP ;
  }

}

f1_setup_failure_t gen_rnd_f1_setup_failure(void)
{
  f1_setup_failure_t dst = {0}; 

  // Mandatory
  //Transaction ID
  dst.trans_id = rand() % 256;

  //Mandatory
  //Cause
  dst.cause.type = rand() % END_CAUSE_F1AP;

  if(dst.cause.type == RADIO_NETWORK_CAUSE_F1AP){
    dst.cause.radio = rand() % END_RADIO_NETWORK_CAUSE_F1AP ;
  } else if(dst.cause.type == TRANSPORT_CAUSE_F1AP){
    dst.cause.trans = rand() % END_TRANSPORT_CAUSE_F1AP ;
  } else if(dst.cause.type == PROTOCOL_CAUSE_F1AP){
    dst.cause.proto = rand() % END_PROTOCOL_CAUSE_F1AP ;
  } else if(dst.cause.type == MISC_CAUSE_F1AP){
    dst.cause.misc =  rand() % END_MISC_CAUSE_F1AP;
  } else {
    assert(0!=0 && "Unknwon cause type" );
  }

  // Optional
  //Time to wait
  dst.time_to_wait =  malloc(sizeof(time_to_wait_f1ap_e));
  assert(dst.time_to_wait != NULL && "Memory exhausted");
  *dst.time_to_wait = rand() % END_TIME_TO_WAIT_F1AP; 

  //Optional
  //Criticality Diagnostics
  dst.diagnose = malloc(sizeof(criticallity_diagnostic_f1ap_t));
  assert(dst.diagnose != NULL && "Mmeory exhausted");
  gen_rnd_criticallity_diagnostic(dst.diagnose); 

  return dst;
}

