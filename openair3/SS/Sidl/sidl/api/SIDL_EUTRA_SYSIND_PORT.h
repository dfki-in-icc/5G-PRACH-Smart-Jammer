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
#include "SidlNrCommon.h"
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

typedef uint8_t PrachPreambleIndex_Type;

typedef Dummy_Type PRTPower_Type;

typedef int LogicalChannelGroup_Type;

typedef uint8_t PHR_Type;

struct int_RepetitionsPerPreambleAttempt_Optional {
	bool d;
	int v;
};

struct RachPreamble_Type {
	PrachPreambleIndex_Type RAPID;
	PRTPower_Type PRTPower;
	struct int_RepetitionsPerPreambleAttempt_Optional RepetitionsPerPreambleAttempt;
};

struct SL_BSR_Value_Type {
	B4_Type DestinationIndex;
	B2_Type LCG_ID;
	B6_Type BufferSize;
};

struct SL_BSR_Value_Type_SL_BSR_ValueList_Type_Dynamic {
	size_t d;
	struct SL_BSR_Value_Type* v;
};

typedef struct SL_BSR_Value_Type_SL_BSR_ValueList_Type_Dynamic SL_BSR_ValueList_Type;

struct SL_BSR_MACHeader_Type {
	B5_Type HeaderFieldLCID;
	B1_Type HeaderFieldE;
	B1_Type HeaderFieldF;
	SL_BSR_ValueList_Type SL_BSR_Value;
};

typedef int BSR_Value_Type;

struct Short_BSR_Type {
	LogicalChannelGroup_Type LCG;
	BSR_Value_Type Value;
};

struct Long_BSR_Type {
	BSR_Value_Type Value_LCG1;
	BSR_Value_Type Value_LCG2;
	BSR_Value_Type Value_LCG3;
	BSR_Value_Type Value_LCG4;
};

enum BSR_Type_Sel {
	BSR_Type_UNBOUND_VALUE = 0,
	BSR_Type_Short = 1,
	BSR_Type_Truncated = 2,
	BSR_Type_Long = 3,
	BSR_Type_Sidelink = 4,
};

union BSR_Type_Value {
	struct Short_BSR_Type Short;
	struct Short_BSR_Type Truncated;
	struct Long_BSR_Type Long;
	struct SL_BSR_MACHeader_Type Sidelink;
};

struct BSR_Type {
	enum BSR_Type_Sel d;
	union BSR_Type_Value v;
};

enum HARQ_Type {
	HARQ_Type_ack = 0,
	HARQ_Type_nack = 1,
};

typedef enum HARQ_Type HARQ_Type;

struct HarqProcessInfo_Type {
	HarqProcessId_Type Id;
	int CURRENT_TX_NB;
};

enum HarqError_Type_Sel {
	HarqError_Type_UNBOUND_VALUE = 0,
	HarqError_Type_UL = 1,
	HarqError_Type_DL = 2,
};

union HarqError_Type_Value {
	struct HarqProcessInfo_Type UL;
	struct HarqProcessInfo_Type DL;
};

struct HarqError_Type {
	enum HarqError_Type_Sel d;
	union HarqError_Type_Value v;
};

struct RlcDiscardInd_Type {
	int SequenceNumber;
};

typedef uint8_t RI_Type;

enum SRSInd_Type {
	SRSInd_Type_SRS_Start = 0,
	SRSInd_Type_SRS_Stop = 1,
};

typedef enum SRSInd_Type SRSInd_Type;

enum SystemIndication_Type_Sel {
	SystemIndication_Type_UNBOUND_VALUE = 0,
	SystemIndication_Type_Error = 1,
	SystemIndication_Type_RachPreamble = 2,
	SystemIndication_Type_SchedReq = 3,
	SystemIndication_Type_BSR = 4,
	SystemIndication_Type_UL_HARQ = 5,
	SystemIndication_Type_C_RNTI = 6,
	SystemIndication_Type_PHR = 7,
	SystemIndication_Type_HarqError = 8,
	SystemIndication_Type_RlcDiscardInd = 9,
	SystemIndication_Type_PeriodicRI = 10,
	SystemIndication_Type_EPHR = 11,
	SystemIndication_Type_CqiInd = 12,
	SystemIndication_Type_SrsInd = 13,
	SystemIndication_Type_DC_PHR = 14,
};

union SystemIndication_Type_Value {
	CHAR_STRING Error;
	struct RachPreamble_Type RachPreamble;
	Null_Type SchedReq;
	struct BSR_Type BSR;
	HARQ_Type UL_HARQ;
	C_RNTI C_RNTI;
	PHR_Type PHR;
	struct HarqError_Type HarqError;
	struct RlcDiscardInd_Type RlcDiscardInd;
	RI_Type PeriodicRI;
	struct MAC_CTRL_ExtPowerHeadRoom_Type EPHR;
	Null_Type CqiInd;
	SRSInd_Type SrsInd;
	struct MAC_CTRL_DC_PowerHeadRoom_Type DC_PHR;
};

struct SystemIndication_Type {
	enum SystemIndication_Type_Sel d;
	union SystemIndication_Type_Value v;
};

struct SYSTEM_IND {
	struct IndAspCommonPart_Type Common;
	struct SystemIndication_Type Indication;
};

SIDL_END_C_INTERFACE
