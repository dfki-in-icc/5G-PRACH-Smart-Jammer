#include "alloc_retention_prio.h"

#include <assert.h>
#include <stdlib.h>

bool eq_alloc_retention_prio(alloc_retention_prio_t const* m0, alloc_retention_prio_t const* m1)
{

  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // Priority Level
  assert(m0->prio_level < 16);
  assert(m1->prio_level < 16);
  if(m0->prio_level != m1->prio_level)
    return false;
                      
  // Mandatory
  // Pre-emption Capability
  if(m0->preempt_capability != m1->preempt_capability)
    return false;

  // Mandatory
  // Pre-emption Vulnerability
  if(m0->preempt_vulnerability != m1->preempt_vulnerability)
    return false;

  return true;
}

