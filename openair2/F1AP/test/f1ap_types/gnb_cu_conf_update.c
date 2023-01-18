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


  size_t sz_neighbour_cell_info; // [0, 512]
  neighbour_cell_info_t* neighbour_cell_info;  

  // Transport Layer Address Info 9.3.2.5
  // Optional
  trans_layer_add_info_t* trans_layer_add_info;

  // Uplink BH Non-UP Traffic Mapping 9.3.1.103
  // Optional
  up_bh_non_up_traff_map_t* up_bh_non_up_traff_map; 

  // BAP Address 9.3.1.111
  // Optional
  bap_address_t*  bap_address;







}


