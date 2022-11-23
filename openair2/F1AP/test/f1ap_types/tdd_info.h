#ifndef F1AP_TDD_INFO_MIR_H
#define F1AP_TDD_INFO_MIR_H 

#include <stdint.h>
#include <stdlib.h>

#include "nr_freq_info.h"
#include "trans_bw.h"
#include "intented_tdd_dl_ul_conf.h"
#include "../byte_array.h"
#include "nr_carrier_lst.h"

typedef struct{

  nr_freq_info_f1ap_t freq_info;

  trans_bw_f1ap_t bw; 

  // optional
  intended_tdd_dl_ul_conf_t* intended_tdd_dl_ul_conf;

  // optional
  byte_array_t* tdd_ul_dl_conf_common;

  // optional
  size_t sz_nr_carrier_lst;
  nr_carrier_lst_f1ap_t* carrier_lst;

} tdd_info_f1ap_t;

void free_tdd_info_f1ap(tdd_info_f1ap_t* src);

bool eq_tdd_info_f1ap( tdd_info_f1ap_t const* m0, tdd_info_f1ap_t const* m1 );


#endif

