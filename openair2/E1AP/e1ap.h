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

#ifndef __E1AP_H_
#define __E1AP_H_

#include <common/utils/LOG/log.h>
#include "openairinterface5g_limits.h"
#include <openair2/RRC/NR/MESSAGES/asn1_msg.h>
#include "openair2/COMMON/e1ap_messages_types.h"
#include "openair3/UTILS/conversions.h"
#include "openair2/F1AP/f1ap_common.h"

#include <E1AP_Cause.h>
#include <E1AP_InitiatingMessage.h>
#include <E1AP_E1AP-PDU.h>
#include <E1AP_ProtocolIE-Field.h>
#include <E1AP_SupportedPLMNs-Item.h>
#include <E1AP_SuccessfulOutcome.h>
#include <E1AP_ProtocolIE-Field.h>
#include <E1AP_UnsuccessfulOutcome.h>
#include <E1AP_GNB-CU-UP-E1SetupFailure.h>
#include <E1AP_GNB-CU-UP-ConfigurationUpdate.h>
#include <E1AP_GNB-CU-UP-TNLA-To-Remove-Item.h>
#include <E1AP_CP-TNL-Information.h>
#include <E1AP_UP-Parameters-Item.h>
#include <E1AP_UP-TNL-Information.h>
#include <E1AP_PDU-Session-Resource-Setup-Item.h>
#include <E1AP_DRB-Setup-Item-EUTRAN.h>
#include <E1AP_DRB-Setup-Item-NG-RAN.h>
#include <E1AP_QoS-Flow-QoS-Parameter-Item.h>
#include <E1AP_QoS-Flow-Item.h>
#include <E1AP_DRB-Failed-List-NG-RAN.h>
#include <E1AP_DRB-Failed-Item-NG-RAN.h>
#include <E1AP_BearerContextSetupResponse.h>
#include <E1AP_BearerContextSetupRequest.h>
#include <E1AP_DRB-To-Setup-Item-EUTRAN.h>
#include <E1AP_DRB-To-Setup-Item-NG-RAN.h>
#include <E1AP_Cell-Group-Information-Item.h>
#include <E1AP_PDU-Session-Resource-To-Setup-Item.h>
#include <E1AP_GTPTunnel.h>
#include <E1AP_Non-Dynamic5QIDescriptor.h>
#include <E1AP_Dynamic5QIDescriptor.h>
#endif


int e1apCUCP_handle_SETUP_REQUEST(instance_t instance,
                                  E1AP_E1AP_PDU_t *pdu);

int e1apCUUP_handle_SETUP_RESPONSE(instance_t instance,
                                   E1AP_E1AP_PDU_t *pdu);

int e1apCUUP_handle_SETUP_FAILURE(instance_t instance,
                                  E1AP_E1AP_PDU_t *pdu);

int e1apCUUP_handle_BEARER_CONTEXT_SETUP_REQUEST(instance_t instance,
                                                 E1AP_E1AP_PDU_t *pdu);

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_RESPONSE(instance_t instance,
                                                  E1AP_E1AP_PDU_t *pdu);

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_FAILURE(instance_t instance,
                                                 E1AP_E1AP_PDU_t *pdu);

void *E1AP_CUUP_task(void *arg);

void *E1AP_CUCP_task(void *arg);
