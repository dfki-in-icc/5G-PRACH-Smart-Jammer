#include "neighbour_cell_info.h"

#include <assert.h>
#include <stdlib.h>

void free_neighbour_cell_info( neighbour_cell_info_t* src)
{
  assert(src != NULL);
  // NR CGI 9.3.1.12
  // Mandatory
  free_nr_cgi(&src->nr_cgi);

  // Intended TDD DL-UL Configuration 9.3.1.89
  // Optional
  assert(src->intended_tdd_dl_ul_conf == NULL && "Not implemented") ;
}

