#ifndef UE_CONTEXT_SETUP_REQUEST_MIR_H
#define UE_CONTEXT_SETUP_REQUEST_MIR_H

#include <stdbool.h>
#include <stdint.h>

#include "drx_cycle.h"
#include "nr_cgi.h"
#include "cu_to_du_rrc_info.h"
#include "cell_ul_conf.h"
#include "scell_to_be_setup.h"
#include "srb_to_be_setup.h"
#include "drb_to_be_setup.h"

typedef struct{

  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  uint32_t gnb_cu_ue_f1ap_id;

  // Optional
  // gNB-DU UE F1AP ID 9.3.1.5
  uint32_t* gnb_du_ue_f1ap_id;

  // Mandatory
  // SpCell ID
  nr_cgi_t sp_cell_id;

  // Mandatory
  // ServCellIndex
  uint8_t serv_cell_idx; // INTEGER (0..31,...) 

  // Optional
  // SpCell UL Configured 9.3.1.33
  cell_ul_conf_e* sp_cell_ul_conf;

  // Mandatory
  // CU to DU RRC Information 9.3.1.25
  cu_to_du_rrc_info_t cu_to_du_rrc_info; 

  // Optional 
  // Candidate SpCell ID
  size_t sz_candidate_sp_cell_id; // max. 64
  nr_cgi_t* candidate_sp_cell_id;

  //Optional 
  // DRX Cycle
  drx_cycle_t* drx_cycle;

  // Optional
  // Resource Coordination Transfer Container
  byte_array_t* resource_coord_transfer_container; 

  // SCell To Be Setup List
  size_t sz_scell_to_be_setup; // [0,32]
  scell_to_be_setup_t* scell_to_be_setup;

  // SRB to Be Setup Item
  size_t sz_srb_to_be_setup; // [0,8]
  srb_to_be_setup_t* srb_to_be_setup;

  // DRB to Be Setup Item
  size_t sz_drb_to_be_setup; // [0,64]
  drb_to_be_setup_t* drb_to_be_setup;

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

} ue_ctx_setup_request_t;

void free_ue_ctx_setup_request(ue_ctx_setup_request_t* src);

bool eq_f1_ue_ctx_setup_request(ue_ctx_setup_request_t const* m0, ue_ctx_setup_request_t const* m1);

#endif

