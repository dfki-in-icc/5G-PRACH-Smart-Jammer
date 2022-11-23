#include "ssb_pos_burst.h"

#include <assert.h>
#include <stdlib.h>

void free_ssb_pos_burst(ssb_pos_burst_t* src)
{
  assert(src != NULL);

  // No heap memory allocated
  (void)src;
}


bool eq_ssb_pos_burst(ssb_pos_burst_t const* m0, ssb_pos_burst_t const* m1 )
{
  if(m0 == m1)
    return true;

  assert(0!= 0 && "NBot implemented");
  return false;
}
