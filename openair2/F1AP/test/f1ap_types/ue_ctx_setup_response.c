#include "ue_ctx_setup_response.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

void free_ue_ctx_setup_response(ue_ctx_setup_response_t* src)
{
  assert(src != NULL);

  // Mandatory
  // gNB-CU UE F1AP ID 9.3.1.4
  //uint32_t gnb_cu_ue_id;

  // Mandatory
  // gNB-DU UE F1AP ID 9.3.1.5
  //uint32_t gnb_du_ue_id;

  // Mandatory 
  //  DU To CU RRC Information 9.3.1.26
  free_du_to_cu_rrc_information_f1ap(&src->du_to_cu_rrc_info);

  // Optional
  // C-RNTI 9.3.1.32
  if(src->c_rnti != NULL)
    free(src->c_rnti);

  // Optional
  // Resource Coordination Transfer Container 
  if(src->res_coord_trans_cont != NULL){
    free_byte_array(*src->res_coord_trans_cont);
    free(src->res_coord_trans_cont);
  }

  // Optional
  // Full Configuration 
  if(src->full_config != NULL)
    free(src->full_config);

  // DRB Setup Item Iist [0, 64]
  for(size_t i = 0; i < src->sz_drb_setup_item; ++i){
    free_drb_setup_item_f1ap(&src->drb_setup_item[i]);
  }
  if(src->drb_setup_item != NULL)
    free(src->drb_setup_item);

  // SRB Failed to Setup List [0,8]
  assert(src->sz_srb_failed_setup_item == 0 && "Not implemented");
  assert(src->srb_failed_setup_item == NULL && "Not implemented");


  // DRB Failed to Setup List [0, 64]
  assert(src->sz_drb_failed_setup_item == 0 && "Not implemented");  
  assert(src->drb_failed_setup_item == NULL && "Not implemented");

  // SCell Failed To Setup List [0, 32]
  assert(src->scell_failed_setup_item == NULL && "Not implemented");

  // Optional
  // Inactivity Monitoring Response
  assert(src->inactivity_mon_response == NULL && "Not implemented");

  // Optional
  // Criticality Diagnostics 9.3.1.3
  assert(src->crit_diagn == NULL && "Not implemented");

  // SRB Setup List [0, 8]
  // No heap memory allocated
//  for(size_t i = 0; i < src->sz_srb_setup_item; ++i){
//    free_srb_setup_item(&src->srb_setup_item[i]);
//  }

  if(src-> srb_setup_item != NULL)
    free(src->srb_setup_item);

  // BH RLC Channel Setup List [0,1]
  // 9.3.1.113
   assert(src->bh_rlc_setup_lst == NULL && "Not implemented");

  // BH RLC Channel Failed to be Setup List
   assert(src->bh_rlc_chn_failed_tbs_lst == NULL && "Not implemented");

  // SL DRB Setup List
  // 9.3.1.120
   assert(src->sl_drb_setup_lst == NULL && "Not implemented");// [1, 512]

  // SL DRB Failed To Setup List
   assert(src->sl_drb_failed_to_setup_lst == NULL && "Not implemented");  

  // Optional
  // Requested Target Cell ID 9.3.1.12 
  assert(src->req_target_cell_id == NULL && "Not implemented");

}

