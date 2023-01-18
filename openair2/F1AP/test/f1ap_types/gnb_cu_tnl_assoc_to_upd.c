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

