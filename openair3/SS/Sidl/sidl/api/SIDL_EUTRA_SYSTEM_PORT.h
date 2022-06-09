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
#include "SidlParts.h"
#include "SidlVals.h"
#include "TtcnCommon.h"
#include "SidlCommon_BcchConfig.h"
#include "SidlCommon_Bcch_BRConfig.h"
#include "SidlCommon_CcchDcchDtchConfig.h"
#include "SidlCommon_InitialCellPower.h"
#include "SidlCommon_PhysicalLayerConfigDL.h"
#include "SidlCommon_PhysicalLayerConfigUL.h"
#include "SidlCommon_RachProcedureConfig.h"
#include "SidlCommon_SciSchedulingConfig.h"
#include "SidlCommon_ServingCellConfig.h"
#include "SidlCommon_StaticCellInfo.h"
#include "SidlCommon_CellConfigRequest.h"

SIDL_BEGIN_C_INTERFACE

typedef B128_Type B128_Key_Type;

typedef CipheringAlgorithm_r12_e CipheringAlgorithm_r12_Type;

struct SecurityActTime_Type_SecurityActTimeList_Type_Dynamic {
	size_t d;
	struct SecurityActTime_Type* v;
};

typedef struct SecurityActTime_Type_SecurityActTimeList_Type_Dynamic SecurityActTimeList_Type;

struct TimingInfo_Type_TimingInfo_Optional {
	bool d;
	struct TimingInfo_Type v;
};

struct CellAttenuationConfig_Type {
	EUTRA_CellId_Type CellId;
	struct Attenuation_Type Attenuation;
	struct TimingInfo_Type_TimingInfo_Optional TimingInfo;
};

enum PdcpCountGetReq_Type_Sel {
	PdcpCountGetReq_Type_UNBOUND_VALUE = 0,
	PdcpCountGetReq_Type_AllRBs = 1,
	PdcpCountGetReq_Type_SingleRB = 2,
};

union PdcpCountGetReq_Type_Value {
	Null_Type AllRBs;
	struct RadioBearerId_Type SingleRB;
};

struct PdcpCountGetReq_Type {
	enum PdcpCountGetReq_Type_Sel d;
	union PdcpCountGetReq_Type_Value v;
};

enum Pdcp_CountReq_Type_Sel {
	Pdcp_CountReq_Type_UNBOUND_VALUE = 0,
	Pdcp_CountReq_Type_Get = 1,
	Pdcp_CountReq_Type_Set = 2,
};

struct PdcpCountInfo_Type_Set_Dynamic {
	size_t d;
	struct PdcpCountInfo_Type* v;
};

union Pdcp_CountReq_Type_Value {
	struct PdcpCountGetReq_Type Get;
	struct PdcpCountInfo_Type_Set_Dynamic Set;
};

struct Pdcp_CountReq_Type {
	enum Pdcp_CountReq_Type_Sel d;
	union Pdcp_CountReq_Type_Value v;
};

enum PDCP_SNLength_Type {
	PDCP_SNLength5 = 0,
	PDCP_SNLength7 = 1,
	PDCP_SNLength12 = 2,
	PDCP_SNLength16 = 3,
};

typedef enum PDCP_SNLength_Type PDCP_SNLength_Type;

struct PDCP_ROHC_Mode_Type {
	PDCP_SNLength_Type SN_Size;
};

struct PDCP_NonROHC_Mode_Type {
	PDCP_SNLength_Type SN_Size;
};

enum PDCP_TestModeInfo_Type_Sel {
	PDCP_TestModeInfo_Type_UNBOUND_VALUE = 0,
	PDCP_TestModeInfo_Type_PDCP_ROHC_Mode = 1,
	PDCP_TestModeInfo_Type_PDCP_NonROHC_Mode = 2,
};

union PDCP_TestModeInfo_Type_Value {
	struct PDCP_ROHC_Mode_Type PDCP_ROHC_Mode;
	struct PDCP_NonROHC_Mode_Type PDCP_NonROHC_Mode;
};

