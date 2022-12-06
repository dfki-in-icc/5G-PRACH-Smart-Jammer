#ifndef FLOWS_MAPPED_TO_DRB_F1AP_MIR_H
#define FLOWS_MAPPED_TO_DRB_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "qos_flow_level_qos_parameter.h"
#include "tsc_traffic_char.h"

typedef enum{

  UL_QOS_FLOW_MAPPING_IND ,
  DL_QOS_FLOW_MAPPING_IND ,

  END_QOS_FLOW_MAPPING_IND

} qos_flow_mapping_ind_e;

typedef struct{
  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  uint8_t qfi; // INTEGER (0 ..63)

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  qos_flow_level_qos_parameter_t qos_flow_level;

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  qos_flow_mapping_ind_e*  qos_flow_mapping_ind;

  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  tsc_traffic_char_t* tsc_traffic_char;

} flows_mapped_to_drb_t;

void free_flows_mapped_to_drb(flows_mapped_to_drb_t* src);

bool eq_flows_mapped_to_drb(flows_mapped_to_drb_t const* m0, flows_mapped_to_drb_t const* m1);

#endif
