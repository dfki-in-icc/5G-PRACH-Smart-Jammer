#include "gnb_cu_conf_update.h"


#include <assert.h>


void free_gnb_cu_conf_update(gnb_cu_conf_update_t* src)
{
  assert(src != NULL);

  // 9.3.1.23 Transaction ID
  // Mandatory 
  // uint8_t trans_id;

  assert(src->sz_cells_to_be_act < 513); // [0,512]
  for(size_t i = 0; i < src->sz_cells_to_be_act; ++i){
    free_activate_cell(&src->cells_to_be_act[i]);
  }
  if(src->sz_cells_to_be_act > 0){
    free(src->cells_to_be_act);
  } 

  assert(src->sz_cells_to_be_deact < 513); 
  for(size_t i = 0; i < src->sz_cells_to_be_deact; ++i){
    free_nr_cgi(&src->cells_to_be_deact[i].nr_cgi);
  } 
  if(src->cells_to_be_deact != NULL)
    free(src->cells_to_be_deact);

  assert(src->sz_gnb_cu_tnl_asso_to_add < 33);
  for(size_t i = 0; i < src->sz_gnb_cu_tnl_asso_to_add; ++i){
    free_gnb_cu_tnl_asso_to_add(&src->gnb_cu_tnl_asso_to_add[i]);
  }
  if(src->gnb_cu_tnl_asso_to_add != NULL){
    free(src->gnb_cu_tnl_asso_to_add); 
  }

  assert(src->sz_gnb_cu_tnl_assoc_to_rem < 33);
  for(size_t i = 0; i < src->sz_gnb_cu_tnl_assoc_to_rem; ++i){
    free_gnb_cu_tnl_assoc_to_rem(&src->gnb_cu_tnl_assoc_to_rem[i]);
  }
  if(src->gnb_cu_tnl_assoc_to_rem != NULL){
    free(src->gnb_cu_tnl_assoc_to_rem);
  }

  assert(src->sz_gnb_cu_tnl_assoc_to_upd < 33);
  for(size_t i = 0; i < src->sz_gnb_cu_tnl_assoc_to_upd; ++i){
    free_gnb_cu_tnl_assoc_to_upd(&src->gnb_cu_tnl_assoc_to_upd[i]);
  }
  if(src->gnb_cu_tnl_assoc_to_upd != NULL){
    free(src->gnb_cu_tnl_assoc_to_upd);
  }

  assert(src->sz_cells_to_be_barred < 513); // [0, 512] 
  for(size_t i = 0; i < src->sz_cells_to_be_barred; ++i){
    free_cells_to_be_barred(&src->cells_to_be_barred[i]);
  } 
  if(src->sz_cells_to_be_barred > 0){
    free(src->cells_to_be_barred);
  }

  assert(src->sz_prot_eutra_resources < 257);
  for(size_t i = 0; i < src->sz_prot_eutra_resources; ++i){
    free_prot_eutra_resources(&src->prot_eutra_resources[i]);
  }
  if(src->prot_eutra_resources != NULL){
    free(src->prot_eutra_resources);
  }

  assert(src->sz_neighbour_cell_info < 513);
  for(size_t i = 0; i < src->sz_neighbour_cell_info; ++i){
    free_neighbour_cell_info(&src->neighbour_cell_info[i]);
  }
  if(src->neighbour_cell_info != NULL){
    free(src->neighbour_cell_info);
  }


  // Transport Layer Address Info 9.3.2.5
  // Optional
  assert(src->trans_layer_add_info == NULL && "Not implemented");

  // Uplink BH Non-UP Traffic Mapping 9.3.1.103
  // Optional
  assert(src->up_bh_non_up_traff_map == NULL && "Not implemented");

  // BAP Address 9.3.1.111
  // Optional
  assert(src->bap_address == NULL && "Not implemented" );
}



