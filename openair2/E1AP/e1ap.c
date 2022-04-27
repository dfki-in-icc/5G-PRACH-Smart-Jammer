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

#include "e1ap.h"

e1ap_message_processing_t e1ap_message_processing[][3] = {

  { 0, 0, 0 } /* Reset */
  { 0, 0, 0 } /* ErrorIndication */
  { 0, 0, 0 } /* privateMessage */
  { e1apCUCP_handle_SETUP_REQUEST, e1apCUUP_handle_SETUP_RESPONSE, e1apCUUP_handle_SETUP_FAILURE } /* gNBCUUPE1Setup */
  { 0, 0, 0 } /* gNBCUCPE1Setup */
  { 0, 0, 0 } /* gNBCUUPConfigurationUpdate */
  { 0, 0, 0 } /* gNBCUCPConfigurationUpdate */
  { 0, 0, 0 } /* E1Release */
  { 0, 0, 0 } /* bearerContextSetup */
  { 0, 0, 0 } /* bearerContextModification */
  { 0, 0, 0 } /* bearerContextModificationRequired */
  { 0, 0, 0 } /* bearerContextRelease */
  { 0, 0, 0 } /* bearerContextReleaseRequired */
  { 0, 0, 0 } /* bearerContextInactivityNotification */
};

int asn1_encoder_xer_print = 1;

int e1ap_assoc_id(bool isCu, instance_t instance) {
  return 0;
}

int e1ap_handle_message(instance_t instance, uint32_t assoc_id, int32_t stream,
                        const uint8_t *const data, const uint32_t data_length) {
  E1AP_E1AP_PDU_t pdu= {0};
  int ret;
  DevAssert(data != NULL);

  if (f1ap_decode_pdu(&pdu, data, data_length) < 0) {
    LOG_E(E1AP, "Failed to decode PDU\n");
    return -1;
  }

  /* Checking procedure Code and direction of message */
  if (pdu.choice.initiatingMessage->procedureCode >=
      sizeof(e1ap_messages_processing) /
      (3 * sizeof(e1ap_message_processing_t))
      || (pdu.present > E1AP_E1AP_PDU_PR_unsuccessfulOutcome)) {
    LOG_E(E1AP, "[SCTP %d] Either procedureCode %ld or direction %d exceed expected\n",
          assoc_id, pdu.choice.initiatingMessage->procedureCode, pdu.present);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E1AP_E1AP_PDU, &pdu);
    return -1;
  }

  if (e1ap_messages_processing[pdu.choice.initiatingMessage->procedureCode][pdu.present - 1] == NULL) {
    // No handler present. This can mean not implemented or no procedure for eNB (wrong direction).
    LOG_E(E1AP, "[SCTP %d] No handler for procedureCode %ld in %s\n",
          assoc_id, pdu.choice.initiatingMessage->procedureCode,
          f1ap_direction2String(pdu.present - 1));
    ret=-1;
  } else {
    /* Calling the right handler */
    LOG_I(E1AP, "Calling handler with instance %ld\n",instance);
    ret = (*e1ap_messages_processing[pdu.choice.initiatingMessage->procedureCode][pdu.present - 1])
          (instance, assoc_id, stream, &pdu);
  }

  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E1AP_E1AP_PDU, &pdu);
  return ret;
}

void cuup_task_handle_sctp_data_ind(instance_t instance, sctp_data_ind_t *sctp_data_ind) {
  int result;
  DevAssert(sctp_data_ind != NULL);
  e1ap_handle_message(instance, sctp_data_ind->assoc_id, sctp_data_ind->stream,
                      sctp_data_ind->buffer, sctp_data_ind->buffer_length);
  result = itti_free(TASK_UNKNOWN, sctp_data_ind->buffer);
  AssertFatal (result == EXIT_SUCCESS, "Failed to free memory (%d)!\n", result);
}

int e1ap_encode_send(bool isCu, instance_t instance, E1AP_E1AP_PDU_t *pdu, uint16_t stream, const char *func) {
  DevAssert(pdu != NULL);

  if (asn1_encoder_xer_print) {
    LOG_E(E1AP, "----------------- ASN1 ENCODER PRINT START ----------------- \n");
    xer_fprint(stdout, &asn_DEF_E1AP_E1AP_PDU, pdu);
    LOG_E(E1AP, "----------------- ASN1 ENCODER PRINT END----------------- \n");
  }

  char errbuf[2048]; /* Buffer for error message */
  size_t errlen = sizeof(errbuf); /* Size of the buffer */
  int ret = asn_check_constraints(&asn_DEF_E1AP_E1AP_PDU, pdu, errbuf, &errlen);

  if(ret) {
    fprintf(stderr, "%s: Constraint validation failed: %s\n", func, errbuf);
  }

  void *buffer = NULL;
  ssize_t encoded = aper_encode_to_new_buffer(&asn_DEF_E1AP_E1AP_PDU, 0, pdu, buffer);

  if (encoded < 0) {
    LOG_E(E1AP, "%s: Failed to encode E1AP message\n", func);
    return -1;
  } else {
    MessageDef *message = itti_alloc_new_message(isCu?TASK_CUCP_E1:TASK_CUUP_E1, 0, SCTP_DATA_REQ);
    sctp_data_req_t *s = &message->ittiMsg.sctp_data_req;
    s->assoc_id      = e1ap_assoc_id(isCu,instance);
    s->buffer        = buffer;
    s->buffer_length = encoded;
    s->stream        = stream;
    LOG_I(E1AP, "%s: Sending ITTI message to SCTP Task\n", func);
    itti_send_msg_to_task(TASK_SCTP, instance, message);
  }

  return encoded;
}

