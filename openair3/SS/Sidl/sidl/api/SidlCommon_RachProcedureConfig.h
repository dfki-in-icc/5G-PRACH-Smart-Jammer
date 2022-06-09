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

typedef BIT_STRING ContentionResolutionId_Type;

struct RACH_ConfigCommon_v1250_rach_ConfigCommon_v1250_Optional {
	bool d;
	struct RACH_ConfigCommon_v1250 v;
};

struct RACH_ConfigCommon_R8andLater_Type {
	struct RACH_ConfigCommon rach_ConfigCommon;
	struct RACH_ConfigCommon_v1250_rach_ConfigCommon_v1250_Optional rach_ConfigCommon_v1250;
};

enum RACH_ConfigCommon_Type_Sel {
	RACH_ConfigCommon_Type_UNBOUND_VALUE = 0,
	RACH_ConfigCommon_Type_R8andLater = 1,
};

union RACH_ConfigCommon_Type_Value {
	struct RACH_ConfigCommon_R8andLater_Type R8andLater;
};

struct RACH_ConfigCommon_Type {
	enum RACH_ConfigCommon_Type_Sel d;
	union RACH_ConfigCommon_Type_Value v;
};

enum RACH_ConfigDedicated_Type_Sel {
	RACH_ConfigDedicated_Type_UNBOUND_VALUE = 0,
	RACH_ConfigDedicated_Type_R8 = 1,
};

union RACH_ConfigDedicated_Type_Value {
	struct RACH_ConfigDedicated R8;
};

struct RACH_ConfigDedicated_Type {
	enum RACH_ConfigDedicated_Type_Sel d;
	union RACH_ConfigDedicated_Type_Value v;
};

enum RAR_RapIdCtrl_Type_Sel {
	RAR_RapIdCtrl_Type_UNBOUND_VALUE = 0,
	RAR_RapIdCtrl_Type_Automatic = 1,
	RAR_RapIdCtrl_Type_Unmatched = 2,
};

union RAR_RapIdCtrl_Type_Value {
	Null_Type Automatic;
	Null_Type Unmatched;
};

struct RAR_RapIdCtrl_Type {
	enum RAR_RapIdCtrl_Type_Sel d;
	union RAR_RapIdCtrl_Type_Value v;
};

struct UplinkGrant_Type {
	B1_Type HoppingFlag;
	B10_Type RB_Allocation;
	B4_Type ModAndCodScheme;
	B3_Type TPC_Command;
	B1_Type UL_Delay;
	B1_Type CQI_Req;
};

enum TempC_RNTI_Type_Sel {
	TempC_RNTI_Type_UNBOUND_VALUE = 0,
	TempC_RNTI_Type_SameAsC_RNTI = 1,
	TempC_RNTI_Type_Explicit = 2,
};

union TempC_RNTI_Type_Value {
	Null_Type SameAsC_RNTI;
	C_RNTI Explicit;
};

struct TempC_RNTI_Type {
	enum TempC_RNTI_Type_Sel d;
	union TempC_RNTI_Type_Value v;
};

struct UplinkGrantCE_ModeA_Type {
	B4_Type Msg3_PUSCH_NB_Index;
	B4_Type Msg3_PUSCH_ResAllocation;
	B2_Type Msg3_PUSCH_Repetitions;
	B3_Type MCS;
	B3_Type TPC_Command;
	B1_Type CSI_Req;
	B1_Type UL_Delay;
	B2_Type Msg3_4_MPDCCH_NB_Index;
};

struct UplinkGrantCE_ModeB_Type {
	B2_Type Msg3_PUSCH_NB_Index;
	B3_Type Msg3_PUSCH_ResAllocation;
	B3_Type Msg3_PUSCH_Repetitions;
	B2_Type TBS;
	B2_Type Msg3_4_MPDCCH_NB_Index;
};

struct UplinkGrantCE_ModeB_Type_ModeB {
	B2_Type Msg3_PUSCH_NB_Index;
	B3_Type Msg3_PUSCH_ResAllocation;
	B3_Type Msg3_PUSCH_Repetitions;
	B2_Type TBS;
	B2_Type Msg3_4_MPDCCH_NB_Index;
};

