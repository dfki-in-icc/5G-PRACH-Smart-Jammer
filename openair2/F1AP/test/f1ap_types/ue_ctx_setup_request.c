#include "ue_ctx_setup_request.h"

#include <assert.h>

void free_ue_ctx_setup_request(ue_ctx_setup_request_t* src)
{
  assert(src != NULL);
  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  //uint32_t gnb_cu_ue_f1ap_id;

  // Optional
  // gNB-DU UE F1AP ID 9.3.1.5
  if(src->gnb_du_ue_f1ap_id != NULL )
    free(src->gnb_du_ue_f1ap_id);

  // Mandatory
  // SpCell ID
  //nr_cgi_t sp_cell_id;

  // Mandatory
  // ServCellIndex
  //uint8_t serv_cell_idx; // INTEGER (0..31,...) 

  // Optional
  // SpCell UL Configured 9.3.1.33
  if(src->sp_cell_ul_conf != NULL)
    free(src->sp_cell_ul_conf);

  // Mandatory
  // CU to DU RRC Information 9.3.1.25
  free_cu_to_du_rrc_info(&src->cu_to_du_rrc_info);

  // Optional 
  // Candidate SpCell ID
  for(size_t i = 0; i < src->sz_candidate_sp_cell_id; ++i){
    free_nr_cgi(&src->candidate_sp_cell_id[i]);
  }
  if(src->candidate_sp_cell_id != NULL)
    free(src->candidate_sp_cell_id);

  //Optional 
  // DRX Cycle
  if(src->drx_cycle != NULL) {
    free_drx_cycle(src->drx_cycle);
    free(src->drx_cycle);
  }

  // Optional
  // Resource Coordination Transfer Container
  if(src->resource_coord_transfer_container != NULL){
    free_byte_array(*src->resource_coord_transfer_container); 
    free(src->resource_coord_transfer_container );
  }

  // SCell To Be Setup List
  assert(src->sz_scell_to_be_setup < 33);
  for(size_t i = 0; i <  src->sz_scell_to_be_setup; ++i){
    free_scell_to_be_setup(&src->scell_to_be_setup[i]);
  }
  if(src->scell_to_be_setup != NULL)
    free(src->scell_to_be_setup);
  
  // SRB to Be Setup Item
  assert(src->sz_srb_to_be_setup < 9);
  for(size_t i = 0; i < src->sz_srb_to_be_setup; ++i ){
    free_srb_to_be_setup(&src->srb_to_be_setup[i]);
  }
  if(src->srb_to_be_setup != NULL)
    free(src->srb_to_be_setup );

  // DRB to Be Setup Item
  assert(src->sz_drb_to_be_setup < 65);
  for(size_t i = 0; i < src-> sz_drb_to_be_setup; ++i){
    free_drb_to_be_setup(&src->drb_to_be_setup[i]);
  }
  if(src->drb_to_be_setup != NULL)
    free(src->drb_to_be_setup);

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

}

bool eq_f1_ue_ctx_setup_request(ue_ctx_setup_request_t const* m0, ue_ctx_setup_request_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  if(m0->gnb_cu_ue_f1ap_id != m1->gnb_cu_ue_f1ap_id)
    return false;

  // Optional
  // gNB-DU UE F1AP ID 9.3.1.5
  if(m0->gnb_du_ue_f1ap_id != m1->gnb_du_ue_f1ap_id){
    if(m0->gnb_du_ue_f1ap_id == NULL)
      return false;
    if(m1->gnb_du_ue_f1ap_id == NULL)
      return false;

    if(*m0->gnb_du_ue_f1ap_id != *m1->gnb_du_ue_f1ap_id)
      return false;
  }

  // Mandatory
  // SpCell ID
  if(eq_nr_cgi(&m0->sp_cell_id, &m1->sp_cell_id) == false)
    return false;

  // Mandatory
  // ServCellIndex
  assert(m0-> serv_cell_idx < 32 && m1->serv_cell_idx < 32) ;
  if(m0->serv_cell_idx != m1->serv_cell_idx)
    return false;
  //uint8_t serv_cell_idx; // INTEGER (0..31,...) 

  // Optional
  // SpCell UL Configured 9.3.1.33
  if(m0->sp_cell_ul_conf != m1->sp_cell_ul_conf){
    if(m0->sp_cell_ul_conf == NULL)
      return false;
    if(m1->sp_cell_ul_conf == NULL)
      return false;

    if(*m0->sp_cell_ul_conf != *m1->sp_cell_ul_conf )
      return false;
  }

  // Mandatory
  // CU to DU RRC Information 9.3.1.25
  if(eq_cu_to_du_rrc_info(&m0->cu_to_du_rrc_info, &m1->cu_to_du_rrc_info) == false)
    return false;

  // Optional 
  // Candidate SpCell ID
  if(m0->sz_candidate_sp_cell_id != m1->sz_candidate_sp_cell_id)
    return false;

  for(size_t i = 0; i < m0->sz_candidate_sp_cell_id; ++i){
    if(eq_nr_cgi(&m0->candidate_sp_cell_id[i], &m1->candidate_sp_cell_id[i]) == false)
      return false;
  }

  //Optional 
  // DRX Cycle
  if(eq_drx_cycle(m0->drx_cycle, m1->drx_cycle) == false)
    return false;

  // Optional
  // Resource Coordination Transfer Container
  if(eq_byte_array(m0->resource_coord_transfer_container, m1->resource_coord_transfer_container  ) == false)
    return false;
 
  // SCell To Be Setup List
  assert(m0->sz_scell_to_be_setup < 33);
  assert(m1->sz_scell_to_be_setup < 33);
  if(m0->sz_scell_to_be_setup != m1->sz_scell_to_be_setup)
    return false;

  for(size_t i = 0; i < m0->sz_scell_to_be_setup; ++i){
    if(eq_scell_to_be_setup(&m0->scell_to_be_setup[i], &m1->scell_to_be_setup[i]) == false)
      return false;
  }

  // SRB to Be Setup Item
  assert(m0->sz_srb_to_be_setup < 9);
  assert(m1->sz_srb_to_be_setup < 9);
  for(size_t i = 0; i < m0->sz_srb_to_be_setup; ++i){
    if(eq_srb_to_be_setup(&m0->srb_to_be_setup[i], &m1->srb_to_be_setup[i]) == false)
      return false;
  }

  // DRB to Be Setup Item
  assert(m0->sz_drb_to_be_setup < 65);
  assert(m1->sz_drb_to_be_setup < 65);
  for(size_t i = 0; i < m0->sz_drb_to_be_setup; ++i){
   if(eq_drb_to_be_setup(&m0->drb_to_be_setup[i], &m1->drb_to_be_setup[i]) == false)
     return false;
  }

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

  return true;
}