void e1ap_itti_send_sctp_close_association(bool isCu, instance_t instance) {
  MessageDef *message = itti_alloc_new_message(TASK_S1AP, 0, SCTP_CLOSE_ASSOCIATION);
  sctp_close_association_t *sctp_close_association = &message->ittiMsg.sctp_close_association;
  sctp_close_association->assoc_id      = e1ap_assoc_id(isCu,instance);
  itti_send_msg_to_task(TASK_SCTP, instance, message);
}

int e1ap_send_RESET(bool isCu, instance_t instance, E1AP_Reset_t *Reset) {
  AssertFatal(false,"Not implemented yet\n");
  E1AP_E1AP_PDU_t pdu= {0};
  return e1ap_encode_send(isCu, instance, &pdu,0, __func__);
}

int e1ap_send_RESET_ACKNOWLEDGE(instance_t instance, E1AP_Reset_t *Reset) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_handle_RESET(instance_t instance,
                      uint32_t assoc_id,
                      uint32_t stream,
                      E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_handle_RESET_ACKNOWLEDGE(instance_t instance,
                                  uint32_t assoc_id,
                                  uint32_t stream,
                                  E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

/*
    Error Indication
*/
int e1ap_handle_ERROR_INDICATION(instance_t instance,
                                 uint32_t assoc_id,
                                 uint32_t stream,
                                 E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_send_ERROR_INDICATION(instance_t instance, E1AP_ErrorIndication_t *ErrorIndication) {
  AssertFatal(false,"Not implemented yet\n");
}


/*
    E1 Setup: can be sent on both ways, to be refined
*/

int e1apCUUP_send_SETUP_REQUEST(instance_t instance, E1AP_Reset_t *Reset) {
  E1AP_E1AP_PDU_t pdu = {0};
  /* Create */
  /* 0. pdu Type */
  pdu.present = E1AP_E1AP_PDU_PR_initiatingMessage;
  asn1cCalloc(pdu.choice.initiatingMessage, initMsg);
  initMsg->procedureCode = E1AP_ProcedureCode_id_gNB_CU_UP_E1Setup;
  initMsg->criticality   = E1AP_Criticality_reject;
  initMsg->present       = E1AP_InitiatingMessage__value_PR_GNB_CU_UP_E1SetupRequest;
  E1AP_GNB_CU_UP_E1SetupRequest_t *e1SetupUP = &initMsg->value.choice.GNB_CU_UP_E1SetupRequest;
  /* mandatory */
  /* c1. Transaction ID (integer value) */
  asn1cSequenceAdd(e1SetupUP->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ieC1);
  ieC1->id                         = E1AP_ProtocolIE_ID_id_TransactionID;
  ieC1->criticality                = E1AP_Criticality_reject;
  ieC1->value.present              = E1AP_GNB_CU_UP_E1SetupRequestIEs__value_PR_TransactionID;
  ieC1->value.choice.TransactionID = E1AP_get_next_transaction_identifier();
  /* mandatory */
  /* c2. GNB_CU_ID (integer value) */
  asn1cSequenceAdd(e1SetupUP->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ieC2);
  ieC2->id                       = E1AP_ProtocolIE_ID_id_gNB_CU_UP_ID;
  ieC2->criticality              = E1AP_Criticality_reject;
  ieC2->value.present            = E1AP_GNB_CU_UP_E1SetupRequestIEs__value_PR_GNB_CU_UP_ID;
  asn_int642INTEGER(&ieC2->value.choice.GNB_CU_UP_ID, 0);
  /* mandatory */
  /* c4. CN Support */
  asn1cSequenceAdd(e1SetupUP->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ieC4);
  iec4->id = E1AP_ProtocolIE_ID_id_CNSupport;
  ieC4->criticality = E1AP_Criticality_reject;
  iec4->value.present = E1AP_GNB_CU_UP_E1SetupRequestIEs__value_PR_CNSupport;
  iec4->value.choice.CNSupport = E1AP_CNSupport_c_5gc;

  /* mandatory */
  /* c5. Supported PLMNs */
  asn1cSequenceAdd(e1SetupUP->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ieC5);
  iec5->id = E1AP_ProtocolIE_ID_id_SupportedPLMNs;
  iec5->criticality = E1AP_Criticality_reject;
  iec5->value.present = E1AP_GNB_CU_UP_E1SetupRequestIEs__value_PR_SupportedPLMNs_List;

  int numSupportedPLMNs = 1;

  for (int i=0; i < numSupportedPLMNs; i++) {
    asn1cSequenceAdd(iec5->value.choice.SupportedPLMNs_List.list, E1AP_SupportedPLMNs_Item_t, supportedPLMN);
    /* 5.1 PLMN Identity */
    OCTET_STRING_fromBuf(&supportedPLMN->pLMN_Identity, "OAI", strlen("OAI"));
  }

  e1ap_encode_send(0, instance, pdu, 0, __func__);
}

int e1apCUCP_send_SETUP_RESPONSE(instance_t instance) {
  E1AP_E1AP_PDU_t pdu = {0};
  /* Create */
  /* 0. pdu Type */
  pdu.present = E1AP_E1AP_PDU_PR_successfulOutcome;
  asn1cCalloc(pdu.choice.successfulOutcome, initMsg);
  initMsg->procedureCode = E1AP_ProcedureCode_id_gNB_CU_UP_E1Setup;
  initMsg->criticality = E1AP_Criticality_reject;
  initMsg->value.present = E1AP_SuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupResponse;
  E1AP_GNB_CU_UP_E1SetupResponse_t *out = &pdu.choice.successfulOutcome->value.choice.GNB_CU_UP_E1SetupResponse;
  /* mandatory */
  /* c1. Transaction ID (integer value) */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupResponseIEs, ieC1);
  ieC1->id                         = E1AP_ProtocolIE_ID_id_TransactionID;
  ieC1->criticality                = E1AP_Criticality_reject;
  ieC1->value.present              = E1AP_GNB_CU_UP_E1SetupResponseIEs__value_PR_TransactionID;
  ieC1->value.choice.TransactionID = //get this from stored transaction IDs in CU

  e1ap_encode_send(0, instance, pdu, 0, __func__);
}

int e1apCUCP_send_SETUP_FAILURE() {
  E1AP_E1AP_PDU_t pdu = {0};
  /* Create */
  /* 0. pdu Type */
  pdu.present = E1AP_E1AP_PDU_PR_unsuccessfulOutcome;
  asn1cCalloc(pdu.choice.unsuccessfulOutcome, initMsg);
  initMsg->procedureCode = E1AP_ProcedureCode_id_gNB_CU_UP_E1Setup;
  initMsg->criticality = E1AP_Criticality_reject;
  initMsg->value.present = E1AP_UnsuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupFailure;
  E1AP_GNB_CU_UP_E1SetupFailure_t *out = &pdu.choice.successfulOutcome->value.choice.GNB_CU_UP_E1SetupFailure;
  /* mandatory */
  /* c1. Transaction ID (integer value) */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupFailureIEs, ieC1);
  ieC1->id                         = E1AP_ProtocolIE_ID_id_TransactionID;
  ieC1->criticality                = E1AP_Criticality_reject;
  ieC1->value.present              = E1AP_GNB_CU_UP_E1SetupResponseIEs__value_PR_TransactionID;
  ieC1->value.choice.TransactionID = //get this from stored transaction IDs in CU
  /* mandatory */
  /* c2. cause (integer value) */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_E1SetupFailureIEs, ieC2);
  ieC2->id                         = E1AP_ProtocolIE_ID_id_Cause;
  ieC2->criticality                = E1AP_Criticality_ignore;
  ieC2->value.present              = E1AP_GNB_CU_UP_E1SetupFailureIEs__value_PR_Cause;
  ieC2->value.choice.Cause.present = E1AP_Cause_PR_radioNetwork; //choose this accordingly
  ieC2->value.choice.Cause.choice.ratioNetwork = E1AP_CauseRadioNetwork_unspecified;

  e1ap_encode_send(0, instance, pdu, 0, __func__);
}

