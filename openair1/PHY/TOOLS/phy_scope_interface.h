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

/*! \file phy_scope_interface.h
 * \brief softscope interface API include file
 * \author Nokia BellLabs France, francois Taburet
 * \date 2019
 * \version 0.1
 * \company Nokia BellLabs France
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */
#ifndef __PHY_SCOPE_INTERFACE_H
#define __PHY_SCOPE_INTERFACE_H
#include <openair1/PHY/defs_gNB.h>
#include <openair1/PHY/defs_nr_UE.h>

typedef struct {
  // Uplink BLock Error Rate
  int idx_ULBLER;

  // Uplink MCS
  float UL_MCS_max;
  int idx_ULMCS;

  // Downlink BLock Error Rate
  int idx_DLBLER;

  // Downlink MCS
  float DL_MCS_max;
  int idx_DLMCS;

  // UL Throughput
  float UL_Throu_max;
  int idx_ULThrou;

  // DL Throughput
  float DL_Throu_max;
  int idx_DLThrou;

}extended_kpi_gNB;

typedef struct {
  int idx_BLER;
  float DL_BLER;

  int idx_throu;
  double throu;

  int idx_mcs;
  uint8_t dl_mcs;

  int idx_nofRBs;
  uint16_t nofRBs;

}extended_kpi_ue;


typedef struct {
  int *argc;
  char **argv;
  RU_t *ru;
  PHY_VARS_gNB *gNB;
} scopeParms_t;

enum UEdataType {
  pbchDlChEstimateTime,
  pdcchRxdataF_comp,
  pdcchLlr,
  pbchRxdataF_comp,
  pbchLlr,
  UEdataTypeNumberOfItems
};

typedef struct scopeData_s {
  int *argc;
  char **argv;
  RU_t *ru;
  PHY_VARS_gNB *gNB;
  void *liveData;
  void *liveDataUE;
  void (*slotFunc)(int32_t *data, int slot,  void *scopeData);
  void (*copyData)(PHY_VARS_NR_UE *,enum UEdataType, void *data, int elementSz, int colSz, int lineSz);
} scopeData_t;

int load_softscope(char *exectype, void *initarg);
int end_forms(void) ;

#define UEscopeCopy(ue, type, ...) if(ue->scopeData) ((scopeData_t*)ue->scopeData)->copyData(ue, type, ##__VA_ARGS__);

void getKPIUE(extended_kpi_ue* kpiStructure);

#endif
