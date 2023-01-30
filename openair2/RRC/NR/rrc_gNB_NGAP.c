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

/*! \file rrc_gNB_NGAP.h
 * \brief rrc NGAP procedures for gNB
 * \author Yoshio INOUE, Masayuki HARADA
 * \date 2020
 * \version 0.1
 * \email: yoshio.inoue@fujitsu.com,masayuki.harada@fujitsu.com
 *         (yoshio.inoue%40fujitsu.com%2cmasayuki.harada%40fujitsu.com) 
 */

#include "rrc_gNB_NGAP.h"
#include "RRC/L2_INTERFACE/openair_rrc_L2_interface.h"
#include "rrc_eNB_S1AP.h"
#include "gnb_config.h"
#include "common/ran_context.h"

#include "oai_asn1.h"
#include "intertask_interface.h"
#include "pdcp.h"
#include "pdcp_primitives.h"

#include "openair3/ocp-gtpu/gtp_itf.h"
#include <openair3/ocp-gtpu/gtp_itf.h>
#include "RRC/LTE/rrc_eNB_GTPV1U.h"
#include "RRC/NR/rrc_gNB_GTPV1U.h"

#include "S1AP_NAS-PDU.h"
#include "executables/softmodem-common.h"
#include "UTIL/OSA/osa_defs.h"
#include "ngap_gNB_defs.h"
#include "ngap_gNB_ue_context.h"
#include "ngap_gNB_management_procedures.h"
#include "NR_ULInformationTransfer.h"
#include "RRC/NR/MESSAGES/asn1_msg.h"
#include "NR_UERadioAccessCapabilityInformation.h"
#include "NR_UE-CapabilityRAT-ContainerList.h"
#include "NGAP_Cause.h"
#include "NGAP_CauseRadioNetwork.h"
#include "f1ap_messages_types.h"
#include "openair2/E1AP/e1ap_asnc.h"

extern RAN_CONTEXT_t RC;

/* Masks for NGAP Encryption algorithms, NEA0 is always supported (not coded) */
static const uint16_t NGAP_ENCRYPTION_NEA1_MASK = 0x8000;
static const uint16_t NGAP_ENCRYPTION_NEA2_MASK = 0x4000;
static const uint16_t NGAP_ENCRYPTION_NEA3_MASK = 0x2000;

/* Masks for NGAP Integrity algorithms, NIA0 is always supported (not coded) */
static const uint16_t NGAP_INTEGRITY_NIA1_MASK = 0x8000;
static const uint16_t NGAP_INTEGRITY_NIA2_MASK = 0x4000;
static const uint16_t NGAP_INTEGRITY_NIA3_MASK = 0x2000;

#define INTEGRITY_ALGORITHM_NONE NR_IntegrityProtAlgorithm_nia0

/*! \fn uint16_t get_next_gNB_ue_ngap_id(uint8_t mod_id)
 *\brief provide a system wide uniq UE id, regardless instance
 *\return the UE initial ID.
 * we simply assume here that 4 biilions UEs can't exist in the system
 */
hash_table_t *uniq_ue_ids;
#define UE_INITIAL_ID_INVALID 0
//------------------------------------------------------------------------------
static uint32_t get_next_gNB_ue_ngap_id()
//------------------------------------------------------------------------------
{
  // should be called only in RRC thread
  // 0 is invalid
  // no collision can occur on 64 bits integer
  // on NGAP, we use only 32 bits,
  // nevertheless it is also uniq accross life time, and even the old UE should have disappear when we get a 4 billions new UE
  static hash_key_t gNB_ue_ngap_id = 1;
  return gNB_ue_ngap_id++;
}

//------------------------------------------------------------------------------
struct rrc_gNB_ue_context_s *rrc_gNB_get_ue_context_from_ngap_ids(instance_t instanceP, const uint32_t gNB_ue_ngap_id)
//------------------------------------------------------------------------------
{
  return rrc_gNB_get_ue_context(RC.nrrrc[GNB_INSTANCE_TO_MODULE_ID(instanceP)], gNB_ue_ngap_id);
}

/*! \fn void process_gNB_security_key (const protocol_ctxt_t* const ctxt_pP, eNB_RRC_UE_t * const ue_context_pP, uint8_t *security_key)
 *\brief save security key.
 *\param ctxt_pP         Running context.
 *\param ue_context_pP   UE context.
 *\param security_key_pP The security key received from NGAP.
 */
//------------------------------------------------------------------------------
void process_gNB_security_key (
  const protocol_ctxt_t *const ctxt_pP,
  rrc_gNB_ue_context_t  *const ue_context_pP,
  uint8_t               *security_key_pP
)
//------------------------------------------------------------------------------
{
  char ascii_buffer[65];
  uint8_t i;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  /* Saves the security key */
  memcpy(UE->kgnb, security_key_pP, SECURITY_KEY_LENGTH);
  memset(UE->nh, 0, SECURITY_KEY_LENGTH);
  UE->nh_ncc = -1;

  for (i = 0; i < 32; i++) {
    sprintf(&ascii_buffer[2 * i], "%02X", UE->kgnb[i]);
  }

  ascii_buffer[2 * i] = '\0';
  LOG_I(NR_RRC, "[gNB %d][UE %x] Saved security key %s\n", ctxt_pP->module_id, UE->rnti, ascii_buffer);
}

//------------------------------------------------------------------------------
void
nr_rrc_pdcp_config_security(
    const protocol_ctxt_t  *const ctxt_pP,
    rrc_gNB_ue_context_t   *const ue_context_pP,
    const uint8_t          enable_ciphering
)
//------------------------------------------------------------------------------
{
  uint8_t                            *kRRCenc = NULL;
  uint8_t                            *kRRCint = NULL;
  uint8_t                            *kUPenc = NULL;
  //uint8_t                            *k_kdf  = NULL;
  static int                          print_keys= 1;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  /* Derive the keys from kgnb */
  if (UE->Srb[1].Active || UE->Srb[2].Active) {
    nr_derive_key_up_enc(UE->ciphering_algorithm, UE->kgnb, &kUPenc);
  }

  nr_derive_key_rrc_enc(UE->ciphering_algorithm, UE->kgnb, &kRRCenc);
  nr_derive_key_rrc_int(UE->integrity_algorithm, UE->kgnb, &kRRCint);
  if (!IS_SOFTMODEM_IQPLAYER) {
    SET_LOG_DUMP(DEBUG_SECURITY) ;
  }

  if ( LOG_DUMPFLAG( DEBUG_SECURITY ) ) {
    if (print_keys == 1 ) {
      print_keys =0;
      LOG_DUMPMSG(NR_RRC, DEBUG_SECURITY, UE->kgnb, 32, "\nKgNB:");
      LOG_DUMPMSG(NR_RRC, DEBUG_SECURITY, kRRCenc, 16,"\nKRRCenc:" );
      LOG_DUMPMSG(NR_RRC, DEBUG_SECURITY, kRRCint, 16,"\nKRRCint:" );
    }
  }

  pdcp_config_set_security(ctxt_pP,
                           NULL, /* pdcp_pP not used anymore in NR */
                           DCCH,
                           DCCH + 2,
                           enable_ciphering ? UE->ciphering_algorithm | (UE->integrity_algorithm << 4) : 0 | (UE->integrity_algorithm << 4),
                           kRRCenc,
                           kRRCint,
                           kUPenc);
}

