#ifndef F1AP_PLMN_MIR_H
#define F1AP_PLMN_MIR_H 

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint16_t mcc;
  uint16_t mnc;
  uint8_t mnc_digit_len;
} plmn_f1ap_t;

bool eq_plmn_f1ap( plmn_f1ap_t const* m0, plmn_f1ap_t const* m1);


#endif

