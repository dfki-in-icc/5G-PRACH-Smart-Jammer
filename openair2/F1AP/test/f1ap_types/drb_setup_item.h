#ifndef DRB_SETUP_ITEM_F1AP_MIR_H
#define DRB_SETUP_ITEM_F1AP_MIR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "dl_up_tnl_info_tbs.h"
#include "up_trans_layer_info.h"

typedef struct {

  // Mandatory
  // DRB ID 9.3.1.8
  uint8_t drb_id; // [1,32]

  // Optional
  // LCID 9.3.1.35
  uint8_t* lc_id; // [1, 32]

  // DL UP TNL Information to be setup
  size_t sz_dl_up_tnl_info_tbs; // [1, 2]
  dl_up_trans_layer_info_t* dl_up_tnl_info_tbs;

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  up_trans_layer_info_t* add_pdcp_dup_tnl_lst;

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  uint8_t* cur_qos_par_set_idx;

} drb_setup_item_f1ap_t;

void free_drb_setup_item_f1ap( drb_setup_item_f1ap_t* src);

bool eq_drb_setup_item_f1ap(drb_setup_item_f1ap_t const* m0, drb_setup_item_f1ap_t const* m1);

#endif