bool eq_gnb_cu_conf_update(gnb_cu_conf_update_t const* m0, gnb_cu_conf_update_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // 9.3.1.23 Transaction ID
  // Mandatory 
  if(m0->trans_id != m1->trans_id)
    return false;

  // Cells to be activated // [0,512]
  if(m0->sz_cells_to_be_act != m1->sz_cells_to_be_act) 
    return false;
  for(size_t i = 0; i <  m0->sz_cells_to_be_act; ++i){
    if(eq_activate_cell(&m0->cells_to_be_act[i], &m1->cells_to_be_act[i]) == false)
      return false;
  }

  // Cells to be deactivated // [0,512]
  if(m0->sz_cells_to_be_deact != m1->sz_cells_to_be_deact)
    return false;
  for(size_t i = 0; i < m0->sz_cells_to_be_deact; ++i){
    if(eq_cells_to_be_deact(&m0->cells_to_be_deact[i], &m1->cells_to_be_deact[i]) == false)
      return false;
  }

  // TNL Association to Add // [0, 32]
  if(m0->sz_gnb_cu_tnl_asso_to_add != m1->sz_gnb_cu_tnl_asso_to_add)
    return false;
  for(size_t i = 0; i < m0->sz_gnb_cu_tnl_asso_to_add; ++i){
    if(eq_gnb_cu_tnl_asso_to_add(&m0->gnb_cu_tnl_asso_to_add[i], &m1->gnb_cu_tnl_asso_to_add[i]) == false)
      return false;
  }

  // TNL Association to Remove // [0, 32]
  if(m0->sz_gnb_cu_tnl_assoc_to_rem != m1->sz_gnb_cu_tnl_assoc_to_rem)
    return false;
  for(size_t i = 0; i < m0->sz_gnb_cu_tnl_assoc_to_rem; ++i){
    if(eq_gnb_cu_tnl_asso_to_rem(&m0->gnb_cu_tnl_assoc_to_rem[i], &m1->gnb_cu_tnl_assoc_to_rem[i]) == false)
      return false;
  }

  // TNL Association to Update // [0, 32]
  if(m0->sz_gnb_cu_tnl_assoc_to_upd != m1->sz_gnb_cu_tnl_assoc_to_upd)
    return false;
  for(size_t i = 0; i < m0->sz_gnb_cu_tnl_assoc_to_upd; ++i){
    if(eq_gnb_cu_tnl_asso_to_upd(&m0->gnb_cu_tnl_assoc_to_upd[i], &m1->gnb_cu_tnl_assoc_to_upd[i]) == false)
      return false;
  }

  // Cells to be Barred // [0, 512] 
  if(m0-> sz_cells_to_be_barred != m1->sz_cells_to_be_barred  )
    return false;
  for(size_t i = 0; i < m0->sz_cells_to_be_barred; ++i){
    if(eq_cells_to_be_barred(&m0->cells_to_be_barred[i], &m1->cells_to_be_barred[i]) == false)
      return false;
  }

  // Protected EUTRA Resources
  if(m0->sz_prot_eutra_resources != m1->sz_prot_eutra_resources)
    return false;

  for(size_t i = 0; i < m0->sz_prot_eutra_resources; ++i){
    if(eq_prot_eutra_resources(&m0->prot_eutra_resources[i], &m1->prot_eutra_resources[i]) == false)
      return false;
  }

  // Neighbour Cells [0, 512]
  assert(m0->sz_neighbour_cell_info == 0 && "Not implemented");
  assert(m1->sz_neighbour_cell_info == 0 && "Not implemented");
  assert(m0->neighbour_cell_info == NULL && "Not implemented");
  assert(m1->neighbour_cell_info == NULL && "Not implemented");

  // Transport Layer Address Info 9.3.2.5
  // Optional
  assert(m0-> trans_layer_add_info == NULL && "Not implemented");
  assert(m1-> trans_layer_add_info == NULL && "Not implemented");

  // Uplink BH Non-UP Traffic Mapping 9.3.1.103
  // Optional
  assert(m0-> up_bh_non_up_traff_map== NULL && "Not implemented");
  assert(m1-> up_bh_non_up_traff_map== NULL && "Not implemented");
 
  // BAP Address 9.3.1.111
  // Optional
  assert(m0->bap_address == NULL && "Not implemented");
  assert(m1->bap_address == NULL && "Not implemented");

  return true;
}