bool eq_f1_ue_ctx_setup_response(ue_ctx_setup_response_t const* m0, ue_ctx_setup_response_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // gNB-CU UE F1AP ID 9.3.1.4
  if(m0->gnb_cu_ue_id != m1->gnb_cu_ue_id)
    return false;

  // Mandatory
  // gNB-DU UE F1AP ID 9.3.1.5
  if(m0-> gnb_du_ue_id != m1->gnb_du_ue_id  )
    return false;

  // Mandatory 
  //  DU To CU RRC Information 9.3.1.26
  if(eq_du_to_cu_rrc_information_f1ap(&m0->du_to_cu_rrc_info, &m1->du_to_cu_rrc_info) == false)
    return false;

  // Optional
  // C-RNTI 9.3.1.32
  if((m0->c_rnti != NULL && m1->c_rnti == NULL) || (m0->c_rnti == NULL && m1->c_rnti != NULL)){
    return false;
  } else if(m0->c_rnti != NULL && m1->c_rnti != NULL){
    if(*m0->c_rnti != *m1->c_rnti)
      return false;
  } // else m0->c_rnti == NULL && m1->c_rnti == NULL

  // Optional
  // Resource Coordination Transfer Container 
  if(eq_byte_array(m0->res_coord_trans_cont, m1->res_coord_trans_cont) == false)
    return false;

  // Optional
  // Full Configuration 
  if(( m0->full_config != NULL && m1->full_config == NULL) || (m0->full_config == NULL && m1->full_config != NULL ) ){
    return false;
  } else if(m0->full_config != NULL && m1->full_config != NULL){
    if(*m0->full_config != *m1->full_config) 
      return false;
  } // else  m0->full_config == NULL && m1->full_config == NULL

  // DRB Setup Item Iist [0, 64]
  if(m0->sz_drb_setup_item != m1->sz_drb_setup_item)
    return false;

  for(size_t i = 0; i < m0->sz_drb_setup_item; ++i){
    if(eq_drb_setup_item_f1ap( &m0->drb_setup_item[i], &m1->drb_setup_item[i]) == false)
      return false;
  }

  // SRB Failed to Setup List [0,8]
  assert(m0->sz_srb_failed_setup_item == 0 && "Not implemented");
  assert(m1->sz_srb_failed_setup_item == 0 && "Not implemented");
  assert(m0->srb_failed_setup_item == NULL && "Not implemented");
  assert(m1->srb_failed_setup_item == NULL && "Not implemented");

  // DRB Failed to Setup List [0, 64]
  assert(m0->sz_drb_failed_setup_item == 0 && "Not implemented");
  assert(m1->sz_drb_failed_setup_item == 0 && "Not implemented");
  assert(m0->drb_failed_setup_item == NULL && "Not implemented");
  assert(m1->drb_failed_setup_item == NULL && "Not implemented");

  // SCell Failed To Setup List [0, 32]
  assert(m0->scell_failed_setup_item == NULL && "Not implemented");
  assert(m1->scell_failed_setup_item == NULL && "Not implemented");

  // Optional
  // Inactivity Monitoring Response
  assert(m0->inactivity_mon_response == NULL && "Not implemented");
  assert(m1->inactivity_mon_response == NULL && "Not implemented");

  // Optional
  // Criticality Diagnostics 9.3.1.3
  assert(m0->crit_diagn == NULL && "Not implemented");
  assert(m1->crit_diagn == NULL && "Not implemented");

  // SRB Setup List [0, 8]
  if(m0->sz_srb_setup_item != m1->sz_srb_setup_item)
    return false;
  for(size_t i = 0; i < m0->sz_srb_setup_item; ++i){
    if(eq_srb_setup_item(&m0->srb_setup_item[i], &m1->srb_setup_item[i] ) == false)
      return false;
  }

  // BH RLC Channel Setup List [0,1]
  // 9.3.1.113
  assert(m0->bh_rlc_setup_lst == NULL && "Not implemented");
  assert(m1->bh_rlc_setup_lst == NULL && "Not implemented");

  // BH RLC Channel Failed to be Setup List
  assert(m0->bh_rlc_chn_failed_tbs_lst == NULL && "Not implemented");
  assert(m1->bh_rlc_chn_failed_tbs_lst == NULL && "Not implemented");

  // SL DRB Setup List
  // 9.3.1.120 // [1, 512]
  assert(m0->sl_drb_setup_lst == NULL && "Not implemented");
  assert(m1->sl_drb_setup_lst == NULL && "Not implemented");

  // SL DRB Failed To Setup List
  assert(m0->sl_drb_failed_to_setup_lst == NULL && "Not implemented");
  assert(m1->sl_drb_failed_to_setup_lst == NULL && "Not implemented");

  // Optional
  // Requested Target Cell ID 9.3.1.12 
  if(eq_nr_cgi(m0-> req_target_cell_id, m1-> req_target_cell_id ) == false )
    return false;

  return true;
}

