#include "criticallity_diagnostic_ie_f1ap.h"

#include <stdlib.h>

bool eq_crit_diagn_ie(crit_diagn_ie_t const* m0, crit_diagn_ie_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;



  if(m0->proc_crit != m1->proc_crit)
    return false;

  if(m0->id != m1->id)
    return false;

  if(m0->type_of_error != m1->type_of_error)
    return false;

  return true;
}

