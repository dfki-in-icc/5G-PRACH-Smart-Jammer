#ifndef F1AP_GNB_CU_TNL_ASSOCIATION_TO_ADD_H
#define F1AP_GNB_CU_TNL_ASSOCIATION_TO_ADD_H

#include "tnl_assoc_usage.h"
#include "cp_trans_layer_info.h"

#include <stdbool.h>

typedef struct{
  // TNL Association Transport Layer Information 9.3.2.4
  // Mandatory
  cp_trans_layer_info_t cp_trans_layer_info;

  // TNL Association Usage  
  // Mandatory
  tnl_assoc_usage_e tnl_assoc_usage;

} gnb_cu_tnl_asso_to_add_t;

void free_gnb_cu_tnl_asso_to_add(gnb_cu_tnl_asso_to_add_t* src);

bool eq_gnb_cu_tnl_asso_to_add(gnb_cu_tnl_asso_to_add_t const* m0, gnb_cu_tnl_asso_to_add_t const* m1);

#endif