int e1apCUCP_handle_SETUP_REQUEST(instance_t instance,
                                  uint32_t assoc_id,
                                  uint32_t stream,
                                  E1AP_E1AP_PDU_t *pdu) {

  E1AP_GNB_CU_UP_E1SetupRequestIEs_t *ie;
  DevAssert(pdu != NULL);
  E1AP_GNB_CU_UP_E1SetupRequest_t *in = pdu->choice.initiatingMessage->value.choice.GNB_CU_UP_E1SetupRequest;

  e1ap_setup_req_t *req = &getCxt(CPtype, instance)->setupReq;

  /* assoc_id */
  req->assoc_id = assoc_id;

  /* transac_id */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_TransactionID, true);
  asn_INTEGER2ulong(&ie->value.choice.TransactionID, &req->transac_id);
  LOG_D(E1AP, "gNB CU UP E1 setup request transaction ID: %d\n", req->transac_id);

  /* gNB CU UP ID */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_gNB_CU_UP_ID, true);
  asn_INTEGER2ulong(&ie->value.choice.GNB_CU_UP_ID, &req->gNB_cu_up_id);
  LOG_D(E1AP, "gNB CU UP ID: %d\n", req->gNB_cu_up_id);

  /* CN Support */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_CNSupport, true);
  asn_INTEGER2ulong(&ie->value.choice.GNB_CU_UP_ID, &req->cn_support);
  LOG_D(E1AP, "E1ap CN support: %d\n", req->cn_support);

  /* Supported PLMNs */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupRequestIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_SupportedPLMNs, true);
  req->supported_plmns = ie->value.choice.SupportedPLMNs_List.list.count;
  LOG_D(E1AP, "Number of supported PLMNs: %d\n", req->supported_plmns);

  for (int i=0; i < req->supported_plmns; i++) {
    E1AP_SupportedPLMNs_Item_t *supported_plmn_item = (E1AP_SupportedPLMNs_Item_t *)(ie->value.choice.SupportedPLMNs_List.list.array[i]);

    /* PLMN Identity */
    OCTET_STRING_TO_INT16(supported_plmn_item.pLMN_Identity, req->plmns[i].id);
    LOG_D(E1AP, "PLMN %d ID: %d\n", i, req->plmns[i].id);
  }

  /* Create ITTI message and send to queue */

}