//------------------------------------------------------------------------------
/*
* Initial UE NAS message on S1AP.
*/
void
rrc_gNB_send_NGAP_NAS_FIRST_REQ(
    const protocol_ctxt_t     *const ctxt_pP,
    rrc_gNB_ue_context_t      *ue_context_pP,
    NR_RRCSetupComplete_IEs_t *rrcSetupComplete
)
//------------------------------------------------------------------------------
{
  // gNB_RRC_INST *rrc = RC.nrrrc[ctxt_pP->module_id];
  MessageDef *message_p = NULL;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  message_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_NAS_FIRST_REQ);
  ngap_nas_first_req_t *req = &NGAP_NAS_FIRST_REQ(message_p);
  memset(req, 0, sizeof(*req));

  UE->gNB_ue_ngap_id = get_next_gNB_ue_ngap_id();
  req->gNB_ue_ngap_id = UE->gNB_ue_ngap_id;

  /* Assume that cause is coded in the same way in RRC and NGap, just check that the value is in NGap range */
  AssertFatal(UE->establishment_cause < NGAP_RRC_CAUSE_LAST, "Establishment cause invalid (%jd/%d) for gNB %d!", UE->establishment_cause, NGAP_RRC_CAUSE_LAST, ctxt_pP->module_id);
  req->establishment_cause = UE->establishment_cause;

  /* Forward NAS message */
  req->nas_pdu.buffer = rrcSetupComplete->dedicatedNAS_Message.buf;
  req->nas_pdu.length = rrcSetupComplete->dedicatedNAS_Message.size;
  // extract_imsi(NGAP_NAS_FIRST_REQ (message_p).nas_pdu.buffer,
  //              NGAP_NAS_FIRST_REQ (message_p).nas_pdu.length,
  //              ue_context_pP);

  /* Fill UE identities with available information */
  req->ue_identity.presenceMask = NGAP_UE_IDENTITIES_NONE;
  /* Fill s-TMSI */
  req->ue_identity.s_tmsi.amf_set_id = UE->Initialue_identity_5g_s_TMSI.amf_set_id;
  req->ue_identity.s_tmsi.amf_pointer = UE->Initialue_identity_5g_s_TMSI.amf_pointer;
  req->ue_identity.s_tmsi.m_tmsi = UE->Initialue_identity_5g_s_TMSI.fiveg_tmsi;

  /* selected_plmn_identity: IE is 1-based, convert to 0-based (C array) */
  int selected_plmn_identity = rrcSetupComplete->selectedPLMN_Identity - 1;
  req->selected_plmn_identity = selected_plmn_identity;

  if (rrcSetupComplete->registeredAMF != NULL) {
      NR_RegisteredAMF_t *r_amf = rrcSetupComplete->registeredAMF;
      req->ue_identity.presenceMask |= NGAP_UE_IDENTITIES_guami;

      if (r_amf->plmn_Identity != NULL) {
          if ((r_amf->plmn_Identity->mcc != NULL) && (r_amf->plmn_Identity->mcc->list.count > 0)) {
              /* Use first indicated PLMN MCC if it is defined */
              req->ue_identity.guami.mcc = *r_amf->plmn_Identity->mcc->list.array[selected_plmn_identity];
              LOG_I(NGAP, "[gNB %d] Build NGAP_NAS_FIRST_REQ adding in s_TMSI: GUMMEI MCC %u ue %x\n", ctxt_pP->module_id, req->ue_identity.guami.mcc, UE->rnti);
          }

          if (r_amf->plmn_Identity->mnc.list.count > 0) {
              /* Use first indicated PLMN MNC if it is defined */
              req->ue_identity.guami.mnc = *r_amf->plmn_Identity->mnc.list.array[selected_plmn_identity];
              LOG_I(NGAP, "[gNB %d] Build NGAP_NAS_FIRST_REQ adding in s_TMSI: GUMMEI MNC %u ue %x\n", ctxt_pP->module_id, req->ue_identity.guami.mnc, UE->rnti);
          }
      } else {
          /* TODO */
      }

      /* amf_Identifier */
      uint32_t amf_Id = BIT_STRING_to_uint32(&r_amf->amf_Identifier);
      req->ue_identity.guami.amf_region_id = amf_Id >> 16;
      req->ue_identity.guami.amf_set_id = UE->Initialue_identity_5g_s_TMSI.amf_set_id;
      req->ue_identity.guami.amf_pointer = UE->Initialue_identity_5g_s_TMSI.amf_pointer;

      // fixme: illogical place to set UE values, should be in the function that call this one
      UE->ue_guami.mcc = req->ue_identity.guami.mcc;
      UE->ue_guami.mnc = req->ue_identity.guami.mnc;
      UE->ue_guami.mnc_len = req->ue_identity.guami.mnc_len;
      UE->ue_guami.amf_region_id = req->ue_identity.guami.amf_region_id;
      UE->ue_guami.amf_set_id = req->ue_identity.guami.amf_set_id;
      UE->ue_guami.amf_pointer = req->ue_identity.guami.amf_pointer;

      LOG_I(NGAP,
            "[gNB %d] Build NGAP_NAS_FIRST_REQ adding in s_TMSI: GUAMI amf_set_id %u amf_region_id %u ue %x\n",
            ctxt_pP->module_id,
            req->ue_identity.guami.amf_set_id,
            req->ue_identity.guami.amf_region_id,
            UE->rnti);
  }

  itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, message_p);
}

//------------------------------------------------------------------------------
int rrc_gNB_process_NGAP_INITIAL_CONTEXT_SETUP_REQ(MessageDef *msg_p, instance_t instance)
//------------------------------------------------------------------------------
{
  protocol_ctxt_t ctxt = {0};
  ngap_initial_context_setup_req_t *req = &NGAP_INITIAL_CONTEXT_SETUP_REQ(msg_p);

  rrc_gNB_ue_context_t *ue_context_p = rrc_gNB_get_ue_context_from_ngap_ids(instance, req->gNB_ue_ngap_id);
  gNB_RRC_UE_t *UE = &ue_context_p->ue_context;

  if (ue_context_p == NULL) {
    /* Can not associate this message to an UE index, send a failure to NGAP and discard it! */
    MessageDef *msg_fail_p = NULL;
    LOG_W(NR_RRC, "[gNB %ld] In NGAP_INITIAL_CONTEXT_SETUP_REQ: unknown UE from NGAP ids (%u)\n", instance, req->gNB_ue_ngap_id);
    msg_fail_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_INITIAL_CONTEXT_SETUP_FAIL);
    NGAP_INITIAL_CONTEXT_SETUP_FAIL(msg_fail_p).gNB_ue_ngap_id = req->gNB_ue_ngap_id;
    // TODO add failure cause when defined!
    itti_send_msg_to_task(TASK_NGAP, instance, msg_fail_p);
    return (-1);
  }
  PROTOCOL_CTXT_SET_BY_INSTANCE(&ctxt, instance, GNB_FLAG_YES, UE->rnti, 0, 0);
  UE->amf_ue_ngap_id = req->amf_ue_ngap_id;
  uint8_t nb_pdusessions_tosetup = req->nb_of_pdusessions;
  if (nb_pdusessions_tosetup != 0) {
    AssertFatal(false, "PDU sessions in Initial context setup request not handled by E1 yet\n");
    /*
    gtpv1u_gnb_create_tunnel_req_t create_tunnel_req = {0};
    for (int i = 0; i < NR_NB_RB_MAX - 3; i++) {
      // if(UE->pduSession[i].statusE1 >= PDU_SESSION_DONE)
      continue;
      // UE->pduSession[i].E1status      = PDU_SESSION_SENT;
      UE->pduSession[i].param = req->pdusession_param[pdu_sessions_done];
      create_tunnel_req.pdusession_id[pdu_sessions_done] = req->pdusession_param[pdu_sessions_done].pdusession_id;
      create_tunnel_req.incoming_rb_id[pdu_sessions_done] = i + 1;
      create_tunnel_req.outgoing_teid[pdu_sessions_done] = req->pdusession_param[pdu_sessions_done].gtp_teid;
      // To be developped: hardcoded first flow
      create_tunnel_req.outgoing_qfi[pdu_sessions_done] = req->pdusession_param[pdu_sessions_done].qos[0].qfi;
      create_tunnel_req.dst_addr[pdu_sessions_done].length = req->pdusession_param[pdu_sessions_done].upf_addr.length;
      memcpy(create_tunnel_req.dst_addr[pdu_sessions_done].buffer, req->pdusession_param[pdu_sessions_done].upf_addr.buffer, sizeof(uint8_t) * 20);
      LOG_I(NR_RRC, "PDUSESSION SETUP: local index %d teid %u, pdusession id %d \n", i, create_tunnel_req.outgoing_teid[pdu_sessions_done], create_tunnel_req.pdusession_id[pdu_sessions_done]);
      pdu_sessions_done++;

      if (pdu_sessions_done >= nb_pdusessions_tosetup) {
        break;
      }
    }

    UE->nbPduSessions = req->nb_of_pdusessions;
    create_tunnel_req.ue_id = UE->rnti;
    create_tunnel_req.num_tunnels = pdu_sessions_done;

    ret = gtpv1u_create_ngu_tunnel(instance, &create_tunnel_req, &create_tunnel_resp);
    if (ret != 0) {
      LOG_E(NR_RRC, "rrc_gNB_process_NGAP_INITIAL_CONTEXT_SETUP_REQ : gtpv1u_create_ngu_tunnel failed,start to release UE %x\n", UE->rnti);
      UE->ue_release_timer_ng = 1;
      UE->ue_release_timer_thres_ng = 100;
      UE->ue_release_timer = 0;
      UE->ue_reestablishment_timer = 0;
      UE->ul_failure_timer = 20000;
      UE->ul_failure_timer = 0;
      return (0);
    }

    nr_rrc_gNB_process_GTPV1U_CREATE_TUNNEL_RESP(&ctxt, &create_tunnel_resp);
    UE->nbPduSessions += req->nb_of_pdusessions;
    UE->established_pdu_sessions_flag = 1;
    */
  }

  /* NAS PDU */
  // this is malloced pointers, we pass it for later free()
  UE->nas_pdu = req->nas_pdu;

  /* security */
  rrc_gNB_process_security(&ctxt, ue_context_p, &req->security_capabilities);
  process_gNB_security_key(&ctxt, ue_context_p, req->security_key);

  /* configure only integrity, ciphering comes after receiving SecurityModeComplete */
  nr_rrc_pdcp_config_security(&ctxt, ue_context_p, 0);

  rrc_gNB_generate_SecurityModeCommand(&ctxt, ue_context_p);

  // in case, send the S1SP initial context response if it is not sent with the attach complete message
  if (UE->StatusRrc == NR_RRC_RECONFIGURED) {
    LOG_I(NR_RRC, "Sending rrc_gNB_send_NGAP_INITIAL_CONTEXT_SETUP_RESP, cause %ld\n", UE->reestablishment_cause);
    rrc_gNB_send_NGAP_INITIAL_CONTEXT_SETUP_RESP(&ctxt, ue_context_p);
  }
  return 0;
}

