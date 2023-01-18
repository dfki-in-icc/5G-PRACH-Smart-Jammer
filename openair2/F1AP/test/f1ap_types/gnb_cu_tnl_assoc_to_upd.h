#ifndef F1AP_GNB_CU_TNL_ASSOC_TO_UPDATE
#define F1AP_GNB_CU_TNL_ASSOC_TO_UPDATE

#include "tnl_assoc_usage.h"
#include "cp_trans_layer_info.h"

typedef struct{
  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  cp_trans_layer_info_t tnl_assoc_trans_addr;

  // TNL Association Usage
  // Optional
 tnl_assoc_usage_e* tnl_association_usage;

} gnb_cu_tnl_assoc_to_upd_t;

void free_gnb_cu_tnl_assoc_to_upd( gnb_cu_tnl_assoc_to_upd_t* src);

#endif