struct PDCP_TestModeInfo_Type {
	enum PDCP_TestModeInfo_Type_Sel d;
	union PDCP_TestModeInfo_Type_Value v;
};

enum PDCP_TestModeConfig_Type_Sel {
	PDCP_TestModeConfig_Type_UNBOUND_VALUE = 0,
	PDCP_TestModeConfig_Type_None = 1,
	PDCP_TestModeConfig_Type_Info = 2,
};

union PDCP_TestModeConfig_Type_Value {
	Null_Type None;
	struct PDCP_TestModeInfo_Type Info;
};

struct PDCP_TestModeConfig_Type {
	enum PDCP_TestModeConfig_Type_Sel d;
	union PDCP_TestModeConfig_Type_Value v;
};

enum PDCP_Config_Type_Sel {
	PDCP_Config_UNBOUND_VALUE = 0,
	PDCP_Config_Type_R8 = 1,
};

union PDCP_Config_Type_Value {
	struct PDCP_Config R8;
};

struct PDCP_Config_Type {
	enum PDCP_Config_Type_Sel d;
	union PDCP_Config_Type_Value v;
};

enum PDCP_RBConfig_Type_Sel {
	PDCP_RBConfig_Type_UNBOUND_VALUE = 0,
	PDCP_RBConfig_Type_Srb = 1,
	PDCP_RBConfig_Type_Drb = 2,
	PDCP_RBConfig_Type_Transparent = 3,
};

union PDCP_RBConfig_Type_Value {
	Null_Type Srb;
	struct PDCP_Config_Type Drb;
	Null_Type Transparent;
};

struct PDCP_RBConfig_Type {
	enum PDCP_RBConfig_Type_Sel d;
	union PDCP_RBConfig_Type_Value v;
};

struct PDCP_RBConfig_Type_Rb_Optional {
	bool d;
	struct PDCP_RBConfig_Type v;
};

struct PDCP_TestModeConfig_Type_TestMode_Optional {
	bool d;
	struct PDCP_TestModeConfig_Type v;
};

struct PDCP_ConfigInfo_Type {
	struct PDCP_RBConfig_Type_Rb_Optional Rb;
	struct PDCP_TestModeConfig_Type_TestMode_Optional TestMode;
};

enum PDCP_Configuration_Type_Sel {
	PDCP_Configuration_Type_UNBOUND_VALUE = 0,
	PDCP_Configuration_Type_None = 1,
	PDCP_Configuration_Type_Config = 2,
};

union PDCP_Configuration_Type_Value {
	Null_Type None;
	struct PDCP_ConfigInfo_Type Config;
};

struct PDCP_Configuration_Type {
	enum PDCP_Configuration_Type_Sel d;
	union PDCP_Configuration_Type_Value v;
};

enum UL_AM_RLC_Type_Sel {
	UL_AM_RLC_Type_UNBOUND_VALUE = 0,
	UL_AM_RLC_Type_R8 = 1,
};

union UL_AM_RLC_Type_Value {
	struct UL_AM_RLC R8;
};

struct UL_AM_RLC_Type {
	enum UL_AM_RLC_Type_Sel d;
	union UL_AM_RLC_Type_Value v;
};

enum DL_AM_RLC_Type_Sel {
	DL_AM_RLC_Type_UNBOUND_VALUE = 0,
	DL_AM_RLC_Type_R8 = 1,
};

union DL_AM_RLC_Type_Value {
	struct DL_AM_RLC R8;
};

struct DL_AM_RLC_Type {
	enum DL_AM_RLC_Type_Sel d;
	union DL_AM_RLC_Type_Value v;
};

enum UL_UM_RLC_Type_Sel {
	UL_UM_RLC_Type_UNBOUND_VALUE = 0,
	UL_UM_RLC_Type_R8 = 1,
};

union UL_UM_RLC_Type_Value {
	struct UL_UM_RLC R8;
};

struct UL_UM_RLC_Type {
	enum UL_UM_RLC_Type_Sel d;
	union UL_UM_RLC_Type_Value v;
};

