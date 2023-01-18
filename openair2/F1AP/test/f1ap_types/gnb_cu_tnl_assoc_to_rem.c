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

