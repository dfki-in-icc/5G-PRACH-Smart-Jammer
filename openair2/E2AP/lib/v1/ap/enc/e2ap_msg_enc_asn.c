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

#include "e2ap_msg_enc_asn.h"

#include "E2AP_E2AP-PDU.h"
#include "E2AP_ProtocolIE-Field.h"
#include "E2AP_Cause.h"
#include "E2AP_CriticalityDiagnostics.h"
#include "E2AP_TimeToWait.h"
#include "E2AP_TNLinformation.h"

#include "E2AP_RICindicationType.h"
#include "E2AP_RICcontrolAckRequest.h"
#include "E2AP_RICcontrolStatus.h"

#include "E2AP_RANfunction-Item.h"
#include "E2AP_RICsubsequentAction.h"
#include "E2AP_RICaction-ToBeSetup-Item.h"
#include "E2AP_E2nodeComponentConfigUpdate-Item.h"
#include "E2AP_E2connectionUpdate-Item.h"

#include "E2AP_InitiatingMessage.h"
#include "E2AP_SuccessfulOutcome.h"
#include "E2AP_UnsuccessfulOutcome.h"
#include "E2AP_GlobalE2node-eNB-ID.h"
#include "E2AP_GlobalE2node-gNB-ID.h"

#include "E2AP_E2nodeComponentGNB-CU-UP-ID.h"
#include "E2AP_E2nodeComponentGNB-DU-ID.h"
#include "E2AP_E2nodeComponentID.h"
#include "E2AP_E2nodeComponentConfigUpdateENgNB.h"
#include "E2AP_E2nodeComponentConfigUpdateGNB.h"
#include "E2AP_E2nodeComponentConfigUpdateNGeNB.h"
#include "E2AP_E2nodeComponentConfigUpdateENB.h"

#include <assert.h>

#include "util/conversions.h"
#include "lib/v1/ap/global_consts.h"

static void free_pdu(E2AP_E2AP_PDU_t* pdu)
{
  assert(pdu != NULL);
  ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2AP_E2AP_PDU, pdu);
  free(pdu);
}

static bool encode(byte_array_t* b, const E2AP_E2AP_PDU_t* pdu)
{
  assert(pdu != NULL);
  assert(b->buf != NULL);
  // xer_fprint(stderr, &asn_DEF_E2AP_E2AP_PDU, pdu);
  const enum asn_transfer_syntax syntax = ATS_ALIGNED_BASIC_PER;
  asn_enc_rval_t er = asn_encode_to_buffer(NULL, syntax, &asn_DEF_E2AP_E2AP_PDU, pdu, b->buf, b->len);
  assert(er.encoded < (ssize_t) b->len);
  if(er.encoded == -1) {
    printf("Failed the encoding in type %s and xml_type = %s\n", er.failed_type->name, er.failed_type->xml_tag);
    fflush(stdout);
    return false;
  }
  assert(er.encoded > -1);
  b->len = er.encoded;
  return true;
}

/* TODO: our type does exactly match the E2 types */
/*static
E2nodeComponentType_t get_e2nodeComponentType(ngran_node_t type)
{
  switch (type) {
    case ngran_eNB:       return E2nodeComponentType_eNB;
    case ngran_ng_eNB:    return E2nodeComponentType_en_gNB;
    case ngran_gNB:       return E2nodeComponentType_gNB;
    case ngran_eNB_CU:    assert(0 && "unhandled type");
    case ngran_ng_eNB_CU: return E2nodeComponentType_ng_eNB;    // does not match exactly!
    case ngran_gNB_CU:    return E2nodeComponentType_gNB_CU_UP; // does not match exactly!
    case ngran_eNB_DU:    assert(0);
    case ngran_gNB_DU:    return E2nodeComponentType_gNB_DU;
    case ngran_eNB_MBMS_STA: assert(0);
  }
  return E2nodeComponentType_eNB;
}
*/

static inline
OCTET_STRING_t copy_ba_to_ostring(byte_array_t ba)
{
  OCTET_STRING_t os = { .size = ba.len }; 
  os.buf = malloc(ba.len);
  memcpy(os.buf, ba.buf, ba.len);
  return os;
}
static inline
BIT_STRING_t	copy_ba_to_bit_string(byte_array_t ba)
{
  BIT_STRING_t bs;
  memset(&bs, 0, sizeof(BIT_STRING_t));
  
  bs.buf = calloc(1, ba.len);
  memcpy(bs.buf, ba.buf, ba.len);
  bs.size = ba.len;
  bs.bits_unused = 0; // FixME

  return bs;
}

static inline E2AP_Cause_t copy_cause(cause_t src)
{
  E2AP_Cause_t dst;
  memset(&dst, 0, sizeof(E2AP_Cause_t));
  switch(src.present) {
    case CAUSE_NOTHING: {
                          assert(0 != 0 && "Not Implemented!");
                          break;
                        }
    case CAUSE_RICREQUEST: {
                             assert(src.ricRequest < 11);
                             dst.present = E2AP_Cause_PR_ricRequest;
                             dst.choice.ricRequest = src.ricRequest; 
                             break;
                           }
    case CAUSE_RICSERVICE: {
                             assert(src.ricService < 3);
                             dst.present = E2AP_Cause_PR_ricService;
                             dst.choice.ricService = src.ricService;
                             break;
                           }
    case CAUSE_TRANSPORT:{
                           assert(src.transport < 2);
                           dst.present = E2AP_Cause_PR_transport;
                           dst.choice.transport = src.transport;
                           break;
                         }
    case CAUSE_PROTOCOL: {

                           assert(src.protocol < 7);
                           dst.present = E2AP_Cause_PR_protocol;
                           dst.choice.protocol = src.protocol;
                           break;
                         }
    case CAUSE_MISC:{
                      assert(src.misc < 4);
                      dst.present = E2AP_Cause_PR_misc;
                      dst.choice.misc = src.misc; 
                      break;
                    }
    default: {
               assert(0!= 0 && "Invalid code path. Error caused assigned");
               break;
             }
  }

  return dst;
}

static inline E2AP_RANfunction_Item_t copy_ran_function(const ran_function_t* src)
{
  E2AP_RANfunction_Item_t dst;
  memset(&dst, 0, sizeof(E2AP_RANfunction_Item_t));
  dst.ranFunctionID = src->id;
  dst.ranFunctionRevision = src->rev;
  dst.ranFunctionDefinition = copy_ba_to_ostring(src->def);
  if(src->oid != NULL){
    dst.ranFunctionOID = malloc(sizeof(E2AP_RANfunctionOID_t));
    *dst.ranFunctionOID = copy_ba_to_ostring(*src->oid);
  }
  return dst;
}

static inline E2AP_E2nodeComponentConfigUpdate_ItemIEs_t* copy_e2_node_component_conf_update(const e2_node_component_config_update_t* src)
{
  E2AP_E2nodeComponentConfigUpdate_ItemIEs_t* comp_update_item_ie = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdate_ItemIEs_t));
  comp_update_item_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate_Item;
  comp_update_item_ie->criticality = E2AP_Criticality_reject;
  comp_update_item_ie->value.present = E2AP_E2nodeComponentConfigUpdate_ItemIEs__value_PR_E2nodeComponentConfigUpdate_Item;

  E2AP_E2nodeComponentConfigUpdate_Item_t* ccui = &comp_update_item_ie->value.choice.E2nodeComponentConfigUpdate_Item;
  // E2 Node Component Type. Mandatory
  ccui->e2nodeComponentType = src-> e2_node_component_type;

  // E2 Node Component ID. Optional
  if (src->id_present != NULL) {
    ccui->e2nodeComponentID = calloc(1, sizeof(E2AP_E2nodeComponentID_t));
    switch (*src->id_present) {
      case E2_NODE_COMPONENT_ID_E2_NODE_COMPONENT_TYPE_GNB_CU_UP:
        ccui->e2nodeComponentID->present = E2AP_E2nodeComponentID_PR_e2nodeComponentTypeGNB_CU_UP;
        ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_CU_UP = calloc(1, sizeof(*ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_CU_UP));
        E2AP_GNB_CU_UP_ID_t* gnb_cu_up_id = &ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_CU_UP->gNB_CU_UP_ID;
        asn_uint642INTEGER(gnb_cu_up_id, src->gnb_cu_up_id);
        break;
      case E2_NODE_COMPONENT_ID_E2_NODE_COMPONENT_TYPE_GNB_DU:
        ccui->e2nodeComponentID->present = E2AP_E2nodeComponentID_PR_e2nodeComponentTypeGNB_DU;
        ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_DU = calloc(1, sizeof(*ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_DU));
        E2AP_GNB_DU_ID_t* gnb_du_id = &ccui->e2nodeComponentID->choice.e2nodeComponentTypeGNB_DU->gNB_DU_ID;
        asn_uint642INTEGER(gnb_du_id, src->gnb_du_id);
        break;
    }
  }
  // E2 Node Component Configuration Update. Mandatory
  E2AP_E2nodeComponentConfigUpdate_t* e2_nccu = &ccui->e2nodeComponentConfigUpdate;
  switch (src->update_present) {
    case E2_NODE_COMPONENT_CONFIG_UPDATE_GNB_CONFIG_UPDATE:
      {
      e2_nccu->present = E2AP_E2nodeComponentConfigUpdate_PR_gNBconfigUpdate;
      e2_nccu->choice.gNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateGNB_t));
      E2AP_E2nodeComponentConfigUpdateGNB_t* gnb = e2_nccu->choice.gNBconfigUpdate;
      if (src->gnb.ngap_gnb_cu_cp) {
        gnb->ngAPconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
        *gnb->ngAPconfigUpdate = copy_ba_to_ostring(*src->gnb.ngap_gnb_cu_cp);
        }
        if (src->gnb.xnap_gnb_cu_cp){
          gnb->xnAPconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
          *gnb->xnAPconfigUpdate = copy_ba_to_ostring(*src->gnb.xnap_gnb_cu_cp);
        }
        if (src->gnb.e1ap_gnb_cu_cp){
          gnb->e1APconfigUpdate = calloc(1,sizeof(OCTET_STRING_t));
          *gnb->e1APconfigUpdate = copy_ba_to_ostring(*src->gnb.e1ap_gnb_cu_cp);
        }
        if (src->gnb.f1ap_gnb_cu_cp){
          gnb->f1APconfigUpdate = calloc(1, sizeof(OCTET_STRING_t)); 
          *gnb->f1APconfigUpdate = copy_ba_to_ostring(*src->gnb.f1ap_gnb_cu_cp);
        }
        break;
      }
    case E2_NODE_COMPONENT_CONFIG_UPDATE_EN_GNB_CONFIG_UPDATE:
      {
      e2_nccu->present = E2AP_E2nodeComponentConfigUpdate_PR_en_gNBconfigUpdate;
      e2_nccu->choice.en_gNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateENgNB_t));
      E2AP_E2nodeComponentConfigUpdateENgNB_t* en_gnb = e2_nccu->choice.en_gNBconfigUpdate;
      if (src->en_gnb.x2ap_en_gnb) {
        en_gnb->x2APconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
        *en_gnb->x2APconfigUpdate = copy_ba_to_ostring(*src->en_gnb.x2ap_en_gnb);
        }
        break;
      }
    case E2_NODE_COMPONENT_CONFIG_UPDATE_NG_ENB_CONFIG_UPDATE:
      {
      e2_nccu->present = E2AP_E2nodeComponentConfigUpdate_PR_ng_eNBconfigUpdate;
      e2_nccu->choice.ng_eNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateNGeNB_t));
      E2AP_E2nodeComponentConfigUpdateNGeNB_t* ng_enb = e2_nccu->choice.ng_eNBconfigUpdate;
      if (src->ng_enb.ngap_ng_enb) {
        ng_enb->ngAPconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
        *ng_enb->ngAPconfigUpdate = copy_ba_to_ostring(*src->ng_enb.ngap_ng_enb);
        }
        if (src->ng_enb.xnap_ng_enb){
          ng_enb->xnAPconfigUpdate =  calloc(1,sizeof(OCTET_STRING_t));
          *ng_enb->xnAPconfigUpdate = copy_ba_to_ostring(*src->ng_enb.xnap_ng_enb);
        }
        break;
      }
    case E2_NODE_COMPONENT_CONFIG_UPDATE_ENB_CONFIG_UPDATE:
      {
      e2_nccu->present = E2AP_E2nodeComponentConfigUpdate_PR_ng_eNBconfigUpdate;
      e2_nccu->choice.eNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateENB_t));
      E2AP_E2nodeComponentConfigUpdateENB_t* enb = e2_nccu->choice.eNBconfigUpdate;
      if (src->enb.s1ap_enb) {
        enb->s1APconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
        *enb->s1APconfigUpdate = copy_ba_to_ostring(*src->enb.s1ap_enb);
        }
        if (src->enb.x2ap_enb){
          enb->x2APconfigUpdate =  calloc(1,sizeof(OCTET_STRING_t));
          *enb->x2APconfigUpdate = copy_ba_to_ostring(*src->enb.x2ap_enb);
        }
        break;
      }
    case E2_NODE_COMPONENT_CONFIG_UPDATE_NOTHING:
    default:
      assert(0!=0 && "Ivalid code path");
  }

  /*
    ///////////////////////////////////
    E2AP_E2nodeComponentConfigUpdate_t dst;
    memset(&dst, 0, sizeof(E2AP_E2nodeComponentConfigUpdate_t));

    switch (src->update_present) {
      case E2_NODE_COMPONENT_CONFIG_UPDATE_GNB_CONFIG_UPDATE:
        {
          dst.present = E2AP_E2nodeComponentConfigUpdate_PR_gNBconfigUpdate;
          dst.choice.gNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateGNB_t));
          E2AP_E2nodeComponentConfigUpdateGNB_t* gnb = dst.choice.gNBconfigUpdate;
          if (src->gnb.ngap_gnb_cu_cp){
            gnb->ngAPconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
            *gnb->ngAPconfigUpdate = copy_ba_to_ostring(*src->gnb.ngap_gnb_cu_cp);
          }
          if (src->gnb.xnap_gnb_cu_cp){
            gnb->xnAPconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
            *gnb->xnAPconfigUpdate = copy_ba_to_ostring(*src->gnb.xnap_gnb_cu_cp);
          }
          if (src->gnb.e1ap_gnb_cu_cp){
            gnb->e1APconfigUpdate = calloc(1,sizeof(OCTET_STRING_t));
            *gnb->e1APconfigUpdate = copy_ba_to_ostring(*src->gnb.e1ap_gnb_cu_cp);
          }
          if (src->gnb.f1ap_gnb_cu_cp){
            gnb->f1APconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
            *gnb->f1APconfigUpdate = copy_ba_to_ostring(*src->gnb.f1ap_gnb_cu_cp);
          }
          break;
        }
      case E2_NODE_COMPONENT_CONFIG_UPDATE_EN_GNB_CONFIG_UPDATE:
        {
          dst.present = E2AP_E2nodeComponentConfigUpdate_PR_en_gNBconfigUpdate;
          dst.choice.en_gNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateENgNB_t));
          E2AP_E2nodeComponentConfigUpdateENgNB_t* en_gnb = dst.choice.en_gNBconfigUpdate;
          if (src->en_gnb.x2ap_en_gnb){
            en_gnb->x2APconfigUpdate = calloc(1, sizeof(OCTET_STRING_t));
            *en_gnb->x2APconfigUpdate = copy_ba_to_ostring(*src->en_gnb.x2ap_en_gnb);
          }
          break;
        }
      case E2_NODE_COMPONENT_CONFIG_UPDATE_NG_ENB_CONFIG_UPDATE:
        {
          dst.present = E2AP_E2nodeComponentConfigUpdate_PR_ng_eNBconfigUpdate;
          dst.choice.ng_eNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateNGeNB_t));
          E2AP_E2nodeComponentConfigUpdateNGeNB_t* ng_enb = dst.choice.ng_eNBconfigUpdate;
          if (src->ng_enb.ngap_ng_enb){
            ng_enb->ngAPconfigUpdate =  calloc(1, sizeof(OCTET_STRING_t));
            *ng_enb->ngAPconfigUpdate = copy_ba_to_ostring(*src->ng_enb.ngap_ng_enb);
          }
          if (src->ng_enb.xnap_ng_enb){
            ng_enb->xnAPconfigUpdate =  calloc(1,sizeof(OCTET_STRING_t));
            *ng_enb->xnAPconfigUpdate = copy_ba_to_ostring(*src->ng_enb.xnap_ng_enb);
          }
          break;
        }
      case E2_NODE_COMPONENT_CONFIG_UPDATE_ENB_CONFIG_UPDATE:
        {
          dst.present = E2AP_E2nodeComponentConfigUpdate_PR_ng_eNBconfigUpdate;
          dst.choice.eNBconfigUpdate = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateENB_t));
          E2AP_E2nodeComponentConfigUpdateENB_t* enb = dst.choice.eNBconfigUpdate;
          if (src->enb.s1ap_enb){
            enb->s1APconfigUpdate =  calloc(1,sizeof(OCTET_STRING_t));
            *enb->s1APconfigUpdate = copy_ba_to_ostring(*src->enb.s1ap_enb);
          }
          if (src->enb.x2ap_enb){
            enb->x2APconfigUpdate =  calloc(1,sizeof(OCTET_STRING_t));
            *enb->x2APconfigUpdate = copy_ba_to_ostring(*src->enb.x2ap_enb);
          }
          break;
        }
      case E2_NODE_COMPONENT_CONFIG_UPDATE_NOTHING:
      default:
        assert(0!=0 && "Ivalid code path");
    }
  */
  return comp_update_item_ie;
}

