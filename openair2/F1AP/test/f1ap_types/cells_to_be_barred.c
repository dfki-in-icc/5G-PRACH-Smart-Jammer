#include "cells_to_be_barred.h"

#include <assert.h>
#include <stdlib.h>

void free_cells_to_be_barred(cells_to_be_barred_t* src)
{
  assert(src != NULL);

  // NR CGI 9.3.1.12
  // Mandatory
  free_nr_cgi(&src->nr_cgi);

  // Cell Barred 
  // Mandatory
  // cell_barred_e cell_barred;

  // IAB Barred 
  // Optional
  assert(src->iab_barred == NULL && "Not implemented");
}

