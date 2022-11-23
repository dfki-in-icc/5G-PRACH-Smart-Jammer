#include "fdd_info.h"

#include <assert.h>
#include <stdio.h>

void free_fdd_info_f1ap(fdd_info_f1ap_t* src)
{
  assert(src != NULL);

  free_nr_freq_info_f1ap(&src->ul_frq_info);

  free_nr_freq_info_f1ap(&src->dl_frq_info);

  free_trans_bw_f1ap(&src->ul_bw);

  free_trans_bw_f1ap(&src->dl_bw);

  for(size_t i = 0; i < src->sz_ul_nr_carrier_lst; ++i){
    free_nr_carrier_lst_f1ap(&src->ul_nr_carrier_lst[i]);
  }
  free(src->ul_nr_carrier_lst);

  for(size_t i = 0; i < src->sz_dl_nr_carrier_lst; ++i){
    free_nr_carrier_lst_f1ap(&src->dl_nr_carrier_lst[i]);
  }
  free(src->dl_nr_carrier_lst);
}

bool eq_fdd_info_f1ap(fdd_info_f1ap_t const* m0, fdd_info_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if(eq_nr_freq_info_f1ap(&m0->ul_frq_info, &m1->ul_frq_info) == false)
    return false;

  if(eq_nr_freq_info_f1ap(&m0->dl_frq_info, &m1->dl_frq_info) == false)
    return false;

  if(eq_trans_bw_f1ap(&m0->ul_bw, &m1->ul_bw) == false )
    return false;

  if(eq_trans_bw_f1ap(&m0->dl_bw, &m1->dl_bw) == false )
    return false;

  if(m0-> sz_ul_nr_carrier_lst != m1->sz_ul_nr_carrier_lst)
    return false;

  for(size_t i =0; i < m0-> sz_ul_nr_carrier_lst; ++i){
    if(eq_nr_carrier_lst_f1ap(&m0->ul_nr_carrier_lst[i], &m1->ul_nr_carrier_lst[i] ) == false)
      return false;
  }

  if(m0-> sz_dl_nr_carrier_lst != m1->sz_dl_nr_carrier_lst)
    return false;

  for(size_t i =0; i < m0-> sz_ul_nr_carrier_lst; ++i){
    if(eq_nr_carrier_lst_f1ap(&m0->dl_nr_carrier_lst[i], &m1->dl_nr_carrier_lst[i] ) == false)
      return false;
  }

  return true;
}

