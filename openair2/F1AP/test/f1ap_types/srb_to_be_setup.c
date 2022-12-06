#include "srb_to_be_setup.h"


#include <assert.h>
#include <stdlib.h>

void free_srb_to_be_setup(srb_to_be_setup_t* src)
{
  assert(src != NULL);

  // Mandatory
  //  SRB ID
//  uint8_t srb_id; // [0,3]

  // Optional
  // Duplication Indication
  if(src->dup_ind != NULL)
    free(src->dup_ind);

  // Optional 
  // Additional Duplication Indication
  if(src->add_dup_ind != NULL)
    free(src->add_dup_ind);
}

bool eq_srb_to_be_setup(srb_to_be_setup_t const* m0, srb_to_be_setup_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  //  SRB ID // [0,3]
  assert(m0->srb_id < 4);
  assert(m1->srb_id < 4);
  if(m0->srb_id != m1->srb_id)
    return false;

  // Optional
  // Duplication Indication
  if(m0->dup_ind != m1->dup_ind){
    if(m0->dup_ind == NULL || m1->dup_ind == NULL)
      return false;

    if(*m0->dup_ind == *m1->dup_ind)
      return false;
  }

  // Optional 
  // Additional Duplication Indication
  if(m0->add_dup_ind != m1->add_dup_ind ){
    if(m0->add_dup_ind == NULL || m1->add_dup_ind == NULL)
      return false;

    if(*m0->add_dup_ind == *m1->add_dup_ind )
      return false;
  }

  return true;
}

