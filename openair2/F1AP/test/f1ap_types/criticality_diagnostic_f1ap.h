#ifndef CRITICALLITY_DIAGNOSTIC_F1AP_MIR_H
#define CRITICALLITY_DIAGNOSTIC_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "triggering_message_f1ap.h"
#include "criticallity_diagnostic_ie_f1ap.h"

typedef struct {

  //optional
  //Procedure Code
  uint8_t* proc_code;

  //optional
  //Triggering Message
  trig_msg_e* trig_msg;

  //optional
  //Procedure Criticality
  proc_crit_e* proc_crit;

  //optional
  //Transaction ID
  uint8_t* trans_id;

  size_t sz_crit_diagn_ie; // max. 256
  crit_diagn_ie_t* crit_diagn_ie;

} criticallity_diagnostic_f1ap_t;

void free_criticallity_diagnose(criticallity_diagnostic_f1ap_t* c);

bool eq_criticallity_diagnose(criticallity_diagnostic_f1ap_t const* m0, criticallity_diagnostic_f1ap_t const* m1);

#endif