//------------------------------------------------------------------------------
void
rrc_gNB_send_NGAP_INITIAL_CONTEXT_SETUP_RESP(
  const protocol_ctxt_t *const ctxt_pP,
  rrc_gNB_ue_context_t          *const ue_context_pP
)
//------------------------------------------------------------------------------
{
  MessageDef *msg_p = NULL;
  int pdu_sessions_done = 0;
  int pdu_sessions_failed = 0;
  msg_p = itti_alloc_new_message (TASK_RRC_ENB, 0, NGAP_INITIAL_CONTEXT_SETUP_RESP);
  ngap_initial_context_setup_resp_t *resp = &NGAP_INITIAL_CONTEXT_SETUP_RESP(msg_p);
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  resp->gNB_ue_ngap_id = UE->gNB_ue_ngap_id;

  for (int pdusession = 0; pdusession < UE->nbPduSessions; pdusession++) {
    pdu_session_param_t *session = &UE->pduSession[pdusession];
    if (session->statusE1 == PDU_SESSION_existing && session->statusF1 == PDU_SESSION_existing) {
      pdu_sessions_done++;
      resp->pdusessions[pdusession].pdusession_id = session->param.pdusession_id;
      // TODO add other information from S1-U when it will be integrated
      resp->pdusessions[pdusession].gtp_teid = UE->gnb_gtp_teid[pdusession];
      memcpy(resp->pdusessions[pdusession].gNB_addr.buffer, UE->gnb_gtp_addrs[pdusession].buffer, 20);
      resp->pdusessions[pdusession].gNB_addr.length = 4;
      /* FIXXX
      resp->pdusessions[pdusession].nb_of_qos_flow = session->param.nb_qos;
      for (qos_flow_index = 0; qos_flow_index < session->param.nb_qos; qos_flow_index++) {
        resp->pdusessions[pdusession].associated_qos_flows[qos_flow_index].qfi = session->param.qos[qos_flow_index].qfi;
        resp->pdusessions[pdusession].associated_qos_flows[qos_flow_index].qos_flow_mapping_ind = QOSFLOW_MAPPING_INDICATION_DL;
      }
      */
      session->statusNGAP = PDU_SESSION_existing;
    } else {
      pdu_sessions_failed++;
      resp->pdusessions_failed[pdusession].pdusession_id = session->param.pdusession_id;
      // TODO add cause when it will be integrated
      resp->pdusessions_failed[pdusession].cause = NGAP_Cause_PR_radioNetwork;
      resp->pdusessions_failed[pdusession].cause_value = NGAP_CauseRadioNetwork_unknown_PDU_session_ID;
      session->statusNGAP = PDU_SESSION_notExisting;
    }
  }

  resp->nb_of_pdusessions = pdu_sessions_done;
  resp->nb_of_pdusessions_failed = pdu_sessions_failed;
  itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);
}

static NR_CipheringAlgorithm_t rrc_gNB_select_ciphering(
    const protocol_ctxt_t *const ctxt_pP,
    uint16_t algorithms)
{
  gNB_RRC_INST *rrc = RC.nrrrc[ctxt_pP->module_id];
  int i;
  /* preset nea0 as fallback */
  int ret = 0;

  /* Select ciphering algorithm based on gNB configuration file and
   * UE's supported algorithms.
   * We take the first from the list that is supported by the UE.
   * The ordering of the list comes from the configuration file.
   */
  for (i = 0; i < rrc->security.ciphering_algorithms_count; i++) {
    int nea_mask[4] = {
      0,
      NGAP_ENCRYPTION_NEA1_MASK,
      NGAP_ENCRYPTION_NEA2_MASK,
      NGAP_ENCRYPTION_NEA3_MASK
    };
    if (rrc->security.ciphering_algorithms[i] == 0) {
      /* nea0 */
      break;
    }
    if (algorithms & nea_mask[rrc->security.ciphering_algorithms[i]]) {
      ret = rrc->security.ciphering_algorithms[i];
      break;
    }
  }

  LOG_I(RRC, "selecting ciphering algorithm %d\n", ret);

  return ret;
}

static e_NR_IntegrityProtAlgorithm rrc_gNB_select_integrity(
    const protocol_ctxt_t *const ctxt_pP,
    uint16_t algorithms)
{
  gNB_RRC_INST *rrc = RC.nrrrc[ctxt_pP->module_id];
  int i;
  /* preset nia0 as fallback */
  int ret = 0;

  /* Select integrity algorithm based on gNB configuration file and
   * UE's supported algorithms.
   * We take the first from the list that is supported by the UE.
   * The ordering of the list comes from the configuration file.
   */
  for (i = 0; i < rrc->security.integrity_algorithms_count; i++) {
    int nia_mask[4] = {
      0,
      NGAP_INTEGRITY_NIA1_MASK,
      NGAP_INTEGRITY_NIA2_MASK,
      NGAP_INTEGRITY_NIA3_MASK
    };
    if (rrc->security.integrity_algorithms[i] == 0) {
      /* nia0 */
      break;
    }
    if (algorithms & nia_mask[rrc->security.integrity_algorithms[i]]) {
      ret = rrc->security.integrity_algorithms[i];
      break;
    }
  }

  LOG_I(RRC, "selecting integrity algorithm %d\n", ret);

  return ret;
}

int
rrc_gNB_process_security(
  const protocol_ctxt_t *const ctxt_pP,
  rrc_gNB_ue_context_t *const ue_context_pP,
  ngap_security_capabilities_t *security_capabilities_pP
) {
  bool                                                  changed = false;
  NR_CipheringAlgorithm_t                               cipheringAlgorithm;
  e_NR_IntegrityProtAlgorithm                           integrityProtAlgorithm;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  /* Save security parameters */
  UE->security_capabilities = *security_capabilities_pP;
  // translation
  LOG_D(NR_RRC,
        "[gNB %d] NAS security_capabilities.encryption_algorithms %u AS ciphering_algorithm %lu NAS security_capabilities.integrity_algorithms %u AS integrity_algorithm %u\n",
        ctxt_pP->module_id,
        UE->security_capabilities.nRencryption_algorithms,
        (unsigned long)UE->ciphering_algorithm,
        UE->security_capabilities.nRintegrity_algorithms,
        UE->integrity_algorithm);
  /* Select relevant algorithms */
  cipheringAlgorithm = rrc_gNB_select_ciphering(ctxt_pP, UE->security_capabilities.nRencryption_algorithms);

  if (UE->ciphering_algorithm != cipheringAlgorithm) {
    UE->ciphering_algorithm = cipheringAlgorithm;
    changed = true;
  }

  integrityProtAlgorithm = rrc_gNB_select_integrity(ctxt_pP, UE->security_capabilities.nRintegrity_algorithms);

  if (UE->integrity_algorithm != integrityProtAlgorithm) {
    UE->integrity_algorithm = integrityProtAlgorithm;
    changed = true;
  }

  LOG_I(NR_RRC,
        "[gNB %d][UE %x] Selected security algorithms (%p): %lx, %x, %s\n",
        ctxt_pP->module_id,
        UE->rnti,
        security_capabilities_pP,
        (unsigned long)cipheringAlgorithm,
        integrityProtAlgorithm,
        changed ? "changed" : "same");
  return changed;
}

//------------------------------------------------------------------------------
int rrc_gNB_process_NGAP_DOWNLINK_NAS(MessageDef *msg_p, instance_t instance, mui_t *rrc_gNB_mui)
//------------------------------------------------------------------------------
{
  uint32_t length;
  uint8_t *buffer;
  struct rrc_gNB_ue_context_s *ue_context_p = NULL;
  protocol_ctxt_t ctxt = {0};
  ngap_downlink_nas_t *req = &NGAP_DOWNLINK_NAS(msg_p);
  ue_context_p = rrc_gNB_get_ue_context_from_ngap_ids(instance, req->gNB_ue_ngap_id);
  gNB_RRC_UE_t *UE = &ue_context_p->ue_context;

  if (ue_context_p == NULL) {
    /* Can not associate this message to an UE index, send a failure to NGAP and discard it! */
    MessageDef *msg_fail_p;
    LOG_W(NR_RRC, "[gNB %ld] In NGAP_DOWNLINK_NAS: unknown UE from NGAP ids (%u)\n", instance, req->gNB_ue_ngap_id);
    msg_fail_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_NAS_NON_DELIVERY_IND);
    NGAP_NAS_NON_DELIVERY_IND(msg_fail_p).gNB_ue_ngap_id = req->gNB_ue_ngap_id;
    NGAP_NAS_NON_DELIVERY_IND(msg_fail_p).nas_pdu.length = req->nas_pdu.length;
    NGAP_NAS_NON_DELIVERY_IND(msg_fail_p).nas_pdu.buffer = req->nas_pdu.buffer;
    // TODO add failure cause when defined!
    itti_send_msg_to_task(TASK_NGAP, instance, msg_fail_p);
    return (-1);
  } else {
    PROTOCOL_CTXT_SET_BY_INSTANCE(&ctxt, instance, GNB_FLAG_YES, UE->rnti, 0, 0);

    /* Is it the first income from NGAP ? */
    if (UE->gNB_ue_ngap_id == 0) {
      UE->gNB_ue_ngap_id = NGAP_DOWNLINK_NAS(msg_p).gNB_ue_ngap_id;
    }

    /* Create message for PDCP (DLInformationTransfer_t) */
    length = do_NR_DLInformationTransfer(instance, &buffer, rrc_gNB_get_next_transaction_identifier(instance), req->nas_pdu.length, req->nas_pdu.buffer);
    LOG_DUMPMSG(NR_RRC, DEBUG_RRC, buffer, length, "[MSG] RRC DL Information Transfer\n");
    /*
     * switch UL or DL NAS message without RRC piggybacked to SRB2 if active.
     */
    switch (RC.nrrrc[ctxt.module_id]->node_type) {
      case ngran_gNB_CU:
      case ngran_gNB_CUCP:
      case ngran_gNB: {
        long srb_id;
        if (UE->Srb[2].Active)
          srb_id = UE->Srb[2].Srb_info.Srb_id;
        else
          srb_id = UE->Srb[1].Srb_info.Srb_id;
        AssertFatal(srb_id > 0 && srb_id < MAX_SRBs, "");
        /* Transfer data to PDCP */
        nr_rrc_data_req(&ctxt, srb_id, (*rrc_gNB_mui)++, SDU_CONFIRM_NO, length, buffer, PDCP_TRANSMISSION_MODE_CONTROL);
      } break;

      case ngran_gNB_DU:
        // nothing to do for DU
        AssertFatal(1 == 0, "nothing to do for DU\n");
        break;

      default:
        LOG_W(NR_RRC, "Unknown node type %d\n", RC.nrrrc[ctxt.module_id]->node_type);
    }
    return (0);
  }
}

