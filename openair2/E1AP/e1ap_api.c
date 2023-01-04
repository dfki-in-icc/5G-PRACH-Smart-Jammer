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

#include <arpa/inet.h>
#include "e1ap_api.h"
#include "UTIL/OSA/osa_defs.h"
#include "nr_pdcp/nr_pdcp_entity.h"
#include "openair2/LAYER2/nr_pdcp/nr_pdcp_e1_api.h"
#include "openair2/RRC/NR/cucp_cuup_if.h"
#include "openair2/RRC/LTE/MESSAGES/asn1_msg.h"
#include "openair3/ocp-gtpu/gtp_itf.h"
#include "e1ap_asnc.h"
#include "e1ap_common.h"
#include "e1ap.h"

struct NR_DRB_ToAddMod;
static void fill_DRB_configList_e1(NR_DRB_ToAddModList_t *DRB_configList, pdu_session_to_setup_t *pdu) {

  for (int i=0; i < pdu->numDRB2Setup; i++) {
    DRB_nGRAN_to_setup_t *drb = pdu->DRBnGRanList + i;
    asn1cSequenceAdd(DRB_configList->list, struct NR_DRB_ToAddMod, ie);
    ie->drb_Identity = drb->id;
    ie->cnAssociation = CALLOC(1, sizeof(*ie->cnAssociation));
    ie->cnAssociation->present = NR_DRB_ToAddMod__cnAssociation_PR_sdap_Config;

    // sdap_Config
    NR_SDAP_Config_t *sdap_config = CALLOC(1, sizeof(*sdap_config));
    memset(sdap_config, 0, sizeof(*sdap_config));
    ie->cnAssociation->choice.sdap_Config = sdap_config;
    sdap_config->pdu_Session = pdu->sessionId;
    sdap_config->sdap_HeaderDL = drb->sDAP_Header_DL;
    sdap_config->sdap_HeaderUL = drb->sDAP_Header_UL;
    sdap_config->defaultDRB = drb->defaultDRB;

    sdap_config->mappedQoS_FlowsToAdd = calloc(1, sizeof(struct NR_SDAP_Config__mappedQoS_FlowsToAdd));
    for (int j=0; j < drb->numQosFlow2Setup; j++) {
      NR_QFI_t *qfi = calloc(1, sizeof(NR_QFI_t));
      *qfi = drb->qosFlows[j].fiveQI;
      ASN_SEQUENCE_ADD(&sdap_config->mappedQoS_FlowsToAdd->list, qfi);
    }
    sdap_config->mappedQoS_FlowsToRelease = NULL;

    // pdcp_Config
    ie->reestablishPDCP = NULL;
    ie->recoverPDCP = NULL;
    NR_PDCP_Config_t *pdcp_config = calloc(1, sizeof(*pdcp_config));
    memset(pdcp_config, 0, sizeof(*pdcp_config));
    ie->pdcp_Config = pdcp_config;
    pdcp_config->drb = calloc(1,sizeof(*pdcp_config->drb));
    pdcp_config->drb->discardTimer = calloc(1, sizeof(*pdcp_config->drb->discardTimer));
    *pdcp_config->drb->discardTimer = drb->discardTimer;
    pdcp_config->drb->pdcp_SN_SizeUL = calloc(1, sizeof(*pdcp_config->drb->pdcp_SN_SizeUL));
    *pdcp_config->drb->pdcp_SN_SizeUL = drb->pDCP_SN_Size_UL;
    pdcp_config->drb->pdcp_SN_SizeDL = calloc(1, sizeof(*pdcp_config->drb->pdcp_SN_SizeDL));
    *pdcp_config->drb->pdcp_SN_SizeDL = drb->pDCP_SN_Size_DL;
    pdcp_config->drb->headerCompression.present = NR_PDCP_Config__drb__headerCompression_PR_notUsed;
    pdcp_config->drb->headerCompression.choice.notUsed = 0;

    pdcp_config->drb->integrityProtection = NULL;
    pdcp_config->drb->statusReportRequired = NULL;
    pdcp_config->drb->outOfOrderDelivery = NULL;
    pdcp_config->moreThanOneRLC = NULL;

    pdcp_config->t_Reordering = calloc(1, sizeof(*pdcp_config->t_Reordering));
    *pdcp_config->t_Reordering = drb->reorderingTimer;
    pdcp_config->ext1 = NULL;

    if (pdu->integrityProtectionIndication == 0 || // Required
        pdu->integrityProtectionIndication == 1) { // Preferred
      pdcp_config->drb->integrityProtection = calloc(1, sizeof(*pdcp_config->drb->integrityProtection));
      *pdcp_config->drb->integrityProtection = NR_PDCP_Config__drb__integrityProtection_enabled;
    }

    if (pdu->confidentialityProtectionIndication == 0 || // Required
        pdu->confidentialityProtectionIndication == 1) { // Preferred
      pdcp_config->ext1 = calloc(1, sizeof(*pdcp_config->ext1));
      pdcp_config->ext1->cipheringDisabled = calloc(1, sizeof(*pdcp_config->ext1->cipheringDisabled));
      *pdcp_config->ext1->cipheringDisabled = NR_PDCP_Config__ext1__cipheringDisabled_true;
    }
  }
}

