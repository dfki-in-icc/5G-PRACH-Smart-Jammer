#ifndef TSC_ASSISTANCE_INFORMATION_F1AP_MIR_H
#define TSC_ASSISTANCE_INFORMATION_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "../byte_array.h"

typedef struct{

  // Mandatory
  // Periodicity 9.3.1.143
  uint32_t periodicity; // INTEGER (0..640000,

  // Optional
  // Burst Arrival Time 9.3.1.144
  byte_array_t* burst_arrival_time;

} tsc_assistance_info_t;

void free_tsc_assistance_info(tsc_assistance_info_t* src);

bool eq_tsc_assistance_info(tsc_assistance_info_t const* m0, tsc_assistance_info_t const* m1);

#endif

