#include "nr_cgi.h"

#include <stdlib.h>

bool eq_nr_cgi( nr_cgi_t const* m0, nr_cgi_t const* m1){
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(eq_plmn_f1ap(&m0->plmn_id, &m1->plmn_id) == false)
    return false;

  if(m0->nr_cell_id != m1->nr_cell_id)
    return false;

  return true;
}

void free_nr_cgi(nr_cgi_t* src)
{
  (void)src;
  // No heap allocated
}
