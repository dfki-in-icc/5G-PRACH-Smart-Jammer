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

#ifndef SS_MESSAGES_TYPES_H_
#define SS_MESSAGES_TYPES_H_

#define SS_SYS_PORT_MSG_IND(mSGpTR)            (mSGpTR)->ittiMsg.ss_sys_port_msg_ind
#define SS_SYS_PORT_MSG_CNF(mSGpTR)            (mSGpTR)->ittiMsg.ss_sys_port_msg_cnf

#define SS_GET_TIM_INFO(mSGpTR)                (mSGpTR)->ittiMsg.ss_get_timinfo
#define SS_SET_TIM_INFO(mSGpTR)                (mSGpTR)->ittiMsg.ss_set_timinfo
#define SS_UPD_TIM_INFO(mSGpTR)                (mSGpTR)->ittiMsg.ss_upd_timinfo
#define SS_CELL_ATTN_LIST_IND(mSGpTR)          (mSGpTR)->ittiMsg.ss_cell_attn_list_ind
#define SS_CELL_ATTN_LIST_CNF(mSGpTR)          (mSGpTR)->ittiMsg.ss_cell_attn_list_cnf

/** PDCP Count */
#define SS_REQ_PDCP_CNT(mSGpTR)                (mSGpTR)->ittiMsg.ss_req_pdcp_cnt
#define SS_GET_PDCP_CNT(mSGpTR)                (mSGpTR)->ittiMsg.ss_get_pdcp_cnt
#define SS_SET_PDCP_CNT(mSGpTR)                (mSGpTR)->ittiMsg.ss_set_pdcp_cnt

#define SS_RRC_PDU_REQ(mSGpTR)                (mSGpTR)->ittiMsg.ss_rrc_pdu_req
#define SS_RRC_PDU_IND(mSGpTR)                (mSGpTR)->ittiMsg.ss_rrc_pdu_ind
#define SS_SYS_PROXY_MSG_CNF(mSGpTR)          (mSGpTR)->ittiMsg.udp_data_ind
#define SS_PAGING_IND(mSGpTR)                 (mSGpTR)->ittiMsg.ss_paging_ind 


/** VNG */
#define SS_VNG_PROXY_REQ(mSGpTR)              (mSGpTR)->ittiMsg.ss_vng_proxy_req
#define SS_VNG_PROXY_RESP(mSGpTR)             (mSGpTR)->ittiMsg.ss_vng_proxy_resp

#define SDU_SIZE                           (512)

/** PORTMAN */
typedef struct ss_sys_port_msg_ind {
  struct SYSTEM_CTRL_REQ* req;
  int userId;
} ss_sys_port_msg_ind_t;

typedef struct ss_sys_port_msg_cnf {
  struct SYSTEM_CTRL_CNF* cnf;
} ss_sys_port_msg_cnf_t;

/** SYS */
typedef struct ss_set_timinfo_s {
  uint16_t sfn;
  uint8_t  sf;
} ss_set_timinfo_t;

typedef ss_set_timinfo_t ss_upd_timinfo_t;

typedef struct ss_get_timinfo_s {
  uint8_t  EnquireTiming;
} ss_get_timinfo_t;

typedef struct ss_cell_attn_list_ind {
  uint16_t cell_id;
  uint8_t attn;               /*!< \brief 0xFF -> Off */
  ss_set_timinfo_t time_info; /*!< \brief Optional.*/
} ss_cell_attn_list_ind_t;

typedef struct ss_cell_attn_list_cnf {
  uint8_t status;
} ss_cell_attn_list_cnf_t;

enum PdcpCountFormat_Type_e {
        E_PdcpCount_Srb = 0,
        E_PdcpCount_DrbLongSQN = 1,
        E_PdcpCount_DrbShortSQN = 2,
        E_NrPdcpCount_Srb = 3,
        E_NrPdcpCount_DrbSQN12 = 4,
        E_NrPdcpCount_DrbSQN18 = 5,
};

typedef enum PdcpCountFormat_Type_e PdcpCountFormat_Type_e;