//------------------------------------------------------------------------------
void
rrc_gNB_send_NGAP_UPLINK_NAS(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  NR_UL_DCCH_Message_t     *const ul_dcch_msg
)
//------------------------------------------------------------------------------
{
    uint32_t pdu_length;
    uint8_t *pdu_buffer;
    MessageDef *msg_p;
    NR_ULInformationTransfer_t *ulInformationTransfer = ul_dcch_msg->message.choice.c1->choice.ulInformationTransfer;
    gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

    if (ulInformationTransfer->criticalExtensions.present == NR_ULInformationTransfer__criticalExtensions_PR_ulInformationTransfer) {
        pdu_length = ulInformationTransfer->criticalExtensions.choice.ulInformationTransfer->dedicatedNAS_Message->size;
        pdu_buffer = ulInformationTransfer->criticalExtensions.choice.ulInformationTransfer->dedicatedNAS_Message->buf;
        msg_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_UPLINK_NAS);
        NGAP_UPLINK_NAS(msg_p).gNB_ue_ngap_id = UE->gNB_ue_ngap_id;
        NGAP_UPLINK_NAS (msg_p).nas_pdu.length = pdu_length;
        NGAP_UPLINK_NAS (msg_p).nas_pdu.buffer = pdu_buffer;
        // extract_imsi(NGAP_UPLINK_NAS (msg_p).nas_pdu.buffer,
        //               NGAP_UPLINK_NAS (msg_p).nas_pdu.length,
        //               ue_context_pP);
        itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);
        LOG_I(NR_RRC,"Send RRC GNB UL Information Transfer \n");
    }
}

//------------------------------------------------------------------------------
void
rrc_gNB_send_NGAP_PDUSESSION_SETUP_RESP(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
)
//------------------------------------------------------------------------------
{
  MessageDef *msg_p;
  int pdu_sessions_done = 0;
  int pdu_sessions_failed = 0;

  msg_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_PDUSESSION_SETUP_RESP);
  ngap_pdusession_setup_resp_t *resp = &NGAP_PDUSESSION_SETUP_RESP(msg_p);
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;
  resp->gNB_ue_ngap_id = UE->gNB_ue_ngap_id;

  for (int pdusession = 0; pdusession < UE->nbPduSessions; pdusession++) {
    pdu_session_param_t *session = &UE->pduSession[pdusession];
    if (session->statusE1 == PDU_SESSION_existing && session->statusF1 == PDU_SESSION_existing) {
      pdusession_setup_t *tmp = &resp->pdusessions[pdu_sessions_done];
      tmp->pdusession_id = session->param.pdusession_id;
      /* FIXXX
      tmp->pdusession_id = 1;
      tmp->nb_of_qos_flow = session->param.nb_qos;
      tmp->gtp_teid = UE->gnb_gtp_teid[pdusession];
      tmp->gNB_addr.pdu_session_type = PDUSessionType_ipv4;
      tmp->gNB_addr.length = UE->gnb_gtp_addrs[pdusession].length;
      memcpy(tmp->gNB_addr.buffer, UE->gnb_gtp_addrs[pdusession].buffer, tmp->gNB_addr.length);
      for (qos_flow_index = 0; qos_flow_index < tmp->nb_of_qos_flow; qos_flow_index++) {
        tmp->associated_qos_flows[qos_flow_index].qfi = session->param.qos[qos_flow_index].qfi;
        tmp->associated_qos_flows[qos_flow_index].qos_flow_mapping_ind = QOSFLOW_MAPPING_INDICATION_DL;
      }
      */
      session->statusNGAP = PDU_SESSION_existing;
      LOG_I(NR_RRC,
            "msg index %d, pdu_sessions index %d, status %d, xid %d): nb_of_pdusessions %d,  pdusession_id %d, teid: %u \n ",
            pdu_sessions_done,
            pdusession,
            session->statusNGAP,
            xid,
            UE->nbPduSessions,
            tmp->pdusession_id,
            tmp->gtp_teid);
      pdu_sessions_done++;
    } else { /* to be improved */
      session->statusNGAP = PDU_SESSION_notExisting;
      resp->pdusessions_failed[pdu_sessions_failed].pdusession_id = session->param.pdusession_id;
      pdu_sessions_failed++;
      // TODO add cause when it will be integrated
    }
    resp->nb_of_pdusessions = pdu_sessions_done;
    resp->nb_of_pdusessions_failed = pdu_sessions_failed;
    // } else {
    //   LOG_D(NR_RRC,"xid does not corresponds  (context pdu_sessions index %d, status %d, xid %d/%d) \n ",
    //         pdusession, UE->pdusession[pdusession].status, xid, UE->pdusession[pdusession].xid);
    // }
  }

  if ((pdu_sessions_done > 0 || pdu_sessions_failed)) {
    LOG_I(NR_RRC, "NGAP_PDUSESSION_SETUP_RESP: sending the message: nb_of_pdusessions %d\n", UE->nbPduSessions);
    itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);
  }

  for(int i = 0; i < NB_RB_MAX; i++) {
    UE->pduSession[i].xid = -1;
  }

  return;
}

//------------------------------------------------------------------------------
void rrc_gNB_process_NGAP_PDUSESSION_SETUP_REQ(MessageDef *msg_p, instance_t instance)
//------------------------------------------------------------------------------
{
  gNB_RRC_INST *rrc;
  rrc_gNB_ue_context_t            *ue_context_p = NULL;
  protocol_ctxt_t                 ctxt={0};

  ngap_pdusession_setup_req_t* msg=&NGAP_PDUSESSION_SETUP_REQ(msg_p);
  ue_context_p = rrc_gNB_get_ue_context_from_ngap_ids(instance, msg->gNB_ue_ngap_id);
  gNB_RRC_UE_t *UE = &ue_context_p->ue_context;
  PROTOCOL_CTXT_SET_BY_MODULE_ID(&ctxt, 0, GNB_FLAG_YES, UE->rnti, 0, 0, 0);
  rrc = RC.nrrrc[ctxt.module_id];
  LOG_I(NR_RRC, "[gNB %ld] gNB_ue_ngap_id %u \n", instance, msg->gNB_ue_ngap_id);

  if (ue_context_p == NULL) {
    MessageDef *msg_fail_p = NULL;
    LOG_W(NR_RRC, "[gNB %ld] In NGAP_PDUSESSION_SETUP_REQ: unknown UE from NGAP ids (%u)\n", instance, msg->gNB_ue_ngap_id);
    msg_fail_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_PDUSESSION_SETUP_REQUEST_FAIL);
    NGAP_PDUSESSION_SETUP_REQ(msg_fail_p).gNB_ue_ngap_id = msg->gNB_ue_ngap_id;
    // TODO add failure cause when defined!
    itti_send_msg_to_task (TASK_NGAP, instance, msg_fail_p);
    return ;
  }

  UE->nbPduSessions = msg->nb_pdusessions_tosetup;
  UE->gNB_ue_ngap_id = msg->gNB_ue_ngap_id;
  UE->amf_ue_ngap_id = msg->amf_ue_ngap_id;

  /* Configurations are referred from rrc_gNB_generate_dedicatedRRCReconfiguration() and
     rrc_gNB_process_RRCReconfigurationComplete()

     At CU-CP we configure the E1 bearer context setup parameters (PDU sessions, DRBs and 
     QoS flows) same as in these functions. At CU-UP we create PDU Sessions and allocate DRBs.
  */
  e1ap_bearer_setup_req_t bearer_req;

  bearer_req.gNB_cu_cp_ue_id = msg->gNB_ue_ngap_id;
  bearer_req.rnti = UE->rnti;
  bearer_req.cipheringAlgorithm = UE->ciphering_algorithm;
  memcpy(bearer_req.encryptionKey, UE->kgnb, sizeof(UE->kgnb));
  bearer_req.integrityProtectionAlgorithm = UE->integrity_algorithm;
  memcpy(bearer_req.integrityProtectionKey, UE->kgnb, sizeof(UE->kgnb));
  bearer_req.ueDlAggMaxBitRate = msg->ueAggMaxBitRateDownlink;

  bearer_req.numPDUSessions = msg->nb_pdusessions_tosetup;

  for (int i=0; i < bearer_req.numPDUSessions; i++) {
    UE->pduSession[i].param = msg->pdusession_setup_params[i];

    pdu_session_to_setup_t *pdu = bearer_req.pduSession + i;
    pdu->sessionId   = msg->pdusession_setup_params[i].pdusession_id;
    // FIXXX
    // pdu->sessionType = msg->pdusession_setup_params[i].upf_addr.pdu_session_type;
    pdu->sst         = msg->allowed_nssai[i].sST;
    if (rrc->security.do_drb_integrity) {
      pdu->integrityProtectionIndication = E1AP_IntegrityProtectionIndication_required;
    } else {
      pdu->integrityProtectionIndication = E1AP_IntegrityProtectionIndication_not_needed;
    }

    if (rrc->security.do_drb_ciphering) {
      pdu->confidentialityProtectionIndication = E1AP_ConfidentialityProtectionIndication_required;
    } else {
      pdu->confidentialityProtectionIndication = E1AP_ConfidentialityProtectionIndication_not_needed;
    }
    /* FIXXX
    pdu->teId = msg->pdusession_setup_params[i].gtp_teid;
    memcpy(&pdu->tlAddress,
           msg->pdusession_setup_params[i].upf_addr.buffer,
           sizeof(uint8_t)*4);
    UE->pduSession[i].param = msg->pdusession_setup_params[i];
    UE->nbPduSessions = msg->nb_pdusessions_tosetup;
    UE->gNB_ue_ngap_id = msg->gNB_ue_ngap_id;
    UE->amf_ue_ngap_id = msg->amf_ue_ngap_id;
    pdu->numDRB2Setup = 1; // One DRB per PDU Session. TODO: Remove hardcoding
    UE->setup_pdu_sessions += pdu->numDRB2Setup;
    */

    for (int j=0; j < pdu->numDRB2Setup; j++) {
      DRB_nGRAN_to_setup_t *drb = pdu->DRBnGRanList + j;

      drb->id = i + j + 1;

      drb->defaultDRB = E1AP_DefaultDRB_true;

      drb->sDAP_Header_UL = !(rrc->configuration.enable_sdap);
      drb->sDAP_Header_DL = !(rrc->configuration.enable_sdap);

      drb->pDCP_SN_Size_UL = E1AP_PDCP_SN_Size_s_18;
      drb->pDCP_SN_Size_DL = E1AP_PDCP_SN_Size_s_18;

      drb->discardTimer = E1AP_DiscardTimer_infinity;
      drb->reorderingTimer = E1AP_T_Reordering_ms0;

      drb->rLC_Mode = E1AP_RLC_Mode_rlc_am;

      drb->numCellGroups = 1; // assume one cell group associated with a DRB

      for (int k=0; k < drb->numCellGroups; k++) {
        cell_group_t *cellGroup = drb->cellGroupList + k;
        cellGroup->id = 0; // MCG
      }
      /* FIXXX

        drb->numQosFlow2Setup = msg->pdusession_setup_params[i].nb_qos;
        for (int k=0; k < drb->numQosFlow2Setup; k++) {
          qos_flow_to_setup_t *qos = drb->qosFlows + k;

          qos->id          = msg->pdusession_setup_params[i].qos[k].qfi;
          qos->fiveQI      = msg->pdusession_setup_params[i].qos[k].fiveQI;
          qos->fiveQI_type = msg->pdusession_setup_params[i].qos[k].fiveQI_type;

          qos->qoSPriorityLevel = msg->pdusession_setup_params[i].qos[k].allocation_retention_priority.priority_level;
          qos->pre_emptionCapability = msg->pdusession_setup_params[i].qos[k].allocation_retention_priority.pre_emp_capability;
          qos->pre_emptionVulnerability = msg->pdusession_setup_params[i].qos[k].allocation_retention_priority.pre_emp_vulnerability;
        }
      */
    }
  }

  rrc->cucp_cuup.bearer_context_setup(&bearer_req, instance);
  return;
}

