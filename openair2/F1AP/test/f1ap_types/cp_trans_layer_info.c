#include "cp_trans_layer_info.h"

#include <assert.h>

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

void free_cp_trans_layer_info(cp_trans_layer_info_t* src)
{
  assert(src != NULL);

  if(src->type == IP_ADDRESS_CP_TRANS_LAYER_INFO ){
    free_endpoint_ip_addr(&src->ep_ip_addr);
  } else if(src->type == IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO){
    free_endpoint_ip_addr_port(&src->ep_ip_addr_port);
  } else {
    assert(0!=0 && "Unknown type");
  }

}

bool eq_cp_trans_layer_info(cp_trans_layer_info_t const* m0, cp_trans_layer_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  if(m0->type != m1->type)
    return false;

  if(m0->type == IP_ADDRESS_CP_TRANS_LAYER_INFO){
    if(eq_bit_string(&m0->ep_ip_addr.trans_layer_add, &m1->ep_ip_addr.trans_layer_add) == false) 
      return false;
  } else if(m0->type == IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO){
    if(eq_endpoint_ip_addr_port(&m0->ep_ip_addr_port, &m1->ep_ip_addr_port) == false)
      return false;
  } else{
    assert(0!=0 && "Unknown type" );
  }

  return true;
}

