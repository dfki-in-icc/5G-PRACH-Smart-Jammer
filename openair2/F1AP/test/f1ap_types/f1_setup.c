#include "f1_setup.h"

#include <assert.h>
#include <stdlib.h>


void free_f1_setup(f1_setup_t* setup)
{
  assert(setup != NULL);
  if(setup->gnb_du_name != NULL){
    free_byte_array(*setup->gnb_du_name);
    free(setup->gnb_du_name);
  }
  if(setup->sz_srv_cell > 0){
    for(size_t i = 0; i < setup->sz_srv_cell; ++i){
      free_gnb_du_srv_cell_f1ap(&setup->srv_cell[i]);
    } 

    free(setup->srv_cell);
  }

  free_gnb_rrc_ver_f1ap(&setup->ver);
}

bool eq_f1_setup(f1_setup_t const* m0, f1_setup_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  // 1 Transaction ID
  if(m0->trans_id != m1->trans_id)
    return false;

  // 2 gNB-DU ID
  if(m0->gnb_du_id != m1->gnb_du_id)
    return false;

  // 3 gNB-DU Name
  if(eq_byte_array(m0->gnb_du_name, m1->gnb_du_name) == false )
    return false;

  // 4 gNB-DU Served Cells List
  if(m0->sz_srv_cell != m1->sz_srv_cell)
    return false;

  for(size_t i = 0; i < m0->sz_srv_cell; ++i){
    if(eq_gnb_du_srv_cell_f1ap(&m0->srv_cell[i], &m1->srv_cell[i]) == false)
      return false;
  }

  // 5 gNB-DU RRC version 
  if(eq_gnb_rrc_ver_f1ap(&m0->ver, &m1->ver) == false)
    return false;

 // 6 Transport Layer Address Info
 assert(m0->tlai == NULL && "Not implemented");
 assert(m1->tlai == NULL && "Not implemented");

  // 7 BAP Address
  assert(m0->bad_add == NULL && "Not implemented" );
  assert(m1->bad_add == NULL && "Not implemented" );

  // 8 Extended gNB-DU Name
  assert(m0->egdn == NULL && "Not implemented");
  assert(m1->egdn == NULL && "Not implemented");

  return true;
}