//------------------------------------------------------------------------------
int rrc_gNB_process_NGAP_PDUSESSION_MODIFY_REQ(MessageDef *msg_p, instance_t instance)
//------------------------------------------------------------------------------
{
  uint8_t                          nb_pdusessions_tomodify;
  rrc_gNB_ue_context_t *ue_context_p = NULL;
  protocol_ctxt_t                  ctxt;
  ngap_pdusession_modify_req_t *req = &NGAP_PDUSESSION_MODIFY_REQ(msg_p);
  nb_pdusessions_tomodify = req->nb_pdusessions_tomodify;

  ue_context_p = rrc_gNB_get_ue_context_from_ngap_ids(instance, req->gNB_ue_ngap_id);
  if (ue_context_p == NULL) {
    LOG_W(NR_RRC, "[gNB %ld] In NGAP_PDUSESSION_MODIFY_REQ: unknown UE from NGAP ids (%u)\n", instance, req->gNB_ue_ngap_id);
    // TODO 
    return (-1);
  } else {
    gNB_RRC_UE_t *UE = &ue_context_p->ue_context;
    PROTOCOL_CTXT_SET_BY_INSTANCE(&ctxt, instance, GNB_FLAG_YES, UE->rnti, 0, 0);
    ctxt.eNB_index = 0;
    // FIXXX
    UE->gNB_ue_ngap_id = req->gNB_ue_ngap_id;
    {
      bool is_treated[NGAP_MAX_PDUSESSION] = {false};
      uint8_t nb_of_failed_pdusessions = 0;

      for (int i = 0; i < nb_pdusessions_tomodify; i++) {
        // pdu_session_param_t *session=&UE->pduSession[i];
        if (is_treated[i] == true) {
          continue;
        }
        
        //Check if same PDU session ID to handle multiple pdu sessions
        for (int j = i + 1; j < nb_pdusessions_tomodify; j++) {
          if (is_treated[j] == false && req->pdusession_modify_params[j].pdusession_id == req->pdusession_modify_params[i].pdusession_id) {
            // handle multiple pdu session id
            LOG_D(NR_RRC, "handle multiple pdu session id \n");
            UE->modify_pdusession[j].status = PDU_SESSION_STATUS_NEW;
            UE->modify_pdusession[j].param.pdusession_id = req->pdusession_modify_params[j].pdusession_id;
            UE->modify_pdusession[j].cause = NGAP_CAUSE_RADIO_NETWORK;
            UE->modify_pdusession[j].cause_value = NGAP_CauseRadioNetwork_multiple_PDU_session_ID_instances;
            nb_of_failed_pdusessions++;
            is_treated[i] = true;
            is_treated[j] = true;
          }
        }
        // handle multiple pdu session id case
        if (is_treated[i] == true) {
          LOG_D(NR_RRC, "handle multiple pdu session id \n");
          UE->modify_pdusession[i].status = PDU_SESSION_STATUS_NEW;
          UE->modify_pdusession[i].param.pdusession_id = req->pdusession_modify_params[i].pdusession_id;
          UE->modify_pdusession[i].cause = NGAP_CAUSE_RADIO_NETWORK;
          UE->modify_pdusession[i].cause_value = NGAP_CauseRadioNetwork_multiple_PDU_session_ID_instances;
          nb_of_failed_pdusessions++;
          continue;
        }

        // Check pdu session ID is established
        for (j = 0; j < NR_NB_RB_MAX -3; j++) {
          if (UE->pduSession[j].param.pdusession_id == req->pdusession_modify_params[i].pdusession_id) {
            if (UE->pduSession[j].status == PDU_SESSION_STATUS_TORELEASE || UE->pduSession[j].status == PDU_SESSION_STATUS_DONE) {
              break;
            }
            // Found established pdu session, prepare to send RRC message
            UE->modify_pdusession[i].status = PDU_SESSION_STATUS_NEW;
            UE->modify_pdusession[i].param.pdusession_id = req->pdusession_modify_params[i].pdusession_id;
            UE->modify_pdusession[i].cause = NGAP_CAUSE_NOTHING;
            if (NGAP_PDUSESSION_MODIFY_REQ(msg_p).pdusession_modify_params[i].nas_pdu.buffer != NULL) {
              UE->modify_pdusession[i].param.nas_pdu = req->pdusession_modify_params[i].nas_pdu;
            }
            // Save new pdu session parameters, qos, upf addr, teid
            /* FIXXX
            for (qos_flow_index = 0; qos_flow_index < req->pdusession_modify_params[i].nb_qos; qos_flow_index++) {
              UE->modify_pdusession[i].param.qos[qos_flow_index] = req->pdusession_modify_params[i].qos[qos_flow_index];
            }
            UE->modify_pdusession[i].param.nb_qos = req->pdusession_modify_params[i].nb_qos;

            UE->modify_pdusession[i].param.upf_addr = UE->pduSession[j].param.upf_addr;
            UE->modify_pdusession[i].param.gtp_teid = UE->pduSession[j].param.gtp_teid;
            */

            is_treated[i] = true;
            break;
          }
        }

        // handle Unknown pdu session ID
        if (is_treated[i] == false) {
          LOG_D(NR_RRC, "handle Unknown pdu session ID \n");
          UE->modify_pdusession[i].status = PDU_SESSION_STATUS_NEW;
          UE->modify_pdusession[i].param.pdusession_id = req->pdusession_modify_params[i].pdusession_id;
          UE->modify_pdusession[i].cause = NGAP_CAUSE_RADIO_NETWORK;
          UE->modify_pdusession[i].cause_value = NGAP_CauseRadioNetwork_unknown_PDU_session_ID;
          nb_of_failed_pdusessions++;
          is_treated[i] = true;
        }
      }

      UE->nb_of_modify_pdusessions = nb_pdusessions_tomodify;
      UE->nb_of_failed_pdusessions = nb_of_failed_pdusessions;
    }

    if (UE->nb_of_failed_pdusessions < UE->nb_of_modify_pdusessions) {
      LOG_D(NR_RRC, "generate RRCReconfiguration \n");
      rrc_gNB_modify_dedicatedRRCReconfiguration(&ctxt, ue_context_p);
    } else { // all pdu modification failed
      LOG_I(NR_RRC, "pdu session modify failed, fill NGAP_PDUSESSION_MODIFY_RESP with the pdu session information that failed to modify \n");
      uint8_t nb_of_pdu_sessions_failed = 0;
      MessageDef *msg_fail_p = NULL;
      msg_fail_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_PDUSESSION_MODIFY_RESP);
      if (msg_fail_p == NULL) {
        LOG_E(NR_RRC, "itti_alloc_new_message failed, msg_fail_p is NULL \n");
        return (-1);
      }
      ngap_pdusession_modify_resp_t *msg = &NGAP_PDUSESSION_MODIFY_RESP(msg_fail_p);
      msg->gNB_ue_ngap_id = req->gNB_ue_ngap_id;
      msg->nb_of_pdusessions = 0;

      for (int i = 0; i < UE->nb_of_failed_pdusessions; i++) {
        msg->pdusessions_failed[i].pdusession_id = UE->modify_pdusession[i].param.pdusession_id;
        msg->pdusessions_failed[i].cause = UE->modify_pdusession[i].cause;
        msg->pdusessions_failed[i].cause_value = UE->modify_pdusession[i].cause_value;
      }

      msg->nb_of_pdusessions_failed = UE->nb_of_failed_pdusessions;
      itti_send_msg_to_task(TASK_NGAP, instance, msg_fail_p);
      UE->nb_of_modify_pdusessions = 0;
      UE->nb_of_failed_pdusessions = 0;
      memset(UE->modify_pdusession, 0, sizeof(UE->modify_pdusession));
      return (0);
    }
  }
  return 0;
}

