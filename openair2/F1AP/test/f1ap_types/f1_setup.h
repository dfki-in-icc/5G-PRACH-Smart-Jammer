#ifndef F1_SETUP_F1AP_MIR_H
#define F1_SETUP_F1AP_MIR_H 

#include "../byte_array.h"

#include "gnb_du_srv_cell.h"
#include "trans_layer_add_info.h"
#include "bad_address.h"
#include "ext_gnb_du_name.h"
#include "trans_layer_add_info.h"
#include "gnb_rrc_ver.h"

#include <stdint.h>

typedef struct
{
  // 0 Message Type
  
  // 1 Transaction ID
  uint8_t trans_id; // 9.3.1.23

  // 2 gNB-DU ID 
  uint64_t gnb_du_id; // 9.3.1.9

  // 3 gNB-DU Name 
  byte_array_t* gnb_du_name;

  // 4 gNB-DU Served Cells List 
  size_t sz_srv_cell; 
  gnb_du_srv_cell_f1ap_t* srv_cell;

  // 5 gNB-DU RRC version 
  gnb_rrc_ver_f1ap_t ver; // 9.3.1.70

  // 6 Transport Layer Address Info
  trans_layer_add_info_t* tlai; // 9.3.2.5

  // 7 BAP Address
  bad_address_t* bad_add; // 9.3.1.111

  // 8 Extended gNB-DU Name
  ext_gnb_du_name_t* egdn; // 9.3.1.205

} f1_setup_t;

void free_f1_setup(f1_setup_t* setup);

bool eq_f1_setup(f1_setup_t const* m0, f1_setup_t const* m1);

#endif

