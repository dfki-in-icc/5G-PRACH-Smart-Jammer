#include "add_pdcp_dup_tnl.h"

#include <assert.h>
#include <stdlib.h>

void free_add_pdcp_dup_tnl( add_pdcp_dup_tnl_t* src)
{
  assert(src != NULL);

  // Mandatory
  // Transport Layer Address 9.3.2.3
  if(src->trans_layer_add.size > 0){
    free(src->trans_layer_add.buf);
  }
//  free_byte_array(src->trans_layer_add); // BIT STRING(SIZE(1..160, ...))

  // Mandatory
  // GTP-TEID 9.3.2.2
  // char gtp_teid[4];

}

bool eq_add_pdcp_dup_tnl( add_pdcp_dup_tnl_t const* m0, add_pdcp_dup_tnl_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->trans_layer_add.size != m1->trans_layer_add.size)
    return false;

  if(memcmp(m0->trans_layer_add.buf, m1->trans_layer_add.buf, m0->trans_layer_add.size) != 0)
    return false;


  if(memcmp(m0->gtp_teid, m1->gtp_teid, 4) != 0)
    return false;


  return true;
}

