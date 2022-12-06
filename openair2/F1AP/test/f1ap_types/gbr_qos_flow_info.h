#ifndef GUARANTEED_BIT_RATE_QOS_FLOW_INFO_F1AP_MIR_H
#define GUARANTEED_BIT_RATE_QOS_FLOW_INFO_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>

typedef struct{
  // Mandatory
  // Maximum Flow Bit Rate Downlink 9.3.1.22
  uint64_t max_flow_bit_rate_downlink; //INTEGER (0..4,000,000,000,000,...)
                                       
  // Mandatory
  // Maximum Flow Bit Rate Uplink 9.3.1.22
  uint64_t max_flow_bit_rate_uplink; //INTEGER (0..4,000,000,000,000,...)
                                       //
  // Mandatory
  // Guaranteed Flow Bit Rate Downlink 9.3.1.22
  uint64_t gbr_flow_bit_rate_downlink; //INTEGER (0..4,000,000,000,000,...)

  //Mandatory
  //Guaranteed Flow Bit Rate Uplink 9.3.1.22
  uint64_t  gbr_flow_bit_rate_uplink; //INTEGER (0..4,000,000,000,000,...)

  //Optional
  //Maximum Packet Loss Rate Downlink 9.3.1.50
  uint16_t* max_pkt_loss_rate_downlink; //INTEGER(0..1000)

  //Optional
  //Maximum Packet Loss Rate Uplink 9.3.1.50
  uint16_t* max_pkt_loss_rate_uplink; //INTEGER(0..1000)
  //Optional 
  //Alternative QoS Parameters Set List 9.3.1.50
  uint16_t* alternative_qos_param; //INTEGER(0..1000)

} gbr_qos_flow_info_t;

void free_gbr_qos_flow_info( gbr_qos_flow_info_t* src);

bool eq_gbr_qos_flow_info(gbr_qos_flow_info_t const* m0, gbr_qos_flow_info_t const* m1);

#endif
