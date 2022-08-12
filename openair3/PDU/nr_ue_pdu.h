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

#ifndef NR_UE_PDU_H_
#define NR_UE_PDU_H_

#define NULLQFI (0)

#include "openair2/SDAP/nr_sdap/nr_sdap.h"

typedef struct nr_ue_pdu_s {
  uint8_t qfi[MAX_QFI];
  uint8_t pdusession_id;
  uint8_t default_qfi;

  struct nr_ue_pdu_s *next_pdu;
} nr_ue_pdu_t;

/* Send data to PDU Layer */
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
                     const uint8_t pdusession_id);

/* Create new PDU Session */
void nr_ue_pdu_new(uint8_t qfi, uint8_t pdusession_id);
/* Get PDU entity using pdusession_id */
nr_ue_pdu_t *nr_ue_pdu_get(uint8_t pdusession_id);
/* Returns the default PDU Session (1st PDU Session in the Linked List) */
nr_ue_pdu_t *nr_ue_pdu_get_default(void);
/* Function to add QFI to PDU Session */
void nr_ue_pdu_qfi_add(nr_ue_pdu_t *pdu, uint8_t qfi);

#endif