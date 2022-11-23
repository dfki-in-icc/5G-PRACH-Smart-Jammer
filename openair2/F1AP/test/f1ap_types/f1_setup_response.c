#include "f1_setup_response.h"

#include <assert.h>
#include <stdlib.h>

void free_f1_setup_response(f1_setup_response_t* sr)
{
  assert(sr != NULL);

  if(sr->gnb_cu_name != NULL){
    free_byte_array(*sr->gnb_cu_name);
    free(sr->gnb_cu_name);
  }

  // 3 Cells to activated list. Optional 
  for(size_t i =0; i < sr->sz_act_cell; ++i){
    free_activate_cell(&sr->act_cell[i]);   
  }

  free(sr->act_cell);

  // 4 gNB-CU RRC version 
  free_gnb_rrc_ver_f1ap(&sr->ver);

  // 5 Transport Layer Address Info
  assert(sr->tlai == NULL && "Not implemented" );

  // 6 Uplink BH Non-UP Traffic Mapping 
  assert(sr->ubh_nup_traff == NULL && "Not implemented");

  // 7 BAP Address
  assert(sr->bad_add == NULL && "Not implemented");

  // 8 Extended gNB-CU Name
  assert(sr->egcn == NULL && "Not implemented");
}

bool eq_f1_setup_response(f1_setup_response_t const* m0, f1_setup_response_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  // 1 Transaction ID
  if(m0->trans_id != m1->trans_id )
    return false;

  // 2 gNB-CU Name
  if(eq_byte_array(m0->gnb_cu_name, m1->gnb_cu_name) == false)
    return false;


  // 3 Cells to activated list. Optional 
  if(m0->sz_act_cell != m1->sz_act_cell)
    return false;

  for(size_t i =0; i < m0->sz_act_cell; ++i){
    if(eq_activate_cell(&m0->act_cell[i], &m1->act_cell[i] ) == false )
      return false;
  }

  // 4 gNB-CU RRC version 
  if(eq_gnb_rrc_ver_f1ap(&m0->ver, &m1->ver) == false )
    return false;

  // 5 Transport Layer Address Info
  assert(m0->tlai == NULL && "Not implemented" );
  assert(m1->tlai == NULL && "Not implemented" );

  // 6 Uplink BH Non-UP Traffic Mapping 
  assert(m0->ubh_nup_traff == NULL && "Not implemented" );
  assert(m1->ubh_nup_traff == NULL && "Not implemented" );

  // 7 BAP Address
  assert(m0->bad_add == NULL && "Not implemented" );
  assert(m1->bad_add == NULL && "Not implemented" );

  // 8 Extended gNB-CU Name
  assert(m0->egcn == NULL && "Not implemented" );
  assert(m1->egcn == NULL && "Not implemented" );

  return true;
}


