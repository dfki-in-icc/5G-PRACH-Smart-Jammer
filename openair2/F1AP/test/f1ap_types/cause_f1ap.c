#include "cause_f1ap.h"

#include <assert.h>
#include <stdlib.h>

bool eq_cause_f1ap(cause_f1ap_t const* m0, cause_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(m0->type != m1->type)
    return false;

  assert(m0->type < END_CAUSE_F1AP);
  assert(m1->type < END_CAUSE_F1AP);

  if(m0->type == RADIO_NETWORK_CAUSE_F1AP ){

    if(m0->radio != m1->radio)
      return false;

  } else if(m0->type == TRANSPORT_CAUSE_F1AP){

      if(m0->trans != m1->trans)
       return false;

  } else if(m0->type == PROTOCOL_CAUSE_F1AP) {
  
    if(m0->proto != m1->proto)
       return false;

  } else if(m0->type == MISC_CAUSE_F1AP){

    if(m0->misc != m1->misc)
       return false;

  } else {
    assert(0!= 0 && "Unknown type");
  }

  return true;
}
