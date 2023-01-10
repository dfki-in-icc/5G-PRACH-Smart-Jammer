#ifndef SL_DRV_FAILED_TO_SETUP_LST_F1AP_MIR_H
#define  SL_DRV_FAILED_TO_SETUP_LST_F1AP_MIR_H

#include "cause_f1ap.h"

#include <stdint.h>

typedef struct{

  // Mandatory
  //9.3.1.120
  uint16_t sl_drb_id; // [1,512]

  // Optional
  cause_f1ap_t* cause;

} sl_drb_failed_to_setup_lst_t;


#endif

