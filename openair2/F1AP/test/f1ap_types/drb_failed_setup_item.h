#ifndef DRB_FAILED_SETUP_ITEM_F1AP_MIR_H
#define DRB_FAILED_SETUP_ITEM_F1AP_MIR_H

#include <stdint.h>
#include "cause_f1ap.h"

typedef struct{
  // Mandatory
  // 9.3.1.8
  uint8_t drb_id;// [1,32]

  // Optional
  // 9.3.1.2
  cause_f1ap_t* cause;

} drb_failed_setup_item_t;

#endif
