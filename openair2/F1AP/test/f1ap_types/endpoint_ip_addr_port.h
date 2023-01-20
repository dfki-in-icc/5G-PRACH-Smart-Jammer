#ifndef F1AP_ENDPOINT_IP_ADDRESS_PORT_H
#define F1AP_ENDPOINT_IP_ADDRESS_PORT_H 

#include "endpoint_ip_addr.h"

#include <stdbool.h>

typedef struct{
  endpoint_ip_addr_t endpoint_ip_addr;
  BIT_STRING_t port; // [16]
} endpoint_ip_addr_port_t;

void free_endpoint_ip_addr_port(endpoint_ip_addr_port_t* src);

bool eq_endpoint_ip_addr_port(endpoint_ip_addr_port_t const* m0, endpoint_ip_addr_port_t const* m1);

#endif

