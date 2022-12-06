#include "drb_info.h"

#include <assert.h>
#include <stdlib.h>

void free_drb_info(drb_info_t* src)
{
  assert(src != NULL);

  // Mandatory
  // DRB QoS 9.3.1.45
  free_qos_flow_level_qos_parameter(&src->drb_qos);

  // Mandatory
  // S-NSSAI 9.3.1.38
  free_s_nssai(&src->nssai);

  // Optional
  // Notification Control 9.3.1.56
  if(src->notification_ctrl != NULL)
    free(src->notification_ctrl);

  // Flows Mapped to DRB
  assert(src->sz_flows_mapped_to_drb < 65);
  for(size_t i = 0; i < src->sz_flows_mapped_to_drb; ++i){
    free_flows_mapped_to_drb(&src->flows_mapped_to_drb[i]);
  }
  if(src->flows_mapped_to_drb != NULL){
    free(src->flows_mapped_to_drb);
  }

}

bool eq_drb_info(drb_info_t const* m0, drb_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // DRB QoS 9.3.1.45
  if(eq_qos_flow_level_qos_parameter(&m0->drb_qos, &m1->drb_qos) == false)
    return false;

  // Mandatory
  // S-NSSAI 9.3.1.38
  if(eq_s_nssai(&m0->nssai, &m1->nssai) == false)
    return false;

  // Optional
  // Notification Control 9.3.1.56
  if(m0->notification_ctrl != m1->notification_ctrl){
    if(m0->notification_ctrl == NULL ||  m1->notification_ctrl == NULL)
      return false;

    if(*m0->notification_ctrl != *m1->notification_ctrl)
      return false;
  }

  // Flows Mapped to DRB
  assert(m0->sz_flows_mapped_to_drb < 65);
  assert(m1->sz_flows_mapped_to_drb < 65);
  if(m0->sz_flows_mapped_to_drb != m1->sz_flows_mapped_to_drb)
    return false;

  for(size_t i = 0; i < m0->sz_flows_mapped_to_drb; ++i){
    if(eq_flows_mapped_to_drb(&m0->flows_mapped_to_drb[i], &m1->flows_mapped_to_drb[i]) == false)
      return false;
  }

  return true;
}

