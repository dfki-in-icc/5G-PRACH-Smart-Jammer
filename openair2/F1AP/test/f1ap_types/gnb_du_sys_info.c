#include "gnb_du_sys_info.h"

#include <assert.h>
#include <stdlib.h>

void free_gnb_du_sys_info( gnb_du_sys_info_t* src)
{
  assert(src != NULL);

  // No heap memory allocated
  (void)src;
}


bool eq_gnb_du_sys_info( gnb_du_sys_info_t const* m0,  gnb_du_sys_info_t const* m1)
{
  if(m0 == m1)
    return true;

  assert(0!=0 && "not implemented" );

  return false;
}

