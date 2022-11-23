#ifndef F1_SETUP_FAILURE_MIR_H
#define F1_SETUP_FAILURE_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "cause_f1ap.h"
#include "time_to_wait_f1ap.h"
#include "criticallity_diagnostic_f1ap.h"

typedef struct{

  // Mandatory
  //Message Type

  // Mandatory
  //Transaction ID
  uint8_t trans_id;

  //Mandatory
  //Cause
  cause_f1ap_t cause;

  // Optional
  //Time to wait
  time_to_wait_f1ap_e* time_to_wait;

  //Optional
  //Criticality Diagnostics
  criticallity_diagnostic_f1ap_t* diagnose;

} f1_setup_failure_t ;

void free_f1_setup_failure(f1_setup_failure_t* sf);

bool eq_f1_setup_failure(f1_setup_failure_t const* m0, f1_setup_failure_t const* m1);

#endif