//------------------------------------------------------------------------------
int
rrc_gNB_send_NGAP_PDUSESSION_MODIFY_RESP(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
)
//------------------------------------------------------------------------------
{
  MessageDef *msg_p = NULL;
  uint8_t pdu_sessions_failed = 0;
  uint8_t pdu_sessions_done = 0;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  msg_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_PDUSESSION_MODIFY_RESP);
  if (msg_p == NULL) {
    LOG_E(NR_RRC, "itti_alloc_new_message failed, msg_p is NULL \n");
    return (-1);
  }
  ngap_pdusession_modify_resp_t *resp = &NGAP_PDUSESSION_MODIFY_RESP(msg_p);
  LOG_I(NR_RRC, "send message NGAP_PDUSESSION_MODIFY_RESP \n");

  resp->gNB_ue_ngap_id = UE->gNB_ue_ngap_id;

  for (int i = 0; i < UE->nb_of_modify_pdusessions; i++) {
    if (xid == UE->modify_pdusession[i].xid) {
      if (UE->modify_pdusession[i].status == PDU_SESSION_STATUS_DONE) {
        int j;
        for (j = 0; j < UE->setup_pdu_sessions; j++) {
          if (UE->modify_pdusession[i].param.pdusession_id == UE->pduSession[j].param.pdusession_id) {
            LOG_I(NR_RRC, "update pdu session %d \n", UE->pduSession[j].param.pdusession_id);
            // Update UE->pduSession
            UE->pduSession[j].status = PDU_SESSION_STATUS_ESTABLISHED;
            UE->pduSession[j].cause = NGAP_CAUSE_NOTHING;
            for (qos_flow_index = 0; qos_flow_index < UE->modify_pdusession[i].param.nb_qos; qos_flow_index++) {
              UE->pduSession[j].param.qos[qos_flow_index] = UE->modify_pdusession[i].param.qos[qos_flow_index];
            }
            break;
          }
        }

        if (j < UE->setup_pdu_sessions) {
          resp->pdusessions[pdu_sessions_done].pdusession_id = UE->modify_pdusession[i].param.pdusession_id;
          for (qos_flow_index = 0; qos_flow_index < UE->modify_pdusession[i].param.nb_qos; qos_flow_index++) {
            resp->pdusessions[pdu_sessions_done].qos[qos_flow_index].qfi = UE->modify_pdusession[i].param.qos[qos_flow_index].qfi;
          }
          resp->pdusessions[pdu_sessions_done].nb_of_qos_flow = UE->modify_pdusession[i].param.nb_qos;
          LOG_I(NR_RRC,
                "Modify Resp (msg index %d, pdu session index %d, status %d, xid %d): nb_of_modify_pdusessions %d,  pdusession_id %d \n ",
                pdu_sessions_done,
                i,
                UE->modify_pdusession[i].status,
                xid,
                UE->nb_of_modify_pdusessions,
                resp->pdusessions[pdu_sessions_done].pdusession_id);
          pdu_sessions_done++;
        } else {
          resp->pdusessions_failed[pdu_sessions_failed].pdusession_id = UE->modify_pdusession[i].param.pdusession_id;
          resp->pdusessions_failed[pdu_sessions_failed].cause = NGAP_CAUSE_RADIO_NETWORK;
          resp->pdusessions_failed[pdu_sessions_failed].cause_value = NGAP_CauseRadioNetwork_unknown_PDU_session_ID;
          pdu_sessions_failed++;
        }
      } else if ((UE->modify_pdusession[i].status == PDU_SESSION_STATUS_NEW) || (UE->modify_pdusession[i].status == PDU_SESSION_STATUS_ESTABLISHED)) {
        LOG_D (NR_RRC, "PDU SESSION is NEW or already ESTABLISHED\n");
      } else if (UE->modify_pdusession[i].status == PDU_SESSION_STATUS_FAILED) {
        resp->pdusessions_failed[pdu_sessions_failed].pdusession_id = UE->modify_pdusession[i].param.pdusession_id;
        resp->pdusessions_failed[pdu_sessions_failed].cause = UE->modify_pdusession[i].cause;
        resp->pdusessions_failed[pdu_sessions_failed].cause_value = UE->modify_pdusession[i].cause_value;
        pdu_sessions_failed++;
      }
    } else {
      LOG_D(NR_RRC, "xid does not correspond (context pdu session index %d, status %d, xid %d/%d) \n ", i, UE->modify_pdusession[i].status, xid, UE->modify_pdusession[i].xid);
    }
  }

  resp->nb_of_pdusessions = pdu_sessions_done;
  resp->nb_of_pdusessions_failed = pdu_sessions_failed;

  if (pdu_sessions_done > 0 || pdu_sessions_failed > 0) {
    LOG_D(NR_RRC, "NGAP_PDUSESSION_MODIFY_RESP: sending the message: nb_of_pdusessions %d, total pdu session %d, index %d\n", UE->nb_of_modify_pdusessions, UE->setup_pdu_sessions, i);
    itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);
  } else {
    itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
  }

  return 0;
}

//------------------------------------------------------------------------------
void
rrc_gNB_send_NGAP_UE_CONTEXT_RELEASE_REQ(
  const module_id_t gnb_mod_idP,
  const rrc_gNB_ue_context_t *const ue_context_pP,
  const ngap_Cause_t causeP,
  const long cause_valueP)
//------------------------------------------------------------------------------
{
  if (ue_context_pP == NULL) {
    LOG_E(RRC, "[gNB] In NGAP_UE_CONTEXT_RELEASE_REQ: invalid UE\n");
  } else {
    MessageDef *msg_context_release_req_p = NULL;
    msg_context_release_req_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_UE_CONTEXT_RELEASE_REQ);
    NGAP_UE_CONTEXT_RELEASE_REQ(msg_context_release_req_p).gNB_ue_ngap_id = UE->gNB_ue_ngap_id;
    NGAP_UE_CONTEXT_RELEASE_REQ(msg_context_release_req_p).cause             = causeP;
    NGAP_UE_CONTEXT_RELEASE_REQ(msg_context_release_req_p).cause_value       = cause_valueP;
    NGAP_UE_CONTEXT_RELEASE_REQ(msg_context_release_req_p).nb_of_pdusessions = UE->setup_pdu_sessions;
    for (int pdusession = 0; pdusession < UE->setup_pdu_sessions; pdusession++) {
      NGAP_UE_CONTEXT_RELEASE_REQ(msg_context_release_req_p).pdusessions[pdusession].pdusession_id = session->param.pdusession_id;
    }
    itti_send_msg_to_task(TASK_NGAP, GNB_MODULE_ID_TO_INSTANCE(gnb_mod_idP), msg_context_release_req_p);
  }
}
/*------------------------------------------------------------------------------*/
int rrc_gNB_process_NGAP_UE_CONTEXT_RELEASE_REQ(MessageDef *msg_p, instance_t instance)
{
  uint32_t gNB_ue_ngap_id;
  struct rrc_gNB_ue_context_s *ue_context_p = NULL;
  gNB_ue_ngap_id = NGAP_UE_CONTEXT_RELEASE_REQ(msg_p).gNB_ue_ngap_id;
  ue_context_p   = rrc_gNB_get_ue_context_from_ngap_ids(instance, UE_INITIAL_ID_INVALID, gNB_ue_ngap_id);

  if (ue_context_p == NULL) {
    /* Can not associate this message to an UE index, send a failure to ngAP and discard it! */
    MessageDef *msg_fail_p;
    LOG_W(RRC, "[gNB %ld] In NGAP_UE_CONTEXT_RELEASE_REQ: unknown UE from gNB_ue_ngap_id (%u)\n",
          instance,
          gNB_ue_ngap_id);
    msg_fail_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_UE_CONTEXT_RELEASE_RESP); /* TODO change message ID. */
    NGAP_UE_CONTEXT_RELEASE_RESP(msg_fail_p).gNB_ue_ngap_id = gNB_ue_ngap_id;
    // TODO add failure cause when defined!
    itti_send_msg_to_task(TASK_NGAP, instance, msg_fail_p);
    return (-1);
  } else {
    /* TODO release context. */
    /* Send the response */
    {
      MessageDef *msg_resp_p;
      msg_resp_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_UE_CONTEXT_RELEASE_RESP);
      NGAP_UE_CONTEXT_RELEASE_RESP(msg_resp_p).gNB_ue_ngap_id = gNB_ue_ngap_id;
      itti_send_msg_to_task(TASK_NGAP, instance, msg_resp_p);
    }
    return (0);
  }
}

