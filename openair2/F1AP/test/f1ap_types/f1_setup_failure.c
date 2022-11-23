#include "f1_setup_failure.h"

#include <assert.h>
#include <stdio.h>

void free_f1_setup_failure( f1_setup_failure_t* sf)
{
  assert(sf != NULL);

  // Mandatory
  // Transaction ID
  //  uint8_t trans_id;

  // Mandatory
  // Cause
  // cause_f1ap_t cause;

  // Optional
  //Time to wait
  if(sf->time_to_wait != NULL){
    free(sf->time_to_wait);
  }

  if(sf->diagnose != NULL){
    free_criticallity_diagnose(sf->diagnose);
    free(sf->diagnose);
  }

}

bool eq_f1_setup_failure(f1_setup_failure_t const* m0, f1_setup_failure_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  // Mandatory
  // Transaction ID
  if(m0->trans_id != m1->trans_id)
    return false;

  // Mandatory
  // Cause
  if(eq_cause_f1ap(&m0->cause, &m1->cause) == false)
    return false;

  // Optional
  //Time to wait
  if(eq_time_to_wait_f1ap(m0->time_to_wait, m1->time_to_wait) == false)
    return false;

  // Optional
  // Criticallity Diagnose
  if(eq_criticallity_diagnose(m0->diagnose, m1->diagnose) == false)
    return false;

  return true;
}

