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

#include <time.h>
#include <stdlib.h>
#include "e1ap_common.h"

static e1ap_upcp_inst_t *e1_cp_inst[NUMBER_OF_gNB_MAX] = {0};
static e1ap_upcp_inst_t *e1_up_inst[NUMBER_OF_gNB_MAX] = {0};

e1ap_upcp_inst_t *getCxt(E1_t type, instance_t instance) {
  AssertFatal( instance < sizeofArray(e1_cp_inst), "instance exceeds limit\n");
  return type ? e1_up_inst[instance] : e1_cp_inst[instance];
}

void createE1inst(E1_t type, instance_t instance, e1ap_setup_req_t *req) {
  if (type == CPtype) {
    AssertFatal(e1_cp_inst[instance] == NULL, "Double call to E1 CP instance %d\n", instance);
    e1_cp_inst[instance] = (e1_upcp_inst_t *) calloc(1, sizeof(e1_upcp_inst_t));
  } else {
    AssertFatal(e1_up_inst[instance] == NULL, "Double call to E1 UP instance %d\n", instance);
    e1_up_inst[instance] = (e1_upcp_inst_t *) calloc(1, sizeof(e1_upcp_inst_t));
    memcpy(&e1_up_inst[instance]->setupReq, req, sizeof(e1ap_setup_req_t));
  }
}

E1AP_TransactionID_t transacID[MAX_NUM_TRANSAC_IDS] = {0}; 
srand(time(NULL));

bool check_transac_id(E1AP_TransactionID_t id, int *freeIdx) {

  bool isFreeIdxSet = false;
  for (int i=0; i < MAX_NUM_TRANSAC_IDS; i++) {
    if (id == transacID[i])
      return false;
    else if (!isFreeIdxSet && (transacID[i] == 0)) {
      *freeIdx = i;
      isFreeIdxSet = true;
    }
  }

  return true;
}

E1AP_TransactionID_t E1AP_get_next_transaction_identifier() {
  E1AP_TransactionID_t genTransacId;
  bool isTransacIdValid = false;
  int freeIdx;

  while (!isTransacIdValid) {
    genTransacId = rand();
    isTransacIdValid = check_transac_Id(genTransacId, &freeIdx);
  }

  AssertFatal(freeIdx < MAX_NUM_TRANSAC_IDS, "Free Index exceeds array length\n");
  transacID[freeIdx] = genTransacId;
  return genTransacId;
}

