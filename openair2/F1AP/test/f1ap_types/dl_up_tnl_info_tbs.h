#ifndef DL_UP_TRANS_LAYER_INFO_F1AP_MIR_H
#define DL_UP_TRANS_LAYER_INFO_F1AP_MIR_H

#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"
#include <stdbool.h>

typedef struct{
  // 9.3.2.1
   BIT_STRING_t trans_layer_addr; // [1, 160]
   char gtp_teid[4];
} dl_up_trans_layer_info_t;

void free_dl_up_trans_layer_info( dl_up_trans_layer_info_t* src);

bool eq_dl_up_trans_layer_info( dl_up_trans_layer_info_t const* m0, dl_up_trans_layer_info_t const* m1);

#endif

