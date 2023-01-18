#ifndef F1AP_ENDPOINT_IP_ADDRESS_H
#define F1AP_ENDPOINT_IP_ADDRESS_H


#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

// 9.3.2.3
typedef struct{
  BIT_STRING_t trans_layer_add;//  [1,160]
} endpoint_ip_addr_t ;

void free_endpoint_ip_addr(endpoint_ip_addr_t* src);

#endif

