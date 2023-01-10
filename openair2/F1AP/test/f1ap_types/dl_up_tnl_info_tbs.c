#include "dl_up_tnl_info_tbs.h"
#include <assert.h>
#include <string.h>

static
bool eq_bit_string(BIT_STRING_t const* m0, BIT_STRING_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->bits_unused != m1->bits_unused)
    return false;

  if(m0->size != m1->size)
    return false;

  assert(m0->size < 1024 && "Security check. Is it really greater than 1024?");
  assert(m1->size < 1024 && "Security check. Is it really greater than 1024?");

  if(memcmp(m0->buf, m1->buf, m0->size) != 0)
    return false;

  return true;
}


void free_dl_up_trans_layer_info( dl_up_trans_layer_info_t* src)
{
  assert(src != NULL);
  assert(src->trans_layer_addr.buf != NULL);

  if(src->trans_layer_addr.buf != NULL)
    free(src->trans_layer_addr.buf );
}

bool eq_dl_up_trans_layer_info( dl_up_trans_layer_info_t const* m0, dl_up_trans_layer_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // 9.3.2.1 // [1, 160]
  assert(m0->trans_layer_addr.size > 0 && m0->trans_layer_addr.size < 161);
  assert(m1->trans_layer_addr.size > 0 && m1->trans_layer_addr.size < 161);
  if(eq_bit_string( &m0->trans_layer_addr, &m1->trans_layer_addr) == false)
    return false;

  if(memcmp(m0->gtp_teid, m1->gtp_teid, 4) != 0) 
    return false;

  return true;
}

