#ifndef QOS_FLOW_LEVEL_QOS_PARAMETER_MIR_H
#define QOS_FLOW_LEVEL_QOS_PARAMETER_MIR_H 

#include <stdbool.h>

#include "non_dyn_5qi_descriptor.h"
#include "dyn_5qi_descriptor.h"
#include "gbr_qos_flow_info.h"
#include "alloc_retention_prio.h"

typedef enum{

  NON_DYN_5QI_DESCRIPTOR ,
  DYN_5QI_DESCRIPTOR ,

  END_5QI_DESCRIPTOR 

} qos_characteristic_e;

typedef enum{
// Details in TS 23.501 [21]. This
// IE applies to non-GBR flows only
// and is ignored otherwise.

  SUBJECT_TO_REFLECTIVE_QOS_ATTR , 
  END_REFLECTIVE_QOS_ATTR , 

} reflective_qos_attr_e; 

typedef enum{

  UL_QOS_MONITORING_REQUEST, 
  DL_QOS_MONITORING_REQUEST, 
  BOTH_QOS_MONITORING_REQUEST,

  END_QOS_MONITORING_REQUEST,

} qos_monitoring_request_e;


// QoS Flow Level QoS Parameters 9.3.1.45
typedef struct{
  //Mandatory 
  // CHOICE QoS Characteristics
  qos_characteristic_e  qos_characteristic;
  union{
    // Non Dynamic 5QI Descriptor 9.3.1.49
    non_dyn_5qi_descriptor_t non_dyn;
    // Dynamic 5QI Descriptor 9.3.1.47
    dyn_5qi_descriptor_t dyn;
  };

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
   alloc_retention_prio_t alloc_retention_prio; 

  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  gbr_qos_flow_info_t* gbr_qos_flow_info;

  // Optional 
  // Reflective QoS Attribute
  reflective_qos_attr_e* reflective_qos_attr;

  // Optional 
  // PDU Session ID
  uint8_t* pdu_session_id;

  // Optional
  //UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  uint64_t* ul_pdu_session_aggr_max_bit_rate; 

  // Optional
  // QoS Monitoring Request
   qos_monitoring_request_e* qos_monitoring_request;

} qos_flow_level_qos_parameter_t; 

void free_qos_flow_level_qos_parameter( qos_flow_level_qos_parameter_t* src);

bool eq_qos_flow_level_qos_parameter(qos_flow_level_qos_parameter_t const* m0, qos_flow_level_qos_parameter_t const* m1);

#endif

