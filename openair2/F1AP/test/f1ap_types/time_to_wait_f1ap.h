#ifndef TIME_TO_WAIT_F1AP_MIR_H
#define TIME_TO_WAIT_F1AP_MIR_H

#include <stdbool.h>

typedef enum{

  V1S_TIME_TO_WAIT_F1AP = 0,
  V2S_TIME_TO_WAIT_F1AP = 1,
  V5S_TIME_TO_WAIT_F1AP = 2,
  V10S_TIME_TO_WAIT_F1AP = 3,
  V20S_TIME_TO_WAIT_F1AP = 4,
  V60S_TIME_TO_WAIT_F1AP = 5,

  END_TIME_TO_WAIT_F1AP = 6,

} time_to_wait_f1ap_e;

bool eq_time_to_wait_f1ap(time_to_wait_f1ap_e const* m0, time_to_wait_f1ap_e const* m1);

#endif