int e1apCUUP_handle_SETUP_RESPONSE(instance_t instance,
                                   uint32_t assoc_id,
                                   uint32_t stream,
                                   E1AP_E1AP_PDU_t *pdu) {
  LOG_D(E1AP, "%s\n", __func__);
  AssertFatal(pdu->present == E1AP_E1AP_PDU_PR_successfulOutcome,
              "pdu->present != E1AP_E1AP_PDU_PR_successfulOutcome\n");
  AssertFatal(pdu->choice.successfulOutcome->procedureCode  == E1AP_ProcedureCode_id_gNB_CU_UP_E1Setup,
              "pdu->choice.successfulOutcome->procedureCode != E1AP_ProcedureCode_id_gNB_CU_UP_E1Setup\n");
  AssertFatal(pdu->choice.successfulOutcome->criticality  == E1AP_Criticality_reject,
              "pdu->choice.successfulOutcome->criticality != E1AP_Criticality_reject\n");
  AssertFatal(pdu->choice.successfulOutcome->value.present  == E1AP_SuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupResponse,
              "pdu->choice.successfulOutcome->value.present != E1AP_SuccessfulOutcome__value_PR_GNB_CU_UP_E1SetupResponse\n");

  E1AP_GNB_CU_UP_E1SetupResponse_t  *in = &pdu.choice.successfulOutcome->value.choice.GNB_CU_UP_E1SetupResponse;
  E1AP_GNB_CU_UP_E1SetupResponseIEs *ie;

  /* transac_id */
  int transaction_id;
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupResponseIEs, ie, in,
                             E1AP_ProtocolIE_ID_id_TransactionID, true);
  asn_INTEGER2ulong(&ie->value.choice.TransactionID, &transaction_id);
  LOG_D(E1AP, "gNB CU UP E1 setup response transaction ID: %d\n", transaction_id);

  /* do the required processing */

}

int e1apCUUP_handle_SETUP_FAILURE(instance_t instance,
                                  uint32_t assoc_id,
                                  uint32_t stream,
                                  E1AP_E1AP_PDU_t *pdu) {
  E1AP_GNB_CU_UP_E1SetupFailureIEs_t *ie;
  DevAssert(pdu != NULL);
  E1AP_GNB_CU_UP_E1SetupFailure_t *in = pdu->choice.unsuccessfulOutcome->value.choice.GNB_CU_UP_E1SetupFailure;
  /* Transaction ID */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupFailureIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_TransactionID, true);
  /* Cause */
  F1AP_FIND_PROTOCOLIE_BY_ID(E1AP_GNB_CU_UP_E1SetupFailureIEs_t, ie, in,
                             E1AP_ProtocolIE_ID_id_Cause, true);

  return 0;
}

/*
  E1 configuration update: can be sent in both ways, to be refined
*/

int e1apCUUP_send_CONFIGURATION_UPDATE(instance_t instance) {
  E1AP_E1AP_PDU_t pdu = {0};
  /* Create */
  /* 0. pdu Type */
  pdu.present = E1AP_E1AP_PDU_PR_initiatingMessage;
  asn1cCalloc(pdu.choice.initiatingMessage, msg);
  msg->procedureCode = E1AP_ProcedureCode_id_gNB_CU_UP_ConfigurationUpdate;
  msg->criticality   = E1AP_Criticality_reject;
  msg->value.present = E1AP_InitiatingMessage__value_PR_GNB_CU_UP_ConfigurationUpdate;
  E1AP_GNB_CU_UP_ConfigurationUpdate_t *out = &pdu.choice.successfulOutcome->value.choice.GNB_CU_UP_ConfigurationUpdate;
  /* mandatory */
  /* c1. Transaction ID (integer value) */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_ConfigurationUpdateIEs_t, ieC1);
  ieC1->id                         = E1AP_ProtocolIE_ID_id_TransactionID;
  ieC1->criticality                = E1AP_Criticality_reject;
  ieC1->value.present              = E1AP_GNB_CU_UP_ConfigurationUpdateIEs__value_PR_TransactionID;
  ieC1->value.choice.TransactionID = //get this from stored transaction IDs in CU
  /* mandatory */
  /* c2. Supported PLMNs */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_ConfigurationUpdateIEs_t, ieC2);
  iec2->id = E1AP_ProtocolIE_ID_id_SupportedPLMNs;
  iec2->criticality = E1AP_Criticality_reject;
  iec2->value.present = E1AP_GNB_CU_UP_ConfigurationUpdateIEs__value_PR_SupportedPLMNs_List;

  int numSupportedPLMNs = 1;

  for (int i=0; i < numSupportedPLMNs; i++) {
    asn1cSequenceAdd(iec2->value.choice.SupportedPLMNs_List.list, E1AP_SupportedPLMNs_Item_t, supportedPLMN);
    /* 5.1 PLMN Identity */
    OCTET_STRING_fromBuf(&supportedPLMN->pLMN_Identity, "OAI", strlen("OAI"));
  }

  /* mandatory */
  /* c3. TNLA to remove list */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_GNB_CU_UP_ConfigurationUpdateIEs_t, ieC3);
  iec3->id = E1AP_ProtocolIE_ID_id_GNB_CU_UP_TNLA_To_Remove_List;
  iec3->criticality = E1AP_Criticality_reject;
  iec3->value.present = E1AP_GNB_CU_UP_ConfigurationUpdateIEs__value_PR_GNB_CU_UP_TNLA_To_Remove_List;

  int numTNLAtoRemoveList = 1;

  for (int i=0; i < numTNLAtoRemoveList; i++) {
    asn1cSequenceAdd(iec2->value.choice.GNB_CU_UP_TNLA_To_Remove_List.list, E1AP_GNB_CU_UP_TNLA_To_Remove_Item_t, TNLAtoRemove);
    TNLAtoRemove->tNLAssociationTransportLayerAddress.present = E1AP_CP_TNL_Information_PR_endpoint_IP_Address;
    TNLAtoRemove->tNLAssociationTransportLayerAddress.choice.endpoint_IP_Address = // TODO: include the ip
  }

  e1ap_encode_send(0, instance, pdu, 0, __func__);
}