enum DL_UM_RLC_Type_Sel {
	DL_UM_RLC_Type_UNBOUND_VALUE = 0,
	DL_UM_RLC_Type_R8 = 1,
};

union DL_UM_RLC_Type_Value {
	struct DL_UM_RLC R8;
};

struct DL_UM_RLC_Type {
	enum DL_UM_RLC_Type_Sel d;
	union DL_UM_RLC_Type_Value v;
};

struct UL_AM_RLC_Type_Tx_Optional {
	bool d;
	struct UL_AM_RLC_Type v;
};

struct DL_AM_RLC_Type_Rx_Optional {
	bool d;
	struct DL_AM_RLC_Type v;
};

struct RLC_Config_v1250_ExtendedLI_Optional {
	bool d;
	struct RLC_Config_v1250 v;
};

struct SS_RLC_AM_Type {
	struct UL_AM_RLC_Type_Tx_Optional Tx;
	struct DL_AM_RLC_Type_Rx_Optional Rx;
	struct RLC_Config_v1250_ExtendedLI_Optional ExtendedLI;
};

struct UL_UM_RLC_Type_SS_RLC_UM_Bi_Directional_Type_Tx_Optional {
	bool d;
	struct UL_UM_RLC_Type v;
};

struct DL_UM_RLC_Type_SS_RLC_UM_Bi_Directional_Type_Rx_Optional {
	bool d;
	struct DL_UM_RLC_Type v;
};

struct SS_RLC_UM_Bi_Directional_Type {
	struct UL_UM_RLC_Type_SS_RLC_UM_Bi_Directional_Type_Tx_Optional Tx;
	struct DL_UM_RLC_Type_SS_RLC_UM_Bi_Directional_Type_Rx_Optional Rx;
};

struct DL_UM_RLC_Type_SS_RLC_UM_Uni_Directional_UL_Type_Rx_Optional {
	bool d;
	struct DL_UM_RLC_Type v;
};

struct SS_RLC_UM_Uni_Directional_UL_Type {
	struct DL_UM_RLC_Type_SS_RLC_UM_Uni_Directional_UL_Type_Rx_Optional Rx;
};

struct UL_UM_RLC_Type_SS_RLC_UM_Uni_Directional_DL_Type_Tx_Optional {
	bool d;
	struct UL_UM_RLC_Type v;
};

struct SS_RLC_UM_Uni_Directional_DL_Type {
	struct UL_UM_RLC_Type_SS_RLC_UM_Uni_Directional_DL_Type_Tx_Optional Tx;
};

enum RLC_RbConfig_Type_Sel {
	RLC_RbConfig_Type_UNBOUND_VALUE = 0,
	RLC_RbConfig_Type_AM = 1,
	RLC_RbConfig_Type_UM = 2,
	RLC_RbConfig_Type_UM_OnlyUL = 3,
	RLC_RbConfig_Type_UM_OnlyDL = 4,
	RLC_RbConfig_Type_TM = 5,
};

union RLC_RbConfig_Type_Value {
	struct SS_RLC_AM_Type AM;
	struct SS_RLC_UM_Bi_Directional_Type UM;
	struct SS_RLC_UM_Uni_Directional_UL_Type UM_OnlyUL;
	struct SS_RLC_UM_Uni_Directional_DL_Type UM_OnlyDL;
	Null_Type TM;
};

struct RLC_RbConfig_Type {
	enum RLC_RbConfig_Type_Sel d;
	union RLC_RbConfig_Type_Value v;
};

enum RLC_ACK_Prohibit_Type {
	RLC_ACK_Prohibit_Prohibit = 0,
	RLC_ACK_Prohibit_Continue = 1,
};

typedef enum RLC_ACK_Prohibit_Type RLC_ACK_Prohibit_Type;

enum RLC_NotACK_NextRLC_PDU_Type {
	RLC_NotACK_NextRLC_PDU_Start = 0,
};

typedef enum RLC_NotACK_NextRLC_PDU_Type RLC_NotACK_NextRLC_PDU_Type;

typedef uint16_t RLC_AM_SequenceNumber_Type;

