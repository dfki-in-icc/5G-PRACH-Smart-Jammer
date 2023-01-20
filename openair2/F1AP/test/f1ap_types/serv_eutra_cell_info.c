#include "serv_eutra_cell_info.h"

#include <assert.h>

void free_serv_eutra_cell_info(serv_eutra_cell_info_t* src)
{
  assert(src != NULL);

// serv_eutra_cell_info_e type;
// union{
//   fdd_info_serv_eutra_cell_info_t fdd;
//   tdd_info_serv_eutra_cell_info_t tdd;
// }; 

 //Optional
  assert(src->prot_eutra_resource_ind == NULL && "Not implemented");

}


bool eq_fdd_info_serv_eutra_cell_info( fdd_info_serv_eutra_cell_info_t const* m0,  fdd_info_serv_eutra_cell_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->ul_offset_to_point_a != m1->ul_offset_to_point_a)
    return false;

  if(m0->dl_offset_to_point_a != m1->dl_offset_to_point_a)
    return false;

  return true;
}

bool eq_tdd_info_serv_eutra_cell_info( tdd_info_serv_eutra_cell_info_t const* m0, tdd_info_serv_eutra_cell_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->offset_to_point_a != m1->offset_to_point_a)
    return false;

  return true;
}

bool eq_serv_eutra_cell_info(serv_eutra_cell_info_t const* m0, serv_eutra_cell_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;
  
  if(m0->type != m1->type)
    return false;

  if(m0->type == FDD_INFO_SERV_EUTRA_CELL_INFO ){
    if(eq_fdd_info_serv_eutra_cell_info(&m0->fdd, &m1->fdd) == false)  
      return false;
  } else if(m0->type == TDD_INFO_SERV_EUTRA_CELL_INFO){
    if(eq_tdd_info_serv_eutra_cell_info(&m0->tdd, &m1->tdd) == false)  
      return false;
  } else {
    assert(0!=0 && "Unknown type" );
  }

  assert(m0->prot_eutra_resource_ind == NULL && "not implemented" );

  return true;
}


