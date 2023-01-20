#ifndef F1AP_TNL_ASSOCIATION_USAGE_H
#define F1AP_TNL_ASSOCIATION_USAGE_H

#include <stdbool.h>

typedef enum{
  UE_TNL_ASSOC_USAGE,
  NON_UE_TNL_ASSOC_USAGE ,
  BOTH_TNL_ASSOC_USAGE ,

  END_TNL_ASSOC_USAGE

} tnl_assoc_usage_e; 

bool eq_tnl_assoc_usage( tnl_assoc_usage_e const* m0, tnl_assoc_usage_e const* m1);

#endif