byte_array_t e2ap_enc_asn_pdu_ba(struct E2AP_E2AP_PDU* pdu)
{
  assert(pdu != NULL);
  byte_array_t ba = {.buf = malloc(16384), .len = 16384};
  const bool success = encode(&ba, pdu);
  assert(success);
  return ba;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Messages for Near-RT RIC Functional Procedures //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
byte_array_t e2ap_enc_subscription_request_asn(const ric_subscription_request_t* sr)
{
  assert(sr != NULL);

  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_request_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);

  printf("[E2AP] SUBSCRIPTION REQUEST generated\n");

  return ba;
}

byte_array_t e2ap_enc_subscription_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type ==  RIC_SUBSCRIPTION_REQUEST );
  return e2ap_enc_subscription_request_asn(&msg->u_msgs.ric_sub_req);
}

struct E2AP_E2AP_PDU* e2ap_enc_subscription_request_asn_pdu(const ric_subscription_request_t* sr)
{
  // action_def is optional, therefore it can be NULL
  assert(sr->event_trigger.buf != NULL && sr->event_trigger.len > 0);
  assert(sr->len_action <= (size_t)MAX_NUM_ACTION_DEF);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICsubscription;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICsubscriptionRequest;

  E2AP_RICsubscriptionRequest_t* out = &pdu->choice.initiatingMessage->value.choice.RICsubscriptionRequest;

  // RIC Request ID. Mandatory.
  E2AP_RICsubscriptionRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICsubscriptionRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICsubscriptionRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = sr->ric_id.ric_inst_id; 
  sub_req->value.choice.RICrequestID.ricRequestorID = sr->ric_id.ric_req_id; 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_RICsubscriptionRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICsubscriptionRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = sr->ric_id.ran_func_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);
  // RIC Subscription Details. Mandatory
  E2AP_RICsubscriptionRequest_IEs_t* sub_det = calloc(1, sizeof(E2AP_RICsubscriptionRequest_IEs_t));
  sub_det->id = E2AP_ProtocolIE_ID_id_RICsubscriptionDetails;
  sub_det->criticality = E2AP_Criticality_reject;
  sub_det->value.present = E2AP_RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;

  //RIC Event Trigger Definition. Mandatory
  sub_det->value.choice.RICsubscriptionDetails.ricEventTriggerDefinition = copy_ba_to_ostring(sr->event_trigger);

  //Sequence of Actions. Mandatory
  for(size_t i = 0; i < sr->len_action; ++i){
    E2AP_RICaction_ToBeSetup_ItemIEs_t* act_setup = calloc(1, sizeof(E2AP_RICaction_ToBeSetup_ItemIEs_t));
    act_setup->id = E2AP_ProtocolIE_ID_id_RICaction_ToBeSetup_Item;
    act_setup->criticality = E2AP_Criticality_reject;
    act_setup->value.present = E2AP_RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;
    E2AP_RICaction_ToBeSetup_Item_t* dst = &act_setup->value.choice.RICaction_ToBeSetup_Item;
    const ric_action_t* src = &sr->action[i]; 
    dst->ricActionID = src->id; 
    dst->ricActionType = src->type;

    // RIC Action Definition. Optional 
    if( src->definition != NULL){
      dst->ricActionDefinition = calloc(1, sizeof(E2AP_RICactionDefinition_t));
      *dst->ricActionDefinition = copy_ba_to_ostring(*src->definition); 
    }

    // RIC Subsequent Action. Optional 
    if(src->subseq_action != NULL){
      dst->ricSubsequentAction = calloc(1, sizeof(E2AP_RICsubsequentAction_t));
      dst->ricSubsequentAction->ricSubsequentActionType = src->subseq_action->type;
      if(src->subseq_action->time_to_wait_ms != NULL){
        assert(*src->subseq_action->time_to_wait_ms < 18);
        // Very strange type. Optional but is not a pointer....
        dst->ricSubsequentAction->ricTimeToWait = *src->subseq_action->time_to_wait_ms;
      }
    }

    rc = ASN_SEQUENCE_ADD(&sub_det->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List.list, act_setup);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_det);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_subscription_response_asn(const ric_subscription_response_t* sr)
{ 
  assert(sr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_response_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_subscription_response_asn_msg(const e2ap_msg_t* msg )
{ 
  assert(msg != NULL);
  assert(msg->type == RIC_SUBSCRIPTION_RESPONSE);
  return e2ap_enc_subscription_response_asn(&msg->u_msgs.ric_sub_resp);
}

struct E2AP_E2AP_PDU* e2ap_enc_subscription_response_asn_pdu(const ric_subscription_response_t* sr)
{
  assert(sr != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICsubscription;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_RICsubscriptionResponse;

  E2AP_RICsubscriptionResponse_t* out = &pdu->choice.successfulOutcome->value.choice.RICsubscriptionResponse;

  // RIC Request ID. Mandatory
  E2AP_RICsubscriptionResponse_IEs_t* req_id = calloc(1, sizeof(E2AP_RICsubscriptionResponse_IEs_t));
  req_id->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  req_id->criticality = E2AP_Criticality_reject;
  req_id->value.present = E2AP_RICsubscriptionResponse_IEs__value_PR_RICrequestID;
  req_id->value.choice.RICrequestID.ricRequestorID = sr->ric_id.ric_req_id;
  req_id->value.choice.RICrequestID.ricInstanceID = sr->ric_id.ric_inst_id;
 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, req_id);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_RICsubscriptionResponse_IEs_t* ran_func = calloc(1, sizeof(E2AP_RICsubscriptionResponse_IEs_t));
  ran_func->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_func->criticality = E2AP_Criticality_reject;
  ran_func->value.present = E2AP_RICsubscriptionResponse_IEs__value_PR_RANfunctionID;
  ran_func->value.choice.RANfunctionID = sr->ric_id.ran_func_id;
 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_func);
  assert(rc == 0);

  // RIC Action Admitted List
  E2AP_RICsubscriptionResponse_IEs_t* act_adm_list = calloc(1, sizeof(E2AP_RICsubscriptionResponse_IEs_t));
  act_adm_list->id = E2AP_ProtocolIE_ID_id_RICactions_Admitted;
  act_adm_list->criticality = E2AP_Criticality_reject;
  act_adm_list->value.present = E2AP_RICsubscriptionResponse_IEs__value_PR_RICaction_Admitted_List;

  for(size_t i = 0; i <  sr->len_admitted; ++i){
    // RIC Action ID. Mandatory
    E2AP_RICaction_Admitted_ItemIEs_t* ai = calloc(1, sizeof(E2AP_RICaction_Admitted_ItemIEs_t));
    ai->id = E2AP_ProtocolIE_ID_id_RICaction_Admitted_Item;
    ai->criticality = E2AP_Criticality_reject;
    ai->value.present = E2AP_RICaction_Admitted_ItemIEs__value_PR_RICaction_Admitted_Item;
    const ric_action_admitted_t* src = &sr->admitted[i];
    ai->value.choice.RICaction_Admitted_Item.ricActionID = src->ric_act_id; 
    rc = ASN_SEQUENCE_ADD(&act_adm_list->value.choice.RICaction_Admitted_List.list,ai);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list,act_adm_list);
  assert(rc == 0);

  // RIC Actions Not Admitted Lists
  E2AP_RICsubscriptionResponse_IEs_t* act_not_adm_list = calloc(1, sizeof(E2AP_RICsubscriptionResponse_IEs_t));
  act_not_adm_list->id = E2AP_ProtocolIE_ID_id_RICactions_NotAdmitted;
  act_not_adm_list->criticality = E2AP_Criticality_reject;
  act_not_adm_list->value.present = E2AP_RICsubscriptionResponse_IEs__value_PR_RICaction_NotAdmitted_List;

  for(size_t i = 0; i < sr->len_na; ++i) {
    // RIC Action ID. Mandatory
    E2AP_RICaction_NotAdmitted_ItemIEs_t* nai = calloc(1, sizeof(E2AP_RICaction_NotAdmitted_ItemIEs_t));
    nai->id = E2AP_ProtocolIE_ID_id_RICaction_NotAdmitted_Item;
    nai->criticality = E2AP_Criticality_reject;
    nai->value.present = E2AP_RICaction_NotAdmitted_ItemIEs__value_PR_RICaction_NotAdmitted_Item;
    const ric_action_not_admitted_t* src = &sr->not_admitted[i];
    E2AP_RICaction_NotAdmitted_Item_t* dst = &nai->value.choice.RICaction_NotAdmitted_Item;
    dst->cause = copy_cause(src->cause);
    dst->ricActionID = src->ric_act_id;
    rc = ASN_SEQUENCE_ADD(&act_not_adm_list->value.choice.RICaction_NotAdmitted_List.list,nai);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, act_not_adm_list);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_subscription_failure_asn(const ric_subscription_failure_t* sf)
{
  assert(sf != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_failure_asn_pdu(sf);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_subscription_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SUBSCRIPTION_FAILURE);
  return e2ap_enc_subscription_failure_asn(&msg->u_msgs.ric_sub_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_subscription_failure_asn_pdu(const ric_subscription_failure_t* sf)
{
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICsubscription;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_RICsubscriptionFailure;

  E2AP_RICsubscriptionFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.RICsubscriptionFailure;

  // RIC Request ID. Mandatory
  E2AP_RICsubscriptionFailure_IEs_t* req_id = calloc(1, sizeof(E2AP_RICsubscriptionFailure_IEs_t));
  req_id->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  req_id->criticality = E2AP_Criticality_reject;
  req_id->value.present = E2AP_RICsubscriptionFailure_IEs__value_PR_RICrequestID;
  req_id->value.choice.RICrequestID.ricRequestorID = sf->ric_id.ric_req_id;
  req_id->value.choice.RICrequestID.ricInstanceID = sf->ric_id.ric_inst_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, req_id);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_RICsubscriptionFailure_IEs_t* ran_func = calloc(1, sizeof(E2AP_RICsubscriptionFailure_IEs_t));
  ran_func->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_func->criticality = E2AP_Criticality_reject;
  ran_func->value.present = E2AP_RICsubscriptionFailure_IEs__value_PR_RANfunctionID;
  ran_func->value.choice.RANfunctionID = sf->ric_id.ran_func_id; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_func);
  assert(rc == 0);

  // RIC Actions Not Admitted List
  E2AP_RICsubscriptionFailure_IEs_t* act_not_adm_list = calloc(1, sizeof(E2AP_RICsubscriptionFailure_IEs_t));
  act_not_adm_list->id = E2AP_ProtocolIE_ID_id_RICactions_NotAdmitted;
  act_not_adm_list->criticality = E2AP_Criticality_reject;
  act_not_adm_list->value.present = E2AP_RICsubscriptionFailure_IEs__value_PR_RICaction_NotAdmitted_List;

  for(size_t i = 0; i < sf->len_na; ++i) {
    // RIC Action ID. Mandatory
    E2AP_RICaction_NotAdmitted_ItemIEs_t* nai = calloc(1, sizeof(E2AP_RICaction_NotAdmitted_ItemIEs_t));
    nai->id = E2AP_ProtocolIE_ID_id_RICaction_NotAdmitted_Item;
    nai->criticality = E2AP_Criticality_reject;
    nai->value.present = E2AP_RICaction_NotAdmitted_ItemIEs__value_PR_RICaction_NotAdmitted_Item;
    E2AP_RICaction_NotAdmitted_Item_t* dst = &nai->value.choice.RICaction_NotAdmitted_Item;
    const ric_action_not_admitted_t* src = &sf->not_admitted[i];
    dst->ricActionID = src->ric_act_id;
    dst->cause = copy_cause(src->cause);
    rc = ASN_SEQUENCE_ADD(&act_not_adm_list->value.choice.RICaction_NotAdmitted_List.list,nai);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, act_not_adm_list);
  assert(rc == 0);

 // Criticality Diagnosis. Optional
 if(sf->crit_diag  != NULL){
   E2AP_RICsubscriptionFailure_IEs_t* crit_diag = calloc(1, sizeof(E2AP_RICsubscriptionFailure_IEs_t));
   crit_diag->id = E2AP_ProtocolIE_ID_id_CriticalityDiagnostics;
   crit_diag->criticality = E2AP_Criticality_reject;
   crit_diag->value.present = E2AP_RICsubscriptionFailure_IEs__value_PR_CriticalityDiagnostics;
   assert(0 != 0 && "Not Implemented yet");
   rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, crit_diag);
   assert(rc == 0);
 }
  return pdu;
}

byte_array_t e2ap_enc_subscription_delete_request_asn(const ric_subscription_delete_request_t* dr)
{
  assert(dr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_delete_request_asn_pdu(dr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_subscription_delete_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == RIC_SUBSCRIPTION_DELETE_REQUEST);
  return e2ap_enc_subscription_delete_request_asn(&msg->u_msgs.ric_sub_del_req);
}

E2AP_E2AP_PDU_t* e2ap_enc_subscription_delete_request_asn_pdu(const ric_subscription_delete_request_t* dr)
{
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICsubscriptionDelete;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICsubscriptionDeleteRequest;

  E2AP_RICsubscriptionDeleteRequest_t* out = &pdu->choice.initiatingMessage->value.choice.RICsubscriptionDeleteRequest;

  // RIC Request ID. Mandatory
  E2AP_RICsubscriptionDeleteRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICsubscriptionDeleteRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICsubscriptionDeleteRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = dr->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = dr->ric_id.ric_req_id;
 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RIC Function ID. Mandatory
  E2AP_RICsubscriptionDeleteRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICsubscriptionDeleteRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICsubscriptionDeleteRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = dr->ric_id.ran_func_id;
 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_subscription_delete_response_asn(const ric_subscription_delete_response_t * dr)
{
  assert(dr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_delete_response_asn_pdu(dr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_subscription_delete_response_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SUBSCRIPTION_DELETE_RESPONSE);
  return e2ap_enc_subscription_delete_response_asn(&msg->u_msgs.ric_sub_del_resp);
}

E2AP_E2AP_PDU_t* e2ap_enc_subscription_delete_response_asn_pdu(const ric_subscription_delete_response_t* dr)
{
  assert(dr != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICsubscriptionDelete;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_RICsubscriptionDeleteResponse;

  E2AP_RICsubscriptionDeleteResponse_t* out = &pdu->choice.successfulOutcome->value.choice.RICsubscriptionDeleteResponse;

  // RIC request ID. Mandatory
  E2AP_RICsubscriptionDeleteResponse_IEs_t* req_id = calloc(1, sizeof(E2AP_RICsubscriptionDeleteResponse_IEs_t));
  req_id->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  req_id->criticality = E2AP_Criticality_reject;
  req_id->value.present = E2AP_RICsubscriptionDeleteResponse_IEs__value_PR_RICrequestID;
  req_id->value.choice.RICrequestID.ricRequestorID = dr->ric_id.ric_req_id;
  req_id->value.choice.RICrequestID.ricInstanceID = dr->ric_id.ric_inst_id; 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list,req_id);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_RICsubscriptionDeleteResponse_IEs_t* ran_func = calloc(1, sizeof(E2AP_RICsubscriptionDeleteResponse_IEs_t));
  ran_func->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_func->criticality = E2AP_Criticality_reject;
  ran_func->value.present = E2AP_RICsubscriptionDeleteResponse_IEs__value_PR_RANfunctionID;
  ran_func->value.choice.RANfunctionID = dr->ric_id.ran_func_id; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_func);
  assert(rc == 0);

  return pdu;
}

byte_array_t e2ap_enc_subscription_delete_failure_asn(const ric_subscription_delete_failure_t* df)
{
  assert(df != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_subscription_delete_failure_asn_pdu(df);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_subscription_delete_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SUBSCRIPTION_DELETE_FAILURE);
  return e2ap_enc_subscription_delete_failure_asn(&msg->u_msgs.ric_sub_del_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_subscription_delete_failure_asn_pdu(const ric_subscription_delete_failure_t* df)
{
  assert(df != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICsubscriptionDelete;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_RICsubscriptionDeleteFailure;

  E2AP_RICsubscriptionDeleteFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.RICsubscriptionDeleteFailure;

  // RIC Request ID. Mandatory
  E2AP_RICsubscriptionDeleteFailure_IEs_t* req_id = calloc(1, sizeof(E2AP_RICsubscriptionDeleteFailure_IEs_t));
  req_id->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  req_id->criticality = E2AP_Criticality_reject;
  req_id->value.present = E2AP_RICsubscriptionDeleteFailure_IEs__value_PR_RICrequestID;
  req_id->value.choice.RICrequestID.ricRequestorID = df->ric_id.ric_req_id; 
  req_id->value.choice.RICrequestID.ricInstanceID = df->ric_id.ric_inst_id; 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, req_id);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_RICsubscriptionDeleteFailure_IEs_t* ran_func = calloc(1, sizeof(E2AP_RICsubscriptionDeleteFailure_IEs_t));
  ran_func->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_func->criticality = E2AP_Criticality_reject;
  ran_func->value.present = E2AP_RICsubscriptionDeleteFailure_IEs__value_PR_RANfunctionID;
  ran_func->value.choice.RANfunctionID = df->ric_id.ran_func_id; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_func);
  assert(rc == 0);

  // Cause. Mandatory
  E2AP_RICsubscriptionDeleteFailure_IEs_t* act_not_adm = calloc(1, sizeof(E2AP_RICsubscriptionDeleteFailure_IEs_t));
  act_not_adm->id = E2AP_ProtocolIE_ID_id_RICactions_NotAdmitted;
  act_not_adm->criticality = E2AP_Criticality_reject;
  act_not_adm->value.present = E2AP_RICsubscriptionDeleteFailure_IEs__value_PR_Cause;
  act_not_adm->value.choice.Cause = copy_cause(df->cause);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list,act_not_adm);
  assert(rc == 0);

 // Criticality Diagnosis. Optional
 if(df->crit_diag  != NULL){
   E2AP_RICsubscriptionDeleteFailure_IEs_t* crit_diag = calloc(1, sizeof(E2AP_RICsubscriptionDeleteFailure_IEs_t));
   crit_diag->id = E2AP_ProtocolIE_ID_id_CriticalityDiagnostics;
   crit_diag->criticality = E2AP_Criticality_reject;
   crit_diag->value.present = E2AP_RICsubscriptionDeleteFailure_IEs__value_PR_CriticalityDiagnostics;
   assert(0 != 0 && "Not implemented");
   //  crit_diag->value.choice.CriticalityDiagnostics = *df->crit_diagnose;
   rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, crit_diag);
   assert(rc == 0);
 }
  return pdu;
}

byte_array_t e2ap_enc_indication_asn(const ric_indication_t* ind) 
{
  assert(ind != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_indication_asn_pdu(ind);
  byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_indication_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == RIC_INDICATION);
  return e2ap_enc_indication_asn(&msg->u_msgs.ric_ind);
}

E2AP_E2AP_PDU_t* e2ap_enc_indication_asn_pdu(const ric_indication_t* ind)
{
  e_E2AP_RICindicationType ric_ind_type = ind->type == RIC_IND_REPORT ? E2AP_RICindicationType_report : E2AP_RICindicationType_insert;
  assert(ric_ind_type == E2AP_RICindicationType_report || ric_ind_type == E2AP_RICindicationType_insert);
  assert(ind->hdr.buf != NULL && ind->hdr.len > 0);
  assert(ind->msg.buf != NULL && ind->msg.len > 0);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICindication;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICindication;

  E2AP_RICindication_t* out = &pdu->choice.initiatingMessage->value.choice.RICindication;

  // RIC Request ID. Mandatory
  E2AP_RICindication_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICindication_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = ind->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = ind->ric_id.ric_req_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  //RAN Function ID. Mandatory
  E2AP_RICindication_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICindication_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = ind->ric_id.ran_func_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);


  // RIC Action ID. Mandatory
  E2AP_RICindication_IEs_t* ric_act = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ric_act->id = E2AP_ProtocolIE_ID_id_RICactionID;
  ric_act->criticality = E2AP_Criticality_reject;
  ric_act->value.present = E2AP_RICindication_IEs__value_PR_RICactionID;
  ric_act->value.choice.RICactionID = ind->action_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_act);
  assert(rc == 0);

  if(ind->sn != NULL){
  //RIC indication SN. Optional
  E2AP_RICindication_IEs_t* ric_ind = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ric_ind->id = E2AP_ProtocolIE_ID_id_RICindicationSN;
  ric_ind->criticality = E2AP_Criticality_reject;
  ric_ind->value.present = E2AP_RICindication_IEs__value_PR_RICindicationSN;
  ric_ind->value.choice.RICindicationSN = *ind->sn;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_ind);
  assert(rc == 0);
  }

  // RIC indication Type. Mandatory
  E2AP_RICindication_IEs_t* ric_type = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ric_type->id = E2AP_ProtocolIE_ID_id_RICindicationType;
  ric_type->criticality = E2AP_Criticality_reject;
  ric_type->value.present = E2AP_RICindication_IEs__value_PR_RICindicationType;
  ric_type->value.choice.RICindicationType = ind->type;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_type);
  assert(rc == 0);

  //RIC indication header. Mandatory
  E2AP_RICindication_IEs_t* ric_header = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ric_header->id = E2AP_ProtocolIE_ID_id_RICindicationHeader;
  ric_header->criticality = E2AP_Criticality_reject;
  ric_header->value.present = E2AP_RICindication_IEs__value_PR_RICindicationHeader;
  ric_header->value.choice.RICindicationHeader = copy_ba_to_ostring(ind->hdr);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_header);
  assert(rc == 0);

  // RIC indication message. Mandatory
  E2AP_RICindication_IEs_t* ric_msg = calloc(1, sizeof(E2AP_RICindication_IEs_t));
  ric_msg->id = E2AP_ProtocolIE_ID_id_RICindicationMessage;
  ric_msg->criticality = E2AP_Criticality_reject;
  ric_msg->value.present = E2AP_RICindication_IEs__value_PR_RICindicationMessage;
  ric_msg->value.choice.RICindicationMessage = copy_ba_to_ostring(ind->msg);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_msg);
  assert(rc == 0);

  // RIC call process id. Optional
  if (ind->call_process_id != NULL) {
    assert(ind->call_process_id->buf != NULL && ind->call_process_id->len > 0);
    E2AP_RICindication_IEs_t* ric_proc = calloc(1, sizeof(E2AP_RICindication_IEs_t));
    ric_proc->id = E2AP_ProtocolIE_ID_id_RICcallProcessID;
    ric_proc->criticality = E2AP_Criticality_reject;
    ric_proc->value.present = E2AP_RICindication_IEs__value_PR_RICcallProcessID;
    ric_proc->value.choice.RICcallProcessID = copy_ba_to_ostring(*ind->call_process_id);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_proc);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_control_request_asn(const ric_control_request_t* ric_req)
{
  assert(ric_req != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_control_request_asn_pdu(ric_req);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu); 
  return ba;
}

