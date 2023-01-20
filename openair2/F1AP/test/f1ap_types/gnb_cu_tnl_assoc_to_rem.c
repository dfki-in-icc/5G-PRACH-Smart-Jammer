#include "gnb_cu_tnl_assoc_to_rem.h"

#include <assert.h>

void free_gnb_cu_tnl_assoc_to_rem(gnb_cu_tnl_assoc_to_rem_t* src)
{
  assert(src != NULL);

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  free_cp_trans_layer_info(&src->tnl_assoc_trans);

  // TNL Association Transport Layer Address gNB-DU
  // Optional
 assert(src->tnl_assoc_trans_du == NULL && "Not implemented");
}

bool eq_gnb_cu_tnl_asso_to_rem(gnb_cu_tnl_assoc_to_rem_t const* m0, gnb_cu_tnl_assoc_to_rem_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  if(eq_cp_trans_layer_info(&m0->tnl_assoc_trans, &m1->tnl_assoc_trans)  == false)
    return false;

  // TNL Association Transport Layer Address gNB-DU
  // Optional
  if(eq_cp_trans_layer_info(m0->tnl_assoc_trans_du, m1->tnl_assoc_trans_du) == false )
      return false;

  return true;
}


