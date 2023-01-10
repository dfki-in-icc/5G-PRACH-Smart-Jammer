#ifndef SRB_FAILED_SETUP_ITEMF1AP_MIR_H
#define SRB_FAILED_SETUP_ITEMF1AP_MIR_H

#include <stdint.h>

#include "cause_f1ap.h"

typedef struct{

  // Mandatory
  // 9.3.1.7
  uint8_t srb_id;// [0,3]

  // Optional
  // 9.3.1.2
  cause_f1ap_t* cause;

} srb_failed_setup_item_t;


#endif