byte_array_t e2ap_enc_control_request_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_CONTROL_REQUEST);
  return  e2ap_enc_control_request_asn(&msg->u_msgs.ric_ctrl_req);
}

E2AP_E2AP_PDU_t* e2ap_enc_control_request_asn_pdu(const ric_control_request_t* ric_req)
{
  assert(ric_req != NULL);
  assert(ric_req->hdr.buf != NULL && ric_req->hdr.len > 0);
  assert(ric_req->msg.buf != NULL && ric_req->msg.len > 0);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICcontrol;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICcontrolRequest;

  E2AP_RICcontrolRequest_t* out = &pdu->choice.initiatingMessage->value.choice.RICcontrolRequest;

  // RIC request id. Mandatory
  E2AP_RICcontrolRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = ric_req->ric_id.ric_inst_id; 
  sub_req->value.choice.RICrequestID.ricRequestorID = ric_req->ric_id.ric_req_id; 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RIC RAN function ID. Mandatory
  E2AP_RICcontrolRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = ric_req->ric_id.ran_func_id; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);

  // RIC Call Process ID. Optional
  if (ric_req->call_process_id != NULL) {
    assert(ric_req->call_process_id->buf != NULL && ric_req->call_process_id->len > 0);
    E2AP_RICcontrolRequest_IEs_t* ric_proc = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
    ric_proc->id = E2AP_ProtocolIE_ID_id_RICcallProcessID;
    ric_proc->criticality = E2AP_Criticality_reject;
    ric_proc->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RICcallProcessID;
    ric_proc->value.choice.RICcallProcessID = copy_ba_to_ostring(*ric_req->call_process_id);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_proc);
    assert(rc == 0);
  }

  // RIC Control Header. Mandatory
  E2AP_RICcontrolRequest_IEs_t* ric_header = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
  ric_header->id = E2AP_ProtocolIE_ID_id_RICcontrolHeader;
  ric_header->criticality = E2AP_Criticality_reject;
  ric_header->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RICcontrolHeader;
  ric_header->value.choice.RICcontrolHeader = copy_ba_to_ostring(ric_req->hdr); 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_header);
  assert(rc == 0);

  // RIC Control Message. Mandatory
  E2AP_RICcontrolRequest_IEs_t* ric_msg = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
  ric_msg->id = E2AP_ProtocolIE_ID_id_RICcontrolMessage;
  ric_msg->criticality = E2AP_Criticality_reject;
  ric_msg->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RICcontrolMessage;
  ric_msg->value.choice.RICcontrolMessage = copy_ba_to_ostring(ric_req->msg);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_msg);
  assert(rc == 0);

  // RIC Control ACK Request. Optional
  if(ric_req->ack_req != NULL){
    E2AP_RICcontrolRequest_IEs_t* ric_ack = calloc(1, sizeof(E2AP_RICcontrolRequest_IEs_t));
    ric_ack->id = E2AP_ProtocolIE_ID_id_RICcontrolAckRequest;
    ric_ack->criticality = E2AP_Criticality_reject;
    ric_ack->value.present = E2AP_RICcontrolRequest_IEs__value_PR_RICcontrolAckRequest;
    ric_ack->value.choice.RICcontrolAckRequest = *ric_req->ack_req; 
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_ack);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_control_ack_asn(const ric_control_acknowledge_t* ca)
{
  assert( ca != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_control_ack_asn_pdu(ca);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu); 
  return ba;
}

byte_array_t e2ap_enc_control_ack_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == RIC_CONTROL_ACKNOWLEDGE);
  return e2ap_enc_control_ack_asn(&msg->u_msgs.ric_ctrl_ack);
}

E2AP_E2AP_PDU_t* e2ap_enc_control_ack_asn_pdu(const ric_control_acknowledge_t* ca)
{
  // Message Type. Mandatory.
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICcontrol;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_RICcontrolAcknowledge;

  E2AP_RICcontrolAcknowledge_t* out = &pdu->choice.successfulOutcome->value.choice.RICcontrolAcknowledge;

  //RIC Request ID. Mandatory
  E2AP_RICcontrolAcknowledge_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICcontrolAcknowledge_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICcontrolAcknowledge_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = ca->ric_id.ric_inst_id; 
  sub_req->value.choice.RICrequestID.ricRequestorID = ca->ric_id.ric_req_id; 
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  //RAN Function ID. Mandatory
  E2AP_RICcontrolAcknowledge_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICcontrolAcknowledge_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICcontrolAcknowledge_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = ca->ric_id.ran_func_id; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);

  //RIC Call process ID. Optional
  if (ca->call_process_id != NULL ) {
    assert(ca->call_process_id->buf  != NULL && ca->call_process_id->len > 0);
    E2AP_RICcontrolAcknowledge_IEs_t* ric_proc = calloc(1, sizeof(E2AP_RICcontrolAcknowledge_IEs_t));
    ric_proc->id = E2AP_ProtocolIE_ID_id_RICcallProcessID;
    ric_proc->criticality = E2AP_Criticality_reject;
    ric_proc->value.present = E2AP_RICcontrolAcknowledge_IEs__value_PR_RICcallProcessID;
    ric_proc->value.choice.RICcallProcessID = copy_ba_to_ostring(*ca->call_process_id);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_proc);
    assert(rc == 0);
  }

  //RIC Control Status. Mandatory
  e_E2AP_RICcontrolStatus ctrl_status = {0};
  if(ca->status ==  RIC_CONTROL_STATUS_SUCCESS ){
    ctrl_status = E2AP_RICcontrolStatus_success;
  } else if(ca->status ==RIC_CONTROL_STATUS_REJECTED){
    ctrl_status = E2AP_RICcontrolStatus_rejected;
  } else if(ca->status == RIC_CONTROL_STATUS_FAILED ){
    ctrl_status = E2AP_RICcontrolStatus_failed;
  } else {
    assert(0!=0 && "Unknown type");
  }

  assert(ctrl_status == E2AP_RICcontrolStatus_success || ctrl_status == E2AP_RICcontrolStatus_rejected || ctrl_status == E2AP_RICcontrolStatus_failed);
  E2AP_RICcontrolAcknowledge_IEs_t* ric_status = calloc(1, sizeof(E2AP_RICcontrolAcknowledge_IEs_t));
  ric_status->id = E2AP_ProtocolIE_ID_id_RICcontrolStatus;
  ric_status->criticality = E2AP_Criticality_reject;
  ric_status->value.present = E2AP_RICcontrolAcknowledge_IEs__value_PR_RICcontrolStatus;
  ric_status->value.choice.RICcontrolStatus = ctrl_status; 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_status);
  assert(rc == 0);

  //RIC Control Outcome. Optional
  if (ca->control_outcome != NULL) {
    assert(ca->control_outcome->buf != NULL && ca->control_outcome->len > 0);
    E2AP_RICcontrolAcknowledge_IEs_t* ric_out = calloc(1, sizeof(E2AP_RICcontrolAcknowledge_IEs_t));
    ric_out->id = E2AP_ProtocolIE_ID_id_RICcontrolOutcome;
    ric_out->criticality = E2AP_Criticality_reject;
    ric_out->value.present = E2AP_RICcontrolAcknowledge_IEs__value_PR_RICcontrolOutcome;
    ric_out->value.choice.RICcontrolOutcome = copy_ba_to_ostring(*ca->control_outcome);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_out);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_control_failure_asn(const ric_control_failure_t* cf)
{
  assert( cf != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_control_failure_asn_pdu(cf);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu); 
  return ba;
}

byte_array_t e2ap_enc_control_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_CONTROL_FAILURE);
  return e2ap_enc_control_failure_asn(&msg->u_msgs.ric_ctrl_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_control_failure_asn_pdu(const ric_control_failure_t* cf)
{
  assert(cf != NULL);
  //Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICcontrol;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_RICcontrolFailure;

  E2AP_RICcontrolFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.RICcontrolFailure;

  //RIC Request ID. Mandatory
  E2AP_RICcontrolFailure_IEs_t* sub_req = calloc(1, sizeof(E2AP_RICcontrolFailure_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_RICcontrolFailure_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = cf->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = cf->ric_id.ric_req_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  //RAN Function ID. Mandatory
  E2AP_RICcontrolFailure_IEs_t* ran_id = calloc(1, sizeof(E2AP_RICcontrolFailure_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_RICcontrolFailure_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = cf->ric_id.ran_func_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);

  //RIC Call process ID. Optional
  if (cf->call_process_id != NULL) {
    assert(cf->call_process_id->buf != NULL && cf->call_process_id->len > 0);
    E2AP_RICcontrolFailure_IEs_t* ric_proc = calloc(1, sizeof(E2AP_RICcontrolFailure_IEs_t));
    ric_proc->id = E2AP_ProtocolIE_ID_id_RICcallProcessID;
    ric_proc->criticality = E2AP_Criticality_reject;
    ric_proc->value.present = E2AP_RICcontrolFailure_IEs__value_PR_RICcallProcessID;

    ric_proc->value.choice.RICcallProcessID = copy_ba_to_ostring(*cf->call_process_id);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_proc);
    assert(rc == 0);
  }

  //Cause. Mandatory
  E2AP_RICcontrolFailure_IEs_t* ric_cause = calloc(1, sizeof(E2AP_RICcontrolFailure_IEs_t));
  ric_cause->id = E2AP_ProtocolIE_ID_id_Cause;
  ric_cause->criticality = E2AP_Criticality_reject;
  ric_cause->value.present = E2AP_RICcontrolFailure_IEs__value_PR_Cause;
  ric_cause->value.choice.Cause = copy_cause(cf->cause); 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_cause);
  assert(rc == 0);

  //RIC Control Outcome. Optional
  if (cf->control_outcome != NULL) {
    assert(cf->control_outcome->buf != NULL && cf->control_outcome->len > 0);
    E2AP_RICcontrolFailure_IEs_t* ric_out = calloc(1, sizeof(E2AP_RICcontrolFailure_IEs_t));
    ric_out->id = E2AP_ProtocolIE_ID_id_RICcontrolOutcome;
    ric_out->criticality = E2AP_Criticality_reject;
    ric_out->value.present = E2AP_RICcontrolFailure_IEs__value_PR_RICcontrolOutcome;
    ric_out->value.choice.RICcontrolOutcome = copy_ba_to_ostring(*cf->control_outcome);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_out);
    assert(rc == 0);
  }
  return pdu;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Messages for global Procedures //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

byte_array_t e2ap_enc_error_indication_asn(const e2ap_error_indication_t* ei)
{
  assert(ei != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_error_indication_asn_pdu(ei);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu); 
  return ba;
}

byte_array_t e2ap_enc_error_indication_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2AP_ERROR_INDICATION);
  return e2ap_enc_error_indication_asn(&msg->u_msgs.err_ind);
}

