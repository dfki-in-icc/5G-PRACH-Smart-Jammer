#include "qos_flow_level_qos_parameter.h"

#include <assert.h>
#include <stdlib.h>

void free_qos_flow_level_qos_parameter( qos_flow_level_qos_parameter_t* src)
{
  assert(src != NULL);

  //Mandatory 
  // CHOICE QoS Characteristics
  assert(src->qos_characteristic < END_5QI_DESCRIPTOR);
  if(src->qos_characteristic == NON_DYN_5QI_DESCRIPTOR){
    free_non_dyn_5qi_descriptor(&src->non_dyn);
  } else if(src->qos_characteristic == DYN_5QI_DESCRIPTOR){
    free_dyn_5qi_descriptor(&src->dyn);
  } else {
    assert(0!=0 && "Unknown type" );
  }

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
  // alloc_retention_prio_t alloc_retention_prio; 

  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  if(src->gbr_qos_flow_info != NULL)
    free_gbr_qos_flow_info(src->gbr_qos_flow_info);

  // Optional 
  // Reflective QoS Attribute
  if(src->reflective_qos_attr != NULL)
    free(src->reflective_qos_attr);
//  reflective_qos_attr_e* reflective_qos_attr;

  // Optional 
  // PDU Session ID
  if(src->pdu_session_id != NULL)
    free(src->pdu_session_id);
//  uint8_t* pdu_session_id;

  // Optional
  //UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  if(src->ul_pdu_session_aggr_max_bit_rate != NULL)
    free(src->ul_pdu_session_aggr_max_bit_rate);
//  uint64_t* ul_pdu_session_aggr_max_bit_rate; 

  // Optional
  // QoS Monitoring Request
  if(src->qos_monitoring_request != NULL)
    free(src->qos_monitoring_request);
//   qos_monitoring_request_e* qos_monitoring_request;

}

bool eq_qos_flow_level_qos_parameter(qos_flow_level_qos_parameter_t const* m0, qos_flow_level_qos_parameter_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  //Mandatory 
  // CHOICE QoS Characteristics
  assert(m0->qos_characteristic < END_5QI_DESCRIPTOR);
  assert(m1->qos_characteristic < END_5QI_DESCRIPTOR);
  if(m0->qos_characteristic == NON_DYN_5QI_DESCRIPTOR){

    if(eq_non_dyn_5qi_descriptor(&m0->non_dyn, &m1->non_dyn ) == false)
      return false;

  } else if(m0->qos_characteristic == DYN_5QI_DESCRIPTOR){
    
    if(eq_dyn_5qi_descriptor(&m0->dyn, &m1->dyn) == false)
      return false;

  } else {
    assert(0!=0 && "Unknown type" );
  }

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
  if(eq_alloc_retention_prio(&m0->alloc_retention_prio, &m1->alloc_retention_prio) == false)
    return false;


  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  if(eq_gbr_qos_flow_info(m0->gbr_qos_flow_info, m1->gbr_qos_flow_info) == false )
    return false;

  // Optional 
  // Reflective QoS Attribute
  if(m0->reflective_qos_attr != m1->reflective_qos_attr){
    if(m0->reflective_qos_attr == NULL || m1->reflective_qos_attr == NULL)
      return false;

    if(*m0->reflective_qos_attr != *m1->reflective_qos_attr)
      return false;
  }

  // Optional 
  // PDU Session ID
  // uint8_t* pdu_session_id;
  if(m0->pdu_session_id != m1->pdu_session_id ){
    if(m0->pdu_session_id == NULL || m1->pdu_session_id == NULL)
      return false;

    if(*m0->pdu_session_id != *m1->pdu_session_id )
      return false;
  }

  // Optional
  // UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  // uint64_t* ul_pdu_session_aggr_max_bit_rate; 
  if(m0->ul_pdu_session_aggr_max_bit_rate != m1->ul_pdu_session_aggr_max_bit_rate ){
    if(m0->ul_pdu_session_aggr_max_bit_rate == NULL || m1->ul_pdu_session_aggr_max_bit_rate == NULL)
      return false;

    if(*m0->ul_pdu_session_aggr_max_bit_rate != *m1->ul_pdu_session_aggr_max_bit_rate )
      return false;
  }

  // Optional
  // QoS Monitoring Request
  // qos_monitoring_request_e* qos_monitoring_request;
  if(m0->qos_monitoring_request != m1->qos_monitoring_request ){
    if(m0->qos_monitoring_request == NULL || m1->qos_monitoring_request == NULL)
      return false;

    if(*m0->qos_monitoring_request != *m1->qos_monitoring_request )
      return false;
  }

  return true;
}

