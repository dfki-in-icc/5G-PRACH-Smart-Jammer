#include "freq_band.h"

#include <assert.h>

void free_freq_band(freq_band_f1ap_t* src)
{
  assert(src != NULL);

  free(src->sul_band);
}

bool eq_freq_band_f1ap(freq_band_f1ap_t const* m0, freq_band_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if(m0-> frq_band != m1->frq_band)
    return false;

  if(m0->sz_sul_band != m1->sz_sul_band)
    return false;

  for(size_t i = 0; i < m0->sz_sul_band; ++i){
    if(m0->sul_band[i] != m1->sul_band[i])
      return false;
  }

  return true;
}

