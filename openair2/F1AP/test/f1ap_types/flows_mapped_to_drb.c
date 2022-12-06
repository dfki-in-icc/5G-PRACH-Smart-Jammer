#include "flows_mapped_to_drb.h"

#include <assert.h>
#include <stdlib.h>

void free_flows_mapped_to_drb(flows_mapped_to_drb_t* src)
{
  assert(src != NULL);

  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  assert(src->qfi < 64); // INTEGER (0 ..63)

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  free_qos_flow_level_qos_parameter(&src->qos_flow_level);

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  if(src->qos_flow_mapping_ind != NULL){
    assert(*src->qos_flow_mapping_ind < END_QOS_FLOW_MAPPING_IND );
    free(src->qos_flow_mapping_ind);
  }
  
  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  if(src-> tsc_traffic_char != NULL){
    free_tsc_traffic_char(src->tsc_traffic_char );
    free(src->tsc_traffic_char );
  }

}

bool eq_flows_mapped_to_drb(flows_mapped_to_drb_t const* m0, flows_mapped_to_drb_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  assert(m0->qfi < 64); // INTEGER (0 ..63)
  assert(m1->qfi < 64); // INTEGER (0 ..63)
  if(m0->qfi != m1->qfi)
    return false;

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  if(eq_qos_flow_level_qos_parameter(&m0->qos_flow_level, &m1->qos_flow_level) == false)
    return false;

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  if(m0->qos_flow_mapping_ind != m1->qos_flow_mapping_ind){
    if(m0->qos_flow_mapping_ind == NULL || m1->qos_flow_mapping_ind == NULL) 
      return false;

    if(*m0->qos_flow_mapping_ind != *m1->qos_flow_mapping_ind)
      return false;
  }

  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  if(m0->tsc_traffic_char != m1->tsc_traffic_char ){
    if(m0->tsc_traffic_char == NULL || m1->tsc_traffic_char == NULL) 
      return false;

    if(eq_tsc_traffic_char(m0->tsc_traffic_char, m1->tsc_traffic_char) == false)
      return false;
  }

  return true;
}

