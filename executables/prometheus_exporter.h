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
#ifndef PROMETHEUS_EXPORTER_H
#define PROMETHEUS_EXPORTER_H 

#include "common/utils/system.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_MAX_PROM_ELEMENTS  100
#define MAX_COMPLEX_ELM_SIZE   8192
#define NUM_MAX_COMPLEX_ELEMENTS  20

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef short int16_t;
typedef struct {
  char      metric_name[128];
  uint32_t  metric_value;
} metric_elm_t;

typedef struct {
  uint32_t  num_metrics;
  metric_elm_t  metric_element[NUM_MAX_PROM_ELEMENTS];
} prom_metrics_t;

typedef struct {
 // char* buf;
  int16_t* iq_data;
  char label[256];
  uint32_t size;
} complex_metric_elm_t;

typedef struct {
  complex_metric_elm_t complex_element[MAX_COMPLEX_ELM_SIZE];
} complex_metrics_t;

enum metrics_key{
  RX_OFFSET = 0, // (1)
  RX_TBS,
  UL_BITS,	
  PDCCH_LEVEL_0,
  PDCCH_LEVEL_1,
  DL_NUM_RB,
  DL_START_RB,  // (7)
  UL_NUM_RB,	
  UL_START_RB,
  DL_TBS,
  UL_TBS,  //  (11)
  DCI_1_1_RB,
  PDCP_UL_RB_ID,
  PDCP_UL_SDU_BUFF_SIZE,
  PDCP_DL_RB_ID,
  PDCP_DL_SDU_BUFF_SIZE,
  DL_MCS,
  UL_MCS,
  RESERVED_19,
  RESERVED_20,
  RESERVED_21,
  RESERVED_22,
  RESERVED_23,
  RESERVED_24,
  RESERVED_25,
  RESERVED_26,
  RESERVED_27,
  RESERVED_28,
  RESERVED_29,
  RESERVED_30,
  RESERVED_31,
  RESERVED_32,
  RESERVED_33,
  RESERVED_34,
  RESERVED_35,
  RESERVED_36,
  RESERVED_37,
  RESERVED_38,
  RESERVED_39,
  RESERVED_40,
};
#define NUM_PROM_ELEMENTS 26

enum complex_metrics_key{
  PBCH_IQ = 0,  // 1
  PBCH_CHEST,
  PDCCH_IQ,
  PDCCH_CHEST,  // 5
  RAR_IQ,
  RAR_CHEST,
  SIB_IQ,
  SIB_CHEST,
  SSS_IQ,
  PROMETHEUS,
  SETTINGS
};

#define NUM_COMPLEX_METRICS_ELEMENTS 1
#define NUM_METHOD 5
#define NUM_PROM_SETTINGS 7
#define NUM_REALTIME_SETTINGS 9

enum MetricSwitchID {
  // real time monitor
  pbch_iq = 0,  // 0
  pbch_ch,
  pdcch_iq,
  pdcch_ch,
  rar_iq,
  rar_ch,  // 5
  sib_iq,
  sib_ch,
  sss_iq
};

enum PromMetricSwitchID {
  // prometheus monitor
  pdcch_pow = 0,
  dl_freq_resource,  
  ul_freq_resource,
  dl_mcs,
  dl_tbs,
  ul_mcs,
  ul_tbs  // 6
};

extern uint8_t  Prom_switch_state[NUM_PROM_SETTINGS];
extern uint8_t  Realtime_switch_state[NUM_REALTIME_SETTINGS];

extern int RegisterMetric(uint32_t key, char* name, uint32_t value);
extern void init_PrometheusNodeExporter_thread(void* arg);
extern int RegisterComplexMetric(uint32_t key, char* label, int16_t* ptr, uint32_t size);
extern int Report_IQ_Metric(char* buf,complex_metric_elm_t* elm);
extern int MonitoringConfig();
extern int use_prometheus;
extern uint32_t prometheus_port;

#define PROM_METRICS(INDEX,NAME,VALUE)  \
  if (use_prometheus > 0){ \
    RegisterMetric(INDEX,NAME,VALUE);\
  }

#endif