#include "ul_up_tnl_info.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

void free_ul_up_tnl_info(ul_up_tnl_info_t* src)
{
  assert(src != NULL);
 
  // Mandatory
  // Transport Layer Address
//  free_byte_array(src->tla);
 
  if(src->tla.size > 0){
    free(src->tla.buf);
  }


  // Mandatory
  // 9.3.2.2
  // char gtp_teid[4];

}

bool eq_ul_up_tnl_info(ul_up_tnl_info_t const* m0, ul_up_tnl_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // Transport Layer Address
  if(m0->tla.size != m1->tla.size)
    return false;

  if(memcmp(m0->tla.buf, m1->tla.buf, m1->tla.size) != 0)
    return false;
 
  // Mandatory
  // 9.3.2.2
  if(memcmp(m0->gtp_teid, m1->gtp_teid, 4) != 0)
    return false;

  return true;
}

