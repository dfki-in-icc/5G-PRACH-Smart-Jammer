#include "gnb_rrc_ver.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

void free_gnb_rrc_ver_f1ap( gnb_rrc_ver_f1ap_t* src)
{
  assert(src != NULL);
  
  free_bit_string(src->latest_rrc_version);

  if(src->latest_rrc_version_enh != NULL){
    free_byte_array(*src->latest_rrc_version_enh);
    free(src->latest_rrc_version_enh); 
  }

}

bool eq_gnb_rrc_ver_f1ap(gnb_rrc_ver_f1ap_t const* m0, gnb_rrc_ver_f1ap_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  if ( eq_bit_string( m0->latest_rrc_version, m1->latest_rrc_version) == false)
    return false;


  if(m0->latest_rrc_version_enh != m1->latest_rrc_version_enh ){
    if(m0->latest_rrc_version_enh == NULL)
      return false;
    if(m1->latest_rrc_version_enh == NULL)
      return false;
    if(eq_byte_array( m0->latest_rrc_version_enh, m1->latest_rrc_version_enh) == false)
      return false;
  }

  return true;
}

