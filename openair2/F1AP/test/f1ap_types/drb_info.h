#ifndef DATA_RADIO_BEARER_INFORMATION_MIR_H
#define DATA_RADIO_BEARER_INFORMATION_MIR_H 

#include <stdbool.h>

#include "flows_mapped_to_drb.h"
#include "qos_flow_level_qos_parameter.h"
#include "s_nssai.h"

typedef enum{

  ACTIVE_NOTIFICATION_CTRL, 
  NOT_ACTIVE_NOTIFICATION_CTRL,

  END_NOTIFICATION_CTRL

} notification_ctrl_e; 

typedef struct{
  // Mandatory
  // DRB QoS 9.3.1.45
  qos_flow_level_qos_parameter_t drb_qos;

  // Mandatory
  // S-NSSAI 9.3.1.38
  s_nssai_t nssai;

  // Optional
  // Notification Control 9.3.1.56
  notification_ctrl_e* notification_ctrl;

  // Flows Mapped to DRB
  size_t sz_flows_mapped_to_drb; // [0,64]
  flows_mapped_to_drb_t* flows_mapped_to_drb;


} drb_info_t;

void free_drb_info(drb_info_t* src);

bool eq_drb_info(drb_info_t const* m0, drb_info_t const* m1);

#endif