//-----------------------------------------------------------------------------
/*
* Process the NG command NGAP_UE_CONTEXT_RELEASE_COMMAND, sent by AMF.
* The gNB should remove all pdu session, NG context, and other context of the UE.
*/
int rrc_gNB_process_NGAP_UE_CONTEXT_RELEASE_COMMAND(MessageDef *msg_p, instance_t instance)
{
  //-----------------------------------------------------------------------------
  uint32_t gNB_ue_ngap_id = 0;
  protocol_ctxt_t ctxt;
  struct rrc_gNB_ue_context_s *ue_context_p = NULL;
  struct rrc_ue_ngap_ids_s *rrc_ue_ngap_ids = NULL;
  gNB_ue_ngap_id = NGAP_UE_CONTEXT_RELEASE_COMMAND(msg_p).gNB_ue_ngap_id;
  ue_context_p = rrc_gNB_get_ue_context_from_ngap_ids(instance, UE_INITIAL_ID_INVALID, gNB_ue_ngap_id);

  if (ue_context_p == NULL) {
    /* Can not associate this message to an UE index */
    MessageDef *msg_complete_p = NULL;
    LOG_W(NR_RRC, "[gNB %ld] In NGAP_UE_CONTEXT_RELEASE_COMMAND: unknown UE from gNB_ue_ngap_id (%u)\n",
          instance,
          gNB_ue_ngap_id);
    msg_complete_p = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_UE_CONTEXT_RELEASE_COMPLETE);
    NGAP_UE_CONTEXT_RELEASE_COMPLETE(msg_complete_p).gNB_ue_ngap_id = gNB_ue_ngap_id;
    itti_send_msg_to_task(TASK_NGAP, instance, msg_complete_p);
    rrc_ue_ngap_ids = rrc_gNB_NGAP_get_ue_ids(RC.nrrrc[instance], UE_INITIAL_ID_INVALID, gNB_ue_ngap_id);

    if (rrc_ue_ngap_ids != NULL) {
      rrc_gNB_NGAP_remove_ue_ids(RC.nrrrc[instance], rrc_ue_ngap_ids);
    }

    return -1;
  } else {
    UE->ue_release_timer_ng = 0;
    UE->ue_release_timer_thres_rrc = 1000;
    PROTOCOL_CTXT_SET_BY_INSTANCE(&ctxt, instance, GNB_FLAG_YES, UE->rnti, 0, 0);
    ctxt.eNB_index = 0;
    rrc_gNB_generate_RRCRelease(&ctxt, ue_context_p);
    return 0;
  }
}

void rrc_gNB_send_NGAP_UE_CONTEXT_RELEASE_COMPLETE(
  instance_t instance,
  uint32_t   gNB_ue_ngap_id) {
  MessageDef *msg = itti_alloc_new_message(TASK_RRC_GNB, 0, NGAP_UE_CONTEXT_RELEASE_COMPLETE);
  NGAP_UE_CONTEXT_RELEASE_COMPLETE(msg).gNB_ue_ngap_id = gNB_ue_ngap_id;
  itti_send_msg_to_task(TASK_NGAP, instance, msg);
}

//------------------------------------------------------------------------------
/*
 * Remove UE ids (gNB_ue_ngap_id and ng_id) from hashtables.
 */
void
rrc_gNB_NGAP_remove_ue_ids(
  gNB_RRC_INST *const rrc_instance_pP,
  struct rrc_ue_ngap_ids_s *const ue_ids_pP
)
//------------------------------------------------------------------------------
{
  hashtable_rc_t h_rc;

  if (rrc_instance_pP == NULL) {
    LOG_E(NR_RRC, "Bad NR RRC instance\n");
    return;
  }

  if (ue_ids_pP == NULL) {
    LOG_E(NR_RRC, "Trying to free a NULL NGAP UE IDs\n");
    return;
  }

  const uint32_t gNB_ue_ngap_id = ue_ids_pP->gNB_ue_ngap_id;
  const uint32_t gNB_ue_ngap_id = ue_ids_pP->gNB_ue_ngap_id;

  if (gNB_ue_ngap_id > 0) {
    h_rc = hashtable_remove(rrc_instance_pP->ngap_id2_ngap_ids, (hash_key_t)gNB_ue_ngap_id);

    if (h_rc != HASH_TABLE_OK) {
      LOG_W(NR_RRC, "NGAP Did not find entry in hashtable ngap_id2_ngap_ids for gNB_ue_ngap_id %u\n", gNB_ue_ngap_id);
    } else {
      LOG_W(NR_RRC, "NGAP removed entry in hashtable ngap_id2_ngap_ids for gNB_ue_ngap_id %u\n", gNB_ue_ngap_id);
    }
  }

  if (gNB_ue_ngap_id != UE_INITIAL_ID_INVALID) {
    h_rc = hashtable_remove(rrc_instance_pP->initial_id2_ngap_ids, (hash_key_t)gNB_ue_ngap_id);

    if (h_rc != HASH_TABLE_OK) {
      LOG_W(NR_RRC, "NGAP Did not find entry in hashtable initial_id2_ngap_ids for gNB_ue_ngap_id %u\n", gNB_ue_ngap_id);
    } else {
      LOG_W(NR_RRC, "NGAP removed entry in hashtable initial_id2_ngap_ids for gNB_ue_ngap_id %u\n", gNB_ue_ngap_id);
    }
  }
}
void
rrc_gNB_send_NGAP_UE_CAPABILITIES_IND(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  NR_UL_DCCH_Message_t     *const ul_dcch_msg
)
//------------------------------------------------------------------------------
{
    NR_UE_CapabilityRAT_ContainerList_t *ueCapabilityRATContainerList = ul_dcch_msg->message.choice.c1->choice.ueCapabilityInformation->criticalExtensions.choice.ueCapabilityInformation->ue_CapabilityRAT_ContainerList;
    /* 4096 is arbitrary, should be big enough */
    unsigned char buf[4096];
    unsigned char *buf2;
    NR_UERadioAccessCapabilityInformation_t rac;
    
    if (ueCapabilityRATContainerList->list.count == 0) {
      LOG_W(RRC, "[gNB %d][UE %x] bad UE capabilities\n", ctxt_pP->module_id, UE->rnti);
    }
    
    asn_enc_rval_t ret = uper_encode_to_buffer(&asn_DEF_NR_UE_CapabilityRAT_ContainerList, NULL, ueCapabilityRATContainerList, buf, 4096);
    
    if (ret.encoded == -1) abort();
    
    memset(&rac, 0, sizeof(NR_UERadioAccessCapabilityInformation_t));
    rac.criticalExtensions.present = NR_UERadioAccessCapabilityInformation__criticalExtensions_PR_c1;
    rac.criticalExtensions.choice.c1 = calloc(1,sizeof(*rac.criticalExtensions.choice.c1));
    rac.criticalExtensions.choice.c1->present = NR_UERadioAccessCapabilityInformation__criticalExtensions__c1_PR_ueRadioAccessCapabilityInformation;
    rac.criticalExtensions.choice.c1->choice.ueRadioAccessCapabilityInformation = calloc(1,sizeof(NR_UERadioAccessCapabilityInformation_IEs_t));
    rac.criticalExtensions.choice.c1->choice.ueRadioAccessCapabilityInformation->ue_RadioAccessCapabilityInfo.buf = buf;
    rac.criticalExtensions.choice.c1->choice.ueRadioAccessCapabilityInformation->ue_RadioAccessCapabilityInfo.size = (ret.encoded+7)/8;
    rac.criticalExtensions.choice.c1->choice.ueRadioAccessCapabilityInformation->nonCriticalExtension = NULL;
    /* 8192 is arbitrary, should be big enough */
    buf2 = malloc16(8192);
    
    if (buf2 == NULL) abort();
    
    ret = uper_encode_to_buffer(&asn_DEF_NR_UERadioAccessCapabilityInformation, NULL, &rac, buf2, 8192);
    
    if (ret.encoded == -1) abort();

    MessageDef *msg_p;
    msg_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_UE_CAPABILITIES_IND);
    NGAP_UE_CAPABILITIES_IND(msg_p).gNB_ue_ngap_id = UE->gNB_ue_ngap_id;
    NGAP_UE_CAPABILITIES_IND (msg_p).ue_radio_cap.length = (ret.encoded+7)/8;
    NGAP_UE_CAPABILITIES_IND (msg_p).ue_radio_cap.buffer = buf2;
    itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);
    LOG_I(NR_RRC,"Send message to ngap: NGAP_UE_CAPABILITIES_IND\n");
}

//------------------------------------------------------------------------------
void
rrc_gNB_send_NGAP_PDUSESSION_RELEASE_RESPONSE(
  const protocol_ctxt_t    *const ctxt_pP,
  rrc_gNB_ue_context_t     *const ue_context_pP,
  uint8_t                   xid
)
//------------------------------------------------------------------------------
{
  int pdu_sessions_released = 0;
  MessageDef   *msg_p;
  msg_p = itti_alloc_new_message (TASK_RRC_GNB, 0, NGAP_PDUSESSION_RELEASE_RESPONSE);
  NGAP_PDUSESSION_RELEASE_RESPONSE(msg_p).gNB_ue_ngap_id = UE->gNB_ue_ngap_id;
  gNB_RRC_UE_t *UE = &ue_context_pP->ue_context;

  for (int i = 0;  i < NB_RB_MAX; i++) {
    if (xid == UE->pduSession[i].xid) {
      NGAP_PDUSESSION_RELEASE_RESPONSE(msg_p).pdusession_release[pdu_sessions_released].pdusession_id = UE->pduSession[i].param.pdusession_id;
      pdu_sessions_released++;
      //clear
      memset(&UE->pduSession[i], 0, sizeof(pdu_session_param_t));
    }
  }

  NGAP_PDUSESSION_RELEASE_RESPONSE (msg_p).nb_of_pdusessions_released = pdu_sessions_released;
  NGAP_PDUSESSION_RELEASE_RESPONSE(msg_p).nb_of_pdusessions_failed = UE->nb_release_of_pdusessions;
  memcpy(&(NGAP_PDUSESSION_RELEASE_RESPONSE(msg_p).pdusessions_failed[0]), &UE->pdusessions_release_failed[0], sizeof(pdusession_failed_t) * UE->nb_release_of_pdusessions);
  UE->setup_pdu_sessions -= pdu_sessions_released;
  LOG_I(NR_RRC,
        "NGAP PDUSESSION RELEASE RESPONSE: GNB_UE_NGAP_ID %u release_pdu_sessions %d setup_pdu_sessions %d \n",
        NGAP_PDUSESSION_RELEASE_RESPONSE(msg_p).gNB_ue_ngap_id,
        pdu_sessions_released,
        UE->setup_pdu_sessions);
  itti_send_msg_to_task (TASK_NGAP, ctxt_pP->instance, msg_p);

  //clear xid
  for(int i = 0; i < NB_RB_MAX; i++) {
    UE->pduSession[i].xid = -1;
  }

  //clear release pdusessions
  UE->nb_release_of_pdusessions = 0;
  memset(&UE->pdusessions_release_failed[0], 0, sizeof(pdusession_failed_t) * NGAP_MAX_PDUSESSION);
}

