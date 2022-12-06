#include "e_utran_qos.h"

#include <assert.h>
#include <stdlib.h>

void free_e_utran_qos( e_utran_qos_t* src)
{
  assert(src != NULL);

  // Mandatory
  // QCI
  //uint8_t qci;

  // Mandatory
  // Allocation and Retention Priority
  //alloc_retention_prio_t alloc; 

  // Optional
  // GBR QoS Information 9.3.1.21
  if(src->gbr_qos_info != NULL){
    free(src->gbr_qos_info);
  }
}

bool eq_e_utran_qos(e_utran_qos_t const* m0, e_utran_qos_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // QCI
  if(m0-> qci != m1->qci)
    return false;

  // Mandatory
  // Allocation and Retention Priority
  if(eq_alloc_retention_prio(&m0->alloc, &m1->alloc) == false)
    return false;

  // Optional
  // GBR QoS Information 9.3.1.21
  if(eq_gbr_qos_info( m1->gbr_qos_info, m0->gbr_qos_info  ) == false)
    return false;

  return true;
}

