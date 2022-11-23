#ifndef F1AP_FREQUENCY_BAND_MIR_H
#define F1AP_FREQUENCY_BAND_MIR_H 

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct{

  uint16_t frq_band;

  size_t sz_sul_band;
  uint16_t* sul_band;

} freq_band_f1ap_t ;

void free_freq_band(freq_band_f1ap_t* src);

bool eq_freq_band_f1ap(freq_band_f1ap_t const* m0, freq_band_f1ap_t const* m1);

#endif

