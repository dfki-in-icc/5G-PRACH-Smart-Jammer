#include "eutra_cell.h"

#include <assert.h>

#include <stdlib.h>

static
bool eq_bit_string_28(BIT_STRING_t m0, BIT_STRING_t m1 )
{
  // 28 bits
  assert(m0.bits_unused == 4);
  assert(m1.bits_unused == 4);

  assert(m0.size == 4);
  assert(m1.size == 4);

  if(m0.bits_unused != m1.bits_unused)
    return false;

  if(m0.size != m1.size)
    return false;

  if(memcmp(m0.buf, m1.buf, m0.size -1) != 0)
    return false;

  // Check last 4 bits
  uint8_t a = m0.buf[3];
  uint8_t b = m1.buf[3];

  a &= 0xF0; 
  b &= 0xF0; 

  if(a != b)
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

bool eq_eutra_cell(eutra_cell_t const* m0, eutra_cell_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory // [28]
  if(eq_bit_string_28(m0->eutra_cell_id, m1->eutra_cell_id) == false )
    return false;

  if(eq_serv_eutra_cell_info(&m0->serv_eutra_cell_info, &m1->serv_eutra_cell_info) == false)
    return false;

  return true;
}

