#include "gnb_du_srv_cell.h"

#include <assert.h>
#include <stdlib.h>

void free_gnb_du_srv_cell_f1ap( gnb_du_srv_cell_f1ap_t* src)
{
  assert(src != NULL);

  free_srv_cell_info(&src->srv_cell_info);

  if(src->info != NULL){
    free_gnb_du_sys_info(src->info);
    free(src->info);
  }

}


bool eq_gnb_du_srv_cell_f1ap(gnb_du_srv_cell_f1ap_t const* m0, gnb_du_srv_cell_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);
  
  if(eq_srv_cell_info(&m0->srv_cell_info, &m1->srv_cell_info) == false )
    return false;

  if(eq_gnb_du_sys_info (m0->info, m1->info) == false)
    return false;

  return true;
}