struct UplinkGrantCE_Mode_Type {
	struct UplinkGrantCE_ModeA_Type CE_ModeA;
	struct UplinkGrantCE_ModeB_Type CE_ModeB;
};

struct UplinkGrantCE_Mode_Type_InitialGrantCE_Mode_Optional {
	bool d;
	struct UplinkGrantCE_Mode_Type v;
};

struct RandomAccessResponseParameters_Type {
	struct RAR_RapIdCtrl_Type RapId;
	struct UplinkGrant_Type InitialGrant;
	RACH_TimingAdvance_Type TimingAdvance;
	struct TempC_RNTI_Type TempC_RNTI;
	struct UplinkGrantCE_Mode_Type_InitialGrantCE_Mode_Optional InitialGrantCE_Mode;
};

struct RandomAccessResponseParameters_Type_RarList_Type_Dynamic {
	size_t d;
	struct RandomAccessResponseParameters_Type* v;
};

typedef struct RandomAccessResponseParameters_Type_RarList_Type_Dynamic RarList_Type;

enum RandomAccessResponse_Type_Sel {
	RandomAccessResponse_Type_UNBOUND_VALUE = 0,
	RandomAccessResponse_Type_None = 1,
	RandomAccessResponse_Type_List = 2,
};

union RandomAccessResponse_Type_Value {
	Null_Type None;
	RarList_Type List;
};

struct RandomAccessResponse_Type {
	enum RandomAccessResponse_Type_Sel d;
	union RandomAccessResponse_Type_Value v;
};

enum RandomAccessBackoffIndicator_Type_Sel {
	RandomAccessBackoffIndicator_Type_UNBOUND_VALUE = 0,
	RandomAccessBackoffIndicator_Type_None = 1,
	RandomAccessBackoffIndicator_Type_Index = 2,
};

union RandomAccessBackoffIndicator_Type_Value {
	Null_Type None;
	uint8_t Index;
};

struct RandomAccessBackoffIndicator_Type {
	enum RandomAccessBackoffIndicator_Type_Sel d;
	union RandomAccessBackoffIndicator_Type_Value v;
};

struct RandomAccessResponseCtrl_Type {
	struct DciDlInfoCommon_Type DciInfo;
	struct RandomAccessResponse_Type Rar;
	struct RandomAccessBackoffIndicator_Type BackoffInd;
};

enum RandomAccessResponseConfig_Type_Sel {
	RandomAccessResponseConfig_Type_UNBOUND_VALUE = 0,
	RandomAccessResponseConfig_Type_Ctrl = 1,
	RandomAccessResponseConfig_Type_Ctrl_CRC_Error = 2,
	RandomAccessResponseConfig_Type_None = 3,
};

union RandomAccessResponseConfig_Type_Value {
	struct RandomAccessResponseCtrl_Type Ctrl;
	struct RandomAccessResponseCtrl_Type Ctrl_CRC_Error;
	Null_Type None;
};

struct RandomAccessResponseConfig_Type {
	enum RandomAccessResponseConfig_Type_Sel d;
	union RandomAccessResponseConfig_Type_Value v;
};

enum ContentionResolution_ContainedId_Type_Sel {
	ContentionResolution_ContainedId_Type_UNBOUND_VALUE = 0,
	ContentionResolution_ContainedId_Type_XorMask = 1,
	ContentionResolution_ContainedId_Type_None = 2,
};

union ContentionResolution_ContainedId_Type_Value {
	ContentionResolutionId_Type XorMask;
	Null_Type None;
};

struct ContentionResolution_ContainedId_Type {
	enum ContentionResolution_ContainedId_Type_Sel d;
	union ContentionResolution_ContainedId_Type_Value v;
};

enum ContentionResolution_ContainedDlschSdu_Type_Sel {
	ContentionResolution_ContainedDlschSdu_Type_UNBOUND_VALUE = 0,
	ContentionResolution_ContainedDlschSdu_Type_RlcPduCCCH = 1,
	ContentionResolution_ContainedDlschSdu_Type_PdcpSduDCCH = 2,
	ContentionResolution_ContainedDlschSdu_Type_None = 3,
};

union ContentionResolution_ContainedDlschSdu_Type_Value {
	OCTET_STRING RlcPduCCCH;
	OCTET_STRING PdcpSduDCCH;
	Null_Type None;
};