int e1apCUCP_send_gNB_DU_CONFIGURATION_FAILURE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_send_gNB_DU_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_CONFIGURATION_UPDATE(instance_t instance,
    uint32_t assoc_id,
    uint32_t stream,
    E1AP_E1AP_PDU_t *pdu) {

  E1AP_GNB_CU_UP_E1SetupRequestIEs_t *ie;
  DevAssert(pdu != NULL);
  E1AP_GNB_CU_UP_E1SetupRequest_t *in = pdu->choice.initiatingMessage->value.choice.GNB_CU_UP_E1SetupRequest;

  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_gNB_DU_CONFIGURATION_UPDATE_ACKNOWLEDGE(instance_t instance,
    uint32_t assoc_id,
    uint32_t stream,
    E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_gNB_DU_CONFIGURATION_FAILURE(instance_t instance,
    uint32_t assoc_id,
    uint32_t stream,
    E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

/*
  E1 release
*/

int e1ap_send_RELEASE_REQUEST(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_send_RELEASE_ACKNOWLEDGE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_handle_RELEASE_REQUEST(instance_t instance,
                                uint32_t assoc_id,
                                uint32_t stream,
                                E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1ap_handle_RELEASE_ACKNOWLEDGE(instance_t instance,
                                    uint32_t assoc_id,
                                    uint32_t stream,
                                    E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

/*
  BEARER CONTEXT SETUP REQUEST
*/

int e1apCUCP_send_BEARER_CONTEXT_SETUP_REQUEST(instance_t instance) {
  E1AP_E1AP_PDU_t pdu = {0};
  e1ap_bearer_setup_req_t bearerCxt = &getCxt(instance)->bearerSetupReq;
  /* Create */
  /* 0. pdu Type */
  pdu.present = E1AP_E1AP_PDU_PR_initiatingMessage;
  asn1cCalloc(pdu.choice.initiatingMessage, msg);
  msg->procedureCode = E1AP_ProcedureCode_id_bearerContextSetup;
  msg->criticality   = E1AP_Criticality_reject;
  msg->value.present = E1AP_InitiatingMessage__value_PR_BearerContextSetupRequest;
  E1AP_BearerContextSetupRequest_t *out = &pdu.choice.successfulOutcome->value.choice.BearerContextSetupRequest;
  /* mandatory */
  /* c1. gNB-CU-UP UE E1AP ID */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC1);
  ieC1->id                         = E1AP_ProtocolIE_ID_id_gNB_CU_CP_UE_E1AP_ID;
  ieC1->criticality                = E1AP_Criticality_reject;
  ieC1->value.present              = E1AP_BearerContextSetupRequestIEs__value_PR_GNB_CU_CP_UE_E1AP_ID;
  ieC1->value.choice.GNB_CU_CP_UE_E1AP_ID = bearerCxt->gNB_cu_cp_ue_id;
  /* mandatory */
  /* c2. Security Information */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC2);
  ieC2->id                         = E1AP_ProtocolIE_ID_id_SecurityInformation;
  ieC2->criticality                = E1AP_Criticality_reject;
  ieC2->value.present              = E1AP_BearerContextSetupRequestIEs__value_PR_SecurityInformation;
  ieC2->value.choice.SecurityInformation.securityAlgorithm.cipheringAlgorithm = bearerCxt->cipheringAlgorithm;
  OCTET_STRING_fromBuf(&ieC2->value.choice.SecurityInformation.uPSecuritykey.encryptionKey, bearerCxt->cipheringAlgorithm, strlen(bearerCxt->cipheringAlgorithm));
  /* mandatory */
  /* c3. UE DL Aggregate Maximum Bit Rate */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC3);
  ieC3->id                         = E1AP_ProtocolIE_ID_id_UEDLAggregateMaximumBitRate;
  ieC3->criticality                = E1AP_Criticality_reject;
  ieC3->value.present              = E1AP_BearerContextSetupRequestIEs__value_PR_BitRate;
  asn_long2INTEGER(&ieC3->value.choice.BitRate, bearerCxt->bitRate);
  /* mandatory */
  /* c4. Serving PLMN */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC4);
  ieC4->id                         = E1AP_ProtocolIE_ID_id_Serving_PLMN;
  ieC4->criticality                = E1AP_Criticality_ignore;
  ieC4->value.present              = E1AP_BearerContextSetupRequestIEs__value_PR_PLMN_Identity;
  uint64_t servingPLMNId = getServingPlmn(); // TODO: define this
  OCTET_STRING_fromBuf(&ieC4->value.choice.PLMN_Identity, servingPLMNId);
  /* mandatory */
  /* Activity Notification Level */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC5);
  ieC5->id                         = E1AP_ProtocolIE_ID_id_ActivityNotificationLevel;
  ieC5->criticality                = E1AP_Criticality_reject;
  ieC5->value.present              = E1AP_BearerContextSetupRequestIEs__value_PR_ActivityNotificationLevel;
  ieC5->value.choice.ActivityNotificationLevel = bearerCxt->activityNotificationLevel;
  /* mandatory */
  /*  */
  asn1cSequenceAdd(out->protocolIEs.list, E1AP_BearerContextSetupRequestIEs_t, ieC6);
  ieC6->id = E1AP_ProtocolIE_ID_id_System_BearerContextSetupRequest;
  ieC6->criticality = E1AP_Criticality_reject;
  if (0) { // EUTRAN
    ieC6->System_BearerContextSetupRequest.present = E1AP_System_BearerContextSetupRequest_PR_e_UTRAN_BearerContextSetupRequest;
    E1AP_EUTRAN_BearerContextSetupRequest_t *msgEUTRAN = calloc(1, sizeof(E1AP_EUTRAN_BearerContextSetupRequest_t));
    msgEUTRAN.id = E1AP_ProtocolIE_ID_id_DRB_To_Setup_List_EUTRAN;
    msgEUTRAN->value.present = E1AP_EUTRAN_BearerContextSetupRequest__value_PR_DRB_To_Setup_List_EUTRAN;
    E1AP_DRB_To_Setup_List_EUTRAN_t *drb2Setup = &msgEUTRAN->value.choice.DRB_To_Setup_List_EUTRAN;

    for (int i=0; i < bearerCxt->maxNumDRBs; i++) {
      asn1cSequenceAdd(drb2Setup->list, E1AP_DRB_To_Setup_Item_EUTRAN, ieC6_1);
      ieC6_1->dRB_ID = bearerCxt->DRBList[i].drbId;

      ieC6_1->pDPC_Configuration.pDCP_SN_Size_UL = bearerCxt->DRBList[i].pDCP_SN_Size_UL;
      ieC6_1->pDPC_Configuration.pDCP_SN_Size_DL = bearerCxt->DRBList[i].pDCP_SN_Size_DL;
      ieC6_1->pDPC_Configuration.rLC_Mode        = bearerCxt->DRBList[i].rLC_Mode;

      ieC6_1->eUTRAN_QoS.qCI = bearerCxt->DRBList[i].qci;
      ieC6_1->eUTRAN_QoS.eUTRANallocationAndRetentionPriority.priorityLevel = bearerCxt->DRBList[i].qosPriorityLevel;
      ieC6_1->eUTRAN_QoS.eUTRANallocationAndRetentionPriority.pre_emptionCapability = bearerCxt->DRBList[i].preEmptionCapability;
      ieC6_1->eUTRAN_QoS.eUTRANallocationAndRetentionPriority.pre_emptionVulnerability = bearerCxt->DRBList[i].preEmptionVulnerability;

      ieC6_1->s1_UL_UP_TNL_Information.present = E1AP_UP_TNL_Information_PR_gTPTunnel;
      asn1cCalloc(ieC6_1->s1_UL_UP_TNL_Information.choice.gTPTunnel, gTPTunnel);
      TRANSPORT_LAYER_ADDRESS_IPv4_TO_BIT_STRING(bearerCxt->DRBList[i].tlAddress, &gTPTunnel->transportLayerAddress);
      INT32_TO_OCTET_STRING(bearerCxt->DRBList[i].teId, &gTPTunnel->gTP_TEID);

      for (int j=0; j < bearerCxt->DRBList[i].maxNumCellGroups; j++) {
        asn1cSequenceAdd(ieC6_1->cell_Group_Information.list, E1AP_Cell_Group_Information_Item, ieC6_1_1);
        ieC6_1_1->cell_Group_ID = bearerCxt->DRBList[i].cellGroupList[j].id;
      }
    }
    ieC6->System_BearerContextSetupRequest.choice.e_UTRAN_BearerContextSetupRequest = (E1AP_ProtocolIE_Container *) msgEUTRAN;
  } else {
    /* mandatory */
    /*  */
    ieC6->System_BearerContextSetupRequest.present = E1AP_System_BearerContextSetupRequest_PR_nG_RAN_BearerContextSetupRequest;
    E1AP_NG_RAN_BearerContextSetupRequest_t *msgNGRAN = calloc(1, sizeof(E1AP_NG_RAN_BearerContextSetupRequest_t));
    msgNGRAN->id = E1AP_ProtocolIE_ID_id_PDU_Session_Resource_To_Setup_List;
    msgNGRAN->value.present = E1AP_NG_RAN_BearerContextSetupRequest__value_PR_PDU_Session_Resource_To_Setup_List;
    E1AP_PDU_Session_Resource_To_Setup_List_t *pdu2Setup = &msgNGRAN->value.choice.PDU_Session_Resource_To_Setup_List;

    for(int i=0; i < bearerCxt->maxNumPDUSessions; i++) {
      asn1cSequenceAdd(pdu2Setup->list, E1AP_PDU_Session_Resource_To_Setup_Item, ieC6_1);
      ieC6_1->pDU_Session_ID = bearerCxt->pduSession[i].sessionId;

      ieC6_1->pDU_Session_Type = bearerCxt->pduSession[i].sessionType;

      INT32_TO_OCTET_STRING(bearerCxt->pduSession[i].sst, &ieC6_1->sNSSAI.sST);

      ieC6_1->securityIndication.integrityProtectionIndication = bearerCxt->pduSession[i].integrityProtectionIndication;
      ieC6_1->securityIndication.confidentialityProtectionIndication = bearerCxt->pduSession[i].confidentialityProtectionIndication;

      ieC6_1->nG_UL_UP_TNL_Information.present = E1AP_UP_TNL_Information_PR_gTPTunnel;
      asn1cCalloc(ieC6_1->nG_UL_UP_TNL_Information.choice.gTPTunnel, gTPTunnel);
      TRANSPORT_LAYER_ADDRESS_IPv4_TO_BIT_STRING(bearerCxt->pduSession[i].tlAddress, &gTPTunnel->transportLayerAddress);
      INT32_TO_OCTET_STRING(bearerCxt->pduSession[i].teId, &gTPTunnel->gTP_TEID);

      for (int j=0; j < bearerCxt->pduSession[i].numDRB2Setup; j++) {
        asn1cSequenceAdd(ieC6_1->dRB_To_Setup_List_NG_RAN.list, E1AP_DRB_To_Setup_Item_NG_RAN, ieC6_1_1);
        ieC6_1_1->dRB_ID = bearerCxt->pduSession[i].DRBnGRanList[j].id;

        ieC6_1_1->sDAP_Configuration.defaultDRB = bearerCxt->pduSession[i].DRBnGRanList[j].defaultDRB;
        ieC6_1_1->sDAP_Configuration.sDAP_Header_UL = bearerCxt->pduSession[i].DRBnGRanList[j].sDAP_Header_UL;
        ieC6_1_1->sDAP_Configuration.sDAP_Header_DL = bearerCxt->pduSession[i].DRBnGRanList[j].sDAP_Header_DL;

        for (int k=0; k < bearerCxt->pduSession[i].DRBnGRanList[j].maxNumCellGroups; k++) {
          asn1cSequenceAdd(ieC6_1_1->cell_Group_Information.list, E1AP_Cell_Group_Information_Item, ieC6_1_1_1);
          ieC6_1_1_1->cell_Group_ID = bearerCxt->pduSession[i].DRBnGRanList[j].cellGroupList[k].id;
        }

        for (int k=0; k < bearerCxt->pduSession[i].DRBnGRanList[j].numQosFlowSetup; k++) {
          asn1cSequenceAdd(ieC6_1_1->qos_flow_Information_To_Be_Setup, E1AP_QoS_Flow_QoS_Parameter_Item, ieC6_1_1_1);
          ieC6_1_1_1->qoS_Flow_Identifier = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].id;

          if (0) { // non Dynamic 5QI
            ieC6_1_1_1->qoSFlowLevelQoSParameters.qoS_Characteristics.present = E1AP_QoS_Characteristics_PR_non_Dynamic_5QI;
            asn1cCalloc(ieC6_1_1_1->qoSFlowLevelQoSParameters.qoS_Characteristics.choice.non_Dynamic_5QI, non_Dynamic_5QI);
            non_Dynamic_5QI->fiveQI = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].fiveQI;
          } else { // dynamic 5QI
            ieC6_1_1_1->qoSFlowLevelQoSParameters.qoS_Characteristics.present = E1AP_QoS_Characteristics_PR_dynamic_5QI;
            asn1cCalloc(ieC6_1_1_1->qoSFlowLevelQoSParameters.qoS_Characteristics.choice.dynamic_5QI, dynamic_5QI);
            dynamic_5QI->qoSPriorityLevel = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].qoSPriorityLevel;
            dynamic_5QI->packetDelayBudget = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].packetDelayBudget;
            dynamic_5QI->packetErrorRate.pER_Scalar = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].packetError_scalar;
            dynamic_5QI->packetErrorRate.pER_Exponent = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].packetError_exponent;
          }

          ieC6_1_1_1->qoSFlowLevelQoSParameters.nGRANallocationRetentionPriority.priorityLevel = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].priorityLevel;
          ieC6_1_1_1->qoSFlowLevelQoSParameters.nGRANallocationRetentionPriority.pre_emptionCapability = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].pre_emptionCapability;
          ieC6_1_1_1->qoSFlowLevelQoSParameters.nGRANallocationRetentionPriority.pre_emptionVulnerability = bearerCxt->pduSession[i].DRBnGRanList[j].qosFlows[k].pre_emptionVulnerability;
        }
      }
    }
    ieC6->System_BearerContextSetupRequest.choice.nG_RAN_BearerContextSetupRequest = (E1AP_ProtocolIE_Container *) msgNGRAN;
  }

  e1ap_encode_send(0, instance, pdu, 0, __func__);
}

