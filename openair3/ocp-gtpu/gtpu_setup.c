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

int get_NGU_S1U_addr(char *addr, int *port)
{
  int num_gnbs = 0;
  char *gnb_ipv4_address_for_NGU = NULL;
  uint32_t gnb_port_for_NGU = 0;
  char *gnb_ipv4_address_for_S1U = NULL;
  uint32_t gnb_port_for_S1U = 0;
  char gtpupath[MAX_OPTNAME_SIZE * 2 + 8];

  paramdef_t GNBSParams[] = GNBSPARAMS_DESC;
  paramdef_t NETParams[] = GNBNETPARAMS_DESC;
  LOG_I(GTPU, "Configuring GTPu\n");

  /* get number of active eNodeBs */
  config_get(GNBSParams, sizeof(GNBSParams) / sizeof(paramdef_t), NULL);
  num_gnbs = GNBSParams[GNB_ACTIVE_GNBS_IDX].numelt;
  AssertFatal(num_gnbs > 0, "Failed to parse config file no active gNodeBs in %s \n", GNB_CONFIG_STRING_ACTIVE_GNBS);

  sprintf(gtpupath, "%s.[%i].%s", GNB_CONFIG_STRING_GNB_LIST, 0, GNB_CONFIG_STRING_NETWORK_INTERFACES_CONFIG);
  config_get(NETParams, sizeof(NETParams) / sizeof(paramdef_t), gtpupath);
  char *cidr = NULL, *address = NULL;
  if (NETParams[1].strptr != NULL) {
    LOG_I(GTPU, "SA mode \n");
    address = strtok_r(gnb_ipv4_address_for_NGU, "/", &cidr);
    *port = gnb_port_for_NGU;
  } else {
    LOG_I(GTPU, "NSA mode \n");
    address = strtok_r(gnb_ipv4_address_for_S1U, "/", &cidr);
    *port = gnb_port_for_S1U;
  }
  if (address == NULL)
    return 1;
  else {
    strcpy(addr, address);
    return 0;
  }
}

instance_t RCconfig_nr_gtpu(void)
{
  char address[64];
  int port;
  int ret = get_NGU_S1U_addr(address, &port);
  instance_t ret_inst = 0;
  if (!ret) {
    eth_params_t IPaddr;
    IPaddr.my_addr = address;
    IPaddr.my_portd = port;
    openAddr_t tmp = {0};
    strcpy(tmp.originHost, IPaddr.my_addr);
    sprintf(tmp.originService, "%d", IPaddr.my_portd);
    strcpy(tmp.destinationService, tmp.originService);
    LOG_I(GTPU, "Configuring GTPu address : %s, port : %s\n", tmp.originHost, tmp.originService);
    ret_inst = gtpv1Init(tmp);
  } else
    LOG_E(GTPU, "invalid address for NGU or S1U\n");

  return ret_inst;
}
