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

typedef MasterInformationBlock_schedulingInfoSIB1_BR_r13 SchedulingInfoSIB1_BR_r13_Type;

typedef struct SystemInformationBlockType1_v1310_IEs_bandwidthReducedAccessRelatedInfo_r13 BandwidthReducedAccessRelatedInfo_Type;

struct int32_t_SI_SubframeOffsetList_Type_Dynamic {
	size_t d;
	int32_t* v;
};

typedef struct int32_t_SI_SubframeOffsetList_Type_Dynamic SI_SubframeOffsetList_Type;

struct Bcch_BRToPbchConfig_Type {
	bool EnableMIB_Repetition;
};

struct Sib1_BRSchedul_Type {
	SchedulingInfoSIB1_BR_r13_Type SchedulingInfoSIB1_BR_r13;
};

struct Sib1_BRSchedul_Type_Sib1_BRSchedul_Optional {
	bool d;
	struct Sib1_BRSchedul_Type v;
};

struct BandwidthReducedAccessRelatedInfo_Type_SiSchedul_Optional {
	bool d;
	BandwidthReducedAccessRelatedInfo_Type v;
};

struct SI_SubframeOffsetList_Type_SubframeOffsetList_Optional {
	bool d;
	SI_SubframeOffsetList_Type v;
};

struct Bcch_BRToPdschConfig_Type {
	struct Sib1_BRSchedul_Type_Sib1_BRSchedul_Optional Sib1_BRSchedul;
	struct BandwidthReducedAccessRelatedInfo_Type_SiSchedul_Optional SiSchedul;
	struct SI_SubframeOffsetList_Type_SubframeOffsetList_Optional SubframeOffsetList;
};

struct BCCH_DL_SCH_Message_BR_BR_SI_List_Type_Dynamic {
	size_t d;
	struct BCCH_DL_SCH_Message_BR* v;
};

typedef struct BCCH_DL_SCH_Message_BR_BR_SI_List_Type_Dynamic BR_SI_List_Type;

struct BR_SI_List_Type_BR_SegmentedSI_List_Type_Dynamic {
	size_t d;
	BR_SI_List_Type* v;
};

typedef struct BR_SI_List_Type_BR_SegmentedSI_List_Type_Dynamic BR_SegmentedSI_List_Type;

struct BCCH_BCH_Message_Bcch_BRInfo_Type_MIB_Optional {
	bool d;
	struct BCCH_BCH_Message v;
};

struct BCCH_DL_SCH_Message_BR_Bcch_BRInfo_Type_SIB1_Optional {
	bool d;
	struct BCCH_DL_SCH_Message_BR v;
};

struct BR_SI_List_Type_SIs_Optional {
	bool d;
	BR_SI_List_Type v;
};

struct BR_SegmentedSI_List_Type_SegmentedSIs_Optional {
	bool d;
	BR_SegmentedSI_List_Type v;
};

struct Bcch_BRInfo_Type {
	struct BCCH_BCH_Message_Bcch_BRInfo_Type_MIB_Optional MIB;
	struct BCCH_DL_SCH_Message_BR_Bcch_BRInfo_Type_SIB1_Optional SIB1;
	struct BR_SI_List_Type_SIs_Optional SIs;
	struct BR_SegmentedSI_List_Type_SegmentedSIs_Optional SegmentedSIs;
};

struct Bcch_BRToPbchConfig_Type_Pbch_Optional {
	bool d;
	struct Bcch_BRToPbchConfig_Type v;
};

struct Bcch_BRToPdschConfig_Type_Pdsch_Optional {
	bool d;
	struct Bcch_BRToPdschConfig_Type v;
};

struct Bcch_BRInfo_Type_BcchInfo_Optional {
	bool d;
	struct Bcch_BRInfo_Type v;
};

struct Null_Type_Bcch_BRConfig_Type_StopSib1Transmission_Optional {
	bool d;
	Null_Type v;
};

struct Bcch_BRConfig_Type {
	struct Bcch_BRToPbchConfig_Type_Pbch_Optional Pbch;
	struct Bcch_BRToPdschConfig_Type_Pdsch_Optional Pdsch;
	struct Bcch_BRInfo_Type_BcchInfo_Optional BcchInfo;
	struct Null_Type_Bcch_BRConfig_Type_StopSib1Transmission_Optional StopSib1Transmission;
};

SIDL_END_C_INTERFACE
