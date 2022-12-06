#ifndef DRX_CYCLE_F1AP_MIR_H
#define DRX_CYCLE_F1AP_MIR_H 

#include <stdbool.h>
#include <stdint.h>

#include "long_drx_cycle_len.h"
#include "short_drx_cycle_len.h"

typedef struct{
  // Mandatory 
  // Long DRX Cycle Length
  long_drx_cycle_len_e long_drx_cycle_len;

  // Optional 
  // Short DRX Cycle Length
  short_drx_cycle_len_e* short_drx_cycle_len;

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  uint8_t* short_drx_cycle_timer;

} drx_cycle_t;

void free_drx_cycle(drx_cycle_t * src);

bool eq_drx_cycle(drx_cycle_t const* m0, drx_cycle_t const* m1);

#endif

