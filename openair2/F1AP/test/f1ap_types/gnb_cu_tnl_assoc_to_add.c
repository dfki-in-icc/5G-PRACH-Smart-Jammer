#include "gnb_cu_tnl_assoc_to_add.h"

#include <assert.h>


void free_gnb_cu_tnl_asso_to_add(gnb_cu_tnl_asso_to_add_t* src)
{
  assert(src != NULL);
  // TNL Association Transport Layer Information 9.3.2.4
  // Mandatory
  free_cp_trans_layer_info(&src->cp_trans_layer_info);

  // TNL Association Usage  
  // Mandatory
  // tnl_assoc_usage_e tnl_assoc_usage;

}