enum RLC_TestModeInfo_Type_Sel {
	RLC_TestModeInfo_Type_UNBOUND_VALUE = 0,
	RLC_TestModeInfo_Type_AckProhibit = 1,
	RLC_TestModeInfo_Type_NotACK_NextRLC_PDU = 2,
	RLC_TestModeInfo_Type_ModifyVTS = 3,
	RLC_TestModeInfo_Type_TransparentMode_UMDwith5BitSN = 4,
	RLC_TestModeInfo_Type_TransparentMode_UMDwith10BitSN = 5,
	RLC_TestModeInfo_Type_TransparentMode_AMD = 6,
};

union RLC_TestModeInfo_Type_Value {
	RLC_ACK_Prohibit_Type AckProhibit;
	RLC_NotACK_NextRLC_PDU_Type NotACK_NextRLC_PDU;
	RLC_AM_SequenceNumber_Type ModifyVTS;
	Null_Type TransparentMode_UMDwith5BitSN;
	Null_Type TransparentMode_UMDwith10BitSN;
	Null_Type TransparentMode_AMD;
};

struct RLC_TestModeInfo_Type {
	enum RLC_TestModeInfo_Type_Sel d;
	union RLC_TestModeInfo_Type_Value v;
};

enum RLC_TestModeConfig_Type_Sel {
	RLC_TestModeConfig_Type_UNBOUND_VALUE = 0,
	RLC_TestModeConfig_Type_None = 1,
	RLC_TestModeConfig_Type_Info = 2,
};

union RLC_TestModeConfig_Type_Value {
	Null_Type None;
	struct RLC_TestModeInfo_Type Info;
};

struct RLC_TestModeConfig_Type {
	enum RLC_TestModeConfig_Type_Sel d;
	union RLC_TestModeConfig_Type_Value v;
};

struct RLC_RbConfig_Type_Rb_Optional {
	bool d;
	struct RLC_RbConfig_Type v;
};

struct RLC_TestModeConfig_Type_TestMode_Optional {
	bool d;
	struct RLC_TestModeConfig_Type v;
};

struct RLC_Configuration_Type {
	struct RLC_RbConfig_Type_Rb_Optional Rb;
	struct RLC_TestModeConfig_Type_TestMode_Optional TestMode;
};

typedef int32_t TestLogicalChannelId_Type;

struct MAC_Test_DLLogChID_Type {
	TestLogicalChannelId_Type LogChId;
	Null_Type ConfigLchId;
};

enum MAC_Test_SCH_NoHeaderManipulation_Type {
	MAC_Test_SCH_NoHeaderManipulation_NormalMode = 0,
	MAC_Test_SCH_NoHeaderManipulation_DL_SCH_Only = 1,
	MAC_Test_SCH_NoHeaderManipulation_DL_UL_SCH = 2,
};

typedef enum MAC_Test_SCH_NoHeaderManipulation_Type MAC_Test_SCH_NoHeaderManipulation_Type;

struct MAC_TestModeInfo_Type {
	struct MAC_Test_DLLogChID_Type DiffLogChId;
	MAC_Test_SCH_NoHeaderManipulation_Type No_HeaderManipulation;
};

enum MAC_TestModeConfig_Type_Sel {
	MAC_TestModeConfig_Type_UNBOUND_VALUE = 0,
	MAC_TestModeConfig_Type_None = 1,
	MAC_TestModeConfig_Type_Info = 2,
};

union MAC_TestModeConfig_Type_Value {
	Null_Type None;
	struct MAC_TestModeInfo_Type Info;
};

struct MAC_TestModeConfig_Type {
	enum MAC_TestModeConfig_Type_Sel d;
	union MAC_TestModeConfig_Type_Value v;
};

typedef LogicalChannelConfig_ul_SpecificParameters_prioritisedBitRate_e PrioritizedBitRate_Type;

struct Null_Type_LAA_UL_Allowed_Optional {
	bool d;
	Null_Type v;
};

struct MAC_LogicalChannelConfig_Type {
	int32_t Priority;
	PrioritizedBitRate_Type PrioritizedBitRate;
	struct Null_Type_LAA_UL_Allowed_Optional LAA_UL_Allowed;
};

