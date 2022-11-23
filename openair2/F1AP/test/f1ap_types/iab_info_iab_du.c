#include "iab_info_iab_du.h"

#include <assert.h>
#include <stdlib.h>

void free_iab_info_iab_du_f1ap(iab_info_iab_du_f1ap_t* src)
{
  assert(src != NULL);

  // No heap allocated
  (void)src;
}

bool eq_iab_info_iab_du_f1ap(iab_info_iab_du_f1ap_t const* m0, iab_info_iab_du_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  assert(0!=0 && "not implemented");
  return false;
}

