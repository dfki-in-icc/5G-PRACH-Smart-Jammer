#ifndef TSC_TRAFFIC_CHARACTERISTICS_F1AP_MIR_H
#define TSC_TRAFFIC_CHARACTERISTICS_F1AP_MIR_H 

#include "tsc_assistance_info.h"

typedef struct{

  // Optional
  // TSC Assistance Information Downlink 9.3.1.142
  tsc_assistance_info_t* down;

  // Optional
  // TSC Assistance Information Uplink 9.3.1.142
  tsc_assistance_info_t* up;

} tsc_traffic_char_t;

void free_tsc_traffic_char(tsc_traffic_char_t* src);

bool eq_tsc_traffic_char(tsc_traffic_char_t const* m0, tsc_traffic_char_t const* m1);


#endif