struct MAC_LogicalChannelConfig_Type_LogicalChannel_Optional {
	bool d;
	struct MAC_LogicalChannelConfig_Type v;
};

struct MAC_TestModeConfig_Type_TestMode_Optional {
	bool d;
	struct MAC_TestModeConfig_Type v;
};

struct MAC_Configuration_Type {
	struct MAC_LogicalChannelConfig_Type_LogicalChannel_Optional LogicalChannel;
	struct MAC_TestModeConfig_Type_TestMode_Optional TestMode;
};

typedef uint8_t LogicalChannelId_Type;

struct PDCP_Configuration_Type_Pdcp_Optional {
	bool d;
	struct PDCP_Configuration_Type v;
};

struct RLC_Configuration_Type_Rlc_Optional {
	bool d;
	struct RLC_Configuration_Type v;
};

struct LogicalChannelId_Type_LogicalChannelId_Optional {
	bool d;
	LogicalChannelId_Type v;
};

struct MAC_Configuration_Type_Mac_Optional {
	bool d;
	struct MAC_Configuration_Type v;
};

struct bool_DiscardULData_Optional {
	bool d;
	bool v;
};

struct RadioBearerConfigInfo_Type {
	struct PDCP_Configuration_Type_Pdcp_Optional Pdcp;
	struct RLC_Configuration_Type_Rlc_Optional Rlc;
	struct LogicalChannelId_Type_LogicalChannelId_Optional LogicalChannelId;
	struct MAC_Configuration_Type_Mac_Optional Mac;
	struct bool_DiscardULData_Optional DiscardULData;
};

enum RadioBearerConfig_Type_Sel {
	RadioBearerConfig_Type_UNBOUND_VALUE = 0,
	RadioBearerConfig_Type_AddOrReconfigure = 1,
	RadioBearerConfig_Type_Release = 2,
};

union RadioBearerConfig_Type_Value {
	struct RadioBearerConfigInfo_Type AddOrReconfigure;
	Null_Type Release;
};

struct RadioBearerConfig_Type {
	enum RadioBearerConfig_Type_Sel d;
	union RadioBearerConfig_Type_Value v;
};

struct RadioBearer_Type {
	struct RadioBearerId_Type Id;
	struct RadioBearerConfig_Type Config;
};

struct PdcpSQN_Type {
	PdcpCountFormat_Type Format;
	int32_t Value;
};

enum PDCP_ActTime_Type_Sel {
	PDCP_ActTime_Type_UNBOUND_VALUE = 0,
	PDCP_ActTime_Type_None = 1,
	PDCP_ActTime_Type_SQN = 2,
};

union PDCP_ActTime_Type_Value {
	Null_Type None;
	struct PdcpSQN_Type SQN;
};

struct PDCP_ActTime_Type {
	enum PDCP_ActTime_Type_Sel d;
	union PDCP_ActTime_Type_Value v;
};

struct SecurityActTime_Type {
	struct RadioBearerId_Type RadioBearerId;
	struct PDCP_ActTime_Type UL;
	struct PDCP_ActTime_Type DL;
};

struct SecurityActTimeList_Type_ActTimeList_Optional {
	bool d;
	SecurityActTimeList_Type v;
};

struct AS_IntegrityInfo_Type {
	CipheringAlgorithm_r12_Type Algorithm;
	B128_Key_Type KRRCint;
	struct SecurityActTimeList_Type_ActTimeList_Optional ActTimeList;
};

struct AS_CipheringInfo_Type {
	CipheringAlgorithm_r12_Type Algorithm;
	B128_Key_Type KRRCenc;
	B128_Key_Type KUPenc;
	SecurityActTimeList_Type ActTimeList;
};

struct AS_IntegrityInfo_Type_Integrity_Optional {
	bool d;
	struct AS_IntegrityInfo_Type v;
};

struct AS_CipheringInfo_Type_Ciphering_Optional {
	bool d;
	struct AS_CipheringInfo_Type v;
};

