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

#ifndef PDU_SESSION_ESTABLISHMENT_ACCEPT_H_
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_H_

#include <stdint.h>

typedef struct qos_rule_s {
  uint8_t qos_rule_id;        /* QoS rule identifier */
  uint16_t qos_rule_length;   /* Length of QoS Rule */
  uint8_t rule_oc;            /* Rule operation code (3bits) */
  uint8_t dqr_bit;            /* DQR bit (1 bit) */
  uint8_t nb_packet_filters;  /* Number of packet filters (4 bits) (=m)*/
  uint8_t qos_rule_pr;        /* QoS rule precedence */
  uint8_t qfi;                /* QoS Flow Identifier */
} qos_rule_t; /* QoS Rule as defined in 24.501 Figure 9.11.4.13.2 */

typedef struct qos_rules_s {
  uint8_t qos_rules_IE;         /* QoS rules IEI */
  uint16_t qos_rules_IE_length; /* Length of QoS rules IE (=u)*/
  qos_rule_t qos_rule;          /* QoS rule */
} qos_rules_t; /* QoS Rules IE as defined in 24.501 9.11.4.13 */

typedef struct pdu_session_establishment_accept_msg_s {
  uint8_t epd;            /* Extended Protocol Discriminator */
  uint8_t pdu_id;         /* PDU Session ID */
  uint8_t pti;            /* Procedure Transaction Identity */
  uint8_t msg_id;         /* PDU Session Establishment Accept Message Identity */
  uint8_t pdu_type;       /* PDU Session Type (4 bits) */
  uint8_t ssc_mode;       /*  SSC Mode (4 bits) */
  qos_rules_t qos_rules;  /* Authorized QoS Rules */
} pdu_session_establishment_accept_msg_t; /* PDU Session Establishment Accept 24.501 Table 8.3.2.1.1 */

#endif