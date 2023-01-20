#ifndef F1AP_GNB_CU_TNL_ASSOC_TO_REM_H 
#define F1AP_GNB_CU_TNL_ASSOC_TO_REM_H  


#include "cp_trans_layer_info.h"

typedef struct{

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
 cp_trans_layer_info_t tnl_assoc_trans;

  // TNL Association Transport Layer Address gNB-DU
  // Optional
 cp_trans_layer_info_t* tnl_assoc_trans_du;

} gnb_cu_tnl_assoc_to_rem_t;

void free_gnb_cu_tnl_assoc_to_rem( gnb_cu_tnl_assoc_to_rem_t* src);

bool eq_gnb_cu_tnl_asso_to_rem(gnb_cu_tnl_assoc_to_rem_t const* m0, gnb_cu_tnl_assoc_to_rem_t const* m1);

#endif

