#ifndef DRB_TO_BE_SETUP_MIR_H
#define DRB_TO_BE_SETUP_MIR_H 

#include <stdint.h>

#include "add_pdcp_dup_tnl.h"
#include "duplication_act_f1ap.h"
#include "e_utran_qos.h"
#include "drb_info.h"
#include "ul_up_tnl_info_lst.h"
#include "ul_conf_f1ap.h"
#include "rlc_mode_f1ap.h"
#include "rlc_dup_info.h"
#include "pdcp_sn_len.h"


typedef enum{

  E_UTRAN_QOS_INFORMATION = 0,
  DRB_INFO_QOS_INFORMATION,

  END_QOS_INFORMATION

} qos_information_e;

typedef struct{

  // Mandatory
  // DRB ID 9.3.1.8
  uint8_t drb_id; // INTEGER [0,32]

  qos_information_e qos_info;

  // Mandatory
  union{
   e_utran_qos_t e_utran_qos;
   drb_info_t drb_info;
  };

  // UL UP TNL Information to be setup List
  size_t sz_ul_up_tnl_info_lst; // [1, 2]
  ul_up_tnl_info_lst_t* ul_up_tnl_info;

  // Mandatory
  // RLC Mode 9.3.1.27
  rlc_mode_f1ap_e rlc_mode;

  // Optional 
  // UL Configuration 9.3.1.31
  ul_conf_f1ap_e* ul_conf;

  // Optional 
  // Duplication Activation 9.3.1.36
  duplication_act_f1ap_e* dup_act;

  // Optional
  // DC Based Duplication Configured
  bool* dc_dup_act_conf;

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  duplication_act_f1ap_e* dc_dup_act;

  // Mandatory
  // DL PDCP SN length
  pdcp_sn_len_e dl_pdcp_sn_len;

  // Optional
  // UL PDCP SN length
  pdcp_sn_len_e* ul_pdcp_sn_len;

  // Additional PDCP Duplication TNL List
  size_t sz_add_pdcp_dup_tnl; // [0,2]
  add_pdcp_dup_tnl_t* add_pdcp_dup_tnl; 

  // Optional
  // RLC Duplication Information 9.3.1.146
  size_t sz_rlc_dup_info; // [0,3]
  rlc_dup_info_t* rlc_dup_info;  

} drb_to_be_setup_t;

void free_drb_to_be_setup(drb_to_be_setup_t* src);

bool eq_drb_to_be_setup(drb_to_be_setup_t const* m0, drb_to_be_setup_t const* m1);

#endif

