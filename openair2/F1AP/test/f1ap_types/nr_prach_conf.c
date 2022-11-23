#include "nr_prach_conf.h"

#include <assert.h>
#include <stdlib.h>

void free_nr_prach_conf(nr_prach_conf_t* src)
{
  assert(src != NULL);
  // No heap allocated
  (void)src;
}

bool eq_nr_prach_conf(nr_prach_conf_t const* m0, nr_prach_conf_t const* m1)
{
  if(m0 == m1)
    return true;

  assert(0 !=0 && "Not implemented");
  return false;
}

