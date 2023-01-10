#ifndef SCELL_FAILED_SETUP_ITEM_F1AP_MIR_H
#define SCELL_FAILED_SETUP_ITEM_F1AP_MIR_H

#include "nr_cgi.h"
#include "cause_f1ap.h"

typedef struct{

  // 9.3.1.12
  nr_cgi_t scell_id;
 
  // 9.3.1.2
  cause_f1ap_t* cause;

} scell_failed_setup_item_t;

#endif
