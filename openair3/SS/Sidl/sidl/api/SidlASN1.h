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
#include "SidlASN1Base.h"
#include "SidlBase.h"

SIDL_BEGIN_C_INTERFACE

typedef BIT_STRING_ELEMENT B2[2];

typedef BIT_STRING_ELEMENT B3[3];

typedef BIT_STRING_ELEMENT B4[4];

typedef BIT_STRING_ELEMENT B5[5];

typedef BIT_STRING_ELEMENT B6[6];

typedef BIT_STRING_ELEMENT B7[7];

typedef BIT_STRING_ELEMENT B8[8];

typedef BIT_STRING_ELEMENT B10[10];

typedef BIT_STRING_ELEMENT B12[12];

typedef BIT_STRING_ELEMENT B13[13];

typedef BIT_STRING_ELEMENT B15[15];

typedef BIT_STRING_ELEMENT B16[16];

typedef BIT_STRING_ELEMENT B17[17];

typedef BIT_STRING_ELEMENT B19[19];

typedef BIT_STRING_ELEMENT B20[20];

typedef BIT_STRING_ELEMENT B24[24];

typedef BIT_STRING_ELEMENT B25[25];

typedef BIT_STRING_ELEMENT B27[27];

typedef BIT_STRING_ELEMENT B28[28];

typedef BIT_STRING_ELEMENT B30[30];

typedef BIT_STRING_ELEMENT B32[32];

typedef BIT_STRING_ELEMENT B39[39];

typedef BIT_STRING_ELEMENT B40[40];

typedef BIT_STRING_ELEMENT B42[42];

typedef BIT_STRING_ELEMENT B48[48];

typedef BIT_STRING_ELEMENT B49[49];

typedef BIT_STRING_ELEMENT B50[50];

typedef BIT_STRING_ELEMENT B60[60];

typedef BIT_STRING_ELEMENT B64[64];

typedef BIT_STRING_ELEMENT B70[70];

typedef BIT_STRING_ELEMENT B80[80];

typedef BIT_STRING_ELEMENT B100[100];

typedef OCTET_STRING_ELEMENT O1[1];

typedef OCTET_STRING_ELEMENT O2[2];

typedef OCTET_STRING_ELEMENT O3[3];

typedef OCTET_STRING_ELEMENT O6[6];

typedef OCTET_STRING_ELEMENT O50[50];

enum AntennaInfoCommon_antennaPortsCount_e {
	AntennaInfoCommon_antennaPortsCount_e_an1 = 0,
	AntennaInfoCommon_antennaPortsCount_e_an2 = 1,
	AntennaInfoCommon_antennaPortsCount_e_an4 = 2,
	AntennaInfoCommon_antennaPortsCount_e_spare1 = 3,
};

typedef enum AntennaInfoCommon_antennaPortsCount_e AntennaInfoCommon_antennaPortsCount_e;

struct AntennaInfoCommon {
	AntennaInfoCommon_antennaPortsCount_e antennaPortsCount;
};

enum AntennaInfoDedicated_transmissionMode_e {
	AntennaInfoDedicated_transmissionMode_e_tm1 = 0,
	AntennaInfoDedicated_transmissionMode_e_tm2 = 1,
	AntennaInfoDedicated_transmissionMode_e_tm3 = 2,
	AntennaInfoDedicated_transmissionMode_e_tm4 = 3,
	AntennaInfoDedicated_transmissionMode_e_tm5 = 4,
	AntennaInfoDedicated_transmissionMode_e_tm6 = 5,
	AntennaInfoDedicated_transmissionMode_e_tm7 = 6,
	AntennaInfoDedicated_transmissionMode_e_tm8_v920 = 7,
};

typedef enum AntennaInfoDedicated_transmissionMode_e AntennaInfoDedicated_transmissionMode_e;

enum AntennaInfoDedicated_codebookSubsetRestriction_Sel {
	AntennaInfoDedicated_codebookSubsetRestriction_UNBOUND_VALUE = 0,
	AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm3 = 1,
	AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm3 = 2,
	AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm4 = 3,
	AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm4 = 4,
	AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm5 = 5,
	AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm5 = 6,
	AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm6 = 7,
	AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm6 = 8,
};

union AntennaInfoDedicated_codebookSubsetRestriction_Value {
	B2 n2TxAntenna_tm3;
	B4 n4TxAntenna_tm3;
	B6 n2TxAntenna_tm4;
	B64 n4TxAntenna_tm4;
	B4 n2TxAntenna_tm5;
	B16 n4TxAntenna_tm5;
	B4 n2TxAntenna_tm6;
	B16 n4TxAntenna_tm6;
};

struct AntennaInfoDedicated_codebookSubsetRestriction {
	enum AntennaInfoDedicated_codebookSubsetRestriction_Sel d;
	union AntennaInfoDedicated_codebookSubsetRestriction_Value v;
};

enum AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e {
	AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e_closedLoop = 0,
	AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e_openLoop = 1,
};

typedef enum AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e;

enum AntennaInfoDedicated_ue_TransmitAntennaSelection_Sel {
	AntennaInfoDedicated_ue_TransmitAntennaSelection_UNBOUND_VALUE = 0,
	AntennaInfoDedicated_ue_TransmitAntennaSelection_release = 1,
	AntennaInfoDedicated_ue_TransmitAntennaSelection_setup = 2,
};

union AntennaInfoDedicated_ue_TransmitAntennaSelection_Value {
	Null release;
	AntennaInfoDedicated_ue_TransmitAntennaSelection_setup_e setup;
};

struct AntennaInfoDedicated_ue_TransmitAntennaSelection {
	enum AntennaInfoDedicated_ue_TransmitAntennaSelection_Sel d;
	union AntennaInfoDedicated_ue_TransmitAntennaSelection_Value v;
};

struct AntennaInfoDedicated_codebookSubsetRestriction_codebookSubsetRestriction_Optional {
	bool d;
	struct AntennaInfoDedicated_codebookSubsetRestriction v;
};

struct AntennaInfoDedicated {
	AntennaInfoDedicated_transmissionMode_e transmissionMode;
	struct AntennaInfoDedicated_codebookSubsetRestriction_codebookSubsetRestriction_Optional codebookSubsetRestriction;
	struct AntennaInfoDedicated_ue_TransmitAntennaSelection ue_TransmitAntennaSelection;
};

enum AntennaInfoDedicated_r10_transmissionMode_r10_e {
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm1 = 0,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm2 = 1,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm3 = 2,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm4 = 3,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm5 = 4,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm6 = 5,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm7 = 6,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm8_v920 = 7,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm9_v1020 = 8,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_tm10_v1130 = 9,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare6 = 10,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare5 = 11,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare4 = 12,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare3 = 13,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare2 = 14,
	AntennaInfoDedicated_r10_transmissionMode_r10_e_spare1 = 15,
};

typedef enum AntennaInfoDedicated_r10_transmissionMode_r10_e AntennaInfoDedicated_r10_transmissionMode_r10_e;

enum AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e {
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e_closedLoop = 0,
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e_openLoop = 1,
};

typedef enum AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e;

enum AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_Sel {
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_UNBOUND_VALUE = 0,
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_release = 1,
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup = 2,
};

union AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_Value {
	Null release;
	AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup_e setup;
};

struct AntennaInfoDedicated_r10_ue_TransmitAntennaSelection {
	enum AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_Sel d;
	union AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_Value v;
};

struct BIT_STRING_AntennaInfoDedicated_r10_codebookSubsetRestriction_r10_Optional {
	bool d;
	BIT_STRING v;
};

struct AntennaInfoDedicated_r10 {
	AntennaInfoDedicated_r10_transmissionMode_r10_e transmissionMode_r10;
	struct BIT_STRING_AntennaInfoDedicated_r10_codebookSubsetRestriction_r10_Optional codebookSubsetRestriction_r10;
	struct AntennaInfoDedicated_r10_ue_TransmitAntennaSelection ue_TransmitAntennaSelection;
};

enum AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e {
	AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e_twoLayers = 0,
	AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e_fourLayers = 1,
	AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e_eightLayers = 2,
};

typedef enum AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e;

struct AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e_maxLayersMIMO_r10_Optional {
	bool d;
	AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e v;
};

struct AntennaInfoDedicated_v10i0 {
	struct AntennaInfoDedicated_v10i0_maxLayersMIMO_r10_e_maxLayersMIMO_r10_Optional maxLayersMIMO_r10;
};

struct AntennaInfoDedicated_v1250 {
	bool alternativeCodebookEnabledFor4TX_r12;
};

enum AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e {
	AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e_on = 0,
};

typedef enum AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e;

struct AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e_ce_UE_TxAntennaSelection_config_r14_Optional {
	bool d;
	AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e v;
};

struct AntennaInfoDedicated_v1430 {
	struct AntennaInfoDedicated_v1430_ce_UE_TxAntennaSelection_config_r14_e_ce_UE_TxAntennaSelection_config_r14_Optional ce_UE_TxAntennaSelection_config_r14;
};

enum AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_Sel {
	AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_UNBOUND_VALUE = 0,
	AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_n2TxAntenna_tm8_r9 = 1,
	AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_n4TxAntenna_tm8_r9 = 2,
};

union AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_Value {
	B6 n2TxAntenna_tm8_r9;
	B32 n4TxAntenna_tm8_r9;
};

struct AntennaInfoDedicated_v920_codebookSubsetRestriction_v920 {
	enum AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_Sel d;
	union AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_Value v;
};

struct AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_codebookSubsetRestriction_v920_Optional {
	bool d;
	struct AntennaInfoDedicated_v920_codebookSubsetRestriction_v920 v;
};

struct AntennaInfoDedicated_v920 {
	struct AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_codebookSubsetRestriction_v920_Optional codebookSubsetRestriction_v920;
};

enum AntennaInfoUL_r10_transmissionModeUL_r10_e {
	AntennaInfoUL_r10_transmissionModeUL_r10_e_tm1 = 0,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_tm2 = 1,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare6 = 2,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare5 = 3,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare4 = 4,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare3 = 5,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare2 = 6,
	AntennaInfoUL_r10_transmissionModeUL_r10_e_spare1 = 7,
};

typedef enum AntennaInfoUL_r10_transmissionModeUL_r10_e AntennaInfoUL_r10_transmissionModeUL_r10_e;

enum AntennaInfoUL_r10_fourAntennaPortActivated_r10_e {
	AntennaInfoUL_r10_fourAntennaPortActivated_r10_e_setup = 0,
};

typedef enum AntennaInfoUL_r10_fourAntennaPortActivated_r10_e AntennaInfoUL_r10_fourAntennaPortActivated_r10_e;

struct AntennaInfoUL_r10_transmissionModeUL_r10_e_transmissionModeUL_r10_Optional {
	bool d;
	AntennaInfoUL_r10_transmissionModeUL_r10_e v;
};

struct AntennaInfoUL_r10_fourAntennaPortActivated_r10_e_fourAntennaPortActivated_r10_Optional {
	bool d;
	AntennaInfoUL_r10_fourAntennaPortActivated_r10_e v;
};

struct AntennaInfoUL_r10 {
	struct AntennaInfoUL_r10_transmissionModeUL_r10_e_transmissionModeUL_r10_Optional transmissionModeUL_r10;
	struct AntennaInfoUL_r10_fourAntennaPortActivated_r10_e_fourAntennaPortActivated_r10_Optional fourAntennaPortActivated_r10;
};

enum MasterInformationBlock_dl_Bandwidth_e {
	MasterInformationBlock_dl_Bandwidth_e_n6 = 0,
	MasterInformationBlock_dl_Bandwidth_e_n15 = 1,
	MasterInformationBlock_dl_Bandwidth_e_n25 = 2,
	MasterInformationBlock_dl_Bandwidth_e_n50 = 3,
	MasterInformationBlock_dl_Bandwidth_e_n75 = 4,
	MasterInformationBlock_dl_Bandwidth_e_n100 = 5,
};

typedef enum MasterInformationBlock_dl_Bandwidth_e MasterInformationBlock_dl_Bandwidth_e;

enum PHICH_Config_phich_Duration_e {
	PHICH_Config_phich_Duration_e_normal = 0,
	PHICH_Config_phich_Duration_e_extended = 1,
};

typedef enum PHICH_Config_phich_Duration_e PHICH_Config_phich_Duration_e;

enum PHICH_Config_phich_Resource_e {
	PHICH_Config_phich_Resource_e_oneSixth = 0,
	PHICH_Config_phich_Resource_e_half = 1,
	PHICH_Config_phich_Resource_e_one = 2,
	PHICH_Config_phich_Resource_e_two = 3,
};

typedef enum PHICH_Config_phich_Resource_e PHICH_Config_phich_Resource_e;

struct PHICH_Config {
	PHICH_Config_phich_Duration_e phich_Duration;
	PHICH_Config_phich_Resource_e phich_Resource;
};

struct MasterInformationBlock {
	MasterInformationBlock_dl_Bandwidth_e dl_Bandwidth;
	struct PHICH_Config phich_Config;
	B8 systemFrameNumber;
	uint8_t schedulingInfoSIB1_BR_r13;
	bool systemInfoUnchanged_BR_r15;
	B4 spare;
};

struct BCCH_BCH_Message {
	struct MasterInformationBlock message;
};

enum AC_BarringConfig_ac_BarringFactor_e {
	AC_BarringConfig_ac_BarringFactor_e_p00 = 0,
	AC_BarringConfig_ac_BarringFactor_e_p05 = 1,
	AC_BarringConfig_ac_BarringFactor_e_p10 = 2,
	AC_BarringConfig_ac_BarringFactor_e_p15 = 3,
	AC_BarringConfig_ac_BarringFactor_e_p20 = 4,
	AC_BarringConfig_ac_BarringFactor_e_p25 = 5,
	AC_BarringConfig_ac_BarringFactor_e_p30 = 6,
	AC_BarringConfig_ac_BarringFactor_e_p40 = 7,
	AC_BarringConfig_ac_BarringFactor_e_p50 = 8,
	AC_BarringConfig_ac_BarringFactor_e_p60 = 9,
	AC_BarringConfig_ac_BarringFactor_e_p70 = 10,
	AC_BarringConfig_ac_BarringFactor_e_p75 = 11,
	AC_BarringConfig_ac_BarringFactor_e_p80 = 12,
	AC_BarringConfig_ac_BarringFactor_e_p85 = 13,
	AC_BarringConfig_ac_BarringFactor_e_p90 = 14,
	AC_BarringConfig_ac_BarringFactor_e_p95 = 15,
};

typedef enum AC_BarringConfig_ac_BarringFactor_e AC_BarringConfig_ac_BarringFactor_e;

enum AC_BarringConfig_ac_BarringTime_e {
	AC_BarringConfig_ac_BarringTime_e_s4 = 0,
	AC_BarringConfig_ac_BarringTime_e_s8 = 1,
	AC_BarringConfig_ac_BarringTime_e_s16 = 2,
	AC_BarringConfig_ac_BarringTime_e_s32 = 3,
	AC_BarringConfig_ac_BarringTime_e_s64 = 4,
	AC_BarringConfig_ac_BarringTime_e_s128 = 5,
	AC_BarringConfig_ac_BarringTime_e_s256 = 6,
	AC_BarringConfig_ac_BarringTime_e_s512 = 7,
};

typedef enum AC_BarringConfig_ac_BarringTime_e AC_BarringConfig_ac_BarringTime_e;

struct AC_BarringConfig {
	AC_BarringConfig_ac_BarringFactor_e ac_BarringFactor;
	AC_BarringConfig_ac_BarringTime_e ac_BarringTime;
	B5 ac_BarringForSpecialAC;
};

struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringInfo_ac_BarringForMO_Signalling_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringInfo_ac_BarringForMO_Data_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct SystemInformationBlockType2_ac_BarringInfo {
	bool ac_BarringForEmergency;
	struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringInfo_ac_BarringForMO_Signalling_Optional ac_BarringForMO_Signalling;
	struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringInfo_ac_BarringForMO_Data_Optional ac_BarringForMO_Data;
};

enum RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e {
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n4 = 0,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n8 = 1,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n12 = 2,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n16 = 3,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n20 = 4,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n24 = 5,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n28 = 6,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n32 = 7,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n36 = 8,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n40 = 9,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n44 = 10,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n48 = 11,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n52 = 12,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n56 = 13,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n60 = 14,
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e_n64 = 15,
};

typedef enum RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e;

enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e {
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n4 = 0,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n8 = 1,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n12 = 2,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n16 = 3,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n20 = 4,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n24 = 5,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n28 = 6,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n32 = 7,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n36 = 8,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n40 = 9,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n44 = 10,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n48 = 11,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n52 = 12,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n56 = 13,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e_n60 = 14,
};

typedef enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e;

enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e {
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e_b56 = 0,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e_b144 = 1,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e_b208 = 2,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e_b256 = 3,
};

typedef enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e;

enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e {
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_minusinfinity = 0,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB0 = 1,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB5 = 2,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB8 = 3,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB10 = 4,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB12 = 5,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB15 = 6,
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e_dB18 = 7,
};

typedef enum RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e;

struct RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig {
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_sizeOfRA_PreamblesGroupA_e sizeOfRA_PreamblesGroupA;
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messageSizeGroupA_e messageSizeGroupA;
	RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_messagePowerOffsetGroupB_e messagePowerOffsetGroupB;
};

struct RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_preamblesGroupAConfig_Optional {
	bool d;
	struct RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig v;
};

struct RACH_ConfigCommon_preambleInfo {
	RACH_ConfigCommon_preambleInfo_numberOfRA_Preambles_e numberOfRA_Preambles;
	struct RACH_ConfigCommon_preambleInfo_preamblesGroupAConfig_preamblesGroupAConfig_Optional preamblesGroupAConfig;
};

enum PowerRampingParameters_powerRampingStep_e {
	PowerRampingParameters_powerRampingStep_e_dB0 = 0,
	PowerRampingParameters_powerRampingStep_e_dB2 = 1,
	PowerRampingParameters_powerRampingStep_e_dB4 = 2,
	PowerRampingParameters_powerRampingStep_e_dB6 = 3,
};

typedef enum PowerRampingParameters_powerRampingStep_e PowerRampingParameters_powerRampingStep_e;

enum PowerRampingParameters_preambleInitialReceivedTargetPower_e {
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_120 = 0,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_118 = 1,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_116 = 2,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_114 = 3,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_112 = 4,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_110 = 5,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_108 = 6,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_106 = 7,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_104 = 8,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_102 = 9,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_100 = 10,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_98 = 11,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_96 = 12,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_94 = 13,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_92 = 14,
	PowerRampingParameters_preambleInitialReceivedTargetPower_e_dBm_90 = 15,
};

typedef enum PowerRampingParameters_preambleInitialReceivedTargetPower_e PowerRampingParameters_preambleInitialReceivedTargetPower_e;

struct PowerRampingParameters {
	PowerRampingParameters_powerRampingStep_e powerRampingStep;
	PowerRampingParameters_preambleInitialReceivedTargetPower_e preambleInitialReceivedTargetPower;
};

enum PreambleTransMax_e {
	PreambleTransMax_e_n3 = 0,
	PreambleTransMax_e_n4 = 1,
	PreambleTransMax_e_n5 = 2,
	PreambleTransMax_e_n6 = 3,
	PreambleTransMax_e_n7 = 4,
	PreambleTransMax_e_n8 = 5,
	PreambleTransMax_e_n10 = 6,
	PreambleTransMax_e_n20 = 7,
	PreambleTransMax_e_n50 = 8,
	PreambleTransMax_e_n100 = 9,
	PreambleTransMax_e_n200 = 10,
};

typedef enum PreambleTransMax_e PreambleTransMax_e;

enum RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e {
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf2 = 0,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf3 = 1,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf4 = 2,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf5 = 3,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf6 = 4,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf7 = 5,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf8 = 6,
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e_sf10 = 7,
};

typedef enum RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e;

enum RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e {
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf8 = 0,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf16 = 1,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf24 = 2,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf32 = 3,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf40 = 4,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf48 = 5,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf56 = 6,
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e_sf64 = 7,
};

typedef enum RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e;

struct RACH_ConfigCommon_ra_SupervisionInfo {
	PreambleTransMax_e preambleTransMax;
	RACH_ConfigCommon_ra_SupervisionInfo_ra_ResponseWindowSize_e ra_ResponseWindowSize;
	RACH_ConfigCommon_ra_SupervisionInfo_mac_ContentionResolutionTimer_e mac_ContentionResolutionTimer;
};

struct RACH_CE_LevelInfo_r13_preambleMappingInfo_r13 {
	uint8_t firstPreamble_r13;
	uint8_t lastPreamble_r13;
};

enum RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e {
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf20 = 0,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf50 = 1,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf80 = 2,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf120 = 3,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf180 = 4,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf240 = 5,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf320 = 6,
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e_sf400 = 7,
};

typedef enum RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e;

enum RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e {
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf80 = 0,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf100 = 1,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf120 = 2,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf160 = 3,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf200 = 4,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf240 = 5,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf480 = 6,
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e_sf960 = 7,
};

typedef enum RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e;

enum RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e {
	RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e_on = 0,
	RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e_off = 1,
};

typedef enum RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e;

enum RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e {
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b328 = 0,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b408 = 1,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b504 = 2,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b600 = 3,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b712 = 4,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b808 = 5,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b936 = 6,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e_b1000or456 = 7,
};

typedef enum RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e;

enum RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e {
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf240 = 0,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf480 = 1,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf960 = 2,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf1920 = 3,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf3840 = 4,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf5760 = 5,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf7680 = 6,
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_sf10240 = 7,
};

typedef enum RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e;

struct RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_mac_ContentionResolutionTimer_r15_Optional {
	bool d;
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e v;
};

struct RACH_CE_LevelInfo_r13_edt_Parameters_r15 {
	uint8_t edt_LastPreamble_r15;
	bool edt_SmallTBS_Enabled_r15;
	RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_TBS_r15_e edt_TBS_r15;
	struct RACH_CE_LevelInfo_r13_edt_Parameters_r15_mac_ContentionResolutionTimer_r15_e_mac_ContentionResolutionTimer_r15_Optional mac_ContentionResolutionTimer_r15;
};

struct RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_Parameters_r15_Optional {
	bool d;
	struct RACH_CE_LevelInfo_r13_edt_Parameters_r15 v;
};

struct RACH_CE_LevelInfo_r13 {
	struct RACH_CE_LevelInfo_r13_preambleMappingInfo_r13 preambleMappingInfo_r13;
	RACH_CE_LevelInfo_r13_ra_ResponseWindowSize_r13_e ra_ResponseWindowSize_r13;
	RACH_CE_LevelInfo_r13_mac_ContentionResolutionTimer_r13_e mac_ContentionResolutionTimer_r13;
	RACH_CE_LevelInfo_r13_rar_HoppingConfig_r13_e rar_HoppingConfig_r13;
	struct RACH_CE_LevelInfo_r13_edt_Parameters_r15_edt_Parameters_r15_Optional edt_Parameters_r15;
};

struct RACH_CE_LevelInfo_r13_RACH_CE_LevelInfoList_r13_Dynamic {
	size_t d;
	struct RACH_CE_LevelInfo_r13* v;
};

typedef struct RACH_CE_LevelInfo_r13_RACH_CE_LevelInfoList_r13_Dynamic RACH_CE_LevelInfoList_r13;

enum RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e {
	RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e_true = 0,
};

typedef enum RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e;

struct PreambleTransMax_e_RACH_ConfigCommon_preambleTransMax_CE_r13_Optional {
	bool d;
	PreambleTransMax_e v;
};

struct RACH_CE_LevelInfoList_r13_RACH_ConfigCommon_rach_CE_LevelInfoList_r13_Optional {
	bool d;
	RACH_CE_LevelInfoList_r13 v;
};

struct RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e_edt_SmallTBS_Subset_r15_Optional {
	bool d;
	RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e v;
};

struct RACH_ConfigCommon {
	struct RACH_ConfigCommon_preambleInfo preambleInfo;
	struct PowerRampingParameters powerRampingParameters;
	struct RACH_ConfigCommon_ra_SupervisionInfo ra_SupervisionInfo;
	uint8_t maxHARQ_Msg3Tx;
	struct PreambleTransMax_e_RACH_ConfigCommon_preambleTransMax_CE_r13_Optional preambleTransMax_CE_r13;
	struct RACH_CE_LevelInfoList_r13_RACH_ConfigCommon_rach_CE_LevelInfoList_r13_Optional rach_CE_LevelInfoList_r13;
	struct RACH_ConfigCommon_edt_SmallTBS_Subset_r15_e_edt_SmallTBS_Subset_r15_Optional edt_SmallTBS_Subset_r15;
};

enum BCCH_Config_modificationPeriodCoeff_e {
	BCCH_Config_modificationPeriodCoeff_e_n2 = 0,
	BCCH_Config_modificationPeriodCoeff_e_n4 = 1,
	BCCH_Config_modificationPeriodCoeff_e_n8 = 2,
	BCCH_Config_modificationPeriodCoeff_e_n16 = 3,
};

typedef enum BCCH_Config_modificationPeriodCoeff_e BCCH_Config_modificationPeriodCoeff_e;

struct BCCH_Config {
	BCCH_Config_modificationPeriodCoeff_e modificationPeriodCoeff;
};

enum PCCH_Config_defaultPagingCycle_e {
	PCCH_Config_defaultPagingCycle_e_rf32 = 0,
	PCCH_Config_defaultPagingCycle_e_rf64 = 1,
	PCCH_Config_defaultPagingCycle_e_rf128 = 2,
	PCCH_Config_defaultPagingCycle_e_rf256 = 3,
};

typedef enum PCCH_Config_defaultPagingCycle_e PCCH_Config_defaultPagingCycle_e;

enum PCCH_Config_nB_e {
	PCCH_Config_nB_e_fourT = 0,
	PCCH_Config_nB_e_twoT = 1,
	PCCH_Config_nB_e_oneT = 2,
	PCCH_Config_nB_e_halfT = 3,
	PCCH_Config_nB_e_quarterT = 4,
	PCCH_Config_nB_e_oneEighthT = 5,
	PCCH_Config_nB_e_oneSixteenthT = 6,
	PCCH_Config_nB_e_oneThirtySecondT = 7,
};

typedef enum PCCH_Config_nB_e PCCH_Config_nB_e;

struct PCCH_Config {
	PCCH_Config_defaultPagingCycle_e defaultPagingCycle;
	PCCH_Config_nB_e nB;
};

struct PRACH_ConfigInfo {
	uint8_t prach_ConfigIndex;
	bool highSpeedFlag;
	uint8_t zeroCorrelationZoneConfig;
	uint8_t prach_FreqOffset;
};

struct PRACH_ConfigSIB {
	uint16_t rootSequenceIndex;
	struct PRACH_ConfigInfo prach_ConfigInfo;
};

struct PDSCH_ConfigCommon {
	int8_t referenceSignalPower;
	uint8_t p_b;
};

enum PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e {
	PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e_interSubFrame = 0,
	PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e_intraAndInterSubFrame = 1,
};

typedef enum PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e;

struct PUSCH_ConfigCommon_pusch_ConfigBasic {
	uint8_t n_SB;
	PUSCH_ConfigCommon_pusch_ConfigBasic_hoppingMode_e hoppingMode;
	uint8_t pusch_HoppingOffset;
	bool enable64QAM;
};

struct UL_ReferenceSignalsPUSCH {
	bool groupHoppingEnabled;
	uint8_t groupAssignmentPUSCH;
	bool sequenceHoppingEnabled;
	uint8_t cyclicShift;
};

struct PUSCH_ConfigCommon {
	struct PUSCH_ConfigCommon_pusch_ConfigBasic pusch_ConfigBasic;
	struct UL_ReferenceSignalsPUSCH ul_ReferenceSignalsPUSCH;
};

enum PUCCH_ConfigCommon_deltaPUCCH_Shift_e {
	PUCCH_ConfigCommon_deltaPUCCH_Shift_e_ds1 = 0,
	PUCCH_ConfigCommon_deltaPUCCH_Shift_e_ds2 = 1,
	PUCCH_ConfigCommon_deltaPUCCH_Shift_e_ds3 = 2,
};

typedef enum PUCCH_ConfigCommon_deltaPUCCH_Shift_e PUCCH_ConfigCommon_deltaPUCCH_Shift_e;

struct PUCCH_ConfigCommon {
	PUCCH_ConfigCommon_deltaPUCCH_Shift_e deltaPUCCH_Shift;
	uint8_t nRB_CQI;
	uint8_t nCS_AN;
	uint16_t n1PUCCH_AN;
};

enum SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e {
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw0 = 0,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw1 = 1,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw2 = 2,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw3 = 3,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw4 = 4,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw5 = 5,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw6 = 6,
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e_bw7 = 7,
};

typedef enum SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e;

enum SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e {
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc0 = 0,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc1 = 1,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc2 = 2,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc3 = 3,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc4 = 4,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc5 = 5,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc6 = 6,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc7 = 7,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc8 = 8,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc9 = 9,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc10 = 10,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc11 = 11,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc12 = 12,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc13 = 13,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc14 = 14,
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e_sc15 = 15,
};

typedef enum SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e;

enum SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e {
	SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e_true = 0,
};

typedef enum SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e;

struct SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e_srs_MaxUpPts_Optional {
	bool d;
	SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e v;
};

struct SoundingRS_UL_ConfigCommon_setup {
	SoundingRS_UL_ConfigCommon_setup_srs_BandwidthConfig_e srs_BandwidthConfig;
	SoundingRS_UL_ConfigCommon_setup_srs_SubframeConfig_e srs_SubframeConfig;
	bool ackNackSRS_SimultaneousTransmission;
	struct SoundingRS_UL_ConfigCommon_setup_srs_MaxUpPts_e_srs_MaxUpPts_Optional srs_MaxUpPts;
};

enum SoundingRS_UL_ConfigCommon_Sel {
	SoundingRS_UL_ConfigCommon_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigCommon_release = 1,
	SoundingRS_UL_ConfigCommon_setup = 2,
};

union SoundingRS_UL_ConfigCommon_Value {
	Null release;
	struct SoundingRS_UL_ConfigCommon_setup setup;
};

struct SoundingRS_UL_ConfigCommon {
	enum SoundingRS_UL_ConfigCommon_Sel d;
	union SoundingRS_UL_ConfigCommon_Value v;
};

enum Alpha_r12_e {
	Alpha_r12_e_al0 = 0,
	Alpha_r12_e_al04 = 1,
	Alpha_r12_e_al05 = 2,
	Alpha_r12_e_al06 = 3,
	Alpha_r12_e_al07 = 4,
	Alpha_r12_e_al08 = 5,
	Alpha_r12_e_al09 = 6,
	Alpha_r12_e_al1 = 7,
};

typedef enum Alpha_r12_e Alpha_r12_e;

enum DeltaFList_PUCCH_deltaF_PUCCH_Format1_e {
	DeltaFList_PUCCH_deltaF_PUCCH_Format1_e_deltaF_2 = 0,
	DeltaFList_PUCCH_deltaF_PUCCH_Format1_e_deltaF0 = 1,
	DeltaFList_PUCCH_deltaF_PUCCH_Format1_e_deltaF2 = 2,
};

typedef enum DeltaFList_PUCCH_deltaF_PUCCH_Format1_e DeltaFList_PUCCH_deltaF_PUCCH_Format1_e;

enum DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e {
	DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e_deltaF1 = 0,
	DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e_deltaF3 = 1,
	DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e_deltaF5 = 2,
};

typedef enum DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e;

enum DeltaFList_PUCCH_deltaF_PUCCH_Format2_e {
	DeltaFList_PUCCH_deltaF_PUCCH_Format2_e_deltaF_2 = 0,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2_e_deltaF0 = 1,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2_e_deltaF1 = 2,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2_e_deltaF2 = 3,
};

typedef enum DeltaFList_PUCCH_deltaF_PUCCH_Format2_e DeltaFList_PUCCH_deltaF_PUCCH_Format2_e;

enum DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e {
	DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e_deltaF_2 = 0,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e_deltaF0 = 1,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e_deltaF2 = 2,
};

typedef enum DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e;

enum DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e {
	DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e_deltaF_2 = 0,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e_deltaF0 = 1,
	DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e_deltaF2 = 2,
};

typedef enum DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e;

struct DeltaFList_PUCCH {
	DeltaFList_PUCCH_deltaF_PUCCH_Format1_e deltaF_PUCCH_Format1;
	DeltaFList_PUCCH_deltaF_PUCCH_Format1b_e deltaF_PUCCH_Format1b;
	DeltaFList_PUCCH_deltaF_PUCCH_Format2_e deltaF_PUCCH_Format2;
	DeltaFList_PUCCH_deltaF_PUCCH_Format2a_e deltaF_PUCCH_Format2a;
	DeltaFList_PUCCH_deltaF_PUCCH_Format2b_e deltaF_PUCCH_Format2b;
};

struct UplinkPowerControlCommon {
	int8_t p0_NominalPUSCH;
	Alpha_r12_e alpha;
	int8_t p0_NominalPUCCH;
	struct DeltaFList_PUCCH deltaFList_PUCCH;
	int8_t deltaPreambleMsg3;
};

enum UL_CyclicPrefixLength_e {
	UL_CyclicPrefixLength_e_len1 = 0,
	UL_CyclicPrefixLength_e_len2 = 1,
};

typedef enum UL_CyclicPrefixLength_e UL_CyclicPrefixLength_e;

enum UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e {
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF_1 = 0,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF0 = 1,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF1 = 2,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF2 = 3,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF3 = 4,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF4 = 5,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF5 = 6,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e_deltaF6 = 7,
};

typedef enum UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e;

enum UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e {
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e_deltaF1 = 0,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e_deltaF2 = 1,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e_spare2 = 2,
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e_spare1 = 3,
};

typedef enum UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e;

struct UplinkPowerControlCommon_v1020 {
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format3_r10_e deltaF_PUCCH_Format3_r10;
	UplinkPowerControlCommon_v1020_deltaF_PUCCH_Format1bCS_r10_e deltaF_PUCCH_Format1bCS_r10;
};

enum RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e {
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e_n1 = 0,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e_n2 = 1,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e_n3 = 2,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e_n4 = 3,
};

typedef enum RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e;

enum RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e {
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s30 = 0,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s60 = 1,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s120 = 2,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s240 = 3,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s300 = 4,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s420 = 5,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s600 = 6,
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e_s900 = 7,
};

typedef enum RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e;

struct uint8_t_RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffset_r12_Optional {
	bool d;
	uint8_t v;
};

struct RACH_ConfigCommon_v1250_txFailParams_r12 {
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailCount_r12_e connEstFailCount_r12;
	RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffsetValidity_r12_e connEstFailOffsetValidity_r12;
	struct uint8_t_RACH_ConfigCommon_v1250_txFailParams_r12_connEstFailOffset_r12_Optional connEstFailOffset_r12;
};

struct RACH_ConfigCommon_v1250 {
	struct RACH_ConfigCommon_v1250_txFailParams_r12 txFailParams_r12;
};

enum PUSCH_ConfigCommon_v1270_enable64QAM_v1270_e {
	PUSCH_ConfigCommon_v1270_enable64QAM_v1270_e_true = 0,
};

typedef enum PUSCH_ConfigCommon_v1270_enable64QAM_v1270_e PUSCH_ConfigCommon_v1270_enable64QAM_v1270_e;

struct PUSCH_ConfigCommon_v1270 {
	PUSCH_ConfigCommon_v1270_enable64QAM_v1270_e enable64QAM_v1270;
};

enum BCCH_Config_v1310_modificationPeriodCoeff_v1310_e {
	BCCH_Config_v1310_modificationPeriodCoeff_v1310_e_n64 = 0,
};

typedef enum BCCH_Config_v1310_modificationPeriodCoeff_v1310_e BCCH_Config_v1310_modificationPeriodCoeff_v1310_e;

struct BCCH_Config_v1310 {
	BCCH_Config_v1310_modificationPeriodCoeff_v1310_e modificationPeriodCoeff_v1310;
};

enum PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e {
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r1 = 0,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r2 = 1,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r4 = 2,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r8 = 3,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r16 = 4,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r32 = 5,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r64 = 6,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r128 = 7,
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e_r256 = 8,
};

typedef enum PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e;

enum PCCH_Config_v1310_nB_v1310_e {
	PCCH_Config_v1310_nB_v1310_e_one64thT = 0,
	PCCH_Config_v1310_nB_v1310_e_one128thT = 1,
	PCCH_Config_v1310_nB_v1310_e_one256thT = 2,
};

typedef enum PCCH_Config_v1310_nB_v1310_e PCCH_Config_v1310_nB_v1310_e;

struct PCCH_Config_v1310_nB_v1310_e_nB_v1310_Optional {
	bool d;
	PCCH_Config_v1310_nB_v1310_e v;
};

struct PCCH_Config_v1310 {
	uint8_t paging_narrowBands_r13;
	PCCH_Config_v1310_mpdcch_NumRepetition_Paging_r13_e mpdcch_NumRepetition_Paging_r13;
	struct PCCH_Config_v1310_nB_v1310_e_nB_v1310_Optional nB_v1310;
};

enum FreqHoppingParameters_r13_dummy_e {
	FreqHoppingParameters_r13_dummy_e_nb2 = 0,
	FreqHoppingParameters_r13_dummy_e_nb4 = 1,
};

typedef enum FreqHoppingParameters_r13_dummy_e FreqHoppingParameters_r13_dummy_e;

enum FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e {
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e_int1 = 0,
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e_int2 = 1,
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e_int4 = 2,
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e_int8 = 3,
};

typedef enum FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e;

enum FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e {
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e_int1 = 0,
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e_int5 = 1,
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e_int10 = 2,
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e_int20 = 3,
};

typedef enum FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e;

enum FreqHoppingParameters_r13_dummy2_Sel {
	FreqHoppingParameters_r13_dummy2_UNBOUND_VALUE = 0,
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13 = 1,
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13 = 2,
};

union FreqHoppingParameters_r13_dummy2_Value {
	FreqHoppingParameters_r13_dummy2_interval_FDD_r13_e interval_FDD_r13;
	FreqHoppingParameters_r13_dummy2_interval_TDD_r13_e interval_TDD_r13;
};

struct FreqHoppingParameters_r13_dummy2 {
	enum FreqHoppingParameters_r13_dummy2_Sel d;
	union FreqHoppingParameters_r13_dummy2_Value v;
};

enum FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e {
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e_int2 = 0,
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e_int4 = 1,
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e_int8 = 2,
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e_int16 = 3,
};

typedef enum FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e;

enum FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e {
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e_int5 = 0,
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e_int10 = 1,
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e_int20 = 2,
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e_int40 = 3,
};

typedef enum FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e;

enum FreqHoppingParameters_r13_dummy3_Sel {
	FreqHoppingParameters_r13_dummy3_UNBOUND_VALUE = 0,
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13 = 1,
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13 = 2,
};

union FreqHoppingParameters_r13_dummy3_Value {
	FreqHoppingParameters_r13_dummy3_interval_FDD_r13_e interval_FDD_r13;
	FreqHoppingParameters_r13_dummy3_interval_TDD_r13_e interval_TDD_r13;
};

struct FreqHoppingParameters_r13_dummy3 {
	enum FreqHoppingParameters_r13_dummy3_Sel d;
	union FreqHoppingParameters_r13_dummy3_Value v;
};

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int1 = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int2 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int4 = 2,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int8 = 3,
};

typedef enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e;

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int1 = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int5 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int10 = 2,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int20 = 3,
};

typedef enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e;

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_Sel {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_UNBOUND_VALUE = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13 = 2,
};

union FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_Value {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_FDD_r13_e interval_FDD_r13;
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_TDD_r13_e interval_TDD_r13;
};

struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13 {
	enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_Sel d;
	union FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_Value v;
};

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int2 = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int4 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int8 = 2,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int16 = 3,
};

typedef enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e;

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int5 = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int10 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int20 = 2,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int40 = 3,
};

typedef enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e;

enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_Sel {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_UNBOUND_VALUE = 0,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13 = 1,
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13 = 2,
};

union FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_Value {
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_FDD_r13_e interval_FDD_r13;
	FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_TDD_r13_e interval_TDD_r13;
};

struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13 {
	enum FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_Sel d;
	union FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_Value v;
};

struct FreqHoppingParameters_r13_dummy_e_dummy_Optional {
	bool d;
	FreqHoppingParameters_r13_dummy_e v;
};

struct FreqHoppingParameters_r13_dummy2_dummy2_Optional {
	bool d;
	struct FreqHoppingParameters_r13_dummy2 v;
};

struct FreqHoppingParameters_r13_dummy3_dummy3_Optional {
	bool d;
	struct FreqHoppingParameters_r13_dummy3 v;
};

struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_ULHoppingConfigCommonModeA_r13_Optional {
	bool d;
	struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13 v;
};

struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_ULHoppingConfigCommonModeB_r13_Optional {
	bool d;
	struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13 v;
};

struct uint8_t_FreqHoppingParameters_r13_dummy4_Optional {
	bool d;
	uint8_t v;
};

struct FreqHoppingParameters_r13 {
	struct FreqHoppingParameters_r13_dummy_e_dummy_Optional dummy;
	struct FreqHoppingParameters_r13_dummy2_dummy2_Optional dummy2;
	struct FreqHoppingParameters_r13_dummy3_dummy3_Optional dummy3;
	struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeA_r13_interval_ULHoppingConfigCommonModeA_r13_Optional interval_ULHoppingConfigCommonModeA_r13;
	struct FreqHoppingParameters_r13_interval_ULHoppingConfigCommonModeB_r13_interval_ULHoppingConfigCommonModeB_r13_Optional interval_ULHoppingConfigCommonModeB_r13;
	struct uint8_t_FreqHoppingParameters_r13_dummy4_Optional dummy4;
};

enum PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e {
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e_r16 = 0,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e_r32 = 1,
};

typedef enum PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e;

enum PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e {
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r192 = 0,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r256 = 1,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r384 = 2,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r512 = 3,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r768 = 4,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r1024 = 5,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r1536 = 6,
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_r2048 = 7,
};

typedef enum PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e;

struct PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e_pdsch_maxNumRepetitionCEmodeA_r13_Optional {
	bool d;
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e v;
};

struct PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_pdsch_maxNumRepetitionCEmodeB_r13_Optional {
	bool d;
	PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e v;
};

struct PDSCH_ConfigCommon_v1310 {
	struct PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeA_r13_e_pdsch_maxNumRepetitionCEmodeA_r13_Optional pdsch_maxNumRepetitionCEmodeA_r13;
	struct PDSCH_ConfigCommon_v1310_pdsch_maxNumRepetitionCEmodeB_r13_e_pdsch_maxNumRepetitionCEmodeB_r13_Optional pdsch_maxNumRepetitionCEmodeB_r13;
};

enum PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e {
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e_r8 = 0,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e_r16 = 1,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e_r32 = 2,
};

typedef enum PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e;

enum PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e {
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r192 = 0,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r256 = 1,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r384 = 2,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r512 = 3,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r768 = 4,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r1024 = 5,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r1536 = 6,
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_r2048 = 7,
};

typedef enum PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e;

struct PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e_pusch_maxNumRepetitionCEmodeA_r13_Optional {
	bool d;
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e v;
};

struct PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_pusch_maxNumRepetitionCEmodeB_r13_Optional {
	bool d;
	PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e v;
};

struct uint8_t_PUSCH_ConfigCommon_v1310_pusch_HoppingOffset_v1310_Optional {
	bool d;
	uint8_t v;
};

struct PUSCH_ConfigCommon_v1310 {
	struct PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeA_r13_e_pusch_maxNumRepetitionCEmodeA_r13_Optional pusch_maxNumRepetitionCEmodeA_r13;
	struct PUSCH_ConfigCommon_v1310_pusch_maxNumRepetitionCEmodeB_r13_e_pusch_maxNumRepetitionCEmodeB_r13_Optional pusch_maxNumRepetitionCEmodeB_r13;
	struct uint8_t_PUSCH_ConfigCommon_v1310_pusch_HoppingOffset_v1310_Optional pusch_HoppingOffset_v1310;
};

typedef uint8_t RSRP_Range;

struct RSRP_Range_RSRP_ThresholdsPrachInfoList_r13_Dynamic {
	size_t d;
	RSRP_Range* v;
};

typedef struct RSRP_Range_RSRP_ThresholdsPrachInfoList_r13_Dynamic RSRP_ThresholdsPrachInfoList_r13;

enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e {
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v1 = 0,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v1dot5 = 1,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v2 = 2,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v2dot5 = 3,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v4 = 4,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v5 = 5,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v8 = 6,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v10 = 7,
};

typedef enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e;

enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e {
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v1 = 0,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v2 = 1,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v4 = 2,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v5 = 3,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v8 = 4,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v10 = 5,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v20 = 6,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_spare = 7,
};

typedef enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e;

enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_Sel {
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_UNBOUND_VALUE = 0,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13 = 1,
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13 = 2,
};

union PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_Value {
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e fdd_r13;
	PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e tdd_r13;
};

struct PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13 {
	enum PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_Sel d;
	union PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_Value v;
};

enum PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e {
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf2 = 0,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf4 = 1,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf8 = 2,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf16 = 3,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf32 = 4,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf64 = 5,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf128 = 6,
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_sf256 = 7,
};

typedef enum PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e;

enum PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e {
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n3 = 0,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n4 = 1,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n5 = 2,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n6 = 3,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n7 = 4,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n8 = 5,
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_n10 = 6,
};

typedef enum PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e;

enum PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e {
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n1 = 0,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n2 = 1,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n4 = 2,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n8 = 3,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n16 = 4,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n32 = 5,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n64 = 6,
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e_n128 = 7,
};

typedef enum PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e;

enum PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e {
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r1 = 0,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r2 = 1,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r4 = 2,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r8 = 3,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r16 = 4,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r32 = 5,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r64 = 6,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r128 = 7,
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e_r256 = 8,
};

typedef enum PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e;

enum PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e {
	PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e_on = 0,
	PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e_off = 1,
};

typedef enum PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e;

struct PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_prach_StartingSubframe_r13_Optional {
	bool d;
	PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e v;
};

struct PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_maxNumPreambleAttemptCE_r13_Optional {
	bool d;
	PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e v;
};

struct uint8_t_PRACH_ParametersCE_r13_mpdcch_NarrowbandsToMonitor_r13_Dynamic {
	size_t d;
	uint8_t* v;
};

struct PRACH_ParametersCE_r13 {
	uint8_t prach_ConfigIndex_r13;
	uint8_t prach_FreqOffset_r13;
	struct PRACH_ParametersCE_r13_prach_StartingSubframe_r13_e_prach_StartingSubframe_r13_Optional prach_StartingSubframe_r13;
	struct PRACH_ParametersCE_r13_maxNumPreambleAttemptCE_r13_e_maxNumPreambleAttemptCE_r13_Optional maxNumPreambleAttemptCE_r13;
	PRACH_ParametersCE_r13_numRepetitionPerPreambleAttempt_r13_e numRepetitionPerPreambleAttempt_r13;
	struct uint8_t_PRACH_ParametersCE_r13_mpdcch_NarrowbandsToMonitor_r13_Dynamic mpdcch_NarrowbandsToMonitor_r13;
	PRACH_ParametersCE_r13_mpdcch_NumRepetition_RA_r13_e mpdcch_NumRepetition_RA_r13;
	PRACH_ParametersCE_r13_prach_HoppingConfig_r13_e prach_HoppingConfig_r13;
};

struct PRACH_ParametersCE_r13_PRACH_ParametersListCE_r13_Dynamic {
	size_t d;
	struct PRACH_ParametersCE_r13* v;
};

typedef struct PRACH_ParametersCE_r13_PRACH_ParametersListCE_r13_Dynamic PRACH_ParametersListCE_r13;

struct PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_mpdcch_startSF_CSS_RA_r13_Optional {
	bool d;
	struct PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13 v;
};

struct uint8_t_PRACH_ConfigSIB_v1310_prach_HoppingOffset_r13_Optional {
	bool d;
	uint8_t v;
};

struct PRACH_ConfigSIB_v1310 {
	RSRP_ThresholdsPrachInfoList_r13 rsrp_ThresholdsPrachInfoList_r13;
	struct PRACH_ConfigSIB_v1310_mpdcch_startSF_CSS_RA_r13_mpdcch_startSF_CSS_RA_r13_Optional mpdcch_startSF_CSS_RA_r13;
	struct uint8_t_PRACH_ConfigSIB_v1310_prach_HoppingOffset_r13_Optional prach_HoppingOffset_r13;
	PRACH_ParametersListCE_r13 prach_ParametersListCE_r13;
};

struct uint16_t_N1PUCCH_AN_InfoList_r13_Dynamic {
	size_t d;
	uint16_t* v;
};

typedef struct uint16_t_N1PUCCH_AN_InfoList_r13_Dynamic N1PUCCH_AN_InfoList_r13;

enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e {
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_n1 = 0,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_n2 = 1,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_n4 = 2,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_n8 = 3,
};

typedef enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e;

enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e {
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_n1 = 0,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_n2 = 1,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_n4 = 2,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_n8 = 3,
};

typedef enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e;

enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e {
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_n4 = 0,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_n8 = 1,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_n16 = 2,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_n32 = 3,
};

typedef enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e;

enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e {
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_n4 = 0,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_n8 = 1,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_n16 = 2,
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_n32 = 3,
};

typedef enum PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e;

struct N1PUCCH_AN_InfoList_r13_PUCCH_ConfigCommon_v1310_n1PUCCH_AN_InfoList_r13_Optional {
	bool d;
	N1PUCCH_AN_InfoList_r13 v;
};

struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_pucch_NumRepetitionCE_Msg4_Level0_r13_Optional {
	bool d;
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e v;
};

struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_pucch_NumRepetitionCE_Msg4_Level1_r13_Optional {
	bool d;
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e v;
};

struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_pucch_NumRepetitionCE_Msg4_Level2_r13_Optional {
	bool d;
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e v;
};

struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_pucch_NumRepetitionCE_Msg4_Level3_r13_Optional {
	bool d;
	PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e v;
};

struct PUCCH_ConfigCommon_v1310 {
	struct N1PUCCH_AN_InfoList_r13_PUCCH_ConfigCommon_v1310_n1PUCCH_AN_InfoList_r13_Optional n1PUCCH_AN_InfoList_r13;
	struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level0_r13_e_pucch_NumRepetitionCE_Msg4_Level0_r13_Optional pucch_NumRepetitionCE_Msg4_Level0_r13;
	struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level1_r13_e_pucch_NumRepetitionCE_Msg4_Level1_r13_Optional pucch_NumRepetitionCE_Msg4_Level1_r13;
	struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level2_r13_e_pucch_NumRepetitionCE_Msg4_Level2_r13_Optional pucch_NumRepetitionCE_Msg4_Level2_r13;
	struct PUCCH_ConfigCommon_v1310_pucch_NumRepetitionCE_Msg4_Level3_r13_e_pucch_NumRepetitionCE_Msg4_Level3_r13_Optional pucch_NumRepetitionCE_Msg4_Level3_r13;
};

enum HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e {
	HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e_true = 0,
};

typedef enum HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e;

enum HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e {
	HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e_true = 0,
};

typedef enum HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e;

struct HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e_highSpeedEnhancedMeasFlag_r14_Optional {
	bool d;
	HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e v;
};

struct HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e_highSpeedEnhancedDemodulationFlag_r14_Optional {
	bool d;
	HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e v;
};

struct HighSpeedConfig_r14 {
	struct HighSpeedConfig_r14_highSpeedEnhancedMeasFlag_r14_e_highSpeedEnhancedMeasFlag_r14_Optional highSpeedEnhancedMeasFlag_r14;
	struct HighSpeedConfig_r14_highSpeedEnhancedDemodulationFlag_r14_e_highSpeedEnhancedDemodulationFlag_r14_Optional highSpeedEnhancedDemodulationFlag_r14;
};

struct PRACH_Config_v1430 {
	uint16_t rootSequenceIndexHighSpeed_r14;
	uint8_t zeroCorrelationZoneConfigHighSpeed_r14;
	uint8_t prach_ConfigIndexHighSpeed_r14;
	uint8_t prach_FreqOffsetHighSpeed_r14;
};

enum PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e {
	PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e_n64 = 0,
	PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e_n128 = 1,
};

typedef enum PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e;

struct PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e_pucch_NumRepetitionCE_Msg4_Level3_r14_Optional {
	bool d;
	PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e v;
};

struct PUCCH_ConfigCommon_v1430 {
	struct PUCCH_ConfigCommon_v1430_pucch_NumRepetitionCE_Msg4_Level3_r14_e_pucch_NumRepetitionCE_Msg4_Level3_r14_Optional pucch_NumRepetitionCE_Msg4_Level3_r14;
};

enum EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e {
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf2 = 0,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf4 = 1,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf8 = 2,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf16 = 3,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf32 = 4,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf64 = 5,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf128 = 6,
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_sf256 = 7,
};

typedef enum EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e;

struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_prach_StartingSubframe_r15_Optional {
	bool d;
	EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e v;
};

struct uint8_t_EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_mpdcch_NarrowbandsToMonitor_r15_Dynamic {
	size_t d;
	uint8_t* v;
};

struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15 {
	uint8_t prach_ConfigIndex_r15;
	uint8_t prach_FreqOffset_r15;
	struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_prach_StartingSubframe_r15_e_prach_StartingSubframe_r15_Optional prach_StartingSubframe_r15;
	struct uint8_t_EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_mpdcch_NarrowbandsToMonitor_r15_Dynamic mpdcch_NarrowbandsToMonitor_r15;
};

struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_Optional {
	bool d;
	struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15 v;
};

struct EDT_PRACH_ParametersCE_r15 {
	struct EDT_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_edt_PRACH_ParametersCE_r15_Optional edt_PRACH_ParametersCE_r15;
};

struct EDT_PRACH_ParametersCE_r15_PRACH_ConfigSIB_v1530_edt_PRACH_ParametersListCE_r15_Dynamic {
	size_t d;
	struct EDT_PRACH_ParametersCE_r15* v;
};

struct PRACH_ConfigSIB_v1530 {
	struct EDT_PRACH_ParametersCE_r15_PRACH_ConfigSIB_v1530_edt_PRACH_ParametersListCE_r15_Dynamic edt_PRACH_ParametersListCE_r15;
};

enum RSS_Config_r15_duration_r15_e {
	RSS_Config_r15_duration_r15_e_sf8 = 0,
	RSS_Config_r15_duration_r15_e_sf16 = 1,
	RSS_Config_r15_duration_r15_e_sf32 = 2,
	RSS_Config_r15_duration_r15_e_sf40 = 3,
};

typedef enum RSS_Config_r15_duration_r15_e RSS_Config_r15_duration_r15_e;

enum RSS_Config_r15_periodicity_r15_e {
	RSS_Config_r15_periodicity_r15_e_ms160 = 0,
	RSS_Config_r15_periodicity_r15_e_ms320 = 1,
	RSS_Config_r15_periodicity_r15_e_ms640 = 2,
	RSS_Config_r15_periodicity_r15_e_ms1280 = 3,
};

typedef enum RSS_Config_r15_periodicity_r15_e RSS_Config_r15_periodicity_r15_e;

enum RSS_Config_r15_powerBoost_r15_e {
	RSS_Config_r15_powerBoost_r15_e_dB0 = 0,
	RSS_Config_r15_powerBoost_r15_e_dB3 = 1,
	RSS_Config_r15_powerBoost_r15_e_dB4dot8 = 2,
	RSS_Config_r15_powerBoost_r15_e_dB6 = 3,
};

typedef enum RSS_Config_r15_powerBoost_r15_e RSS_Config_r15_powerBoost_r15_e;

struct RSS_Config_r15 {
	RSS_Config_r15_duration_r15_e duration_r15;
	uint8_t freqLocation_r15;
	RSS_Config_r15_periodicity_r15_e periodicity_r15;
	RSS_Config_r15_powerBoost_r15_e powerBoost_r15;
	uint8_t timeOffset_r15;
};

enum WUS_Config_r15_maxDurationFactor_r15_e {
	WUS_Config_r15_maxDurationFactor_r15_e_one32th = 0,
	WUS_Config_r15_maxDurationFactor_r15_e_one16th = 1,
	WUS_Config_r15_maxDurationFactor_r15_e_one8th = 2,
	WUS_Config_r15_maxDurationFactor_r15_e_one4th = 3,
};

typedef enum WUS_Config_r15_maxDurationFactor_r15_e WUS_Config_r15_maxDurationFactor_r15_e;

enum WUS_Config_r15_numPOs_r15_e {
	WUS_Config_r15_numPOs_r15_e_n1 = 0,
	WUS_Config_r15_numPOs_r15_e_n2 = 1,
	WUS_Config_r15_numPOs_r15_e_n4 = 2,
	WUS_Config_r15_numPOs_r15_e_spare1 = 3,
};

typedef enum WUS_Config_r15_numPOs_r15_e WUS_Config_r15_numPOs_r15_e;

enum WUS_Config_r15_freqLocation_r15_e {
	WUS_Config_r15_freqLocation_r15_e_n0 = 0,
	WUS_Config_r15_freqLocation_r15_e_n2 = 1,
	WUS_Config_r15_freqLocation_r15_e_n4 = 2,
	WUS_Config_r15_freqLocation_r15_e_spare1 = 3,
};

typedef enum WUS_Config_r15_freqLocation_r15_e WUS_Config_r15_freqLocation_r15_e;

enum WUS_Config_r15_timeOffsetDRX_r15_e {
	WUS_Config_r15_timeOffsetDRX_r15_e_ms40 = 0,
	WUS_Config_r15_timeOffsetDRX_r15_e_ms80 = 1,
	WUS_Config_r15_timeOffsetDRX_r15_e_ms160 = 2,
	WUS_Config_r15_timeOffsetDRX_r15_e_ms240 = 3,
};

typedef enum WUS_Config_r15_timeOffsetDRX_r15_e WUS_Config_r15_timeOffsetDRX_r15_e;

enum WUS_Config_r15_timeOffset_eDRX_Short_r15_e {
	WUS_Config_r15_timeOffset_eDRX_Short_r15_e_ms40 = 0,
	WUS_Config_r15_timeOffset_eDRX_Short_r15_e_ms80 = 1,
	WUS_Config_r15_timeOffset_eDRX_Short_r15_e_ms160 = 2,
	WUS_Config_r15_timeOffset_eDRX_Short_r15_e_ms240 = 3,
};

typedef enum WUS_Config_r15_timeOffset_eDRX_Short_r15_e WUS_Config_r15_timeOffset_eDRX_Short_r15_e;

enum WUS_Config_r15_timeOffset_eDRX_Long_r15_e {
	WUS_Config_r15_timeOffset_eDRX_Long_r15_e_ms1000 = 0,
	WUS_Config_r15_timeOffset_eDRX_Long_r15_e_ms2000 = 1,
};

typedef enum WUS_Config_r15_timeOffset_eDRX_Long_r15_e WUS_Config_r15_timeOffset_eDRX_Long_r15_e;

struct WUS_Config_r15_numPOs_r15_e_numPOs_r15_Optional {
	bool d;
	WUS_Config_r15_numPOs_r15_e v;
};

struct WUS_Config_r15_timeOffset_eDRX_Long_r15_e_timeOffset_eDRX_Long_r15_Optional {
	bool d;
	WUS_Config_r15_timeOffset_eDRX_Long_r15_e v;
};

struct WUS_Config_r15 {
	WUS_Config_r15_maxDurationFactor_r15_e maxDurationFactor_r15;
	struct WUS_Config_r15_numPOs_r15_e_numPOs_r15_Optional numPOs_r15;
	WUS_Config_r15_freqLocation_r15_e freqLocation_r15;
	WUS_Config_r15_timeOffsetDRX_r15_e timeOffsetDRX_r15;
	WUS_Config_r15_timeOffset_eDRX_Short_r15_e timeOffset_eDRX_Short_r15;
	struct WUS_Config_r15_timeOffset_eDRX_Long_r15_e_timeOffset_eDRX_Long_r15_Optional timeOffset_eDRX_Long_r15;
};

enum HighSpeedConfig_v1530_highSpeedMeasGapCE_ModeA_r15_e {
	HighSpeedConfig_v1530_highSpeedMeasGapCE_ModeA_r15_e_true = 0,
};

typedef enum HighSpeedConfig_v1530_highSpeedMeasGapCE_ModeA_r15_e HighSpeedConfig_v1530_highSpeedMeasGapCE_ModeA_r15_e;

struct HighSpeedConfig_v1530 {
	HighSpeedConfig_v1530_highSpeedMeasGapCE_ModeA_r15_e highSpeedMeasGapCE_ModeA_r15;
};

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF_1 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF0 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF1 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF2 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF3 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF4 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF5 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF6 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF1 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF2 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF3 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF4 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF5 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF6 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF7 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF8 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF3 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF4 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF5 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF6 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF7 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF8 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF9 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF10 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF4 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF5 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF6 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF7 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF8 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF9 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF10 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF11 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF13 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF14 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF15 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF16 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF17 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF18 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF19 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF20 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF10 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF11 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF12 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF13 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF14 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF15 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF16 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF17 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF5 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF6 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF7 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF8 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF9 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF10 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF11 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF12 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF6 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF7 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF8 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF9 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF10 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF11 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF12 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF13 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF15 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF16 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF17 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF18 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF19 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF20 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF21 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF22 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e;

enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e {
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF10 = 0,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF11 = 1,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF12 = 2,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF13 = 3,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF14 = 4,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF15 = 5,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF16 = 6,
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF17 = 7,
};

typedef enum DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e;

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF_slotSPUCCH_Format1_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF_slotSPUCCH_Format1a_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF_slotSPUCCH_Format1b_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF_slotSPUCCH_Format3_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF_slotSPUCCH_RM_Format4_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF_slotSPUCCH_TBCC_Format4_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF_subslotSPUCCH_Format1and1a_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF_subslotSPUCCH_Format1b_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF_subslotSPUCCH_RM_Format4_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF_subslotSPUCCH_TBCC_Format4_r15_Optional {
	bool d;
	DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e v;
};

struct DeltaFList_SPUCCH_r15_setup {
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1_r15_e_deltaF_slotSPUCCH_Format1_r15_Optional deltaF_slotSPUCCH_Format1_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1a_r15_e_deltaF_slotSPUCCH_Format1a_r15_Optional deltaF_slotSPUCCH_Format1a_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format1b_r15_e_deltaF_slotSPUCCH_Format1b_r15_Optional deltaF_slotSPUCCH_Format1b_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_Format3_r15_e_deltaF_slotSPUCCH_Format3_r15_Optional deltaF_slotSPUCCH_Format3_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_RM_Format4_r15_e_deltaF_slotSPUCCH_RM_Format4_r15_Optional deltaF_slotSPUCCH_RM_Format4_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_slotSPUCCH_TBCC_Format4_r15_e_deltaF_slotSPUCCH_TBCC_Format4_r15_Optional deltaF_slotSPUCCH_TBCC_Format4_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1and1a_r15_e_deltaF_subslotSPUCCH_Format1and1a_r15_Optional deltaF_subslotSPUCCH_Format1and1a_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_Format1b_r15_e_deltaF_subslotSPUCCH_Format1b_r15_Optional deltaF_subslotSPUCCH_Format1b_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_RM_Format4_r15_e_deltaF_subslotSPUCCH_RM_Format4_r15_Optional deltaF_subslotSPUCCH_RM_Format4_r15;
	struct DeltaFList_SPUCCH_r15_setup_deltaF_subslotSPUCCH_TBCC_Format4_r15_e_deltaF_subslotSPUCCH_TBCC_Format4_r15_Optional deltaF_subslotSPUCCH_TBCC_Format4_r15;
};

enum DeltaFList_SPUCCH_r15_Sel {
	DeltaFList_SPUCCH_r15_UNBOUND_VALUE = 0,
	DeltaFList_SPUCCH_r15_release = 1,
	DeltaFList_SPUCCH_r15_setup = 2,
};

union DeltaFList_SPUCCH_r15_Value {
	Null release;
	struct DeltaFList_SPUCCH_r15_setup setup;
};

struct DeltaFList_SPUCCH_r15 {
	enum DeltaFList_SPUCCH_r15_Sel d;
	union DeltaFList_SPUCCH_r15_Value v;
};

struct UplinkPowerControlCommon_v1530 {
	struct DeltaFList_SPUCCH_r15 deltaFList_SPUCCH_r15;
};

enum WUS_Config_v1560_powerBoost_r15_e {
	WUS_Config_v1560_powerBoost_r15_e_dB0 = 0,
	WUS_Config_v1560_powerBoost_r15_e_dB1dot8 = 1,
	WUS_Config_v1560_powerBoost_r15_e_dB3 = 2,
	WUS_Config_v1560_powerBoost_r15_e_dB4dot8 = 3,
};

typedef enum WUS_Config_v1560_powerBoost_r15_e WUS_Config_v1560_powerBoost_r15_e;

struct WUS_Config_v1560 {
	WUS_Config_v1560_powerBoost_r15_e powerBoost_r15;
};

enum WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e {
	WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e_n1 = 0,
	WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e_n2 = 1,
	WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e_n4 = 2,
	WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e_n8 = 3,
};

typedef enum WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e;

struct WUS_Config_v1610 {
	WUS_Config_v1610_numDRX_CyclesRelaxed_r16_e numDRX_CyclesRelaxed_r16;
};

enum HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e {
	HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e_true = 0,
};

typedef enum HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e;

enum HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e {
	HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e_true = 0,
};

typedef enum HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e;

struct HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e_highSpeedEnhMeasFlag2_r16_Optional {
	bool d;
	HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e v;
};

struct HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e_highSpeedEnhDemodFlag2_r16_Optional {
	bool d;
	HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e v;
};

struct HighSpeedConfig_v1610 {
	struct HighSpeedConfig_v1610_highSpeedEnhMeasFlag2_r16_e_highSpeedEnhMeasFlag2_r16_Optional highSpeedEnhMeasFlag2_r16;
	struct HighSpeedConfig_v1610_highSpeedEnhDemodFlag2_r16_e_highSpeedEnhDemodFlag2_r16_Optional highSpeedEnhDemodFlag2_r16;
};

enum CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e {
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB_4dot77 = 0,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB_3 = 1,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB_1dot77 = 2,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB0 = 3,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB1 = 4,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB2 = 5,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB3 = 6,
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e_dB4dot77 = 7,
};

typedef enum CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e;

struct CRS_ChEstMPDCCH_ConfigCommon_r16 {
	CRS_ChEstMPDCCH_ConfigCommon_r16_powerRatio_r16_e powerRatio_r16;
};

enum GWUS_Config_r16_groupAlternation_r16_e {
	GWUS_Config_r16_groupAlternation_r16_e_true = 0,
};

typedef enum GWUS_Config_r16_groupAlternation_r16_e GWUS_Config_r16_groupAlternation_r16_e;

enum GWUS_Config_r16_commonSequence_r16_e {
	GWUS_Config_r16_commonSequence_r16_e_g0 = 0,
	GWUS_Config_r16_commonSequence_r16_e_g126 = 1,
};

typedef enum GWUS_Config_r16_commonSequence_r16_e GWUS_Config_r16_commonSequence_r16_e;

enum GWUS_TimeParameters_r16_maxDurationFactor_r16_e {
	GWUS_TimeParameters_r16_maxDurationFactor_r16_e_one32th = 0,
	GWUS_TimeParameters_r16_maxDurationFactor_r16_e_one16th = 1,
	GWUS_TimeParameters_r16_maxDurationFactor_r16_e_one8th = 2,
	GWUS_TimeParameters_r16_maxDurationFactor_r16_e_one4th = 3,
};

typedef enum GWUS_TimeParameters_r16_maxDurationFactor_r16_e GWUS_TimeParameters_r16_maxDurationFactor_r16_e;

enum GWUS_TimeParameters_r16_numPOs_r16_e {
	GWUS_TimeParameters_r16_numPOs_r16_e_n1 = 0,
	GWUS_TimeParameters_r16_numPOs_r16_e_n2 = 1,
	GWUS_TimeParameters_r16_numPOs_r16_e_n4 = 2,
	GWUS_TimeParameters_r16_numPOs_r16_e_spare1 = 3,
};

typedef enum GWUS_TimeParameters_r16_numPOs_r16_e GWUS_TimeParameters_r16_numPOs_r16_e;

enum GWUS_TimeParameters_r16_timeOffsetDRX_r16_e {
	GWUS_TimeParameters_r16_timeOffsetDRX_r16_e_ms40 = 0,
	GWUS_TimeParameters_r16_timeOffsetDRX_r16_e_ms80 = 1,
	GWUS_TimeParameters_r16_timeOffsetDRX_r16_e_ms160 = 2,
	GWUS_TimeParameters_r16_timeOffsetDRX_r16_e_ms240 = 3,
};

typedef enum GWUS_TimeParameters_r16_timeOffsetDRX_r16_e GWUS_TimeParameters_r16_timeOffsetDRX_r16_e;

enum GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e {
	GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e_ms40 = 0,
	GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e_ms80 = 1,
	GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e_ms160 = 2,
	GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e_ms240 = 3,
};

typedef enum GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e;

enum GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e {
	GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e_ms1000 = 0,
	GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e_ms2000 = 1,
};

typedef enum GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e;

enum GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e {
	GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_n1 = 0,
	GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_n2 = 1,
	GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_n4 = 2,
	GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_n8 = 3,
};

typedef enum GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e;

enum GWUS_TimeParameters_r16_powerBoost_r16_e {
	GWUS_TimeParameters_r16_powerBoost_r16_e_dB0 = 0,
	GWUS_TimeParameters_r16_powerBoost_r16_e_dB1dot8 = 1,
	GWUS_TimeParameters_r16_powerBoost_r16_e_dB3 = 2,
	GWUS_TimeParameters_r16_powerBoost_r16_e_dB4dot8 = 3,
};

typedef enum GWUS_TimeParameters_r16_powerBoost_r16_e GWUS_TimeParameters_r16_powerBoost_r16_e;

struct GWUS_TimeParameters_r16_numPOs_r16_e_numPOs_r16_Optional {
	bool d;
	GWUS_TimeParameters_r16_numPOs_r16_e v;
};

struct GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e_timeOffset_eDRX_Long_r16_Optional {
	bool d;
	GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e v;
};

struct GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_numDRX_CyclesRelaxed_r16_Optional {
	bool d;
	GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e v;
};

struct GWUS_TimeParameters_r16_powerBoost_r16_e_powerBoost_r16_Optional {
	bool d;
	GWUS_TimeParameters_r16_powerBoost_r16_e v;
};

struct GWUS_TimeParameters_r16 {
	GWUS_TimeParameters_r16_maxDurationFactor_r16_e maxDurationFactor_r16;
	struct GWUS_TimeParameters_r16_numPOs_r16_e_numPOs_r16_Optional numPOs_r16;
	GWUS_TimeParameters_r16_timeOffsetDRX_r16_e timeOffsetDRX_r16;
	GWUS_TimeParameters_r16_timeOffset_eDRX_Short_r16_e timeOffset_eDRX_Short_r16;
	struct GWUS_TimeParameters_r16_timeOffset_eDRX_Long_r16_e_timeOffset_eDRX_Long_r16_Optional timeOffset_eDRX_Long_r16;
	struct GWUS_TimeParameters_r16_numDRX_CyclesRelaxed_r16_e_numDRX_CyclesRelaxed_r16_Optional numDRX_CyclesRelaxed_r16;
	struct GWUS_TimeParameters_r16_powerBoost_r16_e_powerBoost_r16_Optional powerBoost_r16;
};

enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e {
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e_primary = 0,
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e_secondary = 1,
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e_primary3FDM = 2,
};

typedef enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e;

enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e {
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e_n0 = 0,
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e_n2 = 1,
};

typedef enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e;

enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_Sel {
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_UNBOUND_VALUE = 0,
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS = 1,
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS = 2,
};

union GWUS_ResourceConfig_r16_resourceMappingPattern_r16_Value {
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithWUS_e resourceLocationWithWUS;
	GWUS_ResourceConfig_r16_resourceMappingPattern_r16_resourceLocationWithoutWUS_e resourceLocationWithoutWUS;
};

struct GWUS_ResourceConfig_r16_resourceMappingPattern_r16 {
	enum GWUS_ResourceConfig_r16_resourceMappingPattern_r16_Sel d;
	union GWUS_ResourceConfig_r16_resourceMappingPattern_r16_Value v;
};

enum GWUS_NumGroups_r16_e {
	GWUS_NumGroups_r16_e_n1 = 0,
	GWUS_NumGroups_r16_e_n2 = 1,
	GWUS_NumGroups_r16_e_n4 = 2,
	GWUS_NumGroups_r16_e_n8 = 3,
};

typedef enum GWUS_NumGroups_r16_e GWUS_NumGroups_r16_e;

struct GWUS_NumGroups_r16_e_GWUS_NumGroupsList_r16_Dynamic {
	size_t d;
	GWUS_NumGroups_r16_e* v;
};

typedef struct GWUS_NumGroups_r16_e_GWUS_NumGroupsList_r16_Dynamic GWUS_NumGroupsList_r16;

struct uint8_t_GWUS_GroupsForServiceList_r16_Dynamic {
	size_t d;
	uint8_t* v;
};

typedef struct uint8_t_GWUS_GroupsForServiceList_r16_Dynamic GWUS_GroupsForServiceList_r16;

struct GWUS_NumGroupsList_r16_GWUS_ResourceConfig_r16_numGroupsList_r16_Optional {
	bool d;
	GWUS_NumGroupsList_r16 v;
};

struct GWUS_GroupsForServiceList_r16_GWUS_ResourceConfig_r16_groupsForServiceList_r16_Optional {
	bool d;
	GWUS_GroupsForServiceList_r16 v;
};

struct GWUS_ResourceConfig_r16 {
	struct GWUS_ResourceConfig_r16_resourceMappingPattern_r16 resourceMappingPattern_r16;
	struct GWUS_NumGroupsList_r16_GWUS_ResourceConfig_r16_numGroupsList_r16_Optional numGroupsList_r16;
	struct GWUS_GroupsForServiceList_r16_GWUS_ResourceConfig_r16_groupsForServiceList_r16_Optional groupsForServiceList_r16;
};

enum GWUS_PagingProbThresh_r16_e {
	GWUS_PagingProbThresh_r16_e_p20 = 0,
	GWUS_PagingProbThresh_r16_e_p30 = 1,
	GWUS_PagingProbThresh_r16_e_p40 = 2,
	GWUS_PagingProbThresh_r16_e_p50 = 3,
	GWUS_PagingProbThresh_r16_e_p60 = 4,
	GWUS_PagingProbThresh_r16_e_p70 = 5,
	GWUS_PagingProbThresh_r16_e_p80 = 6,
	GWUS_PagingProbThresh_r16_e_p90 = 7,
};

typedef enum GWUS_PagingProbThresh_r16_e GWUS_PagingProbThresh_r16_e;

struct GWUS_PagingProbThresh_r16_e_GWUS_ProbThreshList_r16_Dynamic {
	size_t d;
	GWUS_PagingProbThresh_r16_e* v;
};

typedef struct GWUS_PagingProbThresh_r16_e_GWUS_ProbThreshList_r16_Dynamic GWUS_ProbThreshList_r16;

struct bool_GWUS_GroupNarrowBandList_r16_Dynamic {
	size_t d;
	bool* v;
};

typedef struct bool_GWUS_GroupNarrowBandList_r16_Dynamic GWUS_GroupNarrowBandList_r16;

struct GWUS_Config_r16_groupAlternation_r16_e_groupAlternation_r16_Optional {
	bool d;
	GWUS_Config_r16_groupAlternation_r16_e v;
};

struct GWUS_Config_r16_commonSequence_r16_e_commonSequence_r16_Optional {
	bool d;
	GWUS_Config_r16_commonSequence_r16_e v;
};

struct GWUS_TimeParameters_r16_GWUS_Config_r16_timeParameters_r16_Optional {
	bool d;
	struct GWUS_TimeParameters_r16 v;
};

struct GWUS_ResourceConfig_r16_GWUS_Config_r16_resourceConfig_eDRX_Short_r16_Optional {
	bool d;
	struct GWUS_ResourceConfig_r16 v;
};

struct GWUS_ResourceConfig_r16_GWUS_Config_r16_resourceConfig_eDRX_Long_r16_Optional {
	bool d;
	struct GWUS_ResourceConfig_r16 v;
};

struct GWUS_ProbThreshList_r16_GWUS_Config_r16_probThreshList_r16_Optional {
	bool d;
	GWUS_ProbThreshList_r16 v;
};

struct GWUS_GroupNarrowBandList_r16_GWUS_Config_r16_groupNarrowBandList_r16_Optional {
	bool d;
	GWUS_GroupNarrowBandList_r16 v;
};

struct GWUS_Config_r16 {
	struct GWUS_Config_r16_groupAlternation_r16_e_groupAlternation_r16_Optional groupAlternation_r16;
	struct GWUS_Config_r16_commonSequence_r16_e_commonSequence_r16_Optional commonSequence_r16;
	struct GWUS_TimeParameters_r16_GWUS_Config_r16_timeParameters_r16_Optional timeParameters_r16;
	struct GWUS_ResourceConfig_r16 resourceConfigDRX_r16;
	struct GWUS_ResourceConfig_r16_GWUS_Config_r16_resourceConfig_eDRX_Short_r16_Optional resourceConfig_eDRX_Short_r16;
	struct GWUS_ResourceConfig_r16_GWUS_Config_r16_resourceConfig_eDRX_Long_r16_Optional resourceConfig_eDRX_Long_r16;
	struct GWUS_ProbThreshList_r16_GWUS_Config_r16_probThreshList_r16_Optional probThreshList_r16;
	struct GWUS_GroupNarrowBandList_r16_GWUS_Config_r16_groupNarrowBandList_r16_Optional groupNarrowBandList_r16;
};

struct UplinkPowerControlCommon_v1610 {
	Alpha_r12_e alphaSRS_Add_r16;
	int8_t p0_NominalSRS_Add_r16;
};

enum RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e {
	RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e_enabled = 0,
};

typedef enum RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e;

enum RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e {
	RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e_enabled = 0,
};

typedef enum RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e;

struct UplinkPowerControlCommon_v1020_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1020_Optional {
	bool d;
	struct UplinkPowerControlCommon_v1020 v;
};

struct RACH_ConfigCommon_v1250_RadioResourceConfigCommonSIB_rach_ConfigCommon_v1250_Optional {
	bool d;
	struct RACH_ConfigCommon_v1250 v;
};

struct PUSCH_ConfigCommon_v1270_RadioResourceConfigCommonSIB_pusch_ConfigCommon_v1270_Optional {
	bool d;
	struct PUSCH_ConfigCommon_v1270 v;
};

struct BCCH_Config_v1310_RadioResourceConfigCommonSIB_bcch_Config_v1310_Optional {
	bool d;
	struct BCCH_Config_v1310 v;
};

struct PCCH_Config_v1310_RadioResourceConfigCommonSIB_pcch_Config_v1310_Optional {
	bool d;
	struct PCCH_Config_v1310 v;
};

struct FreqHoppingParameters_r13_RadioResourceConfigCommonSIB_freqHoppingParameters_r13_Optional {
	bool d;
	struct FreqHoppingParameters_r13 v;
};

struct PDSCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pdsch_ConfigCommon_v1310_Optional {
	bool d;
	struct PDSCH_ConfigCommon_v1310 v;
};

struct PUSCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pusch_ConfigCommon_v1310_Optional {
	bool d;
	struct PUSCH_ConfigCommon_v1310 v;
};

struct PRACH_ConfigSIB_v1310_RadioResourceConfigCommonSIB_prach_ConfigCommon_v1310_Optional {
	bool d;
	struct PRACH_ConfigSIB_v1310 v;
};

struct PUCCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pucch_ConfigCommon_v1310_Optional {
	bool d;
	struct PUCCH_ConfigCommon_v1310 v;
};

struct HighSpeedConfig_r14_RadioResourceConfigCommonSIB_highSpeedConfig_r14_Optional {
	bool d;
	struct HighSpeedConfig_r14 v;
};

struct PRACH_Config_v1430_RadioResourceConfigCommonSIB_prach_Config_v1430_Optional {
	bool d;
	struct PRACH_Config_v1430 v;
};

struct PUCCH_ConfigCommon_v1430_RadioResourceConfigCommonSIB_pucch_ConfigCommon_v1430_Optional {
	bool d;
	struct PUCCH_ConfigCommon_v1430 v;
};

struct PRACH_ConfigSIB_v1530_RadioResourceConfigCommonSIB_prach_Config_v1530_Optional {
	bool d;
	struct PRACH_ConfigSIB_v1530 v;
};

struct RSS_Config_r15_RadioResourceConfigCommonSIB_ce_RSS_Config_r15_Optional {
	bool d;
	struct RSS_Config_r15 v;
};

struct WUS_Config_r15_RadioResourceConfigCommonSIB_wus_Config_r15_Optional {
	bool d;
	struct WUS_Config_r15 v;
};

struct HighSpeedConfig_v1530_RadioResourceConfigCommonSIB_highSpeedConfig_v1530_Optional {
	bool d;
	struct HighSpeedConfig_v1530 v;
};

struct UplinkPowerControlCommon_v1530_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1540_Optional {
	bool d;
	struct UplinkPowerControlCommon_v1530 v;
};

struct WUS_Config_v1560_RadioResourceConfigCommonSIB_wus_Config_v1560_Optional {
	bool d;
	struct WUS_Config_v1560 v;
};

struct WUS_Config_v1610_RadioResourceConfigCommonSIB_wus_Config_v1610_Optional {
	bool d;
	struct WUS_Config_v1610 v;
};

struct HighSpeedConfig_v1610_RadioResourceConfigCommonSIB_highSpeedConfig_v1610_Optional {
	bool d;
	struct HighSpeedConfig_v1610 v;
};

struct CRS_ChEstMPDCCH_ConfigCommon_r16_RadioResourceConfigCommonSIB_crs_ChEstMPDCCH_ConfigCommon_r16_Optional {
	bool d;
	struct CRS_ChEstMPDCCH_ConfigCommon_r16 v;
};

struct GWUS_Config_r16_RadioResourceConfigCommonSIB_gwus_Config_r16_Optional {
	bool d;
	struct GWUS_Config_r16 v;
};

struct UplinkPowerControlCommon_v1610_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1610_Optional {
	bool d;
	struct UplinkPowerControlCommon_v1610 v;
};

struct RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e_rss_MeasConfig_r16_Optional {
	bool d;
	RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e v;
};

struct RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e_rss_MeasNonNCL_r16_Optional {
	bool d;
	RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e v;
};

struct B2_RadioResourceConfigCommonSIB_puncturedSubcarriersDL_r16_Optional {
	bool d;
	B2 v;
};

struct bool_RadioResourceConfigCommonSIB_highSpeedInterRAT_NR_r16_Optional {
	bool d;
	bool v;
};

struct RadioResourceConfigCommonSIB {
	struct RACH_ConfigCommon rach_ConfigCommon;
	struct BCCH_Config bcch_Config;
	struct PCCH_Config pcch_Config;
	struct PRACH_ConfigSIB prach_Config;
	struct PDSCH_ConfigCommon pdsch_ConfigCommon;
	struct PUSCH_ConfigCommon pusch_ConfigCommon;
	struct PUCCH_ConfigCommon pucch_ConfigCommon;
	struct SoundingRS_UL_ConfigCommon soundingRS_UL_ConfigCommon;
	struct UplinkPowerControlCommon uplinkPowerControlCommon;
	UL_CyclicPrefixLength_e ul_CyclicPrefixLength;
	struct UplinkPowerControlCommon_v1020_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1020_Optional uplinkPowerControlCommon_v1020;
	struct RACH_ConfigCommon_v1250_RadioResourceConfigCommonSIB_rach_ConfigCommon_v1250_Optional rach_ConfigCommon_v1250;
	struct PUSCH_ConfigCommon_v1270_RadioResourceConfigCommonSIB_pusch_ConfigCommon_v1270_Optional pusch_ConfigCommon_v1270;
	struct BCCH_Config_v1310_RadioResourceConfigCommonSIB_bcch_Config_v1310_Optional bcch_Config_v1310;
	struct PCCH_Config_v1310_RadioResourceConfigCommonSIB_pcch_Config_v1310_Optional pcch_Config_v1310;
	struct FreqHoppingParameters_r13_RadioResourceConfigCommonSIB_freqHoppingParameters_r13_Optional freqHoppingParameters_r13;
	struct PDSCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pdsch_ConfigCommon_v1310_Optional pdsch_ConfigCommon_v1310;
	struct PUSCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pusch_ConfigCommon_v1310_Optional pusch_ConfigCommon_v1310;
	struct PRACH_ConfigSIB_v1310_RadioResourceConfigCommonSIB_prach_ConfigCommon_v1310_Optional prach_ConfigCommon_v1310;
	struct PUCCH_ConfigCommon_v1310_RadioResourceConfigCommonSIB_pucch_ConfigCommon_v1310_Optional pucch_ConfigCommon_v1310;
	struct HighSpeedConfig_r14_RadioResourceConfigCommonSIB_highSpeedConfig_r14_Optional highSpeedConfig_r14;
	struct PRACH_Config_v1430_RadioResourceConfigCommonSIB_prach_Config_v1430_Optional prach_Config_v1430;
	struct PUCCH_ConfigCommon_v1430_RadioResourceConfigCommonSIB_pucch_ConfigCommon_v1430_Optional pucch_ConfigCommon_v1430;
	struct PRACH_ConfigSIB_v1530_RadioResourceConfigCommonSIB_prach_Config_v1530_Optional prach_Config_v1530;
	struct RSS_Config_r15_RadioResourceConfigCommonSIB_ce_RSS_Config_r15_Optional ce_RSS_Config_r15;
	struct WUS_Config_r15_RadioResourceConfigCommonSIB_wus_Config_r15_Optional wus_Config_r15;
	struct HighSpeedConfig_v1530_RadioResourceConfigCommonSIB_highSpeedConfig_v1530_Optional highSpeedConfig_v1530;
	struct UplinkPowerControlCommon_v1530_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1540_Optional uplinkPowerControlCommon_v1540;
	struct WUS_Config_v1560_RadioResourceConfigCommonSIB_wus_Config_v1560_Optional wus_Config_v1560;
	struct WUS_Config_v1610_RadioResourceConfigCommonSIB_wus_Config_v1610_Optional wus_Config_v1610;
	struct HighSpeedConfig_v1610_RadioResourceConfigCommonSIB_highSpeedConfig_v1610_Optional highSpeedConfig_v1610;
	struct CRS_ChEstMPDCCH_ConfigCommon_r16_RadioResourceConfigCommonSIB_crs_ChEstMPDCCH_ConfigCommon_r16_Optional crs_ChEstMPDCCH_ConfigCommon_r16;
	struct GWUS_Config_r16_RadioResourceConfigCommonSIB_gwus_Config_r16_Optional gwus_Config_r16;
	struct UplinkPowerControlCommon_v1610_RadioResourceConfigCommonSIB_uplinkPowerControlCommon_v1610_Optional uplinkPowerControlCommon_v1610;
	struct RadioResourceConfigCommonSIB_rss_MeasConfig_r16_e_rss_MeasConfig_r16_Optional rss_MeasConfig_r16;
	struct RadioResourceConfigCommonSIB_rss_MeasNonNCL_r16_e_rss_MeasNonNCL_r16_Optional rss_MeasNonNCL_r16;
	struct B2_RadioResourceConfigCommonSIB_puncturedSubcarriersDL_r16_Optional puncturedSubcarriersDL_r16;
	struct bool_RadioResourceConfigCommonSIB_highSpeedInterRAT_NR_r16_Optional highSpeedInterRAT_NR_r16;
};

enum UE_TimersAndConstants_t300_e {
	UE_TimersAndConstants_t300_e_ms100 = 0,
	UE_TimersAndConstants_t300_e_ms200 = 1,
	UE_TimersAndConstants_t300_e_ms300 = 2,
	UE_TimersAndConstants_t300_e_ms400 = 3,
	UE_TimersAndConstants_t300_e_ms600 = 4,
	UE_TimersAndConstants_t300_e_ms1000 = 5,
	UE_TimersAndConstants_t300_e_ms1500 = 6,
	UE_TimersAndConstants_t300_e_ms2000 = 7,
};

typedef enum UE_TimersAndConstants_t300_e UE_TimersAndConstants_t300_e;

enum UE_TimersAndConstants_t301_e {
	UE_TimersAndConstants_t301_e_ms100 = 0,
	UE_TimersAndConstants_t301_e_ms200 = 1,
	UE_TimersAndConstants_t301_e_ms300 = 2,
	UE_TimersAndConstants_t301_e_ms400 = 3,
	UE_TimersAndConstants_t301_e_ms600 = 4,
	UE_TimersAndConstants_t301_e_ms1000 = 5,
	UE_TimersAndConstants_t301_e_ms1500 = 6,
	UE_TimersAndConstants_t301_e_ms2000 = 7,
};

typedef enum UE_TimersAndConstants_t301_e UE_TimersAndConstants_t301_e;

enum UE_TimersAndConstants_t310_e {
	UE_TimersAndConstants_t310_e_ms0 = 0,
	UE_TimersAndConstants_t310_e_ms50 = 1,
	UE_TimersAndConstants_t310_e_ms100 = 2,
	UE_TimersAndConstants_t310_e_ms200 = 3,
	UE_TimersAndConstants_t310_e_ms500 = 4,
	UE_TimersAndConstants_t310_e_ms1000 = 5,
	UE_TimersAndConstants_t310_e_ms2000 = 6,
};

typedef enum UE_TimersAndConstants_t310_e UE_TimersAndConstants_t310_e;

enum UE_TimersAndConstants_n310_e {
	UE_TimersAndConstants_n310_e_n1 = 0,
	UE_TimersAndConstants_n310_e_n2 = 1,
	UE_TimersAndConstants_n310_e_n3 = 2,
	UE_TimersAndConstants_n310_e_n4 = 3,
	UE_TimersAndConstants_n310_e_n6 = 4,
	UE_TimersAndConstants_n310_e_n8 = 5,
	UE_TimersAndConstants_n310_e_n10 = 6,
	UE_TimersAndConstants_n310_e_n20 = 7,
};

typedef enum UE_TimersAndConstants_n310_e UE_TimersAndConstants_n310_e;

enum UE_TimersAndConstants_t311_e {
	UE_TimersAndConstants_t311_e_ms1000 = 0,
	UE_TimersAndConstants_t311_e_ms3000 = 1,
	UE_TimersAndConstants_t311_e_ms5000 = 2,
	UE_TimersAndConstants_t311_e_ms10000 = 3,
	UE_TimersAndConstants_t311_e_ms15000 = 4,
	UE_TimersAndConstants_t311_e_ms20000 = 5,
	UE_TimersAndConstants_t311_e_ms30000 = 6,
};

typedef enum UE_TimersAndConstants_t311_e UE_TimersAndConstants_t311_e;

enum UE_TimersAndConstants_n311_e {
	UE_TimersAndConstants_n311_e_n1 = 0,
	UE_TimersAndConstants_n311_e_n2 = 1,
	UE_TimersAndConstants_n311_e_n3 = 2,
	UE_TimersAndConstants_n311_e_n4 = 3,
	UE_TimersAndConstants_n311_e_n5 = 4,
	UE_TimersAndConstants_n311_e_n6 = 5,
	UE_TimersAndConstants_n311_e_n8 = 6,
	UE_TimersAndConstants_n311_e_n10 = 7,
};

typedef enum UE_TimersAndConstants_n311_e UE_TimersAndConstants_n311_e;

enum UE_TimersAndConstants_t300_v1310_e {
	UE_TimersAndConstants_t300_v1310_e_ms2500 = 0,
	UE_TimersAndConstants_t300_v1310_e_ms3000 = 1,
	UE_TimersAndConstants_t300_v1310_e_ms3500 = 2,
	UE_TimersAndConstants_t300_v1310_e_ms4000 = 3,
	UE_TimersAndConstants_t300_v1310_e_ms5000 = 4,
	UE_TimersAndConstants_t300_v1310_e_ms6000 = 5,
	UE_TimersAndConstants_t300_v1310_e_ms8000 = 6,
	UE_TimersAndConstants_t300_v1310_e_ms10000 = 7,
};

typedef enum UE_TimersAndConstants_t300_v1310_e UE_TimersAndConstants_t300_v1310_e;

enum UE_TimersAndConstants_t301_v1310_e {
	UE_TimersAndConstants_t301_v1310_e_ms2500 = 0,
	UE_TimersAndConstants_t301_v1310_e_ms3000 = 1,
	UE_TimersAndConstants_t301_v1310_e_ms3500 = 2,
	UE_TimersAndConstants_t301_v1310_e_ms4000 = 3,
	UE_TimersAndConstants_t301_v1310_e_ms5000 = 4,
	UE_TimersAndConstants_t301_v1310_e_ms6000 = 5,
	UE_TimersAndConstants_t301_v1310_e_ms8000 = 6,
	UE_TimersAndConstants_t301_v1310_e_ms10000 = 7,
};

typedef enum UE_TimersAndConstants_t301_v1310_e UE_TimersAndConstants_t301_v1310_e;

enum UE_TimersAndConstants_t310_v1330_e {
	UE_TimersAndConstants_t310_v1330_e_ms4000 = 0,
	UE_TimersAndConstants_t310_v1330_e_ms6000 = 1,
};

typedef enum UE_TimersAndConstants_t310_v1330_e UE_TimersAndConstants_t310_v1330_e;

enum UE_TimersAndConstants_t300_r15_e {
	UE_TimersAndConstants_t300_r15_e_ms4000 = 0,
	UE_TimersAndConstants_t300_r15_e_ms6000 = 1,
	UE_TimersAndConstants_t300_r15_e_ms8000 = 2,
	UE_TimersAndConstants_t300_r15_e_ms10000 = 3,
	UE_TimersAndConstants_t300_r15_e_ms15000 = 4,
	UE_TimersAndConstants_t300_r15_e_ms25000 = 5,
	UE_TimersAndConstants_t300_r15_e_ms40000 = 6,
	UE_TimersAndConstants_t300_r15_e_ms60000 = 7,
};

typedef enum UE_TimersAndConstants_t300_r15_e UE_TimersAndConstants_t300_r15_e;

struct UE_TimersAndConstants_t300_v1310_e_t300_v1310_Optional {
	bool d;
	UE_TimersAndConstants_t300_v1310_e v;
};

struct UE_TimersAndConstants_t301_v1310_e_t301_v1310_Optional {
	bool d;
	UE_TimersAndConstants_t301_v1310_e v;
};

struct UE_TimersAndConstants_t310_v1330_e_t310_v1330_Optional {
	bool d;
	UE_TimersAndConstants_t310_v1330_e v;
};

struct UE_TimersAndConstants_t300_r15_e_t300_r15_Optional {
	bool d;
	UE_TimersAndConstants_t300_r15_e v;
};

struct UE_TimersAndConstants {
	UE_TimersAndConstants_t300_e t300;
	UE_TimersAndConstants_t301_e t301;
	UE_TimersAndConstants_t310_e t310;
	UE_TimersAndConstants_n310_e n310;
	UE_TimersAndConstants_t311_e t311;
	UE_TimersAndConstants_n311_e n311;
	struct UE_TimersAndConstants_t300_v1310_e_t300_v1310_Optional t300_v1310;
	struct UE_TimersAndConstants_t301_v1310_e_t301_v1310_Optional t301_v1310;
	struct UE_TimersAndConstants_t310_v1330_e_t310_v1330_Optional t310_v1330;
	struct UE_TimersAndConstants_t300_r15_e_t300_r15_Optional t300_r15;
};

typedef uint16_t ARFCN_ValueEUTRA;

enum SystemInformationBlockType2_freqInfo_ul_Bandwidth_e {
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n6 = 0,
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n15 = 1,
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n25 = 2,
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n50 = 3,
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n75 = 4,
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_n100 = 5,
};

typedef enum SystemInformationBlockType2_freqInfo_ul_Bandwidth_e SystemInformationBlockType2_freqInfo_ul_Bandwidth_e;

typedef uint8_t AdditionalSpectrumEmission;

struct ARFCN_ValueEUTRA_SystemInformationBlockType2_freqInfo_ul_CarrierFreq_Optional {
	bool d;
	ARFCN_ValueEUTRA v;
};

struct SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_ul_Bandwidth_Optional {
	bool d;
	SystemInformationBlockType2_freqInfo_ul_Bandwidth_e v;
};

struct SystemInformationBlockType2_freqInfo {
	struct ARFCN_ValueEUTRA_SystemInformationBlockType2_freqInfo_ul_CarrierFreq_Optional ul_CarrierFreq;
	struct SystemInformationBlockType2_freqInfo_ul_Bandwidth_e_ul_Bandwidth_Optional ul_Bandwidth;
	AdditionalSpectrumEmission additionalSpectrumEmission;
};

enum MBSFN_SubframeConfig_radioframeAllocationPeriod_e {
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n1 = 0,
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n2 = 1,
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n4 = 2,
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n8 = 3,
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n16 = 4,
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e_n32 = 5,
};

typedef enum MBSFN_SubframeConfig_radioframeAllocationPeriod_e MBSFN_SubframeConfig_radioframeAllocationPeriod_e;

enum MBSFN_SubframeConfig_subframeAllocation_Sel {
	MBSFN_SubframeConfig_subframeAllocation_UNBOUND_VALUE = 0,
	MBSFN_SubframeConfig_subframeAllocation_oneFrame = 1,
	MBSFN_SubframeConfig_subframeAllocation_fourFrames = 2,
};

union MBSFN_SubframeConfig_subframeAllocation_Value {
	B6 oneFrame;
	B24 fourFrames;
};

struct MBSFN_SubframeConfig_subframeAllocation {
	enum MBSFN_SubframeConfig_subframeAllocation_Sel d;
	union MBSFN_SubframeConfig_subframeAllocation_Value v;
};

struct MBSFN_SubframeConfig {
	MBSFN_SubframeConfig_radioframeAllocationPeriod_e radioframeAllocationPeriod;
	uint8_t radioframeAllocationOffset;
	struct MBSFN_SubframeConfig_subframeAllocation subframeAllocation;
};

struct MBSFN_SubframeConfig_MBSFN_SubframeConfigList_Dynamic {
	size_t d;
	struct MBSFN_SubframeConfig* v;
};

typedef struct MBSFN_SubframeConfig_MBSFN_SubframeConfigList_Dynamic MBSFN_SubframeConfigList;

enum TimeAlignmentTimer_e {
	TimeAlignmentTimer_e_sf500 = 0,
	TimeAlignmentTimer_e_sf750 = 1,
	TimeAlignmentTimer_e_sf1280 = 2,
	TimeAlignmentTimer_e_sf1920 = 3,
	TimeAlignmentTimer_e_sf2560 = 4,
	TimeAlignmentTimer_e_sf5120 = 5,
	TimeAlignmentTimer_e_sf10240 = 6,
	TimeAlignmentTimer_e_infinity = 7,
};

typedef enum TimeAlignmentTimer_e TimeAlignmentTimer_e;

enum SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e {
	SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e_true = 0,
};

typedef enum SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e;

enum SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e {
	SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e_true = 0,
};

typedef enum SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e;

enum SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e {
	SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e_true = 0,
};

typedef enum SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e;

struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringForMO_Signalling_r12_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringForMO_Data_r12_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringPerPLMN_r12_ac_BarringInfo_r12 {
	bool ac_BarringForEmergency_r12;
	struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringForMO_Signalling_r12_Optional ac_BarringForMO_Signalling_r12;
	struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringForMO_Data_r12_Optional ac_BarringForMO_Data_r12;
};

enum AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e {
	AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e_true = 0,
};

typedef enum AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e;

enum AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e {
	AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e_true = 0,
};

typedef enum AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e;

enum AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e {
	AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e_true = 0,
};

typedef enum AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e;

struct AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringInfo_r12_Optional {
	bool d;
	struct AC_BarringPerPLMN_r12_ac_BarringInfo_r12 v;
};

struct AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e_ac_BarringSkipForMMTELVoice_r12_Optional {
	bool d;
	AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e v;
};

struct AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e_ac_BarringSkipForMMTELVideo_r12_Optional {
	bool d;
	AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e v;
};

struct AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e_ac_BarringSkipForSMS_r12_Optional {
	bool d;
	AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e v;
};

struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringForCSFB_r12_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_AC_BarringPerPLMN_r12_ssac_BarringForMMTEL_Voice_r12_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_AC_BarringPerPLMN_r12_ssac_BarringForMMTEL_Video_r12_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringPerPLMN_r12 {
	uint8_t plmn_IdentityIndex_r12;
	struct AC_BarringPerPLMN_r12_ac_BarringInfo_r12_ac_BarringInfo_r12_Optional ac_BarringInfo_r12;
	struct AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVoice_r12_e_ac_BarringSkipForMMTELVoice_r12_Optional ac_BarringSkipForMMTELVoice_r12;
	struct AC_BarringPerPLMN_r12_ac_BarringSkipForMMTELVideo_r12_e_ac_BarringSkipForMMTELVideo_r12_Optional ac_BarringSkipForMMTELVideo_r12;
	struct AC_BarringPerPLMN_r12_ac_BarringSkipForSMS_r12_e_ac_BarringSkipForSMS_r12_Optional ac_BarringSkipForSMS_r12;
	struct AC_BarringConfig_AC_BarringPerPLMN_r12_ac_BarringForCSFB_r12_Optional ac_BarringForCSFB_r12;
	struct AC_BarringConfig_AC_BarringPerPLMN_r12_ssac_BarringForMMTEL_Voice_r12_Optional ssac_BarringForMMTEL_Voice_r12;
	struct AC_BarringConfig_AC_BarringPerPLMN_r12_ssac_BarringForMMTEL_Video_r12_Optional ssac_BarringForMMTEL_Video_r12;
};

struct AC_BarringPerPLMN_r12_AC_BarringPerPLMN_List_r12_Dynamic {
	size_t d;
	struct AC_BarringPerPLMN_r12* v;
};

typedef struct AC_BarringPerPLMN_r12_AC_BarringPerPLMN_List_r12_Dynamic AC_BarringPerPLMN_List_r12;

enum SystemInformationBlockType2_voiceServiceCauseIndication_r12_e {
	SystemInformationBlockType2_voiceServiceCauseIndication_r12_e_true = 0,
};

typedef enum SystemInformationBlockType2_voiceServiceCauseIndication_r12_e SystemInformationBlockType2_voiceServiceCauseIndication_r12_e;

enum BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e {
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p00 = 0,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p05 = 1,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p10 = 2,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p15 = 3,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p20 = 4,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p25 = 5,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p30 = 6,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p40 = 7,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p50 = 8,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p60 = 9,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p70 = 10,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p75 = 11,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p80 = 12,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p85 = 13,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p90 = 14,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e_p95 = 15,
};

typedef enum BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e;

enum BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e {
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s4 = 0,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s8 = 1,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s16 = 2,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s32 = 3,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s64 = 4,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s128 = 5,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s256 = 6,
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e_s512 = 7,
};

typedef enum BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e;

struct BarringPerACDC_Category_r13_acdc_BarringConfig_r13 {
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringFactor_r13_e ac_BarringFactor_r13;
	BarringPerACDC_Category_r13_acdc_BarringConfig_r13_ac_BarringTime_r13_e ac_BarringTime_r13;
};

struct BarringPerACDC_Category_r13_acdc_BarringConfig_r13_acdc_BarringConfig_r13_Optional {
	bool d;
	struct BarringPerACDC_Category_r13_acdc_BarringConfig_r13 v;
};

struct BarringPerACDC_Category_r13 {
	uint8_t acdc_Category_r13;
	struct BarringPerACDC_Category_r13_acdc_BarringConfig_r13_acdc_BarringConfig_r13_Optional acdc_BarringConfig_r13;
};

struct BarringPerACDC_Category_r13_BarringPerACDC_CategoryList_r13_Dynamic {
	size_t d;
	struct BarringPerACDC_Category_r13* v;
};

typedef struct BarringPerACDC_Category_r13_BarringPerACDC_CategoryList_r13_Dynamic BarringPerACDC_CategoryList_r13;

struct ACDC_BarringForCommon_r13 {
	bool acdc_HPLMNonly_r13;
	BarringPerACDC_CategoryList_r13 barringPerACDC_CategoryList_r13;
};

struct ACDC_BarringPerPLMN_r13 {
	uint8_t plmn_IdentityIndex_r13;
	bool acdc_OnlyForHPLMN_r13;
	BarringPerACDC_CategoryList_r13 barringPerACDC_CategoryList_r13;
};

struct ACDC_BarringPerPLMN_r13_ACDC_BarringPerPLMN_List_r13_Dynamic {
	size_t d;
	struct ACDC_BarringPerPLMN_r13* v;
};

typedef struct ACDC_BarringPerPLMN_r13_ACDC_BarringPerPLMN_List_r13_Dynamic ACDC_BarringPerPLMN_List_r13;

enum UDT_Restricting_r13_udt_Restricting_r13_e {
	UDT_Restricting_r13_udt_Restricting_r13_e_true = 0,
};

typedef enum UDT_Restricting_r13_udt_Restricting_r13_e UDT_Restricting_r13_udt_Restricting_r13_e;

enum UDT_Restricting_r13_udt_RestrictingTime_r13_e {
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s4 = 0,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s8 = 1,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s16 = 2,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s32 = 3,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s64 = 4,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s128 = 5,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s256 = 6,
	UDT_Restricting_r13_udt_RestrictingTime_r13_e_s512 = 7,
};

typedef enum UDT_Restricting_r13_udt_RestrictingTime_r13_e UDT_Restricting_r13_udt_RestrictingTime_r13_e;

struct UDT_Restricting_r13_udt_Restricting_r13_e_udt_Restricting_r13_Optional {
	bool d;
	UDT_Restricting_r13_udt_Restricting_r13_e v;
};

struct UDT_Restricting_r13_udt_RestrictingTime_r13_e_udt_RestrictingTime_r13_Optional {
	bool d;
	UDT_Restricting_r13_udt_RestrictingTime_r13_e v;
};

struct UDT_Restricting_r13 {
	struct UDT_Restricting_r13_udt_Restricting_r13_e_udt_Restricting_r13_Optional udt_Restricting_r13;
	struct UDT_Restricting_r13_udt_RestrictingTime_r13_e_udt_RestrictingTime_r13_Optional udt_RestrictingTime_r13;
};

struct UDT_Restricting_r13_UDT_RestrictingPerPLMN_r13_udt_Restricting_r13_Optional {
	bool d;
	struct UDT_Restricting_r13 v;
};

struct UDT_RestrictingPerPLMN_r13 {
	uint8_t plmn_IdentityIndex_r13;
	struct UDT_Restricting_r13_UDT_RestrictingPerPLMN_r13_udt_Restricting_r13_Optional udt_Restricting_r13;
};

struct UDT_RestrictingPerPLMN_r13_UDT_RestrictingPerPLMN_List_r13_Dynamic {
	size_t d;
	struct UDT_RestrictingPerPLMN_r13* v;
};

typedef struct UDT_RestrictingPerPLMN_r13_UDT_RestrictingPerPLMN_List_r13_Dynamic UDT_RestrictingPerPLMN_List_r13;

enum CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e {
	CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e_true = 0,
};

typedef enum CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e;

enum CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e {
	CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e_true = 0,
};

typedef enum CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e;

enum CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e {
	CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e_true = 0,
};

typedef enum CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e;

struct CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e_up_CIoT_EPS_Optimisation_r13_Optional {
	bool d;
	CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e v;
};

struct CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e_cp_CIoT_EPS_Optimisation_r13_Optional {
	bool d;
	CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e v;
};

struct CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e_attachWithoutPDN_Connectivity_r13_Optional {
	bool d;
	CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e v;
};

struct CIOT_OptimisationPLMN_r13 {
	struct CIOT_OptimisationPLMN_r13_up_CIoT_EPS_Optimisation_r13_e_up_CIoT_EPS_Optimisation_r13_Optional up_CIoT_EPS_Optimisation_r13;
	struct CIOT_OptimisationPLMN_r13_cp_CIoT_EPS_Optimisation_r13_e_cp_CIoT_EPS_Optimisation_r13_Optional cp_CIoT_EPS_Optimisation_r13;
	struct CIOT_OptimisationPLMN_r13_attachWithoutPDN_Connectivity_r13_e_attachWithoutPDN_Connectivity_r13_Optional attachWithoutPDN_Connectivity_r13;
};

struct CIOT_OptimisationPLMN_r13_CIOT_EPS_OptimisationInfo_r13_Dynamic {
	size_t d;
	struct CIOT_OptimisationPLMN_r13* v;
};

typedef struct CIOT_OptimisationPLMN_r13_CIOT_EPS_OptimisationInfo_r13_Dynamic CIOT_EPS_OptimisationInfo_r13;

enum SystemInformationBlockType2_useFullResumeID_r13_e {
	SystemInformationBlockType2_useFullResumeID_r13_e_true = 0,
};

typedef enum SystemInformationBlockType2_useFullResumeID_r13_e SystemInformationBlockType2_useFullResumeID_r13_e;

enum SystemInformationBlockType2_unicastFreqHoppingInd_r13_e {
	SystemInformationBlockType2_unicastFreqHoppingInd_r13_e_true = 0,
};

typedef enum SystemInformationBlockType2_unicastFreqHoppingInd_r13_e SystemInformationBlockType2_unicastFreqHoppingInd_r13_e;

enum MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_Sel {
	MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_UNBOUND_VALUE = 0,
	MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_oneFrame_v1430 = 1,
	MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_fourFrames_v1430 = 2,
};

union MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_Value {
	B2 oneFrame_v1430;
	B8 fourFrames_v1430;
};

struct MBSFN_SubframeConfig_v1430_subframeAllocation_v1430 {
	enum MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_Sel d;
	union MBSFN_SubframeConfig_v1430_subframeAllocation_v1430_Value v;
};

struct MBSFN_SubframeConfig_v1430 {
	struct MBSFN_SubframeConfig_v1430_subframeAllocation_v1430 subframeAllocation_v1430;
};

struct MBSFN_SubframeConfig_v1430_MBSFN_SubframeConfigList_v1430_Dynamic {
	size_t d;
	struct MBSFN_SubframeConfig_v1430* v;
};

typedef struct MBSFN_SubframeConfig_v1430_MBSFN_SubframeConfigList_v1430_Dynamic MBSFN_SubframeConfigList_v1430;

enum SystemInformationBlockType2_videoServiceCauseIndication_r14_e {
	SystemInformationBlockType2_videoServiceCauseIndication_r14_e_true = 0,
};

typedef enum SystemInformationBlockType2_videoServiceCauseIndication_r14_e SystemInformationBlockType2_videoServiceCauseIndication_r14_e;

enum PLMN_Info_r15_upperLayerIndication_r15_e {
	PLMN_Info_r15_upperLayerIndication_r15_e_true = 0,
};

typedef enum PLMN_Info_r15_upperLayerIndication_r15_e PLMN_Info_r15_upperLayerIndication_r15_e;

struct PLMN_Info_r15_upperLayerIndication_r15_e_upperLayerIndication_r15_Optional {
	bool d;
	PLMN_Info_r15_upperLayerIndication_r15_e v;
};

struct PLMN_Info_r15 {
	struct PLMN_Info_r15_upperLayerIndication_r15_e_upperLayerIndication_r15_Optional upperLayerIndication_r15;
};

struct PLMN_Info_r15_PLMN_InfoList_r15_Dynamic {
	size_t d;
	struct PLMN_Info_r15* v;
};

typedef struct PLMN_Info_r15_PLMN_InfoList_r15_Dynamic PLMN_InfoList_r15;

enum SystemInformationBlockType2_cp_EDT_r15_e {
	SystemInformationBlockType2_cp_EDT_r15_e_true = 0,
};

typedef enum SystemInformationBlockType2_cp_EDT_r15_e SystemInformationBlockType2_cp_EDT_r15_e;

enum SystemInformationBlockType2_up_EDT_r15_e {
	SystemInformationBlockType2_up_EDT_r15_e_true = 0,
};

typedef enum SystemInformationBlockType2_up_EDT_r15_e SystemInformationBlockType2_up_EDT_r15_e;

enum SystemInformationBlockType2_idleModeMeasurements_r15_e {
	SystemInformationBlockType2_idleModeMeasurements_r15_e_true = 0,
};

typedef enum SystemInformationBlockType2_idleModeMeasurements_r15_e SystemInformationBlockType2_idleModeMeasurements_r15_e;

enum SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e {
	SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e_true = 0,
};

typedef enum SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e;

enum SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e {
	SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e_true = 0,
};

typedef enum SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e;

enum SystemInformationBlockType2_rlos_Enabled_r16_e {
	SystemInformationBlockType2_rlos_Enabled_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_rlos_Enabled_r16_e SystemInformationBlockType2_rlos_Enabled_r16_e;

enum SystemInformationBlockType2_earlySecurityReactivation_r16_e {
	SystemInformationBlockType2_earlySecurityReactivation_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_earlySecurityReactivation_r16_e SystemInformationBlockType2_earlySecurityReactivation_r16_e;

enum SystemInformationBlockType2_cp_EDT_5GC_r16_e {
	SystemInformationBlockType2_cp_EDT_5GC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_cp_EDT_5GC_r16_e SystemInformationBlockType2_cp_EDT_5GC_r16_e;

enum SystemInformationBlockType2_up_EDT_5GC_r16_e {
	SystemInformationBlockType2_up_EDT_5GC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_up_EDT_5GC_r16_e SystemInformationBlockType2_up_EDT_5GC_r16_e;

enum SystemInformationBlockType2_cp_PUR_EPC_r16_e {
	SystemInformationBlockType2_cp_PUR_EPC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_cp_PUR_EPC_r16_e SystemInformationBlockType2_cp_PUR_EPC_r16_e;

enum SystemInformationBlockType2_up_PUR_EPC_r16_e {
	SystemInformationBlockType2_up_PUR_EPC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_up_PUR_EPC_r16_e SystemInformationBlockType2_up_PUR_EPC_r16_e;

enum SystemInformationBlockType2_cp_PUR_5GC_r16_e {
	SystemInformationBlockType2_cp_PUR_5GC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_cp_PUR_5GC_r16_e SystemInformationBlockType2_cp_PUR_5GC_r16_e;

enum SystemInformationBlockType2_up_PUR_5GC_r16_e {
	SystemInformationBlockType2_up_PUR_5GC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_up_PUR_5GC_r16_e SystemInformationBlockType2_up_PUR_5GC_r16_e;

enum SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e {
	SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e_fourBits = 0,
	SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e_both = 1,
};

typedef enum SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e;

enum SystemInformationBlockType2_rai_ActivationEnh_r16_e {
	SystemInformationBlockType2_rai_ActivationEnh_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_rai_ActivationEnh_r16_e SystemInformationBlockType2_rai_ActivationEnh_r16_e;

enum SystemInformationBlockType2_idleModeMeasurementsNR_r16_e {
	SystemInformationBlockType2_idleModeMeasurementsNR_r16_e_true = 0,
};

typedef enum SystemInformationBlockType2_idleModeMeasurementsNR_r16_e SystemInformationBlockType2_idleModeMeasurementsNR_r16_e;

struct SystemInformationBlockType2_ac_BarringInfo_ac_BarringInfo_Optional {
	bool d;
	struct SystemInformationBlockType2_ac_BarringInfo v;
};

struct MBSFN_SubframeConfigList_SystemInformationBlockType2_mbsfn_SubframeConfigList_Optional {
	bool d;
	MBSFN_SubframeConfigList v;
};

struct OCTET_STRING_SystemInformationBlockType2_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct AC_BarringConfig_SystemInformationBlockType2_ssac_BarringForMMTEL_Voice_r9_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_SystemInformationBlockType2_ssac_BarringForMMTEL_Video_r9_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringForCSFB_r10_Optional {
	bool d;
	struct AC_BarringConfig v;
};

struct SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e_ac_BarringSkipForMMTELVoice_r12_Optional {
	bool d;
	SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e v;
};

struct SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e_ac_BarringSkipForMMTELVideo_r12_Optional {
	bool d;
	SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e v;
};

struct SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e_ac_BarringSkipForSMS_r12_Optional {
	bool d;
	SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e v;
};

struct AC_BarringPerPLMN_List_r12_SystemInformationBlockType2_ac_BarringPerPLMN_List_r12_Optional {
	bool d;
	AC_BarringPerPLMN_List_r12 v;
};

struct SystemInformationBlockType2_voiceServiceCauseIndication_r12_e_voiceServiceCauseIndication_r12_Optional {
	bool d;
	SystemInformationBlockType2_voiceServiceCauseIndication_r12_e v;
};

struct ACDC_BarringForCommon_r13_SystemInformationBlockType2_acdc_BarringForCommon_r13_Optional {
	bool d;
	struct ACDC_BarringForCommon_r13 v;
};

struct ACDC_BarringPerPLMN_List_r13_SystemInformationBlockType2_acdc_BarringPerPLMN_List_r13_Optional {
	bool d;
	ACDC_BarringPerPLMN_List_r13 v;
};

struct UDT_Restricting_r13_SystemInformationBlockType2_udt_RestrictingForCommon_r13_Optional {
	bool d;
	struct UDT_Restricting_r13 v;
};

struct UDT_RestrictingPerPLMN_List_r13_SystemInformationBlockType2_udt_RestrictingPerPLMN_List_r13_Optional {
	bool d;
	UDT_RestrictingPerPLMN_List_r13 v;
};

struct CIOT_EPS_OptimisationInfo_r13_SystemInformationBlockType2_cIoT_EPS_OptimisationInfo_r13_Optional {
	bool d;
	CIOT_EPS_OptimisationInfo_r13 v;
};

struct SystemInformationBlockType2_useFullResumeID_r13_e_useFullResumeID_r13_Optional {
	bool d;
	SystemInformationBlockType2_useFullResumeID_r13_e v;
};

struct SystemInformationBlockType2_unicastFreqHoppingInd_r13_e_unicastFreqHoppingInd_r13_Optional {
	bool d;
	SystemInformationBlockType2_unicastFreqHoppingInd_r13_e v;
};

struct MBSFN_SubframeConfigList_v1430_SystemInformationBlockType2_mbsfn_SubframeConfigList_v1430_Optional {
	bool d;
	MBSFN_SubframeConfigList_v1430 v;
};

struct SystemInformationBlockType2_videoServiceCauseIndication_r14_e_videoServiceCauseIndication_r14_Optional {
	bool d;
	SystemInformationBlockType2_videoServiceCauseIndication_r14_e v;
};

struct PLMN_InfoList_r15_SystemInformationBlockType2_plmn_InfoList_r15_Optional {
	bool d;
	PLMN_InfoList_r15 v;
};

struct SystemInformationBlockType2_cp_EDT_r15_e_cp_EDT_r15_Optional {
	bool d;
	SystemInformationBlockType2_cp_EDT_r15_e v;
};

struct SystemInformationBlockType2_up_EDT_r15_e_up_EDT_r15_Optional {
	bool d;
	SystemInformationBlockType2_up_EDT_r15_e v;
};

struct SystemInformationBlockType2_idleModeMeasurements_r15_e_idleModeMeasurements_r15_Optional {
	bool d;
	SystemInformationBlockType2_idleModeMeasurements_r15_e v;
};

struct SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e_reducedCP_LatencyEnabled_r15_Optional {
	bool d;
	SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e v;
};

struct SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e_mbms_ROM_ServiceIndication_r15_Optional {
	bool d;
	SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e v;
};

struct SystemInformationBlockType2_rlos_Enabled_r16_e_rlos_Enabled_r16_Optional {
	bool d;
	SystemInformationBlockType2_rlos_Enabled_r16_e v;
};

struct SystemInformationBlockType2_earlySecurityReactivation_r16_e_earlySecurityReactivation_r16_Optional {
	bool d;
	SystemInformationBlockType2_earlySecurityReactivation_r16_e v;
};

struct SystemInformationBlockType2_cp_EDT_5GC_r16_e_cp_EDT_5GC_r16_Optional {
	bool d;
	SystemInformationBlockType2_cp_EDT_5GC_r16_e v;
};

struct SystemInformationBlockType2_up_EDT_5GC_r16_e_up_EDT_5GC_r16_Optional {
	bool d;
	SystemInformationBlockType2_up_EDT_5GC_r16_e v;
};

struct SystemInformationBlockType2_cp_PUR_EPC_r16_e_cp_PUR_EPC_r16_Optional {
	bool d;
	SystemInformationBlockType2_cp_PUR_EPC_r16_e v;
};

struct SystemInformationBlockType2_up_PUR_EPC_r16_e_up_PUR_EPC_r16_Optional {
	bool d;
	SystemInformationBlockType2_up_PUR_EPC_r16_e v;
};

struct SystemInformationBlockType2_cp_PUR_5GC_r16_e_cp_PUR_5GC_r16_Optional {
	bool d;
	SystemInformationBlockType2_cp_PUR_5GC_r16_e v;
};

struct SystemInformationBlockType2_up_PUR_5GC_r16_e_up_PUR_5GC_r16_Optional {
	bool d;
	SystemInformationBlockType2_up_PUR_5GC_r16_e v;
};

struct SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e_mpdcch_CQI_Reporting_r16_Optional {
	bool d;
	SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e v;
};

struct SystemInformationBlockType2_rai_ActivationEnh_r16_e_rai_ActivationEnh_r16_Optional {
	bool d;
	SystemInformationBlockType2_rai_ActivationEnh_r16_e v;
};

struct SystemInformationBlockType2_idleModeMeasurementsNR_r16_e_idleModeMeasurementsNR_r16_Optional {
	bool d;
	SystemInformationBlockType2_idleModeMeasurementsNR_r16_e v;
};

struct SystemInformationBlockType2 {
	struct SystemInformationBlockType2_ac_BarringInfo_ac_BarringInfo_Optional ac_BarringInfo;
	struct RadioResourceConfigCommonSIB radioResourceConfigCommon;
	struct UE_TimersAndConstants ue_TimersAndConstants;
	struct SystemInformationBlockType2_freqInfo freqInfo;
	struct MBSFN_SubframeConfigList_SystemInformationBlockType2_mbsfn_SubframeConfigList_Optional mbsfn_SubframeConfigList;
	TimeAlignmentTimer_e timeAlignmentTimerCommon;
	struct OCTET_STRING_SystemInformationBlockType2_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct AC_BarringConfig_SystemInformationBlockType2_ssac_BarringForMMTEL_Voice_r9_Optional ssac_BarringForMMTEL_Voice_r9;
	struct AC_BarringConfig_SystemInformationBlockType2_ssac_BarringForMMTEL_Video_r9_Optional ssac_BarringForMMTEL_Video_r9;
	struct AC_BarringConfig_SystemInformationBlockType2_ac_BarringForCSFB_r10_Optional ac_BarringForCSFB_r10;
	struct SystemInformationBlockType2_ac_BarringSkipForMMTELVoice_r12_e_ac_BarringSkipForMMTELVoice_r12_Optional ac_BarringSkipForMMTELVoice_r12;
	struct SystemInformationBlockType2_ac_BarringSkipForMMTELVideo_r12_e_ac_BarringSkipForMMTELVideo_r12_Optional ac_BarringSkipForMMTELVideo_r12;
	struct SystemInformationBlockType2_ac_BarringSkipForSMS_r12_e_ac_BarringSkipForSMS_r12_Optional ac_BarringSkipForSMS_r12;
	struct AC_BarringPerPLMN_List_r12_SystemInformationBlockType2_ac_BarringPerPLMN_List_r12_Optional ac_BarringPerPLMN_List_r12;
	struct SystemInformationBlockType2_voiceServiceCauseIndication_r12_e_voiceServiceCauseIndication_r12_Optional voiceServiceCauseIndication_r12;
	struct ACDC_BarringForCommon_r13_SystemInformationBlockType2_acdc_BarringForCommon_r13_Optional acdc_BarringForCommon_r13;
	struct ACDC_BarringPerPLMN_List_r13_SystemInformationBlockType2_acdc_BarringPerPLMN_List_r13_Optional acdc_BarringPerPLMN_List_r13;
	struct UDT_Restricting_r13_SystemInformationBlockType2_udt_RestrictingForCommon_r13_Optional udt_RestrictingForCommon_r13;
	struct UDT_RestrictingPerPLMN_List_r13_SystemInformationBlockType2_udt_RestrictingPerPLMN_List_r13_Optional udt_RestrictingPerPLMN_List_r13;
	struct CIOT_EPS_OptimisationInfo_r13_SystemInformationBlockType2_cIoT_EPS_OptimisationInfo_r13_Optional cIoT_EPS_OptimisationInfo_r13;
	struct SystemInformationBlockType2_useFullResumeID_r13_e_useFullResumeID_r13_Optional useFullResumeID_r13;
	struct SystemInformationBlockType2_unicastFreqHoppingInd_r13_e_unicastFreqHoppingInd_r13_Optional unicastFreqHoppingInd_r13;
	struct MBSFN_SubframeConfigList_v1430_SystemInformationBlockType2_mbsfn_SubframeConfigList_v1430_Optional mbsfn_SubframeConfigList_v1430;
	struct SystemInformationBlockType2_videoServiceCauseIndication_r14_e_videoServiceCauseIndication_r14_Optional videoServiceCauseIndication_r14;
	struct PLMN_InfoList_r15_SystemInformationBlockType2_plmn_InfoList_r15_Optional plmn_InfoList_r15;
	struct SystemInformationBlockType2_cp_EDT_r15_e_cp_EDT_r15_Optional cp_EDT_r15;
	struct SystemInformationBlockType2_up_EDT_r15_e_up_EDT_r15_Optional up_EDT_r15;
	struct SystemInformationBlockType2_idleModeMeasurements_r15_e_idleModeMeasurements_r15_Optional idleModeMeasurements_r15;
	struct SystemInformationBlockType2_reducedCP_LatencyEnabled_r15_e_reducedCP_LatencyEnabled_r15_Optional reducedCP_LatencyEnabled_r15;
	struct SystemInformationBlockType2_mbms_ROM_ServiceIndication_r15_e_mbms_ROM_ServiceIndication_r15_Optional mbms_ROM_ServiceIndication_r15;
	struct SystemInformationBlockType2_rlos_Enabled_r16_e_rlos_Enabled_r16_Optional rlos_Enabled_r16;
	struct SystemInformationBlockType2_earlySecurityReactivation_r16_e_earlySecurityReactivation_r16_Optional earlySecurityReactivation_r16;
	struct SystemInformationBlockType2_cp_EDT_5GC_r16_e_cp_EDT_5GC_r16_Optional cp_EDT_5GC_r16;
	struct SystemInformationBlockType2_up_EDT_5GC_r16_e_up_EDT_5GC_r16_Optional up_EDT_5GC_r16;
	struct SystemInformationBlockType2_cp_PUR_EPC_r16_e_cp_PUR_EPC_r16_Optional cp_PUR_EPC_r16;
	struct SystemInformationBlockType2_up_PUR_EPC_r16_e_up_PUR_EPC_r16_Optional up_PUR_EPC_r16;
	struct SystemInformationBlockType2_cp_PUR_5GC_r16_e_cp_PUR_5GC_r16_Optional cp_PUR_5GC_r16;
	struct SystemInformationBlockType2_up_PUR_5GC_r16_e_up_PUR_5GC_r16_Optional up_PUR_5GC_r16;
	struct SystemInformationBlockType2_mpdcch_CQI_Reporting_r16_e_mpdcch_CQI_Reporting_r16_Optional mpdcch_CQI_Reporting_r16;
	struct SystemInformationBlockType2_rai_ActivationEnh_r16_e_rai_ActivationEnh_r16_Optional rai_ActivationEnh_r16;
	struct SystemInformationBlockType2_idleModeMeasurementsNR_r16_e_idleModeMeasurementsNR_r16_Optional idleModeMeasurementsNR_r16;
};

enum SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e {
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB0 = 0,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB1 = 1,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB2 = 2,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB3 = 3,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB4 = 4,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB5 = 5,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB6 = 6,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB8 = 7,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB10 = 8,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB12 = 9,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB14 = 10,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB16 = 11,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB18 = 12,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB20 = 13,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB22 = 14,
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e_dB24 = 15,
};

typedef enum SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e;

enum MobilityStateParameters_t_Evaluation_e {
	MobilityStateParameters_t_Evaluation_e_s30 = 0,
	MobilityStateParameters_t_Evaluation_e_s60 = 1,
	MobilityStateParameters_t_Evaluation_e_s120 = 2,
	MobilityStateParameters_t_Evaluation_e_s180 = 3,
	MobilityStateParameters_t_Evaluation_e_s240 = 4,
	MobilityStateParameters_t_Evaluation_e_spare3 = 5,
	MobilityStateParameters_t_Evaluation_e_spare2 = 6,
	MobilityStateParameters_t_Evaluation_e_spare1 = 7,
};

typedef enum MobilityStateParameters_t_Evaluation_e MobilityStateParameters_t_Evaluation_e;

enum MobilityStateParameters_t_HystNormal_e {
	MobilityStateParameters_t_HystNormal_e_s30 = 0,
	MobilityStateParameters_t_HystNormal_e_s60 = 1,
	MobilityStateParameters_t_HystNormal_e_s120 = 2,
	MobilityStateParameters_t_HystNormal_e_s180 = 3,
	MobilityStateParameters_t_HystNormal_e_s240 = 4,
	MobilityStateParameters_t_HystNormal_e_spare3 = 5,
	MobilityStateParameters_t_HystNormal_e_spare2 = 6,
	MobilityStateParameters_t_HystNormal_e_spare1 = 7,
};

typedef enum MobilityStateParameters_t_HystNormal_e MobilityStateParameters_t_HystNormal_e;

struct MobilityStateParameters {
	MobilityStateParameters_t_Evaluation_e t_Evaluation;
	MobilityStateParameters_t_HystNormal_e t_HystNormal;
	uint8_t n_CellChangeMedium;
	uint8_t n_CellChangeHigh;
};

enum SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e {
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e_dB_6 = 0,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e_dB_4 = 1,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e_dB_2 = 2,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e_dB0 = 3,
};

typedef enum SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e;

enum SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e {
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e_dB_6 = 0,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e_dB_4 = 1,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e_dB_2 = 2,
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e_dB0 = 3,
};

typedef enum SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e;

struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF {
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_Medium_e sf_Medium;
	SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF_sf_High_e sf_High;
};

struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars {
	struct MobilityStateParameters mobilityStateParameters;
	struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_q_HystSF q_HystSF;
};

struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_speedStateReselectionPars_Optional {
	bool d;
	struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars v;
};

struct SystemInformationBlockType3_cellReselectionInfoCommon {
	SystemInformationBlockType3_cellReselectionInfoCommon_q_Hyst_e q_Hyst;
	struct SystemInformationBlockType3_cellReselectionInfoCommon_speedStateReselectionPars_speedStateReselectionPars_Optional speedStateReselectionPars;
};

typedef uint8_t ReselectionThreshold;

typedef uint8_t CellReselectionPriority;

struct ReselectionThreshold_SystemInformationBlockType3_cellReselectionServingFreqInfo_s_NonIntraSearch_Optional {
	bool d;
	ReselectionThreshold v;
};

struct SystemInformationBlockType3_cellReselectionServingFreqInfo {
	struct ReselectionThreshold_SystemInformationBlockType3_cellReselectionServingFreqInfo_s_NonIntraSearch_Optional s_NonIntraSearch;
	ReselectionThreshold threshServingLow;
	CellReselectionPriority cellReselectionPriority;
};

typedef int8_t Q_RxLevMin;

typedef int8_t P_Max;

enum AllowedMeasBandwidth_e {
	AllowedMeasBandwidth_e_mbw6 = 0,
	AllowedMeasBandwidth_e_mbw15 = 1,
	AllowedMeasBandwidth_e_mbw25 = 2,
	AllowedMeasBandwidth_e_mbw50 = 3,
	AllowedMeasBandwidth_e_mbw75 = 4,
	AllowedMeasBandwidth_e_mbw100 = 5,
};

typedef enum AllowedMeasBandwidth_e AllowedMeasBandwidth_e;

typedef bool PresenceAntennaPort1;

typedef B2 NeighCellConfig;

typedef uint8_t T_Reselection;

enum SpeedStateScaleFactors_sf_Medium_e {
	SpeedStateScaleFactors_sf_Medium_e_oDot25 = 0,
	SpeedStateScaleFactors_sf_Medium_e_oDot5 = 1,
	SpeedStateScaleFactors_sf_Medium_e_oDot75 = 2,
	SpeedStateScaleFactors_sf_Medium_e_lDot0 = 3,
};

typedef enum SpeedStateScaleFactors_sf_Medium_e SpeedStateScaleFactors_sf_Medium_e;

enum SpeedStateScaleFactors_sf_High_e {
	SpeedStateScaleFactors_sf_High_e_oDot25 = 0,
	SpeedStateScaleFactors_sf_High_e_oDot5 = 1,
	SpeedStateScaleFactors_sf_High_e_oDot75 = 2,
	SpeedStateScaleFactors_sf_High_e_lDot0 = 3,
};

typedef enum SpeedStateScaleFactors_sf_High_e SpeedStateScaleFactors_sf_High_e;

struct SpeedStateScaleFactors {
	SpeedStateScaleFactors_sf_Medium_e sf_Medium;
	SpeedStateScaleFactors_sf_High_e sf_High;
};

struct P_Max_SystemInformationBlockType3_intraFreqCellReselectionInfo_p_Max_Optional {
	bool d;
	P_Max v;
};

struct ReselectionThreshold_SystemInformationBlockType3_intraFreqCellReselectionInfo_s_IntraSearch_Optional {
	bool d;
	ReselectionThreshold v;
};

struct AllowedMeasBandwidth_e_SystemInformationBlockType3_intraFreqCellReselectionInfo_allowedMeasBandwidth_Optional {
	bool d;
	AllowedMeasBandwidth_e v;
};

struct SpeedStateScaleFactors_SystemInformationBlockType3_intraFreqCellReselectionInfo_t_ReselectionEUTRA_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct SystemInformationBlockType3_intraFreqCellReselectionInfo {
	Q_RxLevMin q_RxLevMin;
	struct P_Max_SystemInformationBlockType3_intraFreqCellReselectionInfo_p_Max_Optional p_Max;
	struct ReselectionThreshold_SystemInformationBlockType3_intraFreqCellReselectionInfo_s_IntraSearch_Optional s_IntraSearch;
	struct AllowedMeasBandwidth_e_SystemInformationBlockType3_intraFreqCellReselectionInfo_allowedMeasBandwidth_Optional allowedMeasBandwidth;
	PresenceAntennaPort1 presenceAntennaPort1;
	NeighCellConfig neighCellConfig;
	T_Reselection t_ReselectionEUTRA;
	struct SpeedStateScaleFactors_SystemInformationBlockType3_intraFreqCellReselectionInfo_t_ReselectionEUTRA_SF_Optional t_ReselectionEUTRA_SF;
};

typedef uint8_t ReselectionThresholdQ_r9;

struct SystemInformationBlockType3_s_IntraSearch_v920 {
	ReselectionThreshold s_IntraSearchP_r9;
	ReselectionThresholdQ_r9 s_IntraSearchQ_r9;
};

struct SystemInformationBlockType3_s_NonIntraSearch_v920 {
	ReselectionThreshold s_NonIntraSearchP_r9;
	ReselectionThresholdQ_r9 s_NonIntraSearchQ_r9;
};

typedef int8_t Q_QualMin_r9;

enum CellReselectionSubPriority_r13_e {
	CellReselectionSubPriority_r13_e_oDot2 = 0,
	CellReselectionSubPriority_r13_e_oDot4 = 1,
	CellReselectionSubPriority_r13_e_oDot6 = 2,
	CellReselectionSubPriority_r13_e_oDot8 = 3,
};

typedef enum CellReselectionSubPriority_r13_e CellReselectionSubPriority_r13_e;

struct CellReselectionServingFreqInfo_v1310 {
	CellReselectionSubPriority_r13_e cellReselectionSubPriority_r13;
};

enum RedistributionServingInfo_r13_redistributionFactorCell_r13_e {
	RedistributionServingInfo_r13_redistributionFactorCell_r13_e_true = 0,
};

typedef enum RedistributionServingInfo_r13_redistributionFactorCell_r13_e RedistributionServingInfo_r13_redistributionFactorCell_r13_e;

enum RedistributionServingInfo_r13_t360_r13_e {
	RedistributionServingInfo_r13_t360_r13_e_min4 = 0,
	RedistributionServingInfo_r13_t360_r13_e_min8 = 1,
	RedistributionServingInfo_r13_t360_r13_e_min16 = 2,
	RedistributionServingInfo_r13_t360_r13_e_min32 = 3,
	RedistributionServingInfo_r13_t360_r13_e_infinity = 4,
	RedistributionServingInfo_r13_t360_r13_e_spare3 = 5,
	RedistributionServingInfo_r13_t360_r13_e_spare2 = 6,
	RedistributionServingInfo_r13_t360_r13_e_spare1 = 7,
};

typedef enum RedistributionServingInfo_r13_t360_r13_e RedistributionServingInfo_r13_t360_r13_e;

enum RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e {
	RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e_true = 0,
};

typedef enum RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e;

struct RedistributionServingInfo_r13_redistributionFactorCell_r13_e_redistributionFactorCell_r13_Optional {
	bool d;
	RedistributionServingInfo_r13_redistributionFactorCell_r13_e v;
};

struct RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e_redistrOnPagingOnly_r13_Optional {
	bool d;
	RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e v;
};

struct RedistributionServingInfo_r13 {
	uint8_t redistributionFactorServing_r13;
	struct RedistributionServingInfo_r13_redistributionFactorCell_r13_e_redistributionFactorCell_r13_Optional redistributionFactorCell_r13;
	RedistributionServingInfo_r13_t360_r13_e t360_r13;
	struct RedistributionServingInfo_r13_redistrOnPagingOnly_r13_e_redistrOnPagingOnly_r13_Optional redistrOnPagingOnly_r13;
};

struct Q_QualMin_r9_CellSelectionInfoCE_r13_q_QualMinRSRQ_CE_r13_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct CellSelectionInfoCE_r13 {
	Q_RxLevMin q_RxLevMinCE_r13;
	struct Q_QualMin_r9_CellSelectionInfoCE_r13_q_QualMinRSRQ_CE_r13_Optional q_QualMinRSRQ_CE_r13;
};

typedef uint8_t T_ReselectionEUTRA_CE_r13;

struct Q_QualMin_r9_CellSelectionInfoCE1_r13_q_QualMinRSRQ_CE1_r13_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct CellSelectionInfoCE1_r13 {
	Q_RxLevMin q_RxLevMinCE1_r13;
	struct Q_QualMin_r9_CellSelectionInfoCE1_r13_q_QualMinRSRQ_CE1_r13_Optional q_QualMinRSRQ_CE1_r13;
};

struct CellSelectionInfoCE1_v1360 {
	int8_t delta_RxLevMinCE1_v1360;
};

enum CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e {
	CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e_dB6 = 0,
	CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e_dB9 = 1,
	CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e_dB12 = 2,
	CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e_dB15 = 3,
};

typedef enum CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e;

struct CellReselectionInfoCommon_v1460 {
	CellReselectionInfoCommon_v1460_s_SearchDeltaP_r14_e s_SearchDeltaP_r14;
};

struct CellReselectionInfoHSDN_r15 {
	uint8_t cellEquivalentSize_r15;
};

enum CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e {
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB_6 = 0,
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB_3 = 1,
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB3 = 2,
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB6 = 3,
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB9 = 4,
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e_dB12 = 5,
};

typedef enum CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e;

struct CellSelectionInfoCE_v1530 {
	CellSelectionInfoCE_v1530_powerClass14dBm_Offset_r15_e powerClass14dBm_Offset_r15;
};

enum SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e {
	SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e_enabled = 0,
};

typedef enum SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e;

struct CellReselectionPriority_CellReselectionServingFreqInfo_v1610_altCellReselectionPriority_r16_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CellReselectionSubPriority_r13_e_CellReselectionServingFreqInfo_v1610_altCellReselectionSubPriority_r16_Optional {
	bool d;
	CellReselectionSubPriority_r13_e v;
};

struct CellReselectionServingFreqInfo_v1610 {
	struct CellReselectionPriority_CellReselectionServingFreqInfo_v1610_altCellReselectionPriority_r16_Optional altCellReselectionPriority_r16;
	struct CellReselectionSubPriority_r13_e_CellReselectionServingFreqInfo_v1610_altCellReselectionSubPriority_r16_Optional altCellReselectionSubPriority_r16;
};

struct OCTET_STRING_SystemInformationBlockType3_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType3_s_IntraSearch_v920_s_IntraSearch_v920_Optional {
	bool d;
	struct SystemInformationBlockType3_s_IntraSearch_v920 v;
};

struct SystemInformationBlockType3_s_NonIntraSearch_v920_s_NonIntraSearch_v920_Optional {
	bool d;
	struct SystemInformationBlockType3_s_NonIntraSearch_v920 v;
};

struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMin_r9_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct ReselectionThresholdQ_r9_SystemInformationBlockType3_threshServingLowQ_r9_Optional {
	bool d;
	ReselectionThresholdQ_r9 v;
};

struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMinWB_r11_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMinRSRQ_OnAllSymbols_r12_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct CellReselectionServingFreqInfo_v1310_SystemInformationBlockType3_cellReselectionServingFreqInfo_v1310_Optional {
	bool d;
	struct CellReselectionServingFreqInfo_v1310 v;
};

struct RedistributionServingInfo_r13_SystemInformationBlockType3_redistributionServingInfo_r13_Optional {
	bool d;
	struct RedistributionServingInfo_r13 v;
};

struct CellSelectionInfoCE_r13_SystemInformationBlockType3_cellSelectionInfoCE_r13_Optional {
	bool d;
	struct CellSelectionInfoCE_r13 v;
};

struct T_ReselectionEUTRA_CE_r13_SystemInformationBlockType3_t_ReselectionEUTRA_CE_r13_Optional {
	bool d;
	T_ReselectionEUTRA_CE_r13 v;
};

struct CellSelectionInfoCE1_r13_SystemInformationBlockType3_cellSelectionInfoCE1_r13_Optional {
	bool d;
	struct CellSelectionInfoCE1_r13 v;
};

struct CellSelectionInfoCE1_v1360_SystemInformationBlockType3_cellSelectionInfoCE1_v1360_Optional {
	bool d;
	struct CellSelectionInfoCE1_v1360 v;
};

struct CellReselectionInfoCommon_v1460_SystemInformationBlockType3_cellReselectionInfoCommon_v1460_Optional {
	bool d;
	struct CellReselectionInfoCommon_v1460 v;
};

struct CellReselectionInfoHSDN_r15_SystemInformationBlockType3_cellReselectionInfoHSDN_r15_Optional {
	bool d;
	struct CellReselectionInfoHSDN_r15 v;
};

struct CellSelectionInfoCE_v1530_SystemInformationBlockType3_cellSelectionInfoCE_v1530_Optional {
	bool d;
	struct CellSelectionInfoCE_v1530 v;
};

struct SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e_crs_IntfMitigNeighCellsCE_r15_Optional {
	bool d;
	SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e v;
};

struct CellReselectionServingFreqInfo_v1610_SystemInformationBlockType3_cellReselectionServingFreqInfo_v1610_Optional {
	bool d;
	struct CellReselectionServingFreqInfo_v1610 v;
};

struct SystemInformationBlockType3 {
	struct SystemInformationBlockType3_cellReselectionInfoCommon cellReselectionInfoCommon;
	struct SystemInformationBlockType3_cellReselectionServingFreqInfo cellReselectionServingFreqInfo;
	struct SystemInformationBlockType3_intraFreqCellReselectionInfo intraFreqCellReselectionInfo;
	struct OCTET_STRING_SystemInformationBlockType3_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType3_s_IntraSearch_v920_s_IntraSearch_v920_Optional s_IntraSearch_v920;
	struct SystemInformationBlockType3_s_NonIntraSearch_v920_s_NonIntraSearch_v920_Optional s_NonIntraSearch_v920;
	struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMin_r9_Optional q_QualMin_r9;
	struct ReselectionThresholdQ_r9_SystemInformationBlockType3_threshServingLowQ_r9_Optional threshServingLowQ_r9;
	struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMinWB_r11_Optional q_QualMinWB_r11;
	struct Q_QualMin_r9_SystemInformationBlockType3_q_QualMinRSRQ_OnAllSymbols_r12_Optional q_QualMinRSRQ_OnAllSymbols_r12;
	struct CellReselectionServingFreqInfo_v1310_SystemInformationBlockType3_cellReselectionServingFreqInfo_v1310_Optional cellReselectionServingFreqInfo_v1310;
	struct RedistributionServingInfo_r13_SystemInformationBlockType3_redistributionServingInfo_r13_Optional redistributionServingInfo_r13;
	struct CellSelectionInfoCE_r13_SystemInformationBlockType3_cellSelectionInfoCE_r13_Optional cellSelectionInfoCE_r13;
	struct T_ReselectionEUTRA_CE_r13_SystemInformationBlockType3_t_ReselectionEUTRA_CE_r13_Optional t_ReselectionEUTRA_CE_r13;
	struct CellSelectionInfoCE1_r13_SystemInformationBlockType3_cellSelectionInfoCE1_r13_Optional cellSelectionInfoCE1_r13;
	struct CellSelectionInfoCE1_v1360_SystemInformationBlockType3_cellSelectionInfoCE1_v1360_Optional cellSelectionInfoCE1_v1360;
	struct CellReselectionInfoCommon_v1460_SystemInformationBlockType3_cellReselectionInfoCommon_v1460_Optional cellReselectionInfoCommon_v1460;
	struct CellReselectionInfoHSDN_r15_SystemInformationBlockType3_cellReselectionInfoHSDN_r15_Optional cellReselectionInfoHSDN_r15;
	struct CellSelectionInfoCE_v1530_SystemInformationBlockType3_cellSelectionInfoCE_v1530_Optional cellSelectionInfoCE_v1530;
	struct SystemInformationBlockType3_crs_IntfMitigNeighCellsCE_r15_e_crs_IntfMitigNeighCellsCE_r15_Optional crs_IntfMitigNeighCellsCE_r15;
	struct CellReselectionServingFreqInfo_v1610_SystemInformationBlockType3_cellReselectionServingFreqInfo_v1610_Optional cellReselectionServingFreqInfo_v1610;
};

typedef uint16_t PhysCellId;

enum Q_OffsetRange_e {
	Q_OffsetRange_e_dB_24 = 0,
	Q_OffsetRange_e_dB_22 = 1,
	Q_OffsetRange_e_dB_20 = 2,
	Q_OffsetRange_e_dB_18 = 3,
	Q_OffsetRange_e_dB_16 = 4,
	Q_OffsetRange_e_dB_14 = 5,
	Q_OffsetRange_e_dB_12 = 6,
	Q_OffsetRange_e_dB_10 = 7,
	Q_OffsetRange_e_dB_8 = 8,
	Q_OffsetRange_e_dB_6 = 9,
	Q_OffsetRange_e_dB_5 = 10,
	Q_OffsetRange_e_dB_4 = 11,
	Q_OffsetRange_e_dB_3 = 12,
	Q_OffsetRange_e_dB_2 = 13,
	Q_OffsetRange_e_dB_1 = 14,
	Q_OffsetRange_e_dB0 = 15,
	Q_OffsetRange_e_dB1 = 16,
	Q_OffsetRange_e_dB2 = 17,
	Q_OffsetRange_e_dB3 = 18,
	Q_OffsetRange_e_dB4 = 19,
	Q_OffsetRange_e_dB5 = 20,
	Q_OffsetRange_e_dB6 = 21,
	Q_OffsetRange_e_dB8 = 22,
	Q_OffsetRange_e_dB10 = 23,
	Q_OffsetRange_e_dB12 = 24,
	Q_OffsetRange_e_dB14 = 25,
	Q_OffsetRange_e_dB16 = 26,
	Q_OffsetRange_e_dB18 = 27,
	Q_OffsetRange_e_dB20 = 28,
	Q_OffsetRange_e_dB22 = 29,
	Q_OffsetRange_e_dB24 = 30,
};

typedef enum Q_OffsetRange_e Q_OffsetRange_e;

struct IntraFreqNeighCellInfo {
	PhysCellId physCellId;
	Q_OffsetRange_e q_OffsetCell;
};

struct IntraFreqNeighCellInfo_IntraFreqNeighCellList_Dynamic {
	size_t d;
	struct IntraFreqNeighCellInfo* v;
};

typedef struct IntraFreqNeighCellInfo_IntraFreqNeighCellList_Dynamic IntraFreqNeighCellList;

enum PhysCellIdRange_range_e {
	PhysCellIdRange_range_e_n4 = 0,
	PhysCellIdRange_range_e_n8 = 1,
	PhysCellIdRange_range_e_n12 = 2,
	PhysCellIdRange_range_e_n16 = 3,
	PhysCellIdRange_range_e_n24 = 4,
	PhysCellIdRange_range_e_n32 = 5,
	PhysCellIdRange_range_e_n48 = 6,
	PhysCellIdRange_range_e_n64 = 7,
	PhysCellIdRange_range_e_n84 = 8,
	PhysCellIdRange_range_e_n96 = 9,
	PhysCellIdRange_range_e_n128 = 10,
	PhysCellIdRange_range_e_n168 = 11,
	PhysCellIdRange_range_e_n252 = 12,
	PhysCellIdRange_range_e_n504 = 13,
	PhysCellIdRange_range_e_spare2 = 14,
	PhysCellIdRange_range_e_spare1 = 15,
};

typedef enum PhysCellIdRange_range_e PhysCellIdRange_range_e;

struct PhysCellIdRange_range_e_range_Optional {
	bool d;
	PhysCellIdRange_range_e v;
};

struct PhysCellIdRange {
	PhysCellId start;
	struct PhysCellIdRange_range_e_range_Optional range;
};

struct PhysCellIdRange_IntraFreqBlackCellList_Dynamic {
	size_t d;
	struct PhysCellIdRange* v;
};

typedef struct PhysCellIdRange_IntraFreqBlackCellList_Dynamic IntraFreqBlackCellList;

struct PhysCellIdRange_IntraFreqNeighHSDN_CellList_r15_Dynamic {
	size_t d;
	struct PhysCellIdRange* v;
};

typedef struct PhysCellIdRange_IntraFreqNeighHSDN_CellList_r15_Dynamic IntraFreqNeighHSDN_CellList_r15;

enum RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e {
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g1 = 0,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g2 = 1,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g4 = 2,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g8 = 3,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g16 = 4,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g32 = 5,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g64 = 6,
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e_g128 = 7,
};

typedef enum RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e;

struct RSS_ConfigCarrierInfo_r16 {
	BIT_STRING narrowbandIndex_r16;
	RSS_ConfigCarrierInfo_r16_timeOffsetGranularity_r16_e timeOffsetGranularity_r16;
};

enum RSS_MeasPowerBias_r16_e {
	RSS_MeasPowerBias_r16_e_dB_6 = 0,
	RSS_MeasPowerBias_r16_e_dB_3 = 1,
	RSS_MeasPowerBias_r16_e_dB0 = 2,
	RSS_MeasPowerBias_r16_e_dB3 = 3,
	RSS_MeasPowerBias_r16_e_dB6 = 4,
	RSS_MeasPowerBias_r16_e_dB9 = 5,
	RSS_MeasPowerBias_r16_e_dB12 = 6,
	RSS_MeasPowerBias_r16_e_rssNotUsed = 7,
};

typedef enum RSS_MeasPowerBias_r16_e RSS_MeasPowerBias_r16_e;

struct IntraFreqNeighCellInfo_v1610 {
	RSS_MeasPowerBias_r16_e rss_MeasPowerBias_r16;
};

struct IntraFreqNeighCellInfo_v1610_IntraFreqNeighCellList_v1610_Dynamic {
	size_t d;
	struct IntraFreqNeighCellInfo_v1610* v;
};

typedef struct IntraFreqNeighCellInfo_v1610_IntraFreqNeighCellList_v1610_Dynamic IntraFreqNeighCellList_v1610;

struct IntraFreqNeighCellList_SystemInformationBlockType4_intraFreqNeighCellList_Optional {
	bool d;
	IntraFreqNeighCellList v;
};

struct IntraFreqBlackCellList_SystemInformationBlockType4_intraFreqBlackCellList_Optional {
	bool d;
	IntraFreqBlackCellList v;
};

struct PhysCellIdRange_SystemInformationBlockType4_csg_PhysCellIdRange_Optional {
	bool d;
	struct PhysCellIdRange v;
};

struct OCTET_STRING_SystemInformationBlockType4_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct IntraFreqNeighHSDN_CellList_r15_SystemInformationBlockType4_intraFreqNeighHSDN_CellList_r15_Optional {
	bool d;
	IntraFreqNeighHSDN_CellList_r15 v;
};

struct RSS_ConfigCarrierInfo_r16_SystemInformationBlockType4_rss_ConfigCarrierInfo_r16_Optional {
	bool d;
	struct RSS_ConfigCarrierInfo_r16 v;
};

struct IntraFreqNeighCellList_v1610_SystemInformationBlockType4_intraFreqNeighCellList_v1610_Optional {
	bool d;
	IntraFreqNeighCellList_v1610 v;
};

struct SystemInformationBlockType4 {
	struct IntraFreqNeighCellList_SystemInformationBlockType4_intraFreqNeighCellList_Optional intraFreqNeighCellList;
	struct IntraFreqBlackCellList_SystemInformationBlockType4_intraFreqBlackCellList_Optional intraFreqBlackCellList;
	struct PhysCellIdRange_SystemInformationBlockType4_csg_PhysCellIdRange_Optional csg_PhysCellIdRange;
	struct OCTET_STRING_SystemInformationBlockType4_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct IntraFreqNeighHSDN_CellList_r15_SystemInformationBlockType4_intraFreqNeighHSDN_CellList_r15_Optional intraFreqNeighHSDN_CellList_r15;
	struct RSS_ConfigCarrierInfo_r16_SystemInformationBlockType4_rss_ConfigCarrierInfo_r16_Optional rss_ConfigCarrierInfo_r16;
	struct IntraFreqNeighCellList_v1610_SystemInformationBlockType4_intraFreqNeighCellList_v1610_Optional intraFreqNeighCellList_v1610;
};

struct InterFreqNeighCellInfo {
	PhysCellId physCellId;
	Q_OffsetRange_e q_OffsetCell;
};

struct InterFreqNeighCellInfo_InterFreqNeighCellList_Dynamic {
	size_t d;
	struct InterFreqNeighCellInfo* v;
};

typedef struct InterFreqNeighCellInfo_InterFreqNeighCellList_Dynamic InterFreqNeighCellList;

struct PhysCellIdRange_InterFreqBlackCellList_Dynamic {
	size_t d;
	struct PhysCellIdRange* v;
};

typedef struct PhysCellIdRange_InterFreqBlackCellList_Dynamic InterFreqBlackCellList;

struct InterFreqCarrierFreqInfo_threshX_Q_r9 {
	ReselectionThresholdQ_r9 threshX_HighQ_r9;
	ReselectionThresholdQ_r9 threshX_LowQ_r9;
};

struct P_Max_InterFreqCarrierFreqInfo_p_Max_Optional {
	bool d;
	P_Max v;
};

struct SpeedStateScaleFactors_InterFreqCarrierFreqInfo_t_ReselectionEUTRA_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct CellReselectionPriority_InterFreqCarrierFreqInfo_cellReselectionPriority_Optional {
	bool d;
	CellReselectionPriority v;
};

struct Q_OffsetRange_e_InterFreqCarrierFreqInfo_q_OffsetFreq_Optional {
	bool d;
	Q_OffsetRange_e v;
};

struct InterFreqNeighCellList_InterFreqCarrierFreqInfo_interFreqNeighCellList_Optional {
	bool d;
	InterFreqNeighCellList v;
};

struct InterFreqBlackCellList_InterFreqCarrierFreqInfo_interFreqBlackCellList_Optional {
	bool d;
	InterFreqBlackCellList v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_q_QualMin_r9_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct InterFreqCarrierFreqInfo_threshX_Q_r9_threshX_Q_r9_Optional {
	bool d;
	struct InterFreqCarrierFreqInfo_threshX_Q_r9 v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_q_QualMinWB_r11_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct InterFreqCarrierFreqInfo {
	ARFCN_ValueEUTRA dl_CarrierFreq;
	Q_RxLevMin q_RxLevMin;
	struct P_Max_InterFreqCarrierFreqInfo_p_Max_Optional p_Max;
	T_Reselection t_ReselectionEUTRA;
	struct SpeedStateScaleFactors_InterFreqCarrierFreqInfo_t_ReselectionEUTRA_SF_Optional t_ReselectionEUTRA_SF;
	ReselectionThreshold threshX_High;
	ReselectionThreshold threshX_Low;
	AllowedMeasBandwidth_e allowedMeasBandwidth;
	PresenceAntennaPort1 presenceAntennaPort1;
	struct CellReselectionPriority_InterFreqCarrierFreqInfo_cellReselectionPriority_Optional cellReselectionPriority;
	NeighCellConfig neighCellConfig;
	struct Q_OffsetRange_e_InterFreqCarrierFreqInfo_q_OffsetFreq_Optional q_OffsetFreq;
	struct InterFreqNeighCellList_InterFreqCarrierFreqInfo_interFreqNeighCellList_Optional interFreqNeighCellList;
	struct InterFreqBlackCellList_InterFreqCarrierFreqInfo_interFreqBlackCellList_Optional interFreqBlackCellList;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_q_QualMin_r9_Optional q_QualMin_r9;
	struct InterFreqCarrierFreqInfo_threshX_Q_r9_threshX_Q_r9_Optional threshX_Q_r9;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_q_QualMinWB_r11_Optional q_QualMinWB_r11;
};

struct InterFreqCarrierFreqInfo_InterFreqCarrierFreqList_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo* v;
};

typedef struct InterFreqCarrierFreqInfo_InterFreqCarrierFreqList_Dynamic InterFreqCarrierFreqList;

enum InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e {
	InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e_true = 0,
};

typedef enum InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e;

struct InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional {
	bool d;
	InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_v1250_q_QualMinRSRQ_OnAllSymbols_r12_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct InterFreqCarrierFreqInfo_v1250 {
	struct InterFreqCarrierFreqInfo_v1250_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional reducedMeasPerformance_r12;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_v1250_q_QualMinRSRQ_OnAllSymbols_r12_Optional q_QualMinRSRQ_OnAllSymbols_r12;
};

struct InterFreqCarrierFreqInfo_v1250_InterFreqCarrierFreqList_v1250_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1250* v;
};

typedef struct InterFreqCarrierFreqInfo_v1250_InterFreqCarrierFreqList_v1250_Dynamic InterFreqCarrierFreqList_v1250;

typedef uint32_t ARFCN_ValueEUTRA_r9;

struct InterFreqCarrierFreqInfo_r12_threshX_Q_r12 {
	ReselectionThresholdQ_r9 threshX_HighQ_r12;
	ReselectionThresholdQ_r9 threshX_LowQ_r12;
};

typedef uint16_t FreqBandIndicator_r11;

struct FreqBandIndicator_r11_MultiBandInfoList_r11_Dynamic {
	size_t d;
	FreqBandIndicator_r11* v;
};

typedef struct FreqBandIndicator_r11_MultiBandInfoList_r11_Dynamic MultiBandInfoList_r11;

enum InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e {
	InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e_true = 0,
};

typedef enum InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e;

struct P_Max_InterFreqCarrierFreqInfo_r12_p_Max_r12_Optional {
	bool d;
	P_Max v;
};

struct SpeedStateScaleFactors_InterFreqCarrierFreqInfo_r12_t_ReselectionEUTRA_SF_r12_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct CellReselectionPriority_InterFreqCarrierFreqInfo_r12_cellReselectionPriority_r12_Optional {
	bool d;
	CellReselectionPriority v;
};

struct Q_OffsetRange_e_InterFreqCarrierFreqInfo_r12_q_OffsetFreq_r12_Optional {
	bool d;
	Q_OffsetRange_e v;
};

struct InterFreqNeighCellList_InterFreqCarrierFreqInfo_r12_interFreqNeighCellList_r12_Optional {
	bool d;
	InterFreqNeighCellList v;
};

struct InterFreqBlackCellList_InterFreqCarrierFreqInfo_r12_interFreqBlackCellList_r12_Optional {
	bool d;
	InterFreqBlackCellList v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMin_r12_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct InterFreqCarrierFreqInfo_r12_threshX_Q_r12_threshX_Q_r12_Optional {
	bool d;
	struct InterFreqCarrierFreqInfo_r12_threshX_Q_r12 v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMinWB_r12_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct MultiBandInfoList_r11_InterFreqCarrierFreqInfo_r12_multiBandInfoList_r12_Optional {
	bool d;
	MultiBandInfoList_r11 v;
};

struct InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional {
	bool d;
	InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e v;
};

struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMinRSRQ_OnAllSymbols_r12_Optional {
	bool d;
	Q_QualMin_r9 v;
};

struct InterFreqCarrierFreqInfo_r12 {
	ARFCN_ValueEUTRA_r9 dl_CarrierFreq_r12;
	Q_RxLevMin q_RxLevMin_r12;
	struct P_Max_InterFreqCarrierFreqInfo_r12_p_Max_r12_Optional p_Max_r12;
	T_Reselection t_ReselectionEUTRA_r12;
	struct SpeedStateScaleFactors_InterFreqCarrierFreqInfo_r12_t_ReselectionEUTRA_SF_r12_Optional t_ReselectionEUTRA_SF_r12;
	ReselectionThreshold threshX_High_r12;
	ReselectionThreshold threshX_Low_r12;
	AllowedMeasBandwidth_e allowedMeasBandwidth_r12;
	PresenceAntennaPort1 presenceAntennaPort1_r12;
	struct CellReselectionPriority_InterFreqCarrierFreqInfo_r12_cellReselectionPriority_r12_Optional cellReselectionPriority_r12;
	NeighCellConfig neighCellConfig_r12;
	struct Q_OffsetRange_e_InterFreqCarrierFreqInfo_r12_q_OffsetFreq_r12_Optional q_OffsetFreq_r12;
	struct InterFreqNeighCellList_InterFreqCarrierFreqInfo_r12_interFreqNeighCellList_r12_Optional interFreqNeighCellList_r12;
	struct InterFreqBlackCellList_InterFreqCarrierFreqInfo_r12_interFreqBlackCellList_r12_Optional interFreqBlackCellList_r12;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMin_r12_Optional q_QualMin_r12;
	struct InterFreqCarrierFreqInfo_r12_threshX_Q_r12_threshX_Q_r12_Optional threshX_Q_r12;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMinWB_r12_Optional q_QualMinWB_r12;
	struct MultiBandInfoList_r11_InterFreqCarrierFreqInfo_r12_multiBandInfoList_r12_Optional multiBandInfoList_r12;
	struct InterFreqCarrierFreqInfo_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional reducedMeasPerformance_r12;
	struct Q_QualMin_r9_InterFreqCarrierFreqInfo_r12_q_QualMinRSRQ_OnAllSymbols_r12_Optional q_QualMinRSRQ_OnAllSymbols_r12;
};

struct InterFreqCarrierFreqInfo_r12_InterFreqCarrierFreqListExt_r12_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_r12* v;
};

typedef struct InterFreqCarrierFreqInfo_r12_InterFreqCarrierFreqListExt_r12_Dynamic InterFreqCarrierFreqListExt_r12;

struct P_Max_NS_PmaxValue_r10_additionalPmax_r10_Optional {
	bool d;
	P_Max v;
};

struct NS_PmaxValue_r10 {
	struct P_Max_NS_PmaxValue_r10_additionalPmax_r10_Optional additionalPmax_r10;
	AdditionalSpectrumEmission additionalSpectrumEmission;
};

struct NS_PmaxValue_r10_NS_PmaxList_r10_Dynamic {
	size_t d;
	struct NS_PmaxValue_r10* v;
};

typedef struct NS_PmaxValue_r10_NS_PmaxList_r10_Dynamic NS_PmaxList_r10;

struct NS_PmaxList_r10_MultiBandInfoList_v10j0_Dynamic {
	size_t d;
	NS_PmaxList_r10* v;
};

typedef struct NS_PmaxList_r10_MultiBandInfoList_v10j0_Dynamic MultiBandInfoList_v10j0;

struct NS_PmaxList_r10_InterFreqCarrierFreqInfo_v10j0_freqBandInfo_r10_Optional {
	bool d;
	NS_PmaxList_r10 v;
};

struct MultiBandInfoList_v10j0_InterFreqCarrierFreqInfo_v10j0_multiBandInfoList_v10j0_Optional {
	bool d;
	MultiBandInfoList_v10j0 v;
};

struct InterFreqCarrierFreqInfo_v10j0 {
	struct NS_PmaxList_r10_InterFreqCarrierFreqInfo_v10j0_freqBandInfo_r10_Optional freqBandInfo_r10;
	struct MultiBandInfoList_v10j0_InterFreqCarrierFreqInfo_v10j0_multiBandInfoList_v10j0_Optional multiBandInfoList_v10j0;
};

struct InterFreqCarrierFreqInfo_v10j0_InterFreqCarrierFreqListExt_v1280_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v10j0* v;
};

typedef struct InterFreqCarrierFreqInfo_v10j0_InterFreqCarrierFreqListExt_v1280_Dynamic InterFreqCarrierFreqListExt_v1280;

typedef uint8_t RedistributionFactor_r13;

struct RedistributionNeighCell_r13 {
	PhysCellId physCellId_r13;
	RedistributionFactor_r13 redistributionFactorCell_r13;
};

struct RedistributionNeighCell_r13_RedistributionNeighCellList_r13_Dynamic {
	size_t d;
	struct RedistributionNeighCell_r13* v;
};

typedef struct RedistributionNeighCell_r13_RedistributionNeighCellList_r13_Dynamic RedistributionNeighCellList_r13;

struct RedistributionFactor_r13_RedistributionInterFreqInfo_r13_redistributionFactorFreq_r13_Optional {
	bool d;
	RedistributionFactor_r13 v;
};

struct RedistributionNeighCellList_r13_RedistributionInterFreqInfo_r13_redistributionNeighCellList_r13_Optional {
	bool d;
	RedistributionNeighCellList_r13 v;
};

struct RedistributionInterFreqInfo_r13 {
	struct RedistributionFactor_r13_RedistributionInterFreqInfo_r13_redistributionFactorFreq_r13_Optional redistributionFactorFreq_r13;
	struct RedistributionNeighCellList_r13_RedistributionInterFreqInfo_r13_redistributionNeighCellList_r13_Optional redistributionNeighCellList_r13;
};

struct CellReselectionSubPriority_r13_e_InterFreqCarrierFreqInfo_v1310_cellReselectionSubPriority_r13_Optional {
	bool d;
	CellReselectionSubPriority_r13_e v;
};

struct RedistributionInterFreqInfo_r13_InterFreqCarrierFreqInfo_v1310_redistributionInterFreqInfo_r13_Optional {
	bool d;
	struct RedistributionInterFreqInfo_r13 v;
};

struct CellSelectionInfoCE_r13_InterFreqCarrierFreqInfo_v1310_cellSelectionInfoCE_r13_Optional {
	bool d;
	struct CellSelectionInfoCE_r13 v;
};

struct T_ReselectionEUTRA_CE_r13_InterFreqCarrierFreqInfo_v1310_t_ReselectionEUTRA_CE_r13_Optional {
	bool d;
	T_ReselectionEUTRA_CE_r13 v;
};

struct InterFreqCarrierFreqInfo_v1310 {
	struct CellReselectionSubPriority_r13_e_InterFreqCarrierFreqInfo_v1310_cellReselectionSubPriority_r13_Optional cellReselectionSubPriority_r13;
	struct RedistributionInterFreqInfo_r13_InterFreqCarrierFreqInfo_v1310_redistributionInterFreqInfo_r13_Optional redistributionInterFreqInfo_r13;
	struct CellSelectionInfoCE_r13_InterFreqCarrierFreqInfo_v1310_cellSelectionInfoCE_r13_Optional cellSelectionInfoCE_r13;
	struct T_ReselectionEUTRA_CE_r13_InterFreqCarrierFreqInfo_v1310_t_ReselectionEUTRA_CE_r13_Optional t_ReselectionEUTRA_CE_r13;
};

struct InterFreqCarrierFreqInfo_v1310_InterFreqCarrierFreqList_v1310_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1310* v;
};

typedef struct InterFreqCarrierFreqInfo_v1310_InterFreqCarrierFreqList_v1310_Dynamic InterFreqCarrierFreqList_v1310;

struct InterFreqCarrierFreqInfo_v1310_InterFreqCarrierFreqListExt_v1310_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1310* v;
};

typedef struct InterFreqCarrierFreqInfo_v1310_InterFreqCarrierFreqListExt_v1310_Dynamic InterFreqCarrierFreqListExt_v1310;

struct CellSelectionInfoCE1_r13_InterFreqCarrierFreqInfo_v1350_cellSelectionInfoCE1_r13_Optional {
	bool d;
	struct CellSelectionInfoCE1_r13 v;
};

struct InterFreqCarrierFreqInfo_v1350 {
	struct CellSelectionInfoCE1_r13_InterFreqCarrierFreqInfo_v1350_cellSelectionInfoCE1_r13_Optional cellSelectionInfoCE1_r13;
};

struct InterFreqCarrierFreqInfo_v1350_InterFreqCarrierFreqList_v1350_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1350* v;
};

typedef struct InterFreqCarrierFreqInfo_v1350_InterFreqCarrierFreqList_v1350_Dynamic InterFreqCarrierFreqList_v1350;

struct InterFreqCarrierFreqInfo_v1350_InterFreqCarrierFreqListExt_v1350_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1350* v;
};

typedef struct InterFreqCarrierFreqInfo_v1350_InterFreqCarrierFreqListExt_v1350_Dynamic InterFreqCarrierFreqListExt_v1350;

struct CellSelectionInfoCE1_v1360_InterFreqCarrierFreqInfo_v1360_cellSelectionInfoCE1_v1360_Optional {
	bool d;
	struct CellSelectionInfoCE1_v1360 v;
};

struct InterFreqCarrierFreqInfo_v1360 {
	struct CellSelectionInfoCE1_v1360_InterFreqCarrierFreqInfo_v1360_cellSelectionInfoCE1_v1360_Optional cellSelectionInfoCE1_v1360;
};

struct InterFreqCarrierFreqInfo_v1360_InterFreqCarrierFreqListExt_v1360_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1360* v;
};

typedef struct InterFreqCarrierFreqInfo_v1360_InterFreqCarrierFreqListExt_v1360_Dynamic InterFreqCarrierFreqListExt_v1360;

struct PhysCellIdRange_InterFreqNeighHSDN_CellList_r15_Dynamic {
	size_t d;
	struct PhysCellIdRange* v;
};

typedef struct PhysCellIdRange_InterFreqNeighHSDN_CellList_r15_Dynamic InterFreqNeighHSDN_CellList_r15;

struct InterFreqNeighHSDN_CellList_r15_InterFreqCarrierFreqInfo_v1530_interFreqNeighHSDN_CellList_r15_Optional {
	bool d;
	InterFreqNeighHSDN_CellList_r15 v;
};

struct CellSelectionInfoCE_v1530_InterFreqCarrierFreqInfo_v1530_cellSelectionInfoCE_v1530_Optional {
	bool d;
	struct CellSelectionInfoCE_v1530 v;
};

struct InterFreqCarrierFreqInfo_v1530 {
	bool hsdn_Indication_r15;
	struct InterFreqNeighHSDN_CellList_r15_InterFreqCarrierFreqInfo_v1530_interFreqNeighHSDN_CellList_r15_Optional interFreqNeighHSDN_CellList_r15;
	struct CellSelectionInfoCE_v1530_InterFreqCarrierFreqInfo_v1530_cellSelectionInfoCE_v1530_Optional cellSelectionInfoCE_v1530;
};

struct InterFreqCarrierFreqInfo_v1530_InterFreqCarrierFreqList_v1530_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1530* v;
};

typedef struct InterFreqCarrierFreqInfo_v1530_InterFreqCarrierFreqList_v1530_Dynamic InterFreqCarrierFreqList_v1530;

struct InterFreqCarrierFreqInfo_v1530_InterFreqCarrierFreqListExt_v1530_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1530* v;
};

typedef struct InterFreqCarrierFreqInfo_v1530_InterFreqCarrierFreqListExt_v1530_Dynamic InterFreqCarrierFreqListExt_v1530;

struct PhysCellIdRange_CellList_r15_Dynamic {
	size_t d;
	struct PhysCellIdRange* v;
};

typedef struct PhysCellIdRange_CellList_r15_Dynamic CellList_r15;

enum MeasIdleCarrierEUTRA_r15_reportQuantities_e {
	MeasIdleCarrierEUTRA_r15_reportQuantities_e_rsrp = 0,
	MeasIdleCarrierEUTRA_r15_reportQuantities_e_rsrq = 1,
	MeasIdleCarrierEUTRA_r15_reportQuantities_e_both = 2,
};

typedef enum MeasIdleCarrierEUTRA_r15_reportQuantities_e MeasIdleCarrierEUTRA_r15_reportQuantities_e;

typedef int8_t RSRQ_Range_r13;

struct RSRP_Range_MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_idleRSRP_Threshold_r15_Optional {
	bool d;
	RSRP_Range v;
};

struct RSRQ_Range_r13_MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_idleRSRQ_Threshold_r15_Optional {
	bool d;
	RSRQ_Range_r13 v;
};

struct MeasIdleCarrierEUTRA_r15_qualityThreshold_r15 {
	struct RSRP_Range_MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_idleRSRP_Threshold_r15_Optional idleRSRP_Threshold_r15;
	struct RSRQ_Range_r13_MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_idleRSRQ_Threshold_r15_Optional idleRSRQ_Threshold_r15;
};

struct CellList_r15_MeasIdleCarrierEUTRA_r15_validityArea_r15_Optional {
	bool d;
	CellList_r15 v;
};

struct CellList_r15_MeasIdleCarrierEUTRA_r15_measCellList_r15_Optional {
	bool d;
	CellList_r15 v;
};

struct MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_qualityThreshold_r15_Optional {
	bool d;
	struct MeasIdleCarrierEUTRA_r15_qualityThreshold_r15 v;
};

struct MeasIdleCarrierEUTRA_r15 {
	ARFCN_ValueEUTRA_r9 carrierFreq_r15;
	AllowedMeasBandwidth_e allowedMeasBandwidth_r15;
	struct CellList_r15_MeasIdleCarrierEUTRA_r15_validityArea_r15_Optional validityArea_r15;
	struct CellList_r15_MeasIdleCarrierEUTRA_r15_measCellList_r15_Optional measCellList_r15;
	MeasIdleCarrierEUTRA_r15_reportQuantities_e reportQuantities;
	struct MeasIdleCarrierEUTRA_r15_qualityThreshold_r15_qualityThreshold_r15_Optional qualityThreshold_r15;
};

struct MeasIdleCarrierEUTRA_r15_EUTRA_CarrierList_r15_Dynamic {
	size_t d;
	struct MeasIdleCarrierEUTRA_r15* v;
};

typedef struct MeasIdleCarrierEUTRA_r15_EUTRA_CarrierList_r15_Dynamic EUTRA_CarrierList_r15;

struct MeasIdleConfigSIB_r15 {
	EUTRA_CarrierList_r15 measIdleCarrierListEUTRA_r15;
};

struct InterFreqNeighCellInfo_v1610 {
	RSS_MeasPowerBias_r16_e rss_MeasPowerBias_r16;
};

struct InterFreqNeighCellInfo_v1610_InterFreqNeighCellList_v1610_Dynamic {
	size_t d;
	struct InterFreqNeighCellInfo_v1610* v;
};

typedef struct InterFreqNeighCellInfo_v1610_InterFreqNeighCellList_v1610_Dynamic InterFreqNeighCellList_v1610;

struct CellReselectionPriority_InterFreqCarrierFreqInfo_v1610_altCellReselectionPriority_r16_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CellReselectionSubPriority_r13_e_InterFreqCarrierFreqInfo_v1610_altCellReselectionSubPriority_r16_Optional {
	bool d;
	CellReselectionSubPriority_r13_e v;
};

struct RSS_ConfigCarrierInfo_r16_InterFreqCarrierFreqInfo_v1610_rss_ConfigCarrierInfo_r16_Optional {
	bool d;
	struct RSS_ConfigCarrierInfo_r16 v;
};

struct InterFreqNeighCellList_v1610_InterFreqCarrierFreqInfo_v1610_interFreqNeighCellList_v1610_Optional {
	bool d;
	InterFreqNeighCellList_v1610 v;
};

struct InterFreqCarrierFreqInfo_v1610 {
	struct CellReselectionPriority_InterFreqCarrierFreqInfo_v1610_altCellReselectionPriority_r16_Optional altCellReselectionPriority_r16;
	struct CellReselectionSubPriority_r13_e_InterFreqCarrierFreqInfo_v1610_altCellReselectionSubPriority_r16_Optional altCellReselectionSubPriority_r16;
	struct RSS_ConfigCarrierInfo_r16_InterFreqCarrierFreqInfo_v1610_rss_ConfigCarrierInfo_r16_Optional rss_ConfigCarrierInfo_r16;
	struct InterFreqNeighCellList_v1610_InterFreqCarrierFreqInfo_v1610_interFreqNeighCellList_v1610_Optional interFreqNeighCellList_v1610;
};

struct InterFreqCarrierFreqInfo_v1610_InterFreqCarrierFreqList_v1610_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1610* v;
};

typedef struct InterFreqCarrierFreqInfo_v1610_InterFreqCarrierFreqList_v1610_Dynamic InterFreqCarrierFreqList_v1610;

struct InterFreqCarrierFreqInfo_v1610_InterFreqCarrierFreqListExt_v1610_Dynamic {
	size_t d;
	struct InterFreqCarrierFreqInfo_v1610* v;
};

typedef struct InterFreqCarrierFreqInfo_v1610_InterFreqCarrierFreqListExt_v1610_Dynamic InterFreqCarrierFreqListExt_v1610;

typedef uint32_t ARFCN_ValueNR_r15;

enum MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e {
	MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e_kHz15 = 0,
	MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e_kHz30 = 1,
	MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e_kHz120 = 2,
	MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e_kHz240 = 3,
};

typedef enum MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e;

typedef uint16_t FreqBandIndicatorNR_r15;

struct FreqBandIndicatorNR_r15_MultiFrequencyBandListNR_r15_Dynamic {
	size_t d;
	FreqBandIndicatorNR_r15* v;
};

typedef struct FreqBandIndicatorNR_r15_MultiFrequencyBandListNR_r15_Dynamic MultiFrequencyBandListNR_r15;

typedef uint16_t PhysCellIdNR_r15;

enum PhysCellIdRangeNR_r16_range_e {
	PhysCellIdRangeNR_r16_range_e_n4 = 0,
	PhysCellIdRangeNR_r16_range_e_n8 = 1,
	PhysCellIdRangeNR_r16_range_e_n12 = 2,
	PhysCellIdRangeNR_r16_range_e_n16 = 3,
	PhysCellIdRangeNR_r16_range_e_n24 = 4,
	PhysCellIdRangeNR_r16_range_e_n32 = 5,
	PhysCellIdRangeNR_r16_range_e_n48 = 6,
	PhysCellIdRangeNR_r16_range_e_n64 = 7,
	PhysCellIdRangeNR_r16_range_e_n84 = 8,
	PhysCellIdRangeNR_r16_range_e_n96 = 9,
	PhysCellIdRangeNR_r16_range_e_n128 = 10,
	PhysCellIdRangeNR_r16_range_e_n168 = 11,
	PhysCellIdRangeNR_r16_range_e_n252 = 12,
	PhysCellIdRangeNR_r16_range_e_n504 = 13,
	PhysCellIdRangeNR_r16_range_e_n1008 = 14,
	PhysCellIdRangeNR_r16_range_e_spare1 = 15,
};

typedef enum PhysCellIdRangeNR_r16_range_e PhysCellIdRangeNR_r16_range_e;

struct PhysCellIdRangeNR_r16_range_e_range_Optional {
	bool d;
	PhysCellIdRangeNR_r16_range_e v;
};

struct PhysCellIdRangeNR_r16 {
	PhysCellIdNR_r15 start;
	struct PhysCellIdRangeNR_r16_range_e_range_Optional range;
};

struct PhysCellIdRangeNR_r16_CellListNR_r16_Dynamic {
	size_t d;
	struct PhysCellIdRangeNR_r16* v;
};

typedef struct PhysCellIdRangeNR_r16_CellListNR_r16_Dynamic CellListNR_r16;

enum MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e {
	MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e_rsrp = 0,
	MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e_rsrq = 1,
	MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e_both = 2,
};

typedef enum MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e;

typedef uint8_t RSRP_RangeNR_r15;

typedef uint8_t RSRQ_RangeNR_r15;

struct RSRP_RangeNR_r15_MeasIdleCarrierNR_r16_qualityThresholdNR_r16_idleRSRP_ThresholdNR_r16_Optional {
	bool d;
	RSRP_RangeNR_r15 v;
};

struct RSRQ_RangeNR_r15_MeasIdleCarrierNR_r16_qualityThresholdNR_r16_idleRSRQ_ThresholdNR_r16_Optional {
	bool d;
	RSRQ_RangeNR_r15 v;
};

struct MeasIdleCarrierNR_r16_qualityThresholdNR_r16 {
	struct RSRP_RangeNR_r15_MeasIdleCarrierNR_r16_qualityThresholdNR_r16_idleRSRP_ThresholdNR_r16_Optional idleRSRP_ThresholdNR_r16;
	struct RSRQ_RangeNR_r15_MeasIdleCarrierNR_r16_qualityThresholdNR_r16_idleRSRQ_ThresholdNR_r16_Optional idleRSRQ_ThresholdNR_r16;
};

typedef uint8_t MaxRS_IndexCellQualNR_r15;

typedef uint8_t RS_SINR_RangeNR_r15;

struct RSRP_RangeNR_r15_ThresholdListNR_r15_nr_RSRP_r15_Optional {
	bool d;
	RSRP_RangeNR_r15 v;
};

struct RSRQ_RangeNR_r15_ThresholdListNR_r15_nr_RSRQ_r15_Optional {
	bool d;
	RSRQ_RangeNR_r15 v;
};

struct RS_SINR_RangeNR_r15_ThresholdListNR_r15_nr_SINR_r15_Optional {
	bool d;
	RS_SINR_RangeNR_r15 v;
};

struct ThresholdListNR_r15 {
	struct RSRP_RangeNR_r15_ThresholdListNR_r15_nr_RSRP_r15_Optional nr_RSRP_r15;
	struct RSRQ_RangeNR_r15_ThresholdListNR_r15_nr_RSRQ_r15_Optional nr_RSRQ_r15;
	struct RS_SINR_RangeNR_r15_ThresholdListNR_r15_nr_SINR_r15_Optional nr_SINR_r15;
};

enum MTC_SSB_NR_r15_periodicityAndOffset_r15_Sel {
	MTC_SSB_NR_r15_periodicityAndOffset_r15_UNBOUND_VALUE = 0,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf5_r15 = 1,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf10_r15 = 2,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf20_r15 = 3,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf40_r15 = 4,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf80_r15 = 5,
	MTC_SSB_NR_r15_periodicityAndOffset_r15_sf160_r15 = 6,
};

union MTC_SSB_NR_r15_periodicityAndOffset_r15_Value {
	uint8_t sf5_r15;
	uint8_t sf10_r15;
	uint8_t sf20_r15;
	uint8_t sf40_r15;
	uint8_t sf80_r15;
	uint8_t sf160_r15;
};

struct MTC_SSB_NR_r15_periodicityAndOffset_r15 {
	enum MTC_SSB_NR_r15_periodicityAndOffset_r15_Sel d;
	union MTC_SSB_NR_r15_periodicityAndOffset_r15_Value v;
};

enum MTC_SSB_NR_r15_ssb_Duration_r15_e {
	MTC_SSB_NR_r15_ssb_Duration_r15_e_sf1 = 0,
	MTC_SSB_NR_r15_ssb_Duration_r15_e_sf2 = 1,
	MTC_SSB_NR_r15_ssb_Duration_r15_e_sf3 = 2,
	MTC_SSB_NR_r15_ssb_Duration_r15_e_sf4 = 3,
	MTC_SSB_NR_r15_ssb_Duration_r15_e_sf5 = 4,
};

typedef enum MTC_SSB_NR_r15_ssb_Duration_r15_e MTC_SSB_NR_r15_ssb_Duration_r15_e;

struct MTC_SSB_NR_r15 {
	struct MTC_SSB_NR_r15_periodicityAndOffset_r15 periodicityAndOffset_r15;
	MTC_SSB_NR_r15_ssb_Duration_r15_e ssb_Duration_r15;
};

enum SSB_ToMeasure_r15_Sel {
	SSB_ToMeasure_r15_UNBOUND_VALUE = 0,
	SSB_ToMeasure_r15_shortBitmap_r15 = 1,
	SSB_ToMeasure_r15_mediumBitmap_r15 = 2,
	SSB_ToMeasure_r15_longBitmap_r15 = 3,
};

union SSB_ToMeasure_r15_Value {
	B4 shortBitmap_r15;
	B8 mediumBitmap_r15;
	B64 longBitmap_r15;
};

struct SSB_ToMeasure_r15 {
	enum SSB_ToMeasure_r15_Sel d;
	union SSB_ToMeasure_r15_Value v;
};

struct SS_RSSI_Measurement_r15 {
	BIT_STRING measurementSlots_r15;
	uint8_t endSymbol_r15;
};

struct MaxRS_IndexCellQualNR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_maxRS_IndexCellQual_r16_Optional {
	bool d;
	MaxRS_IndexCellQualNR_r15 v;
};

struct ThresholdListNR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_threshRS_Index_r16_Optional {
	bool d;
	struct ThresholdListNR_r15 v;
};

struct MTC_SSB_NR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_measTimingConfig_r16_Optional {
	bool d;
	struct MTC_SSB_NR_r15 v;
};

struct SSB_ToMeasure_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ssb_ToMeasure_r16_Optional {
	bool d;
	struct SSB_ToMeasure_r15 v;
};

struct SS_RSSI_Measurement_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ss_RSSI_Measurement_r16_Optional {
	bool d;
	struct SS_RSSI_Measurement_r15 v;
};

struct MeasIdleCarrierNR_r16_ssb_MeasConfig_r16 {
	struct MaxRS_IndexCellQualNR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_maxRS_IndexCellQual_r16_Optional maxRS_IndexCellQual_r16;
	struct ThresholdListNR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_threshRS_Index_r16_Optional threshRS_Index_r16;
	struct MTC_SSB_NR_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_measTimingConfig_r16_Optional measTimingConfig_r16;
	struct SSB_ToMeasure_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ssb_ToMeasure_r16_Optional ssb_ToMeasure_r16;
	bool deriveSSB_IndexFromCell_r16;
	struct SS_RSSI_Measurement_r15_MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ss_RSSI_Measurement_r16_Optional ss_RSSI_Measurement_r16;
};

enum BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e {
	BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e_rsrp = 0,
	BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e_rsrq = 1,
	BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e_both = 2,
};

typedef enum BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e;

struct BeamMeasConfigIdleNR_r16 {
	BeamMeasConfigIdleNR_r16_reportQuantityRS_IndexNR_r16_e reportQuantityRS_IndexNR_r16;
	uint8_t maxReportRS_Index_r16;
	bool reportRS_IndexResultsNR_r16;
};

struct MultiFrequencyBandListNR_r15_MeasIdleCarrierNR_r16_frequencyBandList_Optional {
	bool d;
	MultiFrequencyBandListNR_r15 v;
};

struct CellListNR_r16_MeasIdleCarrierNR_r16_measCellListNR_r16_Optional {
	bool d;
	CellListNR_r16 v;
};

struct MeasIdleCarrierNR_r16_qualityThresholdNR_r16_qualityThresholdNR_r16_Optional {
	bool d;
	struct MeasIdleCarrierNR_r16_qualityThresholdNR_r16 v;
};

struct MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ssb_MeasConfig_r16_Optional {
	bool d;
	struct MeasIdleCarrierNR_r16_ssb_MeasConfig_r16 v;
};

struct BeamMeasConfigIdleNR_r16_MeasIdleCarrierNR_r16_beamMeasConfigIdle_r16_Optional {
	bool d;
	struct BeamMeasConfigIdleNR_r16 v;
};

struct MeasIdleCarrierNR_r16 {
	ARFCN_ValueNR_r15 carrierFreqNR_r16;
	MeasIdleCarrierNR_r16_subcarrierSpacingSSB_r16_e subcarrierSpacingSSB_r16;
	struct MultiFrequencyBandListNR_r15_MeasIdleCarrierNR_r16_frequencyBandList_Optional frequencyBandList;
	struct CellListNR_r16_MeasIdleCarrierNR_r16_measCellListNR_r16_Optional measCellListNR_r16;
	MeasIdleCarrierNR_r16_reportQuantitiesNR_r16_e reportQuantitiesNR_r16;
	struct MeasIdleCarrierNR_r16_qualityThresholdNR_r16_qualityThresholdNR_r16_Optional qualityThresholdNR_r16;
	struct MeasIdleCarrierNR_r16_ssb_MeasConfig_r16_ssb_MeasConfig_r16_Optional ssb_MeasConfig_r16;
	struct BeamMeasConfigIdleNR_r16_MeasIdleCarrierNR_r16_beamMeasConfigIdle_r16_Optional beamMeasConfigIdle_r16;
};

struct MeasIdleCarrierNR_r16_NR_CarrierList_r16_Dynamic {
	size_t d;
	struct MeasIdleCarrierNR_r16* v;
};

typedef struct MeasIdleCarrierNR_r16_NR_CarrierList_r16_Dynamic NR_CarrierList_r16;

struct MeasIdleConfigSIB_NR_r16 {
	NR_CarrierList_r16 measIdleCarrierListNR_r16;
};

struct OCTET_STRING_SystemInformationBlockType5_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct InterFreqCarrierFreqList_v1250_SystemInformationBlockType5_interFreqCarrierFreqList_v1250_Optional {
	bool d;
	InterFreqCarrierFreqList_v1250 v;
};

struct InterFreqCarrierFreqListExt_r12_SystemInformationBlockType5_interFreqCarrierFreqListExt_r12_Optional {
	bool d;
	InterFreqCarrierFreqListExt_r12 v;
};

struct InterFreqCarrierFreqListExt_v1280_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1280_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1280 v;
};

struct InterFreqCarrierFreqList_v1310_SystemInformationBlockType5_interFreqCarrierFreqList_v1310_Optional {
	bool d;
	InterFreqCarrierFreqList_v1310 v;
};

struct InterFreqCarrierFreqListExt_v1310_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1310_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1310 v;
};

struct InterFreqCarrierFreqList_v1350_SystemInformationBlockType5_interFreqCarrierFreqList_v1350_Optional {
	bool d;
	InterFreqCarrierFreqList_v1350 v;
};

struct InterFreqCarrierFreqListExt_v1350_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1350_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1350 v;
};

struct InterFreqCarrierFreqListExt_v1360_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1360_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1360 v;
};

struct uint8_t_SystemInformationBlockType5_scptm_FreqOffset_r14_Optional {
	bool d;
	uint8_t v;
};

struct InterFreqCarrierFreqList_v1530_SystemInformationBlockType5_interFreqCarrierFreqList_v1530_Optional {
	bool d;
	InterFreqCarrierFreqList_v1530 v;
};

struct InterFreqCarrierFreqListExt_v1530_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1530_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1530 v;
};

struct MeasIdleConfigSIB_r15_SystemInformationBlockType5_measIdleConfigSIB_r15_Optional {
	bool d;
	struct MeasIdleConfigSIB_r15 v;
};

struct InterFreqCarrierFreqList_v1610_SystemInformationBlockType5_interFreqCarrierFreqList_v1610_Optional {
	bool d;
	InterFreqCarrierFreqList_v1610 v;
};

struct InterFreqCarrierFreqListExt_v1610_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1610_Optional {
	bool d;
	InterFreqCarrierFreqListExt_v1610 v;
};

struct MeasIdleConfigSIB_NR_r16_SystemInformationBlockType5_measIdleConfigSIB_NR_r16_Optional {
	bool d;
	struct MeasIdleConfigSIB_NR_r16 v;
};

struct SystemInformationBlockType5 {
	InterFreqCarrierFreqList interFreqCarrierFreqList;
	struct OCTET_STRING_SystemInformationBlockType5_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct InterFreqCarrierFreqList_v1250_SystemInformationBlockType5_interFreqCarrierFreqList_v1250_Optional interFreqCarrierFreqList_v1250;
	struct InterFreqCarrierFreqListExt_r12_SystemInformationBlockType5_interFreqCarrierFreqListExt_r12_Optional interFreqCarrierFreqListExt_r12;
	struct InterFreqCarrierFreqListExt_v1280_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1280_Optional interFreqCarrierFreqListExt_v1280;
	struct InterFreqCarrierFreqList_v1310_SystemInformationBlockType5_interFreqCarrierFreqList_v1310_Optional interFreqCarrierFreqList_v1310;
	struct InterFreqCarrierFreqListExt_v1310_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1310_Optional interFreqCarrierFreqListExt_v1310;
	struct InterFreqCarrierFreqList_v1350_SystemInformationBlockType5_interFreqCarrierFreqList_v1350_Optional interFreqCarrierFreqList_v1350;
	struct InterFreqCarrierFreqListExt_v1350_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1350_Optional interFreqCarrierFreqListExt_v1350;
	struct InterFreqCarrierFreqListExt_v1360_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1360_Optional interFreqCarrierFreqListExt_v1360;
	struct uint8_t_SystemInformationBlockType5_scptm_FreqOffset_r14_Optional scptm_FreqOffset_r14;
	struct InterFreqCarrierFreqList_v1530_SystemInformationBlockType5_interFreqCarrierFreqList_v1530_Optional interFreqCarrierFreqList_v1530;
	struct InterFreqCarrierFreqListExt_v1530_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1530_Optional interFreqCarrierFreqListExt_v1530;
	struct MeasIdleConfigSIB_r15_SystemInformationBlockType5_measIdleConfigSIB_r15_Optional measIdleConfigSIB_r15;
	struct InterFreqCarrierFreqList_v1610_SystemInformationBlockType5_interFreqCarrierFreqList_v1610_Optional interFreqCarrierFreqList_v1610;
	struct InterFreqCarrierFreqListExt_v1610_SystemInformationBlockType5_interFreqCarrierFreqListExt_v1610_Optional interFreqCarrierFreqListExt_v1610;
	struct MeasIdleConfigSIB_NR_r16_SystemInformationBlockType5_measIdleConfigSIB_NR_r16_Optional measIdleConfigSIB_NR_r16;
};

typedef uint16_t ARFCN_ValueUTRA;

struct CarrierFreqUTRA_FDD_threshX_Q_r9 {
	ReselectionThresholdQ_r9 threshX_HighQ_r9;
	ReselectionThresholdQ_r9 threshX_LowQ_r9;
};

struct CellReselectionPriority_CarrierFreqUTRA_FDD_cellReselectionPriority_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CarrierFreqUTRA_FDD_threshX_Q_r9_threshX_Q_r9_Optional {
	bool d;
	struct CarrierFreqUTRA_FDD_threshX_Q_r9 v;
};

struct CarrierFreqUTRA_FDD {
	ARFCN_ValueUTRA carrierFreq;
	struct CellReselectionPriority_CarrierFreqUTRA_FDD_cellReselectionPriority_Optional cellReselectionPriority;
	ReselectionThreshold threshX_High;
	ReselectionThreshold threshX_Low;
	int8_t q_RxLevMin;
	int8_t p_MaxUTRA;
	int8_t q_QualMin;
	struct CarrierFreqUTRA_FDD_threshX_Q_r9_threshX_Q_r9_Optional threshX_Q_r9;
};

struct CarrierFreqUTRA_FDD_CarrierFreqListUTRA_FDD_Dynamic {
	size_t d;
	struct CarrierFreqUTRA_FDD* v;
};

typedef struct CarrierFreqUTRA_FDD_CarrierFreqListUTRA_FDD_Dynamic CarrierFreqListUTRA_FDD;

struct CellReselectionPriority_CarrierFreqUTRA_TDD_cellReselectionPriority_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CarrierFreqUTRA_TDD {
	ARFCN_ValueUTRA carrierFreq;
	struct CellReselectionPriority_CarrierFreqUTRA_TDD_cellReselectionPriority_Optional cellReselectionPriority;
	ReselectionThreshold threshX_High;
	ReselectionThreshold threshX_Low;
	int8_t q_RxLevMin;
	int8_t p_MaxUTRA;
};

struct CarrierFreqUTRA_TDD_CarrierFreqListUTRA_TDD_Dynamic {
	size_t d;
	struct CarrierFreqUTRA_TDD* v;
};

typedef struct CarrierFreqUTRA_TDD_CarrierFreqListUTRA_TDD_Dynamic CarrierFreqListUTRA_TDD;

enum CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e {
	CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e_true = 0,
};

typedef enum CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e;

struct CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional {
	bool d;
	CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e v;
};

struct CarrierFreqInfoUTRA_v1250 {
	struct CarrierFreqInfoUTRA_v1250_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional reducedMeasPerformance_r12;
};

struct CarrierFreqUTRA_FDD_Ext_r12_threshX_Q_r12 {
	ReselectionThresholdQ_r9 threshX_HighQ_r12;
	ReselectionThresholdQ_r9 threshX_LowQ_r12;
};

typedef uint8_t FreqBandIndicator_UTRA_FDD;

enum CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e {
	CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e_true = 0,
};

typedef enum CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e;

struct CellReselectionPriority_CarrierFreqUTRA_FDD_Ext_r12_cellReselectionPriority_r12_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CarrierFreqUTRA_FDD_Ext_r12_threshX_Q_r12_threshX_Q_r12_Optional {
	bool d;
	struct CarrierFreqUTRA_FDD_Ext_r12_threshX_Q_r12 v;
};

struct FreqBandIndicator_UTRA_FDD_CarrierFreqUTRA_FDD_Ext_r12_multiBandInfoList_r12_Dynamic {
	size_t d;
	FreqBandIndicator_UTRA_FDD* v;
};

struct FreqBandIndicator_UTRA_FDD_CarrierFreqUTRA_FDD_Ext_r12_multiBandInfoList_r12_DynamicOptional {
	bool d;
	struct FreqBandIndicator_UTRA_FDD_CarrierFreqUTRA_FDD_Ext_r12_multiBandInfoList_r12_Dynamic v;
};

struct CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional {
	bool d;
	CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e v;
};

struct CarrierFreqUTRA_FDD_Ext_r12 {
	ARFCN_ValueUTRA carrierFreq_r12;
	struct CellReselectionPriority_CarrierFreqUTRA_FDD_Ext_r12_cellReselectionPriority_r12_Optional cellReselectionPriority_r12;
	ReselectionThreshold threshX_High_r12;
	ReselectionThreshold threshX_Low_r12;
	int8_t q_RxLevMin_r12;
	int8_t p_MaxUTRA_r12;
	int8_t q_QualMin_r12;
	struct CarrierFreqUTRA_FDD_Ext_r12_threshX_Q_r12_threshX_Q_r12_Optional threshX_Q_r12;
	struct FreqBandIndicator_UTRA_FDD_CarrierFreqUTRA_FDD_Ext_r12_multiBandInfoList_r12_DynamicOptional multiBandInfoList_r12;
	struct CarrierFreqUTRA_FDD_Ext_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional reducedMeasPerformance_r12;
};

struct CarrierFreqUTRA_FDD_Ext_r12_CarrierFreqListUTRA_FDD_Ext_r12_Dynamic {
	size_t d;
	struct CarrierFreqUTRA_FDD_Ext_r12* v;
};

typedef struct CarrierFreqUTRA_FDD_Ext_r12_CarrierFreqListUTRA_FDD_Ext_r12_Dynamic CarrierFreqListUTRA_FDD_Ext_r12;

enum CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e {
	CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e_true = 0,
};

typedef enum CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e;

struct CellReselectionPriority_CarrierFreqUTRA_TDD_r12_cellReselectionPriority_r12_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional {
	bool d;
	CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e v;
};

struct CarrierFreqUTRA_TDD_r12 {
	ARFCN_ValueUTRA carrierFreq_r12;
	struct CellReselectionPriority_CarrierFreqUTRA_TDD_r12_cellReselectionPriority_r12_Optional cellReselectionPriority_r12;
	ReselectionThreshold threshX_High_r12;
	ReselectionThreshold threshX_Low_r12;
	int8_t q_RxLevMin_r12;
	int8_t p_MaxUTRA_r12;
	struct CarrierFreqUTRA_TDD_r12_reducedMeasPerformance_r12_e_reducedMeasPerformance_r12_Optional reducedMeasPerformance_r12;
};

struct CarrierFreqUTRA_TDD_r12_CarrierFreqListUTRA_TDD_Ext_r12_Dynamic {
	size_t d;
	struct CarrierFreqUTRA_TDD_r12* v;
};

typedef struct CarrierFreqUTRA_TDD_r12_CarrierFreqListUTRA_TDD_Ext_r12_Dynamic CarrierFreqListUTRA_TDD_Ext_r12;

struct CarrierFreqListUTRA_FDD_SystemInformationBlockType6_carrierFreqListUTRA_FDD_Optional {
	bool d;
	CarrierFreqListUTRA_FDD v;
};

struct CarrierFreqListUTRA_TDD_SystemInformationBlockType6_carrierFreqListUTRA_TDD_Optional {
	bool d;
	CarrierFreqListUTRA_TDD v;
};

struct SpeedStateScaleFactors_SystemInformationBlockType6_t_ReselectionUTRA_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct OCTET_STRING_SystemInformationBlockType6_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType6_CarrierFreqInfoUTRA_v1250_carrierFreqListUTRA_FDD_v1250_Dynamic {
	size_t d;
	struct CarrierFreqInfoUTRA_v1250* v;
};

struct CarrierFreqInfoUTRA_v1250_SystemInformationBlockType6_carrierFreqListUTRA_FDD_v1250_DynamicOptional {
	bool d;
	struct SystemInformationBlockType6_CarrierFreqInfoUTRA_v1250_carrierFreqListUTRA_FDD_v1250_Dynamic v;
};

struct SystemInformationBlockType6_CarrierFreqInfoUTRA_v1250_carrierFreqListUTRA_TDD_v1250_Dynamic {
	size_t d;
	struct CarrierFreqInfoUTRA_v1250* v;
};

struct CarrierFreqInfoUTRA_v1250_SystemInformationBlockType6_carrierFreqListUTRA_TDD_v1250_DynamicOptional {
	bool d;
	struct SystemInformationBlockType6_CarrierFreqInfoUTRA_v1250_carrierFreqListUTRA_TDD_v1250_Dynamic v;
};

struct CarrierFreqListUTRA_FDD_Ext_r12_SystemInformationBlockType6_carrierFreqListUTRA_FDD_Ext_r12_Optional {
	bool d;
	CarrierFreqListUTRA_FDD_Ext_r12 v;
};

struct CarrierFreqListUTRA_TDD_Ext_r12_SystemInformationBlockType6_carrierFreqListUTRA_TDD_Ext_r12_Optional {
	bool d;
	CarrierFreqListUTRA_TDD_Ext_r12 v;
};

struct SystemInformationBlockType6 {
	struct CarrierFreqListUTRA_FDD_SystemInformationBlockType6_carrierFreqListUTRA_FDD_Optional carrierFreqListUTRA_FDD;
	struct CarrierFreqListUTRA_TDD_SystemInformationBlockType6_carrierFreqListUTRA_TDD_Optional carrierFreqListUTRA_TDD;
	T_Reselection t_ReselectionUTRA;
	struct SpeedStateScaleFactors_SystemInformationBlockType6_t_ReselectionUTRA_SF_Optional t_ReselectionUTRA_SF;
	struct OCTET_STRING_SystemInformationBlockType6_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct CarrierFreqInfoUTRA_v1250_SystemInformationBlockType6_carrierFreqListUTRA_FDD_v1250_DynamicOptional carrierFreqListUTRA_FDD_v1250;
	struct CarrierFreqInfoUTRA_v1250_SystemInformationBlockType6_carrierFreqListUTRA_TDD_v1250_DynamicOptional carrierFreqListUTRA_TDD_v1250;
	struct CarrierFreqListUTRA_FDD_Ext_r12_SystemInformationBlockType6_carrierFreqListUTRA_FDD_Ext_r12_Optional carrierFreqListUTRA_FDD_Ext_r12;
	struct CarrierFreqListUTRA_TDD_Ext_r12_SystemInformationBlockType6_carrierFreqListUTRA_TDD_Ext_r12_Optional carrierFreqListUTRA_TDD_Ext_r12;
};

typedef uint16_t ARFCN_ValueGERAN;

enum BandIndicatorGERAN_e {
	BandIndicatorGERAN_e_dcs1800 = 0,
	BandIndicatorGERAN_e_pcs1900 = 1,
};

typedef enum BandIndicatorGERAN_e BandIndicatorGERAN_e;

struct ARFCN_ValueGERAN_ExplicitListOfARFCNs_Dynamic {
	size_t d;
	ARFCN_ValueGERAN* v;
};

typedef struct ARFCN_ValueGERAN_ExplicitListOfARFCNs_Dynamic ExplicitListOfARFCNs;

struct CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs {
	uint8_t arfcn_Spacing;
	uint8_t numberOfFollowingARFCNs;
};

enum CarrierFreqsGERAN_followingARFCNs_Sel {
	CarrierFreqsGERAN_followingARFCNs_UNBOUND_VALUE = 0,
	CarrierFreqsGERAN_followingARFCNs_explicitListOfARFCNs = 1,
	CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs = 2,
	CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs = 3,
};

union CarrierFreqsGERAN_followingARFCNs_Value {
	ExplicitListOfARFCNs explicitListOfARFCNs;
	struct CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs equallySpacedARFCNs;
	OCTET_STRING variableBitMapOfARFCNs;
};

struct CarrierFreqsGERAN_followingARFCNs {
	enum CarrierFreqsGERAN_followingARFCNs_Sel d;
	union CarrierFreqsGERAN_followingARFCNs_Value v;
};

struct CarrierFreqsGERAN {
	ARFCN_ValueGERAN startingARFCN;
	BandIndicatorGERAN_e bandIndicator;
	struct CarrierFreqsGERAN_followingARFCNs followingARFCNs;
};

struct CellReselectionPriority_CarrierFreqsInfoGERAN_commonInfo_cellReselectionPriority_Optional {
	bool d;
	CellReselectionPriority v;
};

struct uint8_t_CarrierFreqsInfoGERAN_commonInfo_p_MaxGERAN_Optional {
	bool d;
	uint8_t v;
};

struct CarrierFreqsInfoGERAN_commonInfo {
	struct CellReselectionPriority_CarrierFreqsInfoGERAN_commonInfo_cellReselectionPriority_Optional cellReselectionPriority;
	B8 ncc_Permitted;
	uint8_t q_RxLevMin;
	struct uint8_t_CarrierFreqsInfoGERAN_commonInfo_p_MaxGERAN_Optional p_MaxGERAN;
	ReselectionThreshold threshX_High;
	ReselectionThreshold threshX_Low;
};

struct CarrierFreqsInfoGERAN {
	struct CarrierFreqsGERAN carrierFreqs;
	struct CarrierFreqsInfoGERAN_commonInfo commonInfo;
};

struct CarrierFreqsInfoGERAN_CarrierFreqsInfoListGERAN_Dynamic {
	size_t d;
	struct CarrierFreqsInfoGERAN* v;
};

typedef struct CarrierFreqsInfoGERAN_CarrierFreqsInfoListGERAN_Dynamic CarrierFreqsInfoListGERAN;

struct SpeedStateScaleFactors_SystemInformationBlockType7_t_ReselectionGERAN_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct CarrierFreqsInfoListGERAN_SystemInformationBlockType7_carrierFreqsInfoList_Optional {
	bool d;
	CarrierFreqsInfoListGERAN v;
};

struct OCTET_STRING_SystemInformationBlockType7_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType7 {
	T_Reselection t_ReselectionGERAN;
	struct SpeedStateScaleFactors_SystemInformationBlockType7_t_ReselectionGERAN_SF_Optional t_ReselectionGERAN_SF;
	struct CarrierFreqsInfoListGERAN_SystemInformationBlockType7_carrierFreqsInfoList_Optional carrierFreqsInfoList;
	struct OCTET_STRING_SystemInformationBlockType7_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SystemTimeInfoCDMA2000_cdma_SystemTime_Sel {
	SystemTimeInfoCDMA2000_cdma_SystemTime_UNBOUND_VALUE = 0,
	SystemTimeInfoCDMA2000_cdma_SystemTime_synchronousSystemTime = 1,
	SystemTimeInfoCDMA2000_cdma_SystemTime_asynchronousSystemTime = 2,
};

union SystemTimeInfoCDMA2000_cdma_SystemTime_Value {
	B39 synchronousSystemTime;
	B49 asynchronousSystemTime;
};

struct SystemTimeInfoCDMA2000_cdma_SystemTime {
	enum SystemTimeInfoCDMA2000_cdma_SystemTime_Sel d;
	union SystemTimeInfoCDMA2000_cdma_SystemTime_Value v;
};

struct SystemTimeInfoCDMA2000 {
	bool cdma_EUTRA_Synchronisation;
	struct SystemTimeInfoCDMA2000_cdma_SystemTime cdma_SystemTime;
};

typedef uint8_t PreRegistrationZoneIdHRPD;

struct PreRegistrationZoneIdHRPD_SecondaryPreRegistrationZoneIdListHRPD_Dynamic {
	size_t d;
	PreRegistrationZoneIdHRPD* v;
};

typedef struct PreRegistrationZoneIdHRPD_SecondaryPreRegistrationZoneIdListHRPD_Dynamic SecondaryPreRegistrationZoneIdListHRPD;

struct PreRegistrationZoneIdHRPD_PreRegistrationInfoHRPD_preRegistrationZoneId_Optional {
	bool d;
	PreRegistrationZoneIdHRPD v;
};

struct SecondaryPreRegistrationZoneIdListHRPD_PreRegistrationInfoHRPD_secondaryPreRegistrationZoneIdList_Optional {
	bool d;
	SecondaryPreRegistrationZoneIdListHRPD v;
};

struct PreRegistrationInfoHRPD {
	bool preRegistrationAllowed;
	struct PreRegistrationZoneIdHRPD_PreRegistrationInfoHRPD_preRegistrationZoneId_Optional preRegistrationZoneId;
	struct SecondaryPreRegistrationZoneIdListHRPD_PreRegistrationInfoHRPD_secondaryPreRegistrationZoneIdList_Optional secondaryPreRegistrationZoneIdList;
};

enum BandclassCDMA2000_e {
	BandclassCDMA2000_e_bc0 = 0,
	BandclassCDMA2000_e_bc1 = 1,
	BandclassCDMA2000_e_bc2 = 2,
	BandclassCDMA2000_e_bc3 = 3,
	BandclassCDMA2000_e_bc4 = 4,
	BandclassCDMA2000_e_bc5 = 5,
	BandclassCDMA2000_e_bc6 = 6,
	BandclassCDMA2000_e_bc7 = 7,
	BandclassCDMA2000_e_bc8 = 8,
	BandclassCDMA2000_e_bc9 = 9,
	BandclassCDMA2000_e_bc10 = 10,
	BandclassCDMA2000_e_bc11 = 11,
	BandclassCDMA2000_e_bc12 = 12,
	BandclassCDMA2000_e_bc13 = 13,
	BandclassCDMA2000_e_bc14 = 14,
	BandclassCDMA2000_e_bc15 = 15,
	BandclassCDMA2000_e_bc16 = 16,
	BandclassCDMA2000_e_bc17 = 17,
	BandclassCDMA2000_e_bc18_v9a0 = 18,
	BandclassCDMA2000_e_bc19_v9a0 = 19,
	BandclassCDMA2000_e_bc20_v9a0 = 20,
	BandclassCDMA2000_e_bc21_v9a0 = 21,
	BandclassCDMA2000_e_spare10 = 22,
	BandclassCDMA2000_e_spare9 = 23,
	BandclassCDMA2000_e_spare8 = 24,
	BandclassCDMA2000_e_spare7 = 25,
	BandclassCDMA2000_e_spare6 = 26,
	BandclassCDMA2000_e_spare5 = 27,
	BandclassCDMA2000_e_spare4 = 28,
	BandclassCDMA2000_e_spare3 = 29,
	BandclassCDMA2000_e_spare2 = 30,
	BandclassCDMA2000_e_spare1 = 31,
};

typedef enum BandclassCDMA2000_e BandclassCDMA2000_e;

struct CellReselectionPriority_BandClassInfoCDMA2000_cellReselectionPriority_Optional {
	bool d;
	CellReselectionPriority v;
};

struct BandClassInfoCDMA2000 {
	BandclassCDMA2000_e bandClass;
	struct CellReselectionPriority_BandClassInfoCDMA2000_cellReselectionPriority_Optional cellReselectionPriority;
	uint8_t threshX_High;
	uint8_t threshX_Low;
};

struct BandClassInfoCDMA2000_BandClassListCDMA2000_Dynamic {
	size_t d;
	struct BandClassInfoCDMA2000* v;
};

typedef struct BandClassInfoCDMA2000_BandClassListCDMA2000_Dynamic BandClassListCDMA2000;

typedef uint16_t ARFCN_ValueCDMA2000;

typedef uint16_t PhysCellIdCDMA2000;

struct PhysCellIdCDMA2000_PhysCellIdListCDMA2000_Dynamic {
	size_t d;
	PhysCellIdCDMA2000* v;
};

typedef struct PhysCellIdCDMA2000_PhysCellIdListCDMA2000_Dynamic PhysCellIdListCDMA2000;

struct NeighCellsPerBandclassCDMA2000 {
	ARFCN_ValueCDMA2000 arfcn;
	PhysCellIdListCDMA2000 physCellIdList;
};

struct NeighCellsPerBandclassCDMA2000_NeighCellsPerBandclassListCDMA2000_Dynamic {
	size_t d;
	struct NeighCellsPerBandclassCDMA2000* v;
};

typedef struct NeighCellsPerBandclassCDMA2000_NeighCellsPerBandclassListCDMA2000_Dynamic NeighCellsPerBandclassListCDMA2000;

struct NeighCellCDMA2000 {
	BandclassCDMA2000_e bandClass;
	NeighCellsPerBandclassListCDMA2000 neighCellsPerFreqList;
};

struct NeighCellCDMA2000_NeighCellListCDMA2000_Dynamic {
	size_t d;
	struct NeighCellCDMA2000* v;
};

typedef struct NeighCellCDMA2000_NeighCellListCDMA2000_Dynamic NeighCellListCDMA2000;

struct SpeedStateScaleFactors_CellReselectionParametersCDMA2000_t_ReselectionCDMA2000_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct CellReselectionParametersCDMA2000 {
	BandClassListCDMA2000 bandClassList;
	NeighCellListCDMA2000 neighCellList;
	T_Reselection t_ReselectionCDMA2000;
	struct SpeedStateScaleFactors_CellReselectionParametersCDMA2000_t_ReselectionCDMA2000_SF_Optional t_ReselectionCDMA2000_SF;
};

struct CellReselectionParametersCDMA2000_SystemInformationBlockType8_parametersHRPD_cellReselectionParametersHRPD_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000 v;
};

struct SystemInformationBlockType8_parametersHRPD {
	struct PreRegistrationInfoHRPD preRegistrationInfoHRPD;
	struct CellReselectionParametersCDMA2000_SystemInformationBlockType8_parametersHRPD_cellReselectionParametersHRPD_Optional cellReselectionParametersHRPD;
};

struct CSFB_RegistrationParam1XRTT {
	B15 sid;
	B16 nid;
	bool multipleSID;
	bool multipleNID;
	bool homeReg;
	bool foreignSIDReg;
	bool foreignNIDReg;
	bool parameterReg;
	bool powerUpReg;
	B7 registrationPeriod;
	B12 registrationZone;
	B3 totalZone;
	B3 zoneTimer;
};

struct CSFB_RegistrationParam1XRTT_SystemInformationBlockType8_parameters1XRTT_csfb_RegistrationParam1XRTT_Optional {
	bool d;
	struct CSFB_RegistrationParam1XRTT v;
};

struct B42_SystemInformationBlockType8_parameters1XRTT_longCodeState1XRTT_Optional {
	bool d;
	B42 v;
};

struct CellReselectionParametersCDMA2000_SystemInformationBlockType8_parameters1XRTT_cellReselectionParameters1XRTT_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000 v;
};

struct SystemInformationBlockType8_parameters1XRTT {
	struct CSFB_RegistrationParam1XRTT_SystemInformationBlockType8_parameters1XRTT_csfb_RegistrationParam1XRTT_Optional csfb_RegistrationParam1XRTT;
	struct B42_SystemInformationBlockType8_parameters1XRTT_longCodeState1XRTT_Optional longCodeState1XRTT;
	struct CellReselectionParametersCDMA2000_SystemInformationBlockType8_parameters1XRTT_cellReselectionParameters1XRTT_Optional cellReselectionParameters1XRTT;
};

struct PhysCellIdCDMA2000_PhysCellIdListCDMA2000_v920_Dynamic {
	size_t d;
	PhysCellIdCDMA2000* v;
};

typedef struct PhysCellIdCDMA2000_PhysCellIdListCDMA2000_v920_Dynamic PhysCellIdListCDMA2000_v920;

struct NeighCellsPerBandclassCDMA2000_v920 {
	PhysCellIdListCDMA2000_v920 physCellIdList_v920;
};

struct NeighCellsPerBandclassCDMA2000_v920_NeighCellsPerBandclassListCDMA2000_v920_Dynamic {
	size_t d;
	struct NeighCellsPerBandclassCDMA2000_v920* v;
};

typedef struct NeighCellsPerBandclassCDMA2000_v920_NeighCellsPerBandclassListCDMA2000_v920_Dynamic NeighCellsPerBandclassListCDMA2000_v920;

struct NeighCellCDMA2000_v920 {
	NeighCellsPerBandclassListCDMA2000_v920 neighCellsPerFreqList_v920;
};

struct NeighCellCDMA2000_v920_NeighCellListCDMA2000_v920_Dynamic {
	size_t d;
	struct NeighCellCDMA2000_v920* v;
};

typedef struct NeighCellCDMA2000_v920_NeighCellListCDMA2000_v920_Dynamic NeighCellListCDMA2000_v920;

struct CellReselectionParametersCDMA2000_v920 {
	NeighCellListCDMA2000_v920 neighCellList_v920;
};

enum CSFB_RegistrationParam1XRTT_v920_powerDownReg_r9_e {
	CSFB_RegistrationParam1XRTT_v920_powerDownReg_r9_e_true = 0,
};

typedef enum CSFB_RegistrationParam1XRTT_v920_powerDownReg_r9_e CSFB_RegistrationParam1XRTT_v920_powerDownReg_r9_e;

struct CSFB_RegistrationParam1XRTT_v920 {
	CSFB_RegistrationParam1XRTT_v920_powerDownReg_r9_e powerDownReg_r9;
};

struct AC_BarringConfig1XRTT_r9 {
	uint8_t ac_Barring0to9_r9;
	uint8_t ac_Barring10_r9;
	uint8_t ac_Barring11_r9;
	uint8_t ac_Barring12_r9;
	uint8_t ac_Barring13_r9;
	uint8_t ac_Barring14_r9;
	uint8_t ac_Barring15_r9;
	uint8_t ac_BarringMsg_r9;
	uint8_t ac_BarringReg_r9;
	uint8_t ac_BarringEmg_r9;
};

enum SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e {
	SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e_true = 0,
};

typedef enum SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e;

enum ParametersCDMA2000_r11_systemTimeInfo_r11_Sel {
	ParametersCDMA2000_r11_systemTimeInfo_r11_UNBOUND_VALUE = 0,
	ParametersCDMA2000_r11_systemTimeInfo_r11_explicitValue = 1,
	ParametersCDMA2000_r11_systemTimeInfo_r11_defaultValue = 2,
};

union ParametersCDMA2000_r11_systemTimeInfo_r11_Value {
	struct SystemTimeInfoCDMA2000 explicitValue;
	Null defaultValue;
};

struct ParametersCDMA2000_r11_systemTimeInfo_r11 {
	enum ParametersCDMA2000_r11_systemTimeInfo_r11_Sel d;
	union ParametersCDMA2000_r11_systemTimeInfo_r11_Value v;
};

struct PhysCellIdCDMA2000_physCellIdList_r11_Dynamic {
	size_t d;
	PhysCellIdCDMA2000* v;
};

struct NeighCellsPerBandclassCDMA2000_r11 {
	ARFCN_ValueCDMA2000 arfcn;
	struct PhysCellIdCDMA2000_physCellIdList_r11_Dynamic physCellIdList_r11;
};

struct NeighCellsPerBandclassCDMA2000_r11_NeighCellCDMA2000_r11_neighFreqInfoList_r11_Dynamic {
	size_t d;
	struct NeighCellsPerBandclassCDMA2000_r11* v;
};

struct NeighCellCDMA2000_r11 {
	BandclassCDMA2000_e bandClass;
	struct NeighCellsPerBandclassCDMA2000_r11_NeighCellCDMA2000_r11_neighFreqInfoList_r11_Dynamic neighFreqInfoList_r11;
};

struct NeighCellCDMA2000_r11_CellReselectionParametersCDMA2000_r11_neighCellList_r11_Dynamic {
	size_t d;
	struct NeighCellCDMA2000_r11* v;
};

struct SpeedStateScaleFactors_CellReselectionParametersCDMA2000_r11_t_ReselectionCDMA2000_SF_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct CellReselectionParametersCDMA2000_r11 {
	BandClassListCDMA2000 bandClassList;
	struct NeighCellCDMA2000_r11_CellReselectionParametersCDMA2000_r11_neighCellList_r11_Dynamic neighCellList_r11;
	T_Reselection t_ReselectionCDMA2000;
	struct SpeedStateScaleFactors_CellReselectionParametersCDMA2000_r11_t_ReselectionCDMA2000_SF_Optional t_ReselectionCDMA2000_SF;
};

struct CellReselectionParametersCDMA2000_r11_ParametersCDMA2000_r11_parametersHRPD_r11_cellReselectionParametersHRPD_r11_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000_r11 v;
};

struct ParametersCDMA2000_r11_parametersHRPD_r11 {
	struct PreRegistrationInfoHRPD preRegistrationInfoHRPD_r11;
	struct CellReselectionParametersCDMA2000_r11_ParametersCDMA2000_r11_parametersHRPD_r11_cellReselectionParametersHRPD_r11_Optional cellReselectionParametersHRPD_r11;
};

enum ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e {
	ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e_true = 0,
};

typedef enum ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e;

struct CSFB_RegistrationParam1XRTT_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_RegistrationParam1XRTT_r11_Optional {
	bool d;
	struct CSFB_RegistrationParam1XRTT v;
};

struct CSFB_RegistrationParam1XRTT_v920_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_RegistrationParam1XRTT_Ext_r11_Optional {
	bool d;
	struct CSFB_RegistrationParam1XRTT_v920 v;
};

struct B42_ParametersCDMA2000_r11_parameters1XRTT_r11_longCodeState1XRTT_r11_Optional {
	bool d;
	B42 v;
};

struct CellReselectionParametersCDMA2000_r11_ParametersCDMA2000_r11_parameters1XRTT_r11_cellReselectionParameters1XRTT_r11_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000_r11 v;
};

struct AC_BarringConfig1XRTT_r9_ParametersCDMA2000_r11_parameters1XRTT_r11_ac_BarringConfig1XRTT_r11_Optional {
	bool d;
	struct AC_BarringConfig1XRTT_r9 v;
};

struct bool_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_SupportForDualRxUEs_r11_Optional {
	bool d;
	bool v;
};

struct ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e_csfb_DualRxTxSupport_r11_Optional {
	bool d;
	ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e v;
};

struct ParametersCDMA2000_r11_parameters1XRTT_r11 {
	struct CSFB_RegistrationParam1XRTT_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_RegistrationParam1XRTT_r11_Optional csfb_RegistrationParam1XRTT_r11;
	struct CSFB_RegistrationParam1XRTT_v920_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_RegistrationParam1XRTT_Ext_r11_Optional csfb_RegistrationParam1XRTT_Ext_r11;
	struct B42_ParametersCDMA2000_r11_parameters1XRTT_r11_longCodeState1XRTT_r11_Optional longCodeState1XRTT_r11;
	struct CellReselectionParametersCDMA2000_r11_ParametersCDMA2000_r11_parameters1XRTT_r11_cellReselectionParameters1XRTT_r11_Optional cellReselectionParameters1XRTT_r11;
	struct AC_BarringConfig1XRTT_r9_ParametersCDMA2000_r11_parameters1XRTT_r11_ac_BarringConfig1XRTT_r11_Optional ac_BarringConfig1XRTT_r11;
	struct bool_ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_SupportForDualRxUEs_r11_Optional csfb_SupportForDualRxUEs_r11;
	struct ParametersCDMA2000_r11_parameters1XRTT_r11_csfb_DualRxTxSupport_r11_e_csfb_DualRxTxSupport_r11_Optional csfb_DualRxTxSupport_r11;
};

struct ParametersCDMA2000_r11_systemTimeInfo_r11_systemTimeInfo_r11_Optional {
	bool d;
	struct ParametersCDMA2000_r11_systemTimeInfo_r11 v;
};

struct ParametersCDMA2000_r11_parametersHRPD_r11_parametersHRPD_r11_Optional {
	bool d;
	struct ParametersCDMA2000_r11_parametersHRPD_r11 v;
};

struct ParametersCDMA2000_r11_parameters1XRTT_r11_parameters1XRTT_r11_Optional {
	bool d;
	struct ParametersCDMA2000_r11_parameters1XRTT_r11 v;
};

struct ParametersCDMA2000_r11 {
	struct ParametersCDMA2000_r11_systemTimeInfo_r11_systemTimeInfo_r11_Optional systemTimeInfo_r11;
	uint8_t searchWindowSize_r11;
	struct ParametersCDMA2000_r11_parametersHRPD_r11_parametersHRPD_r11_Optional parametersHRPD_r11;
	struct ParametersCDMA2000_r11_parameters1XRTT_r11_parameters1XRTT_r11_Optional parameters1XRTT_r11;
};

enum SIB8_PerPLMN_r11_parametersCDMA2000_r11_Sel {
	SIB8_PerPLMN_r11_parametersCDMA2000_r11_UNBOUND_VALUE = 0,
	SIB8_PerPLMN_r11_parametersCDMA2000_r11_explicitValue = 1,
	SIB8_PerPLMN_r11_parametersCDMA2000_r11_defaultValue = 2,
};

union SIB8_PerPLMN_r11_parametersCDMA2000_r11_Value {
	struct ParametersCDMA2000_r11 explicitValue;
	Null defaultValue;
};

struct SIB8_PerPLMN_r11_parametersCDMA2000_r11 {
	enum SIB8_PerPLMN_r11_parametersCDMA2000_r11_Sel d;
	union SIB8_PerPLMN_r11_parametersCDMA2000_r11_Value v;
};

struct SIB8_PerPLMN_r11 {
	uint8_t plmn_Identity_r11;
	struct SIB8_PerPLMN_r11_parametersCDMA2000_r11 parametersCDMA2000_r11;
};

struct SIB8_PerPLMN_r11_SIB8_PerPLMN_List_r11_Dynamic {
	size_t d;
	struct SIB8_PerPLMN_r11* v;
};

typedef struct SIB8_PerPLMN_r11_SIB8_PerPLMN_List_r11_Dynamic SIB8_PerPLMN_List_r11;

struct SystemTimeInfoCDMA2000_SystemInformationBlockType8_systemTimeInfo_Optional {
	bool d;
	struct SystemTimeInfoCDMA2000 v;
};

struct uint8_t_SystemInformationBlockType8_searchWindowSize_Optional {
	bool d;
	uint8_t v;
};

struct SystemInformationBlockType8_parametersHRPD_parametersHRPD_Optional {
	bool d;
	struct SystemInformationBlockType8_parametersHRPD v;
};

struct SystemInformationBlockType8_parameters1XRTT_parameters1XRTT_Optional {
	bool d;
	struct SystemInformationBlockType8_parameters1XRTT v;
};

struct OCTET_STRING_SystemInformationBlockType8_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct bool_SystemInformationBlockType8_csfb_SupportForDualRxUEs_r9_Optional {
	bool d;
	bool v;
};

struct CellReselectionParametersCDMA2000_v920_SystemInformationBlockType8_cellReselectionParametersHRPD_v920_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000_v920 v;
};

struct CellReselectionParametersCDMA2000_v920_SystemInformationBlockType8_cellReselectionParameters1XRTT_v920_Optional {
	bool d;
	struct CellReselectionParametersCDMA2000_v920 v;
};

struct CSFB_RegistrationParam1XRTT_v920_SystemInformationBlockType8_csfb_RegistrationParam1XRTT_v920_Optional {
	bool d;
	struct CSFB_RegistrationParam1XRTT_v920 v;
};

struct AC_BarringConfig1XRTT_r9_SystemInformationBlockType8_ac_BarringConfig1XRTT_r9_Optional {
	bool d;
	struct AC_BarringConfig1XRTT_r9 v;
};

struct SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e_csfb_DualRxTxSupport_r10_Optional {
	bool d;
	SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e v;
};

struct SIB8_PerPLMN_List_r11_SystemInformationBlockType8_sib8_PerPLMN_List_r11_Optional {
	bool d;
	SIB8_PerPLMN_List_r11 v;
};

struct SystemInformationBlockType8 {
	struct SystemTimeInfoCDMA2000_SystemInformationBlockType8_systemTimeInfo_Optional systemTimeInfo;
	struct uint8_t_SystemInformationBlockType8_searchWindowSize_Optional searchWindowSize;
	struct SystemInformationBlockType8_parametersHRPD_parametersHRPD_Optional parametersHRPD;
	struct SystemInformationBlockType8_parameters1XRTT_parameters1XRTT_Optional parameters1XRTT;
	struct OCTET_STRING_SystemInformationBlockType8_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct bool_SystemInformationBlockType8_csfb_SupportForDualRxUEs_r9_Optional csfb_SupportForDualRxUEs_r9;
	struct CellReselectionParametersCDMA2000_v920_SystemInformationBlockType8_cellReselectionParametersHRPD_v920_Optional cellReselectionParametersHRPD_v920;
	struct CellReselectionParametersCDMA2000_v920_SystemInformationBlockType8_cellReselectionParameters1XRTT_v920_Optional cellReselectionParameters1XRTT_v920;
	struct CSFB_RegistrationParam1XRTT_v920_SystemInformationBlockType8_csfb_RegistrationParam1XRTT_v920_Optional csfb_RegistrationParam1XRTT_v920;
	struct AC_BarringConfig1XRTT_r9_SystemInformationBlockType8_ac_BarringConfig1XRTT_r9_Optional ac_BarringConfig1XRTT_r9;
	struct SystemInformationBlockType8_csfb_DualRxTxSupport_r10_e_csfb_DualRxTxSupport_r10_Optional csfb_DualRxTxSupport_r10;
	struct SIB8_PerPLMN_List_r11_SystemInformationBlockType8_sib8_PerPLMN_List_r11_Optional sib8_PerPLMN_List_r11;
};

struct OCTET_STRING_SystemInformationBlockType9_hnb_Name_Optional {
	bool d;
	OCTET_STRING v;
};

struct OCTET_STRING_SystemInformationBlockType9_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType9 {
	struct OCTET_STRING_SystemInformationBlockType9_hnb_Name_Optional hnb_Name;
	struct OCTET_STRING_SystemInformationBlockType9_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

struct O50_SystemInformationBlockType10_dummy_Optional {
	bool d;
	O50 v;
};

struct OCTET_STRING_SystemInformationBlockType10_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType10 {
	B16 messageIdentifier;
	B16 serialNumber;
	O2 warningType;
	struct O50_SystemInformationBlockType10_dummy_Optional dummy;
	struct OCTET_STRING_SystemInformationBlockType10_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SystemInformationBlockType11_warningMessageSegmentType_e {
	SystemInformationBlockType11_warningMessageSegmentType_e_notLastSegment = 0,
	SystemInformationBlockType11_warningMessageSegmentType_e_lastSegment = 1,
};

typedef enum SystemInformationBlockType11_warningMessageSegmentType_e SystemInformationBlockType11_warningMessageSegmentType_e;

struct O1_SystemInformationBlockType11_dataCodingScheme_Optional {
	bool d;
	O1 v;
};

struct OCTET_STRING_SystemInformationBlockType11_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType11 {
	B16 messageIdentifier;
	B16 serialNumber;
	SystemInformationBlockType11_warningMessageSegmentType_e warningMessageSegmentType;
	uint8_t warningMessageSegmentNumber;
	OCTET_STRING warningMessageSegment;
	struct O1_SystemInformationBlockType11_dataCodingScheme_Optional dataCodingScheme;
	struct OCTET_STRING_SystemInformationBlockType11_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e {
	SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e_notLastSegment = 0,
	SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e_lastSegment = 1,
};

typedef enum SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e;

struct O1_SystemInformationBlockType12_r9_dataCodingScheme_r9_Optional {
	bool d;
	O1 v;
};

struct OCTET_STRING_SystemInformationBlockType12_r9_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct OCTET_STRING_SystemInformationBlockType12_r9_warningAreaCoordinatesSegment_r15_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType12_r9 {
	B16 messageIdentifier_r9;
	B16 serialNumber_r9;
	SystemInformationBlockType12_r9_warningMessageSegmentType_r9_e warningMessageSegmentType_r9;
	uint8_t warningMessageSegmentNumber_r9;
	OCTET_STRING warningMessageSegment_r9;
	struct O1_SystemInformationBlockType12_r9_dataCodingScheme_r9_Optional dataCodingScheme_r9;
	struct OCTET_STRING_SystemInformationBlockType12_r9_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct OCTET_STRING_SystemInformationBlockType12_r9_warningAreaCoordinatesSegment_r15_Optional warningAreaCoordinatesSegment_r15;
};

typedef uint8_t MBSFN_AreaId_r12;

enum MBSFN_AreaInfo_r9_non_MBSFNregionLength_e {
	MBSFN_AreaInfo_r9_non_MBSFNregionLength_e_s1 = 0,
	MBSFN_AreaInfo_r9_non_MBSFNregionLength_e_s2 = 1,
};

typedef enum MBSFN_AreaInfo_r9_non_MBSFNregionLength_e MBSFN_AreaInfo_r9_non_MBSFNregionLength_e;

enum MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e {
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e_rf32 = 0,
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e_rf64 = 1,
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e_rf128 = 2,
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e_rf256 = 3,
};

typedef enum MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e;

enum MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e {
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e_rf512 = 0,
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e_rf1024 = 1,
};

typedef enum MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e;

enum MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e {
	MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e_n2 = 0,
	MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e_n7 = 1,
	MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e_n13 = 2,
	MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e_n19 = 3,
};

typedef enum MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e;

struct MBSFN_AreaInfo_r9_mcch_Config_r9 {
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_RepetitionPeriod_r9_e mcch_RepetitionPeriod_r9;
	uint8_t mcch_Offset_r9;
	MBSFN_AreaInfo_r9_mcch_Config_r9_mcch_ModificationPeriod_r9_e mcch_ModificationPeriod_r9;
	B6 sf_AllocInfo_r9;
	MBSFN_AreaInfo_r9_mcch_Config_r9_signallingMCS_r9_e signallingMCS_r9;
};

enum MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e {
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_rf1 = 0,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_rf2 = 1,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_rf4 = 2,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_rf8 = 3,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_rf16 = 4,
};

typedef enum MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e;

enum MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e {
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf1 = 0,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf2 = 1,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf4 = 2,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf8 = 3,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf16 = 4,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf32 = 5,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf64 = 6,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf128 = 7,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_rf256 = 8,
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_spare7 = 9,
};

typedef enum MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e;

struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_mcch_RepetitionPeriod_v1430_Optional {
	bool d;
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e v;
};

struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_mcch_ModificationPeriod_v1430_Optional {
	bool d;
	MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e v;
};

struct MBSFN_AreaInfo_r9_mcch_Config_r14 {
	struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_RepetitionPeriod_v1430_e_mcch_RepetitionPeriod_v1430_Optional mcch_RepetitionPeriod_v1430;
	struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_ModificationPeriod_v1430_e_mcch_ModificationPeriod_v1430_Optional mcch_ModificationPeriod_v1430;
};

enum MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e {
	MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e_kHz7dot5 = 0,
	MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e_kHz1dot25 = 1,
};

typedef enum MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e;

struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_Config_r14_Optional {
	bool d;
	struct MBSFN_AreaInfo_r9_mcch_Config_r14 v;
};

struct MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e_subcarrierSpacingMBMS_r14_Optional {
	bool d;
	MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e v;
};

struct MBSFN_AreaInfo_r9 {
	MBSFN_AreaId_r12 mbsfn_AreaId_r9;
	MBSFN_AreaInfo_r9_non_MBSFNregionLength_e non_MBSFNregionLength;
	uint8_t notificationIndicator_r9;
	struct MBSFN_AreaInfo_r9_mcch_Config_r9 mcch_Config_r9;
	struct MBSFN_AreaInfo_r9_mcch_Config_r14_mcch_Config_r14_Optional mcch_Config_r14;
	struct MBSFN_AreaInfo_r9_subcarrierSpacingMBMS_r14_e_subcarrierSpacingMBMS_r14_Optional subcarrierSpacingMBMS_r14;
};

struct MBSFN_AreaInfo_r9_MBSFN_AreaInfoList_r9_Dynamic {
	size_t d;
	struct MBSFN_AreaInfo_r9* v;
};

typedef struct MBSFN_AreaInfo_r9_MBSFN_AreaInfoList_r9_Dynamic MBSFN_AreaInfoList_r9;

enum MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e {
	MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e_n2 = 0,
	MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e_n4 = 1,
};

typedef enum MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e;

struct MBMS_NotificationConfig_r9 {
	MBMS_NotificationConfig_r9_notificationRepetitionCoeff_r9_e notificationRepetitionCoeff_r9;
	uint8_t notificationOffset_r9;
	uint8_t notificationSF_Index_r9;
};

struct MBMS_NotificationConfig_v1430 {
	uint8_t notificationSF_Index_v1430;
};

enum MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e {
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf1 = 0,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf2 = 1,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf4 = 2,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf8 = 3,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf16 = 4,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf32 = 5,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf64 = 6,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf128 = 7,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_rf256 = 8,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare7 = 9,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare6 = 10,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare5 = 11,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare4 = 12,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare3 = 13,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare2 = 14,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e_spare1 = 15,
};

typedef enum MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e;

enum MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e {
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf1 = 0,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf2 = 1,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf4 = 2,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf8 = 3,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf16 = 4,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf32 = 5,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf64 = 6,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf128 = 7,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf256 = 8,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf512 = 9,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_rf1024 = 10,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_spare5 = 11,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_spare4 = 12,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_spare3 = 13,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_spare2 = 14,
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e_spare1 = 15,
};

typedef enum MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e;

enum MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e {
	MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e_n2 = 0,
	MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e_n7 = 1,
	MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e_n13 = 2,
	MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e_n19 = 3,
};

typedef enum MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e;

struct MBSFN_AreaInfo_r16_mcch_Config_r16 {
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_RepetitionPeriod_r16_e mcch_RepetitionPeriod_r16;
	MBSFN_AreaInfo_r16_mcch_Config_r16_mcch_ModificationPeriod_r16_e mcch_ModificationPeriod_r16;
	uint8_t mcch_Offset_r16;
	B10 sf_AllocInfo_r16;
	MBSFN_AreaInfo_r16_mcch_Config_r16_signallingMCS_r16_e signallingMCS_r16;
};

enum MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e {
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_kHz7dot5 = 0,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_kHz2dot5 = 1,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_kHz1dot25 = 2,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_kHz0dot37 = 3,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_spare4 = 4,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_spare3 = 5,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_spare2 = 6,
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e_spare1 = 7,
};

typedef enum MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e;

enum MBSFN_AreaInfo_r16_timeSeparation_r16_e {
	MBSFN_AreaInfo_r16_timeSeparation_r16_e_sl2 = 0,
	MBSFN_AreaInfo_r16_timeSeparation_r16_e_sl4 = 1,
};

typedef enum MBSFN_AreaInfo_r16_timeSeparation_r16_e MBSFN_AreaInfo_r16_timeSeparation_r16_e;

struct MBSFN_AreaInfo_r16_timeSeparation_r16_e_timeSeparation_r16_Optional {
	bool d;
	MBSFN_AreaInfo_r16_timeSeparation_r16_e v;
};

struct MBSFN_AreaInfo_r16 {
	MBSFN_AreaId_r12 mbsfn_AreaId_r16;
	uint8_t notificationIndicator_r16;
	struct MBSFN_AreaInfo_r16_mcch_Config_r16 mcch_Config_r16;
	MBSFN_AreaInfo_r16_subcarrierSpacingMBMS_r16_e subcarrierSpacingMBMS_r16;
	struct MBSFN_AreaInfo_r16_timeSeparation_r16_e_timeSeparation_r16_Optional timeSeparation_r16;
};

struct MBSFN_AreaInfo_r16_MBSFN_AreaInfoList_r16_Dynamic {
	size_t d;
	struct MBSFN_AreaInfo_r16* v;
};

typedef struct MBSFN_AreaInfo_r16_MBSFN_AreaInfoList_r16_Dynamic MBSFN_AreaInfoList_r16;

struct OCTET_STRING_SystemInformationBlockType13_r9_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct MBMS_NotificationConfig_v1430_SystemInformationBlockType13_r9_notificationConfig_v1430_Optional {
	bool d;
	struct MBMS_NotificationConfig_v1430 v;
};

struct MBSFN_AreaInfoList_r16_SystemInformationBlockType13_r9_mbsfn_AreaInfoList_r16_Optional {
	bool d;
	MBSFN_AreaInfoList_r16 v;
};

struct SystemInformationBlockType13_r9 {
	MBSFN_AreaInfoList_r9 mbsfn_AreaInfoList_r9;
	struct MBMS_NotificationConfig_r9 notificationConfig_r9;
	struct OCTET_STRING_SystemInformationBlockType13_r9_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct MBMS_NotificationConfig_v1430_SystemInformationBlockType13_r9_notificationConfig_v1430_Optional notificationConfig_v1430;
	struct MBSFN_AreaInfoList_r16_SystemInformationBlockType13_r9_mbsfn_AreaInfoList_r16_Optional mbsfn_AreaInfoList_r16;
};

enum EAB_Config_r11_eab_Category_r11_e {
	EAB_Config_r11_eab_Category_r11_e_a = 0,
	EAB_Config_r11_eab_Category_r11_e_b = 1,
	EAB_Config_r11_eab_Category_r11_e_c = 2,
};

typedef enum EAB_Config_r11_eab_Category_r11_e EAB_Config_r11_eab_Category_r11_e;

struct EAB_Config_r11 {
	EAB_Config_r11_eab_Category_r11_e eab_Category_r11;
	B10 eab_BarringBitmap_r11;
};

struct EAB_Config_r11_EAB_ConfigPLMN_r11_eab_Config_r11_Optional {
	bool d;
	struct EAB_Config_r11 v;
};

struct EAB_ConfigPLMN_r11 {
	struct EAB_Config_r11_EAB_ConfigPLMN_r11_eab_Config_r11_Optional eab_Config_r11;
};

enum SystemInformationBlockType14_r11_eab_Param_r11_Sel {
	SystemInformationBlockType14_r11_eab_Param_r11_UNBOUND_VALUE = 0,
	SystemInformationBlockType14_r11_eab_Param_r11_eab_Common_r11 = 1,
	SystemInformationBlockType14_r11_eab_Param_r11_eab_PerPLMN_List_r11 = 2,
};

struct EAB_ConfigPLMN_r11_SystemInformationBlockType14_r11_eab_Param_r11_eab_PerPLMN_List_r11_Dynamic {
	size_t d;
	struct EAB_ConfigPLMN_r11* v;
};

union SystemInformationBlockType14_r11_eab_Param_r11_Value {
	struct EAB_Config_r11 eab_Common_r11;
	struct EAB_ConfigPLMN_r11_SystemInformationBlockType14_r11_eab_Param_r11_eab_PerPLMN_List_r11_Dynamic eab_PerPLMN_List_r11;
};

struct SystemInformationBlockType14_r11_eab_Param_r11 {
	enum SystemInformationBlockType14_r11_eab_Param_r11_Sel d;
	union SystemInformationBlockType14_r11_eab_Param_r11_Value v;
};

enum SystemInformationBlockType14_r11_eab_PerRSRP_r15_e {
	SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_thresh0 = 0,
	SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_thresh1 = 1,
	SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_thresh2 = 2,
	SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_thresh3 = 3,
};

typedef enum SystemInformationBlockType14_r11_eab_PerRSRP_r15_e SystemInformationBlockType14_r11_eab_PerRSRP_r15_e;

struct SystemInformationBlockType14_r11_eab_Param_r11_eab_Param_r11_Optional {
	bool d;
	struct SystemInformationBlockType14_r11_eab_Param_r11 v;
};

struct OCTET_STRING_SystemInformationBlockType14_r11_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_eab_PerRSRP_r15_Optional {
	bool d;
	SystemInformationBlockType14_r11_eab_PerRSRP_r15_e v;
};

struct SystemInformationBlockType14_r11 {
	struct SystemInformationBlockType14_r11_eab_Param_r11_eab_Param_r11_Optional eab_Param_r11;
	struct OCTET_STRING_SystemInformationBlockType14_r11_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType14_r11_eab_PerRSRP_r15_e_eab_PerRSRP_r15_Optional eab_PerRSRP_r15;
};

typedef uint16_t MBMS_SAI_r11;

struct MBMS_SAI_r11_MBMS_SAI_List_r11_Dynamic {
	size_t d;
	MBMS_SAI_r11* v;
};

typedef struct MBMS_SAI_r11_MBMS_SAI_List_r11_Dynamic MBMS_SAI_List_r11;

struct MBMS_SAI_InterFreq_r11 {
	ARFCN_ValueEUTRA_r9 dl_CarrierFreq_r11;
	MBMS_SAI_List_r11 mbms_SAI_List_r11;
};

struct MBMS_SAI_InterFreq_r11_MBMS_SAI_InterFreqList_r11_Dynamic {
	size_t d;
	struct MBMS_SAI_InterFreq_r11* v;
};

typedef struct MBMS_SAI_InterFreq_r11_MBMS_SAI_InterFreqList_r11_Dynamic MBMS_SAI_InterFreqList_r11;

struct MultiBandInfoList_r11_MBMS_SAI_InterFreq_v1140_multiBandInfoList_r11_Optional {
	bool d;
	MultiBandInfoList_r11 v;
};

struct MBMS_SAI_InterFreq_v1140 {
	struct MultiBandInfoList_r11_MBMS_SAI_InterFreq_v1140_multiBandInfoList_r11_Optional multiBandInfoList_r11;
};

struct MBMS_SAI_InterFreq_v1140_MBMS_SAI_InterFreqList_v1140_Dynamic {
	size_t d;
	struct MBMS_SAI_InterFreq_v1140* v;
};

typedef struct MBMS_SAI_InterFreq_v1140_MBMS_SAI_InterFreqList_v1140_Dynamic MBMS_SAI_InterFreqList_v1140;

enum MBMS_CarrierType_r14_carrierType_r14_e {
	MBMS_CarrierType_r14_carrierType_r14_e_mbms = 0,
	MBMS_CarrierType_r14_carrierType_r14_e_fembmsMixed = 1,
	MBMS_CarrierType_r14_carrierType_r14_e_fembmsDedicated = 2,
};

typedef enum MBMS_CarrierType_r14_carrierType_r14_e MBMS_CarrierType_r14_carrierType_r14_e;

struct uint8_t_MBMS_CarrierType_r14_frameOffset_r14_Optional {
	bool d;
	uint8_t v;
};

struct MBMS_CarrierType_r14 {
	MBMS_CarrierType_r14_carrierType_r14_e carrierType_r14;
	struct uint8_t_MBMS_CarrierType_r14_frameOffset_r14_Optional frameOffset_r14;
};

struct MBMS_CarrierType_r14_MBMS_InterFreqCarrierTypeList_r14_Dynamic {
	size_t d;
	struct MBMS_CarrierType_r14* v;
};

typedef struct MBMS_CarrierType_r14_MBMS_InterFreqCarrierTypeList_r14_Dynamic MBMS_InterFreqCarrierTypeList_r14;

struct MBMS_SAI_List_r11_SystemInformationBlockType15_r11_mbms_SAI_IntraFreq_r11_Optional {
	bool d;
	MBMS_SAI_List_r11 v;
};

struct MBMS_SAI_InterFreqList_r11_SystemInformationBlockType15_r11_mbms_SAI_InterFreqList_r11_Optional {
	bool d;
	MBMS_SAI_InterFreqList_r11 v;
};

struct OCTET_STRING_SystemInformationBlockType15_r11_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct MBMS_SAI_InterFreqList_v1140_SystemInformationBlockType15_r11_mbms_SAI_InterFreqList_v1140_Optional {
	bool d;
	MBMS_SAI_InterFreqList_v1140 v;
};

struct MBMS_CarrierType_r14_SystemInformationBlockType15_r11_mbms_IntraFreqCarrierType_r14_Optional {
	bool d;
	struct MBMS_CarrierType_r14 v;
};

struct MBMS_InterFreqCarrierTypeList_r14_SystemInformationBlockType15_r11_mbms_InterFreqCarrierTypeList_r14_Optional {
	bool d;
	MBMS_InterFreqCarrierTypeList_r14 v;
};

struct SystemInformationBlockType15_r11 {
	struct MBMS_SAI_List_r11_SystemInformationBlockType15_r11_mbms_SAI_IntraFreq_r11_Optional mbms_SAI_IntraFreq_r11;
	struct MBMS_SAI_InterFreqList_r11_SystemInformationBlockType15_r11_mbms_SAI_InterFreqList_r11_Optional mbms_SAI_InterFreqList_r11;
	struct OCTET_STRING_SystemInformationBlockType15_r11_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct MBMS_SAI_InterFreqList_v1140_SystemInformationBlockType15_r11_mbms_SAI_InterFreqList_v1140_Optional mbms_SAI_InterFreqList_v1140;
	struct MBMS_CarrierType_r14_SystemInformationBlockType15_r11_mbms_IntraFreqCarrierType_r14_Optional mbms_IntraFreqCarrierType_r14;
	struct MBMS_InterFreqCarrierTypeList_r14_SystemInformationBlockType15_r11_mbms_InterFreqCarrierTypeList_r14_Optional mbms_InterFreqCarrierTypeList_r14;
};

struct B2_SystemInformationBlockType16_r11_timeInfo_r11_dayLightSavingTime_r11_Optional {
	bool d;
	B2 v;
};

struct int16_t_SystemInformationBlockType16_r11_timeInfo_r11_leapSeconds_r11_Optional {
	bool d;
	int16_t v;
};

struct int8_t_SystemInformationBlockType16_r11_timeInfo_r11_localTimeOffset_r11_Optional {
	bool d;
	int8_t v;
};

struct SystemInformationBlockType16_r11_timeInfo_r11 {
	uint64_t timeInfoUTC_r11;
	struct B2_SystemInformationBlockType16_r11_timeInfo_r11_dayLightSavingTime_r11_Optional dayLightSavingTime_r11;
	struct int16_t_SystemInformationBlockType16_r11_timeInfo_r11_leapSeconds_r11_Optional leapSeconds_r11;
	struct int8_t_SystemInformationBlockType16_r11_timeInfo_r11_localTimeOffset_r11_Optional localTimeOffset_r11;
};

struct ReferenceTime_r15 {
	uint32_t refDays_r15;
	uint32_t refSeconds_r15;
	uint16_t refMilliSeconds_r15;
	uint16_t refQuarterMicroSeconds_r15;
};

enum TimeReferenceInfo_r15_timeInfoType_r15_e {
	TimeReferenceInfo_r15_timeInfoType_r15_e_localClock = 0,
};

typedef enum TimeReferenceInfo_r15_timeInfoType_r15_e TimeReferenceInfo_r15_timeInfoType_r15_e;

struct uint8_t_TimeReferenceInfo_r15_uncertainty_r15_Optional {
	bool d;
	uint8_t v;
};

struct TimeReferenceInfo_r15_timeInfoType_r15_e_timeInfoType_r15_Optional {
	bool d;
	TimeReferenceInfo_r15_timeInfoType_r15_e v;
};

struct uint16_t_TimeReferenceInfo_r15_referenceSFN_r15_Optional {
	bool d;
	uint16_t v;
};

struct TimeReferenceInfo_r15 {
	struct ReferenceTime_r15 time_r15;
	struct uint8_t_TimeReferenceInfo_r15_uncertainty_r15_Optional uncertainty_r15;
	struct TimeReferenceInfo_r15_timeInfoType_r15_e_timeInfoType_r15_Optional timeInfoType_r15;
	struct uint16_t_TimeReferenceInfo_r15_referenceSFN_r15_Optional referenceSFN_r15;
};

struct SystemInformationBlockType16_r11_timeInfo_r11_timeInfo_r11_Optional {
	bool d;
	struct SystemInformationBlockType16_r11_timeInfo_r11 v;
};

struct OCTET_STRING_SystemInformationBlockType16_r11_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct TimeReferenceInfo_r15_SystemInformationBlockType16_r11_timeReferenceInfo_r15_Optional {
	bool d;
	struct TimeReferenceInfo_r15 v;
};

struct SystemInformationBlockType16_r11 {
	struct SystemInformationBlockType16_r11_timeInfo_r11_timeInfo_r11_Optional timeInfo_r11;
	struct OCTET_STRING_SystemInformationBlockType16_r11_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct TimeReferenceInfo_r15_SystemInformationBlockType16_r11_timeReferenceInfo_r15_Optional timeReferenceInfo_r15;
};

struct WLAN_OffloadConfig_r12_thresholdRSRP_r12 {
	RSRP_Range thresholdRSRP_Low_r12;
	RSRP_Range thresholdRSRP_High_r12;
};

typedef uint8_t RSRQ_Range;

struct WLAN_OffloadConfig_r12_thresholdRSRQ_r12 {
	RSRQ_Range thresholdRSRQ_Low_r12;
	RSRQ_Range thresholdRSRQ_High_r12;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12 {
	RSRQ_Range thresholdRSRQ_OnAllSymbolsWithWB_Low_r12;
	RSRQ_Range thresholdRSRQ_OnAllSymbolsWithWB_High_r12;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbols_r12 {
	RSRQ_Range thresholdRSRQ_OnAllSymbolsLow_r12;
	RSRQ_Range thresholdRSRQ_OnAllSymbolsHigh_r12;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_WB_r12 {
	RSRQ_Range thresholdRSRQ_WB_Low_r12;
	RSRQ_Range thresholdRSRQ_WB_High_r12;
};

struct WLAN_OffloadConfig_r12_thresholdChannelUtilization_r12 {
	uint8_t thresholdChannelUtilizationLow_r12;
	uint8_t thresholdChannelUtilizationHigh_r12;
};

enum WLAN_backhaulRate_r12_e {
	WLAN_backhaulRate_r12_e_r0 = 0,
	WLAN_backhaulRate_r12_e_r4 = 1,
	WLAN_backhaulRate_r12_e_r8 = 2,
	WLAN_backhaulRate_r12_e_r16 = 3,
	WLAN_backhaulRate_r12_e_r32 = 4,
	WLAN_backhaulRate_r12_e_r64 = 5,
	WLAN_backhaulRate_r12_e_r128 = 6,
	WLAN_backhaulRate_r12_e_r256 = 7,
	WLAN_backhaulRate_r12_e_r512 = 8,
	WLAN_backhaulRate_r12_e_r1024 = 9,
	WLAN_backhaulRate_r12_e_r2048 = 10,
	WLAN_backhaulRate_r12_e_r4096 = 11,
	WLAN_backhaulRate_r12_e_r8192 = 12,
	WLAN_backhaulRate_r12_e_r16384 = 13,
	WLAN_backhaulRate_r12_e_r32768 = 14,
	WLAN_backhaulRate_r12_e_r65536 = 15,
	WLAN_backhaulRate_r12_e_r131072 = 16,
	WLAN_backhaulRate_r12_e_r262144 = 17,
	WLAN_backhaulRate_r12_e_r524288 = 18,
	WLAN_backhaulRate_r12_e_r1048576 = 19,
	WLAN_backhaulRate_r12_e_r2097152 = 20,
	WLAN_backhaulRate_r12_e_r4194304 = 21,
	WLAN_backhaulRate_r12_e_r8388608 = 22,
	WLAN_backhaulRate_r12_e_r16777216 = 23,
	WLAN_backhaulRate_r12_e_r33554432 = 24,
	WLAN_backhaulRate_r12_e_r67108864 = 25,
	WLAN_backhaulRate_r12_e_r134217728 = 26,
	WLAN_backhaulRate_r12_e_r268435456 = 27,
	WLAN_backhaulRate_r12_e_r536870912 = 28,
	WLAN_backhaulRate_r12_e_r1073741824 = 29,
	WLAN_backhaulRate_r12_e_r2147483648 = 30,
	WLAN_backhaulRate_r12_e_r4294967296 = 31,
};

typedef enum WLAN_backhaulRate_r12_e WLAN_backhaulRate_r12_e;

struct WLAN_OffloadConfig_r12_thresholdBackhaul_Bandwidth_r12 {
	WLAN_backhaulRate_r12_e thresholdBackhaulDL_BandwidthLow_r12;
	WLAN_backhaulRate_r12_e thresholdBackhaulDL_BandwidthHigh_r12;
	WLAN_backhaulRate_r12_e thresholdBackhaulUL_BandwidthLow_r12;
	WLAN_backhaulRate_r12_e thresholdBackhaulUL_BandwidthHigh_r12;
};

struct WLAN_OffloadConfig_r12_thresholdWLAN_RSSI_r12 {
	uint8_t thresholdWLAN_RSSI_Low_r12;
	uint8_t thresholdWLAN_RSSI_High_r12;
};

struct WLAN_OffloadConfig_r12_thresholdRSRP_r12_thresholdRSRP_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdRSRP_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_r12_thresholdRSRQ_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbols_r12_thresholdRSRQ_OnAllSymbols_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbols_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdRSRQ_WB_r12_thresholdRSRQ_WB_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_WB_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdChannelUtilization_r12_thresholdChannelUtilization_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdChannelUtilization_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdBackhaul_Bandwidth_r12_thresholdBackhaul_Bandwidth_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdBackhaul_Bandwidth_r12 v;
};

struct WLAN_OffloadConfig_r12_thresholdWLAN_RSSI_r12_thresholdWLAN_RSSI_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12_thresholdWLAN_RSSI_r12 v;
};

struct B16_WLAN_OffloadConfig_r12_offloadPreferenceIndicator_r12_Optional {
	bool d;
	B16 v;
};

struct T_Reselection_WLAN_OffloadConfig_r12_t_SteeringWLAN_r12_Optional {
	bool d;
	T_Reselection v;
};

struct WLAN_OffloadConfig_r12 {
	struct WLAN_OffloadConfig_r12_thresholdRSRP_r12_thresholdRSRP_r12_Optional thresholdRSRP_r12;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_r12_thresholdRSRQ_r12_Optional thresholdRSRQ_r12;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12_thresholdRSRQ_OnAllSymbolsWithWB_r12_Optional thresholdRSRQ_OnAllSymbolsWithWB_r12;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_OnAllSymbols_r12_thresholdRSRQ_OnAllSymbols_r12_Optional thresholdRSRQ_OnAllSymbols_r12;
	struct WLAN_OffloadConfig_r12_thresholdRSRQ_WB_r12_thresholdRSRQ_WB_r12_Optional thresholdRSRQ_WB_r12;
	struct WLAN_OffloadConfig_r12_thresholdChannelUtilization_r12_thresholdChannelUtilization_r12_Optional thresholdChannelUtilization_r12;
	struct WLAN_OffloadConfig_r12_thresholdBackhaul_Bandwidth_r12_thresholdBackhaul_Bandwidth_r12_Optional thresholdBackhaul_Bandwidth_r12;
	struct WLAN_OffloadConfig_r12_thresholdWLAN_RSSI_r12_thresholdWLAN_RSSI_r12_Optional thresholdWLAN_RSSI_r12;
	struct B16_WLAN_OffloadConfig_r12_offloadPreferenceIndicator_r12_Optional offloadPreferenceIndicator_r12;
	struct T_Reselection_WLAN_OffloadConfig_r12_t_SteeringWLAN_r12_Optional t_SteeringWLAN_r12;
};

struct OCTET_STRING_WLAN_Identifiers_r12_ssid_r12_Optional {
	bool d;
	OCTET_STRING v;
};

struct O6_WLAN_Identifiers_r12_bssid_r12_Optional {
	bool d;
	O6 v;
};

struct O6_WLAN_Identifiers_r12_hessid_r12_Optional {
	bool d;
	O6 v;
};

struct WLAN_Identifiers_r12 {
	struct OCTET_STRING_WLAN_Identifiers_r12_ssid_r12_Optional ssid_r12;
	struct O6_WLAN_Identifiers_r12_bssid_r12_Optional bssid_r12;
	struct O6_WLAN_Identifiers_r12_hessid_r12_Optional hessid_r12;
};

struct WLAN_Identifiers_r12_WLAN_Id_List_r12_Dynamic {
	size_t d;
	struct WLAN_Identifiers_r12* v;
};

typedef struct WLAN_Identifiers_r12_WLAN_Id_List_r12_Dynamic WLAN_Id_List_r12;

struct WLAN_OffloadConfig_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_OffloadConfigCommon_r12_Optional {
	bool d;
	struct WLAN_OffloadConfig_r12 v;
};

struct WLAN_Id_List_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_Id_List_r12_Optional {
	bool d;
	WLAN_Id_List_r12 v;
};

struct WLAN_OffloadInfoPerPLMN_r12 {
	struct WLAN_OffloadConfig_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_OffloadConfigCommon_r12_Optional wlan_OffloadConfigCommon_r12;
	struct WLAN_Id_List_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_Id_List_r12_Optional wlan_Id_List_r12;
};

struct SystemInformationBlockType17_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_OffloadInfoPerPLMN_List_r12_Dynamic {
	size_t d;
	struct WLAN_OffloadInfoPerPLMN_r12* v;
};

struct WLAN_OffloadInfoPerPLMN_r12_SystemInformationBlockType17_r12_wlan_OffloadInfoPerPLMN_List_r12_DynamicOptional {
	bool d;
	struct SystemInformationBlockType17_r12_WLAN_OffloadInfoPerPLMN_r12_wlan_OffloadInfoPerPLMN_List_r12_Dynamic v;
};

struct OCTET_STRING_SystemInformationBlockType17_r12_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType17_r12 {
	struct WLAN_OffloadInfoPerPLMN_r12_SystemInformationBlockType17_r12_wlan_OffloadInfoPerPLMN_List_r12_DynamicOptional wlan_OffloadInfoPerPLMN_List_r12;
	struct OCTET_STRING_SystemInformationBlockType17_r12_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SL_CP_Len_r12_e {
	SL_CP_Len_r12_e_normal = 0,
	SL_CP_Len_r12_e_extended = 1,
};

typedef enum SL_CP_Len_r12_e SL_CP_Len_r12_e;

enum SL_PeriodComm_r12_e {
	SL_PeriodComm_r12_e_sf40 = 0,
	SL_PeriodComm_r12_e_sf60 = 1,
	SL_PeriodComm_r12_e_sf70 = 2,
	SL_PeriodComm_r12_e_sf80 = 3,
	SL_PeriodComm_r12_e_sf120 = 4,
	SL_PeriodComm_r12_e_sf140 = 5,
	SL_PeriodComm_r12_e_sf160 = 6,
	SL_PeriodComm_r12_e_sf240 = 7,
	SL_PeriodComm_r12_e_sf280 = 8,
	SL_PeriodComm_r12_e_sf320 = 9,
	SL_PeriodComm_r12_e_spare6 = 10,
	SL_PeriodComm_r12_e_spare5 = 11,
	SL_PeriodComm_r12_e_spare4 = 12,
	SL_PeriodComm_r12_e_spare3 = 13,
	SL_PeriodComm_r12_e_spare2 = 14,
	SL_PeriodComm_r12_e_spare = 15,
};

typedef enum SL_PeriodComm_r12_e SL_PeriodComm_r12_e;

enum SL_OffsetIndicator_r12_Sel {
	SL_OffsetIndicator_r12_UNBOUND_VALUE = 0,
	SL_OffsetIndicator_r12_small_r12 = 1,
	SL_OffsetIndicator_r12_large_r12 = 2,
};

union SL_OffsetIndicator_r12_Value {
	uint16_t small_r12;
	uint16_t large_r12;
};

struct SL_OffsetIndicator_r12 {
	enum SL_OffsetIndicator_r12_Sel d;
	union SL_OffsetIndicator_r12_Value v;
};

enum SubframeBitmapSL_r12_Sel {
	SubframeBitmapSL_r12_UNBOUND_VALUE = 0,
	SubframeBitmapSL_r12_bs4_r12 = 1,
	SubframeBitmapSL_r12_bs8_r12 = 2,
	SubframeBitmapSL_r12_bs12_r12 = 3,
	SubframeBitmapSL_r12_bs16_r12 = 4,
	SubframeBitmapSL_r12_bs30_r12 = 5,
	SubframeBitmapSL_r12_bs40_r12 = 6,
	SubframeBitmapSL_r12_bs42_r12 = 7,
};

union SubframeBitmapSL_r12_Value {
	B4 bs4_r12;
	B8 bs8_r12;
	B12 bs12_r12;
	B16 bs16_r12;
	B30 bs30_r12;
	B40 bs40_r12;
	B42 bs42_r12;
};

struct SubframeBitmapSL_r12 {
	enum SubframeBitmapSL_r12_Sel d;
	union SubframeBitmapSL_r12_Value v;
};

struct SL_TF_ResourceConfig_r12 {
	uint8_t prb_Num_r12;
	uint8_t prb_Start_r12;
	uint8_t prb_End_r12;
	struct SL_OffsetIndicator_r12 offsetIndicator_r12;
	struct SubframeBitmapSL_r12 subframeBitmap_r12;
};

enum SL_HoppingConfigComm_r12_numSubbands_r12_e {
	SL_HoppingConfigComm_r12_numSubbands_r12_e_ns1 = 0,
	SL_HoppingConfigComm_r12_numSubbands_r12_e_ns2 = 1,
	SL_HoppingConfigComm_r12_numSubbands_r12_e_ns4 = 2,
};

typedef enum SL_HoppingConfigComm_r12_numSubbands_r12_e SL_HoppingConfigComm_r12_numSubbands_r12_e;

struct SL_HoppingConfigComm_r12 {
	uint16_t hoppingParameter_r12;
	SL_HoppingConfigComm_r12_numSubbands_r12_e numSubbands_r12;
	uint8_t rb_Offset_r12;
};

typedef BIT_STRING SL_TRPT_Subset_r12;

struct SL_TRPT_Subset_r12_SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12_trpt_Subset_r12_Optional {
	bool d;
	SL_TRPT_Subset_r12 v;
};

struct SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12 {
	struct SL_TF_ResourceConfig_r12 data_TF_ResourceConfig_r12;
	struct SL_TRPT_Subset_r12_SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12_trpt_Subset_r12_Optional trpt_Subset_r12;
};

enum TDD_Config_subframeAssignment_e {
	TDD_Config_subframeAssignment_e_sa0 = 0,
	TDD_Config_subframeAssignment_e_sa1 = 1,
	TDD_Config_subframeAssignment_e_sa2 = 2,
	TDD_Config_subframeAssignment_e_sa3 = 3,
	TDD_Config_subframeAssignment_e_sa4 = 4,
	TDD_Config_subframeAssignment_e_sa5 = 5,
	TDD_Config_subframeAssignment_e_sa6 = 6,
};

typedef enum TDD_Config_subframeAssignment_e TDD_Config_subframeAssignment_e;

enum TDD_Config_specialSubframePatterns_e {
	TDD_Config_specialSubframePatterns_e_ssp0 = 0,
	TDD_Config_specialSubframePatterns_e_ssp1 = 1,
	TDD_Config_specialSubframePatterns_e_ssp2 = 2,
	TDD_Config_specialSubframePatterns_e_ssp3 = 3,
	TDD_Config_specialSubframePatterns_e_ssp4 = 4,
	TDD_Config_specialSubframePatterns_e_ssp5 = 5,
	TDD_Config_specialSubframePatterns_e_ssp6 = 6,
	TDD_Config_specialSubframePatterns_e_ssp7 = 7,
	TDD_Config_specialSubframePatterns_e_ssp8 = 8,
};

typedef enum TDD_Config_specialSubframePatterns_e TDD_Config_specialSubframePatterns_e;

struct TDD_Config {
	TDD_Config_subframeAssignment_e subframeAssignment;
	TDD_Config_specialSubframePatterns_e specialSubframePatterns;
};

struct TDD_Config_SL_CommResourcePool_r12_rxParametersNCell_r12_tdd_Config_r12_Optional {
	bool d;
	struct TDD_Config v;
};

struct SL_CommResourcePool_r12_rxParametersNCell_r12 {
	struct TDD_Config_SL_CommResourcePool_r12_rxParametersNCell_r12_tdd_Config_r12_Optional tdd_Config_r12;
	uint8_t syncConfigIndex_r12;
};

typedef int8_t P0_SL_r12;

struct SL_TxParameters_r12 {
	Alpha_r12_e alpha_r12;
	P0_SL_r12 p0_r12;
};

struct SL_CommResourcePool_r12_txParameters_r12 {
	struct SL_TxParameters_r12 sc_TxParameters_r12;
	struct SL_TxParameters_r12 dataTxParameters_r12;
};

typedef uint8_t SL_Priority_r13;

struct SL_Priority_r13_SL_PriorityList_r13_Dynamic {
	size_t d;
	SL_Priority_r13* v;
};

typedef struct SL_Priority_r13_SL_PriorityList_r13_Dynamic SL_PriorityList_r13;

struct SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12_ue_SelectedResourceConfig_r12_Optional {
	bool d;
	struct SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12 v;
};

struct SL_CommResourcePool_r12_rxParametersNCell_r12_rxParametersNCell_r12_Optional {
	bool d;
	struct SL_CommResourcePool_r12_rxParametersNCell_r12 v;
};

struct SL_CommResourcePool_r12_txParameters_r12_txParameters_r12_Optional {
	bool d;
	struct SL_CommResourcePool_r12_txParameters_r12 v;
};

struct SL_PriorityList_r13_SL_CommResourcePool_r12_priorityList_r13_Optional {
	bool d;
	SL_PriorityList_r13 v;
};

struct SL_CommResourcePool_r12 {
	SL_CP_Len_r12_e sc_CP_Len_r12;
	SL_PeriodComm_r12_e sc_Period_r12;
	struct SL_TF_ResourceConfig_r12 sc_TF_ResourceConfig_r12;
	SL_CP_Len_r12_e data_CP_Len_r12;
	struct SL_HoppingConfigComm_r12 dataHoppingConfig_r12;
	struct SL_CommResourcePool_r12_ue_SelectedResourceConfig_r12_ue_SelectedResourceConfig_r12_Optional ue_SelectedResourceConfig_r12;
	struct SL_CommResourcePool_r12_rxParametersNCell_r12_rxParametersNCell_r12_Optional rxParametersNCell_r12;
	struct SL_CommResourcePool_r12_txParameters_r12_txParameters_r12_Optional txParameters_r12;
	struct SL_PriorityList_r13_SL_CommResourcePool_r12_priorityList_r13_Optional priorityList_r13;
};

struct SL_CommResourcePool_r12_SL_CommRxPoolList_r12_Dynamic {
	size_t d;
	struct SL_CommResourcePool_r12* v;
};

typedef struct SL_CommResourcePool_r12_SL_CommRxPoolList_r12_Dynamic SL_CommRxPoolList_r12;

struct SL_CommResourcePool_r12_SL_CommTxPoolList_r12_Dynamic {
	size_t d;
	struct SL_CommResourcePool_r12* v;
};

typedef struct SL_CommResourcePool_r12_SL_CommTxPoolList_r12_Dynamic SL_CommTxPoolList_r12;

typedef uint8_t SL_OffsetIndicatorSync_r12;

typedef uint8_t SLSSID_r12;

typedef uint8_t RSRP_RangeSL_r12;

struct B19_SL_SyncConfig_r12_txParameters_r12_syncInfoReserved_r12_Optional {
	bool d;
	B19 v;
};

struct SL_SyncConfig_r12_txParameters_r12 {
	struct SL_TxParameters_r12 syncTxParameters_r12;
	RSRP_RangeSL_r12 syncTxThreshIC_r12;
	struct B19_SL_SyncConfig_r12_txParameters_r12_syncInfoReserved_r12_Optional syncInfoReserved_r12;
};

enum SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e {
	SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e_w1 = 0,
	SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e_w2 = 1,
};

typedef enum SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e;

struct SL_SyncConfig_r12_rxParamsNCell_r12 {
	PhysCellId physCellId_r12;
	SL_SyncConfig_r12_rxParamsNCell_r12_discSyncWindow_r12_e discSyncWindow_r12;
};

enum SL_SyncConfig_r12_syncTxPeriodic_r13_e {
	SL_SyncConfig_r12_syncTxPeriodic_r13_e_true = 0,
};

typedef enum SL_SyncConfig_r12_syncTxPeriodic_r13_e SL_SyncConfig_r12_syncTxPeriodic_r13_e;

typedef uint8_t SL_OffsetIndicatorSync_v1430;

enum SL_SyncConfig_r12_gnss_Sync_r14_e {
	SL_SyncConfig_r12_gnss_Sync_r14_e_true = 0,
};

typedef enum SL_SyncConfig_r12_gnss_Sync_r14_e SL_SyncConfig_r12_gnss_Sync_r14_e;

typedef uint8_t SL_OffsetIndicatorSync_r14;

enum SL_SyncConfig_r12_slss_TxDisabled_r15_e {
	SL_SyncConfig_r12_slss_TxDisabled_r15_e_true = 0,
};

typedef enum SL_SyncConfig_r12_slss_TxDisabled_r15_e SL_SyncConfig_r12_slss_TxDisabled_r15_e;

struct SL_SyncConfig_r12_txParameters_r12_txParameters_r12_Optional {
	bool d;
	struct SL_SyncConfig_r12_txParameters_r12 v;
};

struct SL_SyncConfig_r12_rxParamsNCell_r12_rxParamsNCell_r12_Optional {
	bool d;
	struct SL_SyncConfig_r12_rxParamsNCell_r12 v;
};

struct SL_SyncConfig_r12_syncTxPeriodic_r13_e_syncTxPeriodic_r13_Optional {
	bool d;
	SL_SyncConfig_r12_syncTxPeriodic_r13_e v;
};

struct SL_OffsetIndicatorSync_v1430_SL_SyncConfig_r12_syncOffsetIndicator_v1430_Optional {
	bool d;
	SL_OffsetIndicatorSync_v1430 v;
};

struct SL_SyncConfig_r12_gnss_Sync_r14_e_gnss_Sync_r14_Optional {
	bool d;
	SL_SyncConfig_r12_gnss_Sync_r14_e v;
};

struct SL_OffsetIndicatorSync_r14_SL_SyncConfig_r12_syncOffsetIndicator2_r14_Optional {
	bool d;
	SL_OffsetIndicatorSync_r14 v;
};

struct SL_OffsetIndicatorSync_r14_SL_SyncConfig_r12_syncOffsetIndicator3_r14_Optional {
	bool d;
	SL_OffsetIndicatorSync_r14 v;
};

struct SL_SyncConfig_r12_slss_TxDisabled_r15_e_slss_TxDisabled_r15_Optional {
	bool d;
	SL_SyncConfig_r12_slss_TxDisabled_r15_e v;
};

struct SL_SyncConfig_r12 {
	SL_CP_Len_r12_e syncCP_Len_r12;
	SL_OffsetIndicatorSync_r12 syncOffsetIndicator_r12;
	SLSSID_r12 slssid_r12;
	struct SL_SyncConfig_r12_txParameters_r12_txParameters_r12_Optional txParameters_r12;
	struct SL_SyncConfig_r12_rxParamsNCell_r12_rxParamsNCell_r12_Optional rxParamsNCell_r12;
	struct SL_SyncConfig_r12_syncTxPeriodic_r13_e_syncTxPeriodic_r13_Optional syncTxPeriodic_r13;
	struct SL_OffsetIndicatorSync_v1430_SL_SyncConfig_r12_syncOffsetIndicator_v1430_Optional syncOffsetIndicator_v1430;
	struct SL_SyncConfig_r12_gnss_Sync_r14_e_gnss_Sync_r14_Optional gnss_Sync_r14;
	struct SL_OffsetIndicatorSync_r14_SL_SyncConfig_r12_syncOffsetIndicator2_r14_Optional syncOffsetIndicator2_r14;
	struct SL_OffsetIndicatorSync_r14_SL_SyncConfig_r12_syncOffsetIndicator3_r14_Optional syncOffsetIndicator3_r14;
	struct SL_SyncConfig_r12_slss_TxDisabled_r15_e_slss_TxDisabled_r15_Optional slss_TxDisabled_r15;
};

struct SL_SyncConfig_r12_SL_SyncConfigList_r12_Dynamic {
	size_t d;
	struct SL_SyncConfig_r12* v;
};

typedef struct SL_SyncConfig_r12_SL_SyncConfigList_r12_Dynamic SL_SyncConfigList_r12;

struct SL_CommTxPoolList_r12_SystemInformationBlockType18_r12_commConfig_r12_commTxPoolNormalCommon_r12_Optional {
	bool d;
	SL_CommTxPoolList_r12 v;
};

struct SL_CommTxPoolList_r12_SystemInformationBlockType18_r12_commConfig_r12_commTxPoolExceptional_r12_Optional {
	bool d;
	SL_CommTxPoolList_r12 v;
};

struct SL_SyncConfigList_r12_SystemInformationBlockType18_r12_commConfig_r12_commSyncConfig_r12_Optional {
	bool d;
	SL_SyncConfigList_r12 v;
};

struct SystemInformationBlockType18_r12_commConfig_r12 {
	SL_CommRxPoolList_r12 commRxPool_r12;
	struct SL_CommTxPoolList_r12_SystemInformationBlockType18_r12_commConfig_r12_commTxPoolNormalCommon_r12_Optional commTxPoolNormalCommon_r12;
	struct SL_CommTxPoolList_r12_SystemInformationBlockType18_r12_commConfig_r12_commTxPoolExceptional_r12_Optional commTxPoolExceptional_r12;
	struct SL_SyncConfigList_r12_SystemInformationBlockType18_r12_commConfig_r12_commSyncConfig_r12_Optional commSyncConfig_r12;
};

struct SL_CommResourcePool_r12_SL_CommTxPoolListExt_r13_Dynamic {
	size_t d;
	struct SL_CommResourcePool_r12* v;
};

typedef struct SL_CommResourcePool_r12_SL_CommTxPoolListExt_r13_Dynamic SL_CommTxPoolListExt_r13;

enum SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e {
	SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e_true = 0,
};

typedef enum SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e;

enum SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e {
	SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e_true = 0,
};

typedef enum SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e;

struct SystemInformationBlockType18_r12_commConfig_r12_commConfig_r12_Optional {
	bool d;
	struct SystemInformationBlockType18_r12_commConfig_r12 v;
};

struct OCTET_STRING_SystemInformationBlockType18_r12_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SL_CommTxPoolListExt_r13_SystemInformationBlockType18_r12_commTxPoolNormalCommonExt_r13_Optional {
	bool d;
	SL_CommTxPoolListExt_r13 v;
};

struct SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e_commTxResourceUC_ReqAllowed_r13_Optional {
	bool d;
	SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e v;
};

struct SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e_commTxAllowRelayCommon_r13_Optional {
	bool d;
	SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e v;
};

struct SystemInformationBlockType18_r12 {
	struct SystemInformationBlockType18_r12_commConfig_r12_commConfig_r12_Optional commConfig_r12;
	struct OCTET_STRING_SystemInformationBlockType18_r12_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SL_CommTxPoolListExt_r13_SystemInformationBlockType18_r12_commTxPoolNormalCommonExt_r13_Optional commTxPoolNormalCommonExt_r13;
	struct SystemInformationBlockType18_r12_commTxResourceUC_ReqAllowed_r13_e_commTxResourceUC_ReqAllowed_r13_Optional commTxResourceUC_ReqAllowed_r13;
	struct SystemInformationBlockType18_r12_commTxAllowRelayCommon_r13_e_commTxAllowRelayCommon_r13_Optional commTxAllowRelayCommon_r13;
};

enum SL_DiscResourcePool_r12_discPeriod_r12_e {
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf32 = 0,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf64 = 1,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf128 = 2,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf256 = 3,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf512 = 4,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf1024 = 5,
	SL_DiscResourcePool_r12_discPeriod_r12_e_rf16_v1310 = 6,
	SL_DiscResourcePool_r12_discPeriod_r12_e_spare = 7,
};

typedef enum SL_DiscResourcePool_r12_discPeriod_r12_e SL_DiscResourcePool_r12_discPeriod_r12_e;

typedef uint8_t RSRP_RangeSL2_r12;

struct SL_PoolSelectionConfig_r12 {
	RSRP_RangeSL2_r12 threshLow_r12;
	RSRP_RangeSL2_r12 threshHigh_r12;
};

enum SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_Sel {
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_UNBOUND_VALUE = 0,
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_rsrpBased_r12 = 1,
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_random_r12 = 2,
};

union SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_Value {
	struct SL_PoolSelectionConfig_r12 rsrpBased_r12;
	Null random_r12;
};

struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12 {
	enum SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_Sel d;
	union SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12_Value v;
};

enum SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e {
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e_p25 = 0,
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e_p50 = 1,
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e_p75 = 2,
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e_p100 = 3,
};

typedef enum SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e;

struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12 {
	struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_poolSelection_r12 poolSelection_r12;
	SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_txProbability_r12_e txProbability_r12;
};

struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_ue_SelectedResourceConfig_r12_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12 v;
};

struct SL_DiscResourcePool_r12_txParameters_r12 {
	struct SL_TxParameters_r12 txParametersGeneral_r12;
	struct SL_DiscResourcePool_r12_txParameters_r12_ue_SelectedResourceConfig_r12_ue_SelectedResourceConfig_r12_Optional ue_SelectedResourceConfig_r12;
};

struct TDD_Config_SL_DiscResourcePool_r12_rxParameters_r12_tdd_Config_r12_Optional {
	bool d;
	struct TDD_Config v;
};

struct SL_DiscResourcePool_r12_rxParameters_r12 {
	struct TDD_Config_SL_DiscResourcePool_r12_rxParameters_r12_tdd_Config_r12_Optional tdd_Config_r12;
	uint8_t syncConfigIndex_r12;
};

enum SL_DiscResourcePool_r12_discPeriod_v1310_setup_e {
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf4 = 0,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf6 = 1,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf7 = 2,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf8 = 3,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf12 = 4,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf14 = 5,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf24 = 6,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e_rf28 = 7,
};

typedef enum SL_DiscResourcePool_r12_discPeriod_v1310_setup_e SL_DiscResourcePool_r12_discPeriod_v1310_setup_e;

enum SL_DiscResourcePool_r12_discPeriod_v1310_Sel {
	SL_DiscResourcePool_r12_discPeriod_v1310_UNBOUND_VALUE = 0,
	SL_DiscResourcePool_r12_discPeriod_v1310_release = 1,
	SL_DiscResourcePool_r12_discPeriod_v1310_setup = 2,
};

union SL_DiscResourcePool_r12_discPeriod_v1310_Value {
	Null release;
	SL_DiscResourcePool_r12_discPeriod_v1310_setup_e setup;
};

struct SL_DiscResourcePool_r12_discPeriod_v1310 {
	enum SL_DiscResourcePool_r12_discPeriod_v1310_Sel d;
	union SL_DiscResourcePool_r12_discPeriod_v1310_Value v;
};

struct PhysCellId_PhysCellIdList_r13_Dynamic {
	size_t d;
	PhysCellId* v;
};

typedef struct PhysCellId_PhysCellIdList_r13_Dynamic PhysCellIdList_r13;

struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_setup {
	PhysCellIdList_r13 physCellId_r13;
};

enum SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_Sel {
	SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_UNBOUND_VALUE = 0,
	SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_release = 1,
	SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_setup = 2,
};

union SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_Value {
	Null release;
	struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_setup setup;
};

struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13 {
	enum SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_Sel d;
	union SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_Value v;
};

enum TDD_Config_v1130_specialSubframePatterns_v1130_e {
	TDD_Config_v1130_specialSubframePatterns_v1130_e_ssp7 = 0,
	TDD_Config_v1130_specialSubframePatterns_v1130_e_ssp9 = 1,
};

typedef enum TDD_Config_v1130_specialSubframePatterns_v1130_e TDD_Config_v1130_specialSubframePatterns_v1130_e;

struct TDD_Config_v1130 {
	TDD_Config_v1130_specialSubframePatterns_v1130_e specialSubframePatterns_v1130;
};

enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e {
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n6 = 0,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n15 = 1,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n25 = 2,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n50 = 3,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n75 = 4,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_n100 = 5,
};

typedef enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e;

struct ARFCN_ValueEUTRA_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_CarrierFreq_Optional {
	bool d;
	ARFCN_ValueEUTRA v;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_ul_Bandwidth_Optional {
	bool d;
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e v;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo {
	struct ARFCN_ValueEUTRA_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_CarrierFreq_Optional ul_CarrierFreq;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo_ul_Bandwidth_e_ul_Bandwidth_Optional ul_Bandwidth;
	AdditionalSpectrumEmission additionalSpectrumEmission;
};

struct P_Max_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_p_Max_Optional {
	bool d;
	P_Max v;
};

struct TDD_Config_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_tdd_Config_r13_Optional {
	bool d;
	struct TDD_Config v;
};

struct TDD_Config_v1130_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_tdd_Config_v1130_Optional {
	bool d;
	struct TDD_Config_v1130 v;
};

struct uint8_t_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_syncConfigIndex_r13_Optional {
	bool d;
	uint8_t v;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup {
	PhysCellIdList_r13 physCellId_r13;
	struct P_Max_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_p_Max_Optional p_Max;
	struct TDD_Config_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_tdd_Config_r13_Optional tdd_Config_r13;
	struct TDD_Config_v1130_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_tdd_Config_v1130_Optional tdd_Config_v1130;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_freqInfo freqInfo;
	int8_t referenceSignalPower;
	struct uint8_t_SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup_syncConfigIndex_r13_Optional syncConfigIndex_r13;
};

enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_Sel {
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_UNBOUND_VALUE = 0,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_release = 1,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup = 2,
};

union SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_Value {
	Null release;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_setup setup;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13 {
	enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_Sel d;
	union SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_Value v;
};

typedef uint16_t AdditionalSpectrumEmission_v10l0;

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_setup_freqInfo_v1370 {
	AdditionalSpectrumEmission_v10l0 additionalSpectrumEmission_v1370;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_setup {
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_setup_freqInfo_v1370 freqInfo_v1370;
};

enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_Sel {
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_UNBOUND_VALUE = 0,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_release = 1,
	SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_setup = 2,
};

union SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_Value {
	Null release;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_setup setup;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370 {
	enum SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_Sel d;
	union SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_Value v;
};

struct SL_DiscResourcePool_r12_txParameters_r12_txParameters_r12_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_txParameters_r12 v;
};

struct SL_DiscResourcePool_r12_rxParameters_r12_rxParameters_r12_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_rxParameters_r12 v;
};

struct SL_DiscResourcePool_r12_discPeriod_v1310_discPeriod_v1310_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_discPeriod_v1310 v;
};

struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_rxParamsAddNeighFreq_r13_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13 v;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_txParamsAddNeighFreq_r13_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13 v;
};

struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_txParamsAddNeighFreq_v1370_Optional {
	bool d;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370 v;
};

struct SL_DiscResourcePool_r12 {
	SL_CP_Len_r12_e cp_Len_r12;
	SL_DiscResourcePool_r12_discPeriod_r12_e discPeriod_r12;
	uint8_t numRetx_r12;
	uint8_t numRepetition_r12;
	struct SL_TF_ResourceConfig_r12 tf_ResourceConfig_r12;
	struct SL_DiscResourcePool_r12_txParameters_r12_txParameters_r12_Optional txParameters_r12;
	struct SL_DiscResourcePool_r12_rxParameters_r12_rxParameters_r12_Optional rxParameters_r12;
	struct SL_DiscResourcePool_r12_discPeriod_v1310_discPeriod_v1310_Optional discPeriod_v1310;
	struct SL_DiscResourcePool_r12_rxParamsAddNeighFreq_r13_rxParamsAddNeighFreq_r13_Optional rxParamsAddNeighFreq_r13;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_r13_txParamsAddNeighFreq_r13_Optional txParamsAddNeighFreq_r13;
	struct SL_DiscResourcePool_r12_txParamsAddNeighFreq_v1370_txParamsAddNeighFreq_v1370_Optional txParamsAddNeighFreq_v1370;
};

struct SL_DiscResourcePool_r12_SL_DiscRxPoolList_r12_Dynamic {
	size_t d;
	struct SL_DiscResourcePool_r12* v;
};

typedef struct SL_DiscResourcePool_r12_SL_DiscRxPoolList_r12_Dynamic SL_DiscRxPoolList_r12;

struct SL_DiscResourcePool_r12_SL_DiscTxPoolList_r12_Dynamic {
	size_t d;
	struct SL_DiscResourcePool_r12* v;
};

typedef struct SL_DiscResourcePool_r12_SL_DiscTxPoolList_r12_Dynamic SL_DiscTxPoolList_r12;

struct SL_DiscTxPowerInfo_r12 {
	P_Max discMaxTxPower_r12;
};

typedef struct SL_DiscTxPowerInfo_r12 SL_DiscTxPowerInfoList_r12[3];

struct SL_DiscTxPoolList_r12_SystemInformationBlockType19_r12_discConfig_r12_discTxPoolCommon_r12_Optional {
	bool d;
	SL_DiscTxPoolList_r12 v;
};

struct SL_DiscTxPowerInfoList_r12_SystemInformationBlockType19_r12_discConfig_r12_discTxPowerInfo_r12_Optional {
	bool d;
	SL_DiscTxPowerInfoList_r12 v;
};

struct SL_SyncConfigList_r12_SystemInformationBlockType19_r12_discConfig_r12_discSyncConfig_r12_Optional {
	bool d;
	SL_SyncConfigList_r12 v;
};

struct SystemInformationBlockType19_r12_discConfig_r12 {
	SL_DiscRxPoolList_r12 discRxPool_r12;
	struct SL_DiscTxPoolList_r12_SystemInformationBlockType19_r12_discConfig_r12_discTxPoolCommon_r12_Optional discTxPoolCommon_r12;
	struct SL_DiscTxPowerInfoList_r12_SystemInformationBlockType19_r12_discConfig_r12_discTxPowerInfo_r12_Optional discTxPowerInfo_r12;
	struct SL_SyncConfigList_r12_SystemInformationBlockType19_r12_discConfig_r12_discSyncConfig_r12_Optional discSyncConfig_r12;
};

typedef uint8_t MCC_MNC_Digit;

typedef MCC_MNC_Digit MCC[3];

struct MCC_MNC_Digit_MNC_Dynamic {
	size_t d;
	MCC_MNC_Digit* v;
};

typedef struct MCC_MNC_Digit_MNC_Dynamic MNC;

struct MCC_PLMN_Identity_mcc_Optional {
	bool d;
	MCC v;
};

struct PLMN_Identity {
	struct MCC_PLMN_Identity_mcc_Optional mcc;
	MNC mnc;
};

enum PLMN_IdentityInfo2_r12_Sel {
	PLMN_IdentityInfo2_r12_UNBOUND_VALUE = 0,
	PLMN_IdentityInfo2_r12_plmn_Index_r12 = 1,
	PLMN_IdentityInfo2_r12_plmnIdentity_r12 = 2,
};

union PLMN_IdentityInfo2_r12_Value {
	uint8_t plmn_Index_r12;
	struct PLMN_Identity plmnIdentity_r12;
};

struct PLMN_IdentityInfo2_r12 {
	enum PLMN_IdentityInfo2_r12_Sel d;
	union PLMN_IdentityInfo2_r12_Value v;
};

struct PLMN_IdentityInfo2_r12_PLMN_IdentityList4_r12_Dynamic {
	size_t d;
	struct PLMN_IdentityInfo2_r12* v;
};

typedef struct PLMN_IdentityInfo2_r12_PLMN_IdentityList4_r12_Dynamic PLMN_IdentityList4_r12;

struct PLMN_IdentityList4_r12_SL_CarrierFreqInfo_r12_plmn_IdentityList_r12_Optional {
	bool d;
	PLMN_IdentityList4_r12 v;
};

struct SL_CarrierFreqInfo_r12 {
	ARFCN_ValueEUTRA_r9 carrierFreq_r12;
	struct PLMN_IdentityList4_r12_SL_CarrierFreqInfo_r12_plmn_IdentityList_r12_Optional plmn_IdentityList_r12;
};

struct SL_CarrierFreqInfo_r12_SL_CarrierFreqInfoList_r12_Dynamic {
	size_t d;
	struct SL_CarrierFreqInfo_r12* v;
};

typedef struct SL_CarrierFreqInfo_r12_SL_CarrierFreqInfoList_r12_Dynamic SL_CarrierFreqInfoList_r12;

enum SL_DiscTxResourcesInterFreq_r13_Sel {
	SL_DiscTxResourcesInterFreq_r13_UNBOUND_VALUE = 0,
	SL_DiscTxResourcesInterFreq_r13_acquireSI_FromCarrier_r13 = 1,
	SL_DiscTxResourcesInterFreq_r13_discTxPoolCommon_r13 = 2,
	SL_DiscTxResourcesInterFreq_r13_requestDedicated_r13 = 3,
	SL_DiscTxResourcesInterFreq_r13_noTxOnCarrier_r13 = 4,
};

union SL_DiscTxResourcesInterFreq_r13_Value {
	Null acquireSI_FromCarrier_r13;
	SL_DiscTxPoolList_r12 discTxPoolCommon_r13;
	Null requestDedicated_r13;
	Null noTxOnCarrier_r13;
};

struct SL_DiscTxResourcesInterFreq_r13 {
	enum SL_DiscTxResourcesInterFreq_r13_Sel d;
	union SL_DiscTxResourcesInterFreq_r13_Value v;
};

struct SL_DiscRxPoolList_r12_SL_ResourcesInterFreq_r13_discRxResourcesInterFreq_r13_Optional {
	bool d;
	SL_DiscRxPoolList_r12 v;
};

struct SL_DiscTxResourcesInterFreq_r13_SL_ResourcesInterFreq_r13_discTxResourcesInterFreq_r13_Optional {
	bool d;
	struct SL_DiscTxResourcesInterFreq_r13 v;
};

struct SL_ResourcesInterFreq_r13 {
	struct SL_DiscRxPoolList_r12_SL_ResourcesInterFreq_r13_discRxResourcesInterFreq_r13_Optional discRxResourcesInterFreq_r13;
	struct SL_DiscTxResourcesInterFreq_r13_SL_ResourcesInterFreq_r13_discTxResourcesInterFreq_r13_Optional discTxResourcesInterFreq_r13;
};

enum SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e {
	SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e_pCell = 0,
};

typedef enum SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e;

struct SL_SyncConfigNFreq_r13_asyncParameters_r13 {
	SL_CP_Len_r12_e syncCP_Len_r13;
	SL_OffsetIndicatorSync_r12 syncOffsetIndicator_r13;
	SLSSID_r12 slssid_r13;
};

enum SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e {
	SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e_true = 0,
};

typedef enum SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e;

struct B19_SL_SyncConfigNFreq_r13_txParameters_r13_syncInfoReserved_r13_Optional {
	bool d;
	B19 v;
};

struct SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e_syncTxPeriodic_r13_Optional {
	bool d;
	SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e v;
};

struct SL_SyncConfigNFreq_r13_txParameters_r13 {
	struct SL_TxParameters_r12 syncTxParameters_r13;
	RSRP_RangeSL_r12 syncTxThreshIC_r13;
	struct B19_SL_SyncConfigNFreq_r13_txParameters_r13_syncInfoReserved_r13_Optional syncInfoReserved_r13;
	struct SL_SyncConfigNFreq_r13_txParameters_r13_syncTxPeriodic_r13_e_syncTxPeriodic_r13_Optional syncTxPeriodic_r13;
};

enum SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e {
	SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e_w1 = 0,
	SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e_w2 = 1,
};

typedef enum SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e;

struct SL_SyncConfigNFreq_r13_rxParameters_r13 {
	SL_SyncConfigNFreq_r13_rxParameters_r13_discSyncWindow_r13_e discSyncWindow_r13;
};

enum SL_SyncConfigNFreq_r13_gnss_Sync_r14_e {
	SL_SyncConfigNFreq_r13_gnss_Sync_r14_e_true = 0,
};

typedef enum SL_SyncConfigNFreq_r13_gnss_Sync_r14_e SL_SyncConfigNFreq_r13_gnss_Sync_r14_e;

enum SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e {
	SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e_true = 0,
};

typedef enum SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e;

struct SL_SyncConfigNFreq_r13_asyncParameters_r13_asyncParameters_r13_Optional {
	bool d;
	struct SL_SyncConfigNFreq_r13_asyncParameters_r13 v;
};

struct SL_SyncConfigNFreq_r13_txParameters_r13_txParameters_r13_Optional {
	bool d;
	struct SL_SyncConfigNFreq_r13_txParameters_r13 v;
};

struct SL_SyncConfigNFreq_r13_rxParameters_r13_rxParameters_r13_Optional {
	bool d;
	struct SL_SyncConfigNFreq_r13_rxParameters_r13 v;
};

struct SL_OffsetIndicatorSync_v1430_SL_SyncConfigNFreq_r13_syncOffsetIndicator_v1430_Optional {
	bool d;
	SL_OffsetIndicatorSync_v1430 v;
};

struct SL_SyncConfigNFreq_r13_gnss_Sync_r14_e_gnss_Sync_r14_Optional {
	bool d;
	SL_SyncConfigNFreq_r13_gnss_Sync_r14_e v;
};

struct SL_OffsetIndicatorSync_r14_SL_SyncConfigNFreq_r13_syncOffsetIndicator2_r14_Optional {
	bool d;
	SL_OffsetIndicatorSync_r14 v;
};

struct SL_OffsetIndicatorSync_r14_SL_SyncConfigNFreq_r13_syncOffsetIndicator3_r14_Optional {
	bool d;
	SL_OffsetIndicatorSync_r14 v;
};

struct SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e_slss_TxDisabled_r15_Optional {
	bool d;
	SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e v;
};

struct SL_SyncConfigNFreq_r13 {
	struct SL_SyncConfigNFreq_r13_asyncParameters_r13_asyncParameters_r13_Optional asyncParameters_r13;
	struct SL_SyncConfigNFreq_r13_txParameters_r13_txParameters_r13_Optional txParameters_r13;
	struct SL_SyncConfigNFreq_r13_rxParameters_r13_rxParameters_r13_Optional rxParameters_r13;
	struct SL_OffsetIndicatorSync_v1430_SL_SyncConfigNFreq_r13_syncOffsetIndicator_v1430_Optional syncOffsetIndicator_v1430;
	struct SL_SyncConfigNFreq_r13_gnss_Sync_r14_e_gnss_Sync_r14_Optional gnss_Sync_r14;
	struct SL_OffsetIndicatorSync_r14_SL_SyncConfigNFreq_r13_syncOffsetIndicator2_r14_Optional syncOffsetIndicator2_r14;
	struct SL_OffsetIndicatorSync_r14_SL_SyncConfigNFreq_r13_syncOffsetIndicator3_r14_Optional syncOffsetIndicator3_r14;
	struct SL_SyncConfigNFreq_r13_slss_TxDisabled_r15_e_slss_TxDisabled_r15_Optional slss_TxDisabled_r15;
};

struct SL_SyncConfigNFreq_r13_SL_SyncConfigListNFreq_r13_Dynamic {
	size_t d;
	struct SL_SyncConfigNFreq_r13* v;
};

typedef struct SL_SyncConfigNFreq_r13_SL_SyncConfigListNFreq_r13_Dynamic SL_SyncConfigListNFreq_r13;

enum CellSelectionInfoNFreq_r13_q_Hyst_r13_e {
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB0 = 0,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB1 = 1,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB2 = 2,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB3 = 3,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB4 = 4,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB5 = 5,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB6 = 6,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB8 = 7,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB10 = 8,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB12 = 9,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB14 = 10,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB16 = 11,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB18 = 12,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB20 = 13,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB22 = 14,
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e_dB24 = 15,
};

typedef enum CellSelectionInfoNFreq_r13_q_Hyst_r13_e CellSelectionInfoNFreq_r13_q_Hyst_r13_e;

struct uint8_t_CellSelectionInfoNFreq_r13_q_RxLevMinOffset_Optional {
	bool d;
	uint8_t v;
};

struct CellSelectionInfoNFreq_r13 {
	Q_RxLevMin q_RxLevMin_r13;
	struct uint8_t_CellSelectionInfoNFreq_r13_q_RxLevMinOffset_Optional q_RxLevMinOffset;
	CellSelectionInfoNFreq_r13_q_Hyst_r13_e q_Hyst_r13;
	Q_RxLevMin q_RxLevMinReselection_r13;
	T_Reselection t_ReselectionEUTRA_r13;
};

struct SL_DiscTxPowerInfoList_r12_SL_DiscConfigOtherInterFreq_r13_txPowerInfo_r13_Optional {
	bool d;
	SL_DiscTxPowerInfoList_r12 v;
};

struct SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e_refCarrierCommon_r13_Optional {
	bool d;
	SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e v;
};

struct SL_SyncConfigListNFreq_r13_SL_DiscConfigOtherInterFreq_r13_discSyncConfig_r13_Optional {
	bool d;
	SL_SyncConfigListNFreq_r13 v;
};

struct CellSelectionInfoNFreq_r13_SL_DiscConfigOtherInterFreq_r13_discCellSelectionInfo_r13_Optional {
	bool d;
	struct CellSelectionInfoNFreq_r13 v;
};

struct SL_DiscConfigOtherInterFreq_r13 {
	struct SL_DiscTxPowerInfoList_r12_SL_DiscConfigOtherInterFreq_r13_txPowerInfo_r13_Optional txPowerInfo_r13;
	struct SL_DiscConfigOtherInterFreq_r13_refCarrierCommon_r13_e_refCarrierCommon_r13_Optional refCarrierCommon_r13;
	struct SL_SyncConfigListNFreq_r13_SL_DiscConfigOtherInterFreq_r13_discSyncConfig_r13_Optional discSyncConfig_r13;
	struct CellSelectionInfoNFreq_r13_SL_DiscConfigOtherInterFreq_r13_discCellSelectionInfo_r13_Optional discCellSelectionInfo_r13;
};

struct SL_ResourcesInterFreq_r13_SL_CarrierFreqInfo_v1310_discResourcesNonPS_r13_Optional {
	bool d;
	struct SL_ResourcesInterFreq_r13 v;
};

struct SL_ResourcesInterFreq_r13_SL_CarrierFreqInfo_v1310_discResourcesPS_r13_Optional {
	bool d;
	struct SL_ResourcesInterFreq_r13 v;
};

struct SL_DiscConfigOtherInterFreq_r13_SL_CarrierFreqInfo_v1310_discConfigOther_r13_Optional {
	bool d;
	struct SL_DiscConfigOtherInterFreq_r13 v;
};

struct SL_CarrierFreqInfo_v1310 {
	struct SL_ResourcesInterFreq_r13_SL_CarrierFreqInfo_v1310_discResourcesNonPS_r13_Optional discResourcesNonPS_r13;
	struct SL_ResourcesInterFreq_r13_SL_CarrierFreqInfo_v1310_discResourcesPS_r13_Optional discResourcesPS_r13;
	struct SL_DiscConfigOtherInterFreq_r13_SL_CarrierFreqInfo_v1310_discConfigOther_r13_Optional discConfigOther_r13;
};

struct SL_CarrierFreqInfo_v1310_SL_CarrierFreqInfoList_v1310_Dynamic {
	size_t d;
	struct SL_CarrierFreqInfo_v1310* v;
};

typedef struct SL_CarrierFreqInfo_v1310_SL_CarrierFreqInfoList_v1310_Dynamic SL_CarrierFreqInfoList_v1310;

enum SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e {
	SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e_true = 0,
};

typedef enum SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e;

struct SL_CarrierFreqInfoList_v1310_SystemInformationBlockType19_r12_discConfig_v1310_discInterFreqList_v1310_Optional {
	bool d;
	SL_CarrierFreqInfoList_v1310 v;
};

struct SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e_gapRequestsAllowedCommon_Optional {
	bool d;
	SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e v;
};

struct SystemInformationBlockType19_r12_discConfig_v1310 {
	struct SL_CarrierFreqInfoList_v1310_SystemInformationBlockType19_r12_discConfig_v1310_discInterFreqList_v1310_Optional discInterFreqList_v1310;
	struct SystemInformationBlockType19_r12_discConfig_v1310_gapRequestsAllowedCommon_e_gapRequestsAllowedCommon_Optional gapRequestsAllowedCommon;
};

typedef uint8_t RSRP_RangeSL4_r13;

enum SL_DiscConfigRelayUE_r13_hystMax_r13_e {
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dB0 = 0,
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dB3 = 1,
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dB6 = 2,
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dB9 = 3,
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dB12 = 4,
	SL_DiscConfigRelayUE_r13_hystMax_r13_e_dBinf = 5,
};

typedef enum SL_DiscConfigRelayUE_r13_hystMax_r13_e SL_DiscConfigRelayUE_r13_hystMax_r13_e;

enum SL_DiscConfigRelayUE_r13_hystMin_r13_e {
	SL_DiscConfigRelayUE_r13_hystMin_r13_e_dB0 = 0,
	SL_DiscConfigRelayUE_r13_hystMin_r13_e_dB3 = 1,
	SL_DiscConfigRelayUE_r13_hystMin_r13_e_dB6 = 2,
	SL_DiscConfigRelayUE_r13_hystMin_r13_e_dB9 = 3,
	SL_DiscConfigRelayUE_r13_hystMin_r13_e_dB12 = 4,
};

typedef enum SL_DiscConfigRelayUE_r13_hystMin_r13_e SL_DiscConfigRelayUE_r13_hystMin_r13_e;

struct RSRP_RangeSL4_r13_SL_DiscConfigRelayUE_r13_threshHigh_r13_Optional {
	bool d;
	RSRP_RangeSL4_r13 v;
};

struct RSRP_RangeSL4_r13_SL_DiscConfigRelayUE_r13_threshLow_r13_Optional {
	bool d;
	RSRP_RangeSL4_r13 v;
};

struct SL_DiscConfigRelayUE_r13_hystMax_r13_e_hystMax_r13_Optional {
	bool d;
	SL_DiscConfigRelayUE_r13_hystMax_r13_e v;
};

struct SL_DiscConfigRelayUE_r13_hystMin_r13_e_hystMin_r13_Optional {
	bool d;
	SL_DiscConfigRelayUE_r13_hystMin_r13_e v;
};

struct SL_DiscConfigRelayUE_r13 {
	struct RSRP_RangeSL4_r13_SL_DiscConfigRelayUE_r13_threshHigh_r13_Optional threshHigh_r13;
	struct RSRP_RangeSL4_r13_SL_DiscConfigRelayUE_r13_threshLow_r13_Optional threshLow_r13;
	struct SL_DiscConfigRelayUE_r13_hystMax_r13_e_hystMax_r13_Optional hystMax_r13;
	struct SL_DiscConfigRelayUE_r13_hystMin_r13_e_hystMin_r13_Optional hystMin_r13;
};

enum SL_DiscConfigRemoteUE_r13_hystMax_r13_e {
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e_dB0 = 0,
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e_dB3 = 1,
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e_dB6 = 2,
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e_dB9 = 3,
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e_dB12 = 4,
};

typedef enum SL_DiscConfigRemoteUE_r13_hystMax_r13_e SL_DiscConfigRemoteUE_r13_hystMax_r13_e;

enum FilterCoefficient_e {
	FilterCoefficient_e_fc0 = 0,
	FilterCoefficient_e_fc1 = 1,
	FilterCoefficient_e_fc2 = 2,
	FilterCoefficient_e_fc3 = 3,
	FilterCoefficient_e_fc4 = 4,
	FilterCoefficient_e_fc5 = 5,
	FilterCoefficient_e_fc6 = 6,
	FilterCoefficient_e_fc7 = 7,
	FilterCoefficient_e_fc8 = 8,
	FilterCoefficient_e_fc9 = 9,
	FilterCoefficient_e_fc11 = 10,
	FilterCoefficient_e_fc13 = 11,
	FilterCoefficient_e_fc15 = 12,
	FilterCoefficient_e_fc17 = 13,
	FilterCoefficient_e_fc19 = 14,
	FilterCoefficient_e_spare1 = 15,
};

typedef enum FilterCoefficient_e FilterCoefficient_e;

enum ReselectionInfoRelay_r13_minHyst_r13_e {
	ReselectionInfoRelay_r13_minHyst_r13_e_dB0 = 0,
	ReselectionInfoRelay_r13_minHyst_r13_e_dB3 = 1,
	ReselectionInfoRelay_r13_minHyst_r13_e_dB6 = 2,
	ReselectionInfoRelay_r13_minHyst_r13_e_dB9 = 3,
	ReselectionInfoRelay_r13_minHyst_r13_e_dB12 = 4,
	ReselectionInfoRelay_r13_minHyst_r13_e_dBinf = 5,
};

typedef enum ReselectionInfoRelay_r13_minHyst_r13_e ReselectionInfoRelay_r13_minHyst_r13_e;

struct ReselectionInfoRelay_r13_minHyst_r13_e_minHyst_r13_Optional {
	bool d;
	ReselectionInfoRelay_r13_minHyst_r13_e v;
};

struct ReselectionInfoRelay_r13 {
	Q_RxLevMin q_RxLevMin_r13;
	FilterCoefficient_e filterCoefficient_r13;
	struct ReselectionInfoRelay_r13_minHyst_r13_e_minHyst_r13_Optional minHyst_r13;
};

struct RSRP_RangeSL4_r13_SL_DiscConfigRemoteUE_r13_threshHigh_r13_Optional {
	bool d;
	RSRP_RangeSL4_r13 v;
};

struct SL_DiscConfigRemoteUE_r13_hystMax_r13_e_hystMax_r13_Optional {
	bool d;
	SL_DiscConfigRemoteUE_r13_hystMax_r13_e v;
};

struct SL_DiscConfigRemoteUE_r13 {
	struct RSRP_RangeSL4_r13_SL_DiscConfigRemoteUE_r13_threshHigh_r13_Optional threshHigh_r13;
	struct SL_DiscConfigRemoteUE_r13_hystMax_r13_e_hystMax_r13_Optional hystMax_r13;
	struct ReselectionInfoRelay_r13 reselectionInfoIC_r13;
};

struct SystemInformationBlockType19_r12_discConfigRelay_r13 {
	struct SL_DiscConfigRelayUE_r13 relayUE_Config_r13;
	struct SL_DiscConfigRemoteUE_r13 remoteUE_Config_r13;
};

struct SL_DiscTxPoolList_r12_SystemInformationBlockType19_r12_discConfigPS_13_discTxPoolPS_Common_r13_Optional {
	bool d;
	SL_DiscTxPoolList_r12 v;
};

struct SystemInformationBlockType19_r12_discConfigPS_13 {
	SL_DiscRxPoolList_r12 discRxPoolPS_r13;
	struct SL_DiscTxPoolList_r12_SystemInformationBlockType19_r12_discConfigPS_13_discTxPoolPS_Common_r13_Optional discTxPoolPS_Common_r13;
};

struct SystemInformationBlockType19_r12_discConfig_r12_discConfig_r12_Optional {
	bool d;
	struct SystemInformationBlockType19_r12_discConfig_r12 v;
};

struct SL_CarrierFreqInfoList_r12_SystemInformationBlockType19_r12_discInterFreqList_r12_Optional {
	bool d;
	SL_CarrierFreqInfoList_r12 v;
};

struct OCTET_STRING_SystemInformationBlockType19_r12_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType19_r12_discConfig_v1310_discConfig_v1310_Optional {
	bool d;
	struct SystemInformationBlockType19_r12_discConfig_v1310 v;
};

struct SystemInformationBlockType19_r12_discConfigRelay_r13_discConfigRelay_r13_Optional {
	bool d;
	struct SystemInformationBlockType19_r12_discConfigRelay_r13 v;
};

struct SystemInformationBlockType19_r12_discConfigPS_13_discConfigPS_13_Optional {
	bool d;
	struct SystemInformationBlockType19_r12_discConfigPS_13 v;
};

struct SystemInformationBlockType19_r12 {
	struct SystemInformationBlockType19_r12_discConfig_r12_discConfig_r12_Optional discConfig_r12;
	struct SL_CarrierFreqInfoList_r12_SystemInformationBlockType19_r12_discInterFreqList_r12_Optional discInterFreqList_r12;
	struct OCTET_STRING_SystemInformationBlockType19_r12_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType19_r12_discConfig_v1310_discConfig_v1310_Optional discConfig_v1310;
	struct SystemInformationBlockType19_r12_discConfigRelay_r13_discConfigRelay_r13_Optional discConfigRelay_r13;
	struct SystemInformationBlockType19_r12_discConfigPS_13_discConfigPS_13_Optional discConfigPS_13;
};

enum SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e {
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf2 = 0,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf4 = 1,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf8 = 2,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf16 = 3,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf32 = 4,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf64 = 5,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf128 = 6,
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e_rf256 = 7,
};

typedef enum SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e;

enum SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e {
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf2 = 0,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf4 = 1,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf8 = 2,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf16 = 3,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf32 = 4,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf64 = 5,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf128 = 6,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf256 = 7,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf512 = 8,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf1024 = 9,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_r2048 = 10,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf4096 = 11,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf8192 = 12,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf16384 = 13,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf32768 = 14,
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e_rf65536 = 15,
};

typedef enum SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy_e_rf1 = 0,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy2_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy2_e_rf1 = 0,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy2_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy2_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r1 = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r2 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r4 = 2,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r8 = 3,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r16 = 4,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r32 = 5,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r64 = 6,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r128 = 7,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e_r256 = 8,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v1 = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v1dot5 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v2 = 2,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v2dot5 = 3,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v4 = 4,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v5 = 5,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v8 = 6,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e_v10 = 7,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v1 = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v2 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v4 = 2,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v5 = 3,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v8 = 4,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v10 = 5,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e_v20 = 6,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_Sel {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_UNBOUND_VALUE = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14 = 2,
};

union SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_Value {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_fdd_r14_e fdd_r14;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_tdd_r14_e tdd_r14;
};

struct SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14 {
	enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_Sel d;
	union SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14_Value v;
};

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e_off = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e_ce_ModeA = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e_ce_ModeB = 2,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf32 = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf128 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf512 = 2,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf1024 = 3,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf2048 = 4,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf4096 = 5,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf8192 = 6,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e_rf16384 = 7,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e;

enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf32 = 0,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf128 = 1,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf256 = 2,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf512 = 3,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf1024 = 4,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf2048 = 5,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf4096 = 6,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf8192 = 7,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf16384 = 8,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf32768 = 9,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf65536 = 10,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf131072 = 11,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf262144 = 12,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf524288 = 13,
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e_rf1048576 = 14,
};

typedef enum SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e;

struct SystemInformationBlockType20_r13_br_BCCH_Config_r14 {
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy_e dummy;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_dummy2_e dummy2;
	uint8_t mpdcch_Narrowband_SC_MCCH_r14;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_NumRepetition_SC_MCCH_r14_e mpdcch_NumRepetition_SC_MCCH_r14;
	struct SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_StartSF_SC_MCCH_r14 mpdcch_StartSF_SC_MCCH_r14;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14_e mpdcch_PDSCH_HoppingConfig_SC_MCCH_r14;
	ARFCN_ValueEUTRA_r9 sc_mcch_CarrierFreq_r14;
	uint8_t sc_mcch_Offset_BR_r14;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_RepetitionPeriod_BR_r14_e sc_mcch_RepetitionPeriod_BR_r14;
	SystemInformationBlockType20_r13_br_BCCH_Config_r14_sc_mcch_ModificationPeriod_BR_r14_e sc_mcch_ModificationPeriod_BR_r14;
};

enum SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e {
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf10 = 0,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf20 = 1,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf100 = 2,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf300 = 3,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf500 = 4,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf1000 = 5,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf1200 = 6,
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e_psf1600 = 7,
};

typedef enum SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e;

enum SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e {
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf0 = 0,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf1 = 1,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf2 = 2,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf4 = 3,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf8 = 4,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf16 = 5,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf32 = 6,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf64 = 7,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf128 = 8,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf256 = 9,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_ps512 = 10,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf1024 = 11,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf2048 = 12,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf4096 = 13,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf8192 = 14,
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e_psf16384 = 15,
};

typedef enum SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e;

enum SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_Sel {
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_UNBOUND_VALUE = 0,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf10 = 1,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf20 = 2,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf32 = 3,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf40 = 4,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf64 = 5,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf80 = 6,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf128 = 7,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf160 = 8,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf256 = 9,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf320 = 10,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf512 = 11,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf640 = 12,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf1024 = 13,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf2048 = 14,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf4096 = 15,
	SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_sf8192 = 16,
};

union SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_Value {
	uint8_t sf10;
	uint8_t sf20;
	uint8_t sf32;
	uint8_t sf40;
	uint8_t sf64;
	uint8_t sf80;
	uint8_t sf128;
	uint8_t sf160;
	uint8_t sf256;
	uint16_t sf320;
	uint16_t sf512;
	uint16_t sf640;
	uint16_t sf1024;
	uint16_t sf2048;
	uint16_t sf4096;
	uint16_t sf8192;
};

struct SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14 {
	enum SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_Sel d;
	union SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14_Value v;
};

struct SC_MCCH_SchedulingInfo_r14 {
	SC_MCCH_SchedulingInfo_r14_onDurationTimerSCPTM_r14_e onDurationTimerSCPTM_r14;
	SC_MCCH_SchedulingInfo_r14_drx_InactivityTimerSCPTM_r14_e drx_InactivityTimerSCPTM_r14;
	struct SC_MCCH_SchedulingInfo_r14_schedulingPeriodStartOffsetSCPTM_r14 schedulingPeriodStartOffsetSCPTM_r14;
};

enum SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e {
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e_r16 = 0,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e_r32 = 1,
};

typedef enum SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e;

enum SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e {
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r192 = 0,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r256 = 1,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r384 = 2,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r512 = 3,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r768 = 4,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r1024 = 5,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r1536 = 6,
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_r2048 = 7,
};

typedef enum SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e;

enum SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e {
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e_rf1 = 0,
};

typedef enum SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e;

enum SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e {
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e_rf1 = 0,
};

typedef enum SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e;

struct uint8_t_SystemInformationBlockType20_r13_sc_mcch_duration_r13_Optional {
	bool d;
	uint8_t v;
};

struct OCTET_STRING_SystemInformationBlockType20_r13_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType20_r13_br_BCCH_Config_r14_br_BCCH_Config_r14_Optional {
	bool d;
	struct SystemInformationBlockType20_r13_br_BCCH_Config_r14 v;
};

struct SC_MCCH_SchedulingInfo_r14_SystemInformationBlockType20_r13_sc_mcch_SchedulingInfo_r14_Optional {
	bool d;
	struct SC_MCCH_SchedulingInfo_r14 v;
};

struct SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_Optional {
	bool d;
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e v;
};

struct SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_Optional {
	bool d;
	SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e v;
};

struct SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e_sc_mcch_RepetitionPeriod_v1470_Optional {
	bool d;
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e v;
};

struct SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e_sc_mcch_ModificationPeriod_v1470_Optional {
	bool d;
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e v;
};

struct SystemInformationBlockType20_r13 {
	SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_r13_e sc_mcch_RepetitionPeriod_r13;
	uint8_t sc_mcch_Offset_r13;
	uint8_t sc_mcch_FirstSubframe_r13;
	struct uint8_t_SystemInformationBlockType20_r13_sc_mcch_duration_r13_Optional sc_mcch_duration_r13;
	SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_r13_e sc_mcch_ModificationPeriod_r13;
	struct OCTET_STRING_SystemInformationBlockType20_r13_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType20_r13_br_BCCH_Config_r14_br_BCCH_Config_r14_Optional br_BCCH_Config_r14;
	struct SC_MCCH_SchedulingInfo_r14_SystemInformationBlockType20_r13_sc_mcch_SchedulingInfo_r14_Optional sc_mcch_SchedulingInfo_r14;
	struct SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_e_pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14_Optional pdsch_maxNumRepetitionCEmodeA_SC_MTCH_r14;
	struct SystemInformationBlockType20_r13_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_e_pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14_Optional pdsch_maxNumRepetitionCEmodeB_SC_MTCH_r14;
	struct SystemInformationBlockType20_r13_sc_mcch_RepetitionPeriod_v1470_e_sc_mcch_RepetitionPeriod_v1470_Optional sc_mcch_RepetitionPeriod_v1470;
	struct SystemInformationBlockType20_r13_sc_mcch_ModificationPeriod_v1470_e_sc_mcch_ModificationPeriod_v1470_Optional sc_mcch_ModificationPeriod_v1470;
};

enum SubframeBitmapSL_r14_Sel {
	SubframeBitmapSL_r14_UNBOUND_VALUE = 0,
	SubframeBitmapSL_r14_bs10_r14 = 1,
	SubframeBitmapSL_r14_bs16_r14 = 2,
	SubframeBitmapSL_r14_bs20_r14 = 3,
	SubframeBitmapSL_r14_bs30_r14 = 4,
	SubframeBitmapSL_r14_bs40_r14 = 5,
	SubframeBitmapSL_r14_bs50_r14 = 6,
	SubframeBitmapSL_r14_bs60_r14 = 7,
	SubframeBitmapSL_r14_bs100_r14 = 8,
};

union SubframeBitmapSL_r14_Value {
	B10 bs10_r14;
	B16 bs16_r14;
	B20 bs20_r14;
	B30 bs30_r14;
	B40 bs40_r14;
	B50 bs50_r14;
	B60 bs60_r14;
	B100 bs100_r14;
};

struct SubframeBitmapSL_r14 {
	enum SubframeBitmapSL_r14_Sel d;
	union SubframeBitmapSL_r14_Value v;
};

enum SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e {
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n4 = 0,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n5 = 1,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n6 = 2,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n8 = 3,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n9 = 4,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n10 = 5,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n12 = 6,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n15 = 7,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n16 = 8,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n18 = 9,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n20 = 10,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n25 = 11,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n30 = 12,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n48 = 13,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n50 = 14,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n72 = 15,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n75 = 16,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n96 = 17,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_n100 = 18,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare13 = 19,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare12 = 20,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare11 = 21,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare10 = 22,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare9 = 23,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare8 = 24,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare7 = 25,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare6 = 26,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare5 = 27,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare4 = 28,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare3 = 29,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare2 = 30,
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e_spare1 = 31,
};

typedef enum SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e;

enum SL_CommResourcePoolV2X_r14_numSubchannel_r14_e {
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n1 = 0,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n3 = 1,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n5 = 2,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n8 = 3,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n10 = 4,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n15 = 5,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_n20 = 6,
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e_spare1 = 7,
};

typedef enum SL_CommResourcePoolV2X_r14_numSubchannel_r14_e SL_CommResourcePoolV2X_r14_numSubchannel_r14_e;

struct TDD_Config_SL_CommResourcePoolV2X_r14_rxParametersNCell_r14_tdd_Config_r14_Optional {
	bool d;
	struct TDD_Config v;
};

struct SL_CommResourcePoolV2X_r14_rxParametersNCell_r14 {
	struct TDD_Config_SL_CommResourcePoolV2X_r14_rxParametersNCell_r14_tdd_Config_r14_Optional tdd_Config_r14;
	uint8_t syncConfigIndex_r14;
};

typedef uint8_t SL_V2X_TxPoolReportIdentity_r14;

typedef uint8_t Tx_ConfigIndex_r14;

struct Tx_ConfigIndex_r14_tx_ConfigIndexList_r14_Dynamic {
	size_t d;
	Tx_ConfigIndex_r14* v;
};

struct SL_PPPP_TxConfigIndex_r14 {
	SL_Priority_r13 priorityThreshold_r14;
	uint8_t defaultTxConfigIndex_r14;
	uint8_t cbr_ConfigIndex_r14;
	struct Tx_ConfigIndex_r14_tx_ConfigIndexList_r14_Dynamic tx_ConfigIndexList_r14;
};

struct SL_PPPP_TxConfigIndex_r14_SL_CBR_PPPP_TxConfigList_r14_Dynamic {
	size_t d;
	struct SL_PPPP_TxConfigIndex_r14* v;
};

typedef struct SL_PPPP_TxConfigIndex_r14_SL_CBR_PPPP_TxConfigList_r14_Dynamic SL_CBR_PPPP_TxConfigList_r14;

enum SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e {
	SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e_true = 0,
};

typedef enum SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e;

enum SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e {
	SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e_true = 0,
};

typedef enum SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e;

struct SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e_partialSensing_r14_Optional {
	bool d;
	SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e v;
};

struct SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e_randomSelection_r14_Optional {
	bool d;
	SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e v;
};

struct SL_P2X_ResourceSelectionConfig_r14 {
	struct SL_P2X_ResourceSelectionConfig_r14_partialSensing_r14_e_partialSensing_r14_Optional partialSensing_r14;
	struct SL_P2X_ResourceSelectionConfig_r14_randomSelection_r14_e_randomSelection_r14_Optional randomSelection_r14;
};

enum SL_SyncAllowed_r14_gnss_Sync_r14_e {
	SL_SyncAllowed_r14_gnss_Sync_r14_e_true = 0,
};

typedef enum SL_SyncAllowed_r14_gnss_Sync_r14_e SL_SyncAllowed_r14_gnss_Sync_r14_e;

enum SL_SyncAllowed_r14_enb_Sync_r14_e {
	SL_SyncAllowed_r14_enb_Sync_r14_e_true = 0,
};

typedef enum SL_SyncAllowed_r14_enb_Sync_r14_e SL_SyncAllowed_r14_enb_Sync_r14_e;

enum SL_SyncAllowed_r14_ue_Sync_r14_e {
	SL_SyncAllowed_r14_ue_Sync_r14_e_true = 0,
};

typedef enum SL_SyncAllowed_r14_ue_Sync_r14_e SL_SyncAllowed_r14_ue_Sync_r14_e;

struct SL_SyncAllowed_r14_gnss_Sync_r14_e_gnss_Sync_r14_Optional {
	bool d;
	SL_SyncAllowed_r14_gnss_Sync_r14_e v;
};

struct SL_SyncAllowed_r14_enb_Sync_r14_e_enb_Sync_r14_Optional {
	bool d;
	SL_SyncAllowed_r14_enb_Sync_r14_e v;
};

struct SL_SyncAllowed_r14_ue_Sync_r14_e_ue_Sync_r14_Optional {
	bool d;
	SL_SyncAllowed_r14_ue_Sync_r14_e v;
};

struct SL_SyncAllowed_r14 {
	struct SL_SyncAllowed_r14_gnss_Sync_r14_e_gnss_Sync_r14_Optional gnss_Sync_r14;
	struct SL_SyncAllowed_r14_enb_Sync_r14_e_enb_Sync_r14_Optional enb_Sync_r14;
	struct SL_SyncAllowed_r14_ue_Sync_r14_e_ue_Sync_r14_Optional ue_Sync_r14;
};

enum SL_RestrictResourceReservationPeriod_r14_e {
	SL_RestrictResourceReservationPeriod_r14_e_v0dot2 = 0,
	SL_RestrictResourceReservationPeriod_r14_e_v0dot5 = 1,
	SL_RestrictResourceReservationPeriod_r14_e_v1 = 2,
	SL_RestrictResourceReservationPeriod_r14_e_v2 = 3,
	SL_RestrictResourceReservationPeriod_r14_e_v3 = 4,
	SL_RestrictResourceReservationPeriod_r14_e_v4 = 5,
	SL_RestrictResourceReservationPeriod_r14_e_v5 = 6,
	SL_RestrictResourceReservationPeriod_r14_e_v6 = 7,
	SL_RestrictResourceReservationPeriod_r14_e_v7 = 8,
	SL_RestrictResourceReservationPeriod_r14_e_v8 = 9,
	SL_RestrictResourceReservationPeriod_r14_e_v9 = 10,
	SL_RestrictResourceReservationPeriod_r14_e_v10 = 11,
	SL_RestrictResourceReservationPeriod_r14_e_spare4 = 12,
	SL_RestrictResourceReservationPeriod_r14_e_spare3 = 13,
	SL_RestrictResourceReservationPeriod_r14_e_spare2 = 14,
	SL_RestrictResourceReservationPeriod_r14_e_spare1 = 15,
};

typedef enum SL_RestrictResourceReservationPeriod_r14_e SL_RestrictResourceReservationPeriod_r14_e;

struct SL_RestrictResourceReservationPeriod_r14_e_SL_RestrictResourceReservationPeriodList_r14_Dynamic {
	size_t d;
	SL_RestrictResourceReservationPeriod_r14_e* v;
};

typedef struct SL_RestrictResourceReservationPeriod_r14_e_SL_RestrictResourceReservationPeriodList_r14_Dynamic SL_RestrictResourceReservationPeriodList_r14;

struct SL_MinT2Value_r15 {
	SL_PriorityList_r13 priorityList_r15;
	uint8_t minT2Value_r15;
};

struct SL_MinT2Value_r15_SL_MinT2ValueList_r15_Dynamic {
	size_t d;
	struct SL_MinT2Value_r15* v;
};

typedef struct SL_MinT2Value_r15_SL_MinT2ValueList_r15_Dynamic SL_MinT2ValueList_r15;

struct MCS_PSSCH_Range_r15 {
	uint8_t minMCS_PSSCH_r15;
	uint8_t maxMCS_PSSCH_r15;
};

struct SL_PPPP_TxConfigIndex_v1530_MCS_PSSCH_Range_r15_mcs_PSSCH_RangeList_r15_Dynamic {
	size_t d;
	struct MCS_PSSCH_Range_r15* v;
};

struct MCS_PSSCH_Range_r15_SL_PPPP_TxConfigIndex_v1530_mcs_PSSCH_RangeList_r15_DynamicOptional {
	bool d;
	struct SL_PPPP_TxConfigIndex_v1530_MCS_PSSCH_Range_r15_mcs_PSSCH_RangeList_r15_Dynamic v;
};

struct SL_PPPP_TxConfigIndex_v1530 {
	struct MCS_PSSCH_Range_r15_SL_PPPP_TxConfigIndex_v1530_mcs_PSSCH_RangeList_r15_DynamicOptional mcs_PSSCH_RangeList_r15;
};

struct SL_PPPP_TxConfigIndex_v1530_SL_CBR_PPPP_TxConfigList_v1530_Dynamic {
	size_t d;
	struct SL_PPPP_TxConfigIndex_v1530* v;
};

typedef struct SL_PPPP_TxConfigIndex_v1530_SL_CBR_PPPP_TxConfigList_v1530_Dynamic SL_CBR_PPPP_TxConfigList_v1530;

struct SL_OffsetIndicator_r12_SL_CommResourcePoolV2X_r14_sl_OffsetIndicator_r14_Optional {
	bool d;
	struct SL_OffsetIndicator_r12 v;
};

struct uint8_t_SL_CommResourcePoolV2X_r14_startRB_PSCCH_Pool_r14_Optional {
	bool d;
	uint8_t v;
};

struct SL_CommResourcePoolV2X_r14_rxParametersNCell_r14_rxParametersNCell_r14_Optional {
	bool d;
	struct SL_CommResourcePoolV2X_r14_rxParametersNCell_r14 v;
};

struct SL_TxParameters_r12_SL_CommResourcePoolV2X_r14_dataTxParameters_r14_Optional {
	bool d;
	struct SL_TxParameters_r12 v;
};

struct uint8_t_SL_CommResourcePoolV2X_r14_zoneID_r14_Optional {
	bool d;
	uint8_t v;
};

struct uint8_t_SL_CommResourcePoolV2X_r14_threshS_RSSI_CBR_r14_Optional {
	bool d;
	uint8_t v;
};

struct SL_V2X_TxPoolReportIdentity_r14_SL_CommResourcePoolV2X_r14_poolReportId_r14_Optional {
	bool d;
	SL_V2X_TxPoolReportIdentity_r14 v;
};

struct SL_CBR_PPPP_TxConfigList_r14_SL_CommResourcePoolV2X_r14_cbr_pssch_TxConfigList_r14_Optional {
	bool d;
	SL_CBR_PPPP_TxConfigList_r14 v;
};

struct SL_P2X_ResourceSelectionConfig_r14_SL_CommResourcePoolV2X_r14_resourceSelectionConfigP2X_r14_Optional {
	bool d;
	struct SL_P2X_ResourceSelectionConfig_r14 v;
};

struct SL_SyncAllowed_r14_SL_CommResourcePoolV2X_r14_syncAllowed_r14_Optional {
	bool d;
	struct SL_SyncAllowed_r14 v;
};

struct SL_RestrictResourceReservationPeriodList_r14_SL_CommResourcePoolV2X_r14_restrictResourceReservationPeriod_r14_Optional {
	bool d;
	SL_RestrictResourceReservationPeriodList_r14 v;
};

struct SL_MinT2ValueList_r15_SL_CommResourcePoolV2X_r14_sl_MinT2ValueList_r15_Optional {
	bool d;
	SL_MinT2ValueList_r15 v;
};

struct SL_CBR_PPPP_TxConfigList_v1530_SL_CommResourcePoolV2X_r14_cbr_pssch_TxConfigList_v1530_Optional {
	bool d;
	SL_CBR_PPPP_TxConfigList_v1530 v;
};

struct SL_CommResourcePoolV2X_r14 {
	struct SL_OffsetIndicator_r12_SL_CommResourcePoolV2X_r14_sl_OffsetIndicator_r14_Optional sl_OffsetIndicator_r14;
	struct SubframeBitmapSL_r14 sl_Subframe_r14;
	bool adjacencyPSCCH_PSSCH_r14;
	SL_CommResourcePoolV2X_r14_sizeSubchannel_r14_e sizeSubchannel_r14;
	SL_CommResourcePoolV2X_r14_numSubchannel_r14_e numSubchannel_r14;
	uint8_t startRB_Subchannel_r14;
	struct uint8_t_SL_CommResourcePoolV2X_r14_startRB_PSCCH_Pool_r14_Optional startRB_PSCCH_Pool_r14;
	struct SL_CommResourcePoolV2X_r14_rxParametersNCell_r14_rxParametersNCell_r14_Optional rxParametersNCell_r14;
	struct SL_TxParameters_r12_SL_CommResourcePoolV2X_r14_dataTxParameters_r14_Optional dataTxParameters_r14;
	struct uint8_t_SL_CommResourcePoolV2X_r14_zoneID_r14_Optional zoneID_r14;
	struct uint8_t_SL_CommResourcePoolV2X_r14_threshS_RSSI_CBR_r14_Optional threshS_RSSI_CBR_r14;
	struct SL_V2X_TxPoolReportIdentity_r14_SL_CommResourcePoolV2X_r14_poolReportId_r14_Optional poolReportId_r14;
	struct SL_CBR_PPPP_TxConfigList_r14_SL_CommResourcePoolV2X_r14_cbr_pssch_TxConfigList_r14_Optional cbr_pssch_TxConfigList_r14;
	struct SL_P2X_ResourceSelectionConfig_r14_SL_CommResourcePoolV2X_r14_resourceSelectionConfigP2X_r14_Optional resourceSelectionConfigP2X_r14;
	struct SL_SyncAllowed_r14_SL_CommResourcePoolV2X_r14_syncAllowed_r14_Optional syncAllowed_r14;
	struct SL_RestrictResourceReservationPeriodList_r14_SL_CommResourcePoolV2X_r14_restrictResourceReservationPeriod_r14_Optional restrictResourceReservationPeriod_r14;
	struct SL_MinT2ValueList_r15_SL_CommResourcePoolV2X_r14_sl_MinT2ValueList_r15_Optional sl_MinT2ValueList_r15;
	struct SL_CBR_PPPP_TxConfigList_v1530_SL_CommResourcePoolV2X_r14_cbr_pssch_TxConfigList_v1530_Optional cbr_pssch_TxConfigList_v1530;
};

struct SL_CommResourcePoolV2X_r14_SL_CommRxPoolListV2X_r14_Dynamic {
	size_t d;
	struct SL_CommResourcePoolV2X_r14* v;
};

typedef struct SL_CommResourcePoolV2X_r14_SL_CommRxPoolListV2X_r14_Dynamic SL_CommRxPoolListV2X_r14;

struct SL_CommResourcePoolV2X_r14_SL_CommTxPoolListV2X_r14_Dynamic {
	size_t d;
	struct SL_CommResourcePoolV2X_r14* v;
};

typedef struct SL_CommResourcePoolV2X_r14_SL_CommTxPoolListV2X_r14_Dynamic SL_CommTxPoolListV2X_r14;

struct SL_SyncConfig_r12_SL_SyncConfigListV2X_r14_Dynamic {
	size_t d;
	struct SL_SyncConfig_r12* v;
};

typedef struct SL_SyncConfig_r12_SL_SyncConfigListV2X_r14_Dynamic SL_SyncConfigListV2X_r14;

enum PLMN_IdentityInfo_cellReservedForOperatorUse_e {
	PLMN_IdentityInfo_cellReservedForOperatorUse_e_reserved = 0,
	PLMN_IdentityInfo_cellReservedForOperatorUse_e_notReserved = 1,
};

typedef enum PLMN_IdentityInfo_cellReservedForOperatorUse_e PLMN_IdentityInfo_cellReservedForOperatorUse_e;

struct PLMN_IdentityInfo {
	struct PLMN_Identity plmn_Identity;
	PLMN_IdentityInfo_cellReservedForOperatorUse_e cellReservedForOperatorUse;
};

struct PLMN_IdentityInfo_PLMN_IdentityList_Dynamic {
	size_t d;
	struct PLMN_IdentityInfo* v;
};

typedef struct PLMN_IdentityInfo_PLMN_IdentityList_Dynamic PLMN_IdentityList;

enum SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e {
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n6 = 0,
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n15 = 1,
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n25 = 2,
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n50 = 3,
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n75 = 4,
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_n100 = 5,
};

typedef enum SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e;

enum SL_TypeTxSync_r14_e {
	SL_TypeTxSync_r14_e_gnss = 0,
	SL_TypeTxSync_r14_e_enb = 1,
	SL_TypeTxSync_r14_e_ue = 2,
};

typedef enum SL_TypeTxSync_r14_e SL_TypeTxSync_r14_e;

struct SL_SyncConfigNFreq_r13_SL_SyncConfigListNFreqV2X_r14_Dynamic {
	size_t d;
	struct SL_SyncConfigNFreq_r13* v;
};

typedef struct SL_SyncConfigNFreq_r13_SL_SyncConfigListNFreqV2X_r14_Dynamic SL_SyncConfigListNFreqV2X_r14;

enum SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e {
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph60 = 0,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph80 = 1,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph100 = 2,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph120 = 3,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph140 = 4,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph160 = 5,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph180 = 6,
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e_kmph200 = 7,
};

typedef enum SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e;

enum SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e {
	SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e_n0 = 0,
	SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e_n1 = 1,
	SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e_both = 2,
	SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e_spare1 = 3,
};

typedef enum SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e;

enum SL_TxPower_r14_Sel {
	SL_TxPower_r14_UNBOUND_VALUE = 0,
	SL_TxPower_r14_minusinfinity_r14 = 1,
	SL_TxPower_r14_txPower_r14 = 2,
};

union SL_TxPower_r14_Value {
	Null minusinfinity_r14;
	int8_t txPower_r14;
};

struct SL_TxPower_r14 {
	enum SL_TxPower_r14_Sel d;
	union SL_TxPower_r14_Value v;
};

struct SL_TxPower_r14_SL_PSSCH_TxParameters_r14_maxTxPower_r14_Optional {
	bool d;
	struct SL_TxPower_r14 v;
};

struct SL_PSSCH_TxParameters_r14 {
	uint8_t minMCS_PSSCH_r14;
	uint8_t maxMCS_PSSCH_r14;
	uint8_t minSubChannel_NumberPSSCH_r14;
	uint8_t maxSubchannel_NumberPSSCH_r14;
	SL_PSSCH_TxParameters_r14_allowedRetxNumberPSSCH_r14_e allowedRetxNumberPSSCH_r14;
	struct SL_TxPower_r14_SL_PSSCH_TxParameters_r14_maxTxPower_r14_Optional maxTxPower_r14;
};

struct SL_PSSCH_TxParameters_v1530 {
	uint8_t minMCS_PSSCH_r15;
	uint8_t maxMCS_PSSCH_r15;
};

struct SL_TypeTxSync_r14_e_SL_PSSCH_TxConfig_r14_typeTxSync_r14_Optional {
	bool d;
	SL_TypeTxSync_r14_e v;
};

struct SL_PSSCH_TxParameters_v1530_SL_PSSCH_TxConfig_r14_parametersAboveThres_v1530_Optional {
	bool d;
	struct SL_PSSCH_TxParameters_v1530 v;
};

struct SL_PSSCH_TxParameters_v1530_SL_PSSCH_TxConfig_r14_parametersBelowThres_v1530_Optional {
	bool d;
	struct SL_PSSCH_TxParameters_v1530 v;
};

struct SL_PSSCH_TxConfig_r14 {
	struct SL_TypeTxSync_r14_e_SL_PSSCH_TxConfig_r14_typeTxSync_r14_Optional typeTxSync_r14;
	SL_PSSCH_TxConfig_r14_thresUE_Speed_r14_e thresUE_Speed_r14;
	struct SL_PSSCH_TxParameters_r14 parametersAboveThres_r14;
	struct SL_PSSCH_TxParameters_r14 parametersBelowThres_r14;
	struct SL_PSSCH_TxParameters_v1530_SL_PSSCH_TxConfig_r14_parametersAboveThres_v1530_Optional parametersAboveThres_v1530;
	struct SL_PSSCH_TxParameters_v1530_SL_PSSCH_TxConfig_r14_parametersBelowThres_v1530_Optional parametersBelowThres_v1530;
};

struct SL_PSSCH_TxConfig_r14_SL_PSSCH_TxConfigList_r14_Dynamic {
	size_t d;
	struct SL_PSSCH_TxConfig_r14* v;
};

typedef struct SL_PSSCH_TxConfig_r14_SL_PSSCH_TxConfigList_r14_Dynamic SL_PSSCH_TxConfigList_r14;

typedef uint8_t SL_ThresPSSCH_RSRP_r14;

typedef SL_ThresPSSCH_RSRP_r14 SL_ThresPSSCH_RSRP_List_r14[64];

enum SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e {
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_v0 = 0,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_v0dot2 = 1,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_v0dot4 = 2,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_v0dot6 = 3,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_v0dot8 = 4,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_spare3 = 5,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_spare2 = 6,
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e_spare1 = 7,
};

typedef enum SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e;

struct SL_CommTxPoolSensingConfig_r14_p2x_SensingConfig_r14 {
	uint8_t minNumCandidateSF_r14;
	B10 gapCandidateSensing_r14;
};

enum SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e {
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n1 = 0,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n2 = 1,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n3 = 2,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n4 = 3,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n5 = 4,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n6 = 5,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n7 = 6,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n8 = 7,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_n9 = 8,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare7 = 9,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare6 = 10,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare5 = 11,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare4 = 12,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare3 = 13,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare2 = 14,
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_spare1 = 15,
};

typedef enum SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e;

struct SL_RestrictResourceReservationPeriodList_r14_SL_CommTxPoolSensingConfig_r14_restrictResourceReservationPeriod_r14_Optional {
	bool d;
	SL_RestrictResourceReservationPeriodList_r14 v;
};

struct SL_CommTxPoolSensingConfig_r14_p2x_SensingConfig_r14_p2x_SensingConfig_r14_Optional {
	bool d;
	struct SL_CommTxPoolSensingConfig_r14_p2x_SensingConfig_r14 v;
};

struct SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_sl_ReselectAfter_r14_Optional {
	bool d;
	SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e v;
};

struct SL_CommTxPoolSensingConfig_r14 {
	SL_PSSCH_TxConfigList_r14 pssch_TxConfigList_r14;
	SL_ThresPSSCH_RSRP_List_r14 thresPSSCH_RSRP_List_r14;
	struct SL_RestrictResourceReservationPeriodList_r14_SL_CommTxPoolSensingConfig_r14_restrictResourceReservationPeriod_r14_Optional restrictResourceReservationPeriod_r14;
	SL_CommTxPoolSensingConfig_r14_probResourceKeep_r14_e probResourceKeep_r14;
	struct SL_CommTxPoolSensingConfig_r14_p2x_SensingConfig_r14_p2x_SensingConfig_r14_Optional p2x_SensingConfig_r14;
	struct SL_CommTxPoolSensingConfig_r14_sl_ReselectAfter_r14_e_sl_ReselectAfter_r14_Optional sl_ReselectAfter_r14;
};

enum SL_ZoneConfig_r14_zoneLength_r14_e {
	SL_ZoneConfig_r14_zoneLength_r14_e_m5 = 0,
	SL_ZoneConfig_r14_zoneLength_r14_e_m10 = 1,
	SL_ZoneConfig_r14_zoneLength_r14_e_m20 = 2,
	SL_ZoneConfig_r14_zoneLength_r14_e_m50 = 3,
	SL_ZoneConfig_r14_zoneLength_r14_e_m100 = 4,
	SL_ZoneConfig_r14_zoneLength_r14_e_m200 = 5,
	SL_ZoneConfig_r14_zoneLength_r14_e_m500 = 6,
	SL_ZoneConfig_r14_zoneLength_r14_e_spare1 = 7,
};

typedef enum SL_ZoneConfig_r14_zoneLength_r14_e SL_ZoneConfig_r14_zoneLength_r14_e;

enum SL_ZoneConfig_r14_zoneWidth_r14_e {
	SL_ZoneConfig_r14_zoneWidth_r14_e_m5 = 0,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m10 = 1,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m20 = 2,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m50 = 3,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m100 = 4,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m200 = 5,
	SL_ZoneConfig_r14_zoneWidth_r14_e_m500 = 6,
	SL_ZoneConfig_r14_zoneWidth_r14_e_spare1 = 7,
};

typedef enum SL_ZoneConfig_r14_zoneWidth_r14_e SL_ZoneConfig_r14_zoneWidth_r14_e;

struct SL_ZoneConfig_r14 {
	SL_ZoneConfig_r14_zoneLength_r14_e zoneLength_r14;
	SL_ZoneConfig_r14_zoneWidth_r14_e zoneWidth_r14;
	uint8_t zoneIdLongiMod_r14;
	uint8_t zoneIdLatiMod_r14;
};

struct PhysCellIdList_r13_SL_V2X_InterFreqUE_Config_r14_physCellIdList_r14_Optional {
	bool d;
	PhysCellIdList_r13 v;
};

struct SL_TypeTxSync_r14_e_SL_V2X_InterFreqUE_Config_r14_typeTxSync_r14_Optional {
	bool d;
	SL_TypeTxSync_r14_e v;
};

struct SL_SyncConfigListNFreqV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_SyncConfig_r14_Optional {
	bool d;
	SL_SyncConfigListNFreqV2X_r14 v;
};

struct SL_CommRxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommRxPool_r14_Optional {
	bool d;
	SL_CommRxPoolListV2X_r14 v;
};

struct SL_CommTxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommTxPoolNormal_r14_Optional {
	bool d;
	SL_CommTxPoolListV2X_r14 v;
};

struct SL_CommTxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_p2x_CommTxPoolNormal_r14_Optional {
	bool d;
	SL_CommTxPoolListV2X_r14 v;
};

struct SL_CommResourcePoolV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommTxPoolExceptional_r14_Optional {
	bool d;
	struct SL_CommResourcePoolV2X_r14 v;
};

struct SL_CommTxPoolSensingConfig_r14_SL_V2X_InterFreqUE_Config_r14_v2x_ResourceSelectionConfig_r14_Optional {
	bool d;
	struct SL_CommTxPoolSensingConfig_r14 v;
};

struct SL_ZoneConfig_r14_SL_V2X_InterFreqUE_Config_r14_zoneConfig_r14_Optional {
	bool d;
	struct SL_ZoneConfig_r14 v;
};

struct uint16_t_SL_V2X_InterFreqUE_Config_r14_offsetDFN_r14_Optional {
	bool d;
	uint16_t v;
};

struct SL_V2X_InterFreqUE_Config_r14 {
	struct PhysCellIdList_r13_SL_V2X_InterFreqUE_Config_r14_physCellIdList_r14_Optional physCellIdList_r14;
	struct SL_TypeTxSync_r14_e_SL_V2X_InterFreqUE_Config_r14_typeTxSync_r14_Optional typeTxSync_r14;
	struct SL_SyncConfigListNFreqV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_SyncConfig_r14_Optional v2x_SyncConfig_r14;
	struct SL_CommRxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommRxPool_r14_Optional v2x_CommRxPool_r14;
	struct SL_CommTxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommTxPoolNormal_r14_Optional v2x_CommTxPoolNormal_r14;
	struct SL_CommTxPoolListV2X_r14_SL_V2X_InterFreqUE_Config_r14_p2x_CommTxPoolNormal_r14_Optional p2x_CommTxPoolNormal_r14;
	struct SL_CommResourcePoolV2X_r14_SL_V2X_InterFreqUE_Config_r14_v2x_CommTxPoolExceptional_r14_Optional v2x_CommTxPoolExceptional_r14;
	struct SL_CommTxPoolSensingConfig_r14_SL_V2X_InterFreqUE_Config_r14_v2x_ResourceSelectionConfig_r14_Optional v2x_ResourceSelectionConfig_r14;
	struct SL_ZoneConfig_r14_SL_V2X_InterFreqUE_Config_r14_zoneConfig_r14_Optional zoneConfig_r14;
	struct uint16_t_SL_V2X_InterFreqUE_Config_r14_offsetDFN_r14_Optional offsetDFN_r14;
};

struct SL_V2X_InterFreqUE_Config_r14_SL_V2X_UE_ConfigList_r14_Dynamic {
	size_t d;
	struct SL_V2X_InterFreqUE_Config_r14* v;
};

typedef struct SL_V2X_InterFreqUE_Config_r14_SL_V2X_UE_ConfigList_r14_Dynamic SL_V2X_UE_ConfigList_r14;

enum SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_Sel {
	SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_UNBOUND_VALUE = 0,
	SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_additionalSpectrumEmission_r14 = 1,
	SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_additionalSpectrumEmission_v1440 = 2,
};

union SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_Value {
	AdditionalSpectrumEmission additionalSpectrumEmission_r14;
	AdditionalSpectrumEmission_v10l0 additionalSpectrumEmission_v1440;
};

struct SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14 {
	enum SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_Sel d;
	union SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_Value v;
};

typedef uint8_t SL_CBR_r14;

struct SL_CBR_r14_SL_V2X_FreqSelectionConfig_r15_threshCBR_FreqReselection_r15_Optional {
	bool d;
	SL_CBR_r14 v;
};

struct SL_CBR_r14_SL_V2X_FreqSelectionConfig_r15_threshCBR_FreqKeeping_r15_Optional {
	bool d;
	SL_CBR_r14 v;
};

struct SL_V2X_FreqSelectionConfig_r15 {
	SL_PriorityList_r13 priorityList_r15;
	struct SL_CBR_r14_SL_V2X_FreqSelectionConfig_r15_threshCBR_FreqReselection_r15_Optional threshCBR_FreqReselection_r15;
	struct SL_CBR_r14_SL_V2X_FreqSelectionConfig_r15_threshCBR_FreqKeeping_r15_Optional threshCBR_FreqKeeping_r15;
};

struct SL_V2X_FreqSelectionConfig_r15_SL_V2X_FreqSelectionConfigList_r15_Dynamic {
	size_t d;
	struct SL_V2X_FreqSelectionConfig_r15* v;
};

typedef struct SL_V2X_FreqSelectionConfig_r15_SL_V2X_FreqSelectionConfigList_r15_Dynamic SL_V2X_FreqSelectionConfigList_r15;

struct PLMN_IdentityList_SL_InterFreqInfoV2X_r14_plmn_IdentityList_r14_Optional {
	bool d;
	PLMN_IdentityList v;
};

struct P_Max_SL_InterFreqInfoV2X_r14_sl_MaxTxPower_r14_Optional {
	bool d;
	P_Max v;
};

struct SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_sl_Bandwidth_r14_Optional {
	bool d;
	SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e v;
};

struct SL_CommResourcePoolV2X_r14_SL_InterFreqInfoV2X_r14_v2x_SchedulingPool_r14_Optional {
	bool d;
	struct SL_CommResourcePoolV2X_r14 v;
};

struct SL_V2X_UE_ConfigList_r14_SL_InterFreqInfoV2X_r14_v2x_UE_ConfigList_r14_Optional {
	bool d;
	SL_V2X_UE_ConfigList_r14 v;
};

struct SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_additionalSpectrumEmissionV2X_r14_Optional {
	bool d;
	struct SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14 v;
};

struct SL_V2X_FreqSelectionConfigList_r15_SL_InterFreqInfoV2X_r14_v2x_FreqSelectionConfigList_r15_Optional {
	bool d;
	SL_V2X_FreqSelectionConfigList_r15 v;
};

struct SL_InterFreqInfoV2X_r14 {
	struct PLMN_IdentityList_SL_InterFreqInfoV2X_r14_plmn_IdentityList_r14_Optional plmn_IdentityList_r14;
	ARFCN_ValueEUTRA_r9 v2x_CommCarrierFreq_r14;
	struct P_Max_SL_InterFreqInfoV2X_r14_sl_MaxTxPower_r14_Optional sl_MaxTxPower_r14;
	struct SL_InterFreqInfoV2X_r14_sl_Bandwidth_r14_e_sl_Bandwidth_r14_Optional sl_Bandwidth_r14;
	struct SL_CommResourcePoolV2X_r14_SL_InterFreqInfoV2X_r14_v2x_SchedulingPool_r14_Optional v2x_SchedulingPool_r14;
	struct SL_V2X_UE_ConfigList_r14_SL_InterFreqInfoV2X_r14_v2x_UE_ConfigList_r14_Optional v2x_UE_ConfigList_r14;
	struct SL_InterFreqInfoV2X_r14_additionalSpectrumEmissionV2X_r14_additionalSpectrumEmissionV2X_r14_Optional additionalSpectrumEmissionV2X_r14;
	struct SL_V2X_FreqSelectionConfigList_r15_SL_InterFreqInfoV2X_r14_v2x_FreqSelectionConfigList_r15_Optional v2x_FreqSelectionConfigList_r15;
};

struct SL_InterFreqInfoV2X_r14_SL_InterFreqInfoListV2X_r14_Dynamic {
	size_t d;
	struct SL_InterFreqInfoV2X_r14* v;
};

typedef struct SL_InterFreqInfoV2X_r14_SL_InterFreqInfoListV2X_r14_Dynamic SL_InterFreqInfoListV2X_r14;

struct ARFCN_ValueEUTRA_r9_SL_AnchorCarrierFreqList_V2X_r14_Dynamic {
	size_t d;
	ARFCN_ValueEUTRA_r9* v;
};

typedef struct ARFCN_ValueEUTRA_r9_SL_AnchorCarrierFreqList_V2X_r14_Dynamic SL_AnchorCarrierFreqList_V2X_r14;

struct SL_CBR_r14_SL_CBR_Levels_Config_r14_Dynamic {
	size_t d;
	SL_CBR_r14* v;
};

typedef struct SL_CBR_r14_SL_CBR_Levels_Config_r14_Dynamic SL_CBR_Levels_Config_r14;

struct SL_CBR_PSSCH_TxConfig_r14 {
	uint16_t cr_Limit_r14;
	struct SL_PSSCH_TxParameters_r14 tx_Parameters_r14;
};

struct SL_CBR_Levels_Config_r14_cbr_RangeCommonConfigList_r14_Dynamic {
	size_t d;
	SL_CBR_Levels_Config_r14* v;
};

struct SL_CBR_PSSCH_TxConfig_r14_SL_CBR_CommonTxConfigList_r14_sl_CBR_PSSCH_TxConfigList_r14_Dynamic {
	size_t d;
	struct SL_CBR_PSSCH_TxConfig_r14* v;
};

struct SL_CBR_CommonTxConfigList_r14 {
	struct SL_CBR_Levels_Config_r14_cbr_RangeCommonConfigList_r14_Dynamic cbr_RangeCommonConfigList_r14;
	struct SL_CBR_PSSCH_TxConfig_r14_SL_CBR_CommonTxConfigList_r14_sl_CBR_PSSCH_TxConfigList_r14_Dynamic sl_CBR_PSSCH_TxConfigList_r14;
};

struct SL_CommRxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommRxPool_r14_Optional {
	bool d;
	SL_CommRxPoolListV2X_r14 v;
};

struct SL_CommTxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommTxPoolNormalCommon_r14_Optional {
	bool d;
	SL_CommTxPoolListV2X_r14 v;
};

struct SL_CommTxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_p2x_CommTxPoolNormalCommon_r14_Optional {
	bool d;
	SL_CommTxPoolListV2X_r14 v;
};

struct SL_CommResourcePoolV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommTxPoolExceptional_r14_Optional {
	bool d;
	struct SL_CommResourcePoolV2X_r14 v;
};

struct SL_SyncConfigListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_SyncConfig_r14_Optional {
	bool d;
	SL_SyncConfigListV2X_r14 v;
};

struct SL_InterFreqInfoListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_InterFreqInfoList_r14_Optional {
	bool d;
	SL_InterFreqInfoListV2X_r14 v;
};

struct SL_CommTxPoolSensingConfig_r14_SL_V2X_ConfigCommon_r14_v2x_ResourceSelectionConfig_r14_Optional {
	bool d;
	struct SL_CommTxPoolSensingConfig_r14 v;
};

struct SL_ZoneConfig_r14_SL_V2X_ConfigCommon_r14_zoneConfig_r14_Optional {
	bool d;
	struct SL_ZoneConfig_r14 v;
};

struct SL_TypeTxSync_r14_e_SL_V2X_ConfigCommon_r14_typeTxSync_r14_Optional {
	bool d;
	SL_TypeTxSync_r14_e v;
};

struct SL_Priority_r13_SL_V2X_ConfigCommon_r14_thresSL_TxPrioritization_r14_Optional {
	bool d;
	SL_Priority_r13 v;
};

struct SL_AnchorCarrierFreqList_V2X_r14_SL_V2X_ConfigCommon_r14_anchorCarrierFreqList_r14_Optional {
	bool d;
	SL_AnchorCarrierFreqList_V2X_r14 v;
};

struct uint16_t_SL_V2X_ConfigCommon_r14_offsetDFN_r14_Optional {
	bool d;
	uint16_t v;
};

struct SL_CBR_CommonTxConfigList_r14_SL_V2X_ConfigCommon_r14_cbr_CommonTxConfigList_r14_Optional {
	bool d;
	struct SL_CBR_CommonTxConfigList_r14 v;
};

struct SL_V2X_ConfigCommon_r14 {
	struct SL_CommRxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommRxPool_r14_Optional v2x_CommRxPool_r14;
	struct SL_CommTxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommTxPoolNormalCommon_r14_Optional v2x_CommTxPoolNormalCommon_r14;
	struct SL_CommTxPoolListV2X_r14_SL_V2X_ConfigCommon_r14_p2x_CommTxPoolNormalCommon_r14_Optional p2x_CommTxPoolNormalCommon_r14;
	struct SL_CommResourcePoolV2X_r14_SL_V2X_ConfigCommon_r14_v2x_CommTxPoolExceptional_r14_Optional v2x_CommTxPoolExceptional_r14;
	struct SL_SyncConfigListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_SyncConfig_r14_Optional v2x_SyncConfig_r14;
	struct SL_InterFreqInfoListV2X_r14_SL_V2X_ConfigCommon_r14_v2x_InterFreqInfoList_r14_Optional v2x_InterFreqInfoList_r14;
	struct SL_CommTxPoolSensingConfig_r14_SL_V2X_ConfigCommon_r14_v2x_ResourceSelectionConfig_r14_Optional v2x_ResourceSelectionConfig_r14;
	struct SL_ZoneConfig_r14_SL_V2X_ConfigCommon_r14_zoneConfig_r14_Optional zoneConfig_r14;
	struct SL_TypeTxSync_r14_e_SL_V2X_ConfigCommon_r14_typeTxSync_r14_Optional typeTxSync_r14;
	struct SL_Priority_r13_SL_V2X_ConfigCommon_r14_thresSL_TxPrioritization_r14_Optional thresSL_TxPrioritization_r14;
	struct SL_AnchorCarrierFreqList_V2X_r14_SL_V2X_ConfigCommon_r14_anchorCarrierFreqList_r14_Optional anchorCarrierFreqList_r14;
	struct uint16_t_SL_V2X_ConfigCommon_r14_offsetDFN_r14_Optional offsetDFN_r14;
	struct SL_CBR_CommonTxConfigList_r14_SL_V2X_ConfigCommon_r14_cbr_CommonTxConfigList_r14_Optional cbr_CommonTxConfigList_r14;
};

struct ARFCN_ValueNR_r15_SL_NR_AnchorCarrierFreqList_r16_Dynamic {
	size_t d;
	ARFCN_ValueNR_r15* v;
};

typedef struct ARFCN_ValueNR_r15_SL_NR_AnchorCarrierFreqList_r16_Dynamic SL_NR_AnchorCarrierFreqList_r16;

struct SL_V2X_ConfigCommon_r14_SystemInformationBlockType21_r14_sl_V2X_ConfigCommon_r14_Optional {
	bool d;
	struct SL_V2X_ConfigCommon_r14 v;
};

struct OCTET_STRING_SystemInformationBlockType21_r14_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SL_NR_AnchorCarrierFreqList_r16_SystemInformationBlockType21_r14_anchorCarrierFreqListNR_r16_Optional {
	bool d;
	SL_NR_AnchorCarrierFreqList_r16 v;
};

struct SystemInformationBlockType21_r14 {
	struct SL_V2X_ConfigCommon_r14_SystemInformationBlockType21_r14_sl_V2X_ConfigCommon_r14_Optional sl_V2X_ConfigCommon_r14;
	struct OCTET_STRING_SystemInformationBlockType21_r14_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SL_NR_AnchorCarrierFreqList_r16_SystemInformationBlockType21_r14_anchorCarrierFreqListNR_r16_Optional anchorCarrierFreqListNR_r16;
};

enum CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e {
	CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e_kHz15 = 0,
	CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e_kHz30 = 1,
	CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e_kHz120 = 2,
	CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e_kHz240 = 3,
};

typedef enum CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e;

struct CarrierFreqNR_r15_threshX_Q_r15 {
	ReselectionThresholdQ_r9 threshX_HighQ_r15;
	ReselectionThresholdQ_r9 threshX_LowQ_r15;
};

typedef int8_t P_MaxNR_r15;

typedef uint8_t AdditionalSpectrumEmissionNR_r15;

struct P_MaxNR_r15_NS_PmaxValueNR_r15_additionalPmaxNR_r15_Optional {
	bool d;
	P_MaxNR_r15 v;
};

struct NS_PmaxValueNR_r15 {
	struct P_MaxNR_r15_NS_PmaxValueNR_r15_additionalPmaxNR_r15_Optional additionalPmaxNR_r15;
	AdditionalSpectrumEmissionNR_r15 additionalSpectrumEmissionNR_r15;
};

struct NS_PmaxValueNR_r15_NS_PmaxListNR_r15_Dynamic {
	size_t d;
	struct NS_PmaxValueNR_r15* v;
};

typedef struct NS_PmaxValueNR_r15_NS_PmaxListNR_r15_Dynamic NS_PmaxListNR_r15;

struct NS_PmaxListNR_r15_MultiBandNsPmaxListNR_1_v1550_Dynamic {
	size_t d;
	NS_PmaxListNR_r15* v;
};

typedef struct NS_PmaxListNR_r15_MultiBandNsPmaxListNR_1_v1550_Dynamic MultiBandNsPmaxListNR_1_v1550;

struct NS_PmaxListNR_r15_MultiBandNsPmaxListNR_v1550_Dynamic {
	size_t d;
	NS_PmaxListNR_r15* v;
};

typedef struct NS_PmaxListNR_r15_MultiBandNsPmaxListNR_v1550_Dynamic MultiBandNsPmaxListNR_v1550;

struct MultiFrequencyBandListNR_r15_CarrierFreqNR_r15_multiBandInfoList_r15_Optional {
	bool d;
	MultiFrequencyBandListNR_r15 v;
};

struct MultiFrequencyBandListNR_r15_CarrierFreqNR_r15_multiBandInfoListSUL_r15_Optional {
	bool d;
	MultiFrequencyBandListNR_r15 v;
};

struct MTC_SSB_NR_r15_CarrierFreqNR_r15_measTimingConfig_r15_Optional {
	bool d;
	struct MTC_SSB_NR_r15 v;
};

struct SS_RSSI_Measurement_r15_CarrierFreqNR_r15_ss_RSSI_Measurement_r15_Optional {
	bool d;
	struct SS_RSSI_Measurement_r15 v;
};

struct CellReselectionPriority_CarrierFreqNR_r15_cellReselectionPriority_r15_Optional {
	bool d;
	CellReselectionPriority v;
};

struct CellReselectionSubPriority_r13_e_CarrierFreqNR_r15_cellReselectionSubPriority_r15_Optional {
	bool d;
	CellReselectionSubPriority_r13_e v;
};

struct CarrierFreqNR_r15_threshX_Q_r15_threshX_Q_r15_Optional {
	bool d;
	struct CarrierFreqNR_r15_threshX_Q_r15 v;
};

struct int8_t_CarrierFreqNR_r15_q_RxLevMinSUL_r15_Optional {
	bool d;
	int8_t v;
};

struct NS_PmaxListNR_r15_CarrierFreqNR_r15_ns_PmaxListNR_r15_Optional {
	bool d;
	NS_PmaxListNR_r15 v;
};

struct int8_t_CarrierFreqNR_r15_q_QualMin_r15_Optional {
	bool d;
	int8_t v;
};

struct MaxRS_IndexCellQualNR_r15_CarrierFreqNR_r15_maxRS_IndexCellQual_r15_Optional {
	bool d;
	MaxRS_IndexCellQualNR_r15 v;
};

struct ThresholdListNR_r15_CarrierFreqNR_r15_threshRS_Index_r15_Optional {
	bool d;
	struct ThresholdListNR_r15 v;
};

struct MultiBandNsPmaxListNR_1_v1550_CarrierFreqNR_r15_multiBandNsPmaxListNR_v1550_Optional {
	bool d;
	MultiBandNsPmaxListNR_1_v1550 v;
};

struct MultiBandNsPmaxListNR_v1550_CarrierFreqNR_r15_multiBandNsPmaxListNR_SUL_v1550_Optional {
	bool d;
	MultiBandNsPmaxListNR_v1550 v;
};

struct SSB_ToMeasure_r15_CarrierFreqNR_r15_ssb_ToMeasure_r15_Optional {
	bool d;
	struct SSB_ToMeasure_r15 v;
};

struct CarrierFreqNR_r15 {
	ARFCN_ValueNR_r15 carrierFreq_r15;
	struct MultiFrequencyBandListNR_r15_CarrierFreqNR_r15_multiBandInfoList_r15_Optional multiBandInfoList_r15;
	struct MultiFrequencyBandListNR_r15_CarrierFreqNR_r15_multiBandInfoListSUL_r15_Optional multiBandInfoListSUL_r15;
	struct MTC_SSB_NR_r15_CarrierFreqNR_r15_measTimingConfig_r15_Optional measTimingConfig_r15;
	CarrierFreqNR_r15_subcarrierSpacingSSB_r15_e subcarrierSpacingSSB_r15;
	struct SS_RSSI_Measurement_r15_CarrierFreqNR_r15_ss_RSSI_Measurement_r15_Optional ss_RSSI_Measurement_r15;
	struct CellReselectionPriority_CarrierFreqNR_r15_cellReselectionPriority_r15_Optional cellReselectionPriority_r15;
	struct CellReselectionSubPriority_r13_e_CarrierFreqNR_r15_cellReselectionSubPriority_r15_Optional cellReselectionSubPriority_r15;
	ReselectionThreshold threshX_High_r15;
	ReselectionThreshold threshX_Low_r15;
	struct CarrierFreqNR_r15_threshX_Q_r15_threshX_Q_r15_Optional threshX_Q_r15;
	int8_t q_RxLevMin_r15;
	struct int8_t_CarrierFreqNR_r15_q_RxLevMinSUL_r15_Optional q_RxLevMinSUL_r15;
	P_MaxNR_r15 p_MaxNR_r15;
	struct NS_PmaxListNR_r15_CarrierFreqNR_r15_ns_PmaxListNR_r15_Optional ns_PmaxListNR_r15;
	struct int8_t_CarrierFreqNR_r15_q_QualMin_r15_Optional q_QualMin_r15;
	bool deriveSSB_IndexFromCell_r15;
	struct MaxRS_IndexCellQualNR_r15_CarrierFreqNR_r15_maxRS_IndexCellQual_r15_Optional maxRS_IndexCellQual_r15;
	struct ThresholdListNR_r15_CarrierFreqNR_r15_threshRS_Index_r15_Optional threshRS_Index_r15;
	struct MultiBandNsPmaxListNR_1_v1550_CarrierFreqNR_r15_multiBandNsPmaxListNR_v1550_Optional multiBandNsPmaxListNR_v1550;
	struct MultiBandNsPmaxListNR_v1550_CarrierFreqNR_r15_multiBandNsPmaxListNR_SUL_v1550_Optional multiBandNsPmaxListNR_SUL_v1550;
	struct SSB_ToMeasure_r15_CarrierFreqNR_r15_ssb_ToMeasure_r15_Optional ssb_ToMeasure_r15;
};

struct CarrierFreqNR_r15_CarrierFreqListNR_r15_Dynamic {
	size_t d;
	struct CarrierFreqNR_r15* v;
};

typedef struct CarrierFreqNR_r15_CarrierFreqListNR_r15_Dynamic CarrierFreqListNR_r15;

enum MTC_SSB2_LP_NR_r16_periodicity_r16_e {
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_sf10 = 0,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_sf20 = 1,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_sf40 = 2,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_sf80 = 3,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_sf160 = 4,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_spare3 = 5,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_spare2 = 6,
	MTC_SSB2_LP_NR_r16_periodicity_r16_e_spare1 = 7,
};

typedef enum MTC_SSB2_LP_NR_r16_periodicity_r16_e MTC_SSB2_LP_NR_r16_periodicity_r16_e;

struct PhysCellIdNR_r15_MTC_SSB2_LP_NR_r16_pci_List_r16_Dynamic {
	size_t d;
	PhysCellIdNR_r15* v;
};

struct PhysCellIdNR_r15_MTC_SSB2_LP_NR_r16_pci_List_r16_DynamicOptional {
	bool d;
	struct PhysCellIdNR_r15_MTC_SSB2_LP_NR_r16_pci_List_r16_Dynamic v;
};

struct MTC_SSB2_LP_NR_r16 {
	struct PhysCellIdNR_r15_MTC_SSB2_LP_NR_r16_pci_List_r16_DynamicOptional pci_List_r16;
	MTC_SSB2_LP_NR_r16_periodicity_r16_e periodicity_r16;
};

enum SSB_PositionQCL_RelationNR_r16_e {
	SSB_PositionQCL_RelationNR_r16_e_n1 = 0,
	SSB_PositionQCL_RelationNR_r16_e_n2 = 1,
	SSB_PositionQCL_RelationNR_r16_e_n4 = 2,
	SSB_PositionQCL_RelationNR_r16_e_n8 = 3,
};

typedef enum SSB_PositionQCL_RelationNR_r16_e SSB_PositionQCL_RelationNR_r16_e;

struct PhysCellIdNR_r15_WhiteCellListNR_r16_Dynamic {
	size_t d;
	PhysCellIdNR_r15* v;
};

typedef struct PhysCellIdNR_r15_WhiteCellListNR_r16_Dynamic WhiteCellListNR_r16;

enum CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e {
	CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e_true = 0,
};

typedef enum CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e;

struct MTC_SSB2_LP_NR_r16_CarrierFreqNR_v1610_smtc2_LP_r16_Optional {
	bool d;
	struct MTC_SSB2_LP_NR_r16 v;
};

struct SSB_PositionQCL_RelationNR_r16_e_CarrierFreqNR_v1610_ssb_PositionQCL_CommonNR_r16_Optional {
	bool d;
	SSB_PositionQCL_RelationNR_r16_e v;
};

struct WhiteCellListNR_r16_CarrierFreqNR_v1610_whiteCellListNR_r16_Optional {
	bool d;
	WhiteCellListNR_r16 v;
};

struct CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e_highSpeedCarrierNR_r16_Optional {
	bool d;
	CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e v;
};

struct CarrierFreqNR_v1610 {
	struct MTC_SSB2_LP_NR_r16_CarrierFreqNR_v1610_smtc2_LP_r16_Optional smtc2_LP_r16;
	struct SSB_PositionQCL_RelationNR_r16_e_CarrierFreqNR_v1610_ssb_PositionQCL_CommonNR_r16_Optional ssb_PositionQCL_CommonNR_r16;
	struct WhiteCellListNR_r16_CarrierFreqNR_v1610_whiteCellListNR_r16_Optional whiteCellListNR_r16;
	struct CarrierFreqNR_v1610_highSpeedCarrierNR_r16_e_highSpeedCarrierNR_r16_Optional highSpeedCarrierNR_r16;
};

struct CarrierFreqNR_v1610_CarrierFreqListNR_v1610_Dynamic {
	size_t d;
	struct CarrierFreqNR_v1610* v;
};

typedef struct CarrierFreqNR_v1610_CarrierFreqListNR_v1610_Dynamic CarrierFreqListNR_v1610;

struct CarrierFreqListNR_r15_SystemInformationBlockType24_r15_carrierFreqListNR_r15_Optional {
	bool d;
	CarrierFreqListNR_r15 v;
};

struct SpeedStateScaleFactors_SystemInformationBlockType24_r15_t_ReselectionNR_SF_r15_Optional {
	bool d;
	struct SpeedStateScaleFactors v;
};

struct OCTET_STRING_SystemInformationBlockType24_r15_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct CarrierFreqListNR_v1610_SystemInformationBlockType24_r15_carrierFreqListNR_v1610_Optional {
	bool d;
	CarrierFreqListNR_v1610 v;
};

struct SystemInformationBlockType24_r15 {
	struct CarrierFreqListNR_r15_SystemInformationBlockType24_r15_carrierFreqListNR_r15_Optional carrierFreqListNR_r15;
	T_Reselection t_ReselectionNR_r15;
	struct SpeedStateScaleFactors_SystemInformationBlockType24_r15_t_ReselectionNR_SF_r15_Optional t_ReselectionNR_SF_r15;
	struct OCTET_STRING_SystemInformationBlockType24_r15_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct CarrierFreqListNR_v1610_SystemInformationBlockType24_r15_carrierFreqListNR_v1610_Optional carrierFreqListNR_v1610;
};

typedef uint8_t UAC_BarringInfoSetIndex_r15;

struct UAC_BarringPerCat_r15 {
	uint8_t accessCategory_r15;
	UAC_BarringInfoSetIndex_r15 uac_barringInfoSetIndex_r15;
};

struct UAC_BarringPerCat_r15_UAC_BarringPerCatList_r15_Dynamic {
	size_t d;
	struct UAC_BarringPerCat_r15* v;
};

typedef struct UAC_BarringPerCat_r15_UAC_BarringPerCatList_r15_Dynamic UAC_BarringPerCatList_r15;

enum UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_Sel {
	UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_UNBOUND_VALUE = 0,
	UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_uac_ImplicitAC_BarringList_r15 = 1,
	UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_uac_ExplicitAC_BarringList_r15 = 2,
};

union UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_Value {
	UAC_BarringInfoSetIndex_r15 uac_ImplicitAC_BarringList_r15[63];
	UAC_BarringPerCatList_r15 uac_ExplicitAC_BarringList_r15;
};

struct UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15 {
	enum UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_Sel d;
	union UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_Value v;
};

struct UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_uac_AC_BarringListType_r15_Optional {
	bool d;
	struct UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15 v;
};

struct UAC_BarringPerPLMN_r15 {
	uint8_t plmn_IdentityIndex_r15;
	struct UAC_BarringPerPLMN_r15_uac_AC_BarringListType_r15_uac_AC_BarringListType_r15_Optional uac_AC_BarringListType_r15;
};

struct UAC_BarringPerPLMN_r15_UAC_BarringPerPLMN_List_r15_Dynamic {
	size_t d;
	struct UAC_BarringPerPLMN_r15* v;
};

typedef struct UAC_BarringPerPLMN_r15_UAC_BarringPerPLMN_List_r15_Dynamic UAC_BarringPerPLMN_List_r15;

enum UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e {
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p00 = 0,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p05 = 1,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p10 = 2,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p15 = 3,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p20 = 4,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p25 = 5,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p30 = 6,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p40 = 7,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p50 = 8,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p60 = 9,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p70 = 10,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p75 = 11,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p80 = 12,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p85 = 13,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p90 = 14,
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e_p95 = 15,
};

typedef enum UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e;

enum UAC_BarringInfoSet_r15_uac_BarringTime_r15_e {
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s4 = 0,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s8 = 1,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s16 = 2,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s32 = 3,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s64 = 4,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s128 = 5,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s256 = 6,
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e_s512 = 7,
};

typedef enum UAC_BarringInfoSet_r15_uac_BarringTime_r15_e UAC_BarringInfoSet_r15_uac_BarringTime_r15_e;

struct UAC_BarringInfoSet_r15 {
	UAC_BarringInfoSet_r15_uac_BarringFactor_r15_e uac_BarringFactor_r15;
	UAC_BarringInfoSet_r15_uac_BarringTime_r15_e uac_BarringTime_r15;
	B7 uac_BarringForAccessIdentity_r15;
};

struct UAC_BarringInfoSet_r15_UAC_BarringInfoSetList_r15_Dynamic {
	size_t d;
	struct UAC_BarringInfoSet_r15* v;
};

typedef struct UAC_BarringInfoSet_r15_UAC_BarringInfoSetList_r15_Dynamic UAC_BarringInfoSetList_r15;

enum UAC_AC1_SelectAssistInfo_r15_e {
	UAC_AC1_SelectAssistInfo_r15_e_a = 0,
	UAC_AC1_SelectAssistInfo_r15_e_b = 1,
	UAC_AC1_SelectAssistInfo_r15_e_c = 2,
};

typedef enum UAC_AC1_SelectAssistInfo_r15_e UAC_AC1_SelectAssistInfo_r15_e;

enum SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_Sel {
	SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_UNBOUND_VALUE = 0,
	SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_plmnCommon_r15 = 1,
	SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_individualPLMNList_r15 = 2,
};

struct UAC_AC1_SelectAssistInfo_r15_e_individualPLMNList_r15_Dynamic {
	size_t d;
	UAC_AC1_SelectAssistInfo_r15_e* v;
};

union SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_Value {
	UAC_AC1_SelectAssistInfo_r15_e plmnCommon_r15;
	struct UAC_AC1_SelectAssistInfo_r15_e_individualPLMNList_r15_Dynamic individualPLMNList_r15;
};

struct SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15 {
	enum SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_Sel d;
	union SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_Value v;
};

enum SystemInformationBlockType25_r15_ab_PerRSRP_r16_e {
	SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_thresh0 = 0,
	SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_thresh1 = 1,
	SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_thresh2 = 2,
	SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_thresh3 = 3,
};

typedef enum SystemInformationBlockType25_r15_ab_PerRSRP_r16_e SystemInformationBlockType25_r15_ab_PerRSRP_r16_e;

struct UAC_BarringPerCatList_r15_SystemInformationBlockType25_r15_uac_BarringForCommon_r15_Optional {
	bool d;
	UAC_BarringPerCatList_r15 v;
};

struct UAC_BarringPerPLMN_List_r15_SystemInformationBlockType25_r15_uac_BarringPerPLMN_List_r15_Optional {
	bool d;
	UAC_BarringPerPLMN_List_r15 v;
};

struct SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_uac_AC1_SelectAssistInfo_r15_Optional {
	bool d;
	struct SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15 v;
};

struct OCTET_STRING_SystemInformationBlockType25_r15_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_ab_PerRSRP_r16_Optional {
	bool d;
	SystemInformationBlockType25_r15_ab_PerRSRP_r16_e v;
};

struct SystemInformationBlockType25_r15 {
	struct UAC_BarringPerCatList_r15_SystemInformationBlockType25_r15_uac_BarringForCommon_r15_Optional uac_BarringForCommon_r15;
	struct UAC_BarringPerPLMN_List_r15_SystemInformationBlockType25_r15_uac_BarringPerPLMN_List_r15_Optional uac_BarringPerPLMN_List_r15;
	UAC_BarringInfoSetList_r15 uac_BarringInfoSetList_r15;
	struct SystemInformationBlockType25_r15_uac_AC1_SelectAssistInfo_r15_uac_AC1_SelectAssistInfo_r15_Optional uac_AC1_SelectAssistInfo_r15;
	struct OCTET_STRING_SystemInformationBlockType25_r15_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType25_r15_ab_PerRSRP_r16_e_ab_PerRSRP_r16_Optional ab_PerRSRP_r16;
};

struct Tx_ConfigIndex_r14_tx_ConfigIndexList_r15_Dynamic {
	size_t d;
	Tx_ConfigIndex_r14* v;
};

struct MCS_PSSCH_Range_r15_SL_PPPP_TxConfigIndex_r15_mcs_PSSCH_RangeList_r15_Dynamic {
	size_t d;
	struct MCS_PSSCH_Range_r15* v;
};

struct SL_PPPP_TxConfigIndex_r15 {
	SL_Priority_r13 priorityThreshold_r15;
	uint8_t defaultTxConfigIndex_r15;
	uint8_t cbr_ConfigIndex_r15;
	struct Tx_ConfigIndex_r14_tx_ConfigIndexList_r15_Dynamic tx_ConfigIndexList_r15;
	struct MCS_PSSCH_Range_r15_SL_PPPP_TxConfigIndex_r15_mcs_PSSCH_RangeList_r15_Dynamic mcs_PSSCH_RangeList_r15;
};

struct SL_PPPP_TxConfigIndex_r15_SL_CBR_PPPP_TxConfigList_r15_Dynamic {
	size_t d;
	struct SL_PPPP_TxConfigIndex_r15* v;
};

typedef struct SL_PPPP_TxConfigIndex_r15_SL_CBR_PPPP_TxConfigList_r15_Dynamic SL_CBR_PPPP_TxConfigList_r15;

typedef uint8_t SL_Reliability_r15;

typedef B24 SL_DestinationIdentity_r12;

struct SL_DestinationIdentity_r12_SL_DestinationInfoList_r12_Dynamic {
	size_t d;
	SL_DestinationIdentity_r12* v;
};

typedef struct SL_DestinationIdentity_r12_SL_DestinationInfoList_r12_Dynamic SL_DestinationInfoList_r12;

struct ARFCN_ValueEUTRA_r9_allowedCarrierFreqSet1_Dynamic {
	size_t d;
	ARFCN_ValueEUTRA_r9* v;
};

struct ARFCN_ValueEUTRA_r9_allowedCarrierFreqSet2_Dynamic {
	size_t d;
	ARFCN_ValueEUTRA_r9* v;
};

struct SL_AllowedCarrierFreqList_r15 {
	struct ARFCN_ValueEUTRA_r9_allowedCarrierFreqSet1_Dynamic allowedCarrierFreqSet1;
	struct ARFCN_ValueEUTRA_r9_allowedCarrierFreqSet2_Dynamic allowedCarrierFreqSet2;
};

struct SL_DestinationInfoList_r12_SL_PPPR_Dest_CarrierFreq_destinationInfoList_r15_Optional {
	bool d;
	SL_DestinationInfoList_r12 v;
};

struct SL_AllowedCarrierFreqList_r15_SL_PPPR_Dest_CarrierFreq_allowedCarrierFreqList_r15_Optional {
	bool d;
	struct SL_AllowedCarrierFreqList_r15 v;
};

struct SL_PPPR_Dest_CarrierFreq {
	struct SL_DestinationInfoList_r12_SL_PPPR_Dest_CarrierFreq_destinationInfoList_r15_Optional destinationInfoList_r15;
	struct SL_AllowedCarrierFreqList_r15_SL_PPPR_Dest_CarrierFreq_allowedCarrierFreqList_r15_Optional allowedCarrierFreqList_r15;
};

struct SL_PPPR_Dest_CarrierFreq_SL_PPPR_Dest_CarrierFreqList_r15_Dynamic {
	size_t d;
	struct SL_PPPR_Dest_CarrierFreq* v;
};

typedef struct SL_PPPR_Dest_CarrierFreq_SL_PPPR_Dest_CarrierFreqList_r15_Dynamic SL_PPPR_Dest_CarrierFreqList_r15;

struct SL_PPPR_Dest_CarrierFreqList_r15_SL_V2X_PacketDuplicationConfig_r15_allowedCarrierFreqConfig_r15_Optional {
	bool d;
	SL_PPPR_Dest_CarrierFreqList_r15 v;
};

struct SL_V2X_PacketDuplicationConfig_r15 {
	SL_Reliability_r15 threshSL_Reliability_r15;
	struct SL_PPPR_Dest_CarrierFreqList_r15_SL_V2X_PacketDuplicationConfig_r15_allowedCarrierFreqConfig_r15_Optional allowedCarrierFreqConfig_r15;
};

struct ARFCN_ValueEUTRA_r9_SL_V2X_SyncFreqList_r15_Dynamic {
	size_t d;
	ARFCN_ValueEUTRA_r9* v;
};

typedef struct ARFCN_ValueEUTRA_r9_SL_V2X_SyncFreqList_r15_Dynamic SL_V2X_SyncFreqList_r15;

enum SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e {
	SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e_true = 0,
};

typedef enum SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e;

struct SL_InterFreqInfoListV2X_r14_SystemInformationBlockType26_r15_v2x_InterFreqInfoList_r15_Optional {
	bool d;
	SL_InterFreqInfoListV2X_r14 v;
};

struct SL_CBR_PPPP_TxConfigList_r15_SystemInformationBlockType26_r15_cbr_pssch_TxConfigList_r15_Optional {
	bool d;
	SL_CBR_PPPP_TxConfigList_r15 v;
};

struct SL_V2X_PacketDuplicationConfig_r15_SystemInformationBlockType26_r15_v2x_PacketDuplicationConfig_r15_Optional {
	bool d;
	struct SL_V2X_PacketDuplicationConfig_r15 v;
};

struct SL_V2X_SyncFreqList_r15_SystemInformationBlockType26_r15_syncFreqList_r15_Optional {
	bool d;
	SL_V2X_SyncFreqList_r15 v;
};

struct SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e_slss_TxMultiFreq_r15_Optional {
	bool d;
	SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e v;
};

struct SL_V2X_FreqSelectionConfigList_r15_SystemInformationBlockType26_r15_v2x_FreqSelectionConfigList_r15_Optional {
	bool d;
	SL_V2X_FreqSelectionConfigList_r15 v;
};

struct uint8_t_SystemInformationBlockType26_r15_threshS_RSSI_CBR_r15_Optional {
	bool d;
	uint8_t v;
};

struct OCTET_STRING_SystemInformationBlockType26_r15_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType26_r15 {
	struct SL_InterFreqInfoListV2X_r14_SystemInformationBlockType26_r15_v2x_InterFreqInfoList_r15_Optional v2x_InterFreqInfoList_r15;
	struct SL_CBR_PPPP_TxConfigList_r15_SystemInformationBlockType26_r15_cbr_pssch_TxConfigList_r15_Optional cbr_pssch_TxConfigList_r15;
	struct SL_V2X_PacketDuplicationConfig_r15_SystemInformationBlockType26_r15_v2x_PacketDuplicationConfig_r15_Optional v2x_PacketDuplicationConfig_r15;
	struct SL_V2X_SyncFreqList_r15_SystemInformationBlockType26_r15_syncFreqList_r15_Optional syncFreqList_r15;
	struct SystemInformationBlockType26_r15_slss_TxMultiFreq_r15_e_slss_TxMultiFreq_r15_Optional slss_TxMultiFreq_r15;
	struct SL_V2X_FreqSelectionConfigList_r15_SystemInformationBlockType26_r15_v2x_FreqSelectionConfigList_r15_Optional v2x_FreqSelectionConfigList_r15;
	struct uint8_t_SystemInformationBlockType26_r15_threshS_RSSI_CBR_r15_Optional threshS_RSSI_CBR_r15;
	struct OCTET_STRING_SystemInformationBlockType26_r15_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

struct B10_PLMN_Info_r16_nr_BandList_r16_Optional {
	bool d;
	B10 v;
};

struct PLMN_Info_r16 {
	struct B10_PLMN_Info_r16_nr_BandList_r16_Optional nr_BandList_r16;
};

struct PLMN_Info_r16_PLMN_InfoList_r16_Dynamic {
	size_t d;
	struct PLMN_Info_r16* v;
};

typedef struct PLMN_Info_r16_PLMN_InfoList_r16_Dynamic PLMN_InfoList_r16;

struct FreqBandIndicatorNR_r15_BandListENDC_r16_Dynamic {
	size_t d;
	FreqBandIndicatorNR_r15* v;
};

typedef struct FreqBandIndicatorNR_r15_BandListENDC_r16_Dynamic BandListENDC_r16;

struct OCTET_STRING_SystemInformationBlockType26a_r16_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType26a_r16 {
	PLMN_InfoList_r16 plmn_InfoList_r16;
	BandListENDC_r16 bandListENDC_r16;
	struct OCTET_STRING_SystemInformationBlockType26a_r16_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e {
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_10 = 0,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_9 = 1,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_8dot5 = 2,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_8 = 3,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_7 = 4,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_6 = 5,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_5 = 6,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_4dot5 = 7,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_4 = 8,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_3 = 9,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_2 = 10,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_1 = 11,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v_0dot5 = 12,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v0 = 13,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v1 = 14,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v2 = 15,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v3 = 16,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v3dot5 = 17,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v4 = 18,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v5 = 19,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v6 = 20,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v7 = 21,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v7dot5 = 22,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v8 = 23,
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e_v9 = 24,
};

typedef enum CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e;

struct CarrierFreqNBIOT_r16 {
	ARFCN_ValueEUTRA_r9 carrierFreq_r16;
	CarrierFreqNBIOT_r16_carrierFreqOffset_r16_e carrierFreqOffset_r16;
};

struct CarrierFreqNBIOT_r16_CarrierFreqListNBIOT_r16_Dynamic {
	size_t d;
	struct CarrierFreqNBIOT_r16* v;
};

typedef struct CarrierFreqNBIOT_r16_CarrierFreqListNBIOT_r16_Dynamic CarrierFreqListNBIOT_r16;

struct CarrierFreqListNBIOT_r16_SystemInformationBlockType27_r16_carrierFreqListNBIOT_r16_Optional {
	bool d;
	CarrierFreqListNBIOT_r16 v;
};

struct OCTET_STRING_SystemInformationBlockType27_r16_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType27_r16 {
	struct CarrierFreqListNBIOT_r16_SystemInformationBlockType27_r16_carrierFreqListNBIOT_r16_Optional carrierFreqListNBIOT_r16;
	struct OCTET_STRING_SystemInformationBlockType27_r16_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SystemInformationBlockType28_r16_segmentType_r16_e {
	SystemInformationBlockType28_r16_segmentType_r16_e_notLastSegment = 0,
	SystemInformationBlockType28_r16_segmentType_r16_e_lastSegment = 1,
};

typedef enum SystemInformationBlockType28_r16_segmentType_r16_e SystemInformationBlockType28_r16_segmentType_r16_e;

struct OCTET_STRING_SystemInformationBlockType28_r16_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType28_r16 {
	uint8_t segmentNumber_r16;
	SystemInformationBlockType28_r16_segmentType_r16_e segmentType_r16;
	OCTET_STRING segmentContainer_r16;
	struct OCTET_STRING_SystemInformationBlockType28_r16_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum PeriodicityStartPos_r16_Sel {
	PeriodicityStartPos_r16_UNBOUND_VALUE = 0,
	PeriodicityStartPos_r16_periodicity10ms = 1,
	PeriodicityStartPos_r16_periodicity20ms = 2,
	PeriodicityStartPos_r16_periodicity40ms = 3,
	PeriodicityStartPos_r16_periodicity80ms = 4,
	PeriodicityStartPos_r16_periodicity160ms = 5,
	PeriodicityStartPos_r16_spare3 = 6,
	PeriodicityStartPos_r16_spare2 = 7,
	PeriodicityStartPos_r16_spare1 = 8,
};

union PeriodicityStartPos_r16_Value {
	Null periodicity10ms;
	uint8_t periodicity20ms;
	uint8_t periodicity40ms;
	uint8_t periodicity80ms;
	uint8_t periodicity160ms;
	Null spare3;
	Null spare2;
	Null spare1;
};

struct PeriodicityStartPos_r16 {
	enum PeriodicityStartPos_r16_Sel d;
	union PeriodicityStartPos_r16_Value v;
};

enum ResourceReservationConfigDL_r16_resourceReservationFreq_r16_Sel {
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_UNBOUND_VALUE = 0,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap1dot4 = 1,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap3 = 2,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap5 = 3,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap10 = 4,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap15 = 5,
	ResourceReservationConfigDL_r16_resourceReservationFreq_r16_rbg_Bitmap20 = 6,
};

union ResourceReservationConfigDL_r16_resourceReservationFreq_r16_Value {
	B6 rbg_Bitmap1dot4;
	B8 rbg_Bitmap3;
	B13 rbg_Bitmap5;
	B17 rbg_Bitmap10;
	B19 rbg_Bitmap15;
	B25 rbg_Bitmap20;
};

struct ResourceReservationConfigDL_r16_resourceReservationFreq_r16 {
	enum ResourceReservationConfigDL_r16_resourceReservationFreq_r16_Sel d;
	union ResourceReservationConfigDL_r16_resourceReservationFreq_r16_Value v;
};

enum ResourceReservationConfigDL_r16_slotBitmap_r16_Sel {
	ResourceReservationConfigDL_r16_slotBitmap_r16_UNBOUND_VALUE = 0,
	ResourceReservationConfigDL_r16_slotBitmap_r16_slotPattern10ms = 1,
	ResourceReservationConfigDL_r16_slotBitmap_r16_slotPattern40ms = 2,
};

union ResourceReservationConfigDL_r16_slotBitmap_r16_Value {
	B20 slotPattern10ms;
	B80 slotPattern40ms;
};

struct ResourceReservationConfigDL_r16_slotBitmap_r16 {
	enum ResourceReservationConfigDL_r16_slotBitmap_r16_Sel d;
	union ResourceReservationConfigDL_r16_slotBitmap_r16_Value v;
};

struct ResourceReservationConfigDL_r16_resourceReservationFreq_r16_resourceReservationFreq_r16_Optional {
	bool d;
	struct ResourceReservationConfigDL_r16_resourceReservationFreq_r16 v;
};

struct B7_ResourceReservationConfigDL_r16_symbolBitmap1_r16_Optional {
	bool d;
	B7 v;
};

struct B7_ResourceReservationConfigDL_r16_symbolBitmap2_r16_Optional {
	bool d;
	B7 v;
};

struct ResourceReservationConfigDL_r16 {
	struct PeriodicityStartPos_r16 periodicityStartPos_r16;
	struct ResourceReservationConfigDL_r16_resourceReservationFreq_r16_resourceReservationFreq_r16_Optional resourceReservationFreq_r16;
	struct ResourceReservationConfigDL_r16_slotBitmap_r16 slotBitmap_r16;
	struct B7_ResourceReservationConfigDL_r16_symbolBitmap1_r16_Optional symbolBitmap1_r16;
	struct B7_ResourceReservationConfigDL_r16_symbolBitmap2_r16_Optional symbolBitmap2_r16;
};

enum ResourceReservationConfigUL_r16_slotBitmap_r16_Sel {
	ResourceReservationConfigUL_r16_slotBitmap_r16_UNBOUND_VALUE = 0,
	ResourceReservationConfigUL_r16_slotBitmap_r16_slotPattern10ms = 1,
	ResourceReservationConfigUL_r16_slotBitmap_r16_slotPattern40ms = 2,
};

union ResourceReservationConfigUL_r16_slotBitmap_r16_Value {
	B20 slotPattern10ms;
	B80 slotPattern40ms;
};

struct ResourceReservationConfigUL_r16_slotBitmap_r16 {
	enum ResourceReservationConfigUL_r16_slotBitmap_r16_Sel d;
	union ResourceReservationConfigUL_r16_slotBitmap_r16_Value v;
};

struct ResourceReservationConfigUL_r16_slotBitmap_r16_slotBitmap_r16_Optional {
	bool d;
	struct ResourceReservationConfigUL_r16_slotBitmap_r16 v;
};

struct B7_ResourceReservationConfigUL_r16_symbolBitmap1_r16_Optional {
	bool d;
	B7 v;
};

struct B7_ResourceReservationConfigUL_r16_symbolBitmap2_r16_Optional {
	bool d;
	B7 v;
};

struct ResourceReservationConfigUL_r16 {
	struct PeriodicityStartPos_r16 periodicityStartPos_r16;
	struct ResourceReservationConfigUL_r16_slotBitmap_r16_slotBitmap_r16_Optional slotBitmap_r16;
	struct B7_ResourceReservationConfigUL_r16_symbolBitmap1_r16_Optional symbolBitmap1_r16;
	struct B7_ResourceReservationConfigUL_r16_symbolBitmap2_r16_Optional symbolBitmap2_r16;
};

struct ResourceReservationConfigDL_r16_SystemInformationBlockType29_r16_resourceReservationConfigCommonDL_r16_Optional {
	bool d;
	struct ResourceReservationConfigDL_r16 v;
};

struct ResourceReservationConfigUL_r16_SystemInformationBlockType29_r16_resourceReservationConfigCommonUL_r16_Optional {
	bool d;
	struct ResourceReservationConfigUL_r16 v;
};

struct OCTET_STRING_SystemInformationBlockType29_r16_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType29_r16 {
	struct ResourceReservationConfigDL_r16_SystemInformationBlockType29_r16_resourceReservationConfigCommonDL_r16_Optional resourceReservationConfigCommonDL_r16;
	struct ResourceReservationConfigUL_r16_SystemInformationBlockType29_r16_resourceReservationConfigCommonUL_r16_Optional resourceReservationConfigCommonUL_r16;
	struct OCTET_STRING_SystemInformationBlockType29_r16_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum SystemInformation_r8_IEs_sib_TypeAndInfo_s_Sel {
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_UNBOUND_VALUE = 0,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib2 = 1,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib3 = 2,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib4 = 3,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib5 = 4,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib6 = 5,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib7 = 6,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib8 = 7,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib9 = 8,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib10 = 9,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib11 = 10,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib12_v920 = 11,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib13_v920 = 12,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib14_v1130 = 13,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib15_v1130 = 14,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib16_v1130 = 15,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib17_v1250 = 16,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib18_v1250 = 17,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib19_v1250 = 18,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib20_v1310 = 19,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib21_v1430 = 20,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib24_v1530 = 21,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib25_v1530 = 22,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib26_v1530 = 23,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib26a_v1610 = 24,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib27_v1610 = 25,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib28_v1610 = 26,
	SystemInformation_r8_IEs_sib_TypeAndInfo_s_sib29_v1610 = 27,
};

union SystemInformation_r8_IEs_sib_TypeAndInfo_s_Value {
	struct SystemInformationBlockType2 sib2;
	struct SystemInformationBlockType3 sib3;
	struct SystemInformationBlockType4 sib4;
	struct SystemInformationBlockType5 sib5;
	struct SystemInformationBlockType6 sib6;
	struct SystemInformationBlockType7 sib7;
	struct SystemInformationBlockType8 sib8;
	struct SystemInformationBlockType9 sib9;
	struct SystemInformationBlockType10 sib10;
	struct SystemInformationBlockType11 sib11;
	struct SystemInformationBlockType12_r9 sib12_v920;
	struct SystemInformationBlockType13_r9 sib13_v920;
	struct SystemInformationBlockType14_r11 sib14_v1130;
	struct SystemInformationBlockType15_r11 sib15_v1130;
	struct SystemInformationBlockType16_r11 sib16_v1130;
	struct SystemInformationBlockType17_r12 sib17_v1250;
	struct SystemInformationBlockType18_r12 sib18_v1250;
	struct SystemInformationBlockType19_r12 sib19_v1250;
	struct SystemInformationBlockType20_r13 sib20_v1310;
	struct SystemInformationBlockType21_r14 sib21_v1430;
	struct SystemInformationBlockType24_r15 sib24_v1530;
	struct SystemInformationBlockType25_r15 sib25_v1530;
	struct SystemInformationBlockType26_r15 sib26_v1530;
	struct SystemInformationBlockType26a_r16 sib26a_v1610;
	struct SystemInformationBlockType27_r16 sib27_v1610;
	struct SystemInformationBlockType28_r16 sib28_v1610;
	struct SystemInformationBlockType29_r16 sib29_v1610;
};

struct SystemInformation_r8_IEs_sib_TypeAndInfo_s {
	enum SystemInformation_r8_IEs_sib_TypeAndInfo_s_Sel d;
	union SystemInformation_r8_IEs_sib_TypeAndInfo_s_Value v;
};

struct SystemInformation_r8_IEs_sib_TypeAndInfo_s_SystemInformation_r8_IEs_sib_TypeAndInfo_Dynamic {
	size_t d;
	struct SystemInformation_r8_IEs_sib_TypeAndInfo_s* v;
};

typedef struct SystemInformation_r8_IEs_sib_TypeAndInfo_s_SystemInformation_r8_IEs_sib_TypeAndInfo_Dynamic SystemInformation_r8_IEs_sib_TypeAndInfo;

struct SystemInformation_v8a0_IEs_nonCriticalExtension {
};

struct OCTET_STRING_SystemInformation_v8a0_IEs_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformation_v8a0_IEs_nonCriticalExtension_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformation_v8a0_IEs_nonCriticalExtension v;
};

struct SystemInformation_v8a0_IEs {
	struct OCTET_STRING_SystemInformation_v8a0_IEs_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformation_v8a0_IEs_nonCriticalExtension_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformation_v8a0_IEs_SystemInformation_r8_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformation_v8a0_IEs v;
};

struct SystemInformation_r8_IEs {
	SystemInformation_r8_IEs_sib_TypeAndInfo sib_TypeAndInfo;
	struct SystemInformation_v8a0_IEs_SystemInformation_r8_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct OCTET_STRING_SystemInformationBlockPos_r15_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockPos_r15 {
	OCTET_STRING assistanceDataSIB_Element_r15;
	struct OCTET_STRING_SystemInformationBlockPos_r15_lateNonCriticalExtension_Optional lateNonCriticalExtension;
};

enum PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_Sel {
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_UNBOUND_VALUE = 0,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_1_r15 = 1,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_2_r15 = 2,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_3_r15 = 3,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_4_r15 = 4,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_5_r15 = 5,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_6_r15 = 6,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib1_7_r15 = 7,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_1_r15 = 8,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_2_r15 = 9,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_3_r15 = 10,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_4_r15 = 11,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_5_r15 = 12,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_6_r15 = 13,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_7_r15 = 14,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_8_r15 = 15,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_9_r15 = 16,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_10_r15 = 17,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_11_r15 = 18,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_12_r15 = 19,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_13_r15 = 20,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_14_r15 = 21,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_15_r15 = 22,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_16_r15 = 23,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_17_r15 = 24,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_18_r15 = 25,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib2_19_r15 = 26,
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_posSib3_1_r15 = 27,
};

union PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_Value {
	struct SystemInformationBlockPos_r15 posSib1_1_r15;
	struct SystemInformationBlockPos_r15 posSib1_2_r15;
	struct SystemInformationBlockPos_r15 posSib1_3_r15;
	struct SystemInformationBlockPos_r15 posSib1_4_r15;
	struct SystemInformationBlockPos_r15 posSib1_5_r15;
	struct SystemInformationBlockPos_r15 posSib1_6_r15;
	struct SystemInformationBlockPos_r15 posSib1_7_r15;
	struct SystemInformationBlockPos_r15 posSib2_1_r15;
	struct SystemInformationBlockPos_r15 posSib2_2_r15;
	struct SystemInformationBlockPos_r15 posSib2_3_r15;
	struct SystemInformationBlockPos_r15 posSib2_4_r15;
	struct SystemInformationBlockPos_r15 posSib2_5_r15;
	struct SystemInformationBlockPos_r15 posSib2_6_r15;
	struct SystemInformationBlockPos_r15 posSib2_7_r15;
	struct SystemInformationBlockPos_r15 posSib2_8_r15;
	struct SystemInformationBlockPos_r15 posSib2_9_r15;
	struct SystemInformationBlockPos_r15 posSib2_10_r15;
	struct SystemInformationBlockPos_r15 posSib2_11_r15;
	struct SystemInformationBlockPos_r15 posSib2_12_r15;
	struct SystemInformationBlockPos_r15 posSib2_13_r15;
	struct SystemInformationBlockPos_r15 posSib2_14_r15;
	struct SystemInformationBlockPos_r15 posSib2_15_r15;
	struct SystemInformationBlockPos_r15 posSib2_16_r15;
	struct SystemInformationBlockPos_r15 posSib2_17_r15;
	struct SystemInformationBlockPos_r15 posSib2_18_r15;
	struct SystemInformationBlockPos_r15 posSib2_19_r15;
	struct SystemInformationBlockPos_r15 posSib3_1_r15;
};

struct PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s {
	enum PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_Sel d;
	union PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_Value v;
};

struct PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_Dynamic {
	size_t d;
	struct PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s* v;
};

typedef struct PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_s_PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15_Dynamic PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15;

struct PosSystemInformation_r15_IEs_nonCriticalExtension {
};

struct OCTET_STRING_PosSystemInformation_r15_IEs_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct PosSystemInformation_r15_IEs_nonCriticalExtension_nonCriticalExtension_Optional {
	bool d;
	struct PosSystemInformation_r15_IEs_nonCriticalExtension v;
};

struct PosSystemInformation_r15_IEs {
	PosSystemInformation_r15_IEs_posSIB_TypeAndInfo_r15 posSIB_TypeAndInfo_r15;
	struct OCTET_STRING_PosSystemInformation_r15_IEs_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct PosSystemInformation_r15_IEs_nonCriticalExtension_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_criticalExtensionsFuture {
};

enum SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_Sel {
	SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_UNBOUND_VALUE = 0,
	SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_posSystemInformation_r15 = 1,
	SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_criticalExtensionsFuture = 2,
};

union SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_Value {
	struct PosSystemInformation_r15_IEs posSystemInformation_r15;
	struct SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_criticalExtensionsFuture criticalExtensionsFuture;
};

struct SystemInformation_criticalExtensions_criticalExtensionsFuture_r15 {
	enum SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_Sel d;
	union SystemInformation_criticalExtensions_criticalExtensionsFuture_r15_Value v;
};

enum SystemInformation_criticalExtensions_Sel {
	SystemInformation_criticalExtensions_UNBOUND_VALUE = 0,
	SystemInformation_criticalExtensions_systemInformation_r8 = 1,
	SystemInformation_criticalExtensions_criticalExtensionsFuture_r15 = 2,
};

union SystemInformation_criticalExtensions_Value {
	struct SystemInformation_r8_IEs systemInformation_r8;
	struct SystemInformation_criticalExtensions_criticalExtensionsFuture_r15 criticalExtensionsFuture_r15;
};

struct SystemInformation_criticalExtensions {
	enum SystemInformation_criticalExtensions_Sel d;
	union SystemInformation_criticalExtensions_Value v;
};

struct SystemInformation {
	struct SystemInformation_criticalExtensions criticalExtensions;
};

typedef B16 TrackingAreaCode;

typedef B28 CellIdentity;

enum SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e {
	SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e_barred = 0,
	SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e_notBarred = 1,
};

typedef enum SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e;

enum SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e {
	SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e_allowed = 0,
	SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e_notAllowed = 1,
};

typedef enum SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e;

typedef B27 CSG_Identity;

struct CSG_Identity_SystemInformationBlockType1_cellAccessRelatedInfo_csg_Identity_Optional {
	bool d;
	CSG_Identity v;
};

struct SystemInformationBlockType1_cellAccessRelatedInfo {
	PLMN_IdentityList plmn_IdentityList;
	TrackingAreaCode trackingAreaCode;
	CellIdentity cellIdentity;
	SystemInformationBlockType1_cellAccessRelatedInfo_cellBarred_e cellBarred;
	SystemInformationBlockType1_cellAccessRelatedInfo_intraFreqReselection_e intraFreqReselection;
	bool csg_Indication;
	struct CSG_Identity_SystemInformationBlockType1_cellAccessRelatedInfo_csg_Identity_Optional csg_Identity;
};

struct uint8_t_SystemInformationBlockType1_cellSelectionInfo_q_RxLevMinOffset_Optional {
	bool d;
	uint8_t v;
};

struct SystemInformationBlockType1_cellSelectionInfo {
	Q_RxLevMin q_RxLevMin;
	struct uint8_t_SystemInformationBlockType1_cellSelectionInfo_q_RxLevMinOffset_Optional q_RxLevMinOffset;
};

typedef uint8_t FreqBandIndicator;

enum SI_Periodicity_r12_e {
	SI_Periodicity_r12_e_rf8 = 0,
	SI_Periodicity_r12_e_rf16 = 1,
	SI_Periodicity_r12_e_rf32 = 2,
	SI_Periodicity_r12_e_rf64 = 3,
	SI_Periodicity_r12_e_rf128 = 4,
	SI_Periodicity_r12_e_rf256 = 5,
	SI_Periodicity_r12_e_rf512 = 6,
};

typedef enum SI_Periodicity_r12_e SI_Periodicity_r12_e;

enum SIB_Type_e {
	SIB_Type_e_sibType3 = 0,
	SIB_Type_e_sibType4 = 1,
	SIB_Type_e_sibType5 = 2,
	SIB_Type_e_sibType6 = 3,
	SIB_Type_e_sibType7 = 4,
	SIB_Type_e_sibType8 = 5,
	SIB_Type_e_sibType9 = 6,
	SIB_Type_e_sibType10 = 7,
	SIB_Type_e_sibType11 = 8,
	SIB_Type_e_sibType12_v920 = 9,
	SIB_Type_e_sibType13_v920 = 10,
	SIB_Type_e_sibType14_v1130 = 11,
	SIB_Type_e_sibType15_v1130 = 12,
	SIB_Type_e_sibType16_v1130 = 13,
	SIB_Type_e_sibType17_v1250 = 14,
	SIB_Type_e_sibType18_v1250 = 15,
	SIB_Type_e_sibType19_v1250 = 16,
	SIB_Type_e_sibType20_v1310 = 17,
	SIB_Type_e_sibType21_v1430 = 18,
	SIB_Type_e_sibType24_v1530 = 19,
	SIB_Type_e_sibType25_v1530 = 20,
	SIB_Type_e_sibType26_v1530 = 21,
	SIB_Type_e_sibType26a_v1610 = 22,
	SIB_Type_e_sibType27_v1610 = 23,
	SIB_Type_e_sibType28_v1610 = 24,
	SIB_Type_e_sibType29_v1610 = 25,
};

typedef enum SIB_Type_e SIB_Type_e;

struct SIB_Type_e_SIB_MappingInfo_Dynamic {
	size_t d;
	SIB_Type_e* v;
};

typedef struct SIB_Type_e_SIB_MappingInfo_Dynamic SIB_MappingInfo;

struct SchedulingInfo {
	SI_Periodicity_r12_e si_Periodicity;
	SIB_MappingInfo sib_MappingInfo;
};

struct SchedulingInfo_SchedulingInfoList_Dynamic {
	size_t d;
	struct SchedulingInfo* v;
};

typedef struct SchedulingInfo_SchedulingInfoList_Dynamic SchedulingInfoList;

enum SystemInformationBlockType1_si_WindowLength_e {
	SystemInformationBlockType1_si_WindowLength_e_ms1 = 0,
	SystemInformationBlockType1_si_WindowLength_e_ms2 = 1,
	SystemInformationBlockType1_si_WindowLength_e_ms5 = 2,
	SystemInformationBlockType1_si_WindowLength_e_ms10 = 3,
	SystemInformationBlockType1_si_WindowLength_e_ms15 = 4,
	SystemInformationBlockType1_si_WindowLength_e_ms20 = 5,
	SystemInformationBlockType1_si_WindowLength_e_ms40 = 6,
};

typedef enum SystemInformationBlockType1_si_WindowLength_e SystemInformationBlockType1_si_WindowLength_e;

enum SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e {
	SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e_true = 0,
};

typedef enum SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e;

struct uint8_t_CellSelectionInfo_v920_q_QualMinOffset_r9_Optional {
	bool d;
	uint8_t v;
};

struct CellSelectionInfo_v920 {
	Q_QualMin_r9 q_QualMin_r9;
	struct uint8_t_CellSelectionInfo_v920_q_QualMinOffset_r9_Optional q_QualMinOffset_r9;
};

struct CellSelectionInfo_v1130 {
	Q_QualMin_r9 q_QualMinWB_r11;
};

enum SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e {
	SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e;

struct SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e_category0Allowed_r12_Optional {
	bool d;
	SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e v;
};

struct SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250 {
	struct SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250_category0Allowed_r12_e_category0Allowed_r12_Optional category0Allowed_r12;
};

struct CellSelectionInfo_v1250 {
	Q_QualMin_r9 q_QualMinRSRQ_OnAllSymbols_r12;
};

enum SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e {
	SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e;

enum SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e {
	SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e;

enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms20 = 0,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms40 = 1,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms60 = 2,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms80 = 3,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms120 = 4,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms160 = 5,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_ms200 = 6,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e_spare = 7,
};

typedef enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e;

enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e_everyRF = 0,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e_every2ndRF = 1,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e_every4thRF = 2,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e_every8thRF = 3,
};

typedef enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e;

enum SchedulingInfo_BR_r13_si_TBS_r13_e {
	SchedulingInfo_BR_r13_si_TBS_r13_e_b152 = 0,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b208 = 1,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b256 = 2,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b328 = 3,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b408 = 4,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b504 = 5,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b600 = 6,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b712 = 7,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b808 = 8,
	SchedulingInfo_BR_r13_si_TBS_r13_e_b936 = 9,
};

typedef enum SchedulingInfo_BR_r13_si_TBS_r13_e SchedulingInfo_BR_r13_si_TBS_r13_e;

struct SchedulingInfo_BR_r13 {
	uint8_t si_Narrowband_r13;
	SchedulingInfo_BR_r13_si_TBS_r13_e si_TBS_r13;
};

struct SchedulingInfo_BR_r13_SchedulingInfoList_BR_r13_Dynamic {
	size_t d;
	struct SchedulingInfo_BR_r13* v;
};

typedef struct SchedulingInfo_BR_r13_SchedulingInfoList_BR_r13_Dynamic SchedulingInfoList_BR_r13;

enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Sel {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_UNBOUND_VALUE = 0,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_subframePattern10_r13 = 1,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_subframePattern40_r13 = 2,
};

union SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Value {
	B10 subframePattern10_r13;
	B40 subframePattern40_r13;
};

struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13 {
	enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Sel d;
	union SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Value v;
};

enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e_on = 0,
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e_off = 1,
};

typedef enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e;

enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e;

typedef uint8_t SystemInfoValueTagSI_r13;

struct SystemInfoValueTagSI_r13_SystemInfoValueTagList_r13_Dynamic {
	size_t d;
	SystemInfoValueTagSI_r13* v;
};

typedef struct SystemInfoValueTagSI_r13_SystemInfoValueTagList_r13_Dynamic SystemInfoValueTagList_r13;

struct SchedulingInfoList_BR_r13_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_schedulingInfoList_BR_r13_Optional {
	bool d;
	SchedulingInfoList_BR_r13 v;
};

struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Optional {
	bool d;
	struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13 v;
};

struct B10_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_UplinkSubframeBitmapBR_r13_Optional {
	bool d;
	B10 v;
};

struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e_si_ValidityTime_r13_Optional {
	bool d;
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e v;
};

struct SystemInfoValueTagList_r13_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_systemInfoValueTagList_r13_Optional {
	bool d;
	SystemInfoValueTagList_r13 v;
};

struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13 {
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_WindowLength_BR_r13_e si_WindowLength_BR_r13;
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_RepetitionPattern_r13_e si_RepetitionPattern_r13;
	struct SchedulingInfoList_BR_r13_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_schedulingInfoList_BR_r13_Optional schedulingInfoList_BR_r13;
	struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_fdd_DownlinkOrTddSubframeBitmapBR_r13_Optional fdd_DownlinkOrTddSubframeBitmapBR_r13;
	struct B10_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_fdd_UplinkSubframeBitmapBR_r13_Optional fdd_UplinkSubframeBitmapBR_r13;
	uint8_t startSymbolBR_r13;
	SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_HoppingConfigCommon_r13_e si_HoppingConfigCommon_r13;
	struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_si_ValidityTime_r13_e_si_ValidityTime_r13_Optional si_ValidityTime_r13;
	struct SystemInfoValueTagList_r13_SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_systemInfoValueTagList_r13_Optional systemInfoValueTagList_r13;
};

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e_nb2 = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e_nb4 = 1,
};

typedef enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e;

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int1 = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int2 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int4 = 2,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e_int8 = 3,
};

typedef enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e;

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int1 = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int5 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int10 = 2,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e_int20 = 3,
};

typedef enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e;

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_Sel {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_UNBOUND_VALUE = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13 = 2,
};

union SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_Value {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_FDD_r13_e interval_FDD_r13;
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_TDD_r13_e interval_TDD_r13;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13 {
	enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_Sel d;
	union SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_Value v;
};

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int2 = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int4 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int8 = 2,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e_int16 = 3,
};

typedef enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e;

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int5 = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int10 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int20 = 2,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e_int40 = 3,
};

typedef enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e;

enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_Sel {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_UNBOUND_VALUE = 0,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13 = 1,
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13 = 2,
};

union SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_Value {
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_FDD_r13_e interval_FDD_r13;
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_TDD_r13_e interval_TDD_r13;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13 {
	enum SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_Sel d;
	union SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_Value v;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e_mpdcch_pdsch_HoppingNB_r13_Optional {
	bool d;
	SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e v;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_DLHoppingConfigCommonModeA_r13_Optional {
	bool d;
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13 v;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_DLHoppingConfigCommonModeB_r13_Optional {
	bool d;
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13 v;
};

struct uint8_t_SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingOffset_r13_Optional {
	bool d;
	uint8_t v;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13 {
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingNB_r13_e_mpdcch_pdsch_HoppingNB_r13_Optional mpdcch_pdsch_HoppingNB_r13;
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeA_r13_interval_DLHoppingConfigCommonModeA_r13_Optional interval_DLHoppingConfigCommonModeA_r13;
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_interval_DLHoppingConfigCommonModeB_r13_interval_DLHoppingConfigCommonModeB_r13_Optional interval_DLHoppingConfigCommonModeB_r13;
	struct uint8_t_SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_mpdcch_pdsch_HoppingOffset_r13_Optional mpdcch_pdsch_HoppingOffset_r13;
};

enum SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e {
	SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e;

enum TDD_Config_v1430_specialSubframePatterns_v1430_e {
	TDD_Config_v1430_specialSubframePatterns_v1430_e_ssp10 = 0,
};

typedef enum TDD_Config_v1430_specialSubframePatterns_v1430_e TDD_Config_v1430_specialSubframePatterns_v1430_e;

struct TDD_Config_v1430 {
	TDD_Config_v1430_specialSubframePatterns_v1430_e specialSubframePatterns_v1430;
};

struct CellAccessRelatedInfo_r14 {
	PLMN_IdentityList plmn_IdentityList_r14;
	TrackingAreaCode trackingAreaCode_r14;
	CellIdentity cellIdentity_r14;
};

enum TDD_Config_v1450_specialSubframePatterns_v1450_e {
	TDD_Config_v1450_specialSubframePatterns_v1450_e_ssp10_CRS_LessDwPTS = 0,
};

typedef enum TDD_Config_v1450_specialSubframePatterns_v1450_e TDD_Config_v1450_specialSubframePatterns_v1450_e;

struct TDD_Config_v1450 {
	TDD_Config_v1450_specialSubframePatterns_v1450_e specialSubframePatterns_v1450;
};

enum SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e {
	SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e;

enum SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e {
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e_n6 = 0,
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e_n24 = 1,
};

typedef enum SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e;

enum SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_Sel {
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_UNBOUND_VALUE = 0,
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigEnabled = 1,
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs = 2,
};

union SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_Value {
	Null crs_IntfMitigEnabled;
	SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigNumPRBs_e crs_IntfMitigNumPRBs;
};

struct SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15 {
	enum SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_Sel d;
	union SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_Value v;
};

enum SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e {
	SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e_barred = 0,
	SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e_notBarred = 1,
};

typedef enum SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e;

enum PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e {
	PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e_reserved = 0,
	PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e_notReserved = 1,
};

typedef enum PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e;

struct PLMN_IdentityInfo_v1530 {
	PLMN_IdentityInfo_v1530_cellReservedForOperatorUse_CRS_r15_e cellReservedForOperatorUse_CRS_r15;
};

struct PLMN_IdentityInfo_v1530_PLMN_IdentityList_v1530_Dynamic {
	size_t d;
	struct PLMN_IdentityInfo_v1530* v;
};

typedef struct PLMN_IdentityInfo_v1530_PLMN_IdentityList_v1530_Dynamic PLMN_IdentityList_v1530;

enum PosSchedulingInfo_r15_posSI_Periodicity_r15_e {
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf8 = 0,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf16 = 1,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf32 = 2,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf64 = 3,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf128 = 4,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf256 = 5,
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e_rf512 = 6,
};

typedef enum PosSchedulingInfo_r15_posSI_Periodicity_r15_e PosSchedulingInfo_r15_posSI_Periodicity_r15_e;

enum PosSIB_Type_r15_encrypted_r15_e {
	PosSIB_Type_r15_encrypted_r15_e_true = 0,
};

typedef enum PosSIB_Type_r15_encrypted_r15_e PosSIB_Type_r15_encrypted_r15_e;

enum GNSS_ID_r15_gnss_id_r15_e {
	GNSS_ID_r15_gnss_id_r15_e_gps = 0,
	GNSS_ID_r15_gnss_id_r15_e_sbas = 1,
	GNSS_ID_r15_gnss_id_r15_e_qzss = 2,
	GNSS_ID_r15_gnss_id_r15_e_galileo = 3,
	GNSS_ID_r15_gnss_id_r15_e_glonass = 4,
	GNSS_ID_r15_gnss_id_r15_e_bds = 5,
	GNSS_ID_r15_gnss_id_r15_e_navic_v1610 = 6,
};

typedef enum GNSS_ID_r15_gnss_id_r15_e GNSS_ID_r15_gnss_id_r15_e;

struct GNSS_ID_r15 {
	GNSS_ID_r15_gnss_id_r15_e gnss_id_r15;
};

enum SBAS_ID_r15_sbas_id_r15_e {
	SBAS_ID_r15_sbas_id_r15_e_waas = 0,
	SBAS_ID_r15_sbas_id_r15_e_egnos = 1,
	SBAS_ID_r15_sbas_id_r15_e_msas = 2,
	SBAS_ID_r15_sbas_id_r15_e_gagan = 3,
};

typedef enum SBAS_ID_r15_sbas_id_r15_e SBAS_ID_r15_sbas_id_r15_e;

struct SBAS_ID_r15 {
	SBAS_ID_r15_sbas_id_r15_e sbas_id_r15;
};

enum PosSIB_Type_r15_posSibType_r15_e {
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_1 = 0,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_2 = 1,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_3 = 2,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_4 = 3,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_5 = 4,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_6 = 5,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_7 = 6,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_1 = 7,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_2 = 8,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_3 = 9,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_4 = 10,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_5 = 11,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_6 = 12,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_7 = 13,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_8 = 14,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_9 = 15,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_10 = 16,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_11 = 17,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_12 = 18,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_13 = 19,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_14 = 20,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_15 = 21,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_16 = 22,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_17 = 23,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_18 = 24,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_19 = 25,
	PosSIB_Type_r15_posSibType_r15_e_posSibType3_1 = 26,
	PosSIB_Type_r15_posSibType_r15_e_posSibType1_8_v1610 = 27,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_20_v1610 = 28,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_21_v1610 = 29,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_22_v1610 = 30,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_23_v1610 = 31,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_24_v1610 = 32,
	PosSIB_Type_r15_posSibType_r15_e_posSibType2_25_v1610 = 33,
	PosSIB_Type_r15_posSibType_r15_e_posSibType4_1_v1610 = 34,
	PosSIB_Type_r15_posSibType_r15_e_posSibType5_1_v1610 = 35,
};

typedef enum PosSIB_Type_r15_posSibType_r15_e PosSIB_Type_r15_posSibType_r15_e;

struct PosSIB_Type_r15_encrypted_r15_e_encrypted_r15_Optional {
	bool d;
	PosSIB_Type_r15_encrypted_r15_e v;
};

struct GNSS_ID_r15_PosSIB_Type_r15_gnss_id_r15_Optional {
	bool d;
	struct GNSS_ID_r15 v;
};

struct SBAS_ID_r15_PosSIB_Type_r15_sbas_id_r15_Optional {
	bool d;
	struct SBAS_ID_r15 v;
};

struct PosSIB_Type_r15 {
	struct PosSIB_Type_r15_encrypted_r15_e_encrypted_r15_Optional encrypted_r15;
	struct GNSS_ID_r15_PosSIB_Type_r15_gnss_id_r15_Optional gnss_id_r15;
	struct SBAS_ID_r15_PosSIB_Type_r15_sbas_id_r15_Optional sbas_id_r15;
	PosSIB_Type_r15_posSibType_r15_e posSibType_r15;
};

struct PosSIB_Type_r15_PosSIB_MappingInfo_r15_Dynamic {
	size_t d;
	struct PosSIB_Type_r15* v;
};

typedef struct PosSIB_Type_r15_PosSIB_MappingInfo_r15_Dynamic PosSIB_MappingInfo_r15;

struct PosSchedulingInfo_r15 {
	PosSchedulingInfo_r15_posSI_Periodicity_r15_e posSI_Periodicity_r15;
	PosSIB_MappingInfo_r15 posSIB_MappingInfo_r15;
};

struct PosSchedulingInfo_r15_PosSchedulingInfoList_r15_Dynamic {
	size_t d;
	struct PosSchedulingInfo_r15* v;
};

typedef struct PosSchedulingInfo_r15_PosSchedulingInfoList_r15_Dynamic PosSchedulingInfoList_r15;

enum SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e {
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e_barred = 0,
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e_notBarred = 1,
};

typedef enum SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e;

enum SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e {
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e_barred = 0,
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e_notBarred = 1,
};

typedef enum SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e;

enum PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_Sel {
	PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_UNBOUND_VALUE = 0,
	PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_plmn_Identity_r15 = 1,
	PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_plmn_Index_r15 = 2,
};

union PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_Value {
	struct PLMN_Identity plmn_Identity_r15;
	uint8_t plmn_Index_r15;
};

struct PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15 {
	enum PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_Sel d;
	union PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15_Value v;
};

enum PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e {
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e_reserved = 0,
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e_notReserved = 1,
};

typedef enum PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e;

enum PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e {
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e_reserved = 0,
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e_notReserved = 1,
};

typedef enum PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e;

struct PLMN_IdentityInfo_r15 {
	struct PLMN_IdentityInfo_r15_plmn_Identity_5GC_r15 plmn_Identity_5GC_r15;
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_r15_e cellReservedForOperatorUse_r15;
	PLMN_IdentityInfo_r15_cellReservedForOperatorUse_CRS_r15_e cellReservedForOperatorUse_CRS_r15;
};

struct PLMN_IdentityInfo_r15_PLMN_IdentityList_r15_Dynamic {
	size_t d;
	struct PLMN_IdentityInfo_r15* v;
};

typedef struct PLMN_IdentityInfo_r15_PLMN_IdentityList_r15_Dynamic PLMN_IdentityList_r15;

typedef uint8_t RAN_AreaCode_r15;

typedef B24 TrackingAreaCode_5GC_r15;

enum CellIdentity_5GC_r15_Sel {
	CellIdentity_5GC_r15_UNBOUND_VALUE = 0,
	CellIdentity_5GC_r15_cellIdentity_r15 = 1,
	CellIdentity_5GC_r15_cellId_Index_r15 = 2,
};

union CellIdentity_5GC_r15_Value {
	CellIdentity cellIdentity_r15;
	uint8_t cellId_Index_r15;
};

struct CellIdentity_5GC_r15 {
	enum CellIdentity_5GC_r15_Sel d;
	union CellIdentity_5GC_r15_Value v;
};

struct RAN_AreaCode_r15_CellAccessRelatedInfo_5GC_r15_ran_AreaCode_r15_Optional {
	bool d;
	RAN_AreaCode_r15 v;
};

struct CellAccessRelatedInfo_5GC_r15 {
	PLMN_IdentityList_r15 plmn_IdentityList_r15;
	struct RAN_AreaCode_r15_CellAccessRelatedInfo_5GC_r15_ran_AreaCode_r15_Optional ran_AreaCode_r15;
	TrackingAreaCode_5GC_r15 trackingAreaCode_5GC_r15;
	struct CellIdentity_5GC_r15 cellIdentity_5GC_r15;
};

struct CellAccessRelatedInfo_5GC_r15_SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellAccessRelatedInfoList_5GC_r15_Dynamic {
	size_t d;
	struct CellAccessRelatedInfo_5GC_r15* v;
};

struct SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15 {
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_r15_e cellBarred_5GC_r15;
	SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellBarred_5GC_CRS_r15_e cellBarred_5GC_CRS_r15;
	struct CellAccessRelatedInfo_5GC_r15_SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellAccessRelatedInfoList_5GC_r15_Dynamic cellAccessRelatedInfoList_5GC_r15;
};

enum SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e {
	SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e;

enum SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e {
	SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e;

enum SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e {
	SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e;

enum SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e {
	SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e;

enum SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e {
	SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e;

enum SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e {
	SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e_true = 0,
};

typedef enum SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e;

enum PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e {
	PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e_true = 0,
};

typedef enum PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e;

enum PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e {
	PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e_true = 0,
};

typedef enum PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e;

enum PLMN_IdentityInfo_v1610_iab_Support_r16_e {
	PLMN_IdentityInfo_v1610_iab_Support_r16_e_true = 0,
};

typedef enum PLMN_IdentityInfo_v1610_iab_Support_r16_e PLMN_IdentityInfo_v1610_iab_Support_r16_e;

struct PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e_cp_CIoT_5GS_Optimisation_r16_Optional {
	bool d;
	PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e v;
};

struct PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e_up_CIoT_5GS_Optimisation_r16_Optional {
	bool d;
	PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e v;
};

struct PLMN_IdentityInfo_v1610_iab_Support_r16_e_iab_Support_r16_Optional {
	bool d;
	PLMN_IdentityInfo_v1610_iab_Support_r16_e v;
};

struct PLMN_IdentityInfo_v1610 {
	struct PLMN_IdentityInfo_v1610_cp_CIoT_5GS_Optimisation_r16_e_cp_CIoT_5GS_Optimisation_r16_Optional cp_CIoT_5GS_Optimisation_r16;
	struct PLMN_IdentityInfo_v1610_up_CIoT_5GS_Optimisation_r16_e_up_CIoT_5GS_Optimisation_r16_Optional up_CIoT_5GS_Optimisation_r16;
	struct PLMN_IdentityInfo_v1610_iab_Support_r16_e_iab_Support_r16_Optional iab_Support_r16;
};

struct PLMN_IdentityInfo_v1610_PLMN_IdentityList_v1610_Dynamic {
	size_t d;
	struct PLMN_IdentityInfo_v1610* v;
};

typedef struct PLMN_IdentityInfo_v1610_PLMN_IdentityList_v1610_Dynamic PLMN_IdentityList_v1610;

struct SystemInformationBlockType1_v1610_IEs_nonCriticalExtension {
};

struct SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e_eDRX_Allowed_5GC_r16_Optional {
	bool d;
	SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e v;
};

struct SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e_transmissionInControlChRegion_r16_Optional {
	bool d;
	SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e v;
};

struct SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e_campingAllowedInCE_r16_Optional {
	bool d;
	SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e v;
};

struct PLMN_IdentityList_v1610_SystemInformationBlockType1_v1610_IEs_plmn_IdentityList_v1610_Optional {
	bool d;
	PLMN_IdentityList_v1610 v;
};

struct SystemInformationBlockType1_v1610_IEs_nonCriticalExtension_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1610_IEs_nonCriticalExtension v;
};

struct SystemInformationBlockType1_v1610_IEs {
	struct SystemInformationBlockType1_v1610_IEs_eDRX_Allowed_5GC_r16_e_eDRX_Allowed_5GC_r16_Optional eDRX_Allowed_5GC_r16;
	struct SystemInformationBlockType1_v1610_IEs_transmissionInControlChRegion_r16_e_transmissionInControlChRegion_r16_Optional transmissionInControlChRegion_r16;
	struct SystemInformationBlockType1_v1610_IEs_campingAllowedInCE_r16_e_campingAllowedInCE_r16_Optional campingAllowedInCE_r16;
	struct PLMN_IdentityList_v1610_SystemInformationBlockType1_v1610_IEs_plmn_IdentityList_v1610_Optional plmn_IdentityList_v1610;
	struct SystemInformationBlockType1_v1610_IEs_nonCriticalExtension_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e_si_posOffset_r15_Optional {
	bool d;
	SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e v;
};

struct SystemInformationBlockType1_v1610_IEs_SystemInformationBlockType1_v1540_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1610_IEs v;
};

struct SystemInformationBlockType1_v1540_IEs {
	struct SystemInformationBlockType1_v1540_IEs_si_posOffset_r15_e_si_posOffset_r15_Optional si_posOffset_r15;
	struct SystemInformationBlockType1_v1610_IEs_SystemInformationBlockType1_v1540_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e_hsdn_Cell_r15_Optional {
	bool d;
	SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e v;
};

struct CellSelectionInfoCE_v1530_SystemInformationBlockType1_v1530_IEs_cellSelectionInfoCE_v1530_Optional {
	bool d;
	struct CellSelectionInfoCE_v1530 v;
};

struct SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigConfig_r15_Optional {
	bool d;
	struct SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15 v;
};

struct PLMN_IdentityList_v1530_SystemInformationBlockType1_v1530_IEs_plmn_IdentityList_v1530_Optional {
	bool d;
	PLMN_IdentityList_v1530 v;
};

struct PosSchedulingInfoList_r15_SystemInformationBlockType1_v1530_IEs_posSchedulingInfoList_r15_Optional {
	bool d;
	PosSchedulingInfoList_r15 v;
};

struct SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellAccessRelatedInfo_5GC_r15_Optional {
	bool d;
	struct SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15 v;
};

struct SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e_ims_EmergencySupport5GC_r15_Optional {
	bool d;
	SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e v;
};

struct SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e_eCallOverIMS_Support5GC_r15_Optional {
	bool d;
	SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e v;
};

struct SystemInformationBlockType1_v1540_IEs_SystemInformationBlockType1_v1530_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1540_IEs v;
};

struct SystemInformationBlockType1_v1530_IEs {
	struct SystemInformationBlockType1_v1530_IEs_hsdn_Cell_r15_e_hsdn_Cell_r15_Optional hsdn_Cell_r15;
	struct CellSelectionInfoCE_v1530_SystemInformationBlockType1_v1530_IEs_cellSelectionInfoCE_v1530_Optional cellSelectionInfoCE_v1530;
	struct SystemInformationBlockType1_v1530_IEs_crs_IntfMitigConfig_r15_crs_IntfMitigConfig_r15_Optional crs_IntfMitigConfig_r15;
	SystemInformationBlockType1_v1530_IEs_cellBarred_CRS_r15_e cellBarred_CRS_r15;
	struct PLMN_IdentityList_v1530_SystemInformationBlockType1_v1530_IEs_plmn_IdentityList_v1530_Optional plmn_IdentityList_v1530;
	struct PosSchedulingInfoList_r15_SystemInformationBlockType1_v1530_IEs_posSchedulingInfoList_r15_Optional posSchedulingInfoList_r15;
	struct SystemInformationBlockType1_v1530_IEs_cellAccessRelatedInfo_5GC_r15_cellAccessRelatedInfo_5GC_r15_Optional cellAccessRelatedInfo_5GC_r15;
	struct SystemInformationBlockType1_v1530_IEs_ims_EmergencySupport5GC_r15_e_ims_EmergencySupport5GC_r15_Optional ims_EmergencySupport5GC_r15;
	struct SystemInformationBlockType1_v1530_IEs_eCallOverIMS_Support5GC_r15_e_eCallOverIMS_Support5GC_r15_Optional eCallOverIMS_Support5GC_r15;
	struct SystemInformationBlockType1_v1540_IEs_SystemInformationBlockType1_v1530_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct TDD_Config_v1450_SystemInformationBlockType1_v1450_IEs_tdd_Config_v1450_Optional {
	bool d;
	struct TDD_Config_v1450 v;
};

struct SystemInformationBlockType1_v1530_IEs_SystemInformationBlockType1_v1450_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1530_IEs v;
};

struct SystemInformationBlockType1_v1450_IEs {
	struct TDD_Config_v1450_SystemInformationBlockType1_v1450_IEs_tdd_Config_v1450_Optional tdd_Config_v1450;
	struct SystemInformationBlockType1_v1530_IEs_SystemInformationBlockType1_v1450_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e_eCallOverIMS_Support_r14_Optional {
	bool d;
	SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e v;
};

struct TDD_Config_v1430_SystemInformationBlockType1_v1430_IEs_tdd_Config_v1430_Optional {
	bool d;
	struct TDD_Config_v1430 v;
};

struct SystemInformationBlockType1_v1430_IEs_CellAccessRelatedInfo_r14_cellAccessRelatedInfoList_r14_Dynamic {
	size_t d;
	struct CellAccessRelatedInfo_r14* v;
};

struct CellAccessRelatedInfo_r14_SystemInformationBlockType1_v1430_IEs_cellAccessRelatedInfoList_r14_DynamicOptional {
	bool d;
	struct SystemInformationBlockType1_v1430_IEs_CellAccessRelatedInfo_r14_cellAccessRelatedInfoList_r14_Dynamic v;
};

struct SystemInformationBlockType1_v1450_IEs_SystemInformationBlockType1_v1430_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1450_IEs v;
};

struct SystemInformationBlockType1_v1430_IEs {
	struct SystemInformationBlockType1_v1430_IEs_eCallOverIMS_Support_r14_e_eCallOverIMS_Support_r14_Optional eCallOverIMS_Support_r14;
	struct TDD_Config_v1430_SystemInformationBlockType1_v1430_IEs_tdd_Config_v1430_Optional tdd_Config_v1430;
	struct CellAccessRelatedInfo_r14_SystemInformationBlockType1_v1430_IEs_cellAccessRelatedInfoList_r14_DynamicOptional cellAccessRelatedInfoList_r14;
	struct SystemInformationBlockType1_v1450_IEs_SystemInformationBlockType1_v1430_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct CellSelectionInfoCE1_v1360_SystemInformationBlockType1_v1360_IEs_cellSelectionInfoCE1_v1360_Optional {
	bool d;
	struct CellSelectionInfoCE1_v1360 v;
};

struct SystemInformationBlockType1_v1430_IEs_SystemInformationBlockType1_v1360_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1430_IEs v;
};

struct SystemInformationBlockType1_v1360_IEs {
	struct CellSelectionInfoCE1_v1360_SystemInformationBlockType1_v1360_IEs_cellSelectionInfoCE1_v1360_Optional cellSelectionInfoCE1_v1360;
	struct SystemInformationBlockType1_v1430_IEs_SystemInformationBlockType1_v1360_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct CellSelectionInfoCE1_r13_SystemInformationBlockType1_v1350_IEs_cellSelectionInfoCE1_r13_Optional {
	bool d;
	struct CellSelectionInfoCE1_r13 v;
};

struct SystemInformationBlockType1_v1360_IEs_SystemInformationBlockType1_v1350_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1360_IEs v;
};

struct SystemInformationBlockType1_v1350_IEs {
	struct CellSelectionInfoCE1_r13_SystemInformationBlockType1_v1350_IEs_cellSelectionInfoCE1_r13_Optional cellSelectionInfoCE1_r13;
	struct SystemInformationBlockType1_v1360_IEs_SystemInformationBlockType1_v1350_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_freqHoppingParametersDL_r13_Optional {
	bool d;
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13 v;
};

struct SystemInformationBlockType1_v1350_IEs_SystemInformationBlockType1_v1320_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1350_IEs v;
};

struct SystemInformationBlockType1_v1320_IEs {
	struct SystemInformationBlockType1_v1320_IEs_freqHoppingParametersDL_r13_freqHoppingParametersDL_r13_Optional freqHoppingParametersDL_r13;
	struct SystemInformationBlockType1_v1350_IEs_SystemInformationBlockType1_v1320_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct B10_SystemInformationBlockType1_v1310_IEs_hyperSFN_r13_Optional {
	bool d;
	B10 v;
};

struct SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e_eDRX_Allowed_r13_Optional {
	bool d;
	SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e v;
};

struct CellSelectionInfoCE_r13_SystemInformationBlockType1_v1310_IEs_cellSelectionInfoCE_r13_Optional {
	bool d;
	struct CellSelectionInfoCE_r13 v;
};

struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_bandwidthReducedAccessRelatedInfo_r13_Optional {
	bool d;
	struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13 v;
};

struct SystemInformationBlockType1_v1320_IEs_SystemInformationBlockType1_v1310_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1320_IEs v;
};

struct SystemInformationBlockType1_v1310_IEs {
	struct B10_SystemInformationBlockType1_v1310_IEs_hyperSFN_r13_Optional hyperSFN_r13;
	struct SystemInformationBlockType1_v1310_IEs_eDRX_Allowed_r13_e_eDRX_Allowed_r13_Optional eDRX_Allowed_r13;
	struct CellSelectionInfoCE_r13_SystemInformationBlockType1_v1310_IEs_cellSelectionInfoCE_r13_Optional cellSelectionInfoCE_r13;
	struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13_bandwidthReducedAccessRelatedInfo_r13_Optional bandwidthReducedAccessRelatedInfo_r13;
	struct SystemInformationBlockType1_v1320_IEs_SystemInformationBlockType1_v1310_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct CellSelectionInfo_v1250_SystemInformationBlockType1_v1250_IEs_cellSelectionInfo_v1250_Optional {
	bool d;
	struct CellSelectionInfo_v1250 v;
};

struct SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e_freqBandIndicatorPriority_r12_Optional {
	bool d;
	SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e v;
};

struct SystemInformationBlockType1_v1310_IEs_SystemInformationBlockType1_v1250_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1310_IEs v;
};

struct SystemInformationBlockType1_v1250_IEs {
	struct SystemInformationBlockType1_v1250_IEs_cellAccessRelatedInfo_v1250 cellAccessRelatedInfo_v1250;
	struct CellSelectionInfo_v1250_SystemInformationBlockType1_v1250_IEs_cellSelectionInfo_v1250_Optional cellSelectionInfo_v1250;
	struct SystemInformationBlockType1_v1250_IEs_freqBandIndicatorPriority_r12_e_freqBandIndicatorPriority_r12_Optional freqBandIndicatorPriority_r12;
	struct SystemInformationBlockType1_v1310_IEs_SystemInformationBlockType1_v1250_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct TDD_Config_v1130_SystemInformationBlockType1_v1130_IEs_tdd_Config_v1130_Optional {
	bool d;
	struct TDD_Config_v1130 v;
};

struct CellSelectionInfo_v1130_SystemInformationBlockType1_v1130_IEs_cellSelectionInfo_v1130_Optional {
	bool d;
	struct CellSelectionInfo_v1130 v;
};

struct SystemInformationBlockType1_v1250_IEs_SystemInformationBlockType1_v1130_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1250_IEs v;
};

struct SystemInformationBlockType1_v1130_IEs {
	struct TDD_Config_v1130_SystemInformationBlockType1_v1130_IEs_tdd_Config_v1130_Optional tdd_Config_v1130;
	struct CellSelectionInfo_v1130_SystemInformationBlockType1_v1130_IEs_cellSelectionInfo_v1130_Optional cellSelectionInfo_v1130;
	struct SystemInformationBlockType1_v1250_IEs_SystemInformationBlockType1_v1130_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e_ims_EmergencySupport_r9_Optional {
	bool d;
	SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e v;
};

struct CellSelectionInfo_v920_SystemInformationBlockType1_v920_IEs_cellSelectionInfo_v920_Optional {
	bool d;
	struct CellSelectionInfo_v920 v;
};

struct SystemInformationBlockType1_v1130_IEs_SystemInformationBlockType1_v920_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v1130_IEs v;
};

struct SystemInformationBlockType1_v920_IEs {
	struct SystemInformationBlockType1_v920_IEs_ims_EmergencySupport_r9_e_ims_EmergencySupport_r9_Optional ims_EmergencySupport_r9;
	struct CellSelectionInfo_v920_SystemInformationBlockType1_v920_IEs_cellSelectionInfo_v920_Optional cellSelectionInfo_v920;
	struct SystemInformationBlockType1_v1130_IEs_SystemInformationBlockType1_v920_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct OCTET_STRING_SystemInformationBlockType1_v890_IEs_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct SystemInformationBlockType1_v920_IEs_SystemInformationBlockType1_v890_IEs_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v920_IEs v;
};

struct SystemInformationBlockType1_v890_IEs {
	struct OCTET_STRING_SystemInformationBlockType1_v890_IEs_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct SystemInformationBlockType1_v920_IEs_SystemInformationBlockType1_v890_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct P_Max_SystemInformationBlockType1_p_Max_Optional {
	bool d;
	P_Max v;
};

struct TDD_Config_SystemInformationBlockType1_tdd_Config_Optional {
	bool d;
	struct TDD_Config v;
};

struct SystemInformationBlockType1_v890_IEs_SystemInformationBlockType1_nonCriticalExtension_Optional {
	bool d;
	struct SystemInformationBlockType1_v890_IEs v;
};

struct SystemInformationBlockType1 {
	struct SystemInformationBlockType1_cellAccessRelatedInfo cellAccessRelatedInfo;
	struct SystemInformationBlockType1_cellSelectionInfo cellSelectionInfo;
	struct P_Max_SystemInformationBlockType1_p_Max_Optional p_Max;
	FreqBandIndicator freqBandIndicator;
	SchedulingInfoList schedulingInfoList;
	struct TDD_Config_SystemInformationBlockType1_tdd_Config_Optional tdd_Config;
	SystemInformationBlockType1_si_WindowLength_e si_WindowLength;
	uint8_t systemInfoValueTag;
	struct SystemInformationBlockType1_v890_IEs_SystemInformationBlockType1_nonCriticalExtension_Optional nonCriticalExtension;
};

enum BCCH_DL_SCH_MessageType_c1_Sel {
	BCCH_DL_SCH_MessageType_c1_UNBOUND_VALUE = 0,
	BCCH_DL_SCH_MessageType_c1_systemInformation = 1,
	BCCH_DL_SCH_MessageType_c1_systemInformationBlockType1 = 2,
};

union BCCH_DL_SCH_MessageType_c1_Value {
	struct SystemInformation systemInformation;
	struct SystemInformationBlockType1 systemInformationBlockType1;
};

struct BCCH_DL_SCH_MessageType_c1 {
	enum BCCH_DL_SCH_MessageType_c1_Sel d;
	union BCCH_DL_SCH_MessageType_c1_Value v;
};

struct BCCH_DL_SCH_MessageType_messageClassExtension {
};

enum BCCH_DL_SCH_MessageType_Sel {
	BCCH_DL_SCH_MessageType_UNBOUND_VALUE = 0,
	BCCH_DL_SCH_MessageType_c1 = 1,
	BCCH_DL_SCH_MessageType_messageClassExtension = 2,
};

union BCCH_DL_SCH_MessageType_Value {
	struct BCCH_DL_SCH_MessageType_c1 c1;
	struct BCCH_DL_SCH_MessageType_messageClassExtension messageClassExtension;
};

struct BCCH_DL_SCH_MessageType {
	enum BCCH_DL_SCH_MessageType_Sel d;
	union BCCH_DL_SCH_MessageType_Value v;
};

struct BCCH_DL_SCH_Message {
	struct BCCH_DL_SCH_MessageType message;
};

enum BCCH_DL_SCH_MessageType_BR_r13_c1_Sel {
	BCCH_DL_SCH_MessageType_BR_r13_c1_UNBOUND_VALUE = 0,
	BCCH_DL_SCH_MessageType_BR_r13_c1_systemInformation_BR_r13 = 1,
	BCCH_DL_SCH_MessageType_BR_r13_c1_systemInformationBlockType1_BR_r13 = 2,
};

union BCCH_DL_SCH_MessageType_BR_r13_c1_Value {
	struct SystemInformation systemInformation_BR_r13;
	struct SystemInformationBlockType1 systemInformationBlockType1_BR_r13;
};

struct BCCH_DL_SCH_MessageType_BR_r13_c1 {
	enum BCCH_DL_SCH_MessageType_BR_r13_c1_Sel d;
	union BCCH_DL_SCH_MessageType_BR_r13_c1_Value v;
};

struct BCCH_DL_SCH_MessageType_BR_r13_messageClassExtension {
};

enum BCCH_DL_SCH_MessageType_BR_r13_Sel {
	BCCH_DL_SCH_MessageType_BR_r13_UNBOUND_VALUE = 0,
	BCCH_DL_SCH_MessageType_BR_r13_c1 = 1,
	BCCH_DL_SCH_MessageType_BR_r13_messageClassExtension = 2,
};

union BCCH_DL_SCH_MessageType_BR_r13_Value {
	struct BCCH_DL_SCH_MessageType_BR_r13_c1 c1;
	struct BCCH_DL_SCH_MessageType_BR_r13_messageClassExtension messageClassExtension;
};

struct BCCH_DL_SCH_MessageType_BR_r13 {
	enum BCCH_DL_SCH_MessageType_BR_r13_Sel d;
	union BCCH_DL_SCH_MessageType_BR_r13_Value v;
};

struct BCCH_DL_SCH_Message_BR {
	struct BCCH_DL_SCH_MessageType_BR_r13 message;
};

typedef B16 C_RNTI;

enum CQI_ReportModeAperiodic_e {
	CQI_ReportModeAperiodic_e_rm12 = 0,
	CQI_ReportModeAperiodic_e_rm20 = 1,
	CQI_ReportModeAperiodic_e_rm22 = 2,
	CQI_ReportModeAperiodic_e_rm30 = 3,
	CQI_ReportModeAperiodic_e_rm31 = 4,
	CQI_ReportModeAperiodic_e_rm32_v1250 = 5,
	CQI_ReportModeAperiodic_e_rm10_v1310 = 6,
	CQI_ReportModeAperiodic_e_rm11_v1310 = 7,
};

typedef enum CQI_ReportModeAperiodic_e CQI_ReportModeAperiodic_e;

struct CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_subbandCQI {
	uint8_t k;
};

enum CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_Sel {
	CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_UNBOUND_VALUE = 0,
	CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_widebandCQI = 1,
	CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_subbandCQI = 2,
};

union CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_Value {
	Null widebandCQI;
	struct CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_subbandCQI subbandCQI;
};

struct CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic {
	enum CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_Sel d;
	union CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_Value v;
};

struct uint16_t_CQI_ReportPeriodic_setup_ri_ConfigIndex_Optional {
	bool d;
	uint16_t v;
};

struct CQI_ReportPeriodic_setup {
	uint16_t cqi_PUCCH_ResourceIndex;
	uint16_t cqi_pmi_ConfigIndex;
	struct CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic cqi_FormatIndicatorPeriodic;
	struct uint16_t_CQI_ReportPeriodic_setup_ri_ConfigIndex_Optional ri_ConfigIndex;
	bool simultaneousAckNackAndCQI;
};

enum CQI_ReportPeriodic_Sel {
	CQI_ReportPeriodic_UNBOUND_VALUE = 0,
	CQI_ReportPeriodic_release = 1,
	CQI_ReportPeriodic_setup = 2,
};

union CQI_ReportPeriodic_Value {
	Null release;
	struct CQI_ReportPeriodic_setup setup;
};

struct CQI_ReportPeriodic {
	enum CQI_ReportPeriodic_Sel d;
	union CQI_ReportPeriodic_Value v;
};

struct CQI_ReportModeAperiodic_e_CQI_ReportConfig_cqi_ReportModeAperiodic_Optional {
	bool d;
	CQI_ReportModeAperiodic_e v;
};

struct CQI_ReportPeriodic_CQI_ReportConfig_cqi_ReportPeriodic_Optional {
	bool d;
	struct CQI_ReportPeriodic v;
};

struct CQI_ReportConfig {
	struct CQI_ReportModeAperiodic_e_CQI_ReportConfig_cqi_ReportModeAperiodic_Optional cqi_ReportModeAperiodic;
	int8_t nomPDSCH_RS_EPRE_Offset;
	struct CQI_ReportPeriodic_CQI_ReportConfig_cqi_ReportPeriodic_Optional cqi_ReportPeriodic;
};

struct CQI_ReportAperiodic_r10_setup_aperiodicCSI_Trigger_r10 {
	B8 trigger1_r10;
	B8 trigger2_r10;
};

struct CQI_ReportAperiodic_r10_setup_aperiodicCSI_Trigger_r10_aperiodicCSI_Trigger_r10_Optional {
	bool d;
	struct CQI_ReportAperiodic_r10_setup_aperiodicCSI_Trigger_r10 v;
};

struct CQI_ReportAperiodic_r10_setup {
	CQI_ReportModeAperiodic_e cqi_ReportModeAperiodic_r10;
	struct CQI_ReportAperiodic_r10_setup_aperiodicCSI_Trigger_r10_aperiodicCSI_Trigger_r10_Optional aperiodicCSI_Trigger_r10;
};

enum CQI_ReportAperiodic_r10_Sel {
	CQI_ReportAperiodic_r10_UNBOUND_VALUE = 0,
	CQI_ReportAperiodic_r10_release = 1,
	CQI_ReportAperiodic_r10_setup = 2,
};

union CQI_ReportAperiodic_r10_Value {
	Null release;
	struct CQI_ReportAperiodic_r10_setup setup;
};

struct CQI_ReportAperiodic_r10 {
	enum CQI_ReportAperiodic_r10_Sel d;
	union CQI_ReportAperiodic_r10_Value v;
};

enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e {
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e_submode1 = 0,
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e_submode2 = 1,
};

typedef enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e;

struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e_csi_ReportMode_r10_Optional {
	bool d;
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e v;
};

struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10 {
	struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10_csi_ReportMode_r10_e_csi_ReportMode_r10_Optional csi_ReportMode_r10;
};

enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e {
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e_n2 = 0,
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e_n4 = 1,
};

typedef enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e;

struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10 {
	uint8_t k;
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10_periodicityFactor_r10_e periodicityFactor_r10;
};

enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_Sel {
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_UNBOUND_VALUE = 0,
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10 = 1,
	CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10 = 2,
};

union CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_Value {
	struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10 widebandCQI_r10;
	struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10 subbandCQI_r10;
};

struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10 {
	enum CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_Sel d;
	union CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_Value v;
};

enum CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e {
	CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e_setup = 0,
};

typedef enum CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e;

struct uint16_t_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup_ri_ConfigIndex2_r10_Optional {
	bool d;
	uint16_t v;
};

struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup {
	uint16_t cqi_pmi_ConfigIndex2_r10;
	struct uint16_t_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup_ri_ConfigIndex2_r10_Optional ri_ConfigIndex2_r10;
};

enum CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_Sel {
	CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_UNBOUND_VALUE = 0,
	CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_release = 1,
	CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup = 2,
};

union CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_Value {
	Null release;
	struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup setup;
};

struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10 {
	enum CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_Sel d;
	union CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_Value v;
};

struct uint16_t_CQI_ReportPeriodic_r10_setup_cqi_PUCCH_ResourceIndexP1_r10_Optional {
	bool d;
	uint16_t v;
};

struct uint16_t_CQI_ReportPeriodic_r10_setup_ri_ConfigIndex_Optional {
	bool d;
	uint16_t v;
};

struct CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e_cqi_Mask_r9_Optional {
	bool d;
	CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e v;
};

struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_csi_ConfigIndex_r10_Optional {
	bool d;
	struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10 v;
};

struct CQI_ReportPeriodic_r10_setup {
	uint16_t cqi_PUCCH_ResourceIndex_r10;
	struct uint16_t_CQI_ReportPeriodic_r10_setup_cqi_PUCCH_ResourceIndexP1_r10_Optional cqi_PUCCH_ResourceIndexP1_r10;
	uint16_t cqi_pmi_ConfigIndex;
	struct CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10 cqi_FormatIndicatorPeriodic_r10;
	struct uint16_t_CQI_ReportPeriodic_r10_setup_ri_ConfigIndex_Optional ri_ConfigIndex;
	bool simultaneousAckNackAndCQI;
	struct CQI_ReportPeriodic_r10_setup_cqi_Mask_r9_e_cqi_Mask_r9_Optional cqi_Mask_r9;
	struct CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_csi_ConfigIndex_r10_Optional csi_ConfigIndex_r10;
};

enum CQI_ReportPeriodic_r10_Sel {
	CQI_ReportPeriodic_r10_UNBOUND_VALUE = 0,
	CQI_ReportPeriodic_r10_release = 1,
	CQI_ReportPeriodic_r10_setup = 2,
};

union CQI_ReportPeriodic_r10_Value {
	Null release;
	struct CQI_ReportPeriodic_r10_setup setup;
};

struct CQI_ReportPeriodic_r10 {
	enum CQI_ReportPeriodic_r10_Sel d;
	union CQI_ReportPeriodic_r10_Value v;
};

enum CQI_ReportConfig_r10_pmi_RI_Report_r9_e {
	CQI_ReportConfig_r10_pmi_RI_Report_r9_e_setup = 0,
};

typedef enum CQI_ReportConfig_r10_pmi_RI_Report_r9_e CQI_ReportConfig_r10_pmi_RI_Report_r9_e;

enum MeasSubframePattern_r10_subframePatternTDD_r10_Sel {
	MeasSubframePattern_r10_subframePatternTDD_r10_UNBOUND_VALUE = 0,
	MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig1_5_r10 = 1,
	MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10 = 2,
	MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10 = 3,
};

union MeasSubframePattern_r10_subframePatternTDD_r10_Value {
	B20 subframeConfig1_5_r10;
	B70 subframeConfig0_r10;
	B60 subframeConfig6_r10;
};

struct MeasSubframePattern_r10_subframePatternTDD_r10 {
	enum MeasSubframePattern_r10_subframePatternTDD_r10_Sel d;
	union MeasSubframePattern_r10_subframePatternTDD_r10_Value v;
};

enum MeasSubframePattern_r10_Sel {
	MeasSubframePattern_r10_UNBOUND_VALUE = 0,
	MeasSubframePattern_r10_subframePatternFDD_r10 = 1,
	MeasSubframePattern_r10_subframePatternTDD_r10 = 2,
};

union MeasSubframePattern_r10_Value {
	B40 subframePatternFDD_r10;
	struct MeasSubframePattern_r10_subframePatternTDD_r10 subframePatternTDD_r10;
};

struct MeasSubframePattern_r10 {
	enum MeasSubframePattern_r10_Sel d;
	union MeasSubframePattern_r10_Value v;
};

struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_setup {
	struct MeasSubframePattern_r10 csi_MeasSubframeSet1_r10;
	struct MeasSubframePattern_r10 csi_MeasSubframeSet2_r10;
};

enum CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_Sel {
	CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_UNBOUND_VALUE = 0,
	CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_release = 1,
	CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_setup = 2,
};

union CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_Value {
	Null release;
	struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_setup setup;
};

struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10 {
	enum CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_Sel d;
	union CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_Value v;
};

struct CQI_ReportAperiodic_r10_CQI_ReportConfig_r10_cqi_ReportAperiodic_r10_Optional {
	bool d;
	struct CQI_ReportAperiodic_r10 v;
};

struct CQI_ReportPeriodic_r10_CQI_ReportConfig_r10_cqi_ReportPeriodic_r10_Optional {
	bool d;
	struct CQI_ReportPeriodic_r10 v;
};

struct CQI_ReportConfig_r10_pmi_RI_Report_r9_e_pmi_RI_Report_r9_Optional {
	bool d;
	CQI_ReportConfig_r10_pmi_RI_Report_r9_e v;
};

struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_csi_SubframePatternConfig_r10_Optional {
	bool d;
	struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10 v;
};

struct CQI_ReportConfig_r10 {
	struct CQI_ReportAperiodic_r10_CQI_ReportConfig_r10_cqi_ReportAperiodic_r10_Optional cqi_ReportAperiodic_r10;
	int8_t nomPDSCH_RS_EPRE_Offset;
	struct CQI_ReportPeriodic_r10_CQI_ReportConfig_r10_cqi_ReportPeriodic_r10_Optional cqi_ReportPeriodic_r10;
	struct CQI_ReportConfig_r10_pmi_RI_Report_r9_e_pmi_RI_Report_r9_Optional pmi_RI_Report_r9;
	struct CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_csi_SubframePatternConfig_r10_Optional csi_SubframePatternConfig_r10;
};

enum CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e {
	CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e_setup = 0,
};

typedef enum CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e;

typedef uint8_t CQI_ReportPeriodicProcExtId_r11;

struct CQI_ReportPeriodicProcExtId_r11_CQI_ReportPeriodicProcExtToReleaseList_r11_Dynamic {
	size_t d;
	CQI_ReportPeriodicProcExtId_r11* v;
};

typedef struct CQI_ReportPeriodicProcExtId_r11_CQI_ReportPeriodicProcExtToReleaseList_r11_Dynamic CQI_ReportPeriodicProcExtToReleaseList_r11;

enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e {
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e_submode1 = 0,
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e_submode2 = 1,
};

typedef enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e;

struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e_csi_ReportMode_r11_Optional {
	bool d;
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e v;
};

struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11 {
	struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11_csi_ReportMode_r11_e_csi_ReportMode_r11_Optional csi_ReportMode_r11;
};

enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e {
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e_n2 = 0,
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e_n4 = 1,
};

typedef enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e;

struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11 {
	uint8_t k;
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11_periodicityFactor_r11_e periodicityFactor_r11;
};

enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_Sel {
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_UNBOUND_VALUE = 0,
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11 = 1,
	CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11 = 2,
};

union CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_Value {
	struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_widebandCQI_r11 widebandCQI_r11;
	struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_subbandCQI_r11 subbandCQI_r11;
};

struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11 {
	enum CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_Sel d;
	union CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11_Value v;
};

struct uint16_t_CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_setup_ri_ConfigIndex2_r11_Optional {
	bool d;
	uint16_t v;
};

struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_setup {
	uint16_t cqi_pmi_ConfigIndex2_r11;
	struct uint16_t_CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_setup_ri_ConfigIndex2_r11_Optional ri_ConfigIndex2_r11;
};

enum CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_Sel {
	CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_UNBOUND_VALUE = 0,
	CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_release = 1,
	CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_setup = 2,
};

union CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_Value {
	Null release;
	struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_setup setup;
};

struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11 {
	enum CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_Sel d;
	union CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_Value v;
};

typedef uint16_t CRI_ConfigIndex_r13;

struct CRI_ConfigIndex_r13_CRI_ReportConfig_r13_setup_cri_ConfigIndex2_r13_Optional {
	bool d;
	CRI_ConfigIndex_r13 v;
};

struct CRI_ReportConfig_r13_setup {
	CRI_ConfigIndex_r13 cri_ConfigIndex_r13;
	struct CRI_ConfigIndex_r13_CRI_ReportConfig_r13_setup_cri_ConfigIndex2_r13_Optional cri_ConfigIndex2_r13;
};

enum CRI_ReportConfig_r13_Sel {
	CRI_ReportConfig_r13_UNBOUND_VALUE = 0,
	CRI_ReportConfig_r13_release = 1,
	CRI_ReportConfig_r13_setup = 2,
};

union CRI_ReportConfig_r13_Value {
	Null release;
	struct CRI_ReportConfig_r13_setup setup;
};

struct CRI_ReportConfig_r13 {
	enum CRI_ReportConfig_r13_Sel d;
	union CRI_ReportConfig_r13_Value v;
};

enum CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e {
	CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e_n2 = 0,
	CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e_n4 = 1,
};

typedef enum CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e;

struct uint16_t_CQI_ReportPeriodicProcExt_r11_ri_ConfigIndex_r11_Optional {
	bool d;
	uint16_t v;
};

struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_csi_ConfigIndex_r11_Optional {
	bool d;
	struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11 v;
};

struct CRI_ReportConfig_r13_CQI_ReportPeriodicProcExt_r11_cri_ReportConfig_r13_Optional {
	bool d;
	struct CRI_ReportConfig_r13 v;
};

struct CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e_periodicityFactorWB_r13_Optional {
	bool d;
	CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e v;
};

struct CQI_ReportPeriodicProcExt_r11 {
	CQI_ReportPeriodicProcExtId_r11 cqi_ReportPeriodicProcExtId_r11;
	uint16_t cqi_pmi_ConfigIndex_r11;
	struct CQI_ReportPeriodicProcExt_r11_cqi_FormatIndicatorPeriodic_r11 cqi_FormatIndicatorPeriodic_r11;
	struct uint16_t_CQI_ReportPeriodicProcExt_r11_ri_ConfigIndex_r11_Optional ri_ConfigIndex_r11;
	struct CQI_ReportPeriodicProcExt_r11_csi_ConfigIndex_r11_csi_ConfigIndex_r11_Optional csi_ConfigIndex_r11;
	struct CRI_ReportConfig_r13_CQI_ReportPeriodicProcExt_r11_cri_ReportConfig_r13_Optional cri_ReportConfig_r13;
	struct CQI_ReportPeriodicProcExt_r11_periodicityFactorWB_r13_e_periodicityFactorWB_r13_Optional periodicityFactorWB_r13;
};

struct CQI_ReportPeriodicProcExt_r11_CQI_ReportPeriodicProcExtToAddModList_r11_Dynamic {
	size_t d;
	struct CQI_ReportPeriodicProcExt_r11* v;
};

typedef struct CQI_ReportPeriodicProcExt_r11_CQI_ReportPeriodicProcExtToAddModList_r11_Dynamic CQI_ReportPeriodicProcExtToAddModList_r11;

struct CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e_simultaneousAckNackAndCQI_Format3_r11_Optional {
	bool d;
	CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e v;
};

struct CQI_ReportPeriodicProcExtToReleaseList_r11_CQI_ReportPeriodic_v1130_cqi_ReportPeriodicProcExtToReleaseList_r11_Optional {
	bool d;
	CQI_ReportPeriodicProcExtToReleaseList_r11 v;
};

struct CQI_ReportPeriodicProcExtToAddModList_r11_CQI_ReportPeriodic_v1130_cqi_ReportPeriodicProcExtToAddModList_r11_Optional {
	bool d;
	CQI_ReportPeriodicProcExtToAddModList_r11 v;
};

struct CQI_ReportPeriodic_v1130 {
	struct CQI_ReportPeriodic_v1130_simultaneousAckNackAndCQI_Format3_r11_e_simultaneousAckNackAndCQI_Format3_r11_Optional simultaneousAckNackAndCQI_Format3_r11;
	struct CQI_ReportPeriodicProcExtToReleaseList_r11_CQI_ReportPeriodic_v1130_cqi_ReportPeriodicProcExtToReleaseList_r11_Optional cqi_ReportPeriodicProcExtToReleaseList_r11;
	struct CQI_ReportPeriodicProcExtToAddModList_r11_CQI_ReportPeriodic_v1130_cqi_ReportPeriodicProcExtToAddModList_r11_Optional cqi_ReportPeriodicProcExtToAddModList_r11;
};

typedef uint8_t CSI_IM_ConfigId_r11;

struct CSI_IM_ConfigId_r11_CSI_IM_ConfigToReleaseList_r11_Dynamic {
	size_t d;
	CSI_IM_ConfigId_r11* v;
};

typedef struct CSI_IM_ConfigId_r11_CSI_IM_ConfigToReleaseList_r11_Dynamic CSI_IM_ConfigToReleaseList_r11;

struct bool_CSI_IM_Config_r11_interferenceMeasRestriction_r13_Optional {
	bool d;
	bool v;
};

struct CSI_IM_Config_r11 {
	CSI_IM_ConfigId_r11 csi_IM_ConfigId_r11;
	uint8_t resourceConfig_r11;
	uint8_t subframeConfig_r11;
	struct bool_CSI_IM_Config_r11_interferenceMeasRestriction_r13_Optional interferenceMeasRestriction_r13;
};

struct CSI_IM_Config_r11_CSI_IM_ConfigToAddModList_r11_Dynamic {
	size_t d;
	struct CSI_IM_Config_r11* v;
};

typedef struct CSI_IM_Config_r11_CSI_IM_ConfigToAddModList_r11_Dynamic CSI_IM_ConfigToAddModList_r11;

typedef uint8_t CSI_ProcessId_r11;

struct CSI_ProcessId_r11_CSI_ProcessToReleaseList_r11_Dynamic {
	size_t d;
	CSI_ProcessId_r11* v;
};

typedef struct CSI_ProcessId_r11_CSI_ProcessToReleaseList_r11_Dynamic CSI_ProcessToReleaseList_r11;

typedef uint8_t CSI_RS_ConfigNZPId_r11;

struct P_C_AndCBSR_r11 {
	int8_t p_C_r11;
	BIT_STRING codebookSubsetRestriction_r11;
};

struct P_C_AndCBSR_r11_P_C_AndCBSR_Pair_r13a_Dynamic {
	size_t d;
	struct P_C_AndCBSR_r11* v;
};

typedef struct P_C_AndCBSR_r11_P_C_AndCBSR_Pair_r13a_Dynamic P_C_AndCBSR_Pair_r13a;

enum CQI_ReportBothProc_r11_pmi_RI_Report_r11_e {
	CQI_ReportBothProc_r11_pmi_RI_Report_r11_e_setup = 0,
};

typedef enum CQI_ReportBothProc_r11_pmi_RI_Report_r11_e CQI_ReportBothProc_r11_pmi_RI_Report_r11_e;

struct CSI_ProcessId_r11_CQI_ReportBothProc_r11_ri_Ref_CSI_ProcessId_r11_Optional {
	bool d;
	CSI_ProcessId_r11 v;
};

struct CQI_ReportBothProc_r11_pmi_RI_Report_r11_e_pmi_RI_Report_r11_Optional {
	bool d;
	CQI_ReportBothProc_r11_pmi_RI_Report_r11_e v;
};

struct CQI_ReportBothProc_r11 {
	struct CSI_ProcessId_r11_CQI_ReportBothProc_r11_ri_Ref_CSI_ProcessId_r11_Optional ri_Ref_CSI_ProcessId_r11;
	struct CQI_ReportBothProc_r11_pmi_RI_Report_r11_e_pmi_RI_Report_r11_Optional pmi_RI_Report_r11;
};

struct CQI_ReportAperiodicProc_r11 {
	CQI_ReportModeAperiodic_e cqi_ReportModeAperiodic_r11;
	bool trigger01_r11;
	bool trigger10_r11;
	bool trigger11_r11;
};

enum CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e {
	CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e_true = 0,
};

typedef enum CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e;

typedef uint8_t CSI_IM_ConfigId_r12;

enum CSI_Process_r11_csi_IM_ConfigIdList_r12_Sel {
	CSI_Process_r11_csi_IM_ConfigIdList_r12_UNBOUND_VALUE = 0,
	CSI_Process_r11_csi_IM_ConfigIdList_r12_release = 1,
	CSI_Process_r11_csi_IM_ConfigIdList_r12_setup = 2,
};

struct CSI_IM_ConfigId_r12_setup_Dynamic {
	size_t d;
	CSI_IM_ConfigId_r12* v;
};

union CSI_Process_r11_csi_IM_ConfigIdList_r12_Value {
	Null release;
	struct CSI_IM_ConfigId_r12_setup_Dynamic setup;
};

struct CSI_Process_r11_csi_IM_ConfigIdList_r12 {
	enum CSI_Process_r11_csi_IM_ConfigIdList_r12_Sel d;
	union CSI_Process_r11_csi_IM_ConfigIdList_r12_Value v;
};

enum CSI_Process_r11_cqi_ReportAperiodicProc2_r12_Sel {
	CSI_Process_r11_cqi_ReportAperiodicProc2_r12_UNBOUND_VALUE = 0,
	CSI_Process_r11_cqi_ReportAperiodicProc2_r12_release = 1,
	CSI_Process_r11_cqi_ReportAperiodicProc2_r12_setup = 2,
};

union CSI_Process_r11_cqi_ReportAperiodicProc2_r12_Value {
	Null release;
	struct CQI_ReportAperiodicProc_r11 setup;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc2_r12 {
	enum CSI_Process_r11_cqi_ReportAperiodicProc2_r12_Sel d;
	union CSI_Process_r11_cqi_ReportAperiodicProc2_r12_Value v;
};

struct CQI_ReportAperiodicProc_v1310 {
	bool trigger001_r13;
	bool trigger010_r13;
	bool trigger011_r13;
	bool trigger100_r13;
	bool trigger101_r13;
	bool trigger110_r13;
	bool trigger111_r13;
};

enum CSI_Process_r11_cqi_ReportAperiodicProc_v1310_Sel {
	CSI_Process_r11_cqi_ReportAperiodicProc_v1310_UNBOUND_VALUE = 0,
	CSI_Process_r11_cqi_ReportAperiodicProc_v1310_release = 1,
	CSI_Process_r11_cqi_ReportAperiodicProc_v1310_setup = 2,
};

union CSI_Process_r11_cqi_ReportAperiodicProc_v1310_Value {
	Null release;
	struct CQI_ReportAperiodicProc_v1310 setup;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc_v1310 {
	enum CSI_Process_r11_cqi_ReportAperiodicProc_v1310_Sel d;
	union CSI_Process_r11_cqi_ReportAperiodicProc_v1310_Value v;
};

enum CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_Sel {
	CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_UNBOUND_VALUE = 0,
	CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_release = 1,
	CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_setup = 2,
};

union CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_Value {
	Null release;
	struct CQI_ReportAperiodicProc_v1310 setup;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc2_v1310 {
	enum CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_Sel d;
	union CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_Value v;
};

struct P_C_AndCBSR_r13_cbsr_Selection_r13_nonPrecoded_r13 {
	BIT_STRING codebookSubsetRestriction1_r13;
	BIT_STRING codebookSubsetRestriction2_r13;
};

struct P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedK1a_r13 {
	BIT_STRING codebookSubsetRestriction3_r13;
};

struct P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedKN_r13 {
	BIT_STRING codebookSubsetRestriction_r13;
};

enum P_C_AndCBSR_r13_cbsr_Selection_r13_Sel {
	P_C_AndCBSR_r13_cbsr_Selection_r13_UNBOUND_VALUE = 0,
	P_C_AndCBSR_r13_cbsr_Selection_r13_nonPrecoded_r13 = 1,
	P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedK1a_r13 = 2,
	P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedKN_r13 = 3,
};

union P_C_AndCBSR_r13_cbsr_Selection_r13_Value {
	struct P_C_AndCBSR_r13_cbsr_Selection_r13_nonPrecoded_r13 nonPrecoded_r13;
	struct P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedK1a_r13 beamformedK1a_r13;
	struct P_C_AndCBSR_r13_cbsr_Selection_r13_beamformedKN_r13 beamformedKN_r13;
};

struct P_C_AndCBSR_r13_cbsr_Selection_r13 {
	enum P_C_AndCBSR_r13_cbsr_Selection_r13_Sel d;
	union P_C_AndCBSR_r13_cbsr_Selection_r13_Value v;
};

struct P_C_AndCBSR_r13 {
	int8_t p_C_r13;
	struct P_C_AndCBSR_r13_cbsr_Selection_r13 cbsr_Selection_r13;
};

struct P_C_AndCBSR_r13_P_C_AndCBSR_Pair_r13_Dynamic {
	size_t d;
	struct P_C_AndCBSR_r13* v;
};

typedef struct P_C_AndCBSR_r13_P_C_AndCBSR_Pair_r13_Dynamic P_C_AndCBSR_Pair_r13;

enum CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e {
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e_n1 = 0,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e_n2 = 1,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e_n3 = 2,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e_n4 = 3,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e_n8 = 4,
};

typedef enum CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e;

enum CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e {
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e_n1 = 0,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e_n2 = 1,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e_n3 = 2,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e_n4 = 3,
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e_n8 = 4,
};

typedef enum CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e;

enum CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e {
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e_n4 = 0,
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e_n8 = 1,
};

typedef enum CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e;

enum CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e {
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e_n4 = 0,
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e_n8 = 1,
};

typedef enum CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e;

typedef uint8_t CSI_IM_ConfigId_r13;

typedef uint8_t ResourceConfig_r13;

typedef uint8_t NZP_TransmissionComb_r14;

enum NZP_FrequencyDensity_r14_e {
	NZP_FrequencyDensity_r14_e_d1 = 0,
	NZP_FrequencyDensity_r14_e_d2 = 1,
	NZP_FrequencyDensity_r14_e_d3 = 2,
};

typedef enum NZP_FrequencyDensity_r14_e NZP_FrequencyDensity_r14_e;

struct NZP_TransmissionComb_r14_NZP_ResourceConfig_r13_transmissionComb_r14_Optional {
	bool d;
	NZP_TransmissionComb_r14 v;
};

struct NZP_FrequencyDensity_r14_e_NZP_ResourceConfig_r13_frequencyDensity_r14_Optional {
	bool d;
	NZP_FrequencyDensity_r14_e v;
};

struct NZP_ResourceConfig_r13 {
	ResourceConfig_r13 resourceConfig_r13;
	struct NZP_TransmissionComb_r14_NZP_ResourceConfig_r13_transmissionComb_r14_Optional transmissionComb_r14;
	struct NZP_FrequencyDensity_r14_e_NZP_ResourceConfig_r13_frequencyDensity_r14_Optional frequencyDensity_r14;
};

enum CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e {
	CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e_cdm2 = 0,
	CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e_cdm4 = 1,
};

typedef enum CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e;

struct NZP_ResourceConfig_r13_CSI_RS_ConfigNZP_EMIMO_r13_setup_nzp_resourceConfigList_r13_Dynamic {
	size_t d;
	struct NZP_ResourceConfig_r13* v;
};

struct CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e_cdmType_r13_Optional {
	bool d;
	CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e v;
};

struct CSI_RS_ConfigNZP_EMIMO_r13_setup {
	struct NZP_ResourceConfig_r13_CSI_RS_ConfigNZP_EMIMO_r13_setup_nzp_resourceConfigList_r13_Dynamic nzp_resourceConfigList_r13;
	struct CSI_RS_ConfigNZP_EMIMO_r13_setup_cdmType_r13_e_cdmType_r13_Optional cdmType_r13;
};

enum CSI_RS_ConfigNZP_EMIMO_r13_Sel {
	CSI_RS_ConfigNZP_EMIMO_r13_UNBOUND_VALUE = 0,
	CSI_RS_ConfigNZP_EMIMO_r13_release = 1,
	CSI_RS_ConfigNZP_EMIMO_r13_setup = 2,
};

union CSI_RS_ConfigNZP_EMIMO_r13_Value {
	Null release;
	struct CSI_RS_ConfigNZP_EMIMO_r13_setup setup;
};

struct CSI_RS_ConfigNZP_EMIMO_r13 {
	enum CSI_RS_ConfigNZP_EMIMO_r13_Sel d;
	union CSI_RS_ConfigNZP_EMIMO_r13_Value v;
};

struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigNonPrecoded_r13_p_C_AndCBSRList_r13_Optional {
	bool d;
	P_C_AndCBSR_Pair_r13 v;
};

struct CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e_codebookOverSamplingRateConfig_O1_r13_Optional {
	bool d;
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e v;
};

struct CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e_codebookOverSamplingRateConfig_O2_r13_Optional {
	bool d;
	CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigNonPrecoded_r13_csi_IM_ConfigIdList_r13_Dynamic {
	size_t d;
	CSI_IM_ConfigId_r13* v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigNonPrecoded_r13_csi_IM_ConfigIdList_r13_DynamicOptional {
	bool d;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigNonPrecoded_r13_csi_IM_ConfigIdList_r13_Dynamic v;
};

struct CSI_RS_ConfigNZP_EMIMO_r13_CSI_RS_ConfigNonPrecoded_r13_csi_RS_ConfigNZP_EMIMO_r13_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_EMIMO_r13 v;
};

struct CSI_RS_ConfigNonPrecoded_r13 {
	struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigNonPrecoded_r13_p_C_AndCBSRList_r13_Optional p_C_AndCBSRList_r13;
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN1_r13_e codebookConfigN1_r13;
	CSI_RS_ConfigNonPrecoded_r13_codebookConfigN2_r13_e codebookConfigN2_r13;
	struct CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O1_r13_e_codebookOverSamplingRateConfig_O1_r13_Optional codebookOverSamplingRateConfig_O1_r13;
	struct CSI_RS_ConfigNonPrecoded_r13_codebookOverSamplingRateConfig_O2_r13_e_codebookOverSamplingRateConfig_O2_r13_Optional codebookOverSamplingRateConfig_O2_r13;
	uint8_t codebookConfig_r13;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigNonPrecoded_r13_csi_IM_ConfigIdList_r13_DynamicOptional csi_IM_ConfigIdList_r13;
	struct CSI_RS_ConfigNZP_EMIMO_r13_CSI_RS_ConfigNonPrecoded_r13_csi_RS_ConfigNZP_EMIMO_r13_Optional csi_RS_ConfigNZP_EMIMO_r13;
};

typedef uint8_t CSI_RS_ConfigNZPId_r13;

enum CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e {
	CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e_true = 0,
};

typedef enum CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e;

enum CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e {
	CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e_on = 0,
};

typedef enum CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e;

struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r13_csi_RS_ConfigNZPIdListExt_r13_Dynamic {
	size_t d;
	CSI_RS_ConfigNZPId_r13* v;
};

struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r13_csi_RS_ConfigNZPIdListExt_r13_DynamicOptional {
	bool d;
	struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r13_csi_RS_ConfigNZPIdListExt_r13_Dynamic v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r13_csi_IM_ConfigIdList_r13_Dynamic {
	size_t d;
	CSI_IM_ConfigId_r13* v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r13_csi_IM_ConfigIdList_r13_DynamicOptional {
	bool d;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r13_csi_IM_ConfigIdList_r13_Dynamic v;
};

struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r13_p_C_AndCBSR_PerResourceConfigList_r13_Dynamic {
	size_t d;
	P_C_AndCBSR_Pair_r13* v;
};

struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r13_p_C_AndCBSR_PerResourceConfigList_r13_DynamicOptional {
	bool d;
	struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r13_p_C_AndCBSR_PerResourceConfigList_r13_Dynamic v;
};

struct bool_CSI_RS_ConfigBeamformed_r13_ace_For4Tx_PerResourceConfigList_r13_Dynamic {
	size_t d;
	bool* v;
};

struct bool_CSI_RS_ConfigBeamformed_r13_ace_For4Tx_PerResourceConfigList_r13_DynamicOptional {
	bool d;
	struct bool_CSI_RS_ConfigBeamformed_r13_ace_For4Tx_PerResourceConfigList_r13_Dynamic v;
};

struct CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e_alternativeCodebookEnabledBeamformed_r13_Optional {
	bool d;
	CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e v;
};

struct CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e_channelMeasRestriction_r13_Optional {
	bool d;
	CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e v;
};

struct CSI_RS_ConfigBeamformed_r13 {
	struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r13_csi_RS_ConfigNZPIdListExt_r13_DynamicOptional csi_RS_ConfigNZPIdListExt_r13;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r13_csi_IM_ConfigIdList_r13_DynamicOptional csi_IM_ConfigIdList_r13;
	struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r13_p_C_AndCBSR_PerResourceConfigList_r13_DynamicOptional p_C_AndCBSR_PerResourceConfigList_r13;
	struct bool_CSI_RS_ConfigBeamformed_r13_ace_For4Tx_PerResourceConfigList_r13_DynamicOptional ace_For4Tx_PerResourceConfigList_r13;
	struct CSI_RS_ConfigBeamformed_r13_alternativeCodebookEnabledBeamformed_r13_e_alternativeCodebookEnabledBeamformed_r13_Optional alternativeCodebookEnabledBeamformed_r13;
	struct CSI_RS_ConfigBeamformed_r13_channelMeasRestriction_r13_e_channelMeasRestriction_r13_Optional channelMeasRestriction_r13;
};

enum CSI_RS_ConfigEMIMO_r13_setup_Sel {
	CSI_RS_ConfigEMIMO_r13_setup_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_r13_setup_nonPrecoded_r13 = 1,
	CSI_RS_ConfigEMIMO_r13_setup_beamformed_r13 = 2,
};

union CSI_RS_ConfigEMIMO_r13_setup_Value {
	struct CSI_RS_ConfigNonPrecoded_r13 nonPrecoded_r13;
	struct CSI_RS_ConfigBeamformed_r13 beamformed_r13;
};

struct CSI_RS_ConfigEMIMO_r13_setup {
	enum CSI_RS_ConfigEMIMO_r13_setup_Sel d;
	union CSI_RS_ConfigEMIMO_r13_setup_Value v;
};

enum CSI_RS_ConfigEMIMO_r13_Sel {
	CSI_RS_ConfigEMIMO_r13_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_r13_release = 1,
	CSI_RS_ConfigEMIMO_r13_setup = 2,
};

union CSI_RS_ConfigEMIMO_r13_Value {
	Null release;
	struct CSI_RS_ConfigEMIMO_r13_setup setup;
};

struct CSI_RS_ConfigEMIMO_r13 {
	enum CSI_RS_ConfigEMIMO_r13_Sel d;
	union CSI_RS_ConfigEMIMO_r13_Value v;
};

enum CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e {
	CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e_cdm8 = 0,
};

typedef enum CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e;

struct NZP_ResourceConfig_r13_CSI_RS_ConfigNZP_EMIMO_v1430_nzp_resourceConfigListExt_r14_Dynamic {
	size_t d;
	struct NZP_ResourceConfig_r13* v;
};

struct CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e_cdmType_v1430_Optional {
	bool d;
	CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e v;
};

struct CSI_RS_ConfigNZP_EMIMO_v1430 {
	struct NZP_ResourceConfig_r13_CSI_RS_ConfigNZP_EMIMO_v1430_nzp_resourceConfigListExt_r14_Dynamic nzp_resourceConfigListExt_r14;
	struct CSI_RS_ConfigNZP_EMIMO_v1430_cdmType_v1430_e_cdmType_v1430_Optional cdmType_v1430;
};

enum CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e {
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n5 = 0,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n6 = 1,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n7 = 2,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n10 = 3,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n12 = 4,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n14 = 5,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e_n16 = 6,
};

typedef enum CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e;

enum CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e {
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e_n5 = 0,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e_n6 = 1,
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e_n7 = 2,
};

typedef enum CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e;

struct NZP_TransmissionComb_r14_CSI_RS_Config_NZP_v1430_transmissionComb_r14_Optional {
	bool d;
	NZP_TransmissionComb_r14 v;
};

struct NZP_FrequencyDensity_r14_e_CSI_RS_Config_NZP_v1430_frequencyDensity_r14_Optional {
	bool d;
	NZP_FrequencyDensity_r14_e v;
};

struct CSI_RS_Config_NZP_v1430 {
	struct NZP_TransmissionComb_r14_CSI_RS_Config_NZP_v1430_transmissionComb_r14_Optional transmissionComb_r14;
	struct NZP_FrequencyDensity_r14_e_CSI_RS_Config_NZP_v1430_frequencyDensity_r14_Optional frequencyDensity_r14;
};

struct CSI_RS_ConfigNZP_EMIMO_v1430_CSI_RS_ConfigNonPrecoded_v1430_csi_RS_ConfigNZP_EMIMO_v1430_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_EMIMO_v1430 v;
};

struct CSI_RS_ConfigNonPrecoded_v1430 {
	struct CSI_RS_ConfigNZP_EMIMO_v1430_CSI_RS_ConfigNonPrecoded_v1430_csi_RS_ConfigNZP_EMIMO_v1430_Optional csi_RS_ConfigNZP_EMIMO_v1430;
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN1_v1430_e codebookConfigN1_v1430;
	CSI_RS_ConfigNonPrecoded_v1430_codebookConfigN2_v1430_e codebookConfigN2_v1430;
	struct CSI_RS_Config_NZP_v1430 nzp_ResourceConfigTM9_Original_v1430;
};

enum CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e {
	CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e_an1 = 0,
	CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e_an2 = 1,
	CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e_an4 = 2,
	CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e_an8 = 3,
};

typedef enum CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e;

enum CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e {
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e_n1 = 0,
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e_n2 = 1,
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e_n4 = 2,
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e_spare1 = 3,
};

typedef enum CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e;

struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_setup {
	MBSFN_SubframeConfigList subframeConfigList;
};

enum CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_Sel {
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_UNBOUND_VALUE = 0,
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_release = 1,
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_setup = 2,
};

union CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_Value {
	Null release;
	struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_setup setup;
};

struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11 {
	enum CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_Sel d;
	union CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_Value v;
};

struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_mbsfn_SubframeConfigList_r11_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11 v;
};

struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11 {
	uint16_t qcl_ScramblingIdentity_r11;
	CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_crs_PortsCount_r11_e crs_PortsCount_r11;
	struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_mbsfn_SubframeConfigList_r11_mbsfn_SubframeConfigList_r11_Optional mbsfn_SubframeConfigList_r11;
};

typedef uint8_t CSI_RS_ConfigNZPId_v1310;

struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_setup {
	MBSFN_SubframeConfigList_v1430 subframeConfigList_v1430;
};

enum CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_Sel {
	CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_UNBOUND_VALUE = 0,
	CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_release = 1,
	CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_setup = 2,
};

union CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_Value {
	Null release;
	struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_setup setup;
};

struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430 {
	enum CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_Sel d;
	union CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_Value v;
};

struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_qcl_CRS_Info_r11_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11 v;
};

struct CSI_RS_ConfigNZPId_v1310_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZPId_v1310_Optional {
	bool d;
	CSI_RS_ConfigNZPId_v1310 v;
};

struct NZP_TransmissionComb_r14_CSI_RS_ConfigNZP_r11_transmissionComb_r14_Optional {
	bool d;
	NZP_TransmissionComb_r14 v;
};

struct NZP_FrequencyDensity_r14_e_CSI_RS_ConfigNZP_r11_frequencyDensity_r14_Optional {
	bool d;
	NZP_FrequencyDensity_r14_e v;
};

struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_mbsfn_SubframeConfigList_v1430_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430 v;
};

struct CSI_RS_ConfigNZP_r11 {
	CSI_RS_ConfigNZPId_r11 csi_RS_ConfigNZPId_r11;
	CSI_RS_ConfigNZP_r11_antennaPortsCount_r11_e antennaPortsCount_r11;
	uint8_t resourceConfig_r11;
	uint8_t subframeConfig_r11;
	uint16_t scramblingIdentity_r11;
	struct CSI_RS_ConfigNZP_r11_qcl_CRS_Info_r11_qcl_CRS_Info_r11_Optional qcl_CRS_Info_r11;
	struct CSI_RS_ConfigNZPId_v1310_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZPId_v1310_Optional csi_RS_ConfigNZPId_v1310;
	struct NZP_TransmissionComb_r14_CSI_RS_ConfigNZP_r11_transmissionComb_r14_Optional transmissionComb_r14;
	struct NZP_FrequencyDensity_r14_e_CSI_RS_ConfigNZP_r11_frequencyDensity_r14_Optional frequencyDensity_r14;
	struct CSI_RS_ConfigNZP_r11_mbsfn_SubframeConfigList_v1430_mbsfn_SubframeConfigList_v1430_Optional mbsfn_SubframeConfigList_v1430;
};

enum CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e {
	CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e_semiPersistent = 0,
	CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e_aperiodic = 1,
};

typedef enum CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e;

struct CSI_RS_ConfigNZP_Activation_r14 {
	CSI_RS_ConfigNZP_Activation_r14_csi_RS_NZP_mode_r14_e csi_RS_NZP_mode_r14;
	uint8_t activatedResources_r14;
};

struct CSI_RS_ConfigBeamformed_v1430_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZP_ApList_r14_Dynamic {
	size_t d;
	struct CSI_RS_ConfigNZP_r11* v;
};

struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigBeamformed_v1430_csi_RS_ConfigNZP_ApList_r14_DynamicOptional {
	bool d;
	struct CSI_RS_ConfigBeamformed_v1430_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZP_ApList_r14_Dynamic v;
};

struct CSI_RS_Config_NZP_v1430_CSI_RS_ConfigBeamformed_v1430_nzp_ResourceConfigOriginal_v1430_Optional {
	bool d;
	struct CSI_RS_Config_NZP_v1430 v;
};

struct CSI_RS_ConfigNZP_Activation_r14_CSI_RS_ConfigBeamformed_v1430_csi_RS_NZP_Activation_r14_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_Activation_r14 v;
};

struct CSI_RS_ConfigBeamformed_v1430 {
	struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigBeamformed_v1430_csi_RS_ConfigNZP_ApList_r14_DynamicOptional csi_RS_ConfigNZP_ApList_r14;
	struct CSI_RS_Config_NZP_v1430_CSI_RS_ConfigBeamformed_v1430_nzp_ResourceConfigOriginal_v1430_Optional nzp_ResourceConfigOriginal_v1430;
	struct CSI_RS_ConfigNZP_Activation_r14_CSI_RS_ConfigBeamformed_v1430_csi_RS_NZP_Activation_r14_Optional csi_RS_NZP_Activation_r14;
};

enum CSI_RS_ConfigEMIMO_v1430_setup_Sel {
	CSI_RS_ConfigEMIMO_v1430_setup_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1430_setup_nonPrecoded_v1430 = 1,
	CSI_RS_ConfigEMIMO_v1430_setup_beamformed_v1430 = 2,
};

union CSI_RS_ConfigEMIMO_v1430_setup_Value {
	struct CSI_RS_ConfigNonPrecoded_v1430 nonPrecoded_v1430;
	struct CSI_RS_ConfigBeamformed_v1430 beamformed_v1430;
};

struct CSI_RS_ConfigEMIMO_v1430_setup {
	enum CSI_RS_ConfigEMIMO_v1430_setup_Sel d;
	union CSI_RS_ConfigEMIMO_v1430_setup_Value v;
};

enum CSI_RS_ConfigEMIMO_v1430_Sel {
	CSI_RS_ConfigEMIMO_v1430_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1430_release = 1,
	CSI_RS_ConfigEMIMO_v1430_setup = 2,
};

union CSI_RS_ConfigEMIMO_v1430_Value {
	Null release;
	struct CSI_RS_ConfigEMIMO_v1430_setup setup;
};

struct CSI_RS_ConfigEMIMO_v1430 {
	enum CSI_RS_ConfigEMIMO_v1430_Sel d;
	union CSI_RS_ConfigEMIMO_v1430_Value v;
};

enum CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e {
	CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e_true = 0,
};

typedef enum CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e;

enum CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e {
	CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e_on = 0,
};

typedef enum CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e;

struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZPIdListExt_r14_Dynamic {
	size_t d;
	CSI_RS_ConfigNZPId_r13* v;
};

struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZPIdListExt_r14_DynamicOptional {
	bool d;
	struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZPIdListExt_r14_Dynamic v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r14_csi_IM_ConfigIdList_r14_Dynamic {
	size_t d;
	CSI_IM_ConfigId_r13* v;
};

struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r14_csi_IM_ConfigIdList_r14_DynamicOptional {
	bool d;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r14_csi_IM_ConfigIdList_r14_Dynamic v;
};

struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r14_p_C_AndCBSR_PerResourceConfigList_r14_Dynamic {
	size_t d;
	P_C_AndCBSR_Pair_r13* v;
};

struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r14_p_C_AndCBSR_PerResourceConfigList_r14_DynamicOptional {
	bool d;
	struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r14_p_C_AndCBSR_PerResourceConfigList_r14_Dynamic v;
};

struct bool_CSI_RS_ConfigBeamformed_r14_ace_For4Tx_PerResourceConfigList_r14_Dynamic {
	size_t d;
	bool* v;
};

struct bool_CSI_RS_ConfigBeamformed_r14_ace_For4Tx_PerResourceConfigList_r14_DynamicOptional {
	bool d;
	struct bool_CSI_RS_ConfigBeamformed_r14_ace_For4Tx_PerResourceConfigList_r14_Dynamic v;
};

struct CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e_alternativeCodebookEnabledBeamformed_r14_Optional {
	bool d;
	CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e v;
};

struct CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e_channelMeasRestriction_r14_Optional {
	bool d;
	CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e v;
};

struct CSI_RS_ConfigBeamformed_r14_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZP_ApList_r14_Dynamic {
	size_t d;
	struct CSI_RS_ConfigNZP_r11* v;
};

struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZP_ApList_r14_DynamicOptional {
	bool d;
	struct CSI_RS_ConfigBeamformed_r14_CSI_RS_ConfigNZP_r11_csi_RS_ConfigNZP_ApList_r14_Dynamic v;
};

struct CSI_RS_Config_NZP_v1430_CSI_RS_ConfigBeamformed_r14_nzp_ResourceConfigOriginal_v1430_Optional {
	bool d;
	struct CSI_RS_Config_NZP_v1430 v;
};

struct CSI_RS_ConfigNZP_Activation_r14_CSI_RS_ConfigBeamformed_r14_csi_RS_NZP_Activation_r14_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_Activation_r14 v;
};

struct CSI_RS_ConfigBeamformed_r14 {
	struct CSI_RS_ConfigNZPId_r13_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZPIdListExt_r14_DynamicOptional csi_RS_ConfigNZPIdListExt_r14;
	struct CSI_IM_ConfigId_r13_CSI_RS_ConfigBeamformed_r14_csi_IM_ConfigIdList_r14_DynamicOptional csi_IM_ConfigIdList_r14;
	struct P_C_AndCBSR_Pair_r13_CSI_RS_ConfigBeamformed_r14_p_C_AndCBSR_PerResourceConfigList_r14_DynamicOptional p_C_AndCBSR_PerResourceConfigList_r14;
	struct bool_CSI_RS_ConfigBeamformed_r14_ace_For4Tx_PerResourceConfigList_r14_DynamicOptional ace_For4Tx_PerResourceConfigList_r14;
	struct CSI_RS_ConfigBeamformed_r14_alternativeCodebookEnabledBeamformed_r14_e_alternativeCodebookEnabledBeamformed_r14_Optional alternativeCodebookEnabledBeamformed_r14;
	struct CSI_RS_ConfigBeamformed_r14_channelMeasRestriction_r14_e_channelMeasRestriction_r14_Optional channelMeasRestriction_r14;
	struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigBeamformed_r14_csi_RS_ConfigNZP_ApList_r14_DynamicOptional csi_RS_ConfigNZP_ApList_r14;
	struct CSI_RS_Config_NZP_v1430_CSI_RS_ConfigBeamformed_r14_nzp_ResourceConfigOriginal_v1430_Optional nzp_ResourceConfigOriginal_v1430;
	struct CSI_RS_ConfigNZP_Activation_r14_CSI_RS_ConfigBeamformed_r14_csi_RS_NZP_Activation_r14_Optional csi_RS_NZP_Activation_r14;
};

enum CSI_RS_ConfigEMIMO2_r14_Sel {
	CSI_RS_ConfigEMIMO2_r14_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO2_r14_release = 1,
	CSI_RS_ConfigEMIMO2_r14_setup = 2,
};

union CSI_RS_ConfigEMIMO2_r14_Value {
	Null release;
	struct CSI_RS_ConfigBeamformed_r14 setup;
};

struct CSI_RS_ConfigEMIMO2_r14 {
	enum CSI_RS_ConfigEMIMO2_r14_Sel d;
	union CSI_RS_ConfigEMIMO2_r14_Value v;
};

struct uint16_t_CSI_RS_ConfigEMIMO_Hybrid_r14_setup_periodicityOffsetIndex_r14_Optional {
	bool d;
	uint16_t v;
};

struct CSI_RS_ConfigEMIMO2_r14_CSI_RS_ConfigEMIMO_Hybrid_r14_setup_eMIMO_Type2_r14_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO2_r14 v;
};

struct CSI_RS_ConfigEMIMO_Hybrid_r14_setup {
	struct uint16_t_CSI_RS_ConfigEMIMO_Hybrid_r14_setup_periodicityOffsetIndex_r14_Optional periodicityOffsetIndex_r14;
	struct CSI_RS_ConfigEMIMO2_r14_CSI_RS_ConfigEMIMO_Hybrid_r14_setup_eMIMO_Type2_r14_Optional eMIMO_Type2_r14;
};

enum CSI_RS_ConfigEMIMO_Hybrid_r14_Sel {
	CSI_RS_ConfigEMIMO_Hybrid_r14_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_Hybrid_r14_release = 1,
	CSI_RS_ConfigEMIMO_Hybrid_r14_setup = 2,
};

union CSI_RS_ConfigEMIMO_Hybrid_r14_Value {
	Null release;
	struct CSI_RS_ConfigEMIMO_Hybrid_r14_setup setup;
};

struct CSI_RS_ConfigEMIMO_Hybrid_r14 {
	enum CSI_RS_ConfigEMIMO_Hybrid_r14_Sel d;
	union CSI_RS_ConfigEMIMO_Hybrid_r14_Value v;
};

enum CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e {
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n5 = 0,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n6 = 1,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n7 = 2,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n10 = 3,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n12 = 4,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n14 = 5,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_n16 = 6,
};

typedef enum CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e;

enum CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e {
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e_n5 = 0,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e_n6 = 1,
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e_n7 = 2,
};

typedef enum CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e;

struct CSI_RS_ConfigNZP_EMIMO_v1430_CSI_RS_ConfigNonPrecoded_v1480_csi_RS_ConfigNZP_EMIMO_v1480_Optional {
	bool d;
	struct CSI_RS_ConfigNZP_EMIMO_v1430 v;
};

struct CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_codebookConfigN1_v1480_Optional {
	bool d;
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e v;
};

struct CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e_codebookConfigN2_r1480_Optional {
	bool d;
	CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e v;
};

struct CSI_RS_ConfigNonPrecoded_v1480 {
	struct CSI_RS_ConfigNZP_EMIMO_v1430_CSI_RS_ConfigNonPrecoded_v1480_csi_RS_ConfigNZP_EMIMO_v1480_Optional csi_RS_ConfigNZP_EMIMO_v1480;
	struct CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN1_v1480_e_codebookConfigN1_v1480_Optional codebookConfigN1_v1480;
	struct CSI_RS_ConfigNonPrecoded_v1480_codebookConfigN2_r1480_e_codebookConfigN2_r1480_Optional codebookConfigN2_r1480;
	struct CSI_RS_Config_NZP_v1430 nzp_ResourceConfigTM9_Original_v1480;
};

enum CSI_RS_ConfigEMIMO_v1480_setup_Sel {
	CSI_RS_ConfigEMIMO_v1480_setup_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1480_setup_nonPrecoded_v1480 = 1,
	CSI_RS_ConfigEMIMO_v1480_setup_beamformed_v1480 = 2,
};

union CSI_RS_ConfigEMIMO_v1480_setup_Value {
	struct CSI_RS_ConfigNonPrecoded_v1480 nonPrecoded_v1480;
	struct CSI_RS_ConfigBeamformed_v1430 beamformed_v1480;
};

struct CSI_RS_ConfigEMIMO_v1480_setup {
	enum CSI_RS_ConfigEMIMO_v1480_setup_Sel d;
	union CSI_RS_ConfigEMIMO_v1480_setup_Value v;
};

enum CSI_RS_ConfigEMIMO_v1480_Sel {
	CSI_RS_ConfigEMIMO_v1480_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1480_release = 1,
	CSI_RS_ConfigEMIMO_v1480_setup = 2,
};

union CSI_RS_ConfigEMIMO_v1480_Value {
	Null release;
	struct CSI_RS_ConfigEMIMO_v1480_setup setup;
};

struct CSI_RS_ConfigEMIMO_v1480 {
	enum CSI_RS_ConfigEMIMO_v1480_Sel d;
	union CSI_RS_ConfigEMIMO_v1480_Value v;
};

struct P_C_AndCBSR_r15 {
	int8_t p_C_r15;
	BIT_STRING codebookSubsetRestriction4_r15;
};

struct P_C_AndCBSR_r15_P_C_AndCBSR_Pair_r15_Dynamic {
	size_t d;
	struct P_C_AndCBSR_r15* v;
};

typedef struct P_C_AndCBSR_r15_P_C_AndCBSR_Pair_r15_Dynamic P_C_AndCBSR_Pair_r15;

struct P_C_AndCBSR_Pair_r15_CSI_RS_ConfigNonPrecoded_v1530_p_C_AndCBSRList_r15_Optional {
	bool d;
	P_C_AndCBSR_Pair_r15 v;
};

struct CSI_RS_ConfigNonPrecoded_v1530 {
	struct P_C_AndCBSR_Pair_r15_CSI_RS_ConfigNonPrecoded_v1530_p_C_AndCBSRList_r15_Optional p_C_AndCBSRList_r15;
};

enum CSI_RS_ConfigEMIMO_v1530_setup_Sel {
	CSI_RS_ConfigEMIMO_v1530_setup_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1530_setup_nonPrecoded_v1530 = 1,
};

union CSI_RS_ConfigEMIMO_v1530_setup_Value {
	struct CSI_RS_ConfigNonPrecoded_v1530 nonPrecoded_v1530;
};

struct CSI_RS_ConfigEMIMO_v1530_setup {
	enum CSI_RS_ConfigEMIMO_v1530_setup_Sel d;
	union CSI_RS_ConfigEMIMO_v1530_setup_Value v;
};

enum CSI_RS_ConfigEMIMO_v1530_Sel {
	CSI_RS_ConfigEMIMO_v1530_UNBOUND_VALUE = 0,
	CSI_RS_ConfigEMIMO_v1530_release = 1,
	CSI_RS_ConfigEMIMO_v1530_setup = 2,
};

union CSI_RS_ConfigEMIMO_v1530_Value {
	Null release;
	struct CSI_RS_ConfigEMIMO_v1530_setup setup;
};

struct CSI_RS_ConfigEMIMO_v1530 {
	enum CSI_RS_ConfigEMIMO_v1530_Sel d;
	union CSI_RS_ConfigEMIMO_v1530_Value v;
};

struct CQI_ReportBothProc_r11_CSI_Process_r11_cqi_ReportBothProc_r11_Optional {
	bool d;
	struct CQI_ReportBothProc_r11 v;
};

struct uint8_t_CSI_Process_r11_cqi_ReportPeriodicProcId_r11_Optional {
	bool d;
	uint8_t v;
};

struct CQI_ReportAperiodicProc_r11_CSI_Process_r11_cqi_ReportAperiodicProc_r11_Optional {
	bool d;
	struct CQI_ReportAperiodicProc_r11 v;
};

struct CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e_alternativeCodebookEnabledFor4TXProc_r12_Optional {
	bool d;
	CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e v;
};

struct CSI_Process_r11_csi_IM_ConfigIdList_r12_csi_IM_ConfigIdList_r12_Optional {
	bool d;
	struct CSI_Process_r11_csi_IM_ConfigIdList_r12 v;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc2_r12_cqi_ReportAperiodicProc2_r12_Optional {
	bool d;
	struct CSI_Process_r11_cqi_ReportAperiodicProc2_r12 v;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc_v1310_cqi_ReportAperiodicProc_v1310_Optional {
	bool d;
	struct CSI_Process_r11_cqi_ReportAperiodicProc_v1310 v;
};

struct CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_cqi_ReportAperiodicProc2_v1310_Optional {
	bool d;
	struct CSI_Process_r11_cqi_ReportAperiodicProc2_v1310 v;
};

struct CSI_RS_ConfigEMIMO_r13_CSI_Process_r11_eMIMO_Type_r13_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO_r13 v;
};

struct CSI_RS_ConfigEMIMO_v1430_CSI_Process_r11_dummy_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO_v1430 v;
};

struct CSI_RS_ConfigEMIMO_Hybrid_r14_CSI_Process_r11_eMIMO_Hybrid_r14_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO_Hybrid_r14 v;
};

struct bool_CSI_Process_r11_advancedCodebookEnabled_r14_Optional {
	bool d;
	bool v;
};

struct CSI_RS_ConfigEMIMO_v1480_CSI_Process_r11_eMIMO_Type_v1480_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO_v1480 v;
};

struct bool_CSI_Process_r11_feCOMP_CSI_Enabled_v1530_Optional {
	bool d;
	bool v;
};

struct CSI_RS_ConfigEMIMO_v1530_CSI_Process_r11_eMIMO_Type_v1530_Optional {
	bool d;
	struct CSI_RS_ConfigEMIMO_v1530 v;
};

struct CSI_Process_r11 {
	CSI_ProcessId_r11 csi_ProcessId_r11;
	CSI_RS_ConfigNZPId_r11 csi_RS_ConfigNZPId_r11;
	CSI_IM_ConfigId_r11 csi_IM_ConfigId_r11;
	P_C_AndCBSR_Pair_r13a p_C_AndCBSRList_r11;
	struct CQI_ReportBothProc_r11_CSI_Process_r11_cqi_ReportBothProc_r11_Optional cqi_ReportBothProc_r11;
	struct uint8_t_CSI_Process_r11_cqi_ReportPeriodicProcId_r11_Optional cqi_ReportPeriodicProcId_r11;
	struct CQI_ReportAperiodicProc_r11_CSI_Process_r11_cqi_ReportAperiodicProc_r11_Optional cqi_ReportAperiodicProc_r11;
	struct CSI_Process_r11_alternativeCodebookEnabledFor4TXProc_r12_e_alternativeCodebookEnabledFor4TXProc_r12_Optional alternativeCodebookEnabledFor4TXProc_r12;
	struct CSI_Process_r11_csi_IM_ConfigIdList_r12_csi_IM_ConfigIdList_r12_Optional csi_IM_ConfigIdList_r12;
	struct CSI_Process_r11_cqi_ReportAperiodicProc2_r12_cqi_ReportAperiodicProc2_r12_Optional cqi_ReportAperiodicProc2_r12;
	struct CSI_Process_r11_cqi_ReportAperiodicProc_v1310_cqi_ReportAperiodicProc_v1310_Optional cqi_ReportAperiodicProc_v1310;
	struct CSI_Process_r11_cqi_ReportAperiodicProc2_v1310_cqi_ReportAperiodicProc2_v1310_Optional cqi_ReportAperiodicProc2_v1310;
	struct CSI_RS_ConfigEMIMO_r13_CSI_Process_r11_eMIMO_Type_r13_Optional eMIMO_Type_r13;
	struct CSI_RS_ConfigEMIMO_v1430_CSI_Process_r11_dummy_Optional dummy;
	struct CSI_RS_ConfigEMIMO_Hybrid_r14_CSI_Process_r11_eMIMO_Hybrid_r14_Optional eMIMO_Hybrid_r14;
	struct bool_CSI_Process_r11_advancedCodebookEnabled_r14_Optional advancedCodebookEnabled_r14;
	struct CSI_RS_ConfigEMIMO_v1480_CSI_Process_r11_eMIMO_Type_v1480_Optional eMIMO_Type_v1480;
	struct bool_CSI_Process_r11_feCOMP_CSI_Enabled_v1530_Optional feCOMP_CSI_Enabled_v1530;
	struct CSI_RS_ConfigEMIMO_v1530_CSI_Process_r11_eMIMO_Type_v1530_Optional eMIMO_Type_v1530;
};

struct CSI_Process_r11_CSI_ProcessToAddModList_r11_Dynamic {
	size_t d;
	struct CSI_Process_r11* v;
};

typedef struct CSI_Process_r11_CSI_ProcessToAddModList_r11_Dynamic CSI_ProcessToAddModList_r11;

struct CSI_IM_ConfigToReleaseList_r11_CQI_ReportBoth_r11_csi_IM_ConfigToReleaseList_r11_Optional {
	bool d;
	CSI_IM_ConfigToReleaseList_r11 v;
};

struct CSI_IM_ConfigToAddModList_r11_CQI_ReportBoth_r11_csi_IM_ConfigToAddModList_r11_Optional {
	bool d;
	CSI_IM_ConfigToAddModList_r11 v;
};

struct CSI_ProcessToReleaseList_r11_CQI_ReportBoth_r11_csi_ProcessToReleaseList_r11_Optional {
	bool d;
	CSI_ProcessToReleaseList_r11 v;
};

struct CSI_ProcessToAddModList_r11_CQI_ReportBoth_r11_csi_ProcessToAddModList_r11_Optional {
	bool d;
	CSI_ProcessToAddModList_r11 v;
};

struct CQI_ReportBoth_r11 {
	struct CSI_IM_ConfigToReleaseList_r11_CQI_ReportBoth_r11_csi_IM_ConfigToReleaseList_r11_Optional csi_IM_ConfigToReleaseList_r11;
	struct CSI_IM_ConfigToAddModList_r11_CQI_ReportBoth_r11_csi_IM_ConfigToAddModList_r11_Optional csi_IM_ConfigToAddModList_r11;
	struct CSI_ProcessToReleaseList_r11_CQI_ReportBoth_r11_csi_ProcessToReleaseList_r11_Optional csi_ProcessToReleaseList_r11;
	struct CSI_ProcessToAddModList_r11_CQI_ReportBoth_r11_csi_ProcessToAddModList_r11_Optional csi_ProcessToAddModList_r11;
};

struct CQI_ReportConfig_v1130 {
	struct CQI_ReportPeriodic_v1130 cqi_ReportPeriodic_v1130;
	struct CQI_ReportBoth_r11 cqi_ReportBoth_r11;
};

struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_setup {
	B10 csi_MeasSubframeSets_r12;
};

enum CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_Sel {
	CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_UNBOUND_VALUE = 0,
	CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_release = 1,
	CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_setup = 2,
};

union CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_Value {
	Null release;
	struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_setup setup;
};

struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12 {
	enum CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_Sel d;
	union CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_Value v;
};

typedef uint8_t CSI_IM_ConfigId_v1250;

typedef uint8_t CSI_IM_ConfigId_v1310;

struct bool_CSI_IM_ConfigExt_r12_interferenceMeasRestriction_r13_Optional {
	bool d;
	bool v;
};

struct CSI_IM_ConfigId_v1310_CSI_IM_ConfigExt_r12_csi_IM_ConfigId_v1310_Optional {
	bool d;
	CSI_IM_ConfigId_v1310 v;
};

struct CSI_IM_ConfigExt_r12 {
	CSI_IM_ConfigId_v1250 csi_IM_ConfigId_v1250;
	uint8_t resourceConfig_r12;
	uint8_t subframeConfig_r12;
	struct bool_CSI_IM_ConfigExt_r12_interferenceMeasRestriction_r13_Optional interferenceMeasRestriction_r13;
	struct CSI_IM_ConfigId_v1310_CSI_IM_ConfigExt_r12_csi_IM_ConfigId_v1310_Optional csi_IM_ConfigId_v1310;
};

struct CSI_IM_ConfigId_v1250_CQI_ReportBoth_v1250_csi_IM_ConfigToReleaseListExt_r12_Optional {
	bool d;
	CSI_IM_ConfigId_v1250 v;
};

struct CSI_IM_ConfigExt_r12_CQI_ReportBoth_v1250_csi_IM_ConfigToAddModListExt_r12_Optional {
	bool d;
	struct CSI_IM_ConfigExt_r12 v;
};

struct CQI_ReportBoth_v1250 {
	struct CSI_IM_ConfigId_v1250_CQI_ReportBoth_v1250_csi_IM_ConfigToReleaseListExt_r12_Optional csi_IM_ConfigToReleaseListExt_r12;
	struct CSI_IM_ConfigExt_r12_CQI_ReportBoth_v1250_csi_IM_ConfigToAddModListExt_r12_Optional csi_IM_ConfigToAddModListExt_r12;
};

enum CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e {
	CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e_s1 = 0,
	CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e_s2 = 1,
};

typedef enum CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e;

struct CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250 {
	CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250_trigger_SubframeSetIndicator_r12_e trigger_SubframeSetIndicator_r12;
	B8 trigger1_SubframeSetIndicator_r12;
	B8 trigger2_SubframeSetIndicator_r12;
};

struct CQI_ReportAperiodic_v1250_setup {
	struct CQI_ReportAperiodic_v1250_setup_aperiodicCSI_Trigger_v1250 aperiodicCSI_Trigger_v1250;
};

enum CQI_ReportAperiodic_v1250_Sel {
	CQI_ReportAperiodic_v1250_UNBOUND_VALUE = 0,
	CQI_ReportAperiodic_v1250_release = 1,
	CQI_ReportAperiodic_v1250_setup = 2,
};

union CQI_ReportAperiodic_v1250_Value {
	Null release;
	struct CQI_ReportAperiodic_v1250_setup setup;
};

struct CQI_ReportAperiodic_v1250 {
	enum CQI_ReportAperiodic_v1250_Sel d;
	union CQI_ReportAperiodic_v1250_Value v;
};

enum CQI_ReportConfig_v1250_altCQI_Table_r12_e {
	CQI_ReportConfig_v1250_altCQI_Table_r12_e_allSubframes = 0,
	CQI_ReportConfig_v1250_altCQI_Table_r12_e_csi_SubframeSet1 = 1,
	CQI_ReportConfig_v1250_altCQI_Table_r12_e_csi_SubframeSet2 = 2,
	CQI_ReportConfig_v1250_altCQI_Table_r12_e_spare1 = 3,
};

typedef enum CQI_ReportConfig_v1250_altCQI_Table_r12_e CQI_ReportConfig_v1250_altCQI_Table_r12_e;

struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_csi_SubframePatternConfig_r12_Optional {
	bool d;
	struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12 v;
};

struct CQI_ReportBoth_v1250_CQI_ReportConfig_v1250_cqi_ReportBoth_v1250_Optional {
	bool d;
	struct CQI_ReportBoth_v1250 v;
};

struct CQI_ReportAperiodic_v1250_CQI_ReportConfig_v1250_cqi_ReportAperiodic_v1250_Optional {
	bool d;
	struct CQI_ReportAperiodic_v1250 v;
};

struct CQI_ReportConfig_v1250_altCQI_Table_r12_e_altCQI_Table_r12_Optional {
	bool d;
	CQI_ReportConfig_v1250_altCQI_Table_r12_e v;
};

struct CQI_ReportConfig_v1250 {
	struct CQI_ReportConfig_v1250_csi_SubframePatternConfig_r12_csi_SubframePatternConfig_r12_Optional csi_SubframePatternConfig_r12;
	struct CQI_ReportBoth_v1250_CQI_ReportConfig_v1250_cqi_ReportBoth_v1250_Optional cqi_ReportBoth_v1250;
	struct CQI_ReportAperiodic_v1250_CQI_ReportConfig_v1250_cqi_ReportAperiodic_v1250_Optional cqi_ReportAperiodic_v1250;
	struct CQI_ReportConfig_v1250_altCQI_Table_r12_e_altCQI_Table_r12_Optional altCQI_Table_r12;
};

enum CQI_ReportConfig_v920_cqi_Mask_r9_e {
	CQI_ReportConfig_v920_cqi_Mask_r9_e_setup = 0,
};

typedef enum CQI_ReportConfig_v920_cqi_Mask_r9_e CQI_ReportConfig_v920_cqi_Mask_r9_e;

enum CQI_ReportConfig_v920_pmi_RI_Report_r9_e {
	CQI_ReportConfig_v920_pmi_RI_Report_r9_e_setup = 0,
};

typedef enum CQI_ReportConfig_v920_pmi_RI_Report_r9_e CQI_ReportConfig_v920_pmi_RI_Report_r9_e;

struct CQI_ReportConfig_v920_cqi_Mask_r9_e_cqi_Mask_r9_Optional {
	bool d;
	CQI_ReportConfig_v920_cqi_Mask_r9_e v;
};

struct CQI_ReportConfig_v920_pmi_RI_Report_r9_e_pmi_RI_Report_r9_Optional {
	bool d;
	CQI_ReportConfig_v920_pmi_RI_Report_r9_e v;
};

struct CQI_ReportConfig_v920 {
	struct CQI_ReportConfig_v920_cqi_Mask_r9_e_cqi_Mask_r9_Optional cqi_Mask_r9;
	struct CQI_ReportConfig_v920_pmi_RI_Report_r9_e_pmi_RI_Report_r9_Optional pmi_RI_Report_r9;
};

enum CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e {
	CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e_an1 = 0,
	CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e_an2 = 1,
	CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e_an4 = 2,
	CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e_an8 = 3,
};

typedef enum CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e;

struct CSI_RS_Config_r10_csi_RS_r10_setup {
	CSI_RS_Config_r10_csi_RS_r10_setup_antennaPortsCount_r10_e antennaPortsCount_r10;
	uint8_t resourceConfig_r10;
	uint8_t subframeConfig_r10;
	int8_t p_C_r10;
};

enum CSI_RS_Config_r10_csi_RS_r10_Sel {
	CSI_RS_Config_r10_csi_RS_r10_UNBOUND_VALUE = 0,
	CSI_RS_Config_r10_csi_RS_r10_release = 1,
	CSI_RS_Config_r10_csi_RS_r10_setup = 2,
};

union CSI_RS_Config_r10_csi_RS_r10_Value {
	Null release;
	struct CSI_RS_Config_r10_csi_RS_r10_setup setup;
};

struct CSI_RS_Config_r10_csi_RS_r10 {
	enum CSI_RS_Config_r10_csi_RS_r10_Sel d;
	union CSI_RS_Config_r10_csi_RS_r10_Value v;
};

struct ZeroTxPowerCSI_RS_r12 {
	B16 zeroTxPowerResourceConfigList_r12;
	uint8_t zeroTxPowerSubframeConfig_r12;
};

enum ZeroTxPowerCSI_RS_Conf_r12_Sel {
	ZeroTxPowerCSI_RS_Conf_r12_UNBOUND_VALUE = 0,
	ZeroTxPowerCSI_RS_Conf_r12_release = 1,
	ZeroTxPowerCSI_RS_Conf_r12_setup = 2,
};

union ZeroTxPowerCSI_RS_Conf_r12_Value {
	Null release;
	struct ZeroTxPowerCSI_RS_r12 setup;
};

struct ZeroTxPowerCSI_RS_Conf_r12 {
	enum ZeroTxPowerCSI_RS_Conf_r12_Sel d;
	union ZeroTxPowerCSI_RS_Conf_r12_Value v;
};

struct CSI_RS_Config_r10_csi_RS_r10_csi_RS_r10_Optional {
	bool d;
	struct CSI_RS_Config_r10_csi_RS_r10 v;
};

struct ZeroTxPowerCSI_RS_Conf_r12_CSI_RS_Config_r10_zeroTxPowerCSI_RS_r10_Optional {
	bool d;
	struct ZeroTxPowerCSI_RS_Conf_r12 v;
};

struct CSI_RS_Config_r10 {
	struct CSI_RS_Config_r10_csi_RS_r10_csi_RS_r10_Optional csi_RS_r10;
	struct ZeroTxPowerCSI_RS_Conf_r12_CSI_RS_Config_r10_zeroTxPowerCSI_RS_r10_Optional zeroTxPowerCSI_RS_r10;
};

struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigNZPToAddModList_r11_Dynamic {
	size_t d;
	struct CSI_RS_ConfigNZP_r11* v;
};

typedef struct CSI_RS_ConfigNZP_r11_CSI_RS_ConfigNZPToAddModList_r11_Dynamic CSI_RS_ConfigNZPToAddModList_r11;

struct CSI_RS_ConfigNZPId_r11_CSI_RS_ConfigNZPToReleaseList_r11_Dynamic {
	size_t d;
	CSI_RS_ConfigNZPId_r11* v;
};

typedef struct CSI_RS_ConfigNZPId_r11_CSI_RS_ConfigNZPToReleaseList_r11_Dynamic CSI_RS_ConfigNZPToReleaseList_r11;

typedef uint8_t CSI_RS_ConfigZPId_r11;

struct CSI_RS_ConfigZP_r11 {
	CSI_RS_ConfigZPId_r11 csi_RS_ConfigZPId_r11;
	B16 resourceConfigList_r11;
	uint8_t subframeConfig_r11;
};

struct CSI_RS_ConfigZP_r11_CSI_RS_ConfigZPToAddModList_r11_Dynamic {
	size_t d;
	struct CSI_RS_ConfigZP_r11* v;
};

typedef struct CSI_RS_ConfigZP_r11_CSI_RS_ConfigZPToAddModList_r11_Dynamic CSI_RS_ConfigZPToAddModList_r11;

struct CSI_RS_ConfigZPId_r11_CSI_RS_ConfigZPToReleaseList_r11_Dynamic {
	size_t d;
	CSI_RS_ConfigZPId_r11* v;
};

typedef struct CSI_RS_ConfigZPId_r11_CSI_RS_ConfigZPToReleaseList_r11_Dynamic CSI_RS_ConfigZPToReleaseList_r11;

enum CarrierBandwidthEUTRA_dl_Bandwidth_e {
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n6 = 0,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n15 = 1,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n25 = 2,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n50 = 3,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n75 = 4,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_n100 = 5,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare10 = 6,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare9 = 7,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare8 = 8,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare7 = 9,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare6 = 10,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare5 = 11,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare4 = 12,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare3 = 13,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare2 = 14,
	CarrierBandwidthEUTRA_dl_Bandwidth_e_spare1 = 15,
};

typedef enum CarrierBandwidthEUTRA_dl_Bandwidth_e CarrierBandwidthEUTRA_dl_Bandwidth_e;

enum CarrierBandwidthEUTRA_ul_Bandwidth_e {
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n6 = 0,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n15 = 1,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n25 = 2,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n50 = 3,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n75 = 4,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_n100 = 5,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare10 = 6,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare9 = 7,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare8 = 8,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare7 = 9,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare6 = 10,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare5 = 11,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare4 = 12,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare3 = 13,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare2 = 14,
	CarrierBandwidthEUTRA_ul_Bandwidth_e_spare1 = 15,
};

typedef enum CarrierBandwidthEUTRA_ul_Bandwidth_e CarrierBandwidthEUTRA_ul_Bandwidth_e;

enum CipheringAlgorithm_r12_e {
	CipheringAlgorithm_r12_e_eea0 = 0,
	CipheringAlgorithm_r12_e_eea1 = 1,
	CipheringAlgorithm_r12_e_eea2 = 2,
	CipheringAlgorithm_r12_e_eea3_v1130 = 3,
	CipheringAlgorithm_r12_e_spare4 = 4,
	CipheringAlgorithm_r12_e_spare3 = 5,
	CipheringAlgorithm_r12_e_spare2 = 6,
	CipheringAlgorithm_r12_e_spare1 = 7,
};

typedef enum CipheringAlgorithm_r12_e CipheringAlgorithm_r12_e;

struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_own_r10 {
	bool cif_Presence_r10;
};

typedef uint8_t ServCellIndex_r10;

struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_other_r10 {
	ServCellIndex_r10 schedulingCellId_r10;
	uint8_t pdsch_Start_r10;
};

enum CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_Sel {
	CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_UNBOUND_VALUE = 0,
	CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_own_r10 = 1,
	CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_other_r10 = 2,
};

union CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_Value {
	struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_own_r10 own_r10;
	struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_other_r10 other_r10;
};

struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10 {
	enum CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_Sel d;
	union CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10_Value v;
};

struct CrossCarrierSchedulingConfig_r10 {
	struct CrossCarrierSchedulingConfig_r10_schedulingCellInfo_r10 schedulingCellInfo_r10;
};

struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_own_r13 {
	bool cif_Presence_r13;
};

typedef uint8_t ServCellIndex_r13;

struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_other_r13 {
	ServCellIndex_r13 schedulingCellId_r13;
	uint8_t pdsch_Start_r13;
	uint8_t cif_InSchedulingCell_r13;
};

enum CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_Sel {
	CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_UNBOUND_VALUE = 0,
	CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_own_r13 = 1,
	CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_other_r13 = 2,
};

union CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_Value {
	struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_own_r13 own_r13;
	struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_other_r13 other_r13;
};

struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13 {
	enum CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_Sel d;
	union CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13_Value v;
};

struct CrossCarrierSchedulingConfig_r13 {
	struct CrossCarrierSchedulingConfig_r13_schedulingCellInfo_r13 schedulingCellInfo_r13;
};

enum T_Reordering_e {
	T_Reordering_e_ms0 = 0,
	T_Reordering_e_ms5 = 1,
	T_Reordering_e_ms10 = 2,
	T_Reordering_e_ms15 = 3,
	T_Reordering_e_ms20 = 4,
	T_Reordering_e_ms25 = 5,
	T_Reordering_e_ms30 = 6,
	T_Reordering_e_ms35 = 7,
	T_Reordering_e_ms40 = 8,
	T_Reordering_e_ms45 = 9,
	T_Reordering_e_ms50 = 10,
	T_Reordering_e_ms55 = 11,
	T_Reordering_e_ms60 = 12,
	T_Reordering_e_ms65 = 13,
	T_Reordering_e_ms70 = 14,
	T_Reordering_e_ms75 = 15,
	T_Reordering_e_ms80 = 16,
	T_Reordering_e_ms85 = 17,
	T_Reordering_e_ms90 = 18,
	T_Reordering_e_ms95 = 19,
	T_Reordering_e_ms100 = 20,
	T_Reordering_e_ms110 = 21,
	T_Reordering_e_ms120 = 22,
	T_Reordering_e_ms130 = 23,
	T_Reordering_e_ms140 = 24,
	T_Reordering_e_ms150 = 25,
	T_Reordering_e_ms160 = 26,
	T_Reordering_e_ms170 = 27,
	T_Reordering_e_ms180 = 28,
	T_Reordering_e_ms190 = 29,
	T_Reordering_e_ms200 = 30,
	T_Reordering_e_ms1600_v1310 = 31,
};

typedef enum T_Reordering_e T_Reordering_e;

enum T_StatusProhibit_e {
	T_StatusProhibit_e_ms0 = 0,
	T_StatusProhibit_e_ms5 = 1,
	T_StatusProhibit_e_ms10 = 2,
	T_StatusProhibit_e_ms15 = 3,
	T_StatusProhibit_e_ms20 = 4,
	T_StatusProhibit_e_ms25 = 5,
	T_StatusProhibit_e_ms30 = 6,
	T_StatusProhibit_e_ms35 = 7,
	T_StatusProhibit_e_ms40 = 8,
	T_StatusProhibit_e_ms45 = 9,
	T_StatusProhibit_e_ms50 = 10,
	T_StatusProhibit_e_ms55 = 11,
	T_StatusProhibit_e_ms60 = 12,
	T_StatusProhibit_e_ms65 = 13,
	T_StatusProhibit_e_ms70 = 14,
	T_StatusProhibit_e_ms75 = 15,
	T_StatusProhibit_e_ms80 = 16,
	T_StatusProhibit_e_ms85 = 17,
	T_StatusProhibit_e_ms90 = 18,
	T_StatusProhibit_e_ms95 = 19,
	T_StatusProhibit_e_ms100 = 20,
	T_StatusProhibit_e_ms105 = 21,
	T_StatusProhibit_e_ms110 = 22,
	T_StatusProhibit_e_ms115 = 23,
	T_StatusProhibit_e_ms120 = 24,
	T_StatusProhibit_e_ms125 = 25,
	T_StatusProhibit_e_ms130 = 26,
	T_StatusProhibit_e_ms135 = 27,
	T_StatusProhibit_e_ms140 = 28,
	T_StatusProhibit_e_ms145 = 29,
	T_StatusProhibit_e_ms150 = 30,
	T_StatusProhibit_e_ms155 = 31,
	T_StatusProhibit_e_ms160 = 32,
	T_StatusProhibit_e_ms165 = 33,
	T_StatusProhibit_e_ms170 = 34,
	T_StatusProhibit_e_ms175 = 35,
	T_StatusProhibit_e_ms180 = 36,
	T_StatusProhibit_e_ms185 = 37,
	T_StatusProhibit_e_ms190 = 38,
	T_StatusProhibit_e_ms195 = 39,
	T_StatusProhibit_e_ms200 = 40,
	T_StatusProhibit_e_ms205 = 41,
	T_StatusProhibit_e_ms210 = 42,
	T_StatusProhibit_e_ms215 = 43,
	T_StatusProhibit_e_ms220 = 44,
	T_StatusProhibit_e_ms225 = 45,
	T_StatusProhibit_e_ms230 = 46,
	T_StatusProhibit_e_ms235 = 47,
	T_StatusProhibit_e_ms240 = 48,
	T_StatusProhibit_e_ms245 = 49,
	T_StatusProhibit_e_ms250 = 50,
	T_StatusProhibit_e_ms300 = 51,
	T_StatusProhibit_e_ms350 = 52,
	T_StatusProhibit_e_ms400 = 53,
	T_StatusProhibit_e_ms450 = 54,
	T_StatusProhibit_e_ms500 = 55,
	T_StatusProhibit_e_ms800_v1310 = 56,
	T_StatusProhibit_e_ms1000_v1310 = 57,
	T_StatusProhibit_e_ms1200_v1310 = 58,
	T_StatusProhibit_e_ms1600_v1310 = 59,
	T_StatusProhibit_e_ms2000_v1310 = 60,
	T_StatusProhibit_e_ms2400_v1310 = 61,
	T_StatusProhibit_e_spare2 = 62,
	T_StatusProhibit_e_spare1 = 63,
};

typedef enum T_StatusProhibit_e T_StatusProhibit_e;

struct DL_AM_RLC {
	T_Reordering_e t_Reordering;
	T_StatusProhibit_e t_StatusProhibit;
};

enum SN_FieldLength_e {
	SN_FieldLength_e_size5 = 0,
	SN_FieldLength_e_size10 = 1,
};

typedef enum SN_FieldLength_e SN_FieldLength_e;

struct DL_UM_RLC {
	SN_FieldLength_e sn_FieldLength;
	T_Reordering_e t_Reordering;
};

enum DRX_Config_setup_onDurationTimer_e {
	DRX_Config_setup_onDurationTimer_e_psf1 = 0,
	DRX_Config_setup_onDurationTimer_e_psf2 = 1,
	DRX_Config_setup_onDurationTimer_e_psf3 = 2,
	DRX_Config_setup_onDurationTimer_e_psf4 = 3,
	DRX_Config_setup_onDurationTimer_e_psf5 = 4,
	DRX_Config_setup_onDurationTimer_e_psf6 = 5,
	DRX_Config_setup_onDurationTimer_e_psf8 = 6,
	DRX_Config_setup_onDurationTimer_e_psf10 = 7,
	DRX_Config_setup_onDurationTimer_e_psf20 = 8,
	DRX_Config_setup_onDurationTimer_e_psf30 = 9,
	DRX_Config_setup_onDurationTimer_e_psf40 = 10,
	DRX_Config_setup_onDurationTimer_e_psf50 = 11,
	DRX_Config_setup_onDurationTimer_e_psf60 = 12,
	DRX_Config_setup_onDurationTimer_e_psf80 = 13,
	DRX_Config_setup_onDurationTimer_e_psf100 = 14,
	DRX_Config_setup_onDurationTimer_e_psf200 = 15,
};

typedef enum DRX_Config_setup_onDurationTimer_e DRX_Config_setup_onDurationTimer_e;

enum DRX_Config_setup_drx_InactivityTimer_e {
	DRX_Config_setup_drx_InactivityTimer_e_psf1 = 0,
	DRX_Config_setup_drx_InactivityTimer_e_psf2 = 1,
	DRX_Config_setup_drx_InactivityTimer_e_psf3 = 2,
	DRX_Config_setup_drx_InactivityTimer_e_psf4 = 3,
	DRX_Config_setup_drx_InactivityTimer_e_psf5 = 4,
	DRX_Config_setup_drx_InactivityTimer_e_psf6 = 5,
	DRX_Config_setup_drx_InactivityTimer_e_psf8 = 6,
	DRX_Config_setup_drx_InactivityTimer_e_psf10 = 7,
	DRX_Config_setup_drx_InactivityTimer_e_psf20 = 8,
	DRX_Config_setup_drx_InactivityTimer_e_psf30 = 9,
	DRX_Config_setup_drx_InactivityTimer_e_psf40 = 10,
	DRX_Config_setup_drx_InactivityTimer_e_psf50 = 11,
	DRX_Config_setup_drx_InactivityTimer_e_psf60 = 12,
	DRX_Config_setup_drx_InactivityTimer_e_psf80 = 13,
	DRX_Config_setup_drx_InactivityTimer_e_psf100 = 14,
	DRX_Config_setup_drx_InactivityTimer_e_psf200 = 15,
	DRX_Config_setup_drx_InactivityTimer_e_psf300 = 16,
	DRX_Config_setup_drx_InactivityTimer_e_psf500 = 17,
	DRX_Config_setup_drx_InactivityTimer_e_psf750 = 18,
	DRX_Config_setup_drx_InactivityTimer_e_psf1280 = 19,
	DRX_Config_setup_drx_InactivityTimer_e_psf1920 = 20,
	DRX_Config_setup_drx_InactivityTimer_e_psf2560 = 21,
	DRX_Config_setup_drx_InactivityTimer_e_psf0_v1020 = 22,
	DRX_Config_setup_drx_InactivityTimer_e_spare9 = 23,
	DRX_Config_setup_drx_InactivityTimer_e_spare8 = 24,
	DRX_Config_setup_drx_InactivityTimer_e_spare7 = 25,
	DRX_Config_setup_drx_InactivityTimer_e_spare6 = 26,
	DRX_Config_setup_drx_InactivityTimer_e_spare5 = 27,
	DRX_Config_setup_drx_InactivityTimer_e_spare4 = 28,
	DRX_Config_setup_drx_InactivityTimer_e_spare3 = 29,
	DRX_Config_setup_drx_InactivityTimer_e_spare2 = 30,
	DRX_Config_setup_drx_InactivityTimer_e_spare1 = 31,
};

typedef enum DRX_Config_setup_drx_InactivityTimer_e DRX_Config_setup_drx_InactivityTimer_e;

enum DRX_Config_setup_drx_RetransmissionTimer_e {
	DRX_Config_setup_drx_RetransmissionTimer_e_psf1 = 0,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf2 = 1,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf4 = 2,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf6 = 3,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf8 = 4,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf16 = 5,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf24 = 6,
	DRX_Config_setup_drx_RetransmissionTimer_e_psf33 = 7,
};

typedef enum DRX_Config_setup_drx_RetransmissionTimer_e DRX_Config_setup_drx_RetransmissionTimer_e;

enum DRX_Config_setup_longDRX_CycleStartOffset_Sel {
	DRX_Config_setup_longDRX_CycleStartOffset_UNBOUND_VALUE = 0,
	DRX_Config_setup_longDRX_CycleStartOffset_sf10 = 1,
	DRX_Config_setup_longDRX_CycleStartOffset_sf20 = 2,
	DRX_Config_setup_longDRX_CycleStartOffset_sf32 = 3,
	DRX_Config_setup_longDRX_CycleStartOffset_sf40 = 4,
	DRX_Config_setup_longDRX_CycleStartOffset_sf64 = 5,
	DRX_Config_setup_longDRX_CycleStartOffset_sf80 = 6,
	DRX_Config_setup_longDRX_CycleStartOffset_sf128 = 7,
	DRX_Config_setup_longDRX_CycleStartOffset_sf160 = 8,
	DRX_Config_setup_longDRX_CycleStartOffset_sf256 = 9,
	DRX_Config_setup_longDRX_CycleStartOffset_sf320 = 10,
	DRX_Config_setup_longDRX_CycleStartOffset_sf512 = 11,
	DRX_Config_setup_longDRX_CycleStartOffset_sf640 = 12,
	DRX_Config_setup_longDRX_CycleStartOffset_sf1024 = 13,
	DRX_Config_setup_longDRX_CycleStartOffset_sf1280 = 14,
	DRX_Config_setup_longDRX_CycleStartOffset_sf2048 = 15,
	DRX_Config_setup_longDRX_CycleStartOffset_sf2560 = 16,
};

union DRX_Config_setup_longDRX_CycleStartOffset_Value {
	uint8_t sf10;
	uint8_t sf20;
	uint8_t sf32;
	uint8_t sf40;
	uint8_t sf64;
	uint8_t sf80;
	uint8_t sf128;
	uint8_t sf160;
	uint8_t sf256;
	uint16_t sf320;
	uint16_t sf512;
	uint16_t sf640;
	uint16_t sf1024;
	uint16_t sf1280;
	uint16_t sf2048;
	uint16_t sf2560;
};

struct DRX_Config_setup_longDRX_CycleStartOffset {
	enum DRX_Config_setup_longDRX_CycleStartOffset_Sel d;
	union DRX_Config_setup_longDRX_CycleStartOffset_Value v;
};

enum DRX_Config_setup_shortDRX_shortDRX_Cycle_e {
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf2 = 0,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf5 = 1,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf8 = 2,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf10 = 3,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf16 = 4,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf20 = 5,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf32 = 6,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf40 = 7,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf64 = 8,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf80 = 9,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf128 = 10,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf160 = 11,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf256 = 12,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf320 = 13,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf512 = 14,
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e_sf640 = 15,
};

typedef enum DRX_Config_setup_shortDRX_shortDRX_Cycle_e DRX_Config_setup_shortDRX_shortDRX_Cycle_e;

struct DRX_Config_setup_shortDRX {
	DRX_Config_setup_shortDRX_shortDRX_Cycle_e shortDRX_Cycle;
	uint8_t drxShortCycleTimer;
};

struct DRX_Config_setup_shortDRX_shortDRX_Optional {
	bool d;
	struct DRX_Config_setup_shortDRX v;
};

struct DRX_Config_setup {
	DRX_Config_setup_onDurationTimer_e onDurationTimer;
	DRX_Config_setup_drx_InactivityTimer_e drx_InactivityTimer;
	DRX_Config_setup_drx_RetransmissionTimer_e drx_RetransmissionTimer;
	struct DRX_Config_setup_longDRX_CycleStartOffset longDRX_CycleStartOffset;
	struct DRX_Config_setup_shortDRX_shortDRX_Optional shortDRX;
};

enum DRX_Config_Sel {
	DRX_Config_UNBOUND_VALUE = 0,
	DRX_Config_release = 1,
	DRX_Config_setup = 2,
};

union DRX_Config_Value {
	Null release;
	struct DRX_Config_setup setup;
};

struct DRX_Config {
	enum DRX_Config_Sel d;
	union DRX_Config_Value v;
};

enum EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e {
	EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e_sf10 = 0,
	EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e_sf20 = 1,
	EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e_sf40 = 2,
	EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e_sf80 = 3,
};

typedef enum EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e;

struct EIMTA_MainConfig_r12_setup {
	C_RNTI eimta_RNTI_r12;
	EIMTA_MainConfig_r12_setup_eimta_CommandPeriodicity_r12_e eimta_CommandPeriodicity_r12;
	B10 eimta_CommandSubframeSet_r12;
};

enum EIMTA_MainConfig_r12_Sel {
	EIMTA_MainConfig_r12_UNBOUND_VALUE = 0,
	EIMTA_MainConfig_r12_release = 1,
	EIMTA_MainConfig_r12_setup = 2,
};

union EIMTA_MainConfig_r12_Value {
	Null release;
	struct EIMTA_MainConfig_r12_setup setup;
};

struct EIMTA_MainConfig_r12 {
	enum EIMTA_MainConfig_r12_Sel d;
	union EIMTA_MainConfig_r12_Value v;
};

enum EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e {
	EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e_sa2 = 0,
	EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e_sa4 = 1,
	EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e_sa5 = 2,
};

typedef enum EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e;

struct EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_setup {
	MBSFN_SubframeConfigList subframeConfigList_r12;
};

enum EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_Sel {
	EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_UNBOUND_VALUE = 0,
	EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_release = 1,
	EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_setup = 2,
};

union EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_Value {
	Null release;
	struct EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_setup setup;
};

struct EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250 {
	enum EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_Sel d;
	union EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250_Value v;
};

struct EIMTA_MainConfigServCell_r12_setup {
	uint8_t eimta_UL_DL_ConfigIndex_r12;
	EIMTA_MainConfigServCell_r12_setup_eimta_HARQ_ReferenceConfig_r12_e eimta_HARQ_ReferenceConfig_r12;
	struct EIMTA_MainConfigServCell_r12_setup_mbsfn_SubframeConfigList_v1250 mbsfn_SubframeConfigList_v1250;
};

enum EIMTA_MainConfigServCell_r12_Sel {
	EIMTA_MainConfigServCell_r12_UNBOUND_VALUE = 0,
	EIMTA_MainConfigServCell_r12_release = 1,
	EIMTA_MainConfigServCell_r12_setup = 2,
};

union EIMTA_MainConfigServCell_r12_Value {
	Null release;
	struct EIMTA_MainConfigServCell_r12_setup setup;
};

struct EIMTA_MainConfigServCell_r12 {
	enum EIMTA_MainConfigServCell_r12_Sel d;
	union EIMTA_MainConfigServCell_r12_Value v;
};

struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_setup {
	struct MeasSubframePattern_r10 subframePattern_r11;
};

enum EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_Sel {
	EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_UNBOUND_VALUE = 0,
	EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_release = 1,
	EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_setup = 2,
};

union EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_Value {
	Null release;
	struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_setup setup;
};

struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11 {
	enum EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_Sel d;
	union EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_Value v;
};

typedef uint8_t EPDCCH_SetConfigId_r11;

struct EPDCCH_SetConfigId_r11_EPDCCH_SetConfigToReleaseList_r11_Dynamic {
	size_t d;
	EPDCCH_SetConfigId_r11* v;
};

typedef struct EPDCCH_SetConfigId_r11_EPDCCH_SetConfigToReleaseList_r11_Dynamic EPDCCH_SetConfigToReleaseList_r11;

enum EPDCCH_SetConfig_r11_transmissionType_r11_e {
	EPDCCH_SetConfig_r11_transmissionType_r11_e_localised = 0,
	EPDCCH_SetConfig_r11_transmissionType_r11_e_distributed = 1,
};

typedef enum EPDCCH_SetConfig_r11_transmissionType_r11_e EPDCCH_SetConfig_r11_transmissionType_r11_e;

enum EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e {
	EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e_n2 = 0,
	EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e_n4 = 1,
	EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e_n8 = 2,
};

typedef enum EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e;

struct EPDCCH_SetConfig_r11_resourceBlockAssignment_r11 {
	EPDCCH_SetConfig_r11_resourceBlockAssignment_r11_numberPRB_Pairs_r11_e numberPRB_Pairs_r11;
	BIT_STRING resourceBlockAssignment_r11;
};

typedef uint8_t PDSCH_RE_MappingQCL_ConfigId_r11;

enum EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_Sel {
	EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_UNBOUND_VALUE = 0,
	EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_release = 1,
	EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_setup = 2,
};

union EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_Value {
	Null release;
	CSI_RS_ConfigZPId_r11 setup;
};

struct EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12 {
	enum EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_Sel d;
	union EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_Value v;
};

enum EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup_e {
	EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup_e_n6 = 0,
};

typedef enum EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup_e EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup_e;

enum EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_Sel {
	EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_UNBOUND_VALUE = 0,
	EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_release = 1,
	EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup = 2,
};

union EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_Value {
	Null release;
	EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_setup_e setup;
};

struct EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310 {
	enum EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_Sel d;
	union EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_Value v;
};

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf1 = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf2 = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf4 = 2,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf8 = 3,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf16 = 4,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e_sf32 = 5,
};

typedef enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e;

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e_on = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e_off = 1,
};

typedef enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e;

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v1 = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v1dot5 = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v2 = 2,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v2dot5 = 3,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v4 = 4,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v5 = 5,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v8 = 6,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e_v10 = 7,
};

typedef enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e;

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v1 = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v2 = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v4 = 2,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v5 = 3,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v8 = 4,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v10 = 5,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_v20 = 6,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e_spare1 = 7,
};

typedef enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e;

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_Sel {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_UNBOUND_VALUE = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13 = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13 = 2,
};

union EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_Value {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_fdd_r13_e fdd_r13;
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_tdd_r13_e tdd_r13;
};

struct EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13 {
	enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_Sel d;
	union EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13_Value v;
};

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r1 = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r2 = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r4 = 2,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r8 = 3,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r16 = 4,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r32 = 5,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r64 = 6,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r128 = 7,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e_r256 = 8,
};

typedef enum EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e;

struct EPDCCH_SetConfig_r11_mpdcch_config_r13_setup {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_csi_NumRepetitionCE_r13_e csi_NumRepetitionCE_r13;
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_pdsch_HoppingConfig_r13_e mpdcch_pdsch_HoppingConfig_r13;
	struct EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_StartSF_UESS_r13 mpdcch_StartSF_UESS_r13;
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup_mpdcch_NumRepetition_r13_e mpdcch_NumRepetition_r13;
	uint8_t mpdcch_Narrowband_r13;
};

enum EPDCCH_SetConfig_r11_mpdcch_config_r13_Sel {
	EPDCCH_SetConfig_r11_mpdcch_config_r13_UNBOUND_VALUE = 0,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_release = 1,
	EPDCCH_SetConfig_r11_mpdcch_config_r13_setup = 2,
};

union EPDCCH_SetConfig_r11_mpdcch_config_r13_Value {
	Null release;
	struct EPDCCH_SetConfig_r11_mpdcch_config_r13_setup setup;
};

struct EPDCCH_SetConfig_r11_mpdcch_config_r13 {
	enum EPDCCH_SetConfig_r11_mpdcch_config_r13_Sel d;
	union EPDCCH_SetConfig_r11_mpdcch_config_r13_Value v;
};

struct PDSCH_RE_MappingQCL_ConfigId_r11_EPDCCH_SetConfig_r11_re_MappingQCL_ConfigId_r11_Optional {
	bool d;
	PDSCH_RE_MappingQCL_ConfigId_r11 v;
};

struct EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_csi_RS_ConfigZPId2_r12_Optional {
	bool d;
	struct EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12 v;
};

struct EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_numberPRB_Pairs_v1310_Optional {
	bool d;
	struct EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310 v;
};

struct EPDCCH_SetConfig_r11_mpdcch_config_r13_mpdcch_config_r13_Optional {
	bool d;
	struct EPDCCH_SetConfig_r11_mpdcch_config_r13 v;
};

struct EPDCCH_SetConfig_r11 {
	EPDCCH_SetConfigId_r11 setConfigId_r11;
	EPDCCH_SetConfig_r11_transmissionType_r11_e transmissionType_r11;
	struct EPDCCH_SetConfig_r11_resourceBlockAssignment_r11 resourceBlockAssignment_r11;
	uint16_t dmrs_ScramblingSequenceInt_r11;
	uint16_t pucch_ResourceStartOffset_r11;
	struct PDSCH_RE_MappingQCL_ConfigId_r11_EPDCCH_SetConfig_r11_re_MappingQCL_ConfigId_r11_Optional re_MappingQCL_ConfigId_r11;
	struct EPDCCH_SetConfig_r11_csi_RS_ConfigZPId2_r12_csi_RS_ConfigZPId2_r12_Optional csi_RS_ConfigZPId2_r12;
	struct EPDCCH_SetConfig_r11_numberPRB_Pairs_v1310_numberPRB_Pairs_v1310_Optional numberPRB_Pairs_v1310;
	struct EPDCCH_SetConfig_r11_mpdcch_config_r13_mpdcch_config_r13_Optional mpdcch_config_r13;
};

struct EPDCCH_SetConfig_r11_EPDCCH_SetConfigToAddModList_r11_Dynamic {
	size_t d;
	struct EPDCCH_SetConfig_r11* v;
};

typedef struct EPDCCH_SetConfig_r11_EPDCCH_SetConfigToAddModList_r11_Dynamic EPDCCH_SetConfigToAddModList_r11;

struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_subframePatternConfig_r11_Optional {
	bool d;
	struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11 v;
};

struct uint8_t_EPDCCH_Config_r11_config_r11_setup_startSymbol_r11_Optional {
	bool d;
	uint8_t v;
};

struct EPDCCH_SetConfigToReleaseList_r11_EPDCCH_Config_r11_config_r11_setup_setConfigToReleaseList_r11_Optional {
	bool d;
	EPDCCH_SetConfigToReleaseList_r11 v;
};

struct EPDCCH_SetConfigToAddModList_r11_EPDCCH_Config_r11_config_r11_setup_setConfigToAddModList_r11_Optional {
	bool d;
	EPDCCH_SetConfigToAddModList_r11 v;
};

struct EPDCCH_Config_r11_config_r11_setup {
	struct EPDCCH_Config_r11_config_r11_setup_subframePatternConfig_r11_subframePatternConfig_r11_Optional subframePatternConfig_r11;
	struct uint8_t_EPDCCH_Config_r11_config_r11_setup_startSymbol_r11_Optional startSymbol_r11;
	struct EPDCCH_SetConfigToReleaseList_r11_EPDCCH_Config_r11_config_r11_setup_setConfigToReleaseList_r11_Optional setConfigToReleaseList_r11;
	struct EPDCCH_SetConfigToAddModList_r11_EPDCCH_Config_r11_config_r11_setup_setConfigToAddModList_r11_Optional setConfigToAddModList_r11;
};

enum EPDCCH_Config_r11_config_r11_Sel {
	EPDCCH_Config_r11_config_r11_UNBOUND_VALUE = 0,
	EPDCCH_Config_r11_config_r11_release = 1,
	EPDCCH_Config_r11_config_r11_setup = 2,
};

union EPDCCH_Config_r11_config_r11_Value {
	Null release;
	struct EPDCCH_Config_r11_config_r11_setup setup;
};

struct EPDCCH_Config_r11_config_r11 {
	enum EPDCCH_Config_r11_config_r11_Sel d;
	union EPDCCH_Config_r11_config_r11_Value v;
};

struct EPDCCH_Config_r11 {
	struct EPDCCH_Config_r11_config_r11 config_r11;
};

enum LAA_SCellConfiguration_r13_subframeStartPosition_r13_e {
	LAA_SCellConfiguration_r13_subframeStartPosition_r13_e_s0 = 0,
	LAA_SCellConfiguration_r13_subframeStartPosition_r13_e_s07 = 1,
};

typedef enum LAA_SCellConfiguration_r13_subframeStartPosition_r13_e LAA_SCellConfiguration_r13_subframeStartPosition_r13_e;

struct LAA_SCellConfiguration_r13 {
	LAA_SCellConfiguration_r13_subframeStartPosition_r13_e subframeStartPosition_r13;
	B8 laa_SCellSubframeConfig_r13;
};

struct CrossCarrierSchedulingConfigLAA_UL_r14 {
	ServCellIndex_r13 schedulingCellId_r14;
	uint8_t cif_InSchedulingCell_r14;
};

struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_setup {
	struct CrossCarrierSchedulingConfigLAA_UL_r14 crossCarrierSchedulingConfigLAA_UL_r14;
};

enum LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_Sel {
	LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_UNBOUND_VALUE = 0,
	LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_release = 1,
	LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_setup = 2,
};

union LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_Value {
	Null release;
	struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_setup setup;
};

struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14 {
	enum LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_Sel d;
	union LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_Value v;
};

enum LBT_Config_r14_Sel {
	LBT_Config_r14_UNBOUND_VALUE = 0,
	LBT_Config_r14_maxEnergyDetectionThreshold_r14 = 1,
	LBT_Config_r14_energyDetectionThresholdOffset_r14 = 2,
};

union LBT_Config_r14_Value {
	int8_t maxEnergyDetectionThreshold_r14;
	int8_t energyDetectionThresholdOffset_r14;
};

struct LBT_Config_r14 {
	enum LBT_Config_r14_Sel d;
	union LBT_Config_r14_Value v;
};

enum PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e {
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e_sf2 = 0,
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e_sf3 = 1,
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e_sf4 = 2,
};

typedef enum PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e;

enum PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e {
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e_sf2 = 0,
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e_sf3 = 1,
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e_sf4 = 2,
};

typedef enum PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e;

enum PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e {
	PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e_true = 0,
};

typedef enum PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e;

enum PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e {
	PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e_true = 0,
};

typedef enum PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e;

enum PDCCH_CandidateReductionValue_r13_e {
	PDCCH_CandidateReductionValue_r13_e_n0 = 0,
	PDCCH_CandidateReductionValue_r13_e_n33 = 1,
	PDCCH_CandidateReductionValue_r13_e_n66 = 2,
	PDCCH_CandidateReductionValue_r13_e_n100 = 3,
};

typedef enum PDCCH_CandidateReductionValue_r13_e PDCCH_CandidateReductionValue_r13_e;

struct PDCCH_CandidateReductions_r13_setup {
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL1_r13;
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL2_r13;
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL3_r13;
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL4_r13;
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL5_r13;
};

enum PDCCH_CandidateReductions_r13_Sel {
	PDCCH_CandidateReductions_r13_UNBOUND_VALUE = 0,
	PDCCH_CandidateReductions_r13_release = 1,
	PDCCH_CandidateReductions_r13_setup = 2,
};

union PDCCH_CandidateReductions_r13_Value {
	Null release;
	struct PDCCH_CandidateReductions_r13_setup setup;
};

struct PDCCH_CandidateReductions_r13 {
	enum PDCCH_CandidateReductions_r13_Sel d;
	union PDCCH_CandidateReductions_r13_Value v;
};

enum PDCCH_CandidateReductionValue_r14_e {
	PDCCH_CandidateReductionValue_r14_e_n0 = 0,
	PDCCH_CandidateReductionValue_r14_e_n50 = 1,
	PDCCH_CandidateReductionValue_r14_e_n100 = 2,
	PDCCH_CandidateReductionValue_r14_e_n150 = 3,
};

typedef enum PDCCH_CandidateReductionValue_r14_e PDCCH_CandidateReductionValue_r14_e;

struct PDCCH_CandidateReductionsLAA_UL_r14_setup {
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL1_r14;
	PDCCH_CandidateReductionValue_r13_e pdcch_candidateReductionAL2_r14;
	PDCCH_CandidateReductionValue_r14_e pdcch_candidateReductionAL3_r14;
	PDCCH_CandidateReductionValue_r14_e pdcch_candidateReductionAL4_r14;
	PDCCH_CandidateReductionValue_r14_e pdcch_candidateReductionAL5_r14;
};

enum PDCCH_CandidateReductionsLAA_UL_r14_Sel {
	PDCCH_CandidateReductionsLAA_UL_r14_UNBOUND_VALUE = 0,
	PDCCH_CandidateReductionsLAA_UL_r14_release = 1,
	PDCCH_CandidateReductionsLAA_UL_r14_setup = 2,
};

union PDCCH_CandidateReductionsLAA_UL_r14_Value {
	Null release;
	struct PDCCH_CandidateReductionsLAA_UL_r14_setup setup;
};

struct PDCCH_CandidateReductionsLAA_UL_r14 {
	enum PDCCH_CandidateReductionsLAA_UL_r14_Sel d;
	union PDCCH_CandidateReductionsLAA_UL_r14_Value v;
};

struct PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e_maxNumberOfSchedSubframes_Format0B_r14_Optional {
	bool d;
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e v;
};

struct PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e_maxNumberOfSchedSubframes_Format4B_r14_Optional {
	bool d;
	PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e v;
};

struct PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e_skipMonitoringDCI_Format0A_r14_Optional {
	bool d;
	PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e v;
};

struct PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e_skipMonitoringDCI_Format4A_r14_Optional {
	bool d;
	PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e v;
};

struct PDCCH_CandidateReductions_r13_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format0A_r14_Optional {
	bool d;
	struct PDCCH_CandidateReductions_r13 v;
};

struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format4A_r14_Optional {
	bool d;
	struct PDCCH_CandidateReductionsLAA_UL_r14 v;
};

struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format0B_r14_Optional {
	bool d;
	struct PDCCH_CandidateReductionsLAA_UL_r14 v;
};

struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format4B_r14_Optional {
	bool d;
	struct PDCCH_CandidateReductionsLAA_UL_r14 v;
};

struct PDCCH_ConfigLAA_r14 {
	struct PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format0B_r14_e_maxNumberOfSchedSubframes_Format0B_r14_Optional maxNumberOfSchedSubframes_Format0B_r14;
	struct PDCCH_ConfigLAA_r14_maxNumberOfSchedSubframes_Format4B_r14_e_maxNumberOfSchedSubframes_Format4B_r14_Optional maxNumberOfSchedSubframes_Format4B_r14;
	struct PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format0A_r14_e_skipMonitoringDCI_Format0A_r14_Optional skipMonitoringDCI_Format0A_r14;
	struct PDCCH_ConfigLAA_r14_skipMonitoringDCI_Format4A_r14_e_skipMonitoringDCI_Format4A_r14_Optional skipMonitoringDCI_Format4A_r14;
	struct PDCCH_CandidateReductions_r13_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format0A_r14_Optional pdcch_CandidateReductions_Format0A_r14;
	struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format4A_r14_Optional pdcch_CandidateReductions_Format4A_r14;
	struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format0B_r14_Optional pdcch_CandidateReductions_Format0B_r14;
	struct PDCCH_CandidateReductionsLAA_UL_r14_PDCCH_ConfigLAA_r14_pdcch_CandidateReductions_Format4B_r14_Optional pdcch_CandidateReductions_Format4B_r14;
};

enum LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e {
	LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e_true = 0,
};

typedef enum LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e;

struct uint8_t_SoundingRS_UL_ConfigDedicatedAperiodic_v1430_setup_srs_SubframeIndication_r14_Optional {
	bool d;
	uint8_t v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430_setup {
	struct uint8_t_SoundingRS_UL_ConfigDedicatedAperiodic_v1430_setup_srs_SubframeIndication_r14_Optional srs_SubframeIndication_r14;
};

enum SoundingRS_UL_ConfigDedicatedAperiodic_v1430_Sel {
	SoundingRS_UL_ConfigDedicatedAperiodic_v1430_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigDedicatedAperiodic_v1430_release = 1,
	SoundingRS_UL_ConfigDedicatedAperiodic_v1430_setup = 2,
};

union SoundingRS_UL_ConfigDedicatedAperiodic_v1430_Value {
	Null release;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430_setup setup;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430 {
	enum SoundingRS_UL_ConfigDedicatedAperiodic_v1430_Sel d;
	union SoundingRS_UL_ConfigDedicatedAperiodic_v1430_Value v;
};

struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_crossCarrierSchedulingConfig_UL_r14_Optional {
	bool d;
	struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14 v;
};

struct LBT_Config_r14_LAA_SCellConfiguration_v1430_lbt_Config_r14_Optional {
	bool d;
	struct LBT_Config_r14 v;
};

struct PDCCH_ConfigLAA_r14_LAA_SCellConfiguration_v1430_pdcch_ConfigLAA_r14_Optional {
	bool d;
	struct PDCCH_ConfigLAA_r14 v;
};

struct LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e_absenceOfAnyOtherTechnology_r14_Optional {
	bool d;
	LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430_LAA_SCellConfiguration_v1430_soundingRS_UL_ConfigDedicatedAperiodic_v1430_Optional {
	bool d;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430 v;
};

struct LAA_SCellConfiguration_v1430 {
	struct LAA_SCellConfiguration_v1430_crossCarrierSchedulingConfig_UL_r14_crossCarrierSchedulingConfig_UL_r14_Optional crossCarrierSchedulingConfig_UL_r14;
	struct LBT_Config_r14_LAA_SCellConfiguration_v1430_lbt_Config_r14_Optional lbt_Config_r14;
	struct PDCCH_ConfigLAA_r14_LAA_SCellConfiguration_v1430_pdcch_ConfigLAA_r14_Optional pdcch_ConfigLAA_r14;
	struct LAA_SCellConfiguration_v1430_absenceOfAnyOtherTechnology_r14_e_absenceOfAnyOtherTechnology_r14_Optional absenceOfAnyOtherTechnology_r14;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_v1430_LAA_SCellConfiguration_v1430_soundingRS_UL_ConfigDedicatedAperiodic_v1430_Optional soundingRS_UL_ConfigDedicatedAperiodic_v1430;
};

enum LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e {
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps0 = 0,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps8 = 1,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps16 = 2,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps32 = 3,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps64 = 4,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps128 = 5,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps256 = 6,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_infinity = 7,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps512_v1020 = 8,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps1024_v1020 = 9,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_kBps2048_v1020 = 10,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_spare5 = 11,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_spare4 = 12,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_spare3 = 13,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_spare2 = 14,
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e_spare1 = 15,
};

typedef enum LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e;

enum LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e {
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms50 = 0,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms100 = 1,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms150 = 2,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms300 = 3,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms500 = 4,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_ms1000 = 5,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_spare2 = 6,
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e_spare1 = 7,
};

typedef enum LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e;

struct uint8_t_LogicalChannelConfig_ul_SpecificParameters_logicalChannelGroup_Optional {
	bool d;
	uint8_t v;
};

struct LogicalChannelConfig_ul_SpecificParameters {
	uint8_t priority;
	LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e prioritisedBitRate;
	LogicalChannelConfig_ul_SpecificParameters_bucketSizeDuration_e bucketSizeDuration;
	struct uint8_t_LogicalChannelConfig_ul_SpecificParameters_logicalChannelGroup_Optional logicalChannelGroup;
};

enum LogicalChannelConfig_logicalChannelSR_Mask_r9_e {
	LogicalChannelConfig_logicalChannelSR_Mask_r9_e_setup = 0,
};

typedef enum LogicalChannelConfig_logicalChannelSR_Mask_r9_e LogicalChannelConfig_logicalChannelSR_Mask_r9_e;

enum LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e {
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s0 = 0,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s0dot4 = 1,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s0dot8 = 2,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s1dot6 = 3,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s3 = 4,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s6 = 5,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s12 = 6,
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_s30 = 7,
};

typedef enum LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e;

struct LogicalChannelConfig_allowedTTI_Lengths_r15_setup {
	bool shortTTI_r15;
	bool subframeTTI_r15;
};

enum LogicalChannelConfig_allowedTTI_Lengths_r15_Sel {
	LogicalChannelConfig_allowedTTI_Lengths_r15_UNBOUND_VALUE = 0,
	LogicalChannelConfig_allowedTTI_Lengths_r15_release = 1,
	LogicalChannelConfig_allowedTTI_Lengths_r15_setup = 2,
};

union LogicalChannelConfig_allowedTTI_Lengths_r15_Value {
	Null release;
	struct LogicalChannelConfig_allowedTTI_Lengths_r15_setup setup;
};

struct LogicalChannelConfig_allowedTTI_Lengths_r15 {
	enum LogicalChannelConfig_allowedTTI_Lengths_r15_Sel d;
	union LogicalChannelConfig_allowedTTI_Lengths_r15_Value v;
};

enum LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e {
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e_spucch = 0,
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e_pucch = 1,
};

typedef enum LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e;

enum LogicalChannelConfig_logicalChannelSR_Restriction_r15_Sel {
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_UNBOUND_VALUE = 0,
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_release = 1,
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup = 2,
};

union LogicalChannelConfig_logicalChannelSR_Restriction_r15_Value {
	Null release;
	LogicalChannelConfig_logicalChannelSR_Restriction_r15_setup_e setup;
};

struct LogicalChannelConfig_logicalChannelSR_Restriction_r15 {
	enum LogicalChannelConfig_logicalChannelSR_Restriction_r15_Sel d;
	union LogicalChannelConfig_logicalChannelSR_Restriction_r15_Value v;
};

enum LogicalChannelConfig_channelAccessPriority_r15_Sel {
	LogicalChannelConfig_channelAccessPriority_r15_UNBOUND_VALUE = 0,
	LogicalChannelConfig_channelAccessPriority_r15_release = 1,
	LogicalChannelConfig_channelAccessPriority_r15_setup = 2,
};

union LogicalChannelConfig_channelAccessPriority_r15_Value {
	Null release;
	uint8_t setup;
};

struct LogicalChannelConfig_channelAccessPriority_r15 {
	enum LogicalChannelConfig_channelAccessPriority_r15_Sel d;
	union LogicalChannelConfig_channelAccessPriority_r15_Value v;
};

enum LogicalChannelConfig_bitRateMultiplier_r16_e {
	LogicalChannelConfig_bitRateMultiplier_r16_e_x40 = 0,
	LogicalChannelConfig_bitRateMultiplier_r16_e_x70 = 1,
	LogicalChannelConfig_bitRateMultiplier_r16_e_x100 = 2,
	LogicalChannelConfig_bitRateMultiplier_r16_e_x200 = 3,
};

typedef enum LogicalChannelConfig_bitRateMultiplier_r16_e LogicalChannelConfig_bitRateMultiplier_r16_e;

struct LogicalChannelConfig_ul_SpecificParameters_ul_SpecificParameters_Optional {
	bool d;
	struct LogicalChannelConfig_ul_SpecificParameters v;
};

struct LogicalChannelConfig_logicalChannelSR_Mask_r9_e_logicalChannelSR_Mask_r9_Optional {
	bool d;
	LogicalChannelConfig_logicalChannelSR_Mask_r9_e v;
};

struct bool_LogicalChannelConfig_logicalChannelSR_Prohibit_r12_Optional {
	bool d;
	bool v;
};

struct bool_LogicalChannelConfig_laa_UL_Allowed_r14_Optional {
	bool d;
	bool v;
};

struct LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_bitRateQueryProhibitTimer_r14_Optional {
	bool d;
	LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e v;
};

struct LogicalChannelConfig_allowedTTI_Lengths_r15_allowedTTI_Lengths_r15_Optional {
	bool d;
	struct LogicalChannelConfig_allowedTTI_Lengths_r15 v;
};

struct LogicalChannelConfig_logicalChannelSR_Restriction_r15_logicalChannelSR_Restriction_r15_Optional {
	bool d;
	struct LogicalChannelConfig_logicalChannelSR_Restriction_r15 v;
};

struct LogicalChannelConfig_channelAccessPriority_r15_channelAccessPriority_r15_Optional {
	bool d;
	struct LogicalChannelConfig_channelAccessPriority_r15 v;
};

struct B32_LogicalChannelConfig_lch_CellRestriction_r15_Optional {
	bool d;
	B32 v;
};

struct LogicalChannelConfig_bitRateMultiplier_r16_e_bitRateMultiplier_r16_Optional {
	bool d;
	LogicalChannelConfig_bitRateMultiplier_r16_e v;
};

struct LogicalChannelConfig {
	struct LogicalChannelConfig_ul_SpecificParameters_ul_SpecificParameters_Optional ul_SpecificParameters;
	struct LogicalChannelConfig_logicalChannelSR_Mask_r9_e_logicalChannelSR_Mask_r9_Optional logicalChannelSR_Mask_r9;
	struct bool_LogicalChannelConfig_logicalChannelSR_Prohibit_r12_Optional logicalChannelSR_Prohibit_r12;
	struct bool_LogicalChannelConfig_laa_UL_Allowed_r14_Optional laa_UL_Allowed_r14;
	struct LogicalChannelConfig_bitRateQueryProhibitTimer_r14_e_bitRateQueryProhibitTimer_r14_Optional bitRateQueryProhibitTimer_r14;
	struct LogicalChannelConfig_allowedTTI_Lengths_r15_allowedTTI_Lengths_r15_Optional allowedTTI_Lengths_r15;
	struct LogicalChannelConfig_logicalChannelSR_Restriction_r15_logicalChannelSR_Restriction_r15_Optional logicalChannelSR_Restriction_r15;
	struct LogicalChannelConfig_channelAccessPriority_r15_channelAccessPriority_r15_Optional channelAccessPriority_r15;
	struct B32_LogicalChannelConfig_lch_CellRestriction_r15_Optional lch_CellRestriction_r15;
	struct LogicalChannelConfig_bitRateMultiplier_r16_e_bitRateMultiplier_r16_Optional bitRateMultiplier_r16;
};

enum DRX_Config_r13_onDurationTimer_v1310_e {
	DRX_Config_r13_onDurationTimer_v1310_e_psf300 = 0,
	DRX_Config_r13_onDurationTimer_v1310_e_psf400 = 1,
	DRX_Config_r13_onDurationTimer_v1310_e_psf500 = 2,
	DRX_Config_r13_onDurationTimer_v1310_e_psf600 = 3,
	DRX_Config_r13_onDurationTimer_v1310_e_psf800 = 4,
	DRX_Config_r13_onDurationTimer_v1310_e_psf1000 = 5,
	DRX_Config_r13_onDurationTimer_v1310_e_psf1200 = 6,
	DRX_Config_r13_onDurationTimer_v1310_e_psf1600 = 7,
};

typedef enum DRX_Config_r13_onDurationTimer_v1310_e DRX_Config_r13_onDurationTimer_v1310_e;

enum DRX_Config_r13_drx_RetransmissionTimer_v1310_e {
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf40 = 0,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf64 = 1,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf80 = 2,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf96 = 3,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf112 = 4,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf128 = 5,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf160 = 6,
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e_psf320 = 7,
};

typedef enum DRX_Config_r13_drx_RetransmissionTimer_v1310_e DRX_Config_r13_drx_RetransmissionTimer_v1310_e;

enum DRX_Config_r13_drx_ULRetransmissionTimer_r13_e {
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf0 = 0,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf1 = 1,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf2 = 2,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf4 = 3,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf6 = 4,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf8 = 5,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf16 = 6,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf24 = 7,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf33 = 8,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf40 = 9,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf64 = 10,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf80 = 11,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf96 = 12,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf112 = 13,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf128 = 14,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf160 = 15,
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_psf320 = 16,
};

typedef enum DRX_Config_r13_drx_ULRetransmissionTimer_r13_e DRX_Config_r13_drx_ULRetransmissionTimer_r13_e;

struct DRX_Config_r13_onDurationTimer_v1310_e_onDurationTimer_v1310_Optional {
	bool d;
	DRX_Config_r13_onDurationTimer_v1310_e v;
};

struct DRX_Config_r13_drx_RetransmissionTimer_v1310_e_drx_RetransmissionTimer_v1310_Optional {
	bool d;
	DRX_Config_r13_drx_RetransmissionTimer_v1310_e v;
};

struct DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_drx_ULRetransmissionTimer_r13_Optional {
	bool d;
	DRX_Config_r13_drx_ULRetransmissionTimer_r13_e v;
};

struct DRX_Config_r13 {
	struct DRX_Config_r13_onDurationTimer_v1310_e_onDurationTimer_v1310_Optional onDurationTimer_v1310;
	struct DRX_Config_r13_drx_RetransmissionTimer_v1310_e_drx_RetransmissionTimer_v1310_Optional drx_RetransmissionTimer_v1310;
	struct DRX_Config_r13_drx_ULRetransmissionTimer_r13_e_drx_ULRetransmissionTimer_r13_Optional drx_ULRetransmissionTimer_r13;
};

enum MAC_MainConfig_drx_Config_r13_Sel {
	MAC_MainConfig_drx_Config_r13_UNBOUND_VALUE = 0,
	MAC_MainConfig_drx_Config_r13_release = 1,
	MAC_MainConfig_drx_Config_r13_setup = 2,
};

union MAC_MainConfig_drx_Config_r13_Value {
	Null release;
	struct DRX_Config_r13 setup;
};

struct MAC_MainConfig_drx_Config_r13 {
	enum MAC_MainConfig_drx_Config_r13_Sel d;
	union MAC_MainConfig_drx_Config_r13_Value v;
};

typedef bool MAC_MainConfig_e_HARQ_Pattern_r12;

enum MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_Sel {
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_UNBOUND_VALUE = 0,
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_sf5120 = 1,
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_sf10240 = 2,
};

union MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_Value {
	uint8_t sf5120;
	uint8_t sf10240;
};

struct MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup {
	enum MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_Sel d;
	union MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup_Value v;
};

enum MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_Sel {
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_UNBOUND_VALUE = 0,
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_release = 1,
	MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup = 2,
};

union MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_Value {
	Null release;
	struct MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_setup setup;
};

struct MAC_MainConfig_eDRX_Config_CycleStartOffset_r13 {
	enum MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_Sel d;
	union MAC_MainConfig_eDRX_Config_CycleStartOffset_r13_Value v;
};

enum MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e {
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf2 = 0,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf4 = 1,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf8 = 2,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf16 = 3,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf32 = 4,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf64 = 5,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_rf128 = 6,
	MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e_spare = 7,
};

typedef enum MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e MAC_MainConfig_mac_MainConfig_v1020_sCellDeactivationTimer_r10_e;

typedef bool MAC_MainConfig_ul_SCH_Config_ttiBundling;

typedef uint8_t MBMS_SessionInfo_r9_logicalChannelIdentity_r9;

enum TMGI_r9_plmn_Id_r9_Sel {
	TMGI_r9_plmn_Id_r9_UNBOUND_VALUE = 0,
	TMGI_r9_plmn_Id_r9_plmn_Index_r9 = 1,
	TMGI_r9_plmn_Id_r9_explicitValue_r9 = 2,
};

union TMGI_r9_plmn_Id_r9_Value {
	uint8_t plmn_Index_r9;
	struct PLMN_Identity explicitValue_r9;
};

struct TMGI_r9_plmn_Id_r9 {
	enum TMGI_r9_plmn_Id_r9_Sel d;
	union TMGI_r9_plmn_Id_r9_Value v;
};

struct TMGI_r9 {
	struct TMGI_r9_plmn_Id_r9 plmn_Id_r9;
	O3 serviceId_r9;
};

struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional {
	bool d;
	O1 v;
};

struct MBMSSessionInfo_r13 {
	struct TMGI_r9 tmgi_r13;
	struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional sessionId_r13;
};

typedef uint8_t MasterInformationBlock_schedulingInfoSIB1_BR_r13;

enum MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_Sel {
	MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_UNBOUND_VALUE = 0,
	MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_ms40_r12 = 1,
	MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_ms80_r12 = 2,
	MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_ms160_r12 = 3,
};

union MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_Value {
	uint8_t ms40_r12;
	uint8_t ms80_r12;
	uint8_t ms160_r12;
};

struct MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12 {
	enum MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_Sel d;
	union MeasDS_Config_r12_setup_dmtc_PeriodOffset_r12_Value v;
};

enum MeasDS_Config_r12_setup_ds_OccasionDuration_r12_Sel {
	MeasDS_Config_r12_setup_ds_OccasionDuration_r12_UNBOUND_VALUE = 0,
	MeasDS_Config_r12_setup_ds_OccasionDuration_r12_durationFDD_r12 = 1,
	MeasDS_Config_r12_setup_ds_OccasionDuration_r12_durationTDD_r12 = 2,
};

union MeasDS_Config_r12_setup_ds_OccasionDuration_r12_Value {
	uint8_t durationFDD_r12;
	uint8_t durationTDD_r12;
};

struct MeasDS_Config_r12_setup_ds_OccasionDuration_r12 {
	enum MeasDS_Config_r12_setup_ds_OccasionDuration_r12_Sel d;
	union MeasDS_Config_r12_setup_ds_OccasionDuration_r12_Value v;
};

enum MeasGapConfig_setup_gapOffset_Sel {
	MeasGapConfig_setup_gapOffset_UNBOUND_VALUE = 0,
	MeasGapConfig_setup_gapOffset_gp0 = 1,
	MeasGapConfig_setup_gapOffset_gp1 = 2,
	MeasGapConfig_setup_gapOffset_gp2_r14 = 3,
	MeasGapConfig_setup_gapOffset_gp3_r14 = 4,
	MeasGapConfig_setup_gapOffset_gp_ncsg0_r14 = 5,
	MeasGapConfig_setup_gapOffset_gp_ncsg1_r14 = 6,
	MeasGapConfig_setup_gapOffset_gp_ncsg2_r14 = 7,
	MeasGapConfig_setup_gapOffset_gp_ncsg3_r14 = 8,
	MeasGapConfig_setup_gapOffset_gp_nonUniform1_r14 = 9,
	MeasGapConfig_setup_gapOffset_gp_nonUniform2_r14 = 10,
	MeasGapConfig_setup_gapOffset_gp_nonUniform3_r14 = 11,
	MeasGapConfig_setup_gapOffset_gp_nonUniform4_r14 = 12,
	MeasGapConfig_setup_gapOffset_gp4_r15 = 13,
	MeasGapConfig_setup_gapOffset_gp5_r15 = 14,
	MeasGapConfig_setup_gapOffset_gp6_r15 = 15,
	MeasGapConfig_setup_gapOffset_gp7_r15 = 16,
	MeasGapConfig_setup_gapOffset_gp8_r15 = 17,
	MeasGapConfig_setup_gapOffset_gp9_r15 = 18,
	MeasGapConfig_setup_gapOffset_gp10_r15 = 19,
	MeasGapConfig_setup_gapOffset_gp11_r15 = 20,
};

union MeasGapConfig_setup_gapOffset_Value {
	uint8_t gp0;
	uint8_t gp1;
	uint8_t gp2_r14;
	uint8_t gp3_r14;
	uint8_t gp_ncsg0_r14;
	uint8_t gp_ncsg1_r14;
	uint8_t gp_ncsg2_r14;
	uint8_t gp_ncsg3_r14;
	uint16_t gp_nonUniform1_r14;
	uint16_t gp_nonUniform2_r14;
	uint16_t gp_nonUniform3_r14;
	uint16_t gp_nonUniform4_r14;
	uint8_t gp4_r15;
	uint8_t gp5_r15;
	uint8_t gp6_r15;
	uint8_t gp7_r15;
	uint8_t gp8_r15;
	uint8_t gp9_r15;
	uint8_t gp10_r15;
	uint8_t gp11_r15;
};

struct MeasGapConfig_setup_gapOffset {
	enum MeasGapConfig_setup_gapOffset_Sel d;
	union MeasGapConfig_setup_gapOffset_Value v;
};

struct MeasGapConfig_setup {
	struct MeasGapConfig_setup_gapOffset gapOffset;
};

enum MeasGapConfig_Sel {
	MeasGapConfig_UNBOUND_VALUE = 0,
	MeasGapConfig_release = 1,
	MeasGapConfig_setup = 2,
};

union MeasGapConfig_Value {
	Null release;
	struct MeasGapConfig_setup setup;
};

struct MeasGapConfig {
	enum MeasGapConfig_Sel d;
	union MeasGapConfig_Value v;
};

enum MeasSubframePatternPCell_r10_Sel {
	MeasSubframePatternPCell_r10_UNBOUND_VALUE = 0,
	MeasSubframePatternPCell_r10_release = 1,
	MeasSubframePatternPCell_r10_setup = 2,
};

union MeasSubframePatternPCell_r10_Value {
	Null release;
	struct MeasSubframePattern_r10 setup;
};

struct MeasSubframePatternPCell_r10 {
	enum MeasSubframePatternPCell_r10_Sel d;
	union MeasSubframePatternPCell_r10_Value v;
};

typedef B8 MMEC;

struct S_TMSI {
	MMEC mmec;
	B32 m_TMSI;
};

typedef uint8_t IMSI_Digit;

struct IMSI_Digit_IMSI_Dynamic {
	size_t d;
	IMSI_Digit* v;
};

typedef struct IMSI_Digit_IMSI_Dynamic IMSI;

typedef B48 NG_5G_S_TMSI_r15;

typedef B40 I_RNTI_r15;

enum PagingUE_Identity_Sel {
	PagingUE_Identity_UNBOUND_VALUE = 0,
	PagingUE_Identity_s_TMSI = 1,
	PagingUE_Identity_imsi = 2,
	PagingUE_Identity_ng_5G_S_TMSI_r15 = 3,
	PagingUE_Identity_fullI_RNTI_r15 = 4,
};

union PagingUE_Identity_Value {
	struct S_TMSI s_TMSI;
	IMSI imsi;
	NG_5G_S_TMSI_r15 ng_5G_S_TMSI_r15;
	I_RNTI_r15 fullI_RNTI_r15;
};

struct PagingUE_Identity {
	enum PagingUE_Identity_Sel d;
	union PagingUE_Identity_Value v;
};

enum PagingRecord_cn_Domain_e {
	PagingRecord_cn_Domain_e_ps = 0,
	PagingRecord_cn_Domain_e_cs = 1,
};

typedef enum PagingRecord_cn_Domain_e PagingRecord_cn_Domain_e;

struct PagingRecord {
	struct PagingUE_Identity ue_Identity;
	PagingRecord_cn_Domain_e cn_Domain;
};

struct PagingRecord_PagingRecordList_Dynamic {
	size_t d;
	struct PagingRecord* v;
};

typedef struct PagingRecord_PagingRecordList_Dynamic PagingRecordList;

enum Paging_systemInfoModification_e {
	Paging_systemInfoModification_e_true = 0,
};

typedef enum Paging_systemInfoModification_e Paging_systemInfoModification_e;

enum Paging_etws_Indication_e {
	Paging_etws_Indication_e_true = 0,
};

typedef enum Paging_etws_Indication_e Paging_etws_Indication_e;

enum Paging_v920_IEs_cmas_Indication_r9_e {
	Paging_v920_IEs_cmas_Indication_r9_e_true = 0,
};

typedef enum Paging_v920_IEs_cmas_Indication_r9_e Paging_v920_IEs_cmas_Indication_r9_e;

enum Paging_v1130_IEs_eab_ParamModification_r11_e {
	Paging_v1130_IEs_eab_ParamModification_r11_e_true = 0,
};

typedef enum Paging_v1130_IEs_eab_ParamModification_r11_e Paging_v1130_IEs_eab_ParamModification_r11_e;

enum Paging_v1310_IEs_redistributionIndication_r13_e {
	Paging_v1310_IEs_redistributionIndication_r13_e_true = 0,
};

typedef enum Paging_v1310_IEs_redistributionIndication_r13_e Paging_v1310_IEs_redistributionIndication_r13_e;

enum Paging_v1310_IEs_systemInfoModification_eDRX_r13_e {
	Paging_v1310_IEs_systemInfoModification_eDRX_r13_e_true = 0,
};

typedef enum Paging_v1310_IEs_systemInfoModification_eDRX_r13_e Paging_v1310_IEs_systemInfoModification_eDRX_r13_e;

enum Paging_v1530_IEs_accessType_e {
	Paging_v1530_IEs_accessType_e_non3GPP = 0,
};

typedef enum Paging_v1530_IEs_accessType_e Paging_v1530_IEs_accessType_e;

enum PagingRecord_v1610_accessType_r16_e {
	PagingRecord_v1610_accessType_r16_e_non3GPP = 0,
};

typedef enum PagingRecord_v1610_accessType_r16_e PagingRecord_v1610_accessType_r16_e;

enum PagingRecord_v1610_mt_EDT_r16_e {
	PagingRecord_v1610_mt_EDT_r16_e_true = 0,
};

typedef enum PagingRecord_v1610_mt_EDT_r16_e PagingRecord_v1610_mt_EDT_r16_e;

struct PagingRecord_v1610_accessType_r16_e_accessType_r16_Optional {
	bool d;
	PagingRecord_v1610_accessType_r16_e v;
};

struct PagingRecord_v1610_mt_EDT_r16_e_mt_EDT_r16_Optional {
	bool d;
	PagingRecord_v1610_mt_EDT_r16_e v;
};

struct PagingRecord_v1610 {
	struct PagingRecord_v1610_accessType_r16_e_accessType_r16_Optional accessType_r16;
	struct PagingRecord_v1610_mt_EDT_r16_e_mt_EDT_r16_Optional mt_EDT_r16;
};

struct PagingRecord_v1610_PagingRecordList_v1610_Dynamic {
	size_t d;
	struct PagingRecord_v1610* v;
};

typedef struct PagingRecord_v1610_PagingRecordList_v1610_Dynamic PagingRecordList_v1610;

enum Paging_v1610_IEs_uac_ParamModification_r16_e {
	Paging_v1610_IEs_uac_ParamModification_r16_e_true = 0,
};

typedef enum Paging_v1610_IEs_uac_ParamModification_r16_e Paging_v1610_IEs_uac_ParamModification_r16_e;

struct Paging_v1610_IEs_nonCriticalExtension {
};

struct PagingRecordList_v1610_Paging_v1610_IEs_pagingRecordList_v1610_Optional {
	bool d;
	PagingRecordList_v1610 v;
};

struct Paging_v1610_IEs_uac_ParamModification_r16_e_uac_ParamModification_r16_Optional {
	bool d;
	Paging_v1610_IEs_uac_ParamModification_r16_e v;
};

struct Paging_v1610_IEs_nonCriticalExtension_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v1610_IEs_nonCriticalExtension v;
};

struct Paging_v1610_IEs {
	struct PagingRecordList_v1610_Paging_v1610_IEs_pagingRecordList_v1610_Optional pagingRecordList_v1610;
	struct Paging_v1610_IEs_uac_ParamModification_r16_e_uac_ParamModification_r16_Optional uac_ParamModification_r16;
	struct Paging_v1610_IEs_nonCriticalExtension_nonCriticalExtension_Optional nonCriticalExtension;
};

struct Paging_v1530_IEs_accessType_e_accessType_Optional {
	bool d;
	Paging_v1530_IEs_accessType_e v;
};

struct Paging_v1610_IEs_Paging_v1530_IEs_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v1610_IEs v;
};

struct Paging_v1530_IEs {
	struct Paging_v1530_IEs_accessType_e_accessType_Optional accessType;
	struct Paging_v1610_IEs_Paging_v1530_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct Paging_v1310_IEs_redistributionIndication_r13_e_redistributionIndication_r13_Optional {
	bool d;
	Paging_v1310_IEs_redistributionIndication_r13_e v;
};

struct Paging_v1310_IEs_systemInfoModification_eDRX_r13_e_systemInfoModification_eDRX_r13_Optional {
	bool d;
	Paging_v1310_IEs_systemInfoModification_eDRX_r13_e v;
};

struct Paging_v1530_IEs_Paging_v1310_IEs_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v1530_IEs v;
};

struct Paging_v1310_IEs {
	struct Paging_v1310_IEs_redistributionIndication_r13_e_redistributionIndication_r13_Optional redistributionIndication_r13;
	struct Paging_v1310_IEs_systemInfoModification_eDRX_r13_e_systemInfoModification_eDRX_r13_Optional systemInfoModification_eDRX_r13;
	struct Paging_v1530_IEs_Paging_v1310_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct Paging_v1130_IEs_eab_ParamModification_r11_e_eab_ParamModification_r11_Optional {
	bool d;
	Paging_v1130_IEs_eab_ParamModification_r11_e v;
};

struct Paging_v1310_IEs_Paging_v1130_IEs_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v1310_IEs v;
};

struct Paging_v1130_IEs {
	struct Paging_v1130_IEs_eab_ParamModification_r11_e_eab_ParamModification_r11_Optional eab_ParamModification_r11;
	struct Paging_v1310_IEs_Paging_v1130_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct Paging_v920_IEs_cmas_Indication_r9_e_cmas_Indication_r9_Optional {
	bool d;
	Paging_v920_IEs_cmas_Indication_r9_e v;
};

struct Paging_v1130_IEs_Paging_v920_IEs_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v1130_IEs v;
};

struct Paging_v920_IEs {
	struct Paging_v920_IEs_cmas_Indication_r9_e_cmas_Indication_r9_Optional cmas_Indication_r9;
	struct Paging_v1130_IEs_Paging_v920_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct OCTET_STRING_Paging_v890_IEs_lateNonCriticalExtension_Optional {
	bool d;
	OCTET_STRING v;
};

struct Paging_v920_IEs_Paging_v890_IEs_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v920_IEs v;
};

struct Paging_v890_IEs {
	struct OCTET_STRING_Paging_v890_IEs_lateNonCriticalExtension_Optional lateNonCriticalExtension;
	struct Paging_v920_IEs_Paging_v890_IEs_nonCriticalExtension_Optional nonCriticalExtension;
};

struct PagingRecordList_Paging_pagingRecordList_Optional {
	bool d;
	PagingRecordList v;
};

struct Paging_systemInfoModification_e_systemInfoModification_Optional {
	bool d;
	Paging_systemInfoModification_e v;
};

struct Paging_etws_Indication_e_etws_Indication_Optional {
	bool d;
	Paging_etws_Indication_e v;
};

struct Paging_v890_IEs_Paging_nonCriticalExtension_Optional {
	bool d;
	struct Paging_v890_IEs v;
};

struct Paging {
	struct PagingRecordList_Paging_pagingRecordList_Optional pagingRecordList;
	struct Paging_systemInfoModification_e_systemInfoModification_Optional systemInfoModification;
	struct Paging_etws_Indication_e_etws_Indication_Optional etws_Indication;
	struct Paging_v890_IEs_Paging_nonCriticalExtension_Optional nonCriticalExtension;
};

enum PCCH_MessageType_c1_Sel {
	PCCH_MessageType_c1_UNBOUND_VALUE = 0,
	PCCH_MessageType_c1_paging = 1,
};

union PCCH_MessageType_c1_Value {
	struct Paging paging;
};

struct PCCH_MessageType_c1 {
	enum PCCH_MessageType_c1_Sel d;
	union PCCH_MessageType_c1_Value v;
};

struct PCCH_MessageType_messageClassExtension {
};

enum PCCH_MessageType_Sel {
	PCCH_MessageType_UNBOUND_VALUE = 0,
	PCCH_MessageType_c1 = 1,
	PCCH_MessageType_messageClassExtension = 2,
};

union PCCH_MessageType_Value {
	struct PCCH_MessageType_c1 c1;
	struct PCCH_MessageType_messageClassExtension messageClassExtension;
};

struct PCCH_MessageType {
	enum PCCH_MessageType_Sel d;
	union PCCH_MessageType_Value v;
};

struct PCCH_Message {
	struct PCCH_MessageType message;
};

enum PDCP_Config_discardTimer_e {
	PDCP_Config_discardTimer_e_ms50 = 0,
	PDCP_Config_discardTimer_e_ms100 = 1,
	PDCP_Config_discardTimer_e_ms150 = 2,
	PDCP_Config_discardTimer_e_ms300 = 3,
	PDCP_Config_discardTimer_e_ms500 = 4,
	PDCP_Config_discardTimer_e_ms750 = 5,
	PDCP_Config_discardTimer_e_ms1500 = 6,
	PDCP_Config_discardTimer_e_infinity = 7,
};

typedef enum PDCP_Config_discardTimer_e PDCP_Config_discardTimer_e;

struct PDCP_Config_rlc_AM {
	bool statusReportRequired;
};

enum PDCP_Config_rlc_UM_pdcp_SN_Size_e {
	PDCP_Config_rlc_UM_pdcp_SN_Size_e_len7bits = 0,
	PDCP_Config_rlc_UM_pdcp_SN_Size_e_len12bits = 1,
};

typedef enum PDCP_Config_rlc_UM_pdcp_SN_Size_e PDCP_Config_rlc_UM_pdcp_SN_Size_e;

struct PDCP_Config_rlc_UM {
	PDCP_Config_rlc_UM_pdcp_SN_Size_e pdcp_SN_Size;
};

struct PDCP_Config_headerCompression_rohc_profiles {
	bool profile0x0001;
	bool profile0x0002;
	bool profile0x0003;
	bool profile0x0004;
	bool profile0x0006;
	bool profile0x0101;
	bool profile0x0102;
	bool profile0x0103;
	bool profile0x0104;
};

struct uint16_t_PDCP_Config_headerCompression_rohc_maxCID_Optional {
	bool d;
	uint16_t v;
};

struct PDCP_Config_headerCompression_rohc {
	struct uint16_t_PDCP_Config_headerCompression_rohc_maxCID_Optional maxCID;
	struct PDCP_Config_headerCompression_rohc_profiles profiles;
};

enum PDCP_Config_headerCompression_Sel {
	PDCP_Config_headerCompression_UNBOUND_VALUE = 0,
	PDCP_Config_headerCompression_notUsed = 1,
	PDCP_Config_headerCompression_rohc = 2,
};

union PDCP_Config_headerCompression_Value {
	Null notUsed;
	struct PDCP_Config_headerCompression_rohc rohc;
};

struct PDCP_Config_headerCompression {
	enum PDCP_Config_headerCompression_Sel d;
	union PDCP_Config_headerCompression_Value v;
};

enum PDCP_Config_rn_IntegrityProtection_r10_e {
	PDCP_Config_rn_IntegrityProtection_r10_e_enabled = 0,
};

typedef enum PDCP_Config_rn_IntegrityProtection_r10_e PDCP_Config_rn_IntegrityProtection_r10_e;

enum PDCP_Config_pdcp_SN_Size_v1130_e {
	PDCP_Config_pdcp_SN_Size_v1130_e_len15bits = 0,
};

typedef enum PDCP_Config_pdcp_SN_Size_v1130_e PDCP_Config_pdcp_SN_Size_v1130_e;

enum PDCP_Config_t_Reordering_r12_e {
	PDCP_Config_t_Reordering_r12_e_ms0 = 0,
	PDCP_Config_t_Reordering_r12_e_ms20 = 1,
	PDCP_Config_t_Reordering_r12_e_ms40 = 2,
	PDCP_Config_t_Reordering_r12_e_ms60 = 3,
	PDCP_Config_t_Reordering_r12_e_ms80 = 4,
	PDCP_Config_t_Reordering_r12_e_ms100 = 5,
	PDCP_Config_t_Reordering_r12_e_ms120 = 6,
	PDCP_Config_t_Reordering_r12_e_ms140 = 7,
	PDCP_Config_t_Reordering_r12_e_ms160 = 8,
	PDCP_Config_t_Reordering_r12_e_ms180 = 9,
	PDCP_Config_t_Reordering_r12_e_ms200 = 10,
	PDCP_Config_t_Reordering_r12_e_ms220 = 11,
	PDCP_Config_t_Reordering_r12_e_ms240 = 12,
	PDCP_Config_t_Reordering_r12_e_ms260 = 13,
	PDCP_Config_t_Reordering_r12_e_ms280 = 14,
	PDCP_Config_t_Reordering_r12_e_ms300 = 15,
	PDCP_Config_t_Reordering_r12_e_ms500 = 16,
	PDCP_Config_t_Reordering_r12_e_ms750 = 17,
	PDCP_Config_t_Reordering_r12_e_spare14 = 18,
	PDCP_Config_t_Reordering_r12_e_spare13 = 19,
	PDCP_Config_t_Reordering_r12_e_spare12 = 20,
	PDCP_Config_t_Reordering_r12_e_spare11 = 21,
	PDCP_Config_t_Reordering_r12_e_spare10 = 22,
	PDCP_Config_t_Reordering_r12_e_spare9 = 23,
	PDCP_Config_t_Reordering_r12_e_spare8 = 24,
	PDCP_Config_t_Reordering_r12_e_spare7 = 25,
	PDCP_Config_t_Reordering_r12_e_spare6 = 26,
	PDCP_Config_t_Reordering_r12_e_spare5 = 27,
	PDCP_Config_t_Reordering_r12_e_spare4 = 28,
	PDCP_Config_t_Reordering_r12_e_spare3 = 29,
	PDCP_Config_t_Reordering_r12_e_spare2 = 30,
	PDCP_Config_t_Reordering_r12_e_spare1 = 31,
};

typedef enum PDCP_Config_t_Reordering_r12_e PDCP_Config_t_Reordering_r12_e;

enum PDCP_Config_ul_DataSplitThreshold_r13_setup_e {
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b0 = 0,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b100 = 1,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b200 = 2,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b400 = 3,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b800 = 4,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b1600 = 5,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b3200 = 6,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b6400 = 7,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b12800 = 8,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b25600 = 9,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b51200 = 10,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b102400 = 11,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b204800 = 12,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b409600 = 13,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_b819200 = 14,
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e_spare1 = 15,
};

typedef enum PDCP_Config_ul_DataSplitThreshold_r13_setup_e PDCP_Config_ul_DataSplitThreshold_r13_setup_e;

enum PDCP_Config_ul_DataSplitThreshold_r13_Sel {
	PDCP_Config_ul_DataSplitThreshold_r13_UNBOUND_VALUE = 0,
	PDCP_Config_ul_DataSplitThreshold_r13_release = 1,
	PDCP_Config_ul_DataSplitThreshold_r13_setup = 2,
};

union PDCP_Config_ul_DataSplitThreshold_r13_Value {
	Null release;
	PDCP_Config_ul_DataSplitThreshold_r13_setup_e setup;
};

struct PDCP_Config_ul_DataSplitThreshold_r13 {
	enum PDCP_Config_ul_DataSplitThreshold_r13_Sel d;
	union PDCP_Config_ul_DataSplitThreshold_r13_Value v;
};

enum PDCP_Config_pdcp_SN_Size_v1310_e {
	PDCP_Config_pdcp_SN_Size_v1310_e_len18bits = 0,
};

typedef enum PDCP_Config_pdcp_SN_Size_v1310_e PDCP_Config_pdcp_SN_Size_v1310_e;

enum PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e {
	PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e_type1 = 0,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e_type2 = 1,
};

typedef enum PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e;

enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e {
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms5 = 0,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms10 = 1,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms20 = 2,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms30 = 3,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms40 = 4,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms50 = 5,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms60 = 6,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms70 = 7,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms80 = 8,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms90 = 9,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms100 = 10,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms150 = 11,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms200 = 12,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms300 = 13,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms500 = 14,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms1000 = 15,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms2000 = 16,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms5000 = 17,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms10000 = 18,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms20000 = 19,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_ms50000 = 20,
};

typedef enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e;

enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e {
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms5 = 0,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms10 = 1,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms20 = 2,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms30 = 3,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms40 = 4,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms50 = 5,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms60 = 6,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms70 = 7,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms80 = 8,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms90 = 9,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms100 = 10,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms150 = 11,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms200 = 12,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms300 = 13,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms500 = 14,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms1000 = 15,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms2000 = 16,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms5000 = 17,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms10000 = 18,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms20000 = 19,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_ms50000 = 20,
};

typedef enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e;

enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e {
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms1 = 0,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms2 = 1,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms5 = 2,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms10 = 3,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms25 = 4,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms50 = 5,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms100 = 6,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms250 = 7,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms500 = 8,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms2500 = 9,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms5000 = 10,
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_ms25000 = 11,
};

typedef enum PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e;

struct PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e_statusPDU_TypeForPolling_r13_Optional {
	bool d;
	PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e v;
};

struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_statusPDU_Periodicity_Type1_r13_Optional {
	bool d;
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e v;
};

struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_statusPDU_Periodicity_Type2_r13_Optional {
	bool d;
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e v;
};

struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_statusPDU_Periodicity_Offset_r13_Optional {
	bool d;
	PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e v;
};

struct PDCP_Config_statusFeedback_r13_setup {
	struct PDCP_Config_statusFeedback_r13_setup_statusPDU_TypeForPolling_r13_e_statusPDU_TypeForPolling_r13_Optional statusPDU_TypeForPolling_r13;
	struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type1_r13_e_statusPDU_Periodicity_Type1_r13_Optional statusPDU_Periodicity_Type1_r13;
	struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Type2_r13_e_statusPDU_Periodicity_Type2_r13_Optional statusPDU_Periodicity_Type2_r13;
	struct PDCP_Config_statusFeedback_r13_setup_statusPDU_Periodicity_Offset_r13_e_statusPDU_Periodicity_Offset_r13_Optional statusPDU_Periodicity_Offset_r13;
};

enum PDCP_Config_statusFeedback_r13_Sel {
	PDCP_Config_statusFeedback_r13_UNBOUND_VALUE = 0,
	PDCP_Config_statusFeedback_r13_release = 1,
	PDCP_Config_statusFeedback_r13_setup = 2,
};

union PDCP_Config_statusFeedback_r13_Value {
	Null release;
	struct PDCP_Config_statusFeedback_r13_setup setup;
};

struct PDCP_Config_statusFeedback_r13 {
	enum PDCP_Config_statusFeedback_r13_Sel d;
	union PDCP_Config_statusFeedback_r13_Value v;
};

enum PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e {
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b0 = 0,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b100 = 1,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b200 = 2,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b400 = 3,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b800 = 4,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b1600 = 5,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b3200 = 6,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b6400 = 7,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b12800 = 8,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b25600 = 9,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b51200 = 10,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b102400 = 11,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b204800 = 12,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b409600 = 13,
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_b819200 = 14,
};

typedef enum PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e;

struct PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_ul_LWA_DataSplitThreshold_r14_Optional {
	bool d;
	PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e v;
};

struct PDCP_Config_ul_LWA_Config_r14_setup {
	bool ul_LWA_DRB_ViaWLAN_r14;
	struct PDCP_Config_ul_LWA_Config_r14_setup_ul_LWA_DataSplitThreshold_r14_e_ul_LWA_DataSplitThreshold_r14_Optional ul_LWA_DataSplitThreshold_r14;
};

enum PDCP_Config_ul_LWA_Config_r14_Sel {
	PDCP_Config_ul_LWA_Config_r14_UNBOUND_VALUE = 0,
	PDCP_Config_ul_LWA_Config_r14_release = 1,
	PDCP_Config_ul_LWA_Config_r14_setup = 2,
};

union PDCP_Config_ul_LWA_Config_r14_Value {
	Null release;
	struct PDCP_Config_ul_LWA_Config_r14_setup setup;
};

struct PDCP_Config_ul_LWA_Config_r14 {
	enum PDCP_Config_ul_LWA_Config_r14_Sel d;
	union PDCP_Config_ul_LWA_Config_r14_Value v;
};

struct PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14_profiles_r14 {
	bool profile0x0006_r14;
};

struct uint16_t_PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14_maxCID_r14_Optional {
	bool d;
	uint16_t v;
};

struct PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14 {
	struct uint16_t_PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14_maxCID_r14_Optional maxCID_r14;
	struct PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14_profiles_r14 profiles_r14;
};

enum PDCP_Config_uplinkOnlyHeaderCompression_r14_Sel {
	PDCP_Config_uplinkOnlyHeaderCompression_r14_UNBOUND_VALUE = 0,
	PDCP_Config_uplinkOnlyHeaderCompression_r14_notUsed_r14 = 1,
	PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14 = 2,
};

union PDCP_Config_uplinkOnlyHeaderCompression_r14_Value {
	Null notUsed_r14;
	struct PDCP_Config_uplinkOnlyHeaderCompression_r14_rohc_r14 rohc_r14;
};

struct PDCP_Config_uplinkOnlyHeaderCompression_r14 {
	enum PDCP_Config_uplinkOnlyHeaderCompression_r14_Sel d;
	union PDCP_Config_uplinkOnlyHeaderCompression_r14_Value v;
};

enum PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e {
	PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e_kbyte2 = 0,
	PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e_kbyte4 = 1,
	PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e_kbyte8 = 2,
	PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e_spare1 = 3,
};

typedef enum PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e;

enum PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e {
	PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e_sip_SDP = 0,
	PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e_operator = 1,
};

typedef enum PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e;

struct PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e_dictionary_r15_Optional {
	bool d;
	PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e v;
};

struct PDCP_Config_uplinkDataCompression_r15 {
	PDCP_Config_uplinkDataCompression_r15_bufferSize_r15_e bufferSize_r15;
	struct PDCP_Config_uplinkDataCompression_r15_dictionary_r15_e_dictionary_r15_Optional dictionary_r15;
};

enum PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e {
	PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e_configured = 0,
	PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e_activated = 1,
};

typedef enum PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e;

struct PDCP_Config_pdcp_DuplicationConfig_r15_setup {
	PDCP_Config_pdcp_DuplicationConfig_r15_setup_pdcp_Duplication_r15_e pdcp_Duplication_r15;
};

enum PDCP_Config_pdcp_DuplicationConfig_r15_Sel {
	PDCP_Config_pdcp_DuplicationConfig_r15_UNBOUND_VALUE = 0,
	PDCP_Config_pdcp_DuplicationConfig_r15_release = 1,
	PDCP_Config_pdcp_DuplicationConfig_r15_setup = 2,
};

union PDCP_Config_pdcp_DuplicationConfig_r15_Value {
	Null release;
	struct PDCP_Config_pdcp_DuplicationConfig_r15_setup setup;
};

struct PDCP_Config_pdcp_DuplicationConfig_r15 {
	enum PDCP_Config_pdcp_DuplicationConfig_r15_Sel d;
	union PDCP_Config_pdcp_DuplicationConfig_r15_Value v;
};

enum EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e {
	EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e_bits7 = 0,
	EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e_bits15 = 1,
};

typedef enum EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e;

struct EthernetHeaderCompression_r16_ehc_Common_r16 {
	EthernetHeaderCompression_r16_ehc_Common_r16_ehc_CID_Length_r16_e ehc_CID_Length_r16;
};

enum EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e {
	EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e_true = 0,
};

typedef enum EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e;

struct EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e_drb_ContinueEHC_DL_r16_Optional {
	bool d;
	EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e v;
};

struct EthernetHeaderCompression_r16_ehc_Downlink_r16 {
	struct EthernetHeaderCompression_r16_ehc_Downlink_r16_drb_ContinueEHC_DL_r16_e_drb_ContinueEHC_DL_r16_Optional drb_ContinueEHC_DL_r16;
};

enum EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e {
	EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e_true = 0,
};

typedef enum EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e;

struct EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e_drb_ContinueEHC_UL_r16_Optional {
	bool d;
	EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e v;
};

struct EthernetHeaderCompression_r16_ehc_Uplink_r16 {
	uint16_t maxCID_EHC_UL_r16;
	struct EthernetHeaderCompression_r16_ehc_Uplink_r16_drb_ContinueEHC_UL_r16_e_drb_ContinueEHC_UL_r16_Optional drb_ContinueEHC_UL_r16;
};

struct EthernetHeaderCompression_r16_ehc_Downlink_r16_ehc_Downlink_r16_Optional {
	bool d;
	struct EthernetHeaderCompression_r16_ehc_Downlink_r16 v;
};

struct EthernetHeaderCompression_r16_ehc_Uplink_r16_ehc_Uplink_r16_Optional {
	bool d;
	struct EthernetHeaderCompression_r16_ehc_Uplink_r16 v;
};

struct EthernetHeaderCompression_r16 {
	struct EthernetHeaderCompression_r16_ehc_Common_r16 ehc_Common_r16;
	struct EthernetHeaderCompression_r16_ehc_Downlink_r16_ehc_Downlink_r16_Optional ehc_Downlink_r16;
	struct EthernetHeaderCompression_r16_ehc_Uplink_r16_ehc_Uplink_r16_Optional ehc_Uplink_r16;
};

enum SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_Sel {
	SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_UNBOUND_VALUE = 0,
	SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_release = 1,
	SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_setup = 2,
};

union SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_Value {
	Null release;
	struct EthernetHeaderCompression_r16 setup;
};

struct SetupRelease_PDCP_Config_ethernetHeaderCompression_r16 {
	enum SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_Sel d;
	union SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_Value v;
};

struct PDCP_Config_discardTimer_e_discardTimer_Optional {
	bool d;
	PDCP_Config_discardTimer_e v;
};

struct PDCP_Config_rlc_AM_rlc_AM_Optional {
	bool d;
	struct PDCP_Config_rlc_AM v;
};

struct PDCP_Config_rlc_UM_rlc_UM_Optional {
	bool d;
	struct PDCP_Config_rlc_UM v;
};

struct PDCP_Config_rn_IntegrityProtection_r10_e_rn_IntegrityProtection_r10_Optional {
	bool d;
	PDCP_Config_rn_IntegrityProtection_r10_e v;
};

struct PDCP_Config_pdcp_SN_Size_v1130_e_pdcp_SN_Size_v1130_Optional {
	bool d;
	PDCP_Config_pdcp_SN_Size_v1130_e v;
};

struct bool_PDCP_Config_ul_DataSplitDRB_ViaSCG_r12_Optional {
	bool d;
	bool v;
};

struct PDCP_Config_t_Reordering_r12_e_t_Reordering_r12_Optional {
	bool d;
	PDCP_Config_t_Reordering_r12_e v;
};

struct PDCP_Config_ul_DataSplitThreshold_r13_ul_DataSplitThreshold_r13_Optional {
	bool d;
	struct PDCP_Config_ul_DataSplitThreshold_r13 v;
};

struct PDCP_Config_pdcp_SN_Size_v1310_e_pdcp_SN_Size_v1310_Optional {
	bool d;
	PDCP_Config_pdcp_SN_Size_v1310_e v;
};

struct PDCP_Config_statusFeedback_r13_statusFeedback_r13_Optional {
	bool d;
	struct PDCP_Config_statusFeedback_r13 v;
};

struct PDCP_Config_ul_LWA_Config_r14_ul_LWA_Config_r14_Optional {
	bool d;
	struct PDCP_Config_ul_LWA_Config_r14 v;
};

struct PDCP_Config_uplinkOnlyHeaderCompression_r14_uplinkOnlyHeaderCompression_r14_Optional {
	bool d;
	struct PDCP_Config_uplinkOnlyHeaderCompression_r14 v;
};

struct PDCP_Config_uplinkDataCompression_r15_uplinkDataCompression_r15_Optional {
	bool d;
	struct PDCP_Config_uplinkDataCompression_r15 v;
};

struct PDCP_Config_pdcp_DuplicationConfig_r15_pdcp_DuplicationConfig_r15_Optional {
	bool d;
	struct PDCP_Config_pdcp_DuplicationConfig_r15 v;
};

struct SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_PDCP_Config_ethernetHeaderCompression_r16_Optional {
	bool d;
	struct SetupRelease_PDCP_Config_ethernetHeaderCompression_r16 v;
};

struct PDCP_Config {
	struct PDCP_Config_discardTimer_e_discardTimer_Optional discardTimer;
	struct PDCP_Config_rlc_AM_rlc_AM_Optional rlc_AM;
	struct PDCP_Config_rlc_UM_rlc_UM_Optional rlc_UM;
	struct PDCP_Config_headerCompression headerCompression;
	struct PDCP_Config_rn_IntegrityProtection_r10_e_rn_IntegrityProtection_r10_Optional rn_IntegrityProtection_r10;
	struct PDCP_Config_pdcp_SN_Size_v1130_e_pdcp_SN_Size_v1130_Optional pdcp_SN_Size_v1130;
	struct bool_PDCP_Config_ul_DataSplitDRB_ViaSCG_r12_Optional ul_DataSplitDRB_ViaSCG_r12;
	struct PDCP_Config_t_Reordering_r12_e_t_Reordering_r12_Optional t_Reordering_r12;
	struct PDCP_Config_ul_DataSplitThreshold_r13_ul_DataSplitThreshold_r13_Optional ul_DataSplitThreshold_r13;
	struct PDCP_Config_pdcp_SN_Size_v1310_e_pdcp_SN_Size_v1310_Optional pdcp_SN_Size_v1310;
	struct PDCP_Config_statusFeedback_r13_statusFeedback_r13_Optional statusFeedback_r13;
	struct PDCP_Config_ul_LWA_Config_r14_ul_LWA_Config_r14_Optional ul_LWA_Config_r14;
	struct PDCP_Config_uplinkOnlyHeaderCompression_r14_uplinkOnlyHeaderCompression_r14_Optional uplinkOnlyHeaderCompression_r14;
	struct PDCP_Config_uplinkDataCompression_r15_uplinkDataCompression_r15_Optional uplinkDataCompression_r15;
	struct PDCP_Config_pdcp_DuplicationConfig_r15_pdcp_DuplicationConfig_r15_Optional pdcp_DuplicationConfig_r15;
	struct SetupRelease_PDCP_Config_ethernetHeaderCompression_r16_PDCP_Config_ethernetHeaderCompression_r16_Optional ethernetHeaderCompression_r16;
};

struct PRACH_ConfigInfo_PRACH_Config_prach_ConfigInfo_Optional {
	bool d;
	struct PRACH_ConfigInfo v;
};

struct PRACH_Config {
	uint16_t rootSequenceIndex;
	struct PRACH_ConfigInfo_PRACH_Config_prach_ConfigInfo_Optional prach_ConfigInfo;
};

enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e {
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v1 = 0,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v1dot5 = 1,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v2 = 2,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v2dot5 = 3,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v4 = 4,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v5 = 5,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v8 = 6,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e_v10 = 7,
};

typedef enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e;

enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e {
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v1 = 0,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v2 = 1,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v4 = 2,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v5 = 3,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v8 = 4,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v10 = 5,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_v20 = 6,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e_spare = 7,
};

typedef enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e;

enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_Sel {
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_UNBOUND_VALUE = 0,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13 = 1,
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13 = 2,
};

union PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_Value {
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_fdd_r13_e fdd_r13;
	PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_tdd_r13_e tdd_r13;
};

struct PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13 {
	enum PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_Sel d;
	union PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_Value v;
};

struct RSRP_ThresholdsPrachInfoList_r13_PRACH_Config_v1310_rsrp_ThresholdsPrachInfoList_r13_Optional {
	bool d;
	RSRP_ThresholdsPrachInfoList_r13 v;
};

struct PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_mpdcch_startSF_CSS_RA_r13_Optional {
	bool d;
	struct PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13 v;
};

struct uint8_t_PRACH_Config_v1310_prach_HoppingOffset_r13_Optional {
	bool d;
	uint8_t v;
};

struct PRACH_ParametersListCE_r13_PRACH_Config_v1310_prach_ParametersListCE_r13_Optional {
	bool d;
	PRACH_ParametersListCE_r13 v;
};

struct uint8_t_PRACH_Config_v1310_initial_CE_level_r13_Optional {
	bool d;
	uint8_t v;
};

struct PRACH_Config_v1310 {
	struct RSRP_ThresholdsPrachInfoList_r13_PRACH_Config_v1310_rsrp_ThresholdsPrachInfoList_r13_Optional rsrp_ThresholdsPrachInfoList_r13;
	struct PRACH_Config_v1310_mpdcch_startSF_CSS_RA_r13_mpdcch_startSF_CSS_RA_r13_Optional mpdcch_startSF_CSS_RA_r13;
	struct uint8_t_PRACH_Config_v1310_prach_HoppingOffset_r13_Optional prach_HoppingOffset_r13;
	struct PRACH_ParametersListCE_r13_PRACH_Config_v1310_prach_ParametersListCE_r13_Optional prach_ParametersListCE_r13;
	struct uint8_t_PRACH_Config_v1310_initial_CE_level_r13_Optional initial_CE_level_r13;
};

enum PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e {
	PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e_n2 = 0,
	PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e_n4 = 1,
	PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e_n6 = 2,
	PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e_spare1 = 3,
};

typedef enum PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e;

struct PUCCH_ConfigDedicated_ackNackRepetition_setup {
	PUCCH_ConfigDedicated_ackNackRepetition_setup_repetitionFactor_e repetitionFactor;
	uint16_t n1PUCCH_AN_Rep;
};

enum PUCCH_ConfigDedicated_ackNackRepetition_Sel {
	PUCCH_ConfigDedicated_ackNackRepetition_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_ackNackRepetition_release = 1,
	PUCCH_ConfigDedicated_ackNackRepetition_setup = 2,
};

union PUCCH_ConfigDedicated_ackNackRepetition_Value {
	Null release;
	struct PUCCH_ConfigDedicated_ackNackRepetition_setup setup;
};

struct PUCCH_ConfigDedicated_ackNackRepetition {
	enum PUCCH_ConfigDedicated_ackNackRepetition_Sel d;
	union PUCCH_ConfigDedicated_ackNackRepetition_Value v;
};

enum PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e {
	PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e_bundling = 0,
	PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e_multiplexing = 1,
};

typedef enum PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e;

struct PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e_tdd_AckNackFeedbackMode_Optional {
	bool d;
	PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e v;
};

struct PUCCH_ConfigDedicated {
	struct PUCCH_ConfigDedicated_ackNackRepetition ackNackRepetition;
	struct PUCCH_ConfigDedicated_tdd_AckNackFeedbackMode_e_tdd_AckNackFeedbackMode_Optional tdd_AckNackFeedbackMode;
};

enum PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e {
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e_n2 = 0,
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e_n4 = 1,
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e_n6 = 2,
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e_spare1 = 3,
};

typedef enum PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e;

struct PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup {
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup_repetitionFactor_r13_e repetitionFactor_r13;
	uint16_t n1PUCCH_AN_Rep_r13;
};

enum PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_Sel {
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_release = 1,
	PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_ackNackRepetition_r13 {
	enum PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_ackNackRepetition_r13_Value v;
};

enum PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e {
	PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e_bundling = 0,
	PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e_multiplexing = 1,
};

typedef enum PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e;

struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup_n3PUCCH_AN_ListP1_r13_Dynamic {
	size_t d;
	uint16_t* v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup {
	struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup_n3PUCCH_AN_ListP1_r13_Dynamic n3PUCCH_AN_ListP1_r13;
};

enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Sel {
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_release = 1,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13 {
	enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Value v;
};

struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_n3PUCCH_AN_List_r13_Dynamic {
	size_t d;
	uint16_t* v;
};

struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_n3PUCCH_AN_List_r13_DynamicOptional {
	bool d;
	struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_n3PUCCH_AN_List_r13_Dynamic v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13 v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13 {
	struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_n3PUCCH_AN_List_r13_DynamicOptional n3PUCCH_AN_List_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Optional twoAntennaPortActivatedPUCCH_Format3_r13;
};

struct uint16_t_N1PUCCH_AN_CS_r10_Dynamic {
	size_t d;
	uint16_t* v;
};

typedef struct uint16_t_N1PUCCH_AN_CS_r10_Dynamic N1PUCCH_AN_CS_r10;

struct N1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_List_r13_Dynamic {
	size_t d;
	N1PUCCH_AN_CS_r10* v;
};

struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_setup_dummy1_Dynamic {
	size_t d;
	uint16_t* v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_setup {
	struct N1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_List_r13_Dynamic n1PUCCH_AN_CS_List_r13;
	struct uint16_t_PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_setup_dummy1_Dynamic dummy1;
};

enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_Sel {
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_release = 1,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13 {
	enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_Value v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_n1PUCCH_AN_CS_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13 v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13 {
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13_n1PUCCH_AN_CS_r13_n1PUCCH_AN_CS_r13_Optional n1PUCCH_AN_CS_r13;
};

struct Format4_resource_r13 {
	uint8_t startingPRB_format4_r13;
	uint8_t numberOfPRB_format4_r13;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13_Format4_resource_r13_format4_MultiCSI_resourceConfiguration_r13_Dynamic {
	size_t d;
	struct Format4_resource_r13* v;
};

struct Format4_resource_r13_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13_format4_MultiCSI_resourceConfiguration_r13_DynamicOptional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13_Format4_resource_r13_format4_MultiCSI_resourceConfiguration_r13_Dynamic v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13 {
	struct Format4_resource_r13 format4_resourceConfiguration_r13[4];
	struct Format4_resource_r13_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13_format4_MultiCSI_resourceConfiguration_r13_DynamicOptional format4_MultiCSI_resourceConfiguration_r13;
};

struct Format5_resource_r13 {
	uint8_t startingPRB_format5_r13;
	uint8_t cdm_index_format5_r13;
};

struct Format5_resource_r13_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format5_r13_format5_MultiCSI_resourceConfiguration_r13_Optional {
	bool d;
	struct Format5_resource_r13 v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format5_r13 {
	struct Format5_resource_r13 format5_resourceConfiguration_r13[4];
	struct Format5_resource_r13_PUCCH_ConfigDedicated_r13_pucch_Format_r13_format5_r13_format5_MultiCSI_resourceConfiguration_r13_Optional format5_MultiCSI_resourceConfiguration_r13;
};

enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_Sel {
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13 = 1,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13 = 2,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13 = 3,
	PUCCH_ConfigDedicated_r13_pucch_Format_r13_format5_r13 = 4,
};

union PUCCH_ConfigDedicated_r13_pucch_Format_r13_Value {
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format3_r13 format3_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_channelSelection_r13 channelSelection_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format4_r13 format4_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_format5_r13 format5_r13;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13 {
	enum PUCCH_ConfigDedicated_r13_pucch_Format_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_pucch_Format_r13_Value v;
};

enum PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e {
	PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e_true = 0,
};

typedef enum PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e;

enum PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e {
	PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e_true = 0,
};

typedef enum PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e;

struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_setup {
	uint16_t nPUCCH_Identity_r13;
	uint16_t n1PUCCH_AN_r13;
};

enum PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_Sel {
	PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_release = 1,
	PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13 {
	enum PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_Value v;
};

struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_setup {
	uint16_t nkaPUCCH_AN_r13;
};

enum PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_Sel {
	PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_release = 1,
	PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13 {
	enum PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_Value v;
};

enum PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e {
	PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e_dai = 0,
	PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e_cc = 1,
};

typedef enum PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e;

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e_r1 = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e_r2 = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e_r4 = 2,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e_r8 = 3,
};

typedef enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e;

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e_r1 = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e_r2 = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e_r4 = 2,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e_r8 = 3,
};

typedef enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e;

struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format1_r13_e pucch_NumRepetitionCE_format1_r13;
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA_pucch_NumRepetitionCE_format2_r13_e pucch_NumRepetitionCE_format2_r13;
};

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e_r4 = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e_r8 = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e_r16 = 2,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e_r32 = 3,
};

typedef enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e;

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e_r4 = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e_r8 = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e_r16 = 2,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e_r32 = 3,
};

typedef enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e;

struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format1_r13_e pucch_NumRepetitionCE_format1_r13;
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB_pucch_NumRepetitionCE_format2_r13_e pucch_NumRepetitionCE_format2_r13;
};

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_Sel {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB = 2,
};

union PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_Value {
	struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeA modeA;
	struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_modeB modeB;
};

struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup {
	enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_Sel d;
	union PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup_Value v;
};

enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_Sel {
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_release = 1,
	PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup = 2,
};

union PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_Value {
	Null release;
	struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_setup setup;
};

struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13 {
	enum PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_Sel d;
	union PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_Value v;
};

struct PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e_tdd_AckNackFeedbackMode_r13_Optional {
	bool d;
	PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e v;
};

struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_pucch_Format_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13 v;
};

struct PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e_twoAntennaPortActivatedPUCCH_Format1a1b_r13_Optional {
	bool d;
	PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e v;
};

struct PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e_simultaneousPUCCH_PUSCH_r13_Optional {
	bool d;
	PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e v;
};

struct uint16_t_PUCCH_ConfigDedicated_r13_n1PUCCH_AN_RepP1_r13_Optional {
	bool d;
	uint16_t v;
};

struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_nPUCCH_Param_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13 v;
};

struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_nkaPUCCH_Param_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13 v;
};

struct PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e_codebooksizeDetermination_r13_Optional {
	bool d;
	PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e v;
};

struct uint8_t_PUCCH_ConfigDedicated_r13_maximumPayloadCoderate_r13_Optional {
	bool d;
	uint8_t v;
};

struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_pucch_NumRepetitionCE_r13_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13 v;
};

struct PUCCH_ConfigDedicated_r13 {
	struct PUCCH_ConfigDedicated_r13_ackNackRepetition_r13 ackNackRepetition_r13;
	struct PUCCH_ConfigDedicated_r13_tdd_AckNackFeedbackMode_r13_e_tdd_AckNackFeedbackMode_r13_Optional tdd_AckNackFeedbackMode_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_Format_r13_pucch_Format_r13_Optional pucch_Format_r13;
	struct PUCCH_ConfigDedicated_r13_twoAntennaPortActivatedPUCCH_Format1a1b_r13_e_twoAntennaPortActivatedPUCCH_Format1a1b_r13_Optional twoAntennaPortActivatedPUCCH_Format1a1b_r13;
	struct PUCCH_ConfigDedicated_r13_simultaneousPUCCH_PUSCH_r13_e_simultaneousPUCCH_PUSCH_r13_Optional simultaneousPUCCH_PUSCH_r13;
	struct uint16_t_PUCCH_ConfigDedicated_r13_n1PUCCH_AN_RepP1_r13_Optional n1PUCCH_AN_RepP1_r13;
	struct PUCCH_ConfigDedicated_r13_nPUCCH_Param_r13_nPUCCH_Param_r13_Optional nPUCCH_Param_r13;
	struct PUCCH_ConfigDedicated_r13_nkaPUCCH_Param_r13_nkaPUCCH_Param_r13_Optional nkaPUCCH_Param_r13;
	bool spatialBundlingPUCCH_r13;
	bool spatialBundlingPUSCH_r13;
	bool harq_TimingTDD_r13;
	struct PUCCH_ConfigDedicated_r13_codebooksizeDetermination_r13_e_codebooksizeDetermination_r13_Optional codebooksizeDetermination_r13;
	struct uint8_t_PUCCH_ConfigDedicated_r13_maximumPayloadCoderate_r13_Optional maximumPayloadCoderate_r13;
	struct PUCCH_ConfigDedicated_r13_pucch_NumRepetitionCE_r13_pucch_NumRepetitionCE_r13_Optional pucch_NumRepetitionCE_r13;
};

struct uint16_t_PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup_n3PUCCH_AN_ListP1_r13_Dynamic {
	size_t d;
	uint16_t* v;
};

struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup {
	struct uint16_t_PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup_n3PUCCH_AN_ListP1_r13_Dynamic n3PUCCH_AN_ListP1_r13;
};

enum PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Sel {
	PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_UNBOUND_VALUE = 0,
	PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_release = 1,
	PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup = 2,
};

union PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Value {
	Null release;
	struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_setup setup;
};

struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13 {
	enum PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Sel d;
	union PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Value v;
};

struct uint16_t_PUCCH_Format3_Conf_r13_n3PUCCH_AN_List_r13_Dynamic {
	size_t d;
	uint16_t* v;
};

struct uint16_t_PUCCH_Format3_Conf_r13_n3PUCCH_AN_List_r13_DynamicOptional {
	bool d;
	struct uint16_t_PUCCH_Format3_Conf_r13_n3PUCCH_AN_List_r13_Dynamic v;
};

struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Optional {
	bool d;
	struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13 v;
};

struct PUCCH_Format3_Conf_r13 {
	struct uint16_t_PUCCH_Format3_Conf_r13_n3PUCCH_AN_List_r13_DynamicOptional n3PUCCH_AN_List_r13;
	struct PUCCH_Format3_Conf_r13_twoAntennaPortActivatedPUCCH_Format3_r13_twoAntennaPortActivatedPUCCH_Format3_r13_Optional twoAntennaPortActivatedPUCCH_Format3_r13;
};

struct N1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_List_r10_Dynamic {
	size_t d;
	N1PUCCH_AN_CS_r10* v;
};

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup {
	struct N1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_List_r10_Dynamic n1PUCCH_AN_CS_List_r10;
};

enum PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_Sel {
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_release = 1,
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup = 2,
};

union PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_Value {
	Null release;
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup setup;
};

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10 {
	enum PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_Sel d;
	union PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_Value v;
};

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_r10_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10 v;
};

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10 {
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_n1PUCCH_AN_CS_r10_Optional n1PUCCH_AN_CS_r10;
};

enum PUCCH_ConfigDedicated_v1020_pucch_Format_r10_Sel {
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_format3_r10 = 1,
	PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10 = 2,
};

union PUCCH_ConfigDedicated_v1020_pucch_Format_r10_Value {
	struct PUCCH_Format3_Conf_r13 format3_r10;
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10 channelSelection_r10;
};

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10 {
	enum PUCCH_ConfigDedicated_v1020_pucch_Format_r10_Sel d;
	union PUCCH_ConfigDedicated_v1020_pucch_Format_r10_Value v;
};

enum PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e {
	PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e_true = 0,
};

typedef enum PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e;

enum PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e {
	PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e_true = 0,
};

typedef enum PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e;

struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_pucch_Format_r10_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10 v;
};

struct PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e_twoAntennaPortActivatedPUCCH_Format1a1b_r10_Optional {
	bool d;
	PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e v;
};

struct PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e_simultaneousPUCCH_PUSCH_r10_Optional {
	bool d;
	PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e v;
};

struct uint16_t_PUCCH_ConfigDedicated_v1020_n1PUCCH_AN_RepP1_r10_Optional {
	bool d;
	uint16_t v;
};

struct PUCCH_ConfigDedicated_v1020 {
	struct PUCCH_ConfigDedicated_v1020_pucch_Format_r10_pucch_Format_r10_Optional pucch_Format_r10;
	struct PUCCH_ConfigDedicated_v1020_twoAntennaPortActivatedPUCCH_Format1a1b_r10_e_twoAntennaPortActivatedPUCCH_Format1a1b_r10_Optional twoAntennaPortActivatedPUCCH_Format1a1b_r10;
	struct PUCCH_ConfigDedicated_v1020_simultaneousPUCCH_PUSCH_r10_e_simultaneousPUCCH_PUSCH_r10_Optional simultaneousPUCCH_PUSCH_r10;
	struct uint16_t_PUCCH_ConfigDedicated_v1020_n1PUCCH_AN_RepP1_r10_Optional n1PUCCH_AN_RepP1_r10;
};

struct uint16_t_PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_setup_n1PUCCH_AN_CS_ListP1_r11_Dynamic {
	size_t d;
	uint16_t* v;
};

struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_setup {
	struct uint16_t_PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_setup_n1PUCCH_AN_CS_ListP1_r11_Dynamic n1PUCCH_AN_CS_ListP1_r11;
};

enum PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_Sel {
	PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_release = 1,
	PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_setup = 2,
};

union PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_Value {
	Null release;
	struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_setup setup;
};

struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130 {
	enum PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_Sel d;
	union PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_Value v;
};

struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_setup {
	uint16_t nPUCCH_Identity_r11;
	uint16_t n1PUCCH_AN_r11;
};

enum PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_Sel {
	PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_release = 1,
	PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_setup = 2,
};

union PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_Value {
	Null release;
	struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_setup setup;
};

struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11 {
	enum PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_Sel d;
	union PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_Value v;
};

struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_n1PUCCH_AN_CS_v1130_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130 v;
};

struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_nPUCCH_Param_r11_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11 v;
};

struct PUCCH_ConfigDedicated_v1130 {
	struct PUCCH_ConfigDedicated_v1130_n1PUCCH_AN_CS_v1130_n1PUCCH_AN_CS_v1130_Optional n1PUCCH_AN_CS_v1130;
	struct PUCCH_ConfigDedicated_v1130_nPUCCH_Param_r11_nPUCCH_Param_r11_Optional nPUCCH_Param_r11;
};

struct PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_setup {
	uint16_t nkaPUCCH_AN_r12;
};

enum PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_Sel {
	PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_release = 1,
	PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_setup = 2,
};

union PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_Value {
	Null release;
	struct PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_setup setup;
};

struct PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12 {
	enum PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_Sel d;
	union PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12_Value v;
};

struct PUCCH_ConfigDedicated_v1250 {
	struct PUCCH_ConfigDedicated_v1250_nkaPUCCH_Param_r12 nkaPUCCH_Param_r12;
};

struct PUSCH_ConfigDedicated {
	uint8_t betaOffset_ACK_Index;
	uint8_t betaOffset_RI_Index;
	uint8_t betaOffset_CQI_Index;
};

struct uint8_t_PUSCH_ConfigDedicated_r13_betaOffsetMC_r13_betaOffset2_ACK_Index_MC_r13_Optional {
	bool d;
	uint8_t v;
};

struct PUSCH_ConfigDedicated_r13_betaOffsetMC_r13 {
	uint8_t betaOffset_ACK_Index_MC_r13;
	struct uint8_t_PUSCH_ConfigDedicated_r13_betaOffsetMC_r13_betaOffset2_ACK_Index_MC_r13_Optional betaOffset2_ACK_Index_MC_r13;
	uint8_t betaOffset_RI_Index_MC_r13;
	uint8_t betaOffset_CQI_Index_MC_r13;
};

enum PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e {
	PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e_true = 0,
};

typedef enum PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e;

enum PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e {
	PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e_true = 0,
};

typedef enum PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e;

struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_setup {
	uint16_t nPUSCH_Identity_r13;
	uint16_t nDMRS_CSH_Identity_r13;
};

enum PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_Sel {
	PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_UNBOUND_VALUE = 0,
	PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_release = 1,
	PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_setup = 2,
};

union PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_Value {
	Null release;
	struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_setup setup;
};

struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11 {
	enum PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_Sel d;
	union PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_Value v;
};

struct uint8_t_PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffset2_ACK_Index_MC_SubframeSet2_r13_Optional {
	bool d;
	uint8_t v;
};

struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12 {
	uint8_t betaOffset_ACK_Index_MC_SubframeSet2_r13;
	struct uint8_t_PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffset2_ACK_Index_MC_SubframeSet2_r13_Optional betaOffset2_ACK_Index_MC_SubframeSet2_r13;
	uint8_t betaOffset_RI_Index_MC_SubframeSet2_r13;
	uint8_t betaOffset_CQI_Index_MC_SubframeSet2_r13;
};

struct uint8_t_PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffset2_ACK_Index_SubframeSet2_r13_Optional {
	bool d;
	uint8_t v;
};

struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffsetMC_r12_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12 v;
};

struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup {
	uint8_t betaOffset_ACK_Index_SubframeSet2_r13;
	struct uint8_t_PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffset2_ACK_Index_SubframeSet2_r13_Optional betaOffset2_ACK_Index_SubframeSet2_r13;
	uint8_t betaOffset_RI_Index_SubframeSet2_r13;
	uint8_t betaOffset_CQI_Index_SubframeSet2_r13;
	struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffsetMC_r12_Optional betaOffsetMC_r12;
};

enum PUSCH_ConfigDedicated_r13_uciOnPUSCH_Sel {
	PUSCH_ConfigDedicated_r13_uciOnPUSCH_UNBOUND_VALUE = 0,
	PUSCH_ConfigDedicated_r13_uciOnPUSCH_release = 1,
	PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup = 2,
};

union PUSCH_ConfigDedicated_r13_uciOnPUSCH_Value {
	Null release;
	struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_setup setup;
};

struct PUSCH_ConfigDedicated_r13_uciOnPUSCH {
	enum PUSCH_ConfigDedicated_r13_uciOnPUSCH_Sel d;
	union PUSCH_ConfigDedicated_r13_uciOnPUSCH_Value v;
};

enum PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e {
	PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e_on = 0,
};

typedef enum PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e;

struct uint8_t_PUSCH_ConfigDedicated_r13_betaOffset2_ACK_Index_r13_Optional {
	bool d;
	uint8_t v;
};

struct PUSCH_ConfigDedicated_r13_betaOffsetMC_r13_betaOffsetMC_r13_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_r13_betaOffsetMC_r13 v;
};

struct PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e_groupHoppingDisabled_r13_Optional {
	bool d;
	PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e v;
};

struct PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e_dmrs_WithOCC_Activated_r13_Optional {
	bool d;
	PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e v;
};

struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_pusch_DMRS_r11_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11 v;
};

struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_uciOnPUSCH_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_r13_uciOnPUSCH v;
};

struct PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e_pusch_HoppingConfig_r13_Optional {
	bool d;
	PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e v;
};

struct PUSCH_ConfigDedicated_r13 {
	uint8_t betaOffset_ACK_Index_r13;
	struct uint8_t_PUSCH_ConfigDedicated_r13_betaOffset2_ACK_Index_r13_Optional betaOffset2_ACK_Index_r13;
	uint8_t betaOffset_RI_Index_r13;
	uint8_t betaOffset_CQI_Index_r13;
	struct PUSCH_ConfigDedicated_r13_betaOffsetMC_r13_betaOffsetMC_r13_Optional betaOffsetMC_r13;
	struct PUSCH_ConfigDedicated_r13_groupHoppingDisabled_r13_e_groupHoppingDisabled_r13_Optional groupHoppingDisabled_r13;
	struct PUSCH_ConfigDedicated_r13_dmrs_WithOCC_Activated_r13_e_dmrs_WithOCC_Activated_r13_Optional dmrs_WithOCC_Activated_r13;
	struct PUSCH_ConfigDedicated_r13_pusch_DMRS_r11_pusch_DMRS_r11_Optional pusch_DMRS_r11;
	struct PUSCH_ConfigDedicated_r13_uciOnPUSCH_uciOnPUSCH_Optional uciOnPUSCH;
	struct PUSCH_ConfigDedicated_r13_pusch_HoppingConfig_r13_e_pusch_HoppingConfig_r13_Optional pusch_HoppingConfig_r13;
};

struct PUSCH_ConfigDedicated_v1020_betaOffsetMC_r10 {
	uint8_t betaOffset_ACK_Index_MC_r10;
	uint8_t betaOffset_RI_Index_MC_r10;
	uint8_t betaOffset_CQI_Index_MC_r10;
};

enum PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e {
	PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e_true = 0,
};

typedef enum PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e;

enum PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e {
	PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e_true = 0,
};

typedef enum PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e;

struct PUSCH_ConfigDedicated_v1020_betaOffsetMC_r10_betaOffsetMC_r10_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1020_betaOffsetMC_r10 v;
};

struct PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e_groupHoppingDisabled_r10_Optional {
	bool d;
	PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e v;
};

struct PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e_dmrs_WithOCC_Activated_r10_Optional {
	bool d;
	PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e v;
};

struct PUSCH_ConfigDedicated_v1020 {
	struct PUSCH_ConfigDedicated_v1020_betaOffsetMC_r10_betaOffsetMC_r10_Optional betaOffsetMC_r10;
	struct PUSCH_ConfigDedicated_v1020_groupHoppingDisabled_r10_e_groupHoppingDisabled_r10_Optional groupHoppingDisabled_r10;
	struct PUSCH_ConfigDedicated_v1020_dmrs_WithOCC_Activated_r10_e_dmrs_WithOCC_Activated_r10_Optional dmrs_WithOCC_Activated_r10;
};

struct PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_setup {
	uint16_t nPUSCH_Identity_r11;
	uint16_t nDMRS_CSH_Identity_r11;
};

enum PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_Sel {
	PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_UNBOUND_VALUE = 0,
	PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_release = 1,
	PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_setup = 2,
};

union PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_Value {
	Null release;
	struct PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_setup setup;
};

struct PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11 {
	enum PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_Sel d;
	union PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11_Value v;
};

struct PUSCH_ConfigDedicated_v1130 {
	struct PUSCH_ConfigDedicated_v1130_pusch_DMRS_r11 pusch_DMRS_r11;
};

struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup_betaOffsetMC_r12 {
	uint8_t betaOffset_ACK_Index_MC_SubframeSet2_r12;
	uint8_t betaOffset_RI_Index_MC_SubframeSet2_r12;
	uint8_t betaOffset_CQI_Index_MC_SubframeSet2_r12;
};

struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffsetMC_r12_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup_betaOffsetMC_r12 v;
};

struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup {
	uint8_t betaOffset_ACK_Index_SubframeSet2_r12;
	uint8_t betaOffset_RI_Index_SubframeSet2_r12;
	uint8_t betaOffset_CQI_Index_SubframeSet2_r12;
	struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup_betaOffsetMC_r12_betaOffsetMC_r12_Optional betaOffsetMC_r12;
};

enum PUSCH_ConfigDedicated_v1250_uciOnPUSCH_Sel {
	PUSCH_ConfigDedicated_v1250_uciOnPUSCH_UNBOUND_VALUE = 0,
	PUSCH_ConfigDedicated_v1250_uciOnPUSCH_release = 1,
	PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup = 2,
};

union PUSCH_ConfigDedicated_v1250_uciOnPUSCH_Value {
	Null release;
	struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH_setup setup;
};

struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH {
	enum PUSCH_ConfigDedicated_v1250_uciOnPUSCH_Sel d;
	union PUSCH_ConfigDedicated_v1250_uciOnPUSCH_Value v;
};

struct PUSCH_ConfigDedicated_v1250 {
	struct PUSCH_ConfigDedicated_v1250_uciOnPUSCH uciOnPUSCH;
};

enum PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e {
	PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e_on = 0,
};

typedef enum PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e;

enum PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e {
	PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e_bw5 = 0,
};

typedef enum PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e;

enum TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e {
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym1 = 0,
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym2 = 1,
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym3 = 2,
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym4 = 3,
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym5 = 4,
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_sym6 = 5,
};

typedef enum TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e;

enum TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e {
	TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e_true = 0,
};

typedef enum TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e;

struct TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_symPUSCH_UpPTS_r14_Optional {
	bool d;
	TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e v;
};

struct TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e_dmrs_LessUpPTS_Config_r14_Optional {
	bool d;
	TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e v;
};

struct TDD_PUSCH_UpPTS_r14_setup {
	struct TDD_PUSCH_UpPTS_r14_setup_symPUSCH_UpPTS_r14_e_symPUSCH_UpPTS_r14_Optional symPUSCH_UpPTS_r14;
	struct TDD_PUSCH_UpPTS_r14_setup_dmrs_LessUpPTS_Config_r14_e_dmrs_LessUpPTS_Config_r14_Optional dmrs_LessUpPTS_Config_r14;
};

enum TDD_PUSCH_UpPTS_r14_Sel {
	TDD_PUSCH_UpPTS_r14_UNBOUND_VALUE = 0,
	TDD_PUSCH_UpPTS_r14_release = 1,
	TDD_PUSCH_UpPTS_r14_setup = 2,
};

union TDD_PUSCH_UpPTS_r14_Value {
	Null release;
	struct TDD_PUSCH_UpPTS_r14_setup setup;
};

struct TDD_PUSCH_UpPTS_r14 {
	enum TDD_PUSCH_UpPTS_r14_Sel d;
	union TDD_PUSCH_UpPTS_r14_Value v;
};

struct Enable256QAM_r14_setup_tpc_SubframeSet_Configured_r14 {
	bool subframeSet1_DCI_Format0_r14;
	bool subframeSet1_DCI_Format4_r14;
	bool subframeSet2_DCI_Format0_r14;
	bool subframeSet2_DCI_Format4_r14;
};

struct Enable256QAM_r14_setup_tpc_SubframeSet_NotConfigured_r14 {
	bool dci_Format0_r14;
	bool dci_Format4_r14;
};

enum Enable256QAM_r14_setup_Sel {
	Enable256QAM_r14_setup_UNBOUND_VALUE = 0,
	Enable256QAM_r14_setup_tpc_SubframeSet_Configured_r14 = 1,
	Enable256QAM_r14_setup_tpc_SubframeSet_NotConfigured_r14 = 2,
};

union Enable256QAM_r14_setup_Value {
	struct Enable256QAM_r14_setup_tpc_SubframeSet_Configured_r14 tpc_SubframeSet_Configured_r14;
	struct Enable256QAM_r14_setup_tpc_SubframeSet_NotConfigured_r14 tpc_SubframeSet_NotConfigured_r14;
};

struct Enable256QAM_r14_setup {
	enum Enable256QAM_r14_setup_Sel d;
	union Enable256QAM_r14_setup_Value v;
};

enum Enable256QAM_r14_Sel {
	Enable256QAM_r14_UNBOUND_VALUE = 0,
	Enable256QAM_r14_release = 1,
	Enable256QAM_r14_setup = 2,
};

union Enable256QAM_r14_Value {
	Null release;
	struct Enable256QAM_r14_setup setup;
};

struct Enable256QAM_r14 {
	enum Enable256QAM_r14_Sel d;
	union Enable256QAM_r14_Value v;
};

struct PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e_ce_PUSCH_NB_MaxTBS_r14_Optional {
	bool d;
	PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e v;
};

struct PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e_ce_PUSCH_MaxBandwidth_r14_Optional {
	bool d;
	PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e v;
};

struct TDD_PUSCH_UpPTS_r14_PUSCH_ConfigDedicated_v1430_tdd_PUSCH_UpPTS_r14_Optional {
	bool d;
	struct TDD_PUSCH_UpPTS_r14 v;
};

struct Enable256QAM_r14_PUSCH_ConfigDedicated_v1430_enable256QAM_r14_Optional {
	bool d;
	struct Enable256QAM_r14 v;
};

struct PUSCH_ConfigDedicated_v1430 {
	struct PUSCH_ConfigDedicated_v1430_ce_PUSCH_NB_MaxTBS_r14_e_ce_PUSCH_NB_MaxTBS_r14_Optional ce_PUSCH_NB_MaxTBS_r14;
	struct PUSCH_ConfigDedicated_v1430_ce_PUSCH_MaxBandwidth_r14_e_ce_PUSCH_MaxBandwidth_r14_Optional ce_PUSCH_MaxBandwidth_r14;
	struct TDD_PUSCH_UpPTS_r14_PUSCH_ConfigDedicated_v1430_tdd_PUSCH_UpPTS_r14_Optional tdd_PUSCH_UpPTS_r14;
	bool ul_DMRS_IFDMA_r14;
	struct Enable256QAM_r14_PUSCH_ConfigDedicated_v1430_enable256QAM_r14_Optional enable256QAM_r14;
};

enum PhysicalConfigDedicated_ce_Mode_r13_setup_e {
	PhysicalConfigDedicated_ce_Mode_r13_setup_e_ce_ModeA = 0,
	PhysicalConfigDedicated_ce_Mode_r13_setup_e_ce_ModeB = 1,
};

typedef enum PhysicalConfigDedicated_ce_Mode_r13_setup_e PhysicalConfigDedicated_ce_Mode_r13_setup_e;

enum PhysicalConfigDedicated_ce_Mode_r13_Sel {
	PhysicalConfigDedicated_ce_Mode_r13_UNBOUND_VALUE = 0,
	PhysicalConfigDedicated_ce_Mode_r13_release = 1,
	PhysicalConfigDedicated_ce_Mode_r13_setup = 2,
};

union PhysicalConfigDedicated_ce_Mode_r13_Value {
	Null release;
	PhysicalConfigDedicated_ce_Mode_r13_setup_e setup;
};

struct PhysicalConfigDedicated_ce_Mode_r13 {
	enum PhysicalConfigDedicated_ce_Mode_r13_Sel d;
	union PhysicalConfigDedicated_ce_Mode_r13_Value v;
};

struct RACH_ConfigDedicated {
	uint8_t ra_PreambleIndex;
	uint8_t ra_PRACH_MaskIndex;
};

struct RLC_Config_v1250 {
	bool ul_extended_RLC_LI_Field_r12;
	bool dl_extended_RLC_LI_Field_r12;
};

typedef uint8_t SCellIndex_r10;

typedef uint8_t STAG_Id_r11;

enum SchedulingRequestConfig_setup_dsr_TransMax_e {
	SchedulingRequestConfig_setup_dsr_TransMax_e_n4 = 0,
	SchedulingRequestConfig_setup_dsr_TransMax_e_n8 = 1,
	SchedulingRequestConfig_setup_dsr_TransMax_e_n16 = 2,
	SchedulingRequestConfig_setup_dsr_TransMax_e_n32 = 3,
	SchedulingRequestConfig_setup_dsr_TransMax_e_n64 = 4,
	SchedulingRequestConfig_setup_dsr_TransMax_e_spare3 = 5,
	SchedulingRequestConfig_setup_dsr_TransMax_e_spare2 = 6,
	SchedulingRequestConfig_setup_dsr_TransMax_e_spare1 = 7,
};

typedef enum SchedulingRequestConfig_setup_dsr_TransMax_e SchedulingRequestConfig_setup_dsr_TransMax_e;

struct SchedulingRequestConfig_setup {
	uint16_t sr_PUCCH_ResourceIndex;
	uint8_t sr_ConfigIndex;
	SchedulingRequestConfig_setup_dsr_TransMax_e dsr_TransMax;
};

enum SchedulingRequestConfig_Sel {
	SchedulingRequestConfig_UNBOUND_VALUE = 0,
	SchedulingRequestConfig_release = 1,
	SchedulingRequestConfig_setup = 2,
};

union SchedulingRequestConfig_Value {
	Null release;
	struct SchedulingRequestConfig_setup setup;
};

struct SchedulingRequestConfig {
	enum SchedulingRequestConfig_Sel d;
	union SchedulingRequestConfig_Value v;
};

struct uint16_t_SchedulingRequestConfig_v1020_sr_PUCCH_ResourceIndexP1_r10_Optional {
	bool d;
	uint16_t v;
};

struct SchedulingRequestConfig_v1020 {
	struct uint16_t_SchedulingRequestConfig_v1020_sr_PUCCH_ResourceIndexP1_r10_Optional sr_PUCCH_ResourceIndexP1_r10;
};

enum SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e {
	SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e_bw0 = 0,
	SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e_bw1 = 1,
	SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e_bw2 = 2,
	SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e_bw3 = 3,
};

typedef enum SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e;

enum SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e {
	SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e_hbw0 = 0,
	SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e_hbw1 = 1,
	SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e_hbw2 = 2,
	SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e_hbw3 = 3,
};

typedef enum SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e;

enum SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e {
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs0 = 0,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs1 = 1,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs2 = 2,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs3 = 3,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs4 = 4,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs5 = 5,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs6 = 6,
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e_cs7 = 7,
};

typedef enum SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e;

struct SoundingRS_UL_ConfigDedicated_setup {
	SoundingRS_UL_ConfigDedicated_setup_srs_Bandwidth_e srs_Bandwidth;
	SoundingRS_UL_ConfigDedicated_setup_srs_HoppingBandwidth_e srs_HoppingBandwidth;
	uint8_t freqDomainPosition;
	bool duration;
	uint16_t srs_ConfigIndex;
	uint8_t transmissionComb;
	SoundingRS_UL_ConfigDedicated_setup_cyclicShift_e cyclicShift;
};

enum SoundingRS_UL_ConfigDedicated_Sel {
	SoundingRS_UL_ConfigDedicated_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigDedicated_release = 1,
	SoundingRS_UL_ConfigDedicated_setup = 2,
};

union SoundingRS_UL_ConfigDedicated_Value {
	Null release;
	struct SoundingRS_UL_ConfigDedicated_setup setup;
};

struct SoundingRS_UL_ConfigDedicated {
	enum SoundingRS_UL_ConfigDedicated_Sel d;
	union SoundingRS_UL_ConfigDedicated_Value v;
};

enum SRS_AntennaPort_e {
	SRS_AntennaPort_e_an1 = 0,
	SRS_AntennaPort_e_an2 = 1,
	SRS_AntennaPort_e_an4 = 2,
	SRS_AntennaPort_e_spare1 = 3,
};

typedef enum SRS_AntennaPort_e SRS_AntennaPort_e;

struct SoundingRS_UL_ConfigDedicated_v1020 {
	SRS_AntennaPort_e srs_AntennaPort_r10;
};

enum SRS_ConfigAp_r10_srs_BandwidthAp_r10_e {
	SRS_ConfigAp_r10_srs_BandwidthAp_r10_e_bw0 = 0,
	SRS_ConfigAp_r10_srs_BandwidthAp_r10_e_bw1 = 1,
	SRS_ConfigAp_r10_srs_BandwidthAp_r10_e_bw2 = 2,
	SRS_ConfigAp_r10_srs_BandwidthAp_r10_e_bw3 = 3,
};

typedef enum SRS_ConfigAp_r10_srs_BandwidthAp_r10_e SRS_ConfigAp_r10_srs_BandwidthAp_r10_e;

enum SRS_ConfigAp_r10_cyclicShiftAp_r10_e {
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs0 = 0,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs1 = 1,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs2 = 2,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs3 = 3,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs4 = 4,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs5 = 5,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs6 = 6,
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e_cs7 = 7,
};

typedef enum SRS_ConfigAp_r10_cyclicShiftAp_r10_e SRS_ConfigAp_r10_cyclicShiftAp_r10_e;

struct SRS_ConfigAp_r10 {
	SRS_AntennaPort_e srs_AntennaPortAp_r10;
	SRS_ConfigAp_r10_srs_BandwidthAp_r10_e srs_BandwidthAp_r10;
	uint8_t freqDomainPositionAp_r10;
	uint8_t transmissionCombAp_r10;
	SRS_ConfigAp_r10_cyclicShiftAp_r10_e cyclicShiftAp_r10;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_setup {
	struct SRS_ConfigAp_r10 srs_ConfigApDCI_Format0_r10;
	struct SRS_ConfigAp_r10 srs_ConfigApDCI_Format1a2b2c_r10;
};

enum SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_Sel {
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_release = 1,
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_setup = 2,
};

union SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_Value {
	Null release;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_setup setup;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10 {
	enum SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_Sel d;
	union SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_Value v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_SRS_ConfigAp_r10_srs_ConfigApDCI_Format4_r10_Dynamic {
	size_t d;
	struct SRS_ConfigAp_r10* v;
};

struct SRS_ConfigAp_r10_SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ConfigApDCI_Format4_r10_DynamicOptional {
	bool d;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_SRS_ConfigAp_r10_srs_ConfigApDCI_Format4_r10_Dynamic v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_srs_ActivateAp_r10_Optional {
	bool d;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10 v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup {
	uint8_t srs_ConfigIndexAp_r10;
	struct SRS_ConfigAp_r10_SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ConfigApDCI_Format4_r10_DynamicOptional srs_ConfigApDCI_Format4_r10;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup_srs_ActivateAp_r10_srs_ActivateAp_r10_Optional srs_ActivateAp_r10;
};

enum SoundingRS_UL_ConfigDedicatedAperiodic_r10_Sel {
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_release = 1,
	SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup = 2,
};

union SoundingRS_UL_ConfigDedicatedAperiodic_r10_Value {
	Null release;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_setup setup;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10 {
	enum SoundingRS_UL_ConfigDedicatedAperiodic_r10_Sel d;
	union SoundingRS_UL_ConfigDedicatedAperiodic_r10_Value v;
};

typedef uint8_t UE_EUTRA_Capability_v1020_IEs_ue_Category_v1020;

typedef uint8_t UE_EUTRA_Capability_v1170_IEs_ue_Category_v1170;

typedef uint8_t UE_EUTRA_Capability_v11a0_IEs_ue_Category_v11a0;

typedef uint8_t UE_EUTRA_Capability_v1250_IEs_ue_CategoryDL_r12;

typedef uint8_t UE_EUTRA_Capability_v1260_IEs_ue_CategoryDL_v1260;

enum UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e {
	UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e_n17 = 0,
	UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e_m1 = 1,
};

typedef enum UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e;

typedef uint8_t UE_EUTRA_Capability_v1330_IEs_ue_CategoryDL_v1330;

enum UE_EUTRA_Capability_v1350_IEs_ue_CategoryDL_v1350_e {
	UE_EUTRA_Capability_v1350_IEs_ue_CategoryDL_v1350_e_oneBis = 0,
};

typedef enum UE_EUTRA_Capability_v1350_IEs_ue_CategoryDL_v1350_e UE_EUTRA_Capability_v1350_IEs_ue_CategoryDL_v1350_e;

typedef uint8_t UE_EUTRA_Capability_v1450_IEs_ue_CategoryDL_v1450;

typedef uint8_t UE_EUTRA_Capability_v1460_IEs_ue_CategoryDL_v1460;

typedef uint8_t UE_EUTRA_Capability_ue_Category;

typedef uint8_t UE_RadioPagingInfo_r12_ue_Category_v1250;

enum T_PollRetransmit_e {
	T_PollRetransmit_e_ms5 = 0,
	T_PollRetransmit_e_ms10 = 1,
	T_PollRetransmit_e_ms15 = 2,
	T_PollRetransmit_e_ms20 = 3,
	T_PollRetransmit_e_ms25 = 4,
	T_PollRetransmit_e_ms30 = 5,
	T_PollRetransmit_e_ms35 = 6,
	T_PollRetransmit_e_ms40 = 7,
	T_PollRetransmit_e_ms45 = 8,
	T_PollRetransmit_e_ms50 = 9,
	T_PollRetransmit_e_ms55 = 10,
	T_PollRetransmit_e_ms60 = 11,
	T_PollRetransmit_e_ms65 = 12,
	T_PollRetransmit_e_ms70 = 13,
	T_PollRetransmit_e_ms75 = 14,
	T_PollRetransmit_e_ms80 = 15,
	T_PollRetransmit_e_ms85 = 16,
	T_PollRetransmit_e_ms90 = 17,
	T_PollRetransmit_e_ms95 = 18,
	T_PollRetransmit_e_ms100 = 19,
	T_PollRetransmit_e_ms105 = 20,
	T_PollRetransmit_e_ms110 = 21,
	T_PollRetransmit_e_ms115 = 22,
	T_PollRetransmit_e_ms120 = 23,
	T_PollRetransmit_e_ms125 = 24,
	T_PollRetransmit_e_ms130 = 25,
	T_PollRetransmit_e_ms135 = 26,
	T_PollRetransmit_e_ms140 = 27,
	T_PollRetransmit_e_ms145 = 28,
	T_PollRetransmit_e_ms150 = 29,
	T_PollRetransmit_e_ms155 = 30,
	T_PollRetransmit_e_ms160 = 31,
	T_PollRetransmit_e_ms165 = 32,
	T_PollRetransmit_e_ms170 = 33,
	T_PollRetransmit_e_ms175 = 34,
	T_PollRetransmit_e_ms180 = 35,
	T_PollRetransmit_e_ms185 = 36,
	T_PollRetransmit_e_ms190 = 37,
	T_PollRetransmit_e_ms195 = 38,
	T_PollRetransmit_e_ms200 = 39,
	T_PollRetransmit_e_ms205 = 40,
	T_PollRetransmit_e_ms210 = 41,
	T_PollRetransmit_e_ms215 = 42,
	T_PollRetransmit_e_ms220 = 43,
	T_PollRetransmit_e_ms225 = 44,
	T_PollRetransmit_e_ms230 = 45,
	T_PollRetransmit_e_ms235 = 46,
	T_PollRetransmit_e_ms240 = 47,
	T_PollRetransmit_e_ms245 = 48,
	T_PollRetransmit_e_ms250 = 49,
	T_PollRetransmit_e_ms300 = 50,
	T_PollRetransmit_e_ms350 = 51,
	T_PollRetransmit_e_ms400 = 52,
	T_PollRetransmit_e_ms450 = 53,
	T_PollRetransmit_e_ms500 = 54,
	T_PollRetransmit_e_ms800_v1310 = 55,
	T_PollRetransmit_e_ms1000_v1310 = 56,
	T_PollRetransmit_e_ms2000_v1310 = 57,
	T_PollRetransmit_e_ms4000_v1310 = 58,
	T_PollRetransmit_e_spare5 = 59,
	T_PollRetransmit_e_spare4 = 60,
	T_PollRetransmit_e_spare3 = 61,
	T_PollRetransmit_e_spare2 = 62,
	T_PollRetransmit_e_spare1 = 63,
};

typedef enum T_PollRetransmit_e T_PollRetransmit_e;

enum PollPDU_e {
	PollPDU_e_p4 = 0,
	PollPDU_e_p8 = 1,
	PollPDU_e_p16 = 2,
	PollPDU_e_p32 = 3,
	PollPDU_e_p64 = 4,
	PollPDU_e_p128 = 5,
	PollPDU_e_p256 = 6,
	PollPDU_e_pInfinity = 7,
};

typedef enum PollPDU_e PollPDU_e;

enum PollByte_e {
	PollByte_e_kB25 = 0,
	PollByte_e_kB50 = 1,
	PollByte_e_kB75 = 2,
	PollByte_e_kB100 = 3,
	PollByte_e_kB125 = 4,
	PollByte_e_kB250 = 5,
	PollByte_e_kB375 = 6,
	PollByte_e_kB500 = 7,
	PollByte_e_kB750 = 8,
	PollByte_e_kB1000 = 9,
	PollByte_e_kB1250 = 10,
	PollByte_e_kB1500 = 11,
	PollByte_e_kB2000 = 12,
	PollByte_e_kB3000 = 13,
	PollByte_e_kBinfinity = 14,
	PollByte_e_spare1 = 15,
};

typedef enum PollByte_e PollByte_e;

enum UL_AM_RLC_maxRetxThreshold_e {
	UL_AM_RLC_maxRetxThreshold_e_t1 = 0,
	UL_AM_RLC_maxRetxThreshold_e_t2 = 1,
	UL_AM_RLC_maxRetxThreshold_e_t3 = 2,
	UL_AM_RLC_maxRetxThreshold_e_t4 = 3,
	UL_AM_RLC_maxRetxThreshold_e_t6 = 4,
	UL_AM_RLC_maxRetxThreshold_e_t8 = 5,
	UL_AM_RLC_maxRetxThreshold_e_t16 = 6,
	UL_AM_RLC_maxRetxThreshold_e_t32 = 7,
};

typedef enum UL_AM_RLC_maxRetxThreshold_e UL_AM_RLC_maxRetxThreshold_e;

struct UL_AM_RLC {
	T_PollRetransmit_e t_PollRetransmit;
	PollPDU_e pollPDU;
	PollByte_e pollByte;
	UL_AM_RLC_maxRetxThreshold_e maxRetxThreshold;
};

struct UL_UM_RLC {
	SN_FieldLength_e sn_FieldLength;
};

enum UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e {
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF_1 = 0,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF0 = 1,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF1 = 2,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF2 = 3,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF3 = 4,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF4 = 5,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF5 = 6,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e_deltaF6 = 7,
};

typedef enum UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e;

enum UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e {
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e_deltaF1 = 0,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e_deltaF2 = 1,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e_spare2 = 2,
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e_spare1 = 3,
};

typedef enum UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e;

struct UplinkPowerControlCommonPSCell_r12 {
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format3_r12_e deltaF_PUCCH_Format3_r12;
	UplinkPowerControlCommonPSCell_r12_deltaF_PUCCH_Format1bCS_r12_e deltaF_PUCCH_Format1bCS_r12;
	int8_t p0_NominalPUCCH_r12;
	struct DeltaFList_PUCCH deltaFList_PUCCH_r12;
};

struct UplinkPowerControlCommonSCell_v1130 {
	int8_t deltaPreambleMsg3_r11;
};

enum UplinkPowerControlDedicated_deltaMCS_Enabled_e {
	UplinkPowerControlDedicated_deltaMCS_Enabled_e_en0 = 0,
	UplinkPowerControlDedicated_deltaMCS_Enabled_e_en1 = 1,
};

typedef enum UplinkPowerControlDedicated_deltaMCS_Enabled_e UplinkPowerControlDedicated_deltaMCS_Enabled_e;

struct FilterCoefficient_e_UplinkPowerControlDedicated_filterCoefficient_Optional {
	bool d;
	FilterCoefficient_e v;
};

struct UplinkPowerControlDedicated {
	int8_t p0_UE_PUSCH;
	UplinkPowerControlDedicated_deltaMCS_Enabled_e deltaMCS_Enabled;
	bool accumulationEnabled;
	int8_t p0_UE_PUCCH;
	uint8_t pSRS_Offset;
	struct FilterCoefficient_e_UplinkPowerControlDedicated_filterCoefficient_Optional filterCoefficient;
};

enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e {
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e_dB0 = 0,
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e_dB_2 = 1,
};

typedef enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e;

enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e {
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e_dB0 = 0,
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e_dB_2 = 1,
};

typedef enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e;

enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e {
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e_dB0 = 0,
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e_dB_2 = 1,
};

typedef enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e;

enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e {
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e_dB0 = 0,
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e_dB_2 = 1,
};

typedef enum DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e;

struct DeltaTxD_OffsetListPUCCH_r10 {
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1_r10_e deltaTxD_OffsetPUCCH_Format1_r10;
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format1a1b_r10_e deltaTxD_OffsetPUCCH_Format1a1b_r10;
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format22a2b_r10_e deltaTxD_OffsetPUCCH_Format22a2b_r10;
	DeltaTxD_OffsetListPUCCH_r10_deltaTxD_OffsetPUCCH_Format3_r10_e deltaTxD_OffsetPUCCH_Format3_r10;
};

struct DeltaTxD_OffsetListPUCCH_r10_UplinkPowerControlDedicated_v1020_deltaTxD_OffsetListPUCCH_r10_Optional {
	bool d;
	struct DeltaTxD_OffsetListPUCCH_r10 v;
};

struct uint8_t_UplinkPowerControlDedicated_v1020_pSRS_OffsetAp_r10_Optional {
	bool d;
	uint8_t v;
};

struct UplinkPowerControlDedicated_v1020 {
	struct DeltaTxD_OffsetListPUCCH_r10_UplinkPowerControlDedicated_v1020_deltaTxD_OffsetListPUCCH_r10_Optional deltaTxD_OffsetListPUCCH_r10;
	struct uint8_t_UplinkPowerControlDedicated_v1020_pSRS_OffsetAp_r10_Optional pSRS_OffsetAp_r10;
};

enum DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e {
	DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e_dB0 = 0,
	DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e_dB_1 = 1,
};

typedef enum DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e;

struct DeltaTxD_OffsetListPUCCH_v1130 {
	DeltaTxD_OffsetListPUCCH_v1130_deltaTxD_OffsetPUCCH_Format1bCS_r11_e deltaTxD_OffsetPUCCH_Format1bCS_r11;
};

struct uint8_t_UplinkPowerControlDedicated_v1130_pSRS_Offset_v1130_Optional {
	bool d;
	uint8_t v;
};

struct uint8_t_UplinkPowerControlDedicated_v1130_pSRS_OffsetAp_v1130_Optional {
	bool d;
	uint8_t v;
};

struct DeltaTxD_OffsetListPUCCH_v1130_UplinkPowerControlDedicated_v1130_deltaTxD_OffsetListPUCCH_v1130_Optional {
	bool d;
	struct DeltaTxD_OffsetListPUCCH_v1130 v;
};

struct UplinkPowerControlDedicated_v1130 {
	struct uint8_t_UplinkPowerControlDedicated_v1130_pSRS_Offset_v1130_Optional pSRS_Offset_v1130;
	struct uint8_t_UplinkPowerControlDedicated_v1130_pSRS_OffsetAp_v1130_Optional pSRS_OffsetAp_v1130;
	struct DeltaTxD_OffsetListPUCCH_v1130_UplinkPowerControlDedicated_v1130_deltaTxD_OffsetListPUCCH_v1130_Optional deltaTxD_OffsetListPUCCH_v1130;
};

struct UplinkPowerControlDedicated_v1250_set2PowerControlParameter_setup {
	B10 tpc_SubframeSet_r12;
	int8_t p0_NominalPUSCH_SubframeSet2_r12;
	Alpha_r12_e alpha_SubframeSet2_r12;
	int8_t p0_UE_PUSCH_SubframeSet2_r12;
};

enum UplinkPowerControlDedicated_v1250_set2PowerControlParameter_Sel {
	UplinkPowerControlDedicated_v1250_set2PowerControlParameter_UNBOUND_VALUE = 0,
	UplinkPowerControlDedicated_v1250_set2PowerControlParameter_release = 1,
	UplinkPowerControlDedicated_v1250_set2PowerControlParameter_setup = 2,
};

union UplinkPowerControlDedicated_v1250_set2PowerControlParameter_Value {
	Null release;
	struct UplinkPowerControlDedicated_v1250_set2PowerControlParameter_setup setup;
};

struct UplinkPowerControlDedicated_v1250_set2PowerControlParameter {
	enum UplinkPowerControlDedicated_v1250_set2PowerControlParameter_Sel d;
	union UplinkPowerControlDedicated_v1250_set2PowerControlParameter_Value v;
};

struct UplinkPowerControlDedicated_v1250 {
	struct UplinkPowerControlDedicated_v1250_set2PowerControlParameter set2PowerControlParameter;
};

enum UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e {
	UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e_pCell = 0,
	UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e_sCell = 1,
};

typedef enum UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e;

SIDL_END_C_INTERFACE
