#ifndef CAUSE_F1AP_MIR_H
#define CAUSE_F1AP_MIR_H 

#include <stdbool.h>

#include "radio_network_cause.h"
#include "transport_cause_f1ap.h"
#include "protocol_cause_f1ap.h"
#include "misc_cause_f1ap.h"

typedef enum{
  RADIO_NETWORK_CAUSE_F1AP = 0,
  TRANSPORT_CAUSE_F1AP = 1,
  PROTOCOL_CAUSE_F1AP = 2,
  MISC_CAUSE_F1AP = 3,

  END_CAUSE_F1AP

} cause_f1ap_e;


typedef struct{
   cause_f1ap_e type;
  union{
    radio_network_cause_f1ap_e radio;
    transport_cause_f1ap_e trans;
    protocol_cause_f1ap_e proto;
    misc_cause_f1ap_e misc;
  };

} cause_f1ap_t;

bool eq_cause_f1ap(cause_f1ap_t const* m0, cause_f1ap_t const* m1);

#endif

