#ifndef PACKET_ERROR_RATE_F1AP_MIR_H
#define PACKET_ERROR_RATE_F1AP_MIR_H 

#include <stdint.h>

typedef struct{

  uint8_t scalar; // INTEGER (0..9, ...)
  uint8_t exponent; // INTEGER (0..9, ...)

} pkt_error_rate_t;

#endif
