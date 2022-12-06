#ifndef NON_DYNAMIC_5QI_DESCRIPTOR_MIR_H
#define NON_DYNAMIC_5QI_DESCRIPTOR_MIR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct{
  // Mandatory
  //  5QI
  uint8_t five_qi;

  // Optional
  // Priority Level
  uint8_t* prio_level; // [1,127]

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  uint16_t* av_window;

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  uint32_t* max_data_burst_vol; // INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  uint16_t* pkt_delay_budget_downlink; // INTEGER (0..65535, …)

  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  uint16_t* pkt_delay_budget_uplink; // INTEGER (0..65535, …)

} non_dyn_5qi_descriptor_t;

void free_non_dyn_5qi_descriptor(non_dyn_5qi_descriptor_t* src);

bool eq_non_dyn_5qi_descriptor(non_dyn_5qi_descriptor_t const* m0, non_dyn_5qi_descriptor_t const* m1);


#endif
