#ifndef S_NSSAI_MIR_H
#define S_NSSAI_MIR_H 

#include "../byte_array.h"

#include <stdint.h>

typedef struct{
  uint8_t sst;
  byte_array_t* sd; // size 3 bytes
} s_nssai_t;

void free_s_nssai(s_nssai_t * src);

bool eq_s_nssai( s_nssai_t const* m0, s_nssai_t const* m1);

#endif

