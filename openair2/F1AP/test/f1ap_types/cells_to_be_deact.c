#include "cells_to_be_deact.h"
#include <stdlib.h>

bool eq_cells_to_be_deact(cells_to_be_deact_t const* m0, cells_to_be_deact_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // NR CGI 9.3.1.12
  if(eq_nr_cgi(&m0->nr_cgi, &m1->nr_cgi) == false)
    return false;

  return true;
}

