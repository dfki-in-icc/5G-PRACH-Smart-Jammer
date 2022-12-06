#ifndef E_UTRAN_QOS_MIR_H
#define E_UTRAN_QOS_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "alloc_retention_prio.h"
#include "gbr_qos_info.h"

typedef struct{
  // Mandatory
  // QCI
  uint8_t qci;

  // Mandatory
  // Allocation and Retention Priority
  alloc_retention_prio_t alloc; 

  // Optional
  // GBR QoS Information 9.3.1.21
  gbr_qos_info_t* gbr_qos_info; 

} e_utran_qos_t;

void free_e_utran_qos( e_utran_qos_t* src);

bool eq_e_utran_qos( e_utran_qos_t const* m0, e_utran_qos_t const* m1);

#endif