E2AP_E2AP_PDU_t* e2ap_enc_error_indication_asn_pdu(const e2ap_error_indication_t* ei)
{
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_ErrorIndication;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_ErrorIndication;

  E2AP_ErrorIndication_t* out = &pdu->choice.initiatingMessage->value.choice.ErrorIndication;
  //FixME. Here you either have non or both due to ric_id... 
  // RIC Request ID. Optional
  if(ei->ric_id != NULL ){

    //RIC Request ID. Mandatory
    E2AP_ErrorIndication_IEs_t* sub_req = calloc(1, sizeof(E2AP_ErrorIndication_IEs_t));
    sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
    sub_req->criticality = E2AP_Criticality_reject;
    sub_req->value.present = E2AP_ErrorIndication_IEs__value_PR_RICrequestID;
    sub_req->value.choice.RICrequestID.ricInstanceID = ei->ric_id->ric_inst_id; 
    sub_req->value.choice.RICrequestID.ricRequestorID = ei->ric_id->ric_req_id;
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
    assert(rc == 0);

    // RAN Function ID. Optional
    E2AP_ErrorIndication_IEs_t* ran_id = calloc(1, sizeof(E2AP_ErrorIndication_IEs_t));
    ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
    ran_id->criticality = E2AP_Criticality_reject;
    ran_id->value.present = E2AP_ErrorIndication_IEs__value_PR_RANfunctionID;
    ran_id->value.choice.RANfunctionID = ei->ric_id->ran_func_id; 
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
    assert(rc == 0);
  }
  // Cause. Optional
  if(ei->cause!= NULL){
    E2AP_ErrorIndication_IEs_t* ric_cause = calloc(1, sizeof(E2AP_ErrorIndication_IEs_t));
    ric_cause->id = E2AP_ProtocolIE_ID_id_Cause;
    ric_cause->criticality = E2AP_Criticality_reject;
    ric_cause->value.present = E2AP_ErrorIndication_IEs__value_PR_Cause;
    ric_cause->value.choice.Cause = copy_cause(*ei->cause); 
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_cause);
    assert(rc == 0);
  }
  // Criticality Diagnosis. Optional
  if(ei->crit_diag != NULL){
    E2AP_ErrorIndication_IEs_t* crit_diag = calloc(1, sizeof(E2AP_ErrorIndication_IEs_t));
    crit_diag->id = E2AP_ProtocolIE_ID_id_CriticalityDiagnostics;
    crit_diag->criticality = E2AP_Criticality_reject;
    crit_diag->value.present = E2AP_ErrorIndication_IEs__value_PR_CriticalityDiagnostics;
    assert(0!=0 && "Not implemented");
    //  crit_diag->value.choice.CriticalityDiagnostics = *crit_diagnose; 
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, crit_diag);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_setup_request_asn(const e2_setup_request_t* sr)
{
  assert(sr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_setup_request_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_setup_request_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type ==  E2_SETUP_REQUEST );
  return e2ap_enc_setup_request_asn(&msg->u_msgs.e2_stp_req);
}

E2AP_E2AP_PDU_t* e2ap_enc_setup_request_asn_pdu(const e2_setup_request_t* sr)
{
  assert(sr->id.type == ngran_gNB || sr->id.type == ngran_gNB_CU || sr->id.type == ngran_gNB_DU || sr->id.type == ngran_eNB);
  assert(sr->len_rf <= (size_t)MAX_NUM_RAN_FUNC_ID);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E2setup;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E2setupRequest;

  E2AP_E2setupRequest_t* out = &pdu->choice.initiatingMessage->value.choice.E2setupRequest;

  // Global E2 Node ID. Mandatory
  E2AP_E2setupRequestIEs_t* setup_rid = calloc(1, sizeof(E2AP_E2setupRequestIEs_t));
  setup_rid->id = E2AP_ProtocolIE_ID_id_GlobalE2node_ID;
  setup_rid->criticality = E2AP_Criticality_reject;
  setup_rid->value.present = E2AP_E2setupRequestIEs__value_PR_GlobalE2node_ID;

  int rc = 0;
  if (sr->id.type != ngran_eNB) {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_gNB;

    E2AP_GlobalE2node_gNB_ID_t* e2gnb = calloc(1, sizeof(E2AP_GlobalE2node_gNB_ID_t));
    assert(e2gnb != NULL && "Memory exhasued");
    e2gnb->global_gNB_ID.gnb_id.present = E2AP_GNB_ID_Choice_PR_gnb_ID;
    if (sr->id.type == ngran_gNB_CU) {
      E2AP_GNB_CU_UP_ID_t* e2gnb_cu_up_id = calloc(1, sizeof(E2AP_GNB_CU_UP_ID_t));
      assert(e2gnb_cu_up_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_cu_up_id, *sr->id.cu_du_id);
      e2gnb->gNB_CU_UP_ID = e2gnb_cu_up_id;
    }
    else if (sr->id.type == ngran_gNB_DU) {
      E2AP_GNB_DU_ID_t* e2gnb_du_id = calloc(1, sizeof(E2AP_GNB_DU_ID_t));
      assert(e2gnb_du_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_du_id, *sr->id.cu_du_id);
      e2gnb->gNB_DU_ID = e2gnb_du_id;
    }

    const plmn_t* plmn = &sr->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2gnb->global_gNB_ID.plmn_id);
    MACRO_GNB_ID_TO_BIT_STRING(sr->id.nb_id, &e2gnb->global_gNB_ID.gnb_id.choice.gnb_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.gNB = e2gnb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  } else {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_eNB;
    E2AP_GlobalE2node_eNB_ID_t* e2enb = calloc(1, sizeof(E2AP_GlobalE2node_eNB_ID_t));
    assert(e2enb != NULL && "Memory exhasued");
    e2enb->global_eNB_ID.eNB_ID.present = E2AP_ENB_ID_PR_macro_eNB_ID;
    const plmn_t* plmn = &sr->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2enb->global_eNB_ID.pLMN_Identity);
    MACRO_ENB_ID_TO_BIT_STRING(sr->id.nb_id, &e2enb->global_eNB_ID.eNB_ID.choice.macro_eNB_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.eNB = e2enb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  }

  if(sr->len_rf > 0){
  // List of RAN Functions Added
  E2AP_E2setupRequestIEs_t* ran_list = calloc(1, sizeof(E2AP_E2setupRequestIEs_t));
  ran_list->id = E2AP_ProtocolIE_ID_id_RANfunctionsAdded;
  ran_list->criticality = E2AP_Criticality_reject;
  ran_list->value.present = E2AP_E2setupRequestIEs__value_PR_RANfunctions_List;

  for (size_t i = 0; i < sr->len_rf; ++i) {
    E2AP_RANfunction_ItemIEs_t* ran_function_item_ie = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
    ran_function_item_ie->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
    ran_function_item_ie->criticality = E2AP_Criticality_reject;
    ran_function_item_ie->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
    ran_function_item_ie->value.choice.RANfunction_Item = copy_ran_function(&sr->ran_func_item[i]);
    rc = ASN_SEQUENCE_ADD(&ran_list->value.choice.RANfunctions_List.list, ran_function_item_ie);
    assert(rc == 0);
    }
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_list);
    assert(rc == 0);
  }

  if (sr->len_ccu > 0) {
    // E2 Node Component Configuration Update List. Optional
    E2AP_E2setupRequestIEs_t* comp_update_ie = calloc(1, sizeof(E2AP_E2setupRequestIEs_t));
    comp_update_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate;
    comp_update_ie->criticality = E2AP_Criticality_reject;
    comp_update_ie->value.present = E2AP_E2setupRequestIEs__value_PR_E2nodeComponentConfigUpdate_List;

    for(size_t i = 0; i < sr->len_ccu; ++i){
      // E2 Node Component Configuration Update Item
      E2AP_E2nodeComponentConfigUpdate_ItemIEs_t* comp_update_item_ie = copy_e2_node_component_conf_update(&sr->comp_conf_update[i]);
      rc = ASN_SEQUENCE_ADD(&comp_update_ie->value.choice.E2nodeComponentConfigUpdate_List.list, comp_update_item_ie);
      assert(rc == 0);
    }
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, comp_update_ie);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_setup_response_asn(const e2_setup_response_t* sr)
{
  assert(sr);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_setup_response_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_setup_response_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_SETUP_RESPONSE);
  return e2ap_enc_setup_response_asn(&msg->u_msgs.e2_stp_resp);
}

struct E2AP_E2AP_PDU* e2ap_enc_setup_response_asn_pdu(const e2_setup_response_t* sr)
{
  assert(sr != NULL);
  assert(sr->len_acc <= (size_t)MAX_NUM_RAN_FUNC_ID);
  assert(sr->len_rej <= (size_t)MAX_NUM_RAN_FUNC_ID);
  assert(sr->len_ccual <= (size_t)MAX_NUM_E2_NODE_COMPONENTS);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2setup;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_E2setupResponse;

  E2AP_E2setupResponse_t* out = &pdu->choice.successfulOutcome->value.choice.E2setupResponse;

  // Global RIC ID. Mandatory
  E2AP_E2setupResponseIEs_t* setup_rid = calloc(1, sizeof(E2AP_E2setupResponseIEs_t));
  setup_rid->id = E2AP_ProtocolIE_ID_id_GlobalRIC_ID;
  setup_rid->criticality = E2AP_Criticality_reject;
  setup_rid->value.present = E2AP_E2setupResponseIEs__value_PR_GlobalRIC_ID;
  MCC_MNC_TO_PLMNID(sr->id.plmn.mcc, sr->id.plmn.mnc ,sr->id.plmn.mnc_digit_len, &setup_rid->value.choice.GlobalRIC_ID.pLMN_Identity);
  MACRO_ENB_ID_TO_BIT_STRING(sr->id.near_ric_id.double_word, &setup_rid->value.choice.GlobalRIC_ID.ric_ID);

  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
  assert(rc == 0);

  // List of RAN Functions Accepted
  if(sr->len_acc > 0){
    E2AP_E2setupResponseIEs_t* ran_func = calloc(1, sizeof(E2AP_E2setupResponseIEs_t));
    ran_func->id = E2AP_ProtocolIE_ID_id_RANfunctionsAccepted;
    ran_func->criticality = E2AP_Criticality_reject;
    ran_func->value.present = E2AP_E2setupResponseIEs__value_PR_RANfunctionsID_List;
    for (size_t i = 0; i < sr->len_acc; ++i) {
      E2AP_RANfunctionID_ItemIEs_t* ran_item = calloc(1, sizeof(E2AP_RANfunctionID_ItemIEs_t));
      ran_item->id = E2AP_ProtocolIE_ID_id_RANfunctionID_Item;
      ran_item->criticality = E2AP_Criticality_ignore;
      ran_item->value.present = E2AP_RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item;
      // RAN Function ID. Mandatory
      ran_item->value.choice.RANfunctionID_Item.ranFunctionID = sr->accepted[i];
      rc = ASN_SEQUENCE_ADD(&ran_func->value.choice.RANfunctionsID_List.list, ran_item);
      assert(rc == 0);
    }
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_func);
    assert(rc == 0);
  }
  
  // List of RAN Functions Rejected
  if(sr->len_rej > 0){
    E2AP_E2setupResponseIEs_t* ran_rej = calloc(1, sizeof(E2AP_E2setupResponseIEs_t));
    ran_rej->id = E2AP_ProtocolIE_ID_id_RANfunctionsRejected;
    ran_rej->criticality = E2AP_Criticality_reject;
    ran_rej->value.present = E2AP_E2setupResponseIEs__value_PR_RANfunctionsIDcause_List;
    for (size_t i = 0; i < sr->len_rej; ++i) {
      E2AP_RANfunctionIDcause_ItemIEs_t* ran_item = calloc(1, sizeof(E2AP_RANfunctionIDcause_ItemIEs_t));
      ran_item->id = E2AP_ProtocolIE_ID_id_RANfunctionID_Item;
      ran_item->criticality = E2AP_Criticality_ignore;
      ran_item->value.present = E2AP_RANfunctionIDcause_ItemIEs__value_PR_RANfunctionIDcause_Item;
      // RAN Function ID. Mandatory
      ran_item->value.choice.RANfunctionIDcause_Item.ranFunctionID = sr->rejected[i].id; 
      // Cause. Mandatory
      const cause_t* c =  &sr->rejected[i].cause;
      E2AP_Cause_t* dst = &ran_item->value.choice.RANfunctionIDcause_Item.cause;

      switch(c->present){
        case CAUSE_RICREQUEST :{
          dst->present = E2AP_Cause_PR_ricRequest;
          assert(c->ricRequest < 11);
          dst->choice.ricRequest = c->ricRequest;
          break;
                               } 
        case CAUSE_RICSERVICE :{
          dst->present = E2AP_Cause_PR_ricService;
          assert(c->ricService < 3);
          dst->choice.ricService = c->ricService;
          break;
                               }
        case  CAUSE_TRANSPORT :{
          dst->present = E2AP_Cause_PR_transport;
          assert(c->transport < 2);
          dst->choice.transport = c->transport;
          break;
                               }
        case CAUSE_PROTOCOL :{
          dst->present = E2AP_Cause_PR_protocol;
          assert(c->protocol < 7);
          dst->choice.protocol = c->protocol;
          break;
                             }

        case CAUSE_MISC :{
          dst->present = E2AP_Cause_PR_misc;
          assert(c->misc < 4);
          dst->choice.misc = c->misc;
          break;
                         }

        case CAUSE_NOTHING:
        default:{

                  assert(0!=0 && "Invalid code path");
                  break;
                }
      }
      rc = ASN_SEQUENCE_ADD(&ran_rej->value.choice.RANfunctionsID_List.list, ran_item);
      assert(rc == 0);
    }
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_rej);
    assert(rc == 0);
  }

  if(sr->len_ccual > 0){
    // E2 Node Component Configuration Update Acknowledge List
    E2AP_E2setupResponseIEs_t* comp_conf = calloc(1, sizeof(E2AP_E2setupResponseIEs_t));
    comp_conf->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdateAck;
    comp_conf->criticality = E2AP_Criticality_reject;
    comp_conf->value.present = E2AP_E2setupResponseIEs__value_PR_E2nodeComponentConfigUpdateAck_List;

    for (size_t i = 0; i < sr->len_ccual; ++i) {
      E2AP_E2nodeComponentConfigUpdateAck_ItemIEs_t* comp_config = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateAck_ItemIEs_t));
      comp_config->id = E2AP_ProtocolIE_ID_id_RANfunctionID_Item;
      comp_config->criticality = E2AP_Criticality_ignore;
      comp_config->value.present = E2AP_E2nodeComponentConfigUpdateAck_ItemIEs__value_PR_E2nodeComponentConfigUpdateAck_Item;
      assert(0!=0 && "Not implemented");
      // RAN Function ID. Mandatory
      // Cause. Mandatory
