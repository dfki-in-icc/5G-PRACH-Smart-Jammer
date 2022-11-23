#ifndef CRITICALLITY_DIAGNOSTIC_IE_F1AP_MIR_H
#define CRITICALLITY_DIAGNOSTIC_IE_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "procedure_criticallity_f1ap.h"

typedef enum{

  NOT_UNDERSTOOD_CRITICALLITY_DIAGNOSTIC_IE_F1AP = 0,
  MISSING_CRITICALLITY_DIAGNOSTIC_IE_F1AP = 1,

  END_CRITICALLITY_DIAGNOSTIC_IE_F1AP = 2,

} type_error_e ;

typedef struct{

  // mandatory
  proc_crit_e proc_crit;

  //mandatory
  uint16_t id;

  // mandatory
  type_error_e type_of_error;

} crit_diagn_ie_t;

bool eq_crit_diagn_ie(crit_diagn_ie_t const* m0, crit_diagn_ie_t const* m1);


#endif

