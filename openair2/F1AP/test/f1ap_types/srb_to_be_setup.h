#ifndef SRB_TO_BE_SETUP_MIR_H
#define SRB_TO_BE_SETUP_MIR_H 

#include "nr_freq_info.h"

typedef enum{

  THREEE_ADD_DUP_IND,
  FOUR_ADD_DUP_IND,

  END_ADD_DUP_IND,

} add_dup_ind_e;

typedef enum{

  TRUE_DUP_IND,
  FALSE_DUP_IND,

  END_DUP_IND,

} dup_ind_e;


typedef struct{
  // Mandatory
  //  SRB ID
  uint8_t srb_id; // [0,3]

  // Optional
  // Duplication Indication
  dup_ind_e* dup_ind;  

  // Optional 
  // Additional Duplication Indication
  add_dup_ind_e* add_dup_ind;

} srb_to_be_setup_t;

void free_srb_to_be_setup(srb_to_be_setup_t* src);

bool eq_srb_to_be_setup(srb_to_be_setup_t const* m0, srb_to_be_setup_t const* m1);

#endif

