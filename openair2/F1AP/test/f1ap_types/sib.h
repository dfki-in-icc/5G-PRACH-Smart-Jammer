#ifndef F1AP_SIB_MIR_H
#define F1AP_SIB_MIR_H 


#include "../byte_array.h"
#include "sib_area_scope.h"
#include <stdint.h>



typedef struct{

  // mandatory
  uint8_t type; // indicates a certain SIB block,
                // e.g. 2 means sibType2, 3 for
                // sibType3, etc. Values 6, 7, 8
                // and values 10 and higher are
                // not applicable in this version
                // of the specifications.

  // mandatory
  byte_array_t msg;

  // mandatory
  uint8_t tag; // INTEGER (0..31, ...)

  // optional
  sib_area_scope_t* area_scope;

} sib_t ;

void free_sib(sib_t* src);

bool eq_sib(sib_t const* m0, sib_t const* m1);

#endif

