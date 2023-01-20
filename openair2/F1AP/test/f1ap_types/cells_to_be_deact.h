#ifndef F1AP_CELLS_TO_BE_DEACTIVATED_H
#define F1AP_CELLS_TO_BE_DEACTIVATED_H 

#include "nr_cgi.h"
#include <stdbool.h>

typedef struct {
  // NR CGI 9.3.1.12
  nr_cgi_t nr_cgi;

} cells_to_be_deact_t;

bool eq_cells_to_be_deact( cells_to_be_deact_t const* m0, cells_to_be_deact_t const* m1);


#endif

