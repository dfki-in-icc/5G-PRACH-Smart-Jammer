#ifndef BH_RLC_CHN_FAILED_TBS_LST_F1AP_MIR_H
#define BH_RLC_CHN_FAILED_TBS_LST_F1AP_MIR_H

#include "cause_f1ap.h"

#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

typedef struct{

  // Mandatory
  // 9.3.1.113
  BIT_STRING_t bh_rlc_ch_id;

  // Optional
  // 9.3.1.2
  cause_f1ap_t* cause;

} bh_rlc_chn_failed_tbs_lst_t;



#endif