struct ContentionResolution_ContainedDlschSdu_Type {
	enum ContentionResolution_ContainedDlschSdu_Type_Sel d;
	union ContentionResolution_ContainedDlschSdu_Type_Value v;
};

struct TCRNTI_ContentionResolutionMacPdu_Type {
	struct ContentionResolution_ContainedId_Type ContainedId;
	struct ContentionResolution_ContainedDlschSdu_Type ContainedRlcPdu;
};

enum TCRNTI_ContentionResolutionCtrl_Type_Sel {
	TCRNTI_ContentionResolutionCtrl_Type_UNBOUND_VALUE = 0,
	TCRNTI_ContentionResolutionCtrl_Type_MacPdu = 1,
	TCRNTI_ContentionResolutionCtrl_Type_MacPdu_CRC_Error = 2,
	TCRNTI_ContentionResolutionCtrl_Type_NoContResolID = 3,
};

union TCRNTI_ContentionResolutionCtrl_Type_Value {
	struct TCRNTI_ContentionResolutionMacPdu_Type MacPdu;
	struct TCRNTI_ContentionResolutionMacPdu_Type MacPdu_CRC_Error;
	Null_Type NoContResolID;
};

struct TCRNTI_ContentionResolutionCtrl_Type {
	enum TCRNTI_ContentionResolutionCtrl_Type_Sel d;
	union TCRNTI_ContentionResolutionCtrl_Type_Value v;
};

enum CRNTI_ContentionResolutionCtrl_Type_Sel {
	CRNTI_ContentionResolutionCtrl_Type_UNBOUND_VALUE = 0,
	CRNTI_ContentionResolutionCtrl_Type_AutomaticGrant = 1,
	CRNTI_ContentionResolutionCtrl_Type_None = 2,
};

union CRNTI_ContentionResolutionCtrl_Type_Value {
	struct DciUlInfo_Type AutomaticGrant;
	Null_Type None;
};

struct CRNTI_ContentionResolutionCtrl_Type {
	enum CRNTI_ContentionResolutionCtrl_Type_Sel d;
	union CRNTI_ContentionResolutionCtrl_Type_Value v;
};

enum ContentionResolutionCtrl_Type_Sel {
	ContentionResolutionCtrl_Type_UNBOUND_VALUE = 0,
	ContentionResolutionCtrl_Type_TCRNTI_Based = 1,
	ContentionResolutionCtrl_Type_CRNTI_Based = 2,
};

union ContentionResolutionCtrl_Type_Value {
	struct TCRNTI_ContentionResolutionCtrl_Type TCRNTI_Based;
	struct CRNTI_ContentionResolutionCtrl_Type CRNTI_Based;
};

struct ContentionResolutionCtrl_Type {
	enum ContentionResolutionCtrl_Type_Sel d;
	union ContentionResolutionCtrl_Type_Value v;
};

struct RachProcedure_Type {
	struct RandomAccessResponseConfig_Type RAResponse;
	struct ContentionResolutionCtrl_Type ContentionResolutionCtrl;
};

struct RachProcedure_Type_RachProcedureList_Type_Dynamic {
	size_t d;
	struct RachProcedure_Type* v;
};

typedef struct RachProcedure_Type_RachProcedureList_Type_Dynamic RachProcedureList_Type;

struct RACH_ConfigCommon_Type_RACH_ConfigCommon_Optional {
	bool d;
	struct RACH_ConfigCommon_Type v;
};

struct RACH_ConfigDedicated_Type_RACH_ConfigDedicated_Optional {
	bool d;
	struct RACH_ConfigDedicated_Type v;
};

struct RachProcedureList_Type_RachProcedureList_Optional {
	bool d;
	RachProcedureList_Type v;
};

struct RachProcedureConfig_Type {
	struct RACH_ConfigCommon_Type_RACH_ConfigCommon_Optional RACH_ConfigCommon;
	struct RACH_ConfigDedicated_Type_RACH_ConfigDedicated_Optional RACH_ConfigDedicated;
	struct RachProcedureList_Type_RachProcedureList_Optional RachProcedureList;
};

SIDL_END_C_INTERFACE
