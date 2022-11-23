#include "tdd_info.h"

#include <assert.h>
#include <stdio.h>

void free_tdd_info_f1ap(tdd_info_f1ap_t* src)
{
  assert(src != NULL);

  free_nr_freq_info_f1ap(&src->freq_info);

  free_trans_bw_f1ap(&src->bw);

  if(src->intended_tdd_dl_ul_conf != NULL){
    free_intended_tdd_dl_ul_conf(src->intended_tdd_dl_ul_conf);
    free(src->intended_tdd_dl_ul_conf);
  }

  if(src->tdd_ul_dl_conf_common != NULL){
    free_byte_array(*src->tdd_ul_dl_conf_common  );
    free(src->tdd_ul_dl_conf_common );
  }

  for(size_t i =0; i <src->sz_nr_carrier_lst; ++i){
    free_nr_carrier_lst_f1ap(&src->carrier_lst[i]);
  }
  free(src->carrier_lst);
}


bool eq_tdd_info_f1ap( tdd_info_f1ap_t const* m0, tdd_info_f1ap_t const* m1 )
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if(eq_nr_freq_info_f1ap(&m0->freq_info, &m1->freq_info) == false )
    return false;

  if(eq_trans_bw_f1ap(&m0->bw, &m1->bw) == false )
    return false;

  // optional
  assert(m0->intended_tdd_dl_ul_conf == NULL && "Not implemented");
  assert(m1->intended_tdd_dl_ul_conf == NULL && "Not implemented");

  // optional
  if(eq_byte_array(m0->tdd_ul_dl_conf_common, m1->tdd_ul_dl_conf_common) == false)
    return false;

  // optional
  assert(m0-> sz_nr_carrier_lst == 0 && "Not implemented");
  assert(m1-> sz_nr_carrier_lst == 0 && "Not implemented");

  assert(m0->carrier_lst == NULL && "Not implemented");
  assert(m1->carrier_lst == NULL && "Not implemented");

  return true;
}

