#ifndef F1AP_NR_CGI_MIR_H
#define F1AP_NR_CGI_MIR_H 

#include "plmn.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct{
  plmn_f1ap_t plmn_id;
  uint64_t nr_cell_id; // 36 bits
} nr_cgi_t;

bool eq_nr_cgi( nr_cgi_t const* m0, nr_cgi_t const* m1);

#endif


