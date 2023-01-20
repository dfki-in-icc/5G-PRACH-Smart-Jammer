#include "gnb_cu_tnl_assoc_to_add.h"

#include <assert.h>
#include <stdlib.h>


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

bool eq_gnb_cu_tnl_asso_to_add(gnb_cu_tnl_asso_to_add_t const* m0, gnb_cu_tnl_asso_to_add_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // TNL Association Transport Layer Information 9.3.2.4
  // Mandatory
  if(eq_cp_trans_layer_info(&m0->cp_trans_layer_info, &m1->cp_trans_layer_info) == false)
    return false;

  // TNL Association Usage  
  // Mandatory
  if(eq_tnl_assoc_usage(&m0->tnl_assoc_usage, &m1->tnl_assoc_usage) == false) 
    return false;

  return true;
}

