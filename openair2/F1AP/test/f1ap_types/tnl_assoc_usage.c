#include "tnl_assoc_usage.h"

#include <stdlib.h>


bool eq_tnl_assoc_usage( tnl_assoc_usage_e const* m0, tnl_assoc_usage_e const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(*m0 != *m1)
    return false;

  return true;
}

