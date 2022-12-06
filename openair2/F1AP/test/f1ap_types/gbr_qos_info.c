#include "gbr_qos_info.h"

#include <stdlib.h>

bool eq_gbr_qos_info( gbr_qos_info_t const* m0,  gbr_qos_info_t const* m1)
{

  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  //E-RAB Maximum Bit Rate Downlink // max. 4 x 10^12
  if(m0->e_rab_max_bit_rate_downlink != m1->e_rab_max_bit_rate_downlink)
    return false;

  // Mandatory
  // E-RAB Maximum Bit Rate Uplink
  // max. 4 x 10^12   
  if(m0->e_rab_max_bit_rate_upwnlink!= m1->e_rab_max_bit_rate_upwnlink)
    return false;

  // Mandatory
  // E-RAB Guaranteed Bit Rate Downlink
  // max. 4 x 10^12 
  if(m0->e_rab_guaranteed_bit_rate_downlink!= m1->e_rab_guaranteed_bit_rate_downlink)
    return false;

  // Mandatory
  // E-RAB Guaranteed Bit Rate Uplink
  // max. 4 x 10^12 
  if(m0->e_rab_guaranteed_bit_rate_uplink!= m1->e_rab_guaranteed_bit_rate_uplink)
    return false;

  return true;
}

