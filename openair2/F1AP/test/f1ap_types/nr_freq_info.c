#include "nr_freq_info.h"

#include <assert.h>
#include <stdlib.h>

void free_nr_freq_info_f1ap(nr_freq_info_f1ap_t* src)
{
  assert(src != NULL);

  free(src->sul_info);

  for(int i = 0; i < src->sz_frq_band; ++i){
    free_freq_band(&src->frq_band[i]); 
  }
  free(src->frq_band);

  if(src->freq_shift != NULL){
    free_frq_shift(src->freq_shift);
    free(src->freq_shift);
  }
}


bool eq_nr_freq_info_f1ap(nr_freq_info_f1ap_t const* m0, nr_freq_info_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if(m0->nr_arfcn != m1->nr_arfcn)
    return false;

  assert(m0->sul_info == NULL && "Not implemented");
  assert(m1->sul_info == NULL && "Not implemented");

  assert(m0->sz_frq_band > 0 && m0->sz_frq_band < 1025);
  assert(m1->sz_frq_band > 0 && m1->sz_frq_band < 1025);
  if(m0->sz_frq_band != m1->sz_frq_band)
    return false;

  for(size_t i = 0; i < m0->sz_frq_band; ++i){
    if(eq_freq_band_f1ap(&m0->frq_band[i], &m1->frq_band[i] ) == false)
      return false;
  }

  assert(m0->freq_shift == NULL && "Not implemented");
  assert(m1->freq_shift == NULL && "Not implemented");

  return true;
}
