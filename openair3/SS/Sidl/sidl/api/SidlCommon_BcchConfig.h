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

typedef Null_Type BcchToPbchConfig_Type;

typedef SystemInformationBlockType1_si_WindowLength_e SiWindowLength_Type;

struct BCCH_DL_SCH_Message_SI_List_Type_Dynamic {
	size_t d;
	struct BCCH_DL_SCH_Message* v;
};

typedef struct BCCH_DL_SCH_Message_SI_List_Type_Dynamic SI_List_Type;

struct SI_List_Type_SegmentedSI_List_Type_Dynamic {
	size_t d;
	SI_List_Type* v;
};

typedef struct SI_List_Type_SegmentedSI_List_Type_Dynamic SegmentedSI_List_Type;

struct DciDlInfoCommon_Type_Sib1Schedul_Type_DciInfo_Optional {
	bool d;
	struct DciDlInfoCommon_Type v;
};

struct Sib1Schedul_Type {
	struct DciDlInfoCommon_Type_Sib1Schedul_Type_DciInfo_Optional DciInfo;
};

struct DciDlInfoCommon_Type_SingleSiSchedul_Type_DciInfo_Optional {
	bool d;
	struct DciDlInfoCommon_Type v;
};

struct int32_t_SubframeOffset_Optional {
	bool d;
	int32_t v;
};

struct SingleSiSchedul_Type {
	struct DciDlInfoCommon_Type_SingleSiSchedul_Type_DciInfo_Optional DciInfo;
	struct int32_t_SubframeOffset_Optional SubframeOffset;
};

struct SI_Periodicity_r12_e_Periodicity_Optional {
	bool d;
	SI_Periodicity_r12_e v;
};

struct SingleSiSchedul_Type_Window_Dynamic {
	size_t d;
	struct SingleSiSchedul_Type* v;
};

struct SingleSiSchedul_Type_Window_DynamicOptional {
	bool d;
	struct SingleSiSchedul_Type_Window_Dynamic v;
};

struct SiSchedul_Type {
	struct SI_Periodicity_r12_e_Periodicity_Optional Periodicity;
	struct SingleSiSchedul_Type_Window_DynamicOptional Window;
};

struct SiSchedul_Type_SiSchedulList_Type_Dynamic {
	size_t d;
	struct SiSchedul_Type* v;
};

typedef struct SiSchedul_Type_SiSchedulList_Type_Dynamic SiSchedulList_Type;

struct SiWindowLength_Type_WindowLength_Optional {
	bool d;
	SiWindowLength_Type v;
};

struct SiSchedulList_Type_SiList_Optional {
	bool d;
	SiSchedulList_Type v;
};

struct SiSchedulList_Type_SegmentedSiList_Optional {
	bool d;
	SiSchedulList_Type v;
};

struct AllSiSchedul_Type {
	struct SiWindowLength_Type_WindowLength_Optional WindowLength;
	struct SiSchedulList_Type_SiList_Optional SiList;
	struct SiSchedulList_Type_SegmentedSiList_Optional SegmentedSiList;
};

struct Sib1Schedul_Type_Sib1Schedul_Optional {
	bool d;
	struct Sib1Schedul_Type v;
};

struct AllSiSchedul_Type_SiSchedul_Optional {
	bool d;
	struct AllSiSchedul_Type v;
};

struct BcchToPdschConfig_Type {
	struct Sib1Schedul_Type_Sib1Schedul_Optional Sib1Schedul;
	struct AllSiSchedul_Type_SiSchedul_Optional SiSchedul;
};

struct BCCH_BCH_Message_BcchInfo_Type_MIB_Optional {
	bool d;
	struct BCCH_BCH_Message v;
};

struct BCCH_DL_SCH_Message_BcchInfo_Type_SIB1_Optional {
	bool d;
	struct BCCH_DL_SCH_Message v;
};

struct SI_List_Type_SIs_Optional {
	bool d;
	SI_List_Type v;
};

struct SegmentedSI_List_Type_SegmentedSIs_Optional {
	bool d;
	SegmentedSI_List_Type v;
};

struct BcchInfo_Type {
	struct BCCH_BCH_Message_BcchInfo_Type_MIB_Optional MIB;
	struct BCCH_DL_SCH_Message_BcchInfo_Type_SIB1_Optional SIB1;
	struct SI_List_Type_SIs_Optional SIs;
	struct SegmentedSI_List_Type_SegmentedSIs_Optional SegmentedSIs;
};

struct BcchToPbchConfig_Type_Pbch_Optional {
	bool d;
	BcchToPbchConfig_Type v;
};

struct BcchToPdschConfig_Type_Pdsch_Optional {
	bool d;
	struct BcchToPdschConfig_Type v;
};

struct BcchInfo_Type_BcchInfo_Optional {
	bool d;
	struct BcchInfo_Type v;
};

struct Null_Type_BcchConfig_Type_StopSib1Transmission_Optional {
	bool d;
	Null_Type v;
};

struct BcchConfig_Type {
	struct BcchToPbchConfig_Type_Pbch_Optional Pbch;
	struct BcchToPdschConfig_Type_Pdsch_Optional Pdsch;
	struct BcchInfo_Type_BcchInfo_Optional BcchInfo;
	struct Null_Type_BcchConfig_Type_StopSib1Transmission_Optional StopSib1Transmission;
};

SIDL_END_C_INTERFACE
