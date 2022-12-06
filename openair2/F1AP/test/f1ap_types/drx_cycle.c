#include "drx_cycle.h"

#include <assert.h>
#include <stdlib.h>

void free_drx_cycle(drx_cycle_t * src)
{
  assert(src != NULL);

  // Mandatory 
  // Long DRX Cycle Length
  //long_drx_cycle_len_e long_drx_cycle_len;

  // Optional 
  // Short DRX Cycle Length
  if(src->short_drx_cycle_len != NULL)
    free(src->short_drx_cycle_len); 

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  if(src-> short_drx_cycle_timer != NULL) 
    free(src->short_drx_cycle_timer );
}

bool eq_drx_cycle(drx_cycle_t const* m0, drx_cycle_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory 
  // Long DRX Cycle Length
  if(m0->long_drx_cycle_len != m1->long_drx_cycle_len)
    return false;

  // Optional 
  // Short DRX Cycle Length
  if(m0->short_drx_cycle_len != m1->short_drx_cycle_len){
    if(m0->short_drx_cycle_len == NULL ||  m1->short_drx_cycle_len == NULL) 
      return false;

    if(*m0->short_drx_cycle_len != *m1->short_drx_cycle_len)
      return false;
  }

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  if(m0->short_drx_cycle_timer != m1->short_drx_cycle_timer){
    if(m0->short_drx_cycle_timer == NULL || m1->short_drx_cycle_timer == NULL)
      return false;

    if(*m0->short_drx_cycle_timer != *m1->short_drx_cycle_timer)
      return false;

  }

  return true;
}

