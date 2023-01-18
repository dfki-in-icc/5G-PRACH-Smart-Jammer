
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
