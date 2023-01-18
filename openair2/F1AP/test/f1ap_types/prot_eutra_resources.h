#ifndef F1AP_PROTECTED_EUTRA_RESOURCES
#define F1AP_PROTECTED_EUTRA_RESOURCES

#include <stdint.h>
#include <stdlib.h>

#include "eutra_cell.h"

typedef struct {
  uint16_t spec_sharing_group_id; // [1, 256]
                                  //
  size_t sz_eutra_cell; // [1,256]
  eutra_cell_t* eutra_cell;

} prot_eutra_resources_t;

void free_prot_eutra_resources(prot_eutra_resources_t* src);

#endif
