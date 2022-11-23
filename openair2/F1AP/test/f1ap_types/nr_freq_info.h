#ifndef NR_FRQUENCY_INFORMATION_MIR_H
#define NR_FRQUENCY_INFORMATION_MIR_H 

#include "sul_info.h"
#include "freq_band.h"
#include "freq_shift.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct{

//  NR Frequency Info
//  9.3.1.17

  uint32_t nr_arfcn; // max. 3279165

  sul_info_t* sul_info;

  size_t sz_frq_band; // 1 .. 1024
  freq_band_f1ap_t* frq_band; 

  freq_shift_t* freq_shift; 

} nr_freq_info_f1ap_t ;

void free_nr_freq_info_f1ap(nr_freq_info_f1ap_t* src);

bool eq_nr_freq_info_f1ap(nr_freq_info_f1ap_t const* m0, nr_freq_info_f1ap_t const* m1 );

#endif

