#include "eutra_cell.h"

#include <assert.h>

#include <stdlib.h>

static
bool eq_bit_string(BIT_STRING_t m0, BIT_STRING_t m1 )
{
  if(m0.bits_unused != m1.bits_unused)
    return false;

  if(m0.size != m1.size)
    return false;

  if(memcmp(m0.buf, m1.buf, m0.size) != 0)
    return false;

  return true;
}

static
void free_bit_string(BIT_STRING_t src)
{
  if(src.size > 0){
    assert(src.buf != NULL);
    free(src.buf);
  }
}


void free_eutra_cell(eutra_cell_t* src)
{
  assert(src != NULL);

  // Mandatory
  free_bit_string(src->eutra_cell_id); // [28]

  // Served E-UTRA CellInformation 9.3.1.64
  // Mandatory
  free_serv_eutra_cell_info(&src->serv_eutra_cell_info);

}


