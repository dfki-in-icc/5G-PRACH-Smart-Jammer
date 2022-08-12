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

#include "openair3/PDU/nr_ue_pdu.h"
#include "openair2/COMMON/platform_types.h"

typedef struct {
  /* PDU Layer Linked List, every element is a PDU Session*/
  nr_ue_pdu_t *llist;
} nr_ue_pdu_list;

/* Global struct with all the PDU Sessions */
static nr_ue_pdu_list pdus;

void nr_pdu_data_req(protocol_ctxt_t *ctxt_p,
                     const srb_flag_t srb_flag,
                     const rb_id_t rb_id,
                     const mui_t mui,
                     const confirm_t confirm,
                     const sdu_size_t sdu_buffer_size,
                     unsigned char *const sdu_buffer,
                     const pdcp_transmission_mode_t pt_mode,
                     const uint32_t *sourceL2Id,
                     const uint32_t *destinationL2Id,
                     const uint8_t qfi,
                     const bool dc,
                     const uint8_t pdusession_id) {
  LOG_D(PDU, "Sending data to SDAP with PDU Session ID [%u], QFI [%u] \n", pdusession_id, qfi);
  sdap_data_req(ctxt_p, SRB_FLAG_NO, rb_id, 0, 0, sdu_buffer_size, (unsigned char *)sdu_buffer,
                PDCP_TRANSMISSION_MODE_DATA, NULL, NULL, qfi, dc, pdusession_id);
  return;
}

nr_ue_pdu_t *nr_ue_pdu_get(uint8_t pdusession_id) {
  nr_ue_pdu_t *pdu;
  pdu = pdus.llist;

  if(pdu == NULL)
    return NULL;

  while((pdu->pdusession_id != pdusession_id && pdu->next_pdu != NULL))
    pdu = pdu->next_pdu;

  if(pdu->pdusession_id == pdusession_id)
    return pdu;

  return NULL;
}
