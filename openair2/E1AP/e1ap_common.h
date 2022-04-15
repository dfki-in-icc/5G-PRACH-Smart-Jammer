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
 * Author and copyright: Laurent Thomas, open-cells.com
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

#include "E1AP_TransactionID.h"

#define MAX_NUM_TRANSAC_IDS 8
#define E1AP_MAX_NUM_PLMNS

typedef struct PLMN_ID_s {
  int id;
} PLMN_ID_t;

typedef struct e1ap_setup_req_s {
  uint64_t gNB_cu_up_id;
  char *gNB_cu_up_name;
  uint64_t transac_id;
  uint64_t cn_support;
  int supported_plmns; 
  PLMN_ID_t plmns[E1AP_MAX_NUM_PLMNS];
} e1ap_setup_req_t;

typedef struct e1ap_upcp_inst_s {
  e1ap_setup_req_t setupReq;
  uint32_t assoc_id;
} e1ap_upcp_inst_t;

typedef enum {
  CPtype = 0,
  UPtype
} E1_t;
