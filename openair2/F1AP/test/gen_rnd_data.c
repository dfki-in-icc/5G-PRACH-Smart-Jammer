#include "gen_rnd_data.h"
#include "f1ap_types/srv_plmn.h"
#include "f1ap_types/freq_band.h"
#include "f1ap_types/nr_freq_info.h"
#include "f1ap_types/srv_cell_info.h"
#include "f1ap_types/dl_up_tnl_info_tbs.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <poll.h>

static
BIT_STRING_t gen_bit_string(const char* str)
{
  BIT_STRING_t dst = {0};

  dst.size = strlen(str);
  dst.buf = calloc(1, strlen(str));
  assert(dst.buf != NULL);

  memcpy(dst.buf, str, strlen(str) );
  dst.bits_unused = 0;

  return dst;
}

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
s_nssai_t gen_rnd_s_nssai(void)
{
  s_nssai_t dst = {0};

  dst.sst = rand(); 
  
  /*
  dst.sd= calloc(1, sizeof(byte_array_t));
  assert(dst.sd != NULL); 
  dst.sd->len = 3;
  dst.sd->buf = malloc(3);
  uint8_t tmp[3] = {0xde, 0xad, 0xbe};
  memcpy(dst.sd->buf, &tmp, 3); 
  */

  return dst;
}


