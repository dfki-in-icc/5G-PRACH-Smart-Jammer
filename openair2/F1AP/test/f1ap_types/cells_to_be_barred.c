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

bool eq_cell_barred(cell_barred_e const* m0, cell_barred_e const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL )
    return false;

  if(*m0 != *m1)
    return false;

  return true;
}


bool eq_cells_to_be_barred(cells_to_be_barred_t const* m0, cells_to_be_barred_t const* m1)
{
  if(m0 == m1) 
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // NR CGI 9.3.1.12
  // Mandatory
  if(eq_nr_cgi(&m0->nr_cgi, &m1->nr_cgi) == false)
    return false;

  // Cell Barred 
  // Mandatory
  if(eq_cell_barred(&m0->cell_barred, &m1->cell_barred) == false)
    return false;

  // IAB Barred 
  // Optional
  if(eq_cell_barred(m0->iab_barred, m1->iab_barred) == false)
    return false;

  return true;
}