int e1apCUUP_send_BEARER_CONTEXT_SETUP_RESPONSE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_SETUP_FAILURE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_BEARER_CONTEXT_SETUP_REQUEST(instance_t instance,
                                                 uint32_t assoc_id,
                                                 uint32_t stream,
                                                 E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_RESPONSE(instance_t instance,
                                                  uint32_t assoc_id,
                                                  uint32_t stream,
                                                  E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_SETUP_FAILURE(instance_t instance,
                                                 uint32_t assoc_id,
                                                 uint32_t stream,
                                                 E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

/*
  BEARER CONTEXT MODIFICATION REQUEST
*/

int e1apCUCP_send_BEARER_CONTEXT_MODIFICATION_REQUEST(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_MODIFICATION_RESPONSE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_MODIFICATION_FAILURE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_BEARER_CONTEXT_MODIFICATION_REQUEST(instance_t instance,
                                                        uint32_t assoc_id,
                                                        uint32_t stream,
                                                        E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_MODIFICATION_RESPONSE(instance_t instance,
                                                         uint32_t assoc_id,
                                                         uint32_t stream,
                                                         E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_MODIFICATION_FAILURE(instance_t instance,
                                                        uint32_t assoc_id,
                                                        uint32_t stream,
                                                        E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_MODIFICATION_REQUIRED(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_send_BEARER_CONTEXT_MODIFICATION_CONFIRM(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_MODIFICATION_REQUIRED(instance_t instance,
                                                         uint32_t assoc_id,
                                                         uint32_t stream,
                                                         E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_BEARER_CONTEXT_MODIFICATION_CONFIRM(instance_t instance,
                                                        uint32_t assoc_id,
                                                        uint32_t stream,
                                                        E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}
/*
  BEARER CONTEXT RELEASE
*/

int e1apCUCP_send_BEARER_CONTEXT_RELEASE_COMMAND(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_RELEASE_COMPLETE(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_BEARER_CONTEXT_RELEASE_REQUEST(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_handle_BEARER_CONTEXT_RELEASE_COMMAND(instance_t instance,
                                                   uint32_t assoc_id,
                                                   uint32_t stream,
                                                   E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_RELEASE_COMPLETE(instance_t instance,
                                                    uint32_t assoc_id,
                                                    uint32_t stream,
                                                    E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_RELEASE_REQUEST(instance_t instance,
                                                   uint32_t assoc_id,
                                                   uint32_t stream,
                                                   E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

/*
BEARER CONTEXT INACTIVITY NOTIFICATION
 */

int e1apCUUP_send_BEARER_CONTEXT_INACTIVITY_NOTIFICATION(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_BEARER_CONTEXT_INACTIVITY_NOTIFICATION(instance_t instance,
                                                           uint32_t assoc_id,
                                                           uint32_t stream,
                                                           E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}
/*
  DL DATA
*/

int e1apCUUP_send_DL_DATA_NOTIFICATION(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUUP_send_DATA_USAGE_REPORT(instance_t instance) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_DL_DATA_NOTIFICATION(instance_t instance,
                                         uint32_t assoc_id,
                                         uint32_t stream,
                                         E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

int e1apCUCP_handle_send_DATA_USAGE_REPORT(instance_t instance,
                                           uint32_t assoc_id,
                                           uint32_t stream,
                                           E1AP_E1AP_PDU_t *pdu) {
  AssertFatal(false,"Not implemented yet\n");
}

void cuup_task_send_sctp_association_req(instance_t instance, e1ap_setup_req_t *e1ap_setup_req) {
  DevAssert(e1ap_setup_req != NULL);
  MessageDef                 *message_p                   = NULL;
  sctp_new_association_req_t *sctp_new_association_req_p  = NULL;
  message_p = itti_alloc_new_message(TASK_CUUP_E1, 0, SCTP_NEW_ASSOCIATION_REQ);
  sctp_new_association_req_p = &message_p->ittiMsg.sctp_new_association_req;
  sctp_new_association_req_p->ulp_cnx_id = instance;
  sctp_new_association_req_p->port = E1AP_PORT_NUMBER;
  sctp_new_association_req_p->ppid = E1AP_SCTP_PPID;
  sctp_new_association_req_p->in_streams  = e1ap_setup_req->sctp_in_streams;
  sctp_new_association_req_p->out_streams = e1ap_setup_req->sctp_out_streams;
  // remote
  memcpy(&sctp_new_association_req_p->remote_address,
         &e1ap_setup_req->CUCP_e1_ip_address,
         sizeof(e1ap_setup_req->CUCP_e1_ip_address));
  // local
  memcpy(&sctp_new_association_req_p->local_address,
         &e1ap_setup_req->CUUP_e1_ip_address,
         sizeof(e1ap_setup_req->CUUP_e1_ip_address));
  itti_send_msg_to_task(TASK_SCTP, instance, message_p);
}

void cuup_task_handle_sctp_association_resp(instance_t instance, sctp_new_association_resp_t sctp_new_association_resp) {
  DevAssert(sctp_new_association_resp != NULL);

  if (sctp_new_association_resp->sctp_state != SCTP_STATE_ESTABLISHED) {
    LOG_W(E1AP, "Received unsuccessful result for SCTP association (%u), instance %ld, cnx_id %u\n",
          sctp_new_association_resp->sctp_state,
          instance,
          sctp_new_association_resp->ulp_cnx_id);
    return;
  }

  e1ap_setup_req_t *e1ap_cuup_setup_req       = &getCxt(UPtype, instance)->setupReq;
  e1ap_cuup_setup_req->assoc_id               = sctp_new_association_resp->assoc_id;
  e1ap_cuup_setup_req->sctp_in_streams        = sctp_new_association_resp->in_streams;
  e1ap_cuup_setup_req->sctp_out_streams       = sctp_new_association_resp->out_streams;
  e1ap_cuup_setup_req->default_sctp_stream_id = 0;

  e1apCUUP_send_SETUP_REQUEST(instance);
}

void *E1AP_CUUP_task(void *arg) {
  LOG_I(E1AP, "Starting E1AP at CU UP\n");

  // SCTP
  while (1) {
    MessageDef *msg = NULL;
    itti_receive_msg(TASK_CUUP_E1, &msg);
    instance_t myInstance=ITTI_MSG_DESTINATION_INSTANCE(msg);

    switch (ITTI_MSG_ID(msg)) {
      case E1AP_SETUP_REQ:
        LOG_I(E1AP, "CUUP Task Received E1AP_SETUP_REQ\n");
        e1ap_setup_req_t *msgSetup = &E1AP_SETUP_REQ(msg);
        createE1inst(UPtype, instance, msgSetup);

        cuup_task_send_sctp_association_req(instance, msgSetup);
        break;

      case SCTP_NEW_ASSOCIATION_RESP:
        LOG_I(E1AP, "CUUP Task Received SCTP_NEW_ASSOCIATION_RESP\n");
        cuup_task_handle_sctp_association_resp(instance, &msg->ittiMsg.sctp_new_association_resp);
        break;

      case SCTP_DATA_IND:
        LOG_I(E1AP, "CUUP Task Received SCTP_DATA_IND\n");
        cuup_task_handle_sctp_data_ind(instance, &msg->ittiMsg.sctp_data_ind);
        break;
    }
  }
}


