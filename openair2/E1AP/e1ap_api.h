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

#ifndef E1AP_API_H
#define E1AP_API_H

#include "e1ap.h"
#include "e1ap_common.h"
#include "NR_DRB-ToAddModList.h"

void gNB_CU_create_up_ul_tunnel(e1ap_bearer_setup_resp_t *resp,
                                e1ap_bearer_setup_req_t *req,
                                instance_t gtpInst,
                                ue_id_t ue_id,
                                int remote_port,
                                in_addr_t my_addr);

void update_UL_UP_tunnel_info(e1ap_bearer_setup_req_t *req, instance_t instance, ue_id_t ue_id);

void CUUP_process_e1_bearer_context_setup_req(e1ap_bearer_setup_req_t *req, instance_t instance);

void CUUP_process_bearer_context_mod_req(e1ap_bearer_setup_req_t *req, instance_t instance);

#endif