//      comp_config->value.choice.E2nodeComponentConfigUpdateAck_Item = comp_conf_update_ack[i]; 
      rc = ASN_SEQUENCE_ADD(&comp_conf->value.choice.RANfunctionsIDcause_List.list, comp_config);
      assert(rc == 0);
    }
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, comp_conf);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_setup_failure_asn(const e2_setup_failure_t* sf)
{
  assert(sf);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_setup_failure_asn_pdu(sf);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_setup_failure_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == E2_SETUP_FAILURE);
  return e2ap_enc_setup_failure_asn(&msg->u_msgs.e2_stp_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_setup_failure_asn_pdu(const e2_setup_failure_t* sf)
{
  //Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2setup;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_E2setupFailure;

  E2AP_E2setupFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.E2setupFailure;

  // Cause. Mandatory
  E2AP_E2setupFailureIEs_t* cause = calloc(1, sizeof(E2AP_E2setupFailureIEs_t));
  cause->id = E2AP_ProtocolIE_ID_id_E2connectionSetupFailed;
  cause->criticality = E2AP_Criticality_ignore;
  cause->value.present = E2AP_E2setupFailureIEs__value_PR_Cause;
  cause->value.choice.Cause = copy_cause(sf->cause);
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause);
  assert(rc == 0);

  //Time To Wait. Optional 
  if(sf->time_to_wait_ms != NULL){
    E2AP_E2setupFailureIEs_t* time_to_wait = calloc(1, sizeof(E2AP_E2setupFailureIEs_t));
    time_to_wait->id = E2AP_ProtocolIE_ID_id_TimeToWait;
    time_to_wait->criticality = E2AP_Criticality_ignore;
    time_to_wait->value.present = E2AP_E2setupFailureIEs__value_PR_TimeToWait;
    time_to_wait->value.choice.TimeToWait = *sf->time_to_wait_ms;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, time_to_wait);
    assert(rc == 0);
  }

  //Criticality Diagnostics. Optional
  if(sf->crit_diag != NULL){
    E2AP_E2setupFailureIEs_t* crit_diagnostic = calloc(1, sizeof(E2AP_E2setupFailureIEs_t));
    crit_diagnostic->id = E2AP_ProtocolIE_ID_id_CriticalityDiagnostics;
    crit_diagnostic->criticality = E2AP_Criticality_ignore;
    crit_diagnostic->value.present = E2AP_E2setupFailureIEs__value_PR_CriticalityDiagnostics;
    assert(0 != 0 && "Not implemented");
    //  crit_diagnostic->value.choice.CriticalityDiagnostics = *crit_diagn;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, crit_diagnostic);
    assert(rc == 0);
  }

  //Transport Layer Information. Optional
  if(sf->tl_info != NULL){
    E2AP_E2setupFailureIEs_t* transport_layer = calloc(1, sizeof(E2AP_E2setupFailureIEs_t));
    transport_layer->id = E2AP_ProtocolIE_ID_id_TNLinformation;
    transport_layer->criticality = E2AP_Criticality_ignore;
    transport_layer->value.present = E2AP_E2setupFailureIEs__value_PR_TNLinformation;
    transport_layer->value.choice.TNLinformation.tnlAddress = copy_ba_to_bit_string(sf->tl_info->address);
    if(sf->tl_info->port != NULL){
      transport_layer->value.choice.TNLinformation.tnlPort = calloc(1, sizeof(BIT_STRING_t));
      *transport_layer->value.choice.TNLinformation.tnlPort = copy_ba_to_bit_string(*sf->tl_info->port);
    }

    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, transport_layer);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_reset_request_asn(const e2ap_reset_request_t* rr)
{
  assert(rr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_reset_request_asn_pdu(rr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_reset_request_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2AP_RESET_REQUEST );
  return e2ap_enc_reset_request_asn(&msg->u_msgs.rst_req);
}

E2AP_E2AP_PDU_t* e2ap_enc_reset_request_asn_pdu(const e2ap_reset_request_t* rr)
{
  assert(rr != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_Reset;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_ResetRequest;

  E2AP_ResetRequest_t* out = &pdu->choice.initiatingMessage->value.choice.ResetRequest;

  // Cause. Mandatory
  E2AP_ResetRequestIEs_t* cause = calloc(1, sizeof(E2AP_ResetRequestIEs_t));
  cause->criticality = E2AP_Criticality_ignore;
  cause->id = E2AP_ProtocolIE_ID_id_Cause;
  cause->value.present = E2AP_ResetRequestIEs__value_PR_Cause;
  cause->value.choice.Cause = copy_cause(rr->cause);
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_reset_response_asn(const e2ap_reset_response_t* rr)
{
  assert(rr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_reset_response_asn_pdu(rr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_reset_response_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2AP_RESET_RESPONSE);
  return e2ap_enc_reset_response_asn(&msg->u_msgs.rst_resp);
}

E2AP_E2AP_PDU_t* e2ap_enc_reset_response_asn_pdu(const e2ap_reset_response_t* rr)
{
  assert(rr != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_Reset;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_ResetResponse;

  E2AP_ResetResponse_t* out = &pdu->choice.successfulOutcome->value.choice.ResetResponse;

  // Criticality Diagnostics. Optional
  if(rr->crit_diag != NULL){
    E2AP_ResetResponseIEs_t* res = calloc(1, sizeof(E2AP_ResetResponseIEs_t));
    res->id = E2AP_ProtocolIE_ID_id_CriticalityDiagnostics;
    res->criticality = E2AP_Criticality_ignore;
    res->value.present = E2AP_ResetResponseIEs__value_PR_CriticalityDiagnostics;
    assert(0 != 0 && "Not implemented");
    // res->value.choice.CriticalityDiagnostics = *crit_diag;
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, res);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_service_update_asn(const ric_service_update_t* su)
{
  assert(su != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_service_update_asn_pdu(su);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_service_update_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SERVICE_UPDATE);
  return e2ap_enc_service_update_asn(&msg->u_msgs.ric_serv_updt);
}

E2AP_E2AP_PDU_t* e2ap_enc_service_update_asn_pdu(const ric_service_update_t* su)
{
  assert(su->len_added <= (size_t)MAX_NUM_RAN_FUNC_ID);
  assert(su->len_deleted <= (size_t)MAX_NUM_RAN_FUNC_ID );
  assert(su->len_modified <= (size_t)MAX_NUM_RAN_FUNC_ID);
  // Message Type
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICserviceUpdate;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICserviceUpdate;

  E2AP_RICserviceUpdate_t* out = &pdu->choice.initiatingMessage->value.choice.RICserviceUpdate;

  // List of RAN Functions Added
  E2AP_RICserviceUpdate_IEs_t* ran_add = calloc(1, sizeof(E2AP_RICserviceUpdate_IEs_t));
  ran_add->id = E2AP_ProtocolIE_ID_id_RANfunctionsAdded;
  ran_add->criticality = E2AP_Criticality_reject;
  ran_add->value.present = E2AP_RICserviceUpdate_IEs__value_PR_RANfunctions_List;

  for(size_t i = 0; i < su->len_added; ++i){
    E2AP_RANfunction_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
    r->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
    r->criticality = E2AP_Criticality_reject;
    r->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
    r->value.choice.RANfunction_Item = copy_ran_function(&su->added[i]);
    int rc = ASN_SEQUENCE_ADD(&ran_add->value.choice.RANfunctions_List.list, r);
    assert(rc == 0);
  }
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list,ran_add);
  assert(rc == 0);

  // List of RAN Functions Modified
  E2AP_RICserviceUpdate_IEs_t* ran_mod = calloc(1, sizeof(E2AP_RICserviceUpdate_IEs_t));
  ran_mod->id = E2AP_ProtocolIE_ID_id_RANfunctionsModified;
  ran_mod->criticality = E2AP_Criticality_reject;
  ran_mod->value.present = E2AP_RICserviceUpdate_IEs__value_PR_RANfunctions_List;

  for(size_t i = 0; i < su->len_modified; ++i){
    E2AP_RANfunction_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
    r->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
    r->criticality = E2AP_Criticality_reject;
    r->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
    r->value.choice.RANfunction_Item = copy_ran_function(&su->modified[i]);
    rc = ASN_SEQUENCE_ADD(&ran_mod->value.choice.RANfunctions_List.list, r);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_mod);
  assert(rc == 0);

  // List of RAN Functions Deleted
  E2AP_RICserviceUpdate_IEs_t* ran_del = calloc(1, sizeof(E2AP_RICserviceUpdate_IEs_t));
  ran_del->id = E2AP_ProtocolIE_ID_id_RANfunctionsDeleted;
  ran_del->criticality = E2AP_Criticality_reject;
  ran_del->value.present = E2AP_RICserviceUpdate_IEs__value_PR_RANfunctionsID_List;

  for(size_t i = 0; i < su->len_deleted; ++i){
    E2AP_RANfunction_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
    r->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
    r->criticality = E2AP_Criticality_reject;
    r->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
    E2AP_RANfunction_Item_t* dst = &r->value.choice.RANfunction_Item;
    const e2ap_ran_function_id_rev_t* src = &su->deleted[i];
    dst->ranFunctionID = src->id;
    dst->ranFunctionRevision = src->rev;
    rc = ASN_SEQUENCE_ADD(&ran_mod->value.choice.RANfunctions_List.list, r);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_del);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_service_update_ack_asn(const ric_service_update_ack_t* su)
{
  assert(su != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_service_update_ack_asn_pdu(su);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_service_update_ack_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SERVICE_UPDATE_ACKNOWLEDGE );
  return e2ap_enc_service_update_ack_asn(&msg->u_msgs.ric_serv_updt_ack);
}

E2AP_E2AP_PDU_t* e2ap_enc_service_update_ack_asn_pdu(const ric_service_update_ack_t* su)
{
  assert(su != NULL);
  assert(su->len_accepted <= (size_t)MAX_NUM_RAN_FUNC_ID);
  assert(su->len_rejected <= (size_t)MAX_NUM_RAN_FUNC_ID);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICserviceUpdate;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_RICserviceUpdateAcknowledge;

  E2AP_RICserviceUpdateAcknowledge_t* out = &pdu->choice.successfulOutcome->value.choice.RICserviceUpdateAcknowledge;

  if(su->len_accepted > 0){
    // List of RAN Functions Accepted
    E2AP_RICserviceUpdateAcknowledge_IEs_t* update_ack = calloc(1, sizeof(E2AP_RICserviceUpdateAcknowledge_IEs_t));
    update_ack->id = E2AP_ProtocolIE_ID_id_RANfunctionsAccepted;
    update_ack->criticality = E2AP_Criticality_ignore;
    update_ack->value.present = E2AP_RICserviceUpdateAcknowledge_IEs__value_PR_RANfunctionsID_List;
    for(size_t i = 0; i < su->len_accepted; ++i){
      E2AP_RANfunctionID_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunctionID_ItemIEs_t));
      r->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
      r->criticality = E2AP_Criticality_reject;
      r->value.present = E2AP_RANfunctionID_ItemIEs__value_PR_RANfunctionID_Item;
      r->value.choice.RANfunctionID_Item.ranFunctionID = su->accepted[i].id;
      r->value.choice.RANfunctionID_Item.ranFunctionRevision = su->accepted[i].rev;
      int rc = ASN_SEQUENCE_ADD(&update_ack->value.choice.RANfunctionsID_List.list, r);
      assert(rc == 0);
    }
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, update_ack);
    assert(rc == 0);
  }

  if(su->len_rejected > 0) {
    // List of RAN Functions Rejected
    E2AP_RICserviceUpdateAcknowledge_IEs_t* func_reject_ie = calloc(1, sizeof(E2AP_RICserviceUpdateAcknowledge_IEs_t));
    func_reject_ie->id = E2AP_ProtocolIE_ID_id_RANfunctionsRejected;
    func_reject_ie->criticality = E2AP_Criticality_ignore;
    func_reject_ie->value.present = E2AP_RICserviceUpdateAcknowledge_IEs__value_PR_RANfunctionsID_List;
    for(size_t i =0; i < su->len_rejected; ++i){
      E2AP_RANfunctionIDcause_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunctionIDcause_ItemIEs_t));
      r->criticality = E2AP_Criticality_ignore;
      r->id = E2AP_ProtocolIE_ID_id_RANfunctionIEcause_Item;
      r->value.present = E2AP_RANfunctionIDcause_ItemIEs__value_PR_RANfunctionIDcause_Item;
      E2AP_RANfunctionIDcause_Item_t* dst = &r->value.choice.RANfunctionIDcause_Item;
      const rejected_ran_function_t* src = &su->rejected[i];
      dst->ranFunctionID = src->id;
      dst->cause = copy_cause(src->cause);
      int rc = ASN_SEQUENCE_ADD(&func_reject_ie->value.choice.RANfunctionsIDcause_List.list, r);
      assert(rc == 0);
    }
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, func_reject_ie);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_service_update_failure_asn(const ric_service_update_failure_t* uf)
{
  assert(uf != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_service_update_failure_asn_pdu(uf);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_service_update_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SERVICE_UPDATE_FAILURE);
  return e2ap_enc_service_update_failure_asn(&msg->u_msgs.ric_serv_updt_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_service_update_failure_asn_pdu(const ric_service_update_failure_t* uf)
{
  assert(uf != NULL);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_RICserviceUpdate;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_RICserviceUpdateFailure;

  E2AP_RICserviceUpdateFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.RICserviceUpdateFailure;

  // List of RAN Functions Rejected. Mandatory
  E2AP_RICserviceUpdateFailure_IEs_t* update_failure = calloc(1, sizeof(E2AP_RICserviceUpdateFailure_IEs_t));
  update_failure->id = E2AP_ProtocolIE_ID_id_RANfunctionsRejected;
  update_failure->criticality = E2AP_Criticality_reject;
  update_failure->value.present = E2AP_RICserviceUpdateFailure_IEs__value_PR_RANfunctionsIDcause_List;

  int rc = -1;
  for(size_t i = 0; i < uf->len_rej; ++i){
    E2AP_RANfunctionIDcause_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunctionIDcause_ItemIEs_t));
    r->criticality = E2AP_Criticality_ignore;
    r->id = E2AP_ProtocolIE_ID_id_RANfunctionIEcause_Item;
    r->value.present = E2AP_RANfunctionIDcause_ItemIEs__value_PR_RANfunctionIDcause_Item;
    r->value.choice.RANfunctionIDcause_Item.ranFunctionID = uf->rejected[i].id;
    r->value.choice.RANfunctionIDcause_Item.cause = copy_cause(uf->rejected[i].cause);
    rc = ASN_SEQUENCE_ADD(&update_failure->value.choice.RANfunctionsIDcause_List.list, r);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, update_failure);
  assert(rc == 0);

  // Time To Wait. Optional
  if(uf->time_to_wait != NULL){
    E2AP_RICserviceUpdateFailure_IEs_t* time_wait = calloc(1, sizeof(E2AP_RICserviceUpdateFailure_IEs_t));
    time_wait->id = E2AP_ProtocolIE_ID_id_RANfunctionsRejected;
    time_wait->criticality = E2AP_Criticality_reject;
    time_wait->value.present = E2AP_RICserviceUpdateFailure_IEs__value_PR_TimeToWait;

    time_wait->value.choice.TimeToWait = *uf->time_to_wait; 
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, update_failure);
    assert(rc == 0);
  }

  //Criticality Diagnostics. Optional
  if(uf->crit_diag != NULL){
    E2AP_RICserviceUpdateFailure_IEs_t* crit_diag_ie = calloc(1, sizeof(E2AP_RICserviceUpdateFailure_IEs_t));
    crit_diag_ie->id = E2AP_ProtocolIE_ID_id_RANfunctionsRejected;
    crit_diag_ie->criticality = E2AP_Criticality_reject;
    crit_diag_ie->value.present = E2AP_RICserviceUpdateFailure_IEs__value_PR_CriticalityDiagnostics;
    assert(0!=0 && "Not implememnted");
//    crit_diag_ie->value.choice.CriticalityDiagnostics = *crit_diag;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, update_failure);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_service_query_asn(const ric_service_query_t* sq)
{
  assert(sq != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_service_query_asn_pdu(sq);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_service_query_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == RIC_SERVICE_QUERY );
  return e2ap_enc_service_query_asn(&msg->u_msgs.ric_serv_query);
}

struct E2AP_E2AP_PDU* e2ap_enc_service_query_asn_pdu(const ric_service_query_t* sq)
{
  //Message Type. Mandatory
  assert(sq->len_accepted <= (size_t)MAX_NUM_RAN_FUNC_ID);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_RICserviceQuery;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_RICserviceQuery;

  E2AP_RICserviceQuery_t* out = &pdu->choice.initiatingMessage->value.choice.RICserviceQuery;

  // List of RAN Functions Accepted. Mandatory
  E2AP_RICserviceQuery_IEs_t* serv_query_ie = calloc(1, sizeof(E2AP_RICserviceQuery_IEs_t));
  serv_query_ie->id = E2AP_ProtocolIE_ID_id_RANfunctionsAccepted;
  serv_query_ie->criticality = E2AP_Criticality_ignore;
  serv_query_ie->value.present = E2AP_RICserviceQuery_IEs__value_PR_RANfunctionsID_List;
  for(size_t i = 0; i < sq->len_accepted; ++i){
    E2AP_RANfunction_ItemIEs_t* r = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
    r->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
    r->criticality = E2AP_Criticality_reject;
    r->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
    r->value.choice.RANfunction_Item.ranFunctionID = sq->accepted[i].id;
    r->value.choice.RANfunction_Item.ranFunctionRevision = sq->accepted[i].rev;
    int rc = ASN_SEQUENCE_ADD(&serv_query_ie->value.choice.RANfunctionsID_List.list, r);
    assert(rc == 0);
  }
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, serv_query_ie);
  assert(rc == 0);
  return pdu;
}

byte_array_t e2ap_enc_node_configuration_update_asn(const e2_node_configuration_update_t* cu)
{
  assert(cu != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_configuration_update_asn_pdu(cu);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_configuration_update_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_NODE_CONFIGURATION_UPDATE);
  return e2ap_enc_node_configuration_update_asn(&msg->u_msgs.e2_node_conf_updt); 
}

struct E2AP_E2AP_PDU* e2ap_enc_node_configuration_update_asn_pdu(const e2_node_configuration_update_t* cu)
{
  assert(cu->len_ccul <= (size_t)MAX_NUM_ACTION_DEF);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E2nodeConfigurationUpdate;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E2nodeConfigurationUpdate;

  E2AP_E2nodeConfigurationUpdate_t* out = &pdu->choice.initiatingMessage->value.choice.E2nodeConfigurationUpdate;

  // E2 Node Component Configuration Update List
  E2AP_E2nodeConfigurationUpdate_IEs_t* conf_update_ie = calloc(1, sizeof(E2AP_E2nodeConfigurationUpdate_IEs_t));
  conf_update_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate;
  conf_update_ie->criticality = E2AP_Criticality_ignore;
  conf_update_ie->value.present = E2AP_E2nodeConfigurationUpdate_IEs__value_PR_E2nodeComponentConfigUpdate_List;
  for(size_t i = 0; i < cu->len_ccul; ++i){
    E2AP_E2nodeComponentConfigUpdate_ItemIEs_t* n = copy_e2_node_component_conf_update(&cu->comp_conf_update_list[i]);
    int rc = ASN_SEQUENCE_ADD(&conf_update_ie->value.choice.E2nodeComponentConfigUpdate_List.list, n);
    assert(rc == 0);
  }
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, conf_update_ie);
  assert(rc == 0);

  return pdu;
}
 
byte_array_t e2ap_enc_node_configuration_update_ack_asn(const e2_node_configuration_update_ack_t* cua)
{
  assert(cua != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_configuration_update_ack_asn_pdu(cua);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_configuration_update_ack_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_NODE_CONFIGURATION_UPDATE_ACKNOWLEDGE);
  return e2ap_enc_node_configuration_update_ack_asn(&msg->u_msgs.e2_node_conf_updt_ack);
}

E2AP_E2AP_PDU_t* e2ap_enc_node_configuration_update_ack_asn_pdu(const e2_node_configuration_update_ack_t* cua)
{
  assert(cua != NULL);
  assert(cua->len_ccual <= (size_t)MAX_NUM_ACTION_DEF);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2nodeConfigurationUpdate;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_E2nodeConfigurationUpdateAcknowledge;

  E2AP_E2nodeConfigurationUpdateAcknowledge_t* out = &pdu->choice.successfulOutcome->value.choice.E2nodeConfigurationUpdateAcknowledge;

  // E2 Node Component Configuration Update Acknowledge List
  E2AP_E2nodeConfigurationUpdateAcknowledge_IEs_t* conf_update_ack = calloc(1, sizeof(E2AP_E2nodeConfigurationUpdateAcknowledge_IEs_t));
  conf_update_ack->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdateAck;
  conf_update_ack->criticality = E2AP_Criticality_reject;
  conf_update_ack->value.present = E2AP_E2nodeConfigurationUpdateAcknowledge_IEs__value_PR_E2nodeComponentConfigUpdateAck_List;

  for(size_t i = 0; i < cua->len_ccual; ++i){
    E2AP_E2nodeComponentConfigUpdateAck_ItemIEs_t* n = calloc(1, sizeof(E2AP_E2nodeComponentConfigUpdateAck_ItemIEs_t));
    n->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdateAck_Item;
    n->criticality = E2AP_Criticality_reject;
    n->value.present = E2AP_E2nodeComponentConfigUpdateAck_ItemIEs__value_PR_E2nodeComponentConfigUpdateAck_Item;
    E2AP_E2nodeComponentConfigUpdateAck_Item_t* dst = &n->value.choice.E2nodeComponentConfigUpdateAck_Item;
    const e2_node_component_config_update_ack_item_t* src = &cua->comp_conf_update_ack_list[i];
    dst->e2nodeComponentType = src->e2_node_component_type;
    if(src->id_present != NULL){
      dst->e2nodeComponentID = calloc(1, sizeof(E2AP_E2nodeComponentID_t));
      if (src->id_present == E2_NODE_COMPONENT_ID_E2_NODE_COMPONENT_TYPE_GNB_CU_UP)
        dst->e2nodeComponentID->present = E2AP_E2nodeComponentID_PR_e2nodeComponentTypeGNB_CU_UP;
      else // src->id_present == E2_NODE_COMPONENT_ID_E2_NODE_COMPONENT_TYPE_GNB_CU_UP
        dst->e2nodeComponentID->present = E2AP_E2nodeComponentID_PR_e2nodeComponentTypeGNB_DU;
    }
    if(src->failure_cause != NULL){
      dst->e2nodeComponentConfigUpdateAck.failureCause = calloc(1, sizeof(E2AP_Cause_t));
      *dst->e2nodeComponentConfigUpdateAck.failureCause = copy_cause(*src->failure_cause ); 
      dst->e2nodeComponentConfigUpdateAck.updateOutcome = src->update_outcome;
    }
    int rc = ASN_SEQUENCE_ADD(&conf_update_ack->value.choice.E2nodeComponentConfigUpdateAck_List.list, n);
    assert(rc == 0);
  }

  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, conf_update_ack);
  assert(rc == 0);

  return pdu;
}

