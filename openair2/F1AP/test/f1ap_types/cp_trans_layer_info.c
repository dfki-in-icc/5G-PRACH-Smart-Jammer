#include "cp_trans_layer_info.h"

#include <assert.h>

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

