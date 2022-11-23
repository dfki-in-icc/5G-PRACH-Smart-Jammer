#include "plmn.h"

#include <stdlib.h>

bool eq_plmn_f1ap( plmn_f1ap_t const* m0, plmn_f1ap_t const* m1)
{
  if(m0 == m1 )
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(m0->mcc != m1->mcc)
    return false;

  if(m0->mnc != m1->mnc)
    return false;

  if(m0->mnc_digit_len != m1->mnc_digit_len)
    return false;

  return true;
}

