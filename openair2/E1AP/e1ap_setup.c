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

#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include "common/utils/LOG/log.h"
#include "common/utils/nr/nr_common.h"
#include "common/utils/LOG/log_extern.h"
#include "assertions.h"
#include "common/utils/ocp_itti/intertask_interface.h"
#include "openair2/GNB_APP/gnb_paramdef.h"
#include "openair3/ocp-gtpu/gtp_itf.h"

MessageDef *RCconfig_NR_CU_E1(bool separate_CUUP_process)
{
  MessageDef *msgConfig = itti_alloc_new_message(TASK_GNB_APP, 0, E1AP_SETUP_REQ);
  if (!msgConfig)
    return NULL;

  paramdef_t GNBSParams[] = GNBSPARAMS_DESC;
  paramdef_t GNBParams[] = GNBPARAMS_DESC;
  paramdef_t GNBE1Params[] = GNBE1PARAMS_DESC;
  paramlist_def_t GNBParamList = {GNB_CONFIG_STRING_GNB_LIST, NULL, 0};
  paramlist_def_t GNBE1ParamList = {GNB_CONFIG_STRING_E1_PARAMETERS, NULL, 0};
  config_get(GNBSParams, sizeof(GNBSParams) / sizeof(paramdef_t), NULL);
  char aprefix[MAX_OPTNAME_SIZE * 2 + 8];
  sprintf(aprefix, "%s.[%i]", GNB_CONFIG_STRING_GNB_LIST, 0);
  int num_gnbs = GNBSParams[GNB_ACTIVE_GNBS_IDX].numelt;
  AssertFatal(num_gnbs == 1, "Support only one gNB per process\n");

  if (num_gnbs > 0) {
    config_getlist(&GNBParamList, GNBParams, sizeof(GNBParams) / sizeof(paramdef_t), NULL);
    AssertFatal(GNBParamList.paramarray[0][GNB_GNB_ID_IDX].uptr != NULL, "gNB id %u is not defined in configuration file\n", 0);
    config_getlist(&GNBE1ParamList, GNBE1Params, sizeof(GNBE1Params) / sizeof(paramdef_t), aprefix);
    e1ap_setup_req_t *e1Setup = &E1AP_SETUP_REQ(msgConfig);
    msgConfig->ittiMsgHeader.destinationInstance = 0;
    e1Setup->gNB_cu_up_id = *(GNBParamList.paramarray[0][GNB_GNB_ID_IDX].uptr);

    paramdef_t PLMNParams[] = GNBPLMNPARAMS_DESC;
    paramlist_def_t PLMNParamList = {GNB_CONFIG_STRING_PLMN_LIST, NULL, 0};
    /* map parameter checking array instances to parameter definition array instances */
    checkedparam_t config_check_PLMNParams[] = PLMNPARAMS_CHECK;

    for (int I = 0; I < sizeof(PLMNParams) / sizeof(paramdef_t); ++I)
      PLMNParams[I].chkPptr = &(config_check_PLMNParams[I]);

    config_getlist(&PLMNParamList, PLMNParams, sizeof(PLMNParams) / sizeof(paramdef_t), aprefix);
    int numPLMNs = PLMNParamList.numelt;
    e1Setup->supported_plmns = numPLMNs;

    for (int I = 0; I < numPLMNs; I++) {
      e1Setup->plmns[I].mcc = *PLMNParamList.paramarray[I][GNB_MOBILE_COUNTRY_CODE_IDX].uptr;
      e1Setup->plmns[I].mnc = *PLMNParamList.paramarray[I][GNB_MOBILE_NETWORK_CODE_IDX].uptr;
      e1Setup->plmns[I].mnc = *PLMNParamList.paramarray[I][GNB_MNC_DIGIT_LENGTH].u8ptr;
    }

    if (separate_CUUP_process) {
      strcpy(e1Setup->CUCP_e1_ip_address.ipv4_address, *(GNBE1ParamList.paramarray[0][GNB_CONFIG_E1_IPV4_ADDRESS_CUCP].strptr));
      e1Setup->CUCP_e1_ip_address.ipv4 = 1;
      e1Setup->port_cucp = *GNBE1ParamList.paramarray[0][GNB_CONFIG_E1_PORT_CUCP].uptr;
      strcpy(e1Setup->CUUP_e1_ip_address.ipv4_address, *(GNBE1ParamList.paramarray[0][GNB_CONFIG_E1_IPV4_ADDRESS_CUUP].strptr));
      e1Setup->CUUP_e1_ip_address.ipv4 = 1;
      e1Setup->port_cuup = *GNBE1ParamList.paramarray[0][GNB_CONFIG_E1_PORT_CUUP].uptr;
      e1Setup->remoteDUPort = e1Setup->port_cuup; // set same as local port for now TODO: get from F1 config
      e1Setup->cn_support = *GNBE1ParamList.paramarray[0][GNB_CONFIG_E1_CN_SUPPORT].uptr;
    }

    char N3Addr[64];
    int N3Port;
    if (!get_NGU_S1U_addr(N3Addr, &N3Port)) {
      inet_pton(AF_INET, N3Addr, &e1Setup->IPv4AddressN3);
      e1Setup->portN3 = N3Port;
    }
  }
  return msgConfig;
}