byte_array_t e2ap_enc_node_configuration_update_failure_asn(const e2_node_configuration_update_failure_t* cuf)
{
  assert(cuf != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_configuration_update_failure_asn_pdu(cuf);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_configuration_update_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_NODE_CONFIGURATION_UPDATE_FAILURE);
  return e2ap_enc_node_configuration_update_failure_asn(&msg->u_msgs.e2_node_conf_updt_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_node_configuration_update_failure_asn_pdu(const e2_node_configuration_update_failure_t* cuf)
{
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2nodeConfigurationUpdate;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_E2nodeConfigurationUpdateFailure;

  E2AP_E2nodeConfigurationUpdateFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.E2nodeConfigurationUpdateFailure;

  // Cause. Mandatory
  E2AP_E2nodeConfigurationUpdateFailure_IEs_t* cause_ie = calloc(1, sizeof(E2AP_E2nodeConfigurationUpdateFailure_IEs_t));
  cause_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate;
  cause_ie->criticality = E2AP_Criticality_reject;
  cause_ie->value.present = E2AP_E2nodeConfigurationUpdateFailure_IEs__value_PR_Cause;
  cause_ie->value.choice.Cause = copy_cause(cuf->cause);

  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause_ie);
  assert(rc == 0);

  // Time to wait. Optional
  if(cuf->time_wait != NULL){
    E2AP_E2nodeConfigurationUpdateFailure_IEs_t* cause_ie = calloc(1, sizeof(E2AP_E2nodeConfigurationUpdateFailure_IEs_t));
    cause_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate;
    cause_ie->criticality = E2AP_Criticality_reject;
    cause_ie->value.present = E2AP_E2nodeConfigurationUpdateFailure_IEs__value_PR_TimeToWait;
    cause_ie->value.choice.TimeToWait = *cuf->time_wait;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause_ie);
    assert(rc == 0);
  }

  if(cuf->crit_diag != NULL){
    E2AP_E2nodeConfigurationUpdateFailure_IEs_t* cause_ie = calloc(1, sizeof(E2AP_E2nodeConfigurationUpdateFailure_IEs_t));
    cause_ie->id = E2AP_ProtocolIE_ID_id_E2nodeComponentConfigUpdate;
    cause_ie->criticality = E2AP_Criticality_reject;
    cause_ie->value.present = E2AP_E2nodeConfigurationUpdateFailure_IEs__value_PR_CriticalityDiagnostics;
    assert(0!=0 && "Not implemented");
    //cause_ie->value.choice.CriticalityDiagnostics = *cd;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause_ie);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_node_connection_update_asn(const e2_node_connection_update_t* cu)
{
  assert(cu != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_connection_update_asn_pdu(cu);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_connection_update_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_CONNECTION_UPDATE);
  return e2ap_enc_node_connection_update_asn(&msg->u_msgs.e2_conn_updt);
}

E2AP_E2AP_PDU_t* e2ap_enc_node_connection_update_asn_pdu(const e2_node_connection_update_t* cu)
{
  assert(cu != NULL);
  assert(cu->len_add <= (size_t)MAX_NUM_TNLA);
  assert(cu->len_rem <= (size_t)MAX_NUM_TNLA);
  assert(cu->len_mod <= (size_t)MAX_NUM_TNLA);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E2connectionUpdate;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E2connectionUpdate;

  E2AP_E2connectionUpdate_t* out = &pdu->choice.initiatingMessage->value.choice.E2connectionUpdate;

  // E2 Connection To Add List
  if(cu->len_add > 0){
    E2AP_E2connectionUpdate_IEs_t* con_update_ie = calloc(1, sizeof(E2AP_E2connectionUpdate_IEs_t));
    con_update_ie->id = E2AP_ProtocolIE_ID_id_E2connectionUpdateAdd;
    con_update_ie->criticality = E2AP_Criticality_ignore;
    con_update_ie->value.present = E2AP_E2connectionUpdate_IEs__value_PR_E2connectionUpdate_List;

    for (size_t i = 0; i < cu->len_add; ++i) {
      E2AP_E2connectionUpdate_ItemIEs_t* c = calloc(1, sizeof(E2AP_E2connectionUpdate_ItemIEs_t));
      c->id = E2AP_ProtocolIE_ID_id_E2connectionUpdate_Item;
      ;
      c->criticality = E2AP_Criticality_ignore;
      c->value.present = E2AP_E2connectionUpdate_ItemIEs__value_PR_E2connectionUpdate_Item;
      E2AP_E2connectionUpdate_Item_t* dst = &c->value.choice.E2connectionUpdate_Item;
      const e2_connection_update_item_t* src = &cu->add[i];
      dst->tnlUsage = src->usage;
      dst->tnlInformation.tnlAddress = copy_ba_to_bit_string(src->info.tnl_addr);
      if (src->info.tnl_port != NULL) {
        dst->tnlInformation.tnlPort = calloc(1, sizeof(BIT_STRING_t));
        *dst->tnlInformation.tnlPort = copy_ba_to_bit_string(*src->info.tnl_port);
      }

      int rc = ASN_SEQUENCE_ADD(&con_update_ie->value.choice.E2connectionUpdate_List.list, c);
      assert(rc == 0);
  }
   int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, con_update_ie);
   assert(rc == 0);
  }
  // E2 Connection To Remove List
  if(cu->len_rem > 0){
    E2AP_E2connectionUpdate_IEs_t* con_update_ie = calloc(1, sizeof(E2AP_E2connectionUpdate_IEs_t));
    con_update_ie->id = E2AP_ProtocolIE_ID_id_E2connectionUpdateRemove;
    con_update_ie->criticality = E2AP_Criticality_ignore;
    con_update_ie->value.present = E2AP_E2connectionUpdate_IEs__value_PR_E2connectionUpdateRemove_List;

    for (size_t i = 0; i < cu->len_rem; ++i) {
      E2AP_E2connectionUpdateRemove_ItemIEs_t* c = calloc(1, sizeof(E2AP_E2connectionUpdateRemove_ItemIEs_t));
      c->id = E2AP_ProtocolIE_ID_id_E2connectionUpdate_Item;
      ;
      c->criticality = E2AP_Criticality_ignore;
      c->value.present = E2AP_E2connectionUpdateRemove_ItemIEs__value_PR_E2connectionUpdateRemove_Item;
      E2AP_E2connectionUpdateRemove_Item_t* dst = &c->value.choice.E2connectionUpdateRemove_Item;
      const e2_connection_update_remove_item_t* src = &cu->rem[i];
      dst->tnlInformation.tnlAddress = copy_ba_to_bit_string(src->info.tnl_addr);
      if (src->info.tnl_port != NULL) {
        dst->tnlInformation.tnlPort = calloc(1, sizeof(BIT_STRING_t));
        *dst->tnlInformation.tnlPort = copy_ba_to_bit_string(*src->info.tnl_port);
      }
      int rc = ASN_SEQUENCE_ADD(&con_update_ie->value.choice.E2connectionUpdate_List_1.list, c);
      assert(rc == 0);
  }
   int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, con_update_ie);
   assert(rc == 0);
  }

  // E2 Connection To Modify List
  if(cu->len_mod > 0){
    E2AP_E2connectionUpdate_IEs_t* con_update_ie = calloc(1, sizeof(E2AP_E2connectionUpdate_IEs_t));
    con_update_ie->id = E2AP_ProtocolIE_ID_id_E2connectionUpdateModify;
    con_update_ie->criticality = E2AP_Criticality_ignore;
    con_update_ie->value.present = E2AP_E2connectionUpdate_IEs__value_PR_E2connectionUpdate_List;

    for (size_t i = 0; i < cu->len_mod; ++i) {
      E2AP_E2connectionUpdate_ItemIEs_t* c = calloc(1, sizeof(E2AP_E2connectionUpdate_ItemIEs_t));
      c->id = E2AP_ProtocolIE_ID_id_E2connectionUpdateModify;
      ;
      c->criticality = E2AP_Criticality_ignore;
      c->value.present = E2AP_E2connectionUpdate_ItemIEs__value_PR_E2connectionUpdate_Item;
      E2AP_E2connectionUpdate_Item_t* dst = &c->value.choice.E2connectionUpdate_Item;
      const e2_connection_update_item_t* src = &cu->mod[i];
      dst->tnlUsage = src->usage;
      dst->tnlInformation.tnlAddress = copy_ba_to_bit_string(src->info.tnl_addr);
      if (src->info.tnl_port != NULL) {
        dst->tnlInformation.tnlPort = calloc(1, sizeof(BIT_STRING_t));
        *dst->tnlInformation.tnlPort = copy_ba_to_bit_string(*src->info.tnl_port);
      }
      int rc = ASN_SEQUENCE_ADD(&con_update_ie->value.choice.E2connectionUpdate_List.list, c);
      assert(rc == 0);
  }
   int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, con_update_ie);
   assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_node_connection_update_ack_asn(const e2_node_connection_update_ack_t* ca)
{
  assert(ca != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_connection_update_ack_asn_pdu(ca);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_connection_update_ack_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_CONNECTION_UPDATE_ACKNOWLEDGE);
  return e2ap_enc_node_connection_update_ack_asn(&msg->u_msgs.e2_conn_updt_ack);
}

E2AP_E2AP_PDU_t* e2ap_enc_node_connection_update_ack_asn_pdu(const e2_node_connection_update_ack_t* ca)
{
  assert(ca != NULL);
  assert(ca->len_setup <= (size_t)MAX_NUM_TNLA);
  assert(ca->len_failed <= (size_t)MAX_NUM_TNLA);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2connectionUpdate;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_E2connectionUpdateAcknowledge;

  E2AP_E2connectionUpdateAcknowledge_t* out = &pdu->choice.successfulOutcome->value.choice.E2connectionUpdateAcknowledge;

  // E2 Connection Setup List
  if(ca->len_setup > 0){
    E2AP_E2connectionUpdateAck_IEs_t* con_update_ie = calloc(1, sizeof(E2AP_E2connectionUpdateAck_IEs_t));
    con_update_ie->criticality = E2AP_Criticality_ignore;
    con_update_ie->id = E2AP_ProtocolIE_ID_id_E2connectionSetup;
    con_update_ie->value.present = E2AP_E2connectionUpdateAck_IEs__value_PR_E2connectionUpdate_List;

    for(size_t i = 0; i < ca->len_setup; ++i){
      E2AP_E2connectionUpdate_ItemIEs_t* c = calloc(1, sizeof(E2AP_E2connectionUpdate_ItemIEs_t));
      c->id = E2AP_ProtocolIE_ID_id_E2connectionUpdate_Item;
      ;
      c->criticality = E2AP_Criticality_ignore;
      c->value.present = E2AP_E2connectionUpdate_ItemIEs__value_PR_E2connectionUpdate_Item;
      E2AP_E2connectionUpdate_Item_t* dst = &c->value.choice.E2connectionUpdate_Item;
      const e2_connection_update_item_t* src = &ca->setup[i];
      dst->tnlUsage = src->usage;
      dst->tnlInformation.tnlAddress = copy_ba_to_bit_string(src->info.tnl_addr);
      if(src->info.tnl_port != NULL){
        dst->tnlInformation.tnlPort = calloc(1, sizeof(BIT_STRING_t)); 
        *dst->tnlInformation.tnlPort = copy_ba_to_bit_string(*src->info.tnl_port);
      }
      int rc = ASN_SEQUENCE_ADD(&con_update_ie->value.choice.E2connectionUpdate_List.list, c);
      assert(rc == 0);
    }
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, con_update_ie);
    assert(rc == 0);
  }

  // E2 Connection Failed to Setup List
  if(ca->len_failed > 0){
    E2AP_E2connectionUpdateAck_IEs_t* con_update_ie = calloc(1, sizeof(E2AP_E2connectionUpdateAck_IEs_t));
    con_update_ie->criticality = E2AP_Criticality_ignore;
    con_update_ie->id = E2AP_ProtocolIE_ID_id_E2connectionSetupFailed;
    con_update_ie->value.present = E2AP_E2connectionUpdateAck_IEs__value_PR_E2connectionSetupFailed_List;

    for(size_t  i = 0; i < ca->len_failed; ++i){
      E2AP_E2connectionSetupFailed_ItemIEs_t* c = calloc(1, sizeof(E2AP_E2connectionSetupFailed_ItemIEs_t));
      c->id = E2AP_ProtocolIE_ID_id_E2connectionSetupFailed_Item;
      c->criticality = E2AP_Criticality_ignore;
      c->value.present = E2AP_E2connectionSetupFailed_ItemIEs__value_PR_E2connectionSetupFailed_Item;
      E2AP_E2connectionSetupFailed_Item_t* dst = &c->value.choice.E2connectionSetupFailed_Item; // = con_failed[i];
      const e2_connection_setup_failed_t* src = &ca->failed[i]; 
      dst->cause = copy_cause(src->cause);
      dst->tnlInformation.tnlAddress = copy_ba_to_bit_string(src->info.tnl_addr);
      if(src->info.tnl_port != NULL){
        dst->tnlInformation.tnlPort = calloc(1, sizeof(BIT_STRING_t)); 
        *dst->tnlInformation.tnlPort = copy_ba_to_bit_string(*src->info.tnl_port);
      }
      int rc = ASN_SEQUENCE_ADD(&con_update_ie->value.choice.E2connectionSetupFailed_List.list, c);
      assert(rc == 0);
    }
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, con_update_ie);
    assert(rc == 0);
  }
  return pdu;
}

