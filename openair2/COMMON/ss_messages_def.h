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

MESSAGE_DEF(SS_GET_TIM_INFO,                  MESSAGE_PRIORITY_MED, ss_get_timinfo_t                      , ss_get_timinfo)
MESSAGE_DEF(SS_SET_TIM_INFO,                  MESSAGE_PRIORITY_MED, ss_set_timinfo_t                      , ss_set_timinfo)

MESSAGE_DEF(SS_RRC_PDU_REQ,                   MESSAGE_PRIORITY_MED, ss_rrc_pdu_req_t                      , ss_rrc_pdu_req)
MESSAGE_DEF(SS_RRC_PDU_IND,                   MESSAGE_PRIORITY_MED, ss_rrc_pdu_ind_t                      , ss_rrc_pdu_ind)
MESSAGE_DEF(SS_UPD_TIM_INFO,                  MESSAGE_PRIORITY_MED, ss_upd_timinfo_t                      , ss_upd_timinfo)
MESSAGE_DEF(SS_SYS_PORT_MSG_IND,              MESSAGE_PRIORITY_MED, ss_sys_port_msg_ind_t                 , ss_sys_port_msg_ind)
MESSAGE_DEF(SS_SYS_PORT_MSG_CNF,              MESSAGE_PRIORITY_MED, ss_sys_port_msg_cnf_t                 , ss_sys_port_msg_cnf)

MESSAGE_DEF(SS_GET_PDCP_CNT,                  MESSAGE_PRIORITY_MED, ss_get_pdcp_cnt_t                      , ss_get_pdcp_cnt)
MESSAGE_DEF(SS_SET_PDCP_CNT,                  MESSAGE_PRIORITY_MED, ss_set_pdcp_cnt_t                      , ss_set_pdcp_cnt)
MESSAGE_DEF(SS_REQ_PDCP_CNT,                  MESSAGE_PRIORITY_MED, ss_req_pdcp_cnt_t                      , ss_req_pdcp_cnt)

MESSAGE_DEF(SS_VNG_PROXY_REQ,                 MESSAGE_PRIORITY_MED, ss_vng_proxy_req_t                     , ss_vng_proxy_req)
MESSAGE_DEF(SS_VNG_PROXY_RESP,                MESSAGE_PRIORITY_MED, ss_vng_proxy_resp_t                    , ss_vng_proxy_resp)

MESSAGE_DEF(SS_SS_PAGING_IND,                  MESSAGE_PRIORITY_MED, ss_paging_ind_t                      , ss_paging_ind)
