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

/* Mandatory Presence - START */

typedef struct packet_filter_component_s {
  uint8_t pf_comp_type; /* Packet Filter Component Type */
} packet_filter_component_t; /* TS 24.501 Figure 9.11.4.13.3 */

typedef struct packet_filter_create_qos_rule_s {
  uint8_t pf_dir; /* Packet filter direction */
  uint8_t pf_id;  /* Packet filter identifier */
  uint8_t length; /* Length of packet filter contents */
  packet_filter_component_t *pfc; 
} packet_filter_create_qos_rule_t; /* TS 24.501 Figure 9.11.4.13.4 */

typedef struct packet_filter_modify_qos_rule_s {
  uint8_t pf_id;  /* Packet filter identifier */
} packet_filter_modify_qos_rule_t;

typedef struct packet_filter_s {
  union pf_type {
    packet_filter_create_qos_rule_t create;
    packet_filter_modify_qos_rule_t modify;
  } pf_type;
} packet_filter_t;

typedef struct qos_rule_s {
  uint8_t  id;          /* QoS rule identifier */
  uint16_t length;      /* Length of QoS Rule */
  uint8_t  oc;          /* Rule operation code (3bits) */
  uint8_t  dqr;         /* DQR bit (1 bit) */
  uint8_t  nb_pf;       /* Number of packet filters (4 bits) */
  packet_filter_t *pf;  /* Packet filter list */
  uint8_t  prcd;        /* QoS rule precedence */
  uint8_t  qfi;         /* QoS Flow Identifier */
} qos_rule_t;

typedef struct auth_qos_rules_s {
  uint16_t length;      /* Length of QoS rules IE */
  qos_rule_t *qos_rule; /* QoS rule linked list */
} auth_qos_rule_t; /* QoS Rule as defined in 24.501 Figure 9.11.4.13.2 */

typedef struct session_ambr_s {
  uint8_t  length;  /* Length of Session-AMBR contents */
  uint8_t  unit_dl; /* Unit for Session-AMBR for downlink */
  uint16_t sess_dl; /* Session-AMBR for downlink */
  uint8_t  unit_ul; /* Unit for Session-AMBR for uplink */
  uint16_t sess_ul; /* Session-AMBR for uplink */
} session_ambr_t; /* TS 24.501 Figure 9.11.4.14.1 */

/* Mandatory Presence - END */

/* Optional Presence - START */

typedef struct pdu_address_s {
  uint8_t pdu_iei;    /* PDU Address IEI (0x29) */
  uint8_t pdu_length; /* Length of PDU address contents */
  uint8_t pdu_type;   /* PDU session type value */
  int     pdu_addr;   /* PDU address information */
} pdu_address_t; /* TS 24.501 9.11.4.10 */

typedef struct dnn_s {
  uint8_t dnn_iei;    /* DNN IEI (0x25) */
  uint8_t dnn_length; /* Length of DNN contents */
  uint8_t *dnn;       /* DNN Value */
} dnn_t; /* TS 24.501 9.11.2.1A */

/* Optional Presence - STOP */

typedef struct pdu_session_establishment_accept_msg_s {
  /* Mandatory presence */
  uint8_t epd;               /* Extended Protocol Discriminator */
  uint8_t pdu_id;            /* PDU Session ID */
  uint8_t pti;               /* Procedure Transaction Identity */
  uint8_t msg_type;          /* Message Type */
  uint8_t pdu_type;          /* PDU Session Type */
  uint8_t ssc_mode;          /* SSC mode */
  auth_qos_rule_t qos_rules; /* Authorized QoS rules */
  session_ambr_t  sess_ambr; /* Session-AMBR */
  /* Optional presence */

} pdu_session_establishment_accept_msg_t; /* 24.501 Table 8.3.2.1.1 */

typedef struct security_protected_plain_nas_5gs_msg_s {
  uint8_t  epd;           /* Extended Protocol Discriminator */
  uint8_t  sht;           /* Security Header Type */
  uint8_t  msg_type;      /* Message Type */
  uint8_t  payload_type;  /* Payload Container Type */
  uint16_t payload_len;   /* Payload Container Length */
} security_protected_plain_nas_5gs_msg_t; 

typedef struct security_protected_nas_5gs_msg_s {
  uint8_t  epd; /* Extended Protocol Discriminator */
  uint8_t  sht; /* Security Header Type */
  uint32_t mac; /* Message Authentication Code */
  uint8_t  sqn; /* Sequence Number */
} security_protected_nas_5gs_msg_t; /* 24.501 Figure 9.1.1.2 */

/* Function to process PDU SESSION ESTABLISHMENT ACCEPT message, so we can get the QFI and PDU Session ID from CN  */
void process_pdu_session_establishment_accept(uint8_t *buffer, uint32_t msg_length);

#endif