#include "tsc_traffic_char.h"

#include <assert.h>
#include <stdlib.h>

void free_tsc_traffic_char(tsc_traffic_char_t* src)
{
  assert(src != NULL);

  // Optional
  // TSC Assistance Information Downlink 9.3.1.142
  if(src->down != NULL){
    free_tsc_assistance_info(src->down);
    free(src->down);
  }

  // Optional
  // TSC Assistance Information Uplink 9.3.1.142
  if(src->up != NULL){
    free_tsc_assistance_info(src->up);
    free(src->up);
  }
}


bool eq_tsc_traffic_char(tsc_traffic_char_t const* m0, tsc_traffic_char_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(eq_tsc_assistance_info(m0->down, m1->down) == false)
    return false;

  if(eq_tsc_assistance_info(m0->up, m1->up) == false)
    return false;

  return true;
}

