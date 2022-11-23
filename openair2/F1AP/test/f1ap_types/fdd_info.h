#ifndef F1AP_FDD_INFO_MIR_H
#define F1AP_FDD_INFO_MIR_H 

#include "nr_freq_info.h"
#include "nr_carrier_lst.h"
#include "trans_bw.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct{

  nr_freq_info_f1ap_t ul_frq_info;
  nr_freq_info_f1ap_t dl_frq_info;

  trans_bw_f1ap_t ul_bw;
  trans_bw_f1ap_t dl_bw;


  size_t sz_ul_nr_carrier_lst; // max. 5
  nr_carrier_lst_f1ap_t* ul_nr_carrier_lst;

  size_t sz_dl_nr_carrier_lst;
  nr_carrier_lst_f1ap_t* dl_nr_carrier_lst;

} fdd_info_f1ap_t ;

void free_fdd_info_f1ap(fdd_info_f1ap_t* src);

bool eq_fdd_info_f1ap(fdd_info_f1ap_t const* m0, fdd_info_f1ap_t const* m1);

#endif

