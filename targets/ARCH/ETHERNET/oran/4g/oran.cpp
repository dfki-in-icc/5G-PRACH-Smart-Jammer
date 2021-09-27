/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */


#include <stdio.h>
// #include "common_lib.h"
// #include "ethernet_lib.h"
#include "oran_isolate.h"
#include "shared_buffers.h"
#include "low_oran.h"
#include "xran_lib_wrap.hpp"


void xran_fh_rx_callback(void *pCallbackTag, xran_status_t status){
    rte_pause();
}
void xran_fh_srs_callback(void *pCallbackTag, xran_status_t status){
    rte_pause();
}
void xran_fh_rx_prach_callback(void *pCallbackTag, xran_status_t status){
    rte_pause();
}

#ifdef __cplusplus
extern "C"
{
#endif
int start_oran(){

  //xranLibWraper *xranlib = malloc(sizeof(xranLibWraper)); 
  xranLibWraper *xranlib;
  xranlib = new xranLibWraper;
  
  if(xranlib->SetUp() < 0) {
     return (-1);
  }
  xranlib->Init();
  xranlib->Open(nullptr, 
            nullptr, 
            (void *)xran_fh_rx_callback, 
            (void *)xran_fh_rx_prach_callback, 
            (void *)xran_fh_srs_callback);
  xranlib->Start();

  return (0);

}
#ifdef __cplusplus
}
#endif


























