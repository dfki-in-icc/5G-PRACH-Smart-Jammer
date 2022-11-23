#include "s_nssai.h"

#include <stdlib.h>

void free_s_nssai( s_nssai_t * src)
{
  if(src->sd != NULL) { 
    free_byte_array(*src->sd);
    free(src->sd);
  }
}

bool eq_s_nssai( s_nssai_t const* m0, s_nssai_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(m0->sst != m1->sst )
    return false;

  if(eq_byte_array(m0->sd, m1->sd) == false)
    return false;

  return true;
}

