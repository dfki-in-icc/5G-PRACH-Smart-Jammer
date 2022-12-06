#ifndef SCELL_TO_BE_SETUP_MIR_H
#define SCELL_TO_BE_SETUP_MIR_H 

#include <stdbool.h>

#include "nr_cgi.h"
#include "cell_ul_conf.h"

typedef struct{

  // Mandatory
  // SCell ID 
  nr_cgi_t scell_id;

  // Mandatory
  // SCellIndex
  uint8_t scell_idx; // INTEGER (1..31

  // Optional
  // SCell UL Configured
  cell_ul_conf_e* scell_ul_conf;

  // Optional
  // servingCellMO
  uint8_t* serv_cell_mo; // INTEGER(1..64) 

} scell_to_be_setup_t;

void free_scell_to_be_setup( scell_to_be_setup_t* src);

bool eq_scell_to_be_setup(scell_to_be_setup_t const* m0, scell_to_be_setup_t const* m1);

#endif

