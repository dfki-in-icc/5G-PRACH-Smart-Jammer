#include "nr_carrier_lst.h"

#include <assert.h>
#include <stdlib.h>

void free_nr_carrier_lst_f1ap( nr_carrier_lst_f1ap_t* src )
{
  assert(src != NULL);

  // No heap allocated
  (void)src;
}

bool eq_nr_carrier_lst_f1ap( nr_carrier_lst_f1ap_t const* m0,  nr_carrier_lst_f1ap_t const* m1)
{
  assert(0!=0 && "Not implemented");

  return false;
}


