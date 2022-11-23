#ifndef F1AP_SETUP_RESPONSE_MIR_H
#define F1AP_SETUP_RESPONSE_MIR_H 

#include "activate_cell.h"
#include "gnb_rrc_ver.h"
#include "trans_layer_add_info.h"
#include "ubh_nup_traff.h"
#include "bad_address.h"
#include "ext_gnb_cu_name.h"

#include <stdbool.h>
#include <stdint.h>

#include "../byte_array.h"

typedef struct{

  // 0 Message Type
  
  // 1 Transaction ID
  uint8_t trans_id;

  // 2 gNB-CU Name
  byte_array_t* gnb_cu_name;

  // 3 Cells to activated list. Optional 
  size_t sz_act_cell;
  activate_cell_t* act_cell; 

  // 4 gNB-CU RRC version 
  gnb_rrc_ver_f1ap_t ver;

  // 5 Transport Layer Address Info
  trans_layer_add_info_t* tlai;

  // 6 Uplink BH Non-UP Traffic Mapping 
  ubh_nup_traff_t* ubh_nup_traff; 

  // 7 BAP Address
  bad_address_t* bad_add;

  // 8 Extended gNB-CU Name
  ext_gnb_cu_name_t* egcn; 

} f1_setup_response_t;

void free_f1_setup_response(f1_setup_response_t* );

bool eq_f1_setup_response(f1_setup_response_t const* m0, f1_setup_response_t const* m1);

#endif


