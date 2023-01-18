#ifndef F1AP_EUTRA_CELL_H
#define F1AP_EUTRA_CELL_H 

#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"
#include "serv_eutra_cell_info.h"

typedef struct {
  // Mandatory
  BIT_STRING_t eutra_cell_id; // [28]

  // Served E-UTRA CellInformation 9.3.1.64
  // Mandatory
  serv_eutra_cell_info_t serv_eutra_cell_info;

} eutra_cell_t;

void free_eutra_cell(eutra_cell_t* src);

#endif

