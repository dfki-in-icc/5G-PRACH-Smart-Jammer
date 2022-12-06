#include "scell_to_be_setup.h"

#include <assert.h>
#include <stdlib.h>

void free_scell_to_be_setup(scell_to_be_setup_t* src)
{
  assert(src != NULL);

  // Mandatory
  // SCell ID 
  free_nr_cgi(&src->scell_id);

  // Mandatory
  // SCellIndex
  // uint8_t scell_idx; // INTEGER (1..31

  // Optional
  // SCell UL Configured
  if(src-> scell_ul_conf != NULL)
    free(src->scell_ul_conf);

  // Optional
  // servingCellMO
  if(src-> serv_cell_mo != NULL)
    free(src->serv_cell_mo);

}

bool eq_scell_to_be_setup(scell_to_be_setup_t const* m0, scell_to_be_setup_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // SCell ID 
  if(eq_nr_cgi(&m0->scell_id, &m1->scell_id) == false)
    return false;

  // Mandatory
  // SCellIndex // INTEGER (1..31
  assert(m0->scell_idx < 32);
  assert(m1->scell_idx < 32);
  if(m0->scell_idx != m1->scell_idx)
    return false;

  // Optional
  // SCell UL Configured
  if(m0->scell_ul_conf != m1->scell_ul_conf){
    if(m0->scell_ul_conf == NULL || m1->scell_ul_conf == NULL)
      return false;

    if(*m0->scell_ul_conf != *m1->scell_ul_conf)
      return false;
  }

  // Optional
  // servingCellMO
  // INTEGER(1..64) 
  if(m0->serv_cell_mo != m1->serv_cell_mo ){
    if(m0->serv_cell_mo == NULL || m1->serv_cell_mo == NULL)
      return false;

    assert(*m0->serv_cell_mo < 65);
    assert(*m1->serv_cell_mo < 65);

    if(*m0->serv_cell_mo != *m1->serv_cell_mo )
      return false;
  }

  return true;
}
