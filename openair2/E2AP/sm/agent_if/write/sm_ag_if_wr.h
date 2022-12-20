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


#ifndef E2AP_SM_WR_IF_H
#define E2AP_SM_WR_IF_H 

#include "subscribe_timer.h"
#include "../../kpm_sm_v2.02/ie/kpm_data_ie.h"

typedef enum{
  SUBSCRIBE_TIMER = 0,
  SM_AGENT_IF_WRITE_V0_END,
} sm_ag_if_wr_e;

typedef struct {
  union{
    subscribe_timer_t sub_timer;  
  }; 
  sm_ag_if_wr_e type;
} sm_ag_if_wr_t;

#endif

