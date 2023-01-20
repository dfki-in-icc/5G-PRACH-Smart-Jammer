#ifndef F1AP_CP_TRANSPORT_LAYER_INFORMATION_H
#define F1AP_CP_TRANSPORT_LAYER_INFORMATION_H

#include "endpoint_ip_addr.h"
#include "endpoint_ip_addr_port.h"

#include <stdbool.h>

typedef enum{
  IP_ADDRESS_CP_TRANS_LAYER_INFO,
  IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO,
  END_CP_TRANS_LAYER_INFO  
} cp_trans_layer_info_e ;

typedef struct{
  cp_trans_layer_info_e type;
  // 9.3.2.4
  union{
  endpoint_ip_addr_t ep_ip_addr;
  endpoint_ip_addr_port_t ep_ip_addr_port;
  }; 

} cp_trans_layer_info_t ;

void free_cp_trans_layer_info(cp_trans_layer_info_t* src);

bool eq_cp_trans_layer_info(cp_trans_layer_info_t const* m0, cp_trans_layer_info_t const* m1);

#endif
