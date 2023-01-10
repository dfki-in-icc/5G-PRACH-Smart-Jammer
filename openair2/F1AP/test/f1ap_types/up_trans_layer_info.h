#ifndef UP_TRANS_LAYER_INFO_F1AP_MIR_H
#define UP_TRANS_LAYER_INFO_F1AP_MIR_H

#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

typedef enum{
  TRANS_LAYER_UP_TRANS_LAYER_INFO,
  GTP_TEID_UP_TRANS_LAYER_INFO,

  END_UP_TRANS_LAYER_INFO,

} up_trans_layer_info_e ;

typedef struct{
 up_trans_layer_info_e type;
  // 9.3.2.1
  union{
   BIT_STRING_t trans_layer_addr; // [1, 160]
   char gtp_teid[4];
  }; 

} up_trans_layer_info_t;

#endif

