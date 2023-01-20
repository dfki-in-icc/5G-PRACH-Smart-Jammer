#ifndef GNB_CU_CONFIGURATION_UPDATE_H
#define GNB_CU_CONFIGURATION_UPDATE_H


#include <stdint.h>
#include <stdlib.h>

#include "activate_cell.h"
#include "bap_address.h"
#include "cells_to_be_deact.h"
#include "cells_to_be_barred.h"
#include "gnb_cu_tnl_assoc_to_add.h"
#include "gnb_cu_tnl_assoc_to_rem.h"
#include "gnb_cu_tnl_assoc_to_upd.h"
#include "neighbour_cell_info.h"
#include "prot_eutra_resources.h"
#include "trans_layer_add_info.h"
#include "up_bh_non_up_traff_map.h"


// 9.2.1.10
typedef struct{

  // 9.3.1.23 Transaction ID
  // Mandatory 
  uint8_t trans_id;

  size_t sz_cells_to_be_act; // [0,512]
  activate_cell_t* cells_to_be_act;

  size_t sz_cells_to_be_deact;  // [0,512]
  cells_to_be_deact_t* cells_to_be_deact;

  size_t sz_gnb_cu_tnl_asso_to_add; // [0, 32]
  gnb_cu_tnl_asso_to_add_t* gnb_cu_tnl_asso_to_add; 

  size_t sz_gnb_cu_tnl_assoc_to_rem; // [0,32] 
  gnb_cu_tnl_assoc_to_rem_t* gnb_cu_tnl_assoc_to_rem; 

  size_t sz_gnb_cu_tnl_assoc_to_upd; // [0, 32]
  gnb_cu_tnl_assoc_to_upd_t* gnb_cu_tnl_assoc_to_upd;

  size_t sz_cells_to_be_barred; // [0, 512] 
  cells_to_be_barred_t* cells_to_be_barred; 

  size_t sz_prot_eutra_resources;  // [0,256]  
  prot_eutra_resources_t* prot_eutra_resources; 

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

} gnb_cu_conf_update_t;

void free_gnb_cu_conf_update(gnb_cu_conf_update_t* src); 

bool eq_gnb_cu_conf_update(gnb_cu_conf_update_t const* m0, gnb_cu_conf_update_t const* m1);

#endif

