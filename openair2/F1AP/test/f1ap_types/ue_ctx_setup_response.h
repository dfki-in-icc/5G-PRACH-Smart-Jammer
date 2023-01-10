#ifndef F1AP_UE_CTX_SETUP_RESPONSE_MIR_H
#define F1AP_UE_CTX_SETUP_RESPONSE_MIR_H 

#include <stdint.h>

#include "criticality_diagnostic_f1ap.h"
#include "srb_failed_setup_item.h"
#include "drb_failed_setup_item.h"
#include "du_to_cu_rrc_information_f1ap.h"
#include "scell_failed_setup_item.h"
#include "srb_setup_item.h"
#include "bh_rlc_chn_failed_tbs_lst.h"
#include "sl_drb_failed_to_setup_lst.h"

#include "drb_setup_item.h"

typedef enum{
  FULL_FULL_CONFIG_F1AP, 

  END_FULL_CONFIG_F1AP 

} full_config_f1ap_e;


typedef enum{

  NOT_SUPPORTED_INACTIVITY_MON_RESPONSE  ,

  END_INACTIVITY_MON_RESPONSE 

} inactivity_mon_response_e;


typedef struct {

  // Mandatory
  // gNB-CU UE F1AP ID 9.3.1.4
  uint32_t gnb_cu_ue_id;

  // Mandatory
  // gNB-DU UE F1AP ID 9.3.1.5
  uint32_t gnb_du_ue_id;

  // Mandatory 
  //  DU To CU RRC Information 9.3.1.26
  du_to_cu_rrc_information_f1ap_t du_to_cu_rrc_info;

  // Optional
  // C-RNTI 9.3.1.32
  uint16_t* c_rnti; 

  // Optional
  // Resource Coordination Transfer Container 
  byte_array_t* res_coord_trans_cont;

  // Optional
  // Full Configuration 
  full_config_f1ap_e* full_config;

  // DRB Setup Item Iist [0, 64]
  size_t sz_drb_setup_item;
  drb_setup_item_f1ap_t* drb_setup_item;

  // SRB Failed to Setup List [0,8]
  size_t sz_srb_failed_setup_item;  
  srb_failed_setup_item_t* srb_failed_setup_item;

  // DRB Failed to Setup List [0, 64]
  size_t sz_drb_failed_setup_item;  
  drb_failed_setup_item_t* drb_failed_setup_item;

  // SCell Failed To Setup List [0, 32]
  scell_failed_setup_item_t* scell_failed_setup_item;

  // Optional
  // Inactivity Monitoring Response
  inactivity_mon_response_e* inactivity_mon_response;

  // Optional
  // Criticality Diagnostics 9.3.1.3
  criticallity_diagnostic_f1ap_t* crit_diagn;

  // SRB Setup List [0, 8]
  size_t sz_srb_setup_item;
  srb_setup_item_t* srb_setup_item;

  // BH RLC Channel Setup List [0,1]
  // 9.3.1.113
  BIT_STRING_t* bh_rlc_setup_lst;

  // BH RLC Channel Failed to be Setup List
  bh_rlc_chn_failed_tbs_lst_t* bh_rlc_chn_failed_tbs_lst;

  // SL DRB Setup List
  // 9.3.1.120
  uint16_t* sl_drb_setup_lst;// [1, 512]

  // SL DRB Failed To Setup List
  sl_drb_failed_to_setup_lst_t* sl_drb_failed_to_setup_lst;  

  // Optional
  // Requested Target Cell ID 9.3.1.12 
  nr_cgi_t* req_target_cell_id;

} ue_ctx_setup_response_t ;

void free_ue_ctx_setup_response(ue_ctx_setup_response_t* src);

bool eq_f1_ue_ctx_setup_response(ue_ctx_setup_response_t const* m0, ue_ctx_setup_response_t const* m1);


#endif
