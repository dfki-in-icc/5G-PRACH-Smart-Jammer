#include "trans_bw.h"

#include <assert.h>
#include <stdlib.h>

void free_trans_bw_f1ap(trans_bw_f1ap_t* src)
{
  assert(src != NULL);

  // No heap memory allocated
  (void)src;
}

bool eq_trans_bw_f1ap(trans_bw_f1ap_t const* m0, trans_bw_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if(m0->n_rb != m1->n_rb)
    return false;

  if(m0->nr_scs != m1->nr_scs)
    return false;

  return true;
}

