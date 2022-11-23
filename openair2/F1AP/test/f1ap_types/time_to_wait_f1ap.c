#include "time_to_wait_f1ap.h"

#include <stdlib.h>

bool eq_time_to_wait_f1ap(time_to_wait_f1ap_e const* m0, time_to_wait_f1ap_e const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(*m0 != *m1)
    return false;

  return true;
}

