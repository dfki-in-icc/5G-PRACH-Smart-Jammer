#include "drb_setup_item.h"

#include <assert.h>
#include <stdlib.h>

void free_drb_setup_item_f1ap(drb_setup_item_f1ap_t* src)
{
  assert(src != NULL);

  // Mandatory
  // DRB ID 9.3.1.8
  // uint8_t drb_id; // [1,32]

  // Optional
  // LCID 9.3.1.35
  if(src->lc_id != NULL)
    free(src->lc_id);

  // DL UP TNL Information to be setup
  for(size_t i = 0; i < src->sz_dl_up_tnl_info_tbs; ++i)
    free_dl_up_trans_layer_info(&src->dl_up_tnl_info_tbs[i]); 

  if(src->dl_up_tnl_info_tbs != NULL)
    free(src->dl_up_tnl_info_tbs);

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  assert(src->add_pdcp_dup_tnl_lst == NULL && "Not implemented");

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  if(src->cur_qos_par_set_idx != NULL)
    free(src->cur_qos_par_set_idx);
}


bool eq_drb_setup_item_f1ap(drb_setup_item_f1ap_t const* m0, drb_setup_item_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // DRB ID 9.3.1.8 // [1,32]
  assert(m0->drb_id > 0 && m0->drb_id < 33);
  assert(m1->drb_id > 0 && m1->drb_id < 33);
  if(m0->drb_id != m1->drb_id)
    return false;
  
  // Optional
  // LCID 9.3.1.35 // [1, 32]
  if((m0->lc_id != NULL && m1->lc_id == NULL) || (m0->lc_id != NULL && m1->lc_id == NULL)){
    return false;
  } else if(m0->lc_id != NULL && m1->lc_id != NULL){
    assert(*m0->lc_id > 0 && *m0->lc_id < 33);
    assert(*m1->lc_id > 0 && *m1->lc_id < 33);

    if(*m0->lc_id != *m1->lc_id)
      return false;
  } // else m0->lc_id == NULL && m1->lc_id == NULL 

  // DL UP TNL Information to be setup // [1, 2]
  assert(m0->sz_dl_up_tnl_info_tbs == 1 || m0->sz_dl_up_tnl_info_tbs == 2);
  assert(m1->sz_dl_up_tnl_info_tbs == 1 || m1->sz_dl_up_tnl_info_tbs == 2);
  if(m0->sz_dl_up_tnl_info_tbs != m1->sz_dl_up_tnl_info_tbs)
    return false;

  for(size_t i = 0; i < m0->sz_dl_up_tnl_info_tbs; ++i){
    if(eq_dl_up_trans_layer_info(&m0->dl_up_tnl_info_tbs[i], &m1->dl_up_tnl_info_tbs[i]) == false)
      return false;
  }

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  assert(m0->add_pdcp_dup_tnl_lst == NULL && "Not implemented" );
  assert(m1->add_pdcp_dup_tnl_lst == NULL && "Not implemented" );

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  if((m0->cur_qos_par_set_idx != NULL && m1->cur_qos_par_set_idx == NULL) && (m0->cur_qos_par_set_idx == NULL && m1->cur_qos_par_set_idx != NULL)){
    return false;
  } else if(m0->cur_qos_par_set_idx != NULL && m1->cur_qos_par_set_idx != NULL){
      assert(*m0->cur_qos_par_set_idx > 0 && *m0->cur_qos_par_set_idx < 9);
      assert(*m1->cur_qos_par_set_idx > 0 && *m1->cur_qos_par_set_idx < 9);
      if(*m0->cur_qos_par_set_idx != *m1->cur_qos_par_set_idx)
        return false;
  } // else m0->cur_qos_par_set_idx == NULL && m1->cur_qos_par_set_idx == NULL

  return true;
}