static
void gen_srv_cell_info(srv_cell_info_t* dst)
{
  assert(dst!= NULL);

  // NR CGI
  dst->nr_cgi.nr_cell_id = rand()% (1UL << 36);
  dst->nr_cgi.plmn_id.mcc = 505;
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
      dst->srv_plmn[i].tai_slice_sup_lst[j] = gen_rnd_s_nssai(); 
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
activate_cell_t gen_rnd_act_cell()
{
  activate_cell_t dst = {0};

  // NR CGI
  dst.nr_cgi.nr_cell_id = rand()% (1UL << 36);
  dst.nr_cgi.plmn_id.mcc = 505;
  dst.nr_cgi.plmn_id.mnc = 1;
  dst.nr_cgi.plmn_id.mnc_digit_len = 2;

  // NR PCI. Optional
  dst.nr_pci = calloc(1, sizeof(uint16_t)); 
  assert(dst.nr_pci != NULL && "Memory exhausted");
  *dst.nr_pci = rand() % 1008; 

  // gNB-CU System Information. Optional
  dst.sys_info = calloc(1, sizeof( gnb_cu_sys_info_t ) );  
  assert(dst.sys_info != NULL && "Memory exhausted" );

  dst.sys_info->sz_sib = 1; //(rand() % 32) + 1; 
  if(dst.sys_info->sz_sib > 0){
    dst.sys_info->sib = calloc(dst.sys_info->sz_sib, sizeof(sib_t));
    assert(dst.sys_info->sib != NULL && "Memory exhausted");
  }

  for(size_t i = 0; i < dst.sys_info->sz_sib; ++i){
    // mandatory
    dst.sys_info->sib[i].type = (rand() % 3 ) + 2 ;

    // mandatory
    const char* dummy_str = "string";
    dst.sys_info->sib[i].msg.buf = calloc(1, strlen(dummy_str));
    memcpy(dst.sys_info->sib[i].msg.buf, dummy_str, strlen( dummy_str ) );
    dst.sys_info->sib[i].msg.len = strlen(dummy_str);

    // mandatory
    dst.sys_info->sib[i].tag = rand() % 31;

    // optional
    dst.sys_info->sib[i].area_scope = NULL;
  }

  dst.sys_info->id = NULL;

  // Available PLMN List. Optional
  dst.sz_avail_plmn_lst = 0;
  dst.avail_plmn = NULL;

  // Extended Available PLMN List. Optional
  dst.sz_ext_avail_plmn_lst = 0; 
  dst.ext_avail_plmn = NULL;

  // IAB Info IAB-donor-CU. Optional
  dst.ab_info = NULL;

  // Available SNPN ID List
  dst.sz_av_snpn_id_lst = 0;
  dst.av_snpn = NULL;


  return dst;
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
   dst.act_cell[i] = gen_rnd_act_cell();
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

static
byte_array_t gen_byte_array(const char* str)
{
  assert(str != NULL);

  byte_array_t dst = {0};

  size_t const sz = strlen(str);
  dst.buf = malloc(sz);
  assert(dst.buf != NULL && "Memory exhausted");

  dst.len = sz;
  memcpy(dst.buf, str, sz);

  return dst;
}

static
byte_array_t* gen_byte_array_heap(const char* str)
{
  assert(str != NULL);

  byte_array_t* dst = calloc(1, sizeof(byte_array_t));
  assert(dst != NULL && "Memory exhausted");
  *dst = gen_byte_array(str);

  return dst;
}

static
cu_to_du_rrc_info_t gen_cu_to_du_rrc_info(void)
{
  cu_to_du_rrc_info_t dst = {0}; 

  // Optional
  // CG-ConfigInfo
  const char* str_1 = "String One";
  dst.cg_config_info = gen_byte_array_heap(str_1);

  // Optional
  // UE-CapabilityRAT-ContainerList
  const char* str_2 = "String Two";
  dst.ue_capability_rat_con_lst = gen_byte_array_heap(str_2) ;

  // Optional
  // MeasConfig
  const char* str_3 = "String Three";
  dst.meas_config =  gen_byte_array_heap(str_3) ;

  // Optional
  // Handover Preparation Information
//  const char* str_4 = "String Four";
//  dst.handover_prep_info = gen_byte_array_heap(str_4);

  // Optional
  // CellGroupConfig
//  const char* str_5 = "String Five";
//  dst.cell_group_config = gen_byte_array_heap(str_5);

  // Optional
  // Measurement Timing Configuration
//  const char* str_6 = "String Six";
//  dst.meas_timing_conf = gen_byte_array_heap(str_6);

  // Optional
  // UEAssistanceInformation
//  const char* str_7 = "String Seven";
//  dst.ue_assistance_info = gen_byte_array_heap(str_7);

  // Optional
  // CG-Config
//  const char* str_8 = "String Eight";
//  dst.cg_config = gen_byte_array_heap(str_8);

  // Optional
  // UEAssistanceInformationEUTRA
//  const char* str_9 = "String Nine";
//  dst.UE_assistance_info_eutra = gen_byte_array_heap(str_9);

  return dst;
}

static
nr_cgi_t gen_rnd_nr_cgi(void)
{
  nr_cgi_t dst = {0}; 
  dst.nr_cell_id = rand()% (1UL << 36);
  dst.plmn_id.mcc = 505;
  dst.plmn_id.mnc = 1;
  dst.plmn_id.mnc_digit_len = 2;

  return dst;
}

static
drx_cycle_t* gen_rnd_drx_cycle(void)
{
  drx_cycle_t* dst = malloc(sizeof(drx_cycle_t));
  assert(dst != NULL && "Memory exhausted");

  // Mandatory 
  // Long DRX Cycle Length
  //long_drx_cycle_len_e long_drx_cycle_len;
  dst->long_drx_cycle_len = rand() % END_LONG_DRX_CYCLE_LEN;

  // Optional 
  // Short DRX Cycle Length
  //short_drx_cycle_len_e* short_drx_cycle_len;
  dst->short_drx_cycle_len = NULL;

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  dst->short_drx_cycle_timer = NULL;

  return dst;
}

static
scell_to_be_setup_t gen_rnd_scell_to_be_setup(void)
{
  scell_to_be_setup_t dst = {0}; 

  // Mandatory
  // SCell ID 
  dst.scell_id = gen_rnd_nr_cgi();

  // Mandatory
  // SCellIndex
  dst.scell_idx = (rand() % 31) + 1; // INTEGER (1..31

  // Optional
  // SCell UL Configured
  dst.scell_ul_conf = NULL;

  // Optional
  // servingCellMO
  dst.serv_cell_mo = NULL; // INTEGER(1..64) 

  return dst;
}

static
srb_to_be_setup_t gen_rnd_srb_to_be_setup(void)
{
  srb_to_be_setup_t dst = {0};

  // Mandatory
  //  SRB ID
  dst.srb_id = rand() % 4; // [0,3]

  // Optional
  // Duplication Indication
  dst.dup_ind = NULL;  

  // Optional 
  // Additional Duplication Indication
  dst.add_dup_ind = NULL;

  return dst;
}

static
e_utran_qos_t gen_rnd_e_utran_qos(void)
{
  e_utran_qos_t dst = {0};
  // Mandatory
  // QCI
  dst.qci = rand();

  // Mandatory
  // Allocation and Retention Priority alloc_retention_prio_t 
  dst.alloc.preempt_capability = rand() % END_PREEMPT_CAPABILITY; 
  dst.alloc.preempt_vulnerability = rand() % END_PREEMPT_VULNERABILITY;
  dst.alloc.prio_level = rand() % 16; //  // INTEGER (0..15)

  // Optional
  // GBR QoS Information 9.3.1.21
  dst.gbr_qos_info = NULL; 

  return dst;
}

static
non_dyn_5qi_descriptor_t gen_rnd_non_dyn_5qi_descriptor(void)
{
  // Non Dynamic 5QI Descriptor 9.3.1.49
  non_dyn_5qi_descriptor_t dst = {0};

  // Mandatory
  //  5QI
  dst.five_qi = rand();

  // Optional
  // Priority Level
  dst.prio_level = NULL; // [1,127]

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  dst.av_window = NULL;

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  dst.max_data_burst_vol = NULL; // INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  dst.pkt_delay_budget_downlink = NULL; // INTEGER (0..65535, …)

  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  dst.pkt_delay_budget_uplink = NULL; // INTEGER (0..65535, …)

  return dst;
}

static
dyn_5qi_descriptor_t gen_rnd_dyn_5qi_descriptor(void)
{
  // Dynamic 5QI Descriptor 9.3.1.47
  dyn_5qi_descriptor_t dst = {0};

  //Mandatory
  //QoS Priority Level
  dst.qos_prio_level = (rand() % 127) + 1; // INTEGER (1..127)

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  dst.pkt_delay_budget = rand() % 1024; // INTEGER (0..1023, ...)

  // Mandatory
  // Packet Error Rate 9.3.1.52
  dst.pkt_error_rate.scalar = rand() % 10;   // INTEGER (0..9, ...)
  dst.pkt_error_rate.exponent = rand() % 10; // INTEGER (0..9, ...)

  // Optional
  // 5QI
  dst.five_qi = NULL;

  // C-ifGBRflow
  // Delay Critical 
  dst.delay_critical = NULL; //rand() % END_DELAY_CRITICAL;

  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  dst.av_window = NULL; //rand() % 4096; // INTEGER (0..4095, ...)

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  dst.max_data_burst_vol = NULL; //INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  dst.ext_pkt_delay_budget = NULL; //INTEGER (0..65535, …) 

  // Optional 
  // CN Packet Delay Budget Downlink
  dst.cn_pkt_dalay_budget_downlink = NULL; 

  // Optional
  // CN Packet Delay Budget Uplink
  dst.cn_pkt_dalay_budget_uplink = NULL; 

  return dst;
}

static
gbr_qos_flow_info_t* gen_rnd_gbr_qos_flow_info(void)
{
  // GBR QoS Flow Information 9.3.1.46
  gbr_qos_flow_info_t* dst = calloc(1, sizeof(gbr_qos_flow_info_t));

  // Mandatory
  // Maximum Flow Bit Rate Downlink 9.3.1.22
  dst->max_flow_bit_rate_downlink = rand(); //INTEGER (0..4,000,000,000,000,...) rand() return int which only has 32 bits
                                       
  // Mandatory
  // Maximum Flow Bit Rate Uplink 9.3.1.22
  dst->max_flow_bit_rate_uplink = rand(); //INTEGER (0..4,000,000,000,000,...) rand() return int which only has 32 bits

  // Mandatory
  // Guaranteed Flow Bit Rate Downlink 9.3.1.22
  dst->gbr_flow_bit_rate_downlink = rand(); //INTEGER (0..4,000,000,000,000,...) rand() return int which only has 32 bits

  //Mandatory
  //Guaranteed Flow Bit Rate Uplink 9.3.1.22
  dst->gbr_flow_bit_rate_uplink = rand(); //INTEGER (0..4,000,000,000,000,...) rand() return int which only has 32 bits

  //Optional
  //Maximum Packet Loss Rate Downlink 9.3.1.50
  dst->max_pkt_loss_rate_downlink = NULL; //INTEGER(0..1000)

  //Optional
  //Maximum Packet Loss Rate Uplink 9.3.1.50
  dst->max_pkt_loss_rate_uplink = NULL; //INTEGER(0..1000)
                                       
  //Optional 
  //Alternative QoS Parameters Set List 9.3.1.50
  dst->alternative_qos_param = NULL; //INTEGER(0..1000)

  return dst;
}

static
qos_flow_level_qos_parameter_t gen_qos_flow_level_qos_parameter(void)
{
  qos_flow_level_qos_parameter_t dst = {0};

  //Mandatory 
  // CHOICE QoS Characteristics qos_characteristic_e  
  dst.qos_characteristic = rand() % END_5QI_DESCRIPTOR;
  assert(dst.qos_characteristic == NON_DYN_5QI_DESCRIPTOR ||  dst.qos_characteristic == DYN_5QI_DESCRIPTOR);

  if(dst.qos_characteristic == NON_DYN_5QI_DESCRIPTOR){
    dst.non_dyn = gen_rnd_non_dyn_5qi_descriptor() ;
  } else if (dst.qos_characteristic == DYN_5QI_DESCRIPTOR){ // dst.qos_characteristic ==  
    dst.dyn = gen_rnd_dyn_5qi_descriptor();
  } else {
    assert(0!=0 && "Unknown type");
  }

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
  dst.alloc_retention_prio.preempt_capability = rand() % END_PREEMPT_CAPABILITY; 
  dst.alloc_retention_prio.preempt_vulnerability = rand() % END_PREEMPT_VULNERABILITY;
  dst.alloc_retention_prio.prio_level = rand() % 16; //  // INTEGER (0..15)

  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  dst.gbr_qos_flow_info = NULL; // gen_rnd_gbr_qos_flow_info();

  // Optional 
  // Reflective QoS Attribute
  dst.reflective_qos_attr = NULL;

  // Optional 
  // PDU Session ID
  dst.pdu_session_id = NULL;

  // Optional
  //UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  dst.ul_pdu_session_aggr_max_bit_rate = NULL; 

  // Optional
  // QoS Monitoring Request
   dst.qos_monitoring_request = NULL;

  return dst;
}

static
flows_mapped_to_drb_t gen_flows_mapped_to_drb(void)
{
  flows_mapped_to_drb_t dst = {0}; 
  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  dst.qfi = rand() % 64; // INTEGER (0 ..63)

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  dst.qos_flow_level = gen_qos_flow_level_qos_parameter();

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  dst.qos_flow_mapping_ind = NULL;

  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  dst.tsc_traffic_char = NULL;

  return dst;
}

static
drb_info_t gen_rnd_drb_info(void)
{
  drb_info_t dst = {0}; 
  
  // Mandatory
  // DRB QoS 9.3.1.45
  dst.drb_qos = gen_qos_flow_level_qos_parameter();
   
  // Mandatory
  // S-NSSAI 9.3.1.38
  dst.nssai = gen_rnd_s_nssai();

  // Optional
  // Notification Control 9.3.1.56 notification_ctrl_e
  dst.notification_ctrl = NULL;

  // Flows Mapped to DRB [1,64]
  dst.sz_flows_mapped_to_drb = (rand() % 64) + 1;
  if(dst.sz_flows_mapped_to_drb > 0){
    dst.flows_mapped_to_drb = calloc(dst.sz_flows_mapped_to_drb, sizeof(flows_mapped_to_drb_t));
    assert(dst.flows_mapped_to_drb != NULL && "Memory exhausted");
  }

  for(size_t i = 0; i < dst.sz_flows_mapped_to_drb; ++i){
    dst.flows_mapped_to_drb[i] = gen_flows_mapped_to_drb();
  }

  return dst;
}

static
ul_up_tnl_info_t gen_rnd_ul_up_tnl_info(void)
{
  ul_up_tnl_info_t dst = {0};

  // Mandatory
  // Transport Layer Address
  const char* str = "192.168.1.1";
  dst.tla = gen_bit_string(str); 

  // Mandatory
  // 9.3.2.2
  const char str_gtp[4] =  "ADAD";
  memcpy(dst.gtp_teid, str_gtp, 4);

  return dst;
}

static
ul_up_tnl_info_lst_t gen_rnd_ul_up_tnl_info_lst(void)
{
  ul_up_tnl_info_lst_t dst = {0}; 

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  dst.tnl_info = gen_rnd_ul_up_tnl_info();

  // Optional
  // BH Information 9.3.1.114
  dst.bh_info = NULL; 

  return dst;
}

static
drb_to_be_setup_t gen_drb_to_be_setup(void)
{
  drb_to_be_setup_t dst = {0}; 

  // Mandatory
  // DRB ID 9.3.1.8
  dst.drb_id = rand() % 33; // INTEGER [0,32]

  dst.qos_info = rand() % END_QOS_INFORMATION;
  assert(dst.qos_info == E_UTRAN_QOS_INFORMATION || dst.qos_info == DRB_INFO_QOS_INFORMATION);
  if(dst.qos_info == E_UTRAN_QOS_INFORMATION ){
    dst.e_utran_qos = gen_rnd_e_utran_qos();
  } else { // dst.qos_info == DRB_INFO_QOS_INFORMATION
    dst.drb_info = gen_rnd_drb_info();
  }

  
  // UL UP TNL Information to be setup List
  dst.sz_ul_up_tnl_info_lst = (rand() % 2) + 1; // [1, 2]
  if(dst.sz_ul_up_tnl_info_lst > 0){
    dst.ul_up_tnl_info = calloc(dst.sz_ul_up_tnl_info_lst, sizeof(ul_up_tnl_info_lst_t));
    assert(dst.ul_up_tnl_info != NULL && "Memory exhausted");
  }
  for(size_t i = 0; i < dst.sz_ul_up_tnl_info_lst; ++i){
    dst.ul_up_tnl_info[i] = gen_rnd_ul_up_tnl_info_lst(); 
  }

  // Mandatory
  // RLC Mode 9.3.1.27
  dst.rlc_mode = rand() % END_RLC_MODE_F1AP;

  // Optional 
  // UL Configuration 9.3.1.31
  dst.ul_conf = NULL;

  // Optional 
  // Duplication Activation 9.3.1.36
  dst.dup_act = NULL;

  // Optional
  // DC Based Duplication Configured
  dst.dc_dup_act_conf = NULL;

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  dst.dc_dup_act = NULL;

  // Mandatory
  // DL PDCP SN length
  dst.dl_pdcp_sn_len = rand() % END_PDCP_SN_LEN;

  // Optional
  // UL PDCP SN length
  dst.ul_pdcp_sn_len = NULL;

  // Additional PDCP Duplication TNL List
  dst.sz_add_pdcp_dup_tnl = 0; // [0,2]
  dst.add_pdcp_dup_tnl = NULL; 

  // Optional
  // RLC Duplication Information 9.3.1.146
  dst.sz_rlc_dup_info = 0; // [0,3]
  dst.rlc_dup_info = NULL;  

  return dst;
}

ue_ctx_setup_request_t gen_rnd_ue_ctx_setup_request(void)
{
  ue_ctx_setup_request_t  dst = {0};

  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  dst.gnb_cu_ue_f1ap_id = rand();

  // Optional
  // gNB-DU UE F1AP ID 9.3.1.5
  dst.gnb_du_ue_f1ap_id = malloc(sizeof(uint32_t));
  *dst.gnb_du_ue_f1ap_id = rand(); 

  // Mandatory
  // SpCell ID nr_cgi_t 
  dst.sp_cell_id = gen_rnd_nr_cgi(); 

     // Mandatory
  // ServCellIndex
  dst.serv_cell_idx = rand() % 32; // INTEGER (0..31,...) 

  // Optional
  // SpCell UL Configured 9.3.1.33
  dst.sp_cell_ul_conf = NULL;

  // Mandatory
  // CU to DU RRC Information 9.3.1.25
  dst.cu_to_du_rrc_info = gen_cu_to_du_rrc_info() ;  // cu_to_du_rrc_info_t 

  // Optional 
  // Candidate SpCell ID
  dst.sz_candidate_sp_cell_id = 1; //rand()  % 65; // max. 64
  if(dst.sz_candidate_sp_cell_id > 0){
    dst.candidate_sp_cell_id = calloc(dst.sz_candidate_sp_cell_id, sizeof(nr_cgi_t)); 
    assert(dst.candidate_sp_cell_id != NULL && "Memory exhausted");
  }                                           
  for(size_t i = 0 ; i < dst.sz_candidate_sp_cell_id; ++i){
    dst.candidate_sp_cell_id[i] = gen_rnd_nr_cgi();  
  }

  //Optional 
  // DRX Cycle
  dst.drx_cycle = gen_rnd_drx_cycle();

  // Optional
  // Resource Coordination Transfer Container
  const char* str_resource_coord = "String for coordination";
  dst.resource_coord_transfer_container = gen_byte_array_heap(str_resource_coord); 
  // SCell To Be Setup List
  dst.sz_scell_to_be_setup = rand() % 33;// [0,32]
  if(dst.sz_scell_to_be_setup > 0){
    dst.scell_to_be_setup = calloc(dst.sz_scell_to_be_setup, sizeof(scell_to_be_setup_t )  );
    assert(dst.scell_to_be_setup != NULL && "Memory exhausted");
  }
  for(size_t i = 0; i < dst.sz_scell_to_be_setup; ++i){
    dst.scell_to_be_setup[i] = gen_rnd_scell_to_be_setup();  
  }

  // SRB to Be Setup Item
  dst.sz_srb_to_be_setup = 8; // rand() % 9; //  [0,8]
  if(dst.sz_srb_to_be_setup > 0) {
    dst. srb_to_be_setup = calloc(dst.sz_srb_to_be_setup, sizeof(srb_to_be_setup_t)); 
    assert(dst. srb_to_be_setup != NULL && "Memory exhausted");
  }
  for(size_t i =0; i < dst.sz_srb_to_be_setup; ++i ) {
    dst.srb_to_be_setup[i] = gen_rnd_srb_to_be_setup();
  }

  // DRB to Be Setup Item
  dst.sz_drb_to_be_setup = rand() % 65; // [0,64]
  if(dst.sz_drb_to_be_setup > 0)  {
    dst.drb_to_be_setup = calloc(dst.sz_drb_to_be_setup, sizeof(drb_to_be_setup_t) );
    assert(dst.drb_to_be_setup != NULL && "Memory exhausted" );
  }
  for(size_t i = 0; i < dst.sz_drb_to_be_setup; ++i){
    dst.drb_to_be_setup[i] = gen_drb_to_be_setup();
  }

  //

  // Optional
  // Inactivity Monitoring Request
 
  //Optional 
  // RAT-Frequency Priority Information 9.3.1.34


  //Optional
  //RRC-Container 9.3.1.6


  //Optional 
  //Masked IMEISV 9.3.1.55


  //Optional
  //Serving PLMN 9.3.1.14

  
  // C-ifDRBSetup Optional
  // gNB-DU UE Aggregate Maximum Bit Rate Uplink 9.3.1.22
  

  //Optional
  //RRC Delivery Status Request
  

  // Optional 
  // Resource Coordination Transfer Information 9.3.1.73


  //Optional
  //servingCellMO 


  //Optional 
  //New gNB-CU UE F1AP ID 9.3.1.4
 

  //Optional
  // RAN UE ID


  //Optional
  // Trace Activation 9.3.1.88


  //Optional
  // Additional RRM Policy Index 9.3.1.90


  // BH RLC Channel to be Setup Item [0, 65536]
  // size_t sz_bh_rlc_chan_to_be_setup; 
  // bh_rlc_chan_to_be_setup_t* ; 

  //Optional
  // Configured BAP Address 9.3.1.111

  //Optional
  // NR V2X Services Authorized 9.3.1.116

  //Optional
  // LTE V2X Services Authorized 9.3.1.117

  //Optional
  // NR UE Sidelink Aggregate Maximum Bit Rate 9.3.1.119

  //Optional
  // LTE UE Sidelink Aggregate Maximum Bit Rate 9.3.1.118

  //Optional
  // PC5 Link Aggregate Bit Rate 9.3.1.22

  //Optional
  //SL DRB to Be Setup List [0, 64]

  //Optional
  //Conditional Inter-DU Mobility Information 

  // Optional
  // Management Based MDT PLMN List 9.3.1.151

  //Optional
  // Serving NID 9.3.1.155

  //Optional
  //F1-C Transfer Path 9.3.1.207 


  return dst;
}

static
du_to_cu_rrc_information_f1ap_t gen_rnd_du_to_cu_rrc_information_f1ap(void)
{
  du_to_cu_rrc_information_f1ap_t dst = {0};

  // Mandatory
  // CellGroupConfig
  dst.cell_group_config = gen_byte_array("Cell group ID");

  // Optional
  // MeasGapConfig
  dst.meas_gap_info = NULL; 

  // Optional
  // Requested P-MaxFR1
  dst.req_p_max_fr1 = NULL; 

  // Optional
  // DRX Long Cycle Start Offset
  dst.drx_lng_cyc_str_off = NULL; // [0..10239]

  // Optional
  // Selected BandCombinationIndex
  dst.bnd_comb_idx = NULL; 

  // Optional
  // Selected FeatureSetEntryIndex
  dst.sel_feature_set_entry_idx = NULL; 

  // Optional
  // Ph-InfoSCG
  dst.ph_info_scg = NULL; 

  // Optional
  // Requested BandCombinationIndex
  dst.req_bnd_cmb_idx = NULL; 

  // Optional
  // Requested FeatureSetEntryIndex
  dst.req_feat_set_entry_idx = NULL; 

  // Optional
  // DRX Config
  dst.drx_cnfg = NULL; 

  // Optional
  // PDCCH BlindDetectionSCG
  dst.pdcch_blind_det_scg = NULL; 

  // Optional
  // Requested PDCCH BlindDetectionSCG
  dst.req_pdcch_blnd_det_scg = NULL; 

  // Optional
  // Ph-InfoMCG
  dst.ph_info_mcg = NULL; 

  // Optional
  // MeasGapSharingConfig 
  dst.meas_gap_shr_conf = NULL; 

  // Optional
  // SL-PHY-MAC-RLC-Config
  dst.sl_phy_mac_rlc_conf = NULL; 

  // Optional
  // SL-ConfigDedicatedEUTRA-Info 
  dst.sl_conf_dedica_eutra_info = NULL; 

  // Optional
  // Requested P-MaxFR2 
  dst.req_p_max_fr2 = NULL; 

  return dst;
}

static
dl_up_trans_layer_info_t gen_rnd_dl_up_tnl_info_tbs(void)
{
  dl_up_trans_layer_info_t dst = {0};

  char const ip[] = "192.168.1.1";

  dst.trans_layer_addr.bits_unused = 0;
  dst.trans_layer_addr.size = strlen(ip);

  dst.trans_layer_addr.buf = calloc(1, strlen(ip));
  assert(dst.trans_layer_addr.buf != NULL && "Memory exhausted");

  memcpy(dst.trans_layer_addr.buf, ip, strlen(ip) );

  uint8_t arr[4] = {0xDE, 0xAD, 0xBE, 0xEF};
  memcpy(dst.gtp_teid, arr, 4); 

  return dst;
}

static
drb_setup_item_f1ap_t gen_rnd_drb_setup_item(void)
{
  drb_setup_item_f1ap_t dst = {0}; 
 
  // Mandatory
  // DRB ID 9.3.1.8
  dst.drb_id = (rand() % 32) + 1; // [1,32]
 
  // Optional
  // LCID 9.3.1.35
  dst.lc_id = malloc(sizeof(uint8_t) );
  assert(dst.lc_id != NULL && "Memory exhausted");
  *dst.lc_id = (rand() % 32) + 1; // [1, 32]


  // DL UP TNL Information to be setup
  dst.sz_dl_up_tnl_info_tbs = (rand() % 2) + 1; 
  assert(dst.sz_dl_up_tnl_info_tbs > 0);

  dst.dl_up_tnl_info_tbs = calloc(dst.sz_dl_up_tnl_info_tbs, sizeof(dl_up_trans_layer_info_t));
  assert(dst.dl_up_tnl_info_tbs != NULL && "Not implemented");

  for(size_t i = 0; i < dst.sz_dl_up_tnl_info_tbs; ++i){
    dst.dl_up_tnl_info_tbs[i] = gen_rnd_dl_up_tnl_info_tbs(); 
  }

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  dst.add_pdcp_dup_tnl_lst = NULL;

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  dst.cur_qos_par_set_idx = NULL;

  return dst;
}

static
srb_setup_item_t gen_rnd_srb_setup_item(void)
{
  srb_setup_item_t dst = {0};

  // 9.3.1.7
  dst.srb_id = rand() % 4; // [0,3]

  // 9.3.1.35
   dst.lc_id = (rand()% 32) + 1; // [1,32]

  return dst;
}

ue_ctx_setup_response_t gen_rnd_ue_ctx_setup_response(void)
{
  ue_ctx_setup_response_t dst = {0};

  // Mandatory
  // gNB-CU UE F1AP ID 9.3.1.4
  dst.gnb_cu_ue_id = rand();

  // Mandatory
  // gNB-DU UE F1AP ID 9.3.1.5
  dst.gnb_du_ue_id = rand();

  // Mandatory 
  //  DU To CU RRC Information 9.3.1.26
  dst.du_to_cu_rrc_info = gen_rnd_du_to_cu_rrc_information_f1ap();

  // Optional
  // C-RNTI 9.3.1.32
  dst.c_rnti = calloc(1, sizeof(uint16_t));
  assert(dst.c_rnti != NULL && "Memory exhausted" );
  *dst.c_rnti = rand();

  // Optional
  // Resource Coordination Transfer Container 
  dst.res_coord_trans_cont = NULL;

  // Optional
  // Full Configuration 
  dst.full_config = NULL;

  // DRB Setup Item Iist [0, 64]
  dst.sz_drb_setup_item = 1; //rand() % 65;
  if(dst.sz_drb_setup_item > 0){
    dst.drb_setup_item = calloc(dst.sz_drb_setup_item, sizeof(drb_setup_item_f1ap_t));
    assert( dst.drb_setup_item != NULL && "Memory exhausted");
  }
  for(size_t i = 0; i < dst.sz_drb_setup_item; ++i){
    dst.drb_setup_item[i] = gen_rnd_drb_setup_item(); 
  }

  // SRB Failed to Setup List [0,8]
  dst.sz_srb_failed_setup_item = 0;  
  dst.srb_failed_setup_item = NULL;

  // DRB Failed to Setup List [0, 64]
  dst.sz_drb_failed_setup_item = 0;  
  dst.drb_failed_setup_item = NULL;

  // SCell Failed To Setup List [0, 32]
  dst.scell_failed_setup_item = NULL;

  // Optional
  // Inactivity Monitoring Response
  dst.inactivity_mon_response = NULL;

  // Optional
  // Criticality Diagnostics 9.3.1.3
  dst.crit_diagn = NULL;

  // SRB Setup List [0, 8]
  dst.sz_srb_setup_item = rand() % 9;  
  if(dst.sz_srb_setup_item > 0){
    dst.srb_setup_item = calloc(dst.sz_srb_setup_item, sizeof(srb_setup_item_t) );
    assert(dst.srb_setup_item != NULL && "Memory exhausted" );
  }
  for(size_t i = 0; i < dst.sz_srb_setup_item; ++i){
    dst.srb_setup_item[i] = gen_rnd_srb_setup_item();
  }


  // BH RLC Channel Setup List [0,1]
  // 9.3.1.113
  dst.bh_rlc_setup_lst = NULL;

  // BH RLC Channel Failed to be Setup List
  dst.bh_rlc_chn_failed_tbs_lst = NULL;

  // SL DRB Setup List
  // 9.3.1.120
  dst.sl_drb_setup_lst = NULL;// [1, 512]

  // SL DRB Failed To Setup List
  dst.sl_drb_failed_to_setup_lst = NULL;  

  // Optional
  // Requested Target Cell ID 9.3.1.12 
  dst.req_target_cell_id = NULL;

  return dst;
}


///////
//////
//////

static
cells_to_be_deact_t gen_rnd_cells_to_be_deact()
{
  cells_to_be_deact_t dst = {0}; 

  // NR CGI 9.3.1.12
  dst.nr_cgi = gen_rnd_nr_cgi();

  return dst;
}

static
endpoint_ip_addr_t gen_rnd_ep_ip_addr(void)
{
  endpoint_ip_addr_t dst = {0};

  const char* str = "192.168.1.1";
  dst.trans_layer_add = gen_bit_string(str); 

  return dst;
}

static
endpoint_ip_addr_port_t gen_rnd_ep_ip_addr_port(void)
{
  endpoint_ip_addr_port_t dst = {0};

  dst.endpoint_ip_addr = gen_rnd_ep_ip_addr();
  const char* port = "42";
  dst.port = gen_bit_string(port);

  return dst;
}

static
cp_trans_layer_info_t gen_rnd_cp_trans_layer_info(void)
{
  cp_trans_layer_info_t dst = {0}; 

  dst.type = rand()% END_CP_TRANS_LAYER_INFO;

  // 9.3.2.4
  if(dst.type == IP_ADDRESS_CP_TRANS_LAYER_INFO){
    dst.ep_ip_addr = gen_rnd_ep_ip_addr();
  } else if(dst.type ==  IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO){
    dst.ep_ip_addr_port = gen_rnd_ep_ip_addr_port();
  } else {
    assert(0!=0 && "Not implemented");
  }

  return dst;
}

static
gnb_cu_tnl_asso_to_add_t gen_rnd_gnb_cu_tnl_asso_to_add(void)
{
  gnb_cu_tnl_asso_to_add_t dst; 

  // TNL Association Transport Layer Information 9.3.2.4
  // Mandatory
  dst.cp_trans_layer_info = gen_rnd_cp_trans_layer_info();

  // TNL Association Usage  
  // Mandatory
  dst.tnl_assoc_usage = rand() % END_TNL_ASSOC_USAGE;

  return dst;
}

static
gnb_cu_tnl_assoc_to_rem_t gen_rnd_gnb_cu_tnl_assoc_to_rem(void)
{
  gnb_cu_tnl_assoc_to_rem_t dst; 

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst.tnl_assoc_trans =  gen_rnd_cp_trans_layer_info();

  // TNL Association Transport Layer Address gNB-DU
  // Optional
  dst.tnl_assoc_trans_du = NULL;

  return dst;
}

static
gnb_cu_tnl_assoc_to_upd_t gen_rnd_gnb_cu_tnl_assoc_to_upd(void)
{
  gnb_cu_tnl_assoc_to_upd_t dst; 

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst.tnl_assoc_trans_addr = gen_rnd_cp_trans_layer_info();

  // TNL Association Usage
  // Optional
  dst.tnl_association_usage = NULL;

  return dst;
}

static
cells_to_be_barred_t gen_rnd_cells_to_be_barred(void)
{
  cells_to_be_barred_t dst = {0};

  // NR CGI 9.3.1.12
  // Mandatory
  dst.nr_cgi = gen_rnd_nr_cgi();

  // Cell Barred 
  // Mandatory
  dst.cell_barred = rand() % END_CELL_BARRED;

  // IAB Barred 
  // Optional
  dst.iab_barred = NULL;

  return dst;
}

static
serv_eutra_cell_info_t gen_rnd_serv_eutra_cell_info(void)
{
  serv_eutra_cell_info_t dst = {0}; 

  dst.type = rand() % END_SERV_EUTRA_CELL_INFO;
  if( dst.type == FDD_INFO_SERV_EUTRA_CELL_INFO){
    dst.fdd.dl_offset_to_point_a = rand() % 2200; // [0,2199]
    dst.fdd.ul_offset_to_point_a = rand() % 2200; // [0,2199]
  } else if( dst.type == TDD_INFO_SERV_EUTRA_CELL_INFO){
    dst.tdd.offset_to_point_a = rand() % 2200; // [0,2199]
  } else {
    assert(0!=0 && "Not implemented");
  }

  dst.prot_eutra_resource_ind = NULL;

  return dst;
}

static
eutra_cell_t gen_rnd_eutra_cell(void)
{
  eutra_cell_t dst; 

  // Mandatory
  const char* cell_id = "1234";
  dst.eutra_cell_id = gen_bit_string(cell_id); // [28]
  dst.eutra_cell_id.bits_unused = 4; 

  // Served E-UTRA CellInformation 9.3.1.64
  // Mandatory
  dst.serv_eutra_cell_info = gen_rnd_serv_eutra_cell_info();

  return dst;
}

static
prot_eutra_resources_t gen_rnd_prot_eutra_resources()
{
  prot_eutra_resources_t dst = {0};

  dst.spec_sharing_group_id = (rand() % 256) + 1; // [1, 256]

  dst.sz_eutra_cell = (rand() % 256) + 1;

  dst.eutra_cell = calloc(dst.sz_eutra_cell, sizeof(eutra_cell_t));
  assert(dst.eutra_cell != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz_eutra_cell; ++i){
    dst.eutra_cell[i] = gen_rnd_eutra_cell(); 
  }

  return dst;
}

gnb_cu_conf_update_t gen_rnd_gnb_cu_conf_update(void)
{
  gnb_cu_conf_update_t  dst = {0};

  // 9.3.1.23 Transaction ID
  // Mandatory 
  dst.trans_id = rand();

  dst.sz_cells_to_be_act = 1; // rand()%513; // [0,512]
  if(dst.sz_cells_to_be_act > 0){
    dst.cells_to_be_act = calloc(dst.sz_cells_to_be_act, sizeof(activate_cell_t)), 
    assert(dst.cells_to_be_act != NULL && "Memory exhausted");
  }
  for(size_t i = 0; i < dst.sz_cells_to_be_act; ++i){
    dst.cells_to_be_act[i] = gen_rnd_act_cell(); 
  }

  dst.sz_cells_to_be_deact = rand() % 513;  // [0,512]
  if(dst.sz_cells_to_be_deact > 0){
    dst.cells_to_be_deact = calloc(dst.sz_cells_to_be_deact, sizeof(cells_to_be_deact_t));
    assert(dst.cells_to_be_deact != NULL && "Memory exhausted");
  }
  for(size_t i = 0; i < dst.sz_cells_to_be_deact; ++i){
    dst.cells_to_be_deact[i] = gen_rnd_cells_to_be_deact();
  }

  dst.sz_gnb_cu_tnl_asso_to_add = rand()% 33; // [0, 32]
  if(dst.sz_gnb_cu_tnl_asso_to_add > 0){
    dst.gnb_cu_tnl_asso_to_add = calloc(dst.sz_gnb_cu_tnl_asso_to_add, sizeof(gnb_cu_tnl_asso_to_add_t));
    assert(dst.gnb_cu_tnl_asso_to_add != NULL && "Memory exhausted");
  }                                            
  for(size_t i = 0; i < dst.sz_gnb_cu_tnl_asso_to_add; ++i){
    dst.gnb_cu_tnl_asso_to_add[i] = gen_rnd_gnb_cu_tnl_asso_to_add(); 
  }

  dst.sz_gnb_cu_tnl_assoc_to_rem = rand()%33; // [0,32] 
  if(dst.sz_gnb_cu_tnl_assoc_to_rem > 0){
   dst.gnb_cu_tnl_assoc_to_rem = calloc(dst.sz_gnb_cu_tnl_assoc_to_rem, sizeof(gnb_cu_tnl_assoc_to_rem_t)); 
   assert(dst.gnb_cu_tnl_assoc_to_rem != NULL && "Memory exhausted");
  }
  for(size_t i =0; i < dst.sz_gnb_cu_tnl_assoc_to_rem; ++i){
    dst.gnb_cu_tnl_assoc_to_rem[i] = gen_rnd_gnb_cu_tnl_assoc_to_rem(); 
  } 

  dst.sz_gnb_cu_tnl_assoc_to_upd = rand()%33;
  if(dst.sz_gnb_cu_tnl_assoc_to_upd > 0){
    dst.gnb_cu_tnl_assoc_to_upd = calloc(dst.sz_gnb_cu_tnl_assoc_to_upd, sizeof(gnb_cu_tnl_assoc_to_upd_t));
    assert(dst.gnb_cu_tnl_assoc_to_upd != NULL && "Memory exhausted");
  }
  for(size_t i =0; i < dst.sz_gnb_cu_tnl_assoc_to_upd; ++i){
    dst.gnb_cu_tnl_assoc_to_upd[i] = gen_rnd_gnb_cu_tnl_assoc_to_upd();
  }

  dst.sz_cells_to_be_barred = rand()%513;
  if(dst.sz_cells_to_be_barred > 0){
    dst.cells_to_be_barred = calloc(dst.sz_cells_to_be_barred, sizeof( cells_to_be_barred_t ) );
    assert(dst.cells_to_be_barred != NULL && "Memory exhausted" );
  }
  for(size_t i = 0; i < dst.sz_cells_to_be_barred; ++i){
    dst.cells_to_be_barred[i] = gen_rnd_cells_to_be_barred();
  }


  dst.sz_prot_eutra_resources = rand() % 257;  // [0,256]  
  if(dst.sz_prot_eutra_resources > 0){
    dst.prot_eutra_resources = calloc(dst.sz_prot_eutra_resources , sizeof(prot_eutra_resources_t));
    assert( dst.prot_eutra_resources != NULL && "memory exhausted" );
  }                                                
  for(size_t i = 0; i < dst.sz_prot_eutra_resources; ++i){
    dst.prot_eutra_resources[i] = gen_rnd_prot_eutra_resources(); 
  }                                                  

  dst.sz_neighbour_cell_info = 0; // [0, 512]
  dst.neighbour_cell_info = NULL;  

  // Transport Layer Address Info 9.3.2.5
  // Optional
  dst.trans_layer_add_info = NULL;

  // Uplink BH Non-UP Traffic Mapping 9.3.1.103
  // Optional
  dst.up_bh_non_up_traff_map = NULL; 

  // BAP Address 9.3.1.111
  // Optional
  dst.bap_address = NULL;

  return dst;
}




