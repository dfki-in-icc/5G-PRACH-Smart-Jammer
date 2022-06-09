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
 *
 * File ss_eNB_proxy_iface.h
 * Brief: Implements the port manager handling for SS mode.
 * Author: Anurag Asokan, Manu Agrawal, Vaibhav Shrivastava, Vijay Chadachan
 * Company: Firecell
 * Email: anurag.ashokan@firecell.io, manu.agrawal@firecell.io, vaibhav.shrivastava@firecell.io,vijay.chadachan@firecell.io
 */

#ifndef _SS_ENB_PROXY_IFACE_
#define _SS_ENB_PROXY_IFACE_

typedef enum proxy_ss_message_id {
  SS_ATTN_LIST = 1,
  SS_ATTN_LIST_CNF = 2,
  SS_CELL_CONFIG = 3,
  SS_CELL_CONFIG_CNF = 4,
  SS_CELL_RELEASE = 5,
  SS_CELL_RELEASE_CNF = 6,
  SS_VNG_CMD_REQ = 7,
  SS_VNG_CMD_RESP = 8,
  SS_INVALID_MSG = 0xFF
} proxy_ss_msgs_e;

/*
 * Proxy SS Header: To be used for comunication between Proxy and SS-eNB.
 *
 * preamble: 0xFEEDC0DE for messages coming from SSeNB to Proxy
 *           0xF00DC0DE for messages coming from Proxy to SSeNB
 * msg_id  : To identify the message that is sent.
 * cell_id : Cell ID
 * length  : Length of the message in bytes.
 */
typedef struct proxy_ss_header_s {
  uint32_t preamble;
  proxy_ss_msgs_e  msg_id;
  uint8_t  cell_id;
  uint16_t length; /** NOTE: Not needed */
} proxy_ss_header_t,
 *proxy_ss_header_p;

typedef struct attenuationConfigReq_s
{
    proxy_ss_header_t header;
    uint8_t attnVal;
} attenuationConfigReq_t;

typedef struct attenuationConfigCnf_s
{
    proxy_ss_header_t header;
    uint8_t status;
} attenuationConfigCnf_t;

typedef struct CellConfigReq_s
{
    proxy_ss_header_t header;
    int16_t maxRefPower;
    uint8_t initialAttenuation;
    uint16_t dl_earfcn;
} CellConfigReq_t;

typedef struct CellConfigCnf_s
{
    proxy_ss_header_t header;
    uint8_t status;
} CellConfigCnf_t;

typedef struct VngCmdReq_s
{
    proxy_ss_header_t header;
    uint8_t bw;
    int32_t NocLevel;
    uint8_t cmd;
} VngCmdReq_t;

typedef struct VngCmdResp_s
{
    proxy_ss_header_t header;
    uint8_t status;
} VngCmdResp_t;


#endif /* _SS_ENB_PROXY_IFACE_ */
