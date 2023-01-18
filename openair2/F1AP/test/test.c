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

#include <assert.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <poll.h>

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_F1AP-PDU.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_InitiatingMessage.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SuccessfulOutcome.h"
#include "../../../openair3/UTILS/conversions.h"

#include "byte_array.h"
#include "cp_asn_to_ir.h"
#include "cp_ir_to_asn.h" 
#include "defer.h"
#include "enc_dec_f1ap.h"
#include "gen_rnd_data.h"
#include "f1ap_types/f1_setup.h"
#include "f1ap_types/f1_setup_response.h"
#include "f1ap_types/f1_setup_failure.h"
#include "f1ap_types/ue_ctx_setup_request.h"
#include "f1ap_types/gnb_cu_conf_update.h"

void test_f1_setup_f1ap()
{
  f1_setup_t msg = gen_rnd_f1_setup();
  defer({ free_f1_setup(&msg); });

  F1AP_F1AP_PDU_t pdu = cp_f1_setup_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); });

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); });

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); });

  f1_setup_t out = cp_f1_setup_ir(&pdu2);
  defer({ free_f1_setup(&out); });

  assert(eq_f1_setup(&msg, &out) == true);
}

static
void test_f1_setup_response_f1ap()
{
  f1_setup_response_t msg = gen_rnd_f1_setup_response();
  defer({ free_f1_setup_response(&msg); });

  F1AP_F1AP_PDU_t pdu = cp_f1_setup_response_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); });

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); });

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); });

  f1_setup_response_t out = cp_f1_setup_response_ir(&pdu2);
  defer({ free_f1_setup_response(&out); });

  assert(eq_f1_setup_response(&msg, &out) == true);
}

static
void test_f1_setup_failure_f1ap()
{
  f1_setup_failure_t msg = gen_rnd_f1_setup_failure();
  defer({ free_f1_setup_failure(&msg); });

  F1AP_F1AP_PDU_t pdu = cp_f1_setup_failure_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); } );

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); } );

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); } ); 

  f1_setup_failure_t out = cp_f1_setup_failure_ir(&pdu2);
  defer({ free_f1_setup_failure(&out); });

  assert(eq_f1_setup_failure(&msg, &out) == true);
}

void test_f1_ue_ctx_setup_request()
{
  ue_ctx_setup_request_t msg = gen_rnd_ue_ctx_setup_request();
  defer({ free_ue_ctx_setup_request(&msg); });

  F1AP_F1AP_PDU_t pdu = cp_ue_ctx_setup_request_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); } );

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); } );

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); } ); 

  ue_ctx_setup_request_t out = cp_ue_ctx_setup_request_ir(&pdu2);
  defer({ free_ue_ctx_setup_request(&out); });

  assert(eq_f1_ue_ctx_setup_request(&msg, &out) == true);
}

void test_f1_ue_ctx_setup_response()
{
  ue_ctx_setup_response_t msg = gen_rnd_ue_ctx_setup_response();
  defer({ free_ue_ctx_setup_response(&msg); }); 

  F1AP_F1AP_PDU_t pdu = cp_ue_ctx_setup_response_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); } );

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); } );

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); } ); 

  ue_ctx_setup_response_t out = cp_ue_ctx_setup_response_ir(&pdu2);
  defer({ free_ue_ctx_setup_response(&out); });

  assert(eq_f1_ue_ctx_setup_response(&msg, &out) == true);
}

void test_gnb_cu_conf_update()
{
  gnb_cu_conf_update_t msg = gen_rnd_gnb_cu_conf_update();
  defer({ free_gnb_cu_conf_update(&msg); }); 

  F1AP_F1AP_PDU_t pdu = cp_gnb_cu_conf_update_asn(&msg);
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu); } );

  byte_array_t ba = encode_pdu_f1ap(&pdu);
  defer({ free_byte_array(ba); } );

  F1AP_F1AP_PDU_t pdu2 = decode_pdu_f1ap(ba); 
  defer({ ASN_STRUCT_RESET(asn_DEF_F1AP_F1AP_PDU, &pdu2); } ); 




}


int main()
{
  time_t t;
  srand((unsigned) time(&t));

  test_f1_setup_f1ap();
  test_f1_setup_response_f1ap();
  test_f1_setup_failure_f1ap();
  test_f1_ue_ctx_setup_request();
  test_f1_ue_ctx_setup_response();

  test_gnb_cu_conf_update();
//  test_gnb_cu_conf_update_ack();

//  test_dl_rrc_msg_trans();
//  test_ul_rrc_msg_trans();

//  test_ue_ctx_mod_req();



  // Class 2: 35 Procedures

  return EXIT_SUCCESS;
}

/*
// Class 1: 21 Procedures

1. F1 Setup Request/Response
2. UE Context Setup Req/Resp 
3. UE Context Release Req/Resp 
4. UE Context Modification Req/Resp 
5. Initial UL/DL RRC Message Transfer 


Reset
F1 Setup
gNB-DU Configuration Update
gNB-CU Configuration Update
UE Context Setup
UE Context Release (gNB-CU initiated)
UE Context Modification (gNB-CU initiated)
UE Context Modification Required (gNB-DU initiated)
Write-Replace Warning
PWS Cancel
gNB-DU Resource Coordination
F1 Removal
BAP Mapping Configuration
GNB-DU Resource Configuration
IAB TNL Address Allocation
IAB UP Configuration Update
Resource Status Reporting Initiation
Positioning Measurement
Positioning Information Exchange
TRP Information Exchange
Positioning Activation
E-CID Measurement Initiation

// Class 2:  Procedures
Error Indication
UE Context Release Request (gNB-DU initiated)
Initial UL RRC Message Transfer
DL RRC Message Transfer
UL RRC Message Transfer
UE Inactivity Notification
System Information Delivery
Paging
Notify
PWS Restart Indication
PWS Failure Indication
gNB-DU Status Indication
RRC Delivery Report
Network Access Rate Reduction
Trace Start
Deactivate Trace
DU-CU Radio Information Transfer
CU-DU Radio Information Transfer
Resource Status Reporting
Access And Mobility Indication
Reference Time Information
Reporting Control
Reference Time Information Report
Access Success
Cell Traffic Trace
Positioning Assistance Information Control
Positioning Assistance Information Feedback
Positioning Measurement Report
Positioning Measurement Abort
Positioning Measurement Failure Indication
Positioning Measurement Update
Positioning Deactivation
E-CID Measurement Failure Indication
E-CID Measurement Report
E-CID Measurement Termination
Positioning Information Update
*/