byte_array_t e2ap_enc_node_connection_update_failure_asn(const e2_node_connection_update_failure_t* cu)
{
  assert(cu != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_node_connection_update_failure_asn_pdu(cu);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_node_connection_update_failure_asn_msg(const e2ap_msg_t* msg) 
{
  assert(msg != NULL);
  assert(msg->type == E2_CONNECTION_UPDATE_FAILURE);
  return e2ap_enc_node_connection_update_failure_asn(&msg->u_msgs.e2_conn_updt_fail);
}

E2AP_E2AP_PDU_t* e2ap_enc_node_connection_update_failure_asn_pdu(const e2_node_connection_update_failure_t* cf)
{
  assert(cf != NULL);
  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_unsuccessfulOutcome;
  pdu->choice.unsuccessfulOutcome = calloc(1, sizeof(E2AP_UnsuccessfulOutcome_t));
  pdu->choice.unsuccessfulOutcome->procedureCode = E2AP_ProcedureCode_id_E2connectionUpdate;
  pdu->choice.unsuccessfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.unsuccessfulOutcome->value.present = E2AP_UnsuccessfulOutcome__value_PR_E2connectionUpdateFailure;

  E2AP_E2connectionUpdateFailure_t* out = &pdu->choice.unsuccessfulOutcome->value.choice.E2connectionUpdateFailure;

  // Cause. Mandatory
  E2AP_E2connectionUpdateFailure_IEs_t* cause = calloc(1, sizeof(E2AP_E2connectionUpdateFailure_IEs_t));
  cause->id = E2AP_ProtocolIE_ID_id_E2connectionFailed_Item;
  cause->criticality = E2AP_Criticality_reject;
  cause->value.present = E2AP_E2connectionUpdateFailure_IEs__value_PR_Cause;
  cause->value.choice.Cause = copy_cause(cf->cause);

  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, cause);
  assert(rc == 0);

  // Time to wait. Optional
  if (cf->time_wait != NULL) {
    E2AP_E2connectionUpdateFailure_IEs_t* time = calloc(1, sizeof(E2AP_E2connectionUpdateFailure_IEs_t));
    time->id = E2AP_ProtocolIE_ID_id_E2connectionFailed_Item;
    time->criticality = E2AP_Criticality_reject;
    time->value.present = E2AP_E2connectionUpdateFailure_IEs__value_PR_TimeToWait;
    time->value.choice.TimeToWait = *cf->time_wait;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, time);
    assert(rc == 0);
  }

  // Critical diagnoses. Optional
  if (cf->crit_diag != NULL) {
    E2AP_E2connectionUpdateFailure_IEs_t* crit = calloc(1, sizeof(E2AP_E2connectionUpdateFailure_IEs_t));
    crit->id = E2AP_ProtocolIE_ID_id_E2connectionFailed_Item;
    crit->criticality = E2AP_Criticality_reject;
    crit->value.present = E2AP_E2connectionUpdateFailure_IEs__value_PR_CriticalityDiagnostics;
    assert(0 != 0 && "Not implemented!");
    //     crit->value.choice.CriticalityDiagnostics = *crit_diag;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, crit);
    assert(rc == 0);
  }
  return pdu;
}


// xApp -> iApp
byte_array_t e2ap_enc_e42_subscription_request_asn(const e42_ric_subscription_request_t* sr)
{
  assert(sr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_e42_subscription_request_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  //printf("[E2AP] SUBSCRIPTION REQUEST generated\n");
  return ba;
}

byte_array_t e2ap_enc_e42_subscription_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == E42_RIC_SUBSCRIPTION_REQUEST );
  return e2ap_enc_e42_subscription_request_asn(&msg->u_msgs.e42_ric_sub_req);
}

struct E2AP_E2AP_PDU* e2ap_enc_e42_subscription_request_asn_pdu(const e42_ric_subscription_request_t* e42_sr)
{
  assert(e42_sr != NULL);
  ric_subscription_request_t const* sr = &e42_sr->sr;

  // action_def is optional, therefore it can be NULL
  assert(sr->event_trigger.buf != NULL && sr->event_trigger.len > 0);
  assert(sr->len_action <= (size_t)MAX_NUM_ACTION_DEF);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E42RICsubscription;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E42RICsubscriptionRequest;

  E2AP_E42RICsubscriptionRequest_t* out = &pdu->choice.initiatingMessage->value.choice.E42RICsubscriptionRequest;

  // XAPP ID. Mandatory
  E2AP_E42RICsubscriptionRequest_IEs_t* xapp_id = calloc(1, sizeof(E2AP_E42RICsubscriptionRequest_IEs_t));
  xapp_id->id = E2AP_ProtocolIE_ID_id_XAPP_ID;
  xapp_id->criticality = E2AP_Criticality_reject;
  xapp_id->value.present = E2AP_E42RICsubscriptionRequest_IEs__value_PR_XAPP_ID;
  xapp_id->value.choice.XAPP_ID = e42_sr->xapp_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, xapp_id);
  assert(rc == 0);

  // Global E2 Node ID. Mandatory
  E2AP_E42RICsubscriptionRequest_IEs_t* setup_rid = calloc(1, sizeof(E2AP_E42RICsubscriptionRequest_IEs_t));
  setup_rid->id = E2AP_ProtocolIE_ID_id_GlobalE2node_ID;
  setup_rid->criticality = E2AP_Criticality_reject;
  setup_rid->value.present = E2AP_E42RICsubscriptionRequest_IEs__value_PR_GlobalE2node_ID;
  assert(e42_sr->id.type == ngran_gNB || e42_sr->id.type == ngran_gNB_CU || e42_sr->id.type == ngran_gNB_DU || e42_sr->id.type == ngran_eNB);
  if (e42_sr->id.type != ngran_eNB) {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_gNB;

    E2AP_GlobalE2node_gNB_ID_t* e2gnb = calloc(1, sizeof(E2AP_GlobalE2node_gNB_ID_t));
    assert(e2gnb != NULL && "Memory exhasued");
    e2gnb->global_gNB_ID.gnb_id.present = E2AP_GNB_ID_Choice_PR_gnb_ID;
    if (e42_sr->id.type == ngran_gNB_CU) {
      E2AP_GNB_CU_UP_ID_t* e2gnb_cu_up_id = calloc(1, sizeof(E2AP_GNB_CU_UP_ID_t));
      assert(e2gnb_cu_up_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_cu_up_id, *e42_sr->id.cu_du_id);
      e2gnb->gNB_CU_UP_ID = e2gnb_cu_up_id;
    }
    else if (e42_sr->id.type == ngran_gNB_DU) {
      E2AP_GNB_DU_ID_t* e2gnb_du_id = calloc(1, sizeof(E2AP_GNB_DU_ID_t));
      assert(e2gnb_du_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_du_id, *e42_sr->id.cu_du_id);
      e2gnb->gNB_DU_ID = e2gnb_du_id;
    }

    const plmn_t *plmn = &e42_sr->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2gnb->global_gNB_ID.plmn_id);
    MACRO_GNB_ID_TO_BIT_STRING(e42_sr->id.nb_id, &e2gnb->global_gNB_ID.gnb_id.choice.gnb_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.gNB = e2gnb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  } else {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_eNB;
    E2AP_GlobalE2node_eNB_ID_t* e2enb = calloc(1, sizeof(E2AP_GlobalE2node_eNB_ID_t));
    assert(e2enb != NULL && "Memory exhasued");
    e2enb->global_eNB_ID.eNB_ID.present = E2AP_ENB_ID_PR_macro_eNB_ID;
    const plmn_t* plmn = &e42_sr->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2enb->global_eNB_ID.pLMN_Identity);
    MACRO_ENB_ID_TO_BIT_STRING(e42_sr->id.nb_id, &e2enb->global_eNB_ID.eNB_ID.choice.macro_eNB_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.eNB = e2enb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  }

  // RIC Request ID. Mandatory.
  E2AP_E42RICsubscriptionRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_E42RICsubscriptionRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_E42RICsubscriptionRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = sr->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = sr->ric_id.ric_req_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RAN Function ID. Mandatory
  E2AP_E42RICsubscriptionRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_E42RICsubscriptionRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_E42RICsubscriptionRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = sr->ric_id.ran_func_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);
  // RIC Subscription Details. Mandatory
  E2AP_E42RICsubscriptionRequest_IEs_t* sub_det = calloc(1, sizeof(E2AP_E42RICsubscriptionRequest_IEs_t));
  sub_det->id = E2AP_ProtocolIE_ID_id_RICsubscriptionDetails;
  sub_det->criticality = E2AP_Criticality_reject;
  sub_det->value.present = E2AP_E42RICsubscriptionRequest_IEs__value_PR_RICsubscriptionDetails;

  //RIC Event Trigger Definition. Mandatory
  sub_det->value.choice.RICsubscriptionDetails.ricEventTriggerDefinition = copy_ba_to_ostring(sr->event_trigger);

  //Sequence of Actions. Mandatory
  for(size_t i = 0; i < sr->len_action; ++i){
    E2AP_RICaction_ToBeSetup_ItemIEs_t* act_setup = calloc(1, sizeof(E2AP_RICaction_ToBeSetup_ItemIEs_t));
    act_setup->id = E2AP_ProtocolIE_ID_id_RICaction_ToBeSetup_Item;
    act_setup->criticality = E2AP_Criticality_reject;
    act_setup->value.present = E2AP_RICaction_ToBeSetup_ItemIEs__value_PR_RICaction_ToBeSetup_Item;
    E2AP_RICaction_ToBeSetup_Item_t* dst = &act_setup->value.choice.RICaction_ToBeSetup_Item;
    const ric_action_t* src = &sr->action[i];
    dst->ricActionID = src->id;
    dst->ricActionType = src->type;

    // RIC Action Definition. Optional 
    if( src->definition != NULL){
      dst->ricActionDefinition = calloc(1, sizeof(E2AP_RICactionDefinition_t));
      *dst->ricActionDefinition = copy_ba_to_ostring(*src->definition);
    }

    // RIC Subsequent Action. Optional 
    if(src->subseq_action != NULL){
      dst->ricSubsequentAction = calloc(1, sizeof(E2AP_RICsubsequentAction_t));
      dst->ricSubsequentAction->ricSubsequentActionType = src->subseq_action->type;
      if(src->subseq_action->time_to_wait_ms != NULL){
        assert(*src->subseq_action->time_to_wait_ms < 18);
        // Very strange type. Optional but is not a pointer....
        dst->ricSubsequentAction->ricTimeToWait = *src->subseq_action->time_to_wait_ms;
      }
    }

    rc = ASN_SEQUENCE_ADD(&sub_det->value.choice.RICsubscriptionDetails.ricAction_ToBeSetup_List.list, act_setup);
    assert(rc == 0);
  }
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_det);
  assert(rc == 0);
  return pdu;
}


// xApp -> iApp
byte_array_t e2ap_enc_e42_setup_request_asn(const e42_setup_request_t* sr)
{
  assert(sr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_e42_setup_request_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_e42_setup_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type ==  E42_SETUP_REQUEST );
  return e2ap_enc_e42_setup_request_asn(&msg->u_msgs.e42_stp_req);
}

struct E2AP_E2AP_PDU* e2ap_enc_e42_setup_request_asn_pdu(const e42_setup_request_t* sr)
{
  assert(sr->len_rf <= (size_t)MAX_NUM_RAN_FUNC_ID);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E42setup;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E42setupRequest;

  E2AP_E42setupRequest_t* out = &pdu->choice.initiatingMessage->value.choice.E42setupRequest;

  if(sr->len_rf > 0){
    // List of RAN Functions Added
    E2AP_E42setupRequestIEs_t* ran_list = calloc(1, sizeof(E2AP_E42setupRequestIEs_t));
    ran_list->id = E2AP_ProtocolIE_ID_id_RANfunctionsAdded;
    ran_list->criticality = E2AP_Criticality_reject;
    ran_list->value.present = E2AP_E42setupRequestIEs__value_PR_RANfunctions_List;

    for (size_t i = 0; i < sr->len_rf; ++i) {
      E2AP_RANfunction_ItemIEs_t* ran_function_item_ie = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
      ran_function_item_ie->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
      ran_function_item_ie->criticality = E2AP_Criticality_reject;
      ran_function_item_ie->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
      ran_function_item_ie->value.choice.RANfunction_Item = copy_ran_function(&sr->ran_func_item[i]);
      int rc = ASN_SEQUENCE_ADD(&ran_list->value.choice.RANfunctions_List.list, ran_function_item_ie);
      assert(rc == 0);
    }
    int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_list);
    assert(rc == 0);
  }

  return pdu;
}

// iApp -> xApp
byte_array_t e2ap_enc_e42_setup_response_asn(const e42_setup_response_t* sr)
{
  assert(sr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_e42_setup_response_asn_pdu(sr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);

  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_e42_setup_response_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type ==  E42_SETUP_RESPONSE);
  return e2ap_enc_e42_setup_response_asn(&msg->u_msgs.e42_stp_resp);
}

struct E2AP_E2AP_PDU* e2ap_enc_e42_setup_response_asn_pdu(const e42_setup_response_t* sr)
{
  assert(sr != NULL);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_successfulOutcome;
  pdu->choice.successfulOutcome = calloc(1, sizeof(E2AP_SuccessfulOutcome_t));
  pdu->choice.successfulOutcome->procedureCode = E2AP_ProcedureCode_id_E42setup;
  pdu->choice.successfulOutcome->criticality = E2AP_Criticality_reject;
  pdu->choice.successfulOutcome->value.present = E2AP_SuccessfulOutcome__value_PR_E42setupResponse;

  E2AP_E42setupResponse_t* out = &pdu->choice.successfulOutcome->value.choice.E42setupResponse;

  // XAPP ID. Mandatory
  E2AP_E42setupResponseIEs_t* setup_rid = calloc(1, sizeof(E2AP_E42setupResponseIEs_t));
  setup_rid->id = E2AP_ProtocolIE_ID_id_XAPP_ID;
  setup_rid->criticality = E2AP_Criticality_reject;
  setup_rid->value.present = E2AP_E42setupResponseIEs__value_PR_XAPP_ID;
  setup_rid->value.choice.XAPP_ID = sr-> xapp_id;

  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
  assert(rc == 0);
  assert(sr->len_e2_nodes_conn > 0 && "No global node conected??");