typedef struct pdcp_count_rb_s {
  uint8_t rb_id;
  uint8_t is_srb;
  PdcpCountFormat_Type_e ul_format;
  PdcpCountFormat_Type_e dl_format;
  uint32_t ul_count;
  uint32_t dl_count;
} pdcp_count_rb_t;

typedef struct ss_set_pdcp_cnt_s {
  pdcp_count_rb_t rb_list[MAX_RBS];
} ss_set_pdcp_cnt_t;

typedef struct ss_get_pdcp_cnt_s {
  //struct PdcpCountInfo_Type_Get_Dynamic Get;
  uint32_t size;
  pdcp_count_rb_t rb_info[MAX_RBS];
} ss_get_pdcp_cnt_t;

typedef struct ss_req_pdcp_cnt_s {
  rnti_t rnti;
  uint8_t rb_id;
} ss_req_pdcp_cnt_t;

/** SRB */
typedef struct ss_rrc_pdu_req_s {
  uint8_t   srb_id;
  uint32_t  sdu_size;
  uint8_t   sdu[SDU_SIZE];
  uint16_t  rnti;
} ss_rrc_pdu_req_t;

typedef struct ss_rrc_pdu_ind_s {
  uint8_t   srb_id;
  uint32_t  sdu_size;
  uint8_t   sdu[SDU_SIZE];
  uint16_t  rnti;
  frame_t     frame;         /*!< \brief  LTE frame number.*/
  sub_frame_t subframe;      /*!< \brief  LTE sub frame number.*/
} ss_rrc_pdu_ind_t;

/** VNG */

typedef struct ss_vng_proxy_resp_s {
  uint8_t     cell_id; /** Cell_id of the cell for 
                           which VNG request came */
  uint32_t    sfn_sf;  /** Time at which response was 
                           received from Proxy in the SYS task */
  uint8_t     status;  /** 0 Success: 1 Failure */
} ss_vng_proxy_resp_t;

typedef enum carrierBandwidthEUTRA_dl_Bandwidth_e {
        carrierBandwidthEUTRA_dl_Bandwidth_e_n6 = 0,
        carrierBandwidthEUTRA_dl_Bandwidth_e_n15 = 1,
        carrierBandwidthEUTRA_dl_Bandwidth_e_n25 = 2,
        carrierBandwidthEUTRA_dl_Bandwidth_e_n50 = 3,
        carrierBandwidthEUTRA_dl_Bandwidth_e_n75 = 4,
        carrierBandwidthEUTRA_dl_Bandwidth_e_n100 = 5,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare10 = 6,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare9 = 7,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare8 = 8,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare7 = 9,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare6 = 10,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare5 = 11,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare4 = 12,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare3 = 13,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare2 = 14,
        carrierBandwidthEUTRA_dl_Bandwidth_e_spare1 = 15,
        carrierBandwidthEUTRA_dl_Bandwidth_e_NONE = 16,
        carrierBandwidthEUTRA_dl_Bandwidth_e_INVALID = 0xFF
} Dl_Bw_e;

typedef enum VngCmd_e {
  INVALID = 0,
  CONFIGURE = 1,
  ACTIVATE,
  DEACTIVATE
} VngCmd;

typedef struct ss_vng_proxy_req_s {
  uint16_t    cell_id;    /** PCI of the cell for 
                           which VNG request came */
  Dl_Bw_e     bw;         /** DL Bandwidth enum (ASN1) */
  int32_t     Noc_level;  /** 0 Success: 1 Failure */
  VngCmd      cmd;        /** CONF, ACTV, DEACTV */
} ss_vng_proxy_req_t;


typedef struct ss_paging_identity_s {
  /* UE identity index value.
   * Specified in 3GPP TS 36.304
   */
  unsigned ue_index_value:10;

  /* UE paging identity */
  ue_paging_identity_t ue_paging_identity;

  /* Indicates origin of paging */
  cn_domain_t cn_domain;
}ss_paging_identity_t;
typedef struct ss_paging_ind_s {
  uint16_t sfn;
  uint8_t  sf;
  ss_paging_identity_t *paging_recordList;
} ss_paging_ind_t;
#endif /* SS_MESSAGES_TYPES_H_ */