struct AS_SecStartRestart_Type {
	struct AS_IntegrityInfo_Type_Integrity_Optional Integrity;
	struct AS_CipheringInfo_Type_Ciphering_Optional Ciphering;
};

enum AS_Security_Type_Sel {
	AS_Security_Type_UNBOUND_VALUE = 0,
	AS_Security_Type_StartRestart = 1,
	AS_Security_Type_Release = 2,
};

union AS_Security_Type_Value {
	struct AS_SecStartRestart_Type StartRestart;
	Null_Type Release;
};

struct AS_Security_Type {
	enum AS_Security_Type_Sel d;
	union AS_Security_Type_Value v;
};

struct UE_Category_v1020_Type_ue_Category_V1020_Optional {
	bool d;
	UE_Category_v1020_Type v;
};

struct UE_Category_v1170_Type_ue_Category_v1170_Optional {
	bool d;
	UE_Category_v1170_Type v;
};

struct UE_Category_v11a0_Type_ue_Category_v11a0_Optional {
	bool d;
	UE_Category_v11a0_Type v;
};

struct UE_Category_v1250_Type_ue_Category_v1250_Optional {
	bool d;
	UE_Category_v1250_Type v;
};

struct UE_CategoryDL_r12_Type_ue_CategoryDL_r12_Optional {
	bool d;
	UE_CategoryDL_r12_Type v;
};

struct UE_CategoryDL_v1260_Type_ue_CategoryDL_v1260_Optional {
	bool d;
	UE_CategoryDL_v1260_Type v;
};

struct UE_CategoryDL_v1310_Type_ue_CategoryDL_v1310_Optional {
	bool d;
	UE_CategoryDL_v1310_Type v;
};

struct UE_CategoryDL_v1330_Type_ue_CategoryDL_v1330_Optional {
	bool d;
	UE_CategoryDL_v1330_Type v;
};

struct UE_CategoryDL_v1350_Type_ue_CategoryDL_v1350_Optional {
	bool d;
	UE_CategoryDL_v1350_Type v;
};

struct UE_CategoryDL_v1460_Type_ue_CategoryDL_v1460_Optional {
	bool d;
	UE_CategoryDL_v1460_Type v;
};

struct UE_CategoryInfo_Type {
	UE_Category_Type ue_Category;
	struct UE_Category_v1020_Type_ue_Category_V1020_Optional ue_Category_V1020;
	struct UE_Category_v1170_Type_ue_Category_v1170_Optional ue_Category_v1170;
	struct UE_Category_v11a0_Type_ue_Category_v11a0_Optional ue_Category_v11a0;
	struct UE_Category_v1250_Type_ue_Category_v1250_Optional ue_Category_v1250;
	struct UE_CategoryDL_r12_Type_ue_CategoryDL_r12_Optional ue_CategoryDL_r12;
	struct UE_CategoryDL_v1260_Type_ue_CategoryDL_v1260_Optional ue_CategoryDL_v1260;
	struct UE_CategoryDL_v1310_Type_ue_CategoryDL_v1310_Optional ue_CategoryDL_v1310;
	struct UE_CategoryDL_v1330_Type_ue_CategoryDL_v1330_Optional ue_CategoryDL_v1330;
	struct UE_CategoryDL_v1350_Type_ue_CategoryDL_v1350_Optional ue_CategoryDL_v1350;
	struct UE_CategoryDL_v1460_Type_ue_CategoryDL_v1460_Optional ue_CategoryDL_v1460;
};

struct int32_t_SubframeOffsetList_Type_Dynamic {
	size_t d;
	int32_t* v;
};

typedef struct int32_t_SubframeOffsetList_Type_Dynamic SubframeOffsetList_Type;

struct SubframeOffsetList_Type_SubframeOffsetList_Optional {
	bool d;
	SubframeOffsetList_Type v;
};

struct PagingTrigger_Type {
	struct PCCH_Message Paging;
	struct SubframeOffsetList_Type_SubframeOffsetList_Optional SubframeOffsetList;
};

