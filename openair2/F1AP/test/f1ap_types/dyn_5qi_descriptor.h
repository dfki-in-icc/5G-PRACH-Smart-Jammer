#ifndef DYNAMIC_5QI_DESCRIPTOR_MIR_H
#define DYNAMIC_5QI_DESCRIPTOR_MIR_H 

#include "pkt_error_rate.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum{

  DELAY_CRITICAL,
  NON_DELAY_CRITICAL,

  END_DELAY_CRITICAL,

} delay_critical_e; 

// Dynamic 5QI Descriptor 9.3.1.47
typedef struct{

  //Mandatory
  //QoS Priority Level
  uint8_t qos_prio_level; // INTEGER (1..127)

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  uint16_t pkt_delay_budget; // INTEGER (0..1023, ...)

  // Mandatory
  // Packet Error Rate 9.3.1.52
  pkt_error_rate_t pkt_error_rate;

  // Optional
  // 5QI
  uint8_t* five_qi;

  // C-ifGBRflow
  // Delay Critical
  delay_critical_e* delay_critical;

  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  uint16_t* av_window; // INTEGER (0..4095, ...)

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  uint64_t* max_data_burst_vol; //INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  uint16_t* ext_pkt_delay_budget; //INTEGER (0..65535, …) 

  // Optional 
  // CN Packet Delay Budget Downlink
  uint16_t* cn_pkt_dalay_budget_downlink; 

  // Optional
  // CN Packet Delay Budget Uplink
  uint16_t* cn_pkt_dalay_budget_uplink; 

} dyn_5qi_descriptor_t;

void free_dyn_5qi_descriptor(dyn_5qi_descriptor_t* src);

bool eq_dyn_5qi_descriptor(dyn_5qi_descriptor_t const* m0, dyn_5qi_descriptor_t const* m1);

#endif

