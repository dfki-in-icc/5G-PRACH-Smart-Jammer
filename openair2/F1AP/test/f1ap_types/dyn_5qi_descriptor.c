#include "dyn_5qi_descriptor.h"

#include <assert.h>
#include <stdlib.h>

void free_dyn_5qi_descriptor(dyn_5qi_descriptor_t* src)
{
  assert(src != NULL);

  //Mandatory
  //QoS Priority Level
  assert(src->qos_prio_level > 0 && src->qos_prio_level < 128);

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  assert(src->pkt_delay_budget < 1024);// INTEGER (0..1023, ...)

  // Mandatory
  // Packet Error Rate 9.3.1.52
  //pkt_error_rate_t pkt_error_rate;

  // Optional
  // 5QI
  if(src->five_qi != NULL)
    free(src->five_qi);

  // C-ifGBRflow
  // Delay Critical
  if(src->delay_critical != NULL){
    assert(*src->delay_critical < END_DELAY_CRITICAL);
    free(src->delay_critical );
  }
  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  if(src->av_window != NULL){
  assert(*src->av_window < 4096); // INTEGER (0..4095, ...)
  free(src->av_window);                                //
  }
  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  if(src->max_data_burst_vol != NULL ){
    assert(*src->max_data_burst_vol < 2000000);
    free(src->max_data_burst_vol);
  }
  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  if(src->ext_pkt_delay_budget){
    free(src->ext_pkt_delay_budget);
  }

  // Optional 
  // CN Packet Delay Budget Downlink
  if(src->cn_pkt_dalay_budget_downlink != NULL){
    free(src->cn_pkt_dalay_budget_downlink);
  }

  // Optional
  // CN Packet Delay Budget Uplink
  if(src->cn_pkt_dalay_budget_uplink != NULL){
    free(src->cn_pkt_dalay_budget_uplink);
  }

}

bool eq_dyn_5qi_descriptor(dyn_5qi_descriptor_t const* m0, dyn_5qi_descriptor_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  //Mandatory
  //QoS Priority Level
  assert(m0->qos_prio_level > 0 && m0->qos_prio_level < 128);
  assert(m1->qos_prio_level > 0 && m1->qos_prio_level < 128);
  if(m0->qos_prio_level != m1->qos_prio_level)
    return false;

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  assert(m0->pkt_delay_budget < 1024);// INTEGER (0..1023, ...)
  assert(m1->pkt_delay_budget < 1024);// INTEGER (0..1023, ...)
  if(m0->pkt_delay_budget != m1->pkt_delay_budget) 
    return false; 

  // Mandatory
  // Packet Error Rate 9.3.1.52
  assert(m0->pkt_error_rate.exponent < 10 );
  assert(m1->pkt_error_rate.exponent < 10 );
  if(m0->pkt_error_rate.exponent != m1->pkt_error_rate.exponent)
    return false;

  assert(m0->pkt_error_rate.scalar < 10 );
  assert(m1->pkt_error_rate.scalar < 10 );
  if(m0->pkt_error_rate.scalar != m1->pkt_error_rate.scalar)
    return false;

  // Optional
  // 5QI
  if(m0->five_qi != m1->five_qi){
    if(m0->five_qi == NULL || m1->five_qi == NULL)
      return false;

    if(*m0->five_qi != *m1->five_qi)
      return false;
  }

  // C-ifGBRflow
  // Delay Critical
  if(m0->delay_critical != m1->delay_critical ){
    if(m0->delay_critical == NULL || m1->delay_critical == NULL)
      return false;

    if(*m0->delay_critical != *m1->delay_critical )
      return false;
  }


  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  if(m0->av_window != m1->av_window ){
    if(m0->av_window == NULL || m1->av_window == NULL)
      return false;

    assert(*m0->av_window < 4096); // INTEGER (0..4095, ...)

    if(*m0->av_window != *m1->av_window )
      return false;
  }

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  if(m0->max_data_burst_vol != m1->max_data_burst_vol ){
    if(m0->max_data_burst_vol == NULL || m1->max_data_burst_vol == NULL)
      return false;

    if(*m0->max_data_burst_vol != *m1->max_data_burst_vol )
      return false;
  }

  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  if(m0->ext_pkt_delay_budget != m1->ext_pkt_delay_budget ){
    if(m0->ext_pkt_delay_budget == NULL || m1->ext_pkt_delay_budget == NULL)
      return false;

    if(*m0->ext_pkt_delay_budget != *m1->ext_pkt_delay_budget )
      return false;
  }

  // Optional 
  // CN Packet Delay Budget Downlink
  if(m0->cn_pkt_dalay_budget_downlink != m1->cn_pkt_dalay_budget_downlink ){
    if(m0->cn_pkt_dalay_budget_downlink == NULL || m1->cn_pkt_dalay_budget_downlink == NULL)
      return false;

    if(*m0->cn_pkt_dalay_budget_downlink != *m1->cn_pkt_dalay_budget_downlink )
      return false;
  }

  // Optional
  // CN Packet Delay Budget Uplink
  if(m0->cn_pkt_dalay_budget_uplink != m1->cn_pkt_dalay_budget_uplink ){
    if(m0->cn_pkt_dalay_budget_uplink == NULL || m1->cn_pkt_dalay_budget_uplink == NULL)
      return false;

    if(*m0->cn_pkt_dalay_budget_uplink != *m1->cn_pkt_dalay_budget_uplink )
      return false;
  }

  return true;
}