static int drb_config_N3gtpu_create(e1ap_bearer_setup_req_t * const req,
                                    gtpv1u_gnb_create_tunnel_resp_t *create_tunnel_resp,
                                    instance_t instance) {

  gtpv1u_gnb_create_tunnel_req_t create_tunnel_req={0};

  NR_DRB_ToAddModList_t DRB_configList = {0};
  for (int i=0; i < req->numPDUSessions; i++) {
    pdu_session_to_setup_t *const pdu = &req->pduSession[i];
    create_tunnel_req.pdusession_id[i] = pdu->sessionId;
    create_tunnel_req.incoming_rb_id[i] = pdu->DRBnGRanList[0].id; // taking only the first DRB. TODO:change this
    memcpy(&create_tunnel_req.dst_addr[i].buffer,
           &pdu->tlAddress,
           sizeof(uint8_t)*4);
    create_tunnel_req.dst_addr[i].length = 32; // 8bits * 4bytes
    create_tunnel_req.outgoing_teid[i] = pdu->teId;
    fill_DRB_configList_e1(&DRB_configList, pdu);
  }
  create_tunnel_req.num_tunnels = req->numPDUSessions;
  create_tunnel_req.ue_id = (req->gNB_cu_cp_ue_id & 0xFFFF);

  // Create N3 tunnel
  int ret = gtpv1u_create_ngu_tunnel(instance,
                                     &create_tunnel_req,
                                     create_tunnel_resp);
  if (ret != 0) {
    LOG_E(NR_RRC,"rrc_gNB_process_NGAP_PDUSESSION_SETUP_REQ : gtpv1u_create_ngu_tunnel failed,start to release UE id %ld\n",
          create_tunnel_req.ue_id);
    return ret;
  }

  // Configure DRBs
  uint8_t *kUPenc = NULL;
  uint8_t *kUPint = NULL;

  nr_derive_key_up_enc(req->cipheringAlgorithm,
                       (uint8_t *)req->encryptionKey,
                       &kUPenc);

  nr_derive_key_up_int(req->integrityProtectionAlgorithm,
                       (uint8_t *)req->integrityProtectionKey,
                       &kUPint);

  nr_pdcp_e1_add_drbs(true, // set this to notify PDCP that his not UE
                      create_tunnel_req.ue_id,
                      &DRB_configList,
                      (req->integrityProtectionAlgorithm << 4) | req->cipheringAlgorithm,
                      kUPenc,
                      kUPint);
  return ret;
}

void CUUP_process_e1_bearer_context_setup_req(e1ap_bearer_setup_req_t *const req, instance_t instance) {

  gtpv1u_gnb_create_tunnel_resp_t create_tunnel_resp_N3={0};

  // GTP tunnel for UL
  instance_t gtpInst = getCxtE1(UPtype, instance)->gtpInstN3;
  drb_config_N3gtpu_create(req, &create_tunnel_resp_N3, gtpInst);

  e1ap_bearer_setup_resp_t resp = {0};

  int remote_port = getCxtE1(UPtype, instance)->setupReq.remoteDUPort;
  in_addr_t my_addr;
  inet_pton(AF_INET,
            getCxtE1(UPtype, instance)->setupReq.CUUP_e1_ip_address.ipv4_address,
            &my_addr);

  gtpInst = getCxtE1(UPtype, instance)->gtpInstF1U;
  fill_e1ap_bearer_setup_resp(&resp, req, gtpInst, req->gNB_cu_cp_ue_id, remote_port, my_addr);

  resp.gNB_cu_cp_ue_id = req->gNB_cu_cp_ue_id;
  resp.numPDUSessions = req->numPDUSessions;
  for (int i=0; i < req->numPDUSessions; i++) {
    pdu_session_setup_t    *pduSetup  = resp.pduSession + i;
    pdu_session_to_setup_t *pdu2Setup = req->pduSession + i;

    pduSetup->id = pdu2Setup->sessionId;
    memcpy(&pduSetup->tlAddress,
           &getCxtE1(UPtype, instance)->setupReq.IPv4AddressN3,
           sizeof(in_addr_t));
    pduSetup->teId = create_tunnel_resp_N3.gnb_NGu_teid[i];
    pduSetup->numDRBSetup = pdu2Setup->numDRB2Setup;

    // At this point we don't have a way to know the DRBs that failed to setup
    // We assume all DRBs to setup have are setup successfully so we always send successful outcome in response
    // TODO: Modify nr_pdcp_add_drbs() to return DRB list that failed to setup to support E1AP
    pduSetup->numDRBFailed = 0;
  }

  e1apCUUP_send_BEARER_CONTEXT_SETUP_RESPONSE(instance, &resp);
}

void CUUP_process_bearer_context_mod_req(e1ap_bearer_setup_req_t *const req, instance_t instance) {
  instance_t gtpInst = getCxtE1(UPtype, instance)->gtpInstF1U;
  CU_update_UP_DL_tunnel(req, gtpInst, req->gNB_cu_cp_ue_id);
  // TODO: send bearer cxt mod response
}

void CUUP_process_bearer_release_command(e1ap_bearer_release_cmd_t *const cmd, instance_t instance) {
  instance_t gtpInst = getCxtE1(UPtype, instance)->gtpInstN3;
  newGtpuDeleteAllTunnels(gtpInst, cmd->gNB_cu_up_ue_id);
  gtpInst = getCxtE1(UPtype, instance)->gtpInstF1U;
  newGtpuDeleteAllTunnels(gtpInst, cmd->gNB_cu_up_ue_id);

  e1apCUUP_send_BEARER_CONTEXT_RELEASE_COMPLETE(instance, cmd);
}

