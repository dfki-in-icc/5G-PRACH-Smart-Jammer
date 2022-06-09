/*
 * Copyright 2022 Sequans Communications.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "SidlCompiler.h"
#include "SidlASN1.h"
#include "SidlASN1Base.h"
#include "SidlBase.h"
#include "SidlCommon.h"
#include "SidlCommonBase.h"

SIDL_BEGIN_C_INTERFACE

typedef uint8_t AntennaPortId_Type;

typedef int8_t ToRS_EPRE_Ratio_Type;

typedef uint8_t CfiValue_Type;

typedef struct MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12 DMTC_PeriodOffset_r12_Type;

typedef struct MeasDS_Config_r12_setup_ds_OccasionDuration_r12 DS_OccasionDuration_r12_Type;

enum AntennaInfoCommon_Type_Sel {
	AntennaInfoCommon_Type_UNBOUND_VALUE = 0,
	AntennaInfoCommon_Type_R8 = 1,
};

union AntennaInfoCommon_Type_Value {
	struct AntennaInfoCommon R8;
};

struct AntennaInfoCommon_Type {
	enum AntennaInfoCommon_Type_Sel d;
	union AntennaInfoCommon_Type_Value v;
};

struct AntennaPortInfo_Type {
	Dummy_Type PowerAttenuation;
	Dummy_Type PropagationDelay;
};

enum AntennaPortConfig_Type_Sel {
	AntennaPortConfig_Type_UNBOUND_VALUE = 0,
	AntennaPortConfig_Type_AddOrReconfigure = 1,
	AntennaPortConfig_Type_Release = 2,
};

union AntennaPortConfig_Type_Value {
	struct AntennaPortInfo_Type AddOrReconfigure;
	Null_Type Release;
};

struct AntennaPortConfig_Type {
	enum AntennaPortConfig_Type_Sel d;
	union AntennaPortConfig_Type_Value v;
};

struct AntennaPort_Type {
	AntennaPortId_Type Id;
	struct AntennaPortConfig_Type Config;
};

struct AntennaPort_Type_AntennaPort_Dynamic {
	size_t d;
	struct AntennaPort_Type* v;
};

struct DownlinkAntennaGroupConfig_Type {
	struct AntennaInfoCommon_Type AntennaInfoCommon;
	struct AntennaPort_Type_AntennaPort_Dynamic AntennaPort;
};

struct ToRS_EPRE_Ratio_Type_RA_Optional {
	bool d;
	ToRS_EPRE_Ratio_Type v;
};

struct ToRS_EPRE_Ratio_Type_RB_Optional {
	bool d;
	ToRS_EPRE_Ratio_Type v;
};

struct ToRS_EPRE_Ratios_Type {
	struct ToRS_EPRE_Ratio_Type_RA_Optional RA;
	struct ToRS_EPRE_Ratio_Type_RB_Optional RB;
};

struct ToRS_EPRE_Ratios_Type_PbchConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PbchConfig_Type {
	struct ToRS_EPRE_Ratios_Type_PbchConfig_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct CfiValue_Type_CfiValue_Optional {
	bool d;
	CfiValue_Type v;
};

struct ToRS_EPRE_Ratios_Type_PcfichConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PcfichConfig_Type {
	struct CfiValue_Type_CfiValue_Optional CfiValue;
	struct ToRS_EPRE_Ratios_Type_PcfichConfig_Type_RelativeTxPower_Optional RelativeTxPower;
};

enum PHICH_Config_Type_Sel {
	PHICH_Config_Type_UNBOUND_VALUE = 0,
	PHICH_Config_Type_R8 = 1,
};

union PHICH_Config_Type_Value {
	struct PHICH_Config R8;
};

struct PHICH_Config_Type {
	enum PHICH_Config_Type_Sel d;
	union PHICH_Config_Type_Value v;
};

struct PHICH_Config_Type_PhichConfig_Optional {
	bool d;
	struct PHICH_Config_Type v;
};

struct ToRS_EPRE_Ratios_Type_PhichConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PhichConfig_Type {
	struct PHICH_Config_Type_PhichConfig_Optional PhichConfig;
	struct ToRS_EPRE_Ratios_Type_PhichConfig_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct CCE_StartIndex_DL_UL_Type {
	int32_t CCE_StartIndex_DL;
	int32_t CCE_StartIndex_UL;
};

struct CCE_StartIndex_DL_UL_Type_CCE_StartIndexList_Type_Dynamic {
	size_t d;
	struct CCE_StartIndex_DL_UL_Type* v;
};

typedef struct CCE_StartIndex_DL_UL_Type_CCE_StartIndexList_Type_Dynamic CCE_StartIndexList_Type;

struct PdcchCandidate_Type {
	C_RNTI RNTI;
	CCE_StartIndexList_Type CCE_StartIndexList;
};

struct PdcchCandidate_Type_PdcchCandidateList_Type_Dynamic {
	size_t d;
	struct PdcchCandidate_Type* v;
};

typedef struct PdcchCandidate_Type_PdcchCandidateList_Type_Dynamic PdcchCandidateList_Type;

struct uint8_t_CommonSearchSpaceFormat_Optional {
	bool d;
	uint8_t v;
};

struct uint8_t_UeSpecificSearchSpaceFormat_Optional {
	bool d;
	uint8_t v;
};

struct PdcchCandidateList_Type_PdcchCandidateList_Optional {
	bool d;
	PdcchCandidateList_Type v;
};

struct ToRS_EPRE_Ratios_Type_PdcchConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PdcchConfig_Type {
	struct uint8_t_CommonSearchSpaceFormat_Optional CommonSearchSpaceFormat;
	struct uint8_t_UeSpecificSearchSpaceFormat_Optional UeSpecificSearchSpaceFormat;
	struct PdcchCandidateList_Type_PdcchCandidateList_Optional PdcchCandidateList;
	struct ToRS_EPRE_Ratios_Type_PdcchConfig_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct ToRS_EPRE_Ratios_Type_RachResponse_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct ToRS_EPRE_Ratios_Type_BcchOnPdsch_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct ToRS_EPRE_Ratios_Type_PcchOnPdsch_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct ToRS_EPRE_Ratios_Type_CcchOnPdsch_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct ToRS_EPRE_Ratios_Type_DcchDtchOnPdsch_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct ToRS_EPRE_Ratios_Type_OcngOnPdsch_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PdschRelativeTxPower_Type {
	struct ToRS_EPRE_Ratios_Type_RachResponse_Optional RachResponse;
	struct ToRS_EPRE_Ratios_Type_BcchOnPdsch_Optional BcchOnPdsch;
	struct ToRS_EPRE_Ratios_Type_PcchOnPdsch_Optional PcchOnPdsch;
	struct ToRS_EPRE_Ratios_Type_CcchOnPdsch_Optional CcchOnPdsch;
	struct ToRS_EPRE_Ratios_Type_DcchDtchOnPdsch_Optional DcchDtchOnPdsch;
	struct ToRS_EPRE_Ratios_Type_OcngOnPdsch_Optional OcngOnPdsch;
};

struct PdschRelativeTxPower_Type_RelativeTxPower_Optional {
	bool d;
	struct PdschRelativeTxPower_Type v;
};

struct PDSCH_ConfigCommon_v1310_PDSCH_ConfigCommon_Optional {
	bool d;
	struct PDSCH_ConfigCommon_v1310 v;
};

struct PdschConfig_Type {
	struct PdschRelativeTxPower_Type_RelativeTxPower_Optional RelativeTxPower;
	struct PDSCH_ConfigCommon_v1310_PDSCH_ConfigCommon_Optional PDSCH_ConfigCommon;
};

struct ToRS_EPRE_Ratios_Type_PrimarySyncSignal_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PrimarySyncSignal_Type {
	struct ToRS_EPRE_Ratios_Type_PrimarySyncSignal_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct ToRS_EPRE_Ratios_Type_SecondarySyncSignal_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct SecondarySyncSignal_Type {
	struct ToRS_EPRE_Ratios_Type_SecondarySyncSignal_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct CSI_RS_Config_r10_CSI_RS_Config_Optional {
	bool d;
	struct CSI_RS_Config_r10 v;
};

struct CSI_RS_ConfigNZPToAddModList_r11_CSI_RS_ConfigNZPToAddModList_Optional {
	bool d;
	CSI_RS_ConfigNZPToAddModList_r11 v;
};

struct CSI_RS_ConfigNZPToReleaseList_r11_CSI_RS_ConfigNZPToReleaseModList_Optional {
	bool d;
	CSI_RS_ConfigNZPToReleaseList_r11 v;
};

struct CSI_RS_ConfigZPToAddModList_r11_CSI_RS_ConfigZPToAddModList_Optional {
	bool d;
	CSI_RS_ConfigZPToAddModList_r11 v;
};

struct CSI_RS_ConfigZPToReleaseList_r11_CSI_RS_ConfigZPToReleaseModList_Optional {
	bool d;
	CSI_RS_ConfigZPToReleaseList_r11 v;
};

struct ToRS_EPRE_Ratios_Type_CSI_RS_Config_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct CSI_RS_Config_Type {
	struct CSI_RS_Config_r10_CSI_RS_Config_Optional CSI_RS_Config;
	struct CSI_RS_ConfigNZPToAddModList_r11_CSI_RS_ConfigNZPToAddModList_Optional CSI_RS_ConfigNZPToAddModList;
	struct CSI_RS_ConfigNZPToReleaseList_r11_CSI_RS_ConfigNZPToReleaseModList_Optional CSI_RS_ConfigNZPToReleaseModList;
	struct CSI_RS_ConfigZPToAddModList_r11_CSI_RS_ConfigZPToAddModList_Optional CSI_RS_ConfigZPToAddModList;
	struct CSI_RS_ConfigZPToReleaseList_r11_CSI_RS_ConfigZPToReleaseModList_Optional CSI_RS_ConfigZPToReleaseModList;
	struct ToRS_EPRE_Ratios_Type_CSI_RS_Config_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct ToRS_EPRE_Ratios_Type_PmchConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PmchConfig_Type {
	struct ToRS_EPRE_Ratios_Type_PmchConfig_Type_RelativeTxPower_Optional RelativeTxPower;
};

struct ToRS_EPRE_Ratios_Type_EpdcchConfig_Type_RelativeTxPower_Optional {
	bool d;
	struct ToRS_EPRE_Ratios_Type v;
};

struct PdcchCandidateList_Type_ePdcchCandidateList_Optional {
	bool d;
	PdcchCandidateList_Type v;
};

struct EPDCCH_Config_r11_EPDCCH_Config_Optional {
	bool d;
	struct EPDCCH_Config_r11 v;
};

struct FreqHoppingParameters_r13_mPDCCH_FreqHoppingParameters_r13_Optional {
	bool d;
	struct FreqHoppingParameters_r13 v;
};

struct EpdcchConfig_Type {
	struct ToRS_EPRE_Ratios_Type_EpdcchConfig_Type_RelativeTxPower_Optional RelativeTxPower;
	struct PdcchCandidateList_Type_ePdcchCandidateList_Optional ePdcchCandidateList;
	struct EPDCCH_Config_r11_EPDCCH_Config_Optional EPDCCH_Config;
	struct FreqHoppingParameters_r13_mPDCCH_FreqHoppingParameters_r13_Optional mPDCCH_FreqHoppingParameters_r13;
};

struct DMTC_PeriodOffset_r12_Type_DMTC_PeriodOffset_Optional {
	bool d;
	DMTC_PeriodOffset_r12_Type v;
};

struct DS_OccasionDuration_r12_Type_DS_OccasionDuration_r12_Optional {
	bool d;
	DS_OccasionDuration_r12_Type v;
};

struct DS_RS_Config_Type {
	struct DMTC_PeriodOffset_r12_Type_DMTC_PeriodOffset_Optional DMTC_PeriodOffset;
	struct DS_OccasionDuration_r12_Type_DS_OccasionDuration_r12_Optional DS_OccasionDuration_r12;
};

struct DownlinkAntennaGroupConfig_Type_AntennaGroup_Optional {
	bool d;
	struct DownlinkAntennaGroupConfig_Type v;
};

struct PbchConfig_Type_Pbch_Optional {
	bool d;
	struct PbchConfig_Type v;
};

struct PcfichConfig_Type_Pcfich_Optional {
	bool d;
	struct PcfichConfig_Type v;
};

struct PhichConfig_Type_Phich_Optional {
	bool d;
	struct PhichConfig_Type v;
};

struct PdcchConfig_Type_Pdcch_Optional {
	bool d;
	struct PdcchConfig_Type v;
};

struct PdschConfig_Type_Pdsch_Optional {
	bool d;
	struct PdschConfig_Type v;
};

struct PrimarySyncSignal_Type_Pss_Optional {
	bool d;
	struct PrimarySyncSignal_Type v;
};

struct SecondarySyncSignal_Type_Sss_Optional {
	bool d;
	struct SecondarySyncSignal_Type v;
};

struct CSI_RS_Config_Type_CSI_RS_Config_Optional {
	bool d;
	struct CSI_RS_Config_Type v;
};

struct PmchConfig_Type_Pmch_Optional {
	bool d;
	struct PmchConfig_Type v;
};

struct EpdcchConfig_Type_Epdcch_Optional {
	bool d;
	struct EpdcchConfig_Type v;
};

struct DS_RS_Config_Type_DS_RS_Config_Optional {
	bool d;
	struct DS_RS_Config_Type v;
};

struct PhysicalLayerConfigDL_Type {
	struct DownlinkAntennaGroupConfig_Type_AntennaGroup_Optional AntennaGroup;
	struct PbchConfig_Type_Pbch_Optional Pbch;
	struct PcfichConfig_Type_Pcfich_Optional Pcfich;
	struct PhichConfig_Type_Phich_Optional Phich;
	struct PdcchConfig_Type_Pdcch_Optional Pdcch;
	struct PdschConfig_Type_Pdsch_Optional Pdsch;
	struct PrimarySyncSignal_Type_Pss_Optional Pss;
	struct SecondarySyncSignal_Type_Sss_Optional Sss;
	struct CSI_RS_Config_Type_CSI_RS_Config_Optional CSI_RS_Config;
	struct PmchConfig_Type_Pmch_Optional Pmch;
	struct EpdcchConfig_Type_Epdcch_Optional Epdcch;
	struct DS_RS_Config_Type_DS_RS_Config_Optional DS_RS_Config;
};

SIDL_END_C_INTERFACE
