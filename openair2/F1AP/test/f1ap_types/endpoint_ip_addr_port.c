#include "endpoint_ip_addr_port.h"

#include <assert.h>

#include <stdbool.h>

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

void free_endpoint_ip_addr_port(endpoint_ip_addr_port_t* src)
{
  assert(src != NULL);

  free_endpoint_ip_addr(&src->endpoint_ip_addr);

  free_bit_string(src->port);
}

