#ifndef ALLOCATION_RETENTION_PRIORITY_MIR_H
#define ALLOCATION_RETENTION_PRIORITY_MIR_H 

#include <stdbool.h>
#include <stdint.h>


typedef enum{

  SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY, 
  MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY,

  END_PREEMPT_CAPABILITY,

} preempt_capability_e;

typedef enum{

  NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY, 
  PRE_EMPTABLE_PREEMPT_VULNERABILITY,

  END_PREEMPT_VULNERABILITY

} preempt_vulnerability_e;


typedef struct{
  // Mandatory
  // Priority Level
  uint8_t prio_level; // INTEGER (0..15)
                      
  // Mandatory
  // Pre-emption Capability
  preempt_capability_e preempt_capability; 

  // Mandatory
  // Pre-emption Vulnerability
  preempt_vulnerability_e preempt_vulnerability; 

} alloc_retention_prio_t;

bool eq_alloc_retention_prio(alloc_retention_prio_t const* m0, alloc_retention_prio_t const* m1);

#endif
