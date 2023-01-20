#include "gnb_cu_tnl_assoc_to_upd.h"

#include <assert.h>

void free_gnb_cu_tnl_assoc_to_upd( gnb_cu_tnl_assoc_to_upd_t* src)
{
  assert(src != NULL);

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  free_cp_trans_layer_info(&src->tnl_assoc_trans_addr);

  // TNL Association Usage
  // Optional
 assert(src->tnl_association_usage == NULL);
}

bool eq_gnb_cu_tnl_asso_to_upd(gnb_cu_tnl_assoc_to_upd_t const* m0, gnb_cu_tnl_assoc_to_upd_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  if(eq_cp_trans_layer_info(&m0->tnl_assoc_trans_addr, &m1->tnl_assoc_trans_addr) == false)
    return false;

  // TNL Association Usage
  // Optional
  if(eq_tnl_assoc_usage(m0->tnl_association_usage, m1->tnl_association_usage) == false)
    return false;

  return true;
}


