#ifndef GURANTEED_BIT_RATE_QOS_INFORMATION_MIR_H
#define GURANTEED_BIT_RATE_QOS_INFORMATION_MIR_H 

#include <stdbool.h>
#include <stdint.h>

// GBR QoS Information 9.3.1.21
typedef struct{

  // Mandatory
  //E-RAB Maximum Bit Rate Downlink
  uint64_t e_rab_max_bit_rate_downlink; // max. 4 x 10^12  

  // Mandatory
  // E-RAB Maximum Bit Rate Uplink
  uint64_t e_rab_max_bit_rate_upwnlink; // max. 4 x 10^12   

  // Mandatory
  // E-RAB Guaranteed Bit Rate Downlink
  uint64_t e_rab_guaranteed_bit_rate_downlink;  // max. 4 x 10^12 

  // Mandatory
  // E-RAB Guaranteed Bit Rate Uplink
  uint64_t e_rab_guaranteed_bit_rate_uplink;  // max. 4 x 10^12 

} gbr_qos_info_t; 

bool eq_gbr_qos_info( gbr_qos_info_t const* m0,  gbr_qos_info_t const* m1);

#endif

