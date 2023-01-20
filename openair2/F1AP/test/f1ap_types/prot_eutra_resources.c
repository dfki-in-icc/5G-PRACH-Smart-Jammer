
#include "prot_eutra_resources.h"

#include <assert.h>

void free_prot_eutra_resources(prot_eutra_resources_t* src)
{
  assert(src != NULL);

  //uint16_t spec_sharing_group_id; // [1, 256]
                                  //
  assert(src->sz_eutra_cell > 0 && src->sz_eutra_cell < 257); // [1,256]
  for(size_t i = 0; i < src-> sz_eutra_cell; ++i){
    free_eutra_cell(&src->eutra_cell[i]);
  }                      //
 
  free(src->eutra_cell);

}

bool eq_prot_eutra_resources(prot_eutra_resources_t const* m0, prot_eutra_resources_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  assert(m0->spec_sharing_group_id > 0);
  assert(m1->spec_sharing_group_id > 0);
  assert(m0->spec_sharing_group_id < 257);
  assert(m1->spec_sharing_group_id < 257);

  if(m0->spec_sharing_group_id != m1->spec_sharing_group_id)
    return false;
  
  if(m0->sz_eutra_cell != m1->sz_eutra_cell)
    return false;

  for(size_t i = 0; i < m0->sz_eutra_cell; ++i){
    if(eq_eutra_cell(&m0->eutra_cell[i], &m1->eutra_cell[i] ) == false)
      return false;
  }

  return true;
}

