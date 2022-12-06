#include "gbr_qos_flow_info.h"

#include <assert.h>
#include <stdlib.h>

void free_gbr_qos_flow_info(gbr_qos_flow_info_t* src)
{
  assert(src != NULL);

  // Mandatory
  // Maximum Flow Bit Rate Downlink 9.3.1.22
  assert(src->max_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
                                       
  // Mandatory
  // Maximum Flow Bit Rate Uplink 9.3.1.22
  assert(src->max_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
                                       //
  // Mandatory
  // Guaranteed Flow Bit Rate Downlink 9.3.1.22
  assert(src->gbr_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)

  //Mandatory
  //Guaranteed Flow Bit Rate Uplink 9.3.1.22
  assert(src->gbr_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)

  //Optional
  //Maximum Packet Loss Rate Downlink 9.3.1.50
  if(src->max_pkt_loss_rate_downlink != NULL){
   assert(*src->max_pkt_loss_rate_downlink < 1001 ); //INTEGER(0..1000)
   free(src->max_pkt_loss_rate_downlink);
  }

  //Optional
  //Maximum Packet Loss Rate Uplink 9.3.1.50
  if(src->max_pkt_loss_rate_uplink != NULL) {
    assert(*src->max_pkt_loss_rate_uplink < 1001); //INTEGER(0..1000)
    free(src->max_pkt_loss_rate_uplink);
  }
  
  //Optional 
  //Alternative QoS Parameters Set List 9.3.1.50
  if(src->alternative_qos_param != NULL){
    assert(*src->alternative_qos_param  < 1001); //INTEGER(0..1000)
    free(src->alternative_qos_param);
  }

}

bool eq_gbr_qos_flow_info(gbr_qos_flow_info_t const* m0, gbr_qos_flow_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL ||  m1 == NULL)
    return false;

  // Mandatory
  // Maximum Flow Bit Rate Downlink 9.3.1.22
  assert(m0->max_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  assert(m1->max_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  if(m0->max_flow_bit_rate_downlink != m1->max_flow_bit_rate_downlink)
    return false;
                                       
  // Mandatory
  // Maximum Flow Bit Rate Uplink 9.3.1.22
  assert(m0->max_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  assert(m1->max_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  if(m0->max_flow_bit_rate_uplink != m1->max_flow_bit_rate_uplink  )
    return false;

  // Mandatory
  // Guaranteed Flow Bit Rate Downlink 9.3.1.22
  assert(m0->gbr_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  assert(m1->gbr_flow_bit_rate_downlink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  if(m0->gbr_flow_bit_rate_downlink != m1->gbr_flow_bit_rate_downlink)
    return false; 

  //Mandatory
  //Guaranteed Flow Bit Rate Uplink 9.3.1.22
  assert(m0->gbr_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  assert(m1->gbr_flow_bit_rate_uplink < 4000000000001UL); //INTEGER (0..4,000,000,000,000,...)
  if(m0->gbr_flow_bit_rate_uplink != m1->gbr_flow_bit_rate_uplink  )
    return false;

  //Optional
  //Maximum Packet Loss Rate Downlink 9.3.1.50
  if(m0->max_pkt_loss_rate_downlink != m1->max_pkt_loss_rate_downlink){
    if(m0->max_pkt_loss_rate_downlink == NULL || m1->max_pkt_loss_rate_downlink == NULL)
      return false;

   assert(*m0->max_pkt_loss_rate_downlink < 1001 ); //INTEGER(0..1000)
   assert(*m1->max_pkt_loss_rate_downlink < 1001 ); //INTEGER(0..1000)

    if(*m0->max_pkt_loss_rate_downlink != *m1->max_pkt_loss_rate_downlink)
      return false;
  }

  //Optional
  //Maximum Packet Loss Rate Uplink 9.3.1.50
  if(m0->max_pkt_loss_rate_uplink != m1->max_pkt_loss_rate_uplink ){
    if(m0->max_pkt_loss_rate_uplink == NULL || m1->max_pkt_loss_rate_uplink == NULL)
      return false;

   assert(*m0->max_pkt_loss_rate_uplink < 1001 ); //INTEGER(0..1000)
   assert(*m1->max_pkt_loss_rate_uplink < 1001 ); //INTEGER(0..1000)

    if(*m0->max_pkt_loss_rate_uplink != *m1->max_pkt_loss_rate_uplink )
      return false;
  }

  //Optional 
  //Alternative QoS Parameters Set List 9.3.1.50
  if(m0->alternative_qos_param != m1->alternative_qos_param ){
    if(m0->alternative_qos_param == NULL || m1->alternative_qos_param == NULL)
      return false;

   assert(*m0->alternative_qos_param < 1001 ); //INTEGER(0..1000)
   assert(*m1->alternative_qos_param < 1001 ); //INTEGER(0..1000)

    if(*m0->alternative_qos_param != *m1->alternative_qos_param )
      return false;
  }

  return true;
}

