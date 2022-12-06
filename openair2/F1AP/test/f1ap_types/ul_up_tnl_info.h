#ifndef UL_UP_TNL_INFORMATION_F1AP_MIR_H
#define UL_UP_TNL_INFORMATION_F1AP_MIR_H 

#include <stdbool.h>

#include "../byte_array.h"
#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

typedef struct{

  // Mandatory
  // Transport Layer Address
  BIT_STRING_t tla; // [1, 160]

  // Mandatory
  // 9.3.2.2
  char gtp_teid[4];
} ul_up_tnl_info_t;

void free_ul_up_tnl_info(ul_up_tnl_info_t* src);

bool eq_ul_up_tnl_info(ul_up_tnl_info_t const* m0, ul_up_tnl_info_t const* m1);

#endif