enum SystemRequest_Type_Sel {
	SystemRequest_Type_UNBOUND_VALUE = 0,
	SystemRequest_Type_Cell = 1,
	SystemRequest_Type_CellAttenuationList = 2,
	SystemRequest_Type_RadioBearerList = 3,
	SystemRequest_Type_EnquireTiming = 4,
	SystemRequest_Type_AS_Security = 5,
	SystemRequest_Type_Paging = 7,
	SystemRequest_Type_PdcpCount = 10,
	SystemRequest_Type_UE_Cat_Info = 18,
};

struct CellAttenuationConfig_Type_CellAttenuationList_Dynamic {
	size_t d;
	struct CellAttenuationConfig_Type* v;
};

struct RadioBearer_Type_RadioBearerList_Dynamic {
	size_t d;
	struct RadioBearer_Type* v;
};

union SystemRequest_Type_Value {
	struct CellConfigRequest_Type Cell;
	struct CellAttenuationConfig_Type_CellAttenuationList_Dynamic CellAttenuationList;
	struct RadioBearer_Type_RadioBearerList_Dynamic RadioBearerList;
	Null_Type EnquireTiming;
	struct AS_Security_Type AS_Security;
	struct PagingTrigger_Type Paging;
	struct Pdcp_CountReq_Type PdcpCount;
	struct UE_CategoryInfo_Type UE_Cat_Info;
};

struct SystemRequest_Type {
	enum SystemRequest_Type_Sel d;
	union SystemRequest_Type_Value v;
};

enum SystemConfirm_Type_Sel {
	SystemConfirm_Type_UNBOUND_VALUE = 0,
	SystemConfirm_Type_Cell = 1,
	SystemConfirm_Type_CellAttenuationList = 2,
	SystemConfirm_Type_RadioBearerList = 3,
	SystemConfirm_Type_EnquireTiming = 4,
	SystemConfirm_Type_AS_Security = 5,
	SystemConfirm_Type_Sps = 6,
	SystemConfirm_Type_Paging = 7,
	SystemConfirm_Type_L1MacIndCtrl = 8,
	SystemConfirm_Type_RlcIndCtrl = 9,
	SystemConfirm_Type_PdcpCount = 10,
	SystemConfirm_Type_PdcpHandoverControl = 11,
	SystemConfirm_Type_L1_TestMode = 12,
	SystemConfirm_Type_PdcchOrder = 13,
	SystemConfirm_Type_ActivateScell = 14,
	SystemConfirm_Type_MbmsConfig = 15,
	SystemConfirm_Type_PDCCH_MCCH_ChangeNotification = 16,
	SystemConfirm_Type_MSI_Config = 17,
	SystemConfirm_Type_UE_Cat_Info = 18,
	SystemConfirm_Type_OCNG_Config = 19,
	SystemConfirm_Type_DirectIndicationInfo = 20,
};

union SystemConfirm_Type_Value {
	Null_Type Cell;
	Null_Type CellAttenuationList;
	Null_Type RadioBearerList;
	Null_Type EnquireTiming;
	Null_Type AS_Security;
	Null_Type Sps;
	Null_Type Paging;
	Null_Type L1MacIndCtrl;
	Null_Type RlcIndCtrl;
	struct Pdcp_CountCnf_Type PdcpCount;
	Null_Type PdcpHandoverControl;
	Null_Type L1_TestMode;
	Null_Type PdcchOrder;
	Null_Type ActivateScell;
	Null_Type MbmsConfig;
	Null_Type PDCCH_MCCH_ChangeNotification;
	Null_Type MSI_Config;
	Null_Type UE_Cat_Info;
	Null_Type OCNG_Config;
	Null_Type DirectIndicationInfo;
};

struct SystemConfirm_Type {
	enum SystemConfirm_Type_Sel d;
	union SystemConfirm_Type_Value v;
};

struct SYSTEM_CTRL_REQ {
	struct ReqAspCommonPart_Type Common;
	struct SystemRequest_Type Request;
};

struct SYSTEM_CTRL_CNF {
	struct CnfAspCommonPart_Type Common;
	struct SystemConfirm_Type Confirm;
};

SIDL_END_C_INTERFACE