  for(size_t i = 0; i < sr->len_e2_nodes_conn; ++i){

    // E2 Node Connected List
    E2AP_E42setupResponseIEs_t* conn_list = calloc(1, sizeof(E2AP_E42setupResponseIEs_t));
    conn_list->id = E2AP_ProtocolIE_ID_id_E2nodesConnected;
    conn_list->criticality = E2AP_Criticality_reject;
    conn_list->value.present = E2AP_E42setupResponseIEs__value_PR_E2nodeConnected_List;

    // Global E2 Node ID. Mandatory
    E2AP_E2nodeConnected_ItemIEs_t* conn_item = calloc(1, sizeof(E2AP_E2nodeConnected_ItemIEs_t));
    assert(conn_item != NULL && "memory exhausted");
    conn_item->id = E2AP_ProtocolIE_ID_id_GlobalE2node_ID;
    conn_item->criticality = E2AP_Criticality_reject;
    conn_item->value.present = E2AP_E2nodeConnected_ItemIEs__value_PR_GlobalE2node_ID;

    global_e2_node_id_t const* src_id = &sr->nodes[i].id;
    assert(src_id->type == ngran_gNB || src_id->type == ngran_gNB_CU || src_id->type == ngran_gNB_DU || src_id->type == ngran_eNB);
    if (src_id->type != ngran_eNB) {
      E2AP_GlobalE2node_gNB_ID_t* e2gnb = calloc(1, sizeof(E2AP_GlobalE2node_gNB_ID_t));
      assert(e2gnb != NULL && "Memory exhasued");
      e2gnb->global_gNB_ID.gnb_id.present = E2AP_GNB_ID_Choice_PR_gnb_ID;
      if (src_id->type == ngran_gNB_CU) {
        E2AP_GNB_CU_UP_ID_t* e2gnb_cu_up_id = calloc(1, sizeof(E2AP_GNB_CU_UP_ID_t));
        assert(e2gnb_cu_up_id != NULL && "Memory exhasued");
        asn_uint642INTEGER(e2gnb_cu_up_id, *src_id->cu_du_id);
        e2gnb->gNB_CU_UP_ID = e2gnb_cu_up_id;
      }
      else if (src_id->type == ngran_gNB_DU) {
        E2AP_GNB_DU_ID_t* e2gnb_du_id = calloc(1, sizeof(E2AP_GNB_DU_ID_t));
        assert(e2gnb_du_id != NULL && "Memory exhasued");
        asn_uint642INTEGER(e2gnb_du_id, *src_id->cu_du_id);
        e2gnb->gNB_DU_ID = e2gnb_du_id;
      }

      const plmn_t* plmn = &src_id->plmn;
      MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2gnb->global_gNB_ID.plmn_id);
      MACRO_GNB_ID_TO_BIT_STRING(src_id->nb_id, &e2gnb->global_gNB_ID.gnb_id.choice.gnb_ID);

      conn_item->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_gNB;
      conn_item->value.choice.GlobalE2node_ID.choice.gNB = e2gnb;

      rc = ASN_SEQUENCE_ADD(&conn_list->value.choice.E2nodeConnected_List.protocolIEs.list, conn_item);
      assert(rc == 0);
    } else {
      E2AP_GlobalE2node_eNB_ID_t* e2enb = calloc(1, sizeof(E2AP_GlobalE2node_eNB_ID_t));
      assert(e2enb != NULL && "Memory exhasued");
      e2enb->global_eNB_ID.eNB_ID.present = E2AP_ENB_ID_PR_macro_eNB_ID;
      const plmn_t* plmn = &src_id->plmn;
      MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2enb->global_eNB_ID.pLMN_Identity);
      MACRO_ENB_ID_TO_BIT_STRING(src_id->nb_id, &e2enb->global_eNB_ID.eNB_ID.choice.macro_eNB_ID);

      conn_item->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_eNB;
      conn_item->value.choice.GlobalE2node_ID.choice.eNB = e2enb;
      rc = ASN_SEQUENCE_ADD(&conn_list->value.choice.E2nodeConnected_List.protocolIEs.list, conn_item);
      assert(rc == 0);
    }

    // RAN functions
    E2AP_E2nodeConnected_ItemIEs_t* conn_rf = calloc(1, sizeof(E2AP_E2nodeConnected_ItemIEs_t));
    assert(conn_rf != NULL && "memory exhausted");
    conn_rf->id = E2AP_ProtocolIE_ID_id_RANfunctionsAdded; //  E2AP_ProtocolIE_ID_id_RANfunctions_List;// E2AP_ProtocolIE_ID_id_E2nodesConnected;
    conn_rf->criticality = E2AP_Criticality_reject;
    conn_rf->value.present = E2AP_E2nodeConnected_ItemIEs__value_PR_RANfunctions_List;

    for(size_t j = 0; j < sr->nodes[i].len_rf; ++j){
      ran_function_t const* src_rf  = &sr->nodes[i].ack_rf[j];
      E2AP_RANfunction_ItemIEs_t* ran_function_item_ie = calloc(1, sizeof(E2AP_RANfunction_ItemIEs_t));
      ran_function_item_ie->id = E2AP_ProtocolIE_ID_id_RANfunction_Item;
      ran_function_item_ie->criticality = E2AP_Criticality_reject;
      ran_function_item_ie->value.present = E2AP_RANfunction_ItemIEs__value_PR_RANfunction_Item;
      ran_function_item_ie->value.choice.RANfunction_Item = copy_ran_function(src_rf);
      rc = ASN_SEQUENCE_ADD(&conn_rf->value.choice.RANfunctions_List.list, ran_function_item_ie);
      assert(rc == 0);
    }

    rc = ASN_SEQUENCE_ADD(&conn_list->value.choice.E2nodeConnected_List.protocolIEs.list, conn_rf);
    assert(rc == 0);

    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, conn_list);
    assert(rc == 0);
  }

  const E2AP_E42setupRequest_t* out_deb = &pdu->choice.initiatingMessage->value.choice.E42setupRequest;
  assert(out_deb->protocolIEs.list.count > 0 && "Need to have some functionality");
  //printf("From encoding %d \n", out_deb->protocolIEs.list.count );
  //fflush(stdout);

  return pdu;
}



// xApp -> iApp
byte_array_t e2ap_enc_e42_subscription_delete_request_asn(const e42_ric_subscription_delete_request_t* sdr)
{
  assert(sdr != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_e42_subscription_delete_request_asn_pdu(sdr);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_e42_subscription_delete_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == E42_RIC_SUBSCRIPTION_DELETE_REQUEST);
  return e2ap_enc_e42_subscription_delete_request_asn(&msg->u_msgs.e42_ric_sub_del_req);

}

struct E2AP_E2AP_PDU* e2ap_enc_e42_subscription_delete_request_asn_pdu(const e42_ric_subscription_delete_request_t* e42_sdr)
{
  assert(e42_sdr != NULL);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E42RICsubscriptionDelete;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E42RICsubscriptionDeleteRequest;

  E2AP_E42RICsubscriptionDeleteRequest_t* out = &pdu->choice.initiatingMessage->value.choice.E42RICsubscriptionDeleteRequest;

  // XAPP ID. Mandatory
  E2AP_E42RICsubscriptionDeleteRequest_IEs_t* xapp_id = calloc(1, sizeof(E2AP_E42RICsubscriptionDeleteRequest_IEs_t));
  xapp_id->id = E2AP_ProtocolIE_ID_id_XAPP_ID;
  xapp_id->criticality = E2AP_Criticality_reject;
  xapp_id->value.present = E2AP_E42RICsubscriptionDeleteRequest_IEs__value_PR_XAPP_ID;
  xapp_id->value.choice.XAPP_ID = e42_sdr->xapp_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, xapp_id);
  assert(rc == 0);


  ric_subscription_delete_request_t const* const sdr = &e42_sdr->sdr;
  // RIC Request ID. Mandatory
  E2AP_E42RICsubscriptionDeleteRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_E42RICsubscriptionDeleteRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_E42RICsubscriptionDeleteRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = sdr->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = sdr->ric_id.ric_req_id;
 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RIC RAN Function ID. Mandatory
  E2AP_E42RICsubscriptionDeleteRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_E42RICsubscriptionDeleteRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_E42RICsubscriptionDeleteRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = sdr->ric_id.ran_func_id;
 
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);
  return pdu;
}


// xApp -> iApp
byte_array_t e2ap_enc_e42_control_request_asn(const e42_ric_control_request_t* ric_req)
{
  assert(ric_req != NULL);
  E2AP_E2AP_PDU_t* pdu = e2ap_enc_e42_control_request_asn_pdu(ric_req);
  const byte_array_t ba = e2ap_enc_asn_pdu_ba(pdu);
  free_pdu(pdu);
  return ba;
}

byte_array_t e2ap_enc_e42_control_request_asn_msg(const e2ap_msg_t* msg)
{
  assert(msg != NULL);
  assert(msg->type == E42_RIC_CONTROL_REQUEST);
  return  e2ap_enc_e42_control_request_asn(&msg->u_msgs.e42_ric_ctrl_req);
}

E2AP_E2AP_PDU_t* e2ap_enc_e42_control_request_asn_pdu(const e42_ric_control_request_t* e42_ric_req)
{
  assert(e42_ric_req != NULL);

  ric_control_request_t const* ric_req = &e42_ric_req->ctrl_req;
  assert(ric_req != NULL);
  assert(ric_req->hdr.buf != NULL && ric_req->hdr.len > 0);
  assert(ric_req->msg.buf != NULL && ric_req->msg.len > 0);

  // Message Type. Mandatory
  E2AP_E2AP_PDU_t* pdu = calloc(1, sizeof(E2AP_E2AP_PDU_t));
  pdu->present = E2AP_E2AP_PDU_PR_initiatingMessage;
  pdu->choice.initiatingMessage = calloc(1, sizeof(E2AP_InitiatingMessage_t));
  pdu->choice.initiatingMessage->procedureCode = E2AP_ProcedureCode_id_E42RICcontrol;
  pdu->choice.initiatingMessage->criticality = E2AP_Criticality_reject;
  pdu->choice.initiatingMessage->value.present = E2AP_InitiatingMessage__value_PR_E42RICcontrolRequest;

  E2AP_E42RICcontrolRequest_t* out = &pdu->choice.initiatingMessage->value.choice.E42RICcontrolRequest;

  // XAPP ID. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* xapp_id = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  xapp_id->id = E2AP_ProtocolIE_ID_id_XAPP_ID;
  xapp_id->criticality = E2AP_Criticality_reject;
  xapp_id->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_XAPP_ID;
  xapp_id->value.choice.XAPP_ID = e42_ric_req->xapp_id;
  int rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, xapp_id);
  assert(rc == 0);

  // Global E2 Node ID. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* setup_rid = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  setup_rid->id = E2AP_ProtocolIE_ID_id_GlobalE2node_ID;
  setup_rid->criticality = E2AP_Criticality_reject;
  setup_rid->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_GlobalE2node_ID;
  assert(e42_ric_req->id.type == ngran_gNB || e42_ric_req->id.type == ngran_gNB_CU || e42_ric_req->id.type == ngran_gNB_DU || e42_ric_req->id.type == ngran_eNB);
  if (e42_ric_req->id.type != ngran_eNB) {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_gNB;
    E2AP_GlobalE2node_gNB_ID_t* e2gnb = calloc(1, sizeof(E2AP_GlobalE2node_gNB_ID_t));
    assert(e2gnb != NULL && "Memory exhasued");
    e2gnb->global_gNB_ID.gnb_id.present = E2AP_GNB_ID_Choice_PR_gnb_ID;
    if (e42_ric_req->id.type == ngran_gNB_CU) {
      E2AP_GNB_CU_UP_ID_t* e2gnb_cu_up_id = calloc(1, sizeof(E2AP_GNB_CU_UP_ID_t));
      assert(e2gnb_cu_up_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_cu_up_id, *e42_ric_req->id.cu_du_id);
      e2gnb->gNB_CU_UP_ID = e2gnb_cu_up_id;
    }
    else if (e42_ric_req->id.type == ngran_gNB_DU) {
      E2AP_GNB_DU_ID_t* e2gnb_du_id = calloc(1, sizeof(E2AP_GNB_DU_ID_t));
      assert(e2gnb_du_id != NULL && "Memory exhasued");
      asn_uint642INTEGER(e2gnb_du_id, *e42_ric_req->id.cu_du_id);
      e2gnb->gNB_DU_ID = e2gnb_du_id;
    }

    const plmn_t* plmn = &e42_ric_req->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2gnb->global_gNB_ID.plmn_id);
    MACRO_GNB_ID_TO_BIT_STRING(e42_ric_req->id.nb_id, &e2gnb->global_gNB_ID.gnb_id.choice.gnb_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.gNB = e2gnb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  } else {
    setup_rid->value.choice.GlobalE2node_ID.present = E2AP_GlobalE2node_ID_PR_eNB;
    E2AP_GlobalE2node_eNB_ID_t* e2enb = calloc(1, sizeof(E2AP_GlobalE2node_eNB_ID_t));
    assert(e2enb != NULL && "Memory exhasued");
    e2enb->global_eNB_ID.eNB_ID.present = E2AP_ENB_ID_PR_macro_eNB_ID;
    const plmn_t* plmn = &e42_ric_req->id.plmn;
    MCC_MNC_TO_PLMNID(plmn->mcc, plmn->mnc, plmn->mnc_digit_len, &e2enb->global_eNB_ID.pLMN_Identity);
    MACRO_ENB_ID_TO_BIT_STRING(e42_ric_req->id.nb_id, &e2enb->global_eNB_ID.eNB_ID.choice.macro_eNB_ID);
    setup_rid->value.choice.GlobalE2node_ID.choice.eNB = e2enb;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, setup_rid);
    assert(rc == 0);
  }



  // RIC request id. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* sub_req = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  sub_req->id = E2AP_ProtocolIE_ID_id_RICrequestID;
  sub_req->criticality = E2AP_Criticality_reject;
  sub_req->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RICrequestID;
  sub_req->value.choice.RICrequestID.ricInstanceID = ric_req->ric_id.ric_inst_id;
  sub_req->value.choice.RICrequestID.ricRequestorID = ric_req->ric_id.ric_req_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, sub_req);
  assert(rc == 0);

  // RIC RAN function ID. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* ran_id = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  ran_id->id = E2AP_ProtocolIE_ID_id_RANfunctionID;
  ran_id->criticality = E2AP_Criticality_reject;
  ran_id->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RANfunctionID;
  ran_id->value.choice.RANfunctionID = ric_req->ric_id.ran_func_id;
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ran_id);
  assert(rc == 0);

  // RIC Call Process ID. Optional 
  if (ric_req->call_process_id != NULL) {
    assert(ric_req->call_process_id->buf != NULL && ric_req->call_process_id->len > 0);
    E2AP_E42RICcontrolRequest_IEs_t* ric_proc = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
    ric_proc->id = E2AP_ProtocolIE_ID_id_RICcallProcessID;
    ric_proc->criticality = E2AP_Criticality_reject;
    ric_proc->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RICcallProcessID;
    ric_proc->value.choice.RICcallProcessID = copy_ba_to_ostring(*ric_req->call_process_id);
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_proc);
    assert(rc == 0);
  }

  // RIC Control Header. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* ric_header = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  ric_header->id = E2AP_ProtocolIE_ID_id_RICcontrolHeader;
  ric_header->criticality = E2AP_Criticality_reject;
  ric_header->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RICcontrolHeader;
  ric_header->value.choice.RICcontrolHeader = copy_ba_to_ostring(ric_req->hdr);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_header);
  assert(rc == 0);

  // RIC Control Message. Mandatory
  E2AP_E42RICcontrolRequest_IEs_t* ric_msg = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
  ric_msg->id = E2AP_ProtocolIE_ID_id_RICcontrolMessage;
  ric_msg->criticality = E2AP_Criticality_reject;
  ric_msg->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RICcontrolMessage;
  ric_msg->value.choice.RICcontrolMessage = copy_ba_to_ostring(ric_req->msg);
  rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_msg);
  assert(rc == 0);

  // RIC Control ACK Request. Optional
  if(ric_req->ack_req != NULL){
    E2AP_E42RICcontrolRequest_IEs_t* ric_ack = calloc(1, sizeof(E2AP_E42RICcontrolRequest_IEs_t));
    ric_ack->id = E2AP_ProtocolIE_ID_id_RICcontrolAckRequest;
    ric_ack->criticality = E2AP_Criticality_reject;
    ric_ack->value.present = E2AP_E42RICcontrolRequest_IEs__value_PR_RICcontrolAckRequest;
    ric_ack->value.choice.RICcontrolAckRequest = *ric_req->ack_req;
    rc = ASN_SEQUENCE_ADD(&out->protocolIEs.list, ric_ack);
    assert(rc == 0);
  }
  return pdu;
}


