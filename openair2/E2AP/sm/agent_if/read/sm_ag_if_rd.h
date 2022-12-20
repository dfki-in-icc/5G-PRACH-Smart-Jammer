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


#ifndef SM_READ_INTERFACE_H
#define SM_READ_INTERFACE_H

// Interface between the SM and the agent/server. 
// The SM can call the functions here defined and implemented on the RAN/server to read data.

#include "../../kpm_sm_v2.02/ie/kpm_data_ie.h"

typedef enum{
  KPM_STATS_V0, 
  SM_AGENT_IF_READ_V0_END,
} sm_ag_if_rd_e;

// Do not change the order of data in the struct
typedef struct{
  union {
    kpm_ind_data_t kpm_stats;
  };
  sm_ag_if_rd_e type;
} sm_ag_if_rd_t;


void free_sm_ag_if_rd(sm_ag_if_rd_t* d);

sm_ag_if_rd_t cp_sm_ag_if_rd(sm_ag_if_rd_t const* d);

#endif

