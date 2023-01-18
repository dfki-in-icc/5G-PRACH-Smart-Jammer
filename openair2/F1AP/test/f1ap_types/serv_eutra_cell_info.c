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

