#include "non_dyn_5qi_descriptor.h"

#include <assert.h>
#include <stdlib.h>

void free_non_dyn_5qi_descriptor( non_dyn_5qi_descriptor_t* src)
{
  assert(src != NULL);

  // Mandatory
  //  5QI
  // uint8_t five_qi;

  // Optional
  // Priority Level
  if(src->prio_level != NULL){
    assert(*src->prio_level > 0 && *src->prio_level < 128 );
    free(src->prio_level);
  }

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  if(src-> av_window != NULL){
    assert(*src->av_window < 4096);
    free(src->av_window);
  }

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  if(src->max_data_burst_vol != NULL){
    assert(*src-> max_data_burst_vol < 2000001); // INTEGER (0..4095, ..., 4096..2000000)
    free(src->max_data_burst_vol); 
  } 

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  if(src-> pkt_delay_budget_downlink != NULL){
    free(src->pkt_delay_budget_downlink);
  }
  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  if(src->pkt_delay_budget_uplink != NULL ) 
    free(src->pkt_delay_budget_uplink );

}

bool eq_non_dyn_5qi_descriptor(non_dyn_5qi_descriptor_t const* m0, non_dyn_5qi_descriptor_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  //  5QI
  if(m0->five_qi != m1->five_qi)
    return false;
  // uint8_t five_qi;

  // Optional
  // Priority Level
  if(m0->prio_level != m1->prio_level){
    if(m0->prio_level == NULL || m1->prio_level == NULL) 
      return false;

    assert(*m0->prio_level > 0 && *m0->prio_level < 128 );
    assert(*m1->prio_level > 0 && *m1->prio_level < 128 );

    if(*m0->prio_level != *m1->prio_level)
      return false;
  }

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  if(m0->av_window != m1->av_window){
    if(m0->av_window == NULL || m1->av_window == NULL) 
      return false;

    assert(*m0->av_window <  4096);
    assert(*m1->av_window <  4096);

    if(*m0->av_window != *m1->av_window )
      return false;
  }
  
  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  if(m0->max_data_burst_vol != m1->max_data_burst_vol ){
    if(m0->max_data_burst_vol == NULL || m1->max_data_burst_vol == NULL) 
      return false;

    assert(*m0->max_data_burst_vol <  2000001);
    assert(*m1->max_data_burst_vol <  2000001);

    if(*m0->max_data_burst_vol != *m1->max_data_burst_vol )
      return false;
  }

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  if(m0->pkt_delay_budget_downlink != m1->pkt_delay_budget_downlink ){
    if(m0->pkt_delay_budget_downlink == NULL || m1->pkt_delay_budget_downlink == NULL) 
      return false;

    if(*m0->pkt_delay_budget_downlink != *m1->pkt_delay_budget_downlink)
      return false;
  }

  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  if(m0->pkt_delay_budget_uplink != m1->pkt_delay_budget_uplink ){
    if(m0->pkt_delay_budget_uplink == NULL || m1->pkt_delay_budget_uplink == NULL) 
      return false;

    if(*m0->pkt_delay_budget_uplink != *m1->pkt_delay_budget_uplink)
      return false;
  }

  return true;
}