//------------------------------------------------------------------------------
int rrc_gNB_process_NGAP_PDUSESSION_RELEASE_COMMAND(MessageDef *msg_p, instance_t instance)
//------------------------------------------------------------------------------
{
  uint32_t                        gNB_ue_ngap_id;
  rrc_gNB_ue_context_t           *ue_context_p = NULL;
  protocol_ctxt_t                 ctxt;
  pdusession_release_t            pdusession_release_params[NGAP_MAX_PDUSESSION];
  uint8_t                         nb_pdusessions_torelease;
  uint8_t xid;
  int i, pdusession;
  uint8_t b_existed,is_existed;
  uint8_t pdusession_release_drb = 0;

  memcpy(&pdusession_release_params[0], &(NGAP_PDUSESSION_RELEASE_COMMAND (msg_p).pdusession_release_params[0]),
    sizeof(pdusession_release_t)*NGAP_MAX_PDUSESSION);
  gNB_ue_ngap_id = NGAP_PDUSESSION_RELEASE_COMMAND(msg_p).gNB_ue_ngap_id;
  nb_pdusessions_torelease = NGAP_PDUSESSION_RELEASE_COMMAND(msg_p).nb_pdusessions_torelease;
  if (nb_pdusessions_torelease > NGAP_MAX_PDUSESSION) {
    return -1;
  }
  ue_context_p   = rrc_gNB_get_ue_context_from_ngap_ids(instance, UE_INITIAL_ID_INVALID, gNB_ue_ngap_id);
  LOG_I(NR_RRC, "[gNB %ld] gNB_ue_ngap_id %u \n", instance, gNB_ue_ngap_id);

  if (ue_context_p != NULL) {
    PROTOCOL_CTXT_SET_BY_INSTANCE(&ctxt, instance, GNB_FLAG_YES, UE->rnti, 0, 0);
    xid = rrc_gNB_get_next_transaction_identifier(ctxt.module_id);
    LOG_I(NR_RRC,"PDU Session Release Command: AMF_UE_NGAP_ID %lu  GNB_UE_NGAP_ID %u release_pdusessions %d \n",
          NGAP_PDUSESSION_RELEASE_COMMAND (msg_p).amf_ue_ngap_id&0x000000FFFFFFFFFF, gNB_ue_ngap_id, nb_pdusessions_torelease);

    for (pdusession = 0; pdusession < nb_pdusessions_torelease; pdusession++) {
      b_existed = 0;
      is_existed = 0;

      for (i = pdusession-1; i >= 0; i--) {
        if (pdusession_release_params[pdusession].pdusession_id == pdusession_release_params[i].pdusession_id) {
          is_existed = 1;
          break;
        }
      }

      if(is_existed == 1) {
        // pdusession_id is existed
        continue;
      }

      for (i = 0;  i < NR_NB_RB_MAX; i++) {
        if (pdusession_release_params[pdusession].pdusession_id == UE->pduSession[i].param.pdusession_id) {
          b_existed = 1;
          break;
        }
      }

      if(b_existed == 0) {
        // no pdusession_id
        LOG_I(NR_RRC, "no pdusession_id \n");
        UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].pdusession_id = pdusession_release_params[pdusession].pdusession_id;
        UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].cause = NGAP_CAUSE_RADIO_NETWORK;
        UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].cause_value = 30;
        UE->nb_release_of_pdusessions++;
      } else {
        if (UE->pduSession[i].status == PDU_SESSION_STATUS_FAILED) {
          UE->pduSession[i].xid = xid;
          continue;
        } else if (UE->pduSession[i].status == PDU_SESSION_STATUS_ESTABLISHED) {
          LOG_I(NR_RRC, "RELEASE pdusession %d \n", UE->pduSession[i].param.pdusession_id);
          UE->pduSession[i].status = PDU_SESSION_STATUS_TORELEASE;
          UE->pduSession[i].xid = xid;
          pdusession_release_drb++;
        } else {
          // pdusession_id status NG
          UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].pdusession_id = pdusession_release_params[pdusession].pdusession_id;
          UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].cause = NGAP_CAUSE_RADIO_NETWORK;
          UE->pdusessions_release_failed[UE->nb_release_of_pdusessions].cause_value = 0;
          UE->nb_release_of_pdusessions++;
        }
      }
    }

    if(pdusession_release_drb > 0) {
      //TODO RRCReconfiguration To UE
      LOG_I(NR_RRC, "Send RRCReconfiguration To UE \n");
      rrc_gNB_generate_dedicatedRRCReconfiguration_release(&ctxt, ue_context_p, xid, NGAP_PDUSESSION_RELEASE_COMMAND (msg_p).nas_pdu.length, NGAP_PDUSESSION_RELEASE_COMMAND (msg_p).nas_pdu.buffer);
    } else {
      //gtp tunnel delete
      LOG_I(NR_RRC, "gtp tunnel delete \n");
      gtpv1u_gnb_delete_tunnel_req_t req={0};
      req.ue_id = UE->rnti;

      for(i = 0; i < NB_RB_MAX; i++) {
        if (xid == UE->pduSession[i].xid) {
          req.pdusession_id[req.num_pdusession++] = UE->gnb_gtp_psi[i];
          UE->gnb_gtp_teid[i] = 0;
          memset(&UE->gnb_gtp_addrs[i], 0, sizeof(UE->gnb_gtp_addrs[i]));
          UE->gnb_gtp_psi[i] = 0;
        }
      }
      gtpv1u_delete_ngu_tunnel(instance, &req);
      //NGAP_PDUSESSION_RELEASE_RESPONSE
      rrc_gNB_send_NGAP_PDUSESSION_RELEASE_RESPONSE(&ctxt, ue_context_p, xid);
      LOG_I(NR_RRC, "Send PDU Session Release Response \n");
    }
  } else {
    LOG_E(NR_RRC, "PDU Session Release Command: AMF_UE_NGAP_ID %lu  GNB_UE_NGAP_ID %u  Error ue_context_p NULL \n",
          NGAP_PDUSESSION_RELEASE_COMMAND (msg_p).amf_ue_ngap_id&0x000000FFFFFFFFFF, NGAP_PDUSESSION_RELEASE_COMMAND(msg_p).gNB_ue_ngap_id);
    return -1;
  }

  return 0;
}

void nr_rrc_rx_tx(void) {
  // check timers

  // check if UEs are lost, to remove them from upper layers

  //

}

/*------------------------------------------------------------------------------*/
int rrc_gNB_process_PAGING_IND(MessageDef *msg_p, instance_t instance)
{
  for (uint16_t tai_size = 0; tai_size < NGAP_PAGING_IND(msg_p).tai_size; tai_size++) {
    LOG_I(NR_RRC,"[gNB %ld] In NGAP_PAGING_IND: MCC %d, MNC %d, TAC %d\n", instance, NGAP_PAGING_IND(msg_p).plmn_identity[tai_size].mcc,
          NGAP_PAGING_IND(msg_p).plmn_identity[tai_size].mnc, NGAP_PAGING_IND(msg_p).tac[tai_size]);

    for (uint8_t j = 0; j < RC.nrrrc[instance]->configuration.num_plmn; j++) {
      if (RC.nrrrc[instance]->configuration.mcc[j] == NGAP_PAGING_IND(msg_p).plmn_identity[tai_size].mcc
          && RC.nrrrc[instance]->configuration.mnc[j] == NGAP_PAGING_IND(msg_p).plmn_identity[tai_size].mnc
          && RC.nrrrc[instance]->configuration.tac == NGAP_PAGING_IND(msg_p).tac[tai_size]) {
        for (uint8_t CC_id = 0; CC_id < MAX_NUM_CCs; CC_id++) {
          if (NODE_IS_CU(RC.nrrrc[instance]->node_type)) {
            MessageDef *m = itti_alloc_new_message(TASK_RRC_GNB, 0, F1AP_PAGING_IND);
            F1AP_PAGING_IND (m).mcc              = RC.nrrrc[j]->configuration.mcc[0];
            F1AP_PAGING_IND (m).mnc              = RC.nrrrc[j]->configuration.mnc[0];
            F1AP_PAGING_IND (m).mnc_digit_length = RC.nrrrc[j]->configuration.mnc_digit_length[0];
            F1AP_PAGING_IND (m).nr_cellid        = RC.nrrrc[j]->nr_cellid;
            F1AP_PAGING_IND (m).ueidentityindexvalue = (uint16_t)(NGAP_PAGING_IND(msg_p).ue_paging_identity.s_tmsi.m_tmsi%1024);
            F1AP_PAGING_IND (m).fiveg_s_tmsi = NGAP_PAGING_IND(msg_p).ue_paging_identity.s_tmsi.m_tmsi;
            F1AP_PAGING_IND (m).paging_drx = NGAP_PAGING_IND(msg_p).paging_drx;
            LOG_E(F1AP, "ueidentityindexvalue %u fiveg_s_tmsi %ld paging_drx %u\n", F1AP_PAGING_IND (m).ueidentityindexvalue, F1AP_PAGING_IND (m).fiveg_s_tmsi, F1AP_PAGING_IND (m).paging_drx);
            itti_send_msg_to_task(TASK_CU_F1, instance, m);
          } else {
            rrc_gNB_generate_pcch_msg(NGAP_PAGING_IND(msg_p).ue_paging_identity.s_tmsi.m_tmsi,(uint8_t)NGAP_PAGING_IND(msg_p).paging_drx, instance, CC_id);
          } // end of nodetype check
        } // end of cc loop
      } // end of mcc mnc check
    } // end of num_plmn
  } // end of tai size

  return 0;
}
