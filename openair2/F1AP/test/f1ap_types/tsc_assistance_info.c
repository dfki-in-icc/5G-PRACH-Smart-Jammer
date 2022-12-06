#include "tsc_assistance_info.h"

#include <assert.h>
#include <stdlib.h>

void free_tsc_assistance_info(tsc_assistance_info_t* src)
{
  assert(src != NULL);

  // Mandatory
  // Periodicity 9.3.1.143
  // uint32_t periodicity; // INTEGER (0..640000,

  // Optional
  // Burst Arrival Time 9.3.1.144
  if(src->burst_arrival_time != NULL){
    free_byte_array(*src->burst_arrival_time);
    free(src->burst_arrival_time);
  }
}

bool eq_tsc_assistance_info(tsc_assistance_info_t const* m0, tsc_assistance_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // Periodicity 9.3.1.143
  assert(m0->periodicity < 640001);// INTEGER (0..640000,
  assert(m1->periodicity < 640001);// INTEGER (0..640000,

  // Optional
  // Burst Arrival Time 9.3.1.144
  if(eq_byte_array(m0->burst_arrival_time, m1->burst_arrival_time) == false)
    return false;

  return true;
}

