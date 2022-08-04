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

#ifdef __cplusplus
extern "C" {
#endif

  int gtpv1u_create_s1u_tunnel(const instance_t instance, const gtpv1u_enb_create_tunnel_req_t  *create_tunnel_req,
                               gtpv1u_enb_create_tunnel_resp_t *create_tunnel_resp){
    return 0;
  }
  int gtpv1u_update_s1u_tunnel(const instance_t instanceP,
                               const gtpv1u_enb_create_tunnel_req_t   *create_tunnel_req_pP,
                               const rnti_t prior_rnti
                               ){
    return 0;
  }
  
  int gtpv1u_delete_s1u_tunnel( const instance_t instance, const gtpv1u_enb_delete_tunnel_req_t *const req_pP){
    return 0;
  }

  int gtpv1u_create_x2u_tunnel(const instance_t instanceP,
                               const gtpv1u_enb_create_x2u_tunnel_req_t   *const create_tunnel_req_pP,
                               gtpv1u_enb_create_x2u_tunnel_resp_t *const create_tunnel_resp_pP){
    return 0;
  }

  int gtpv1u_delete_x2u_tunnel( const instance_t instanceP,
                                const gtpv1u_enb_delete_tunnel_req_t *const req_pP){
    return 0;
  }
  int
  gtpv1u_create_ngu_tunnel(
                           const instance_t instanceP,
                           const gtpv1u_gnb_create_tunnel_req_t   *const create_tunnel_req_pP,
                           gtpv1u_gnb_create_tunnel_resp_t *const create_tunnel_resp_pP){
    return 0;
  }
  
  int gtpv1u_delete_ngu_tunnel( const instance_t instance,
                                gtpv1u_gnb_delete_tunnel_req_t *req) {
    return 0;
  }
  
  int gtpv1u_update_ngu_tunnel( const instance_t                              instanceP,
                                const gtpv1u_gnb_create_tunnel_req_t *const  create_tunnel_req_pP,
                                const ue_id_t                                  prior_rnti
                                ) {
    return 0;
  }

  // New API
  teid_t newGtpuCreateTunnel(instance_t instance, ue_id_t ue_id, int incoming_bearer_id, int outgoing_rb_id, teid_t teid,
                              int outgoing_qfi, transport_layer_addr_t remoteAddr, int port, gtpCallback callBack,
                             gtpCallbackSDAP callBackSDAP) {
    return 0;
  }
  void GtpuUpdateTunnelOutgoingTeid(instance_t instance, ue_id_t ue_id, ebi_t bearer_id, teid_t newOutgoingTeid) {
  }
  
  int newGtpuDeleteAllTunnels(instance_t instance, ue_id_t ue_id) { return 0;
  }
  int newGtpuDeleteTunnels(instance_t instance, ue_id_t ue_id, int nbTunnels, pdusessionid_t *pdusession_id) {
    return 0;
  }
  
#ifdef __cplusplus
}
#endif

