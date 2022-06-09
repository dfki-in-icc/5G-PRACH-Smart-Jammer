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
#include "SidlCommonBase.h"

SIDL_BEGIN_C_INTERFACE

enum EUTRA_CellId_Type {
	eutra_Cell_NonSpecific = 0,
	eutra_Cell1 = 1,
	eutra_Cell2 = 2,
	eutra_Cell3 = 3,
	eutra_Cell4 = 4,
	eutra_Cell6 = 5,
	eutra_Cell10 = 6,
	eutra_Cell11 = 7,
	eutra_Cell12 = 8,
	eutra_Cell13 = 9,
	eutra_Cell14 = 10,
	eutra_Cell23 = 11,
	eutra_Cell28 = 12,
	eutra_Cell29 = 13,
	eutra_Cell30 = 14,
	eutra_Cell31 = 15,
	eutra_CellA = 16,
	eutra_CellB = 17,
	eutra_CellC = 18,
	eutra_CellD = 19,
	eutra_CellE = 20,
	eutra_CellG = 21,
	eutra_CellH = 22,
	eutra_CellI = 23,
	eutra_CellJ = 24,
	eutra_CellK = 25,
	eutra_CellL = 26,
	eutra_CellM = 27,
};

typedef enum EUTRA_CellId_Type EUTRA_CellId_Type;

enum NR_CellId_Type {
	nr_Cell_NonSpecific = 0,
	nr_Cell1 = 1,
	nr_Cell2 = 2,
	nr_Cell3 = 3,
	nr_Cell4 = 4,
	nr_Cell6 = 5,
	nr_Cell10 = 6,
	nr_Cell11 = 7,
	nr_Cell12 = 8,
	nr_Cell13 = 9,
	nr_Cell14 = 10,
	nr_Cell23 = 11,
	nr_Cell28 = 12,
	nr_Cell29 = 13,
	nr_Cell30 = 14,
	nr_Cell31 = 15,
};

typedef enum NR_CellId_Type NR_CellId_Type;

typedef uint8_t SRB_Identity_Type;

typedef uint8_t DRB_Identity_Type;

typedef uint8_t Pmch_InfoIndex_Type;

typedef MBMS_SessionInfo_r9_logicalChannelIdentity_r9 LogicalChannelIdentity_r9_Type;

typedef uint16_t RACH_TimingAdvance_Type;

typedef uint8_t ImcsValue_Type;

typedef uint8_t RedundancyVersion_Type;

typedef uint8_t RepetitionNumber_Type;

typedef uint8_t DCISubframeRepetitionNumber_Type;

typedef uint8_t TimingAdvanceIndex_Type;

struct PmchLogicalChannel_Type {
	Pmch_InfoIndex_Type Pmch_InfoIndex;
	LogicalChannelIdentity_r9_Type LogicalChannelIdentity;
};

struct MRB_Identity_Type {
	MBSFN_AreaId_r12 Mbsfn_AreaId;
	struct PmchLogicalChannel_Type PmchLogicalChannel;
};

struct SC_MRB_Identity_Type {
	struct MBMSSessionInfo_r13 MbmsSessionInfo;
};

struct QosFlow_Identification_Type {
	int32_t PDU_SessionId;
	int32_t QFI;
};

typedef uint16_t SystemFrameNumber_Type;

enum SystemFrameNumberInfo_Type_Sel {
	SystemFrameNumberInfo_Type_UNBOUND_VALUE = 0,
	SystemFrameNumberInfo_Type_Number = 1,
	SystemFrameNumberInfo_Type_Any = 2,
};

union SystemFrameNumberInfo_Type_Value {
	SystemFrameNumber_Type Number;
	Null_Type Any;
};

struct SystemFrameNumberInfo_Type {
	enum SystemFrameNumberInfo_Type_Sel d;
	union SystemFrameNumberInfo_Type_Value v;
};

typedef uint8_t SubFrameNumber_Type;

enum SubFrameInfo_Type_Sel {
	SubFrameInfo_Type_UNBOUND_VALUE = 0,
	SubFrameInfo_Type_Number = 1,
	SubFrameInfo_Type_Any = 2,
};

union SubFrameInfo_Type_Value {
	SubFrameNumber_Type Number;
	Null_Type Any;
};

struct SubFrameInfo_Type {
	enum SubFrameInfo_Type_Sel d;
	union SubFrameInfo_Type_Value v;
};

typedef struct SystemFrameNumberInfo_Type HyperSystemFrameNumberInfo_Type;

enum SlotOffset_Type_Sel {
	SlotOffset_Type_UNBOUND_VALUE = 0,
	SlotOffset_Type_Numerology0 = 1,
	SlotOffset_Type_Numerology1 = 2,
	SlotOffset_Type_Numerology2 = 3,
	SlotOffset_Type_Numerology3 = 4,
	SlotOffset_Type_Numerology4 = 5,
};

union SlotOffset_Type_Value {
	Null_Type Numerology0;
	uint8_t Numerology1;
	uint8_t Numerology2;
	uint8_t Numerology3;
	uint8_t Numerology4;
};

struct SlotOffset_Type {
	enum SlotOffset_Type_Sel d;
	union SlotOffset_Type_Value v;
};

enum SlotTimingInfo_Type_Sel {
	SlotTimingInfo_Type_UNBOUND_VALUE = 0,
	SlotTimingInfo_Type_SlotOffset = 1,
	SlotTimingInfo_Type_FirstSlot = 2,
	SlotTimingInfo_Type_Any = 3,
};

union SlotTimingInfo_Type_Value {
	struct SlotOffset_Type SlotOffset;
	Null_Type FirstSlot;
	Null_Type Any;
};

struct SlotTimingInfo_Type {
	enum SlotTimingInfo_Type_Sel d;
	union SlotTimingInfo_Type_Value v;
};

struct SubFrameTiming_Type {
	struct SystemFrameNumberInfo_Type SFN;
	struct SubFrameInfo_Type Subframe;
	HyperSystemFrameNumberInfo_Type HSFN;
	struct SlotTimingInfo_Type Slot;
};

enum TimingInfo_Type_Sel {
	TimingInfo_Type_UNBOUND_VALUE = 0,
	TimingInfo_Type_SubFrame = 1,
	TimingInfo_Type_Now = 2,
	TimingInfo_Type_None = 3,
};

union TimingInfo_Type_Value {
	struct SubFrameTiming_Type SubFrame;
	Null_Type Now;
	Null_Type None;
};

struct TimingInfo_Type {
	enum TimingInfo_Type_Sel d;
	union TimingInfo_Type_Value v;
};

struct ReqAspControlInfo_Type {
	bool CnfFlag;
	bool FollowOnFlag;
};

enum RlcBearerRouting_Type_Sel {
	RlcBearerRouting_Type_UNBOUND_VALUE = 0,
	RlcBearerRouting_Type_EUTRA = 1,
	RlcBearerRouting_Type_NR = 2,
	RlcBearerRouting_Type_None = 3,
};

union RlcBearerRouting_Type_Value {
	EUTRA_CellId_Type EUTRA;
	NR_CellId_Type NR;
	Null_Type None;
};

struct RlcBearerRouting_Type {
	enum RlcBearerRouting_Type_Sel d;
	union RlcBearerRouting_Type_Value v;
};

enum ConfirmationResult_Type_Sel {
	ConfirmationResult_Type_UNBOUND_VALUE = 0,
	ConfirmationResult_Type_Success = 1,
	ConfirmationResult_Type_Error = 2,
};

union ConfirmationResult_Type_Value {
	Null_Type Success;
	int32_t Error;
};

struct ConfirmationResult_Type {
	enum ConfirmationResult_Type_Sel d;
	union ConfirmationResult_Type_Value v;
};

enum RadioBearerId_Type_Sel {
	RadioBearerId_Type_UNBOUND_VALUE = 0,
	RadioBearerId_Type_Srb = 1,
	RadioBearerId_Type_Drb = 2,
	RadioBearerId_Type_Mrb = 3,
	RadioBearerId_Type_ScMrb = 4,
};

union RadioBearerId_Type_Value {
	SRB_Identity_Type Srb;
	DRB_Identity_Type Drb;
	struct MRB_Identity_Type Mrb;
	struct SC_MRB_Identity_Type ScMrb;
};

struct RadioBearerId_Type {
	enum RadioBearerId_Type_Sel d;
	union RadioBearerId_Type_Value v;
};

enum RoutingInfo_Type_Sel {
	RoutingInfo_Type_UNBOUND_VALUE = 0,
	RoutingInfo_Type_None = 1,
	RoutingInfo_Type_RadioBearerId = 2,
	RoutingInfo_Type_QosFlow = 3,
};

union RoutingInfo_Type_Value {
	Null_Type None;
	struct RadioBearerId_Type RadioBearerId;
	struct QosFlow_Identification_Type QosFlow;
};

struct RoutingInfo_Type {
	enum RoutingInfo_Type_Sel d;
	union RoutingInfo_Type_Value v;
};

struct IntegrityErrorIndication_Type {
	bool Nas;
	bool Pdcp;
};

struct ErrorIndication_Type {
	struct IntegrityErrorIndication_Type Integrity;
	int32_t System;
};

enum IndicationStatus_Type_Sel {
	IndicationStatus_Type_UNBOUND_VALUE = 0,
	IndicationStatus_Type_Ok = 1,
	IndicationStatus_Type_Error = 2,
};

union IndicationStatus_Type_Value {
	Null_Type Ok;
	struct ErrorIndication_Type Error;
};

struct IndicationStatus_Type {
	enum IndicationStatus_Type_Sel d;
	union IndicationStatus_Type_Value v;
};

struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional {
	bool d;
	struct RlcBearerRouting_Type v;
};

struct IndAspCommonPart_Type {
	EUTRA_CellId_Type CellId;
	struct RoutingInfo_Type RoutingInfo;
	struct TimingInfo_Type TimingInfo;
	struct IndicationStatus_Type Status;
	struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional RlcBearerRouting;
};

struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional {
	bool d;
	struct RlcBearerRouting_Type v;
};

struct ReqAspCommonPart_Type {
	EUTRA_CellId_Type CellId;
	struct RoutingInfo_Type RoutingInfo;
	struct TimingInfo_Type TimingInfo;
	struct ReqAspControlInfo_Type ControlInfo;
	struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional RlcBearerRouting;
};

struct CnfAspCommonPart_Type {
	EUTRA_CellId_Type CellId;
	struct RoutingInfo_Type RoutingInfo;
	struct TimingInfo_Type TimingInfo;
	struct ConfirmationResult_Type Result;
};

enum PdcpCountFormat_Type {
	PdcpCount_Srb = 0,
	PdcpCount_DrbLongSQN = 1,
	PdcpCount_DrbShortSQN = 2,
	NrPdcpCount_Srb = 3,
	NrPdcpCount_DrbSQN12 = 4,
	NrPdcpCount_DrbSQN18 = 5,
};

typedef enum PdcpCountFormat_Type PdcpCountFormat_Type;

typedef B32_Type PdcpCountValue_Type;

struct PdcpCount_Type {
	PdcpCountFormat_Type Format;
	PdcpCountValue_Type Value;
};

struct PdcpCount_Type_UL_Optional {
	bool d;
	struct PdcpCount_Type v;
};

struct PdcpCount_Type_DL_Optional {
	bool d;
	struct PdcpCount_Type v;
};

struct PdcpCountInfo_Type {
	struct RadioBearerId_Type RadioBearerId;
	struct PdcpCount_Type_UL_Optional UL;
	struct PdcpCount_Type_DL_Optional DL;
};

enum Pdcp_CountCnf_Type_Sel {
	Pdcp_CountCnf_Type_UNBOUND_VALUE = 0,
	Pdcp_CountCnf_Type_Get = 1,
	Pdcp_CountCnf_Type_Set = 2,
};

struct PdcpCountInfo_Type_Get_Dynamic {
	size_t d;
	struct PdcpCountInfo_Type* v;
};

union Pdcp_CountCnf_Type_Value {
	struct PdcpCountInfo_Type_Get_Dynamic Get;
	Null_Type Set;
};

struct Pdcp_CountCnf_Type {
	enum Pdcp_CountCnf_Type_Sel d;
	union Pdcp_CountCnf_Type_Value v;
};

enum CellConfigCapability_Type {
	Capability_broadcastOnlyCell = 0,
	Capability_minimumUplinkCell = 1,
	Capability_fullCell = 2,
};

typedef enum CellConfigCapability_Type CellConfigCapability_Type;

typedef B6_Type N_PSCCH_Type;

struct EUTRA_CellId_Type_EUTRA_CellIdList_Type_Dynamic {
	size_t d;
	EUTRA_CellId_Type* v;
};

typedef struct EUTRA_CellId_Type_EUTRA_CellIdList_Type_Dynamic EUTRA_CellIdList_Type;

struct FreqDomainSchedulExplicit_Type {
	int32_t FirstRbIndex;
	int32_t Nprb;
};

enum Imcs_Type_Sel {
	Imcs_Type_UNBOUND_VALUE = 0,
	Imcs_Type_Value = 1,
	Imcs_Type_NotUsed = 2,
};

union Imcs_Type_Value {
	ImcsValue_Type Value;
	Null_Type NotUsed;
};

struct Imcs_Type {
	enum Imcs_Type_Sel d;
	union Imcs_Type_Value v;
};

enum PdcchDciFormat_Type {
	dci_0 = 0,
	dci_0A = 1,
	dci_0B = 2,
	dci_1 = 3,
	dci_1A = 4,
	dci_1B = 5,
	dci_1C = 6,
	dci_1D = 7,
	dci_2 = 8,
	dci_2A = 9,
	dci_2B = 10,
	dci_2C = 11,
	dci_2D = 12,
	dci_3 = 13,
	dci_3A = 14,
	dci_4 = 15,
	dci_4A = 16,
	dci_4B = 17,
	dci_6_0A = 18,
	dci_6_0B = 19,
	dci_6_1A = 20,
	dci_6_1B = 21,
	dci_6_2 = 22,
};

typedef enum PdcchDciFormat_Type PdcchDciFormat_Type;

enum PdcchResourceAllocation_Type {
	ra_0 = 0,
	ra_1 = 1,
	ra_2_Localised = 2,
	ra_2_Distributed = 3,
	ra_Unspecified = 4,
};

typedef enum PdcchResourceAllocation_Type PdcchResourceAllocation_Type;

struct RedundancyVersion_Type_RedundancyVersionListDL_Type_Dynamic {
	size_t d;
	RedundancyVersion_Type* v;
};

typedef struct RedundancyVersion_Type_RedundancyVersionListDL_Type_Dynamic RedundancyVersionListDL_Type;

struct RetransmissionAsynchronous_Type {
	RedundancyVersion_Type RedundencyVer;
	int32_t SubframeOffset;
	RepetitionNumber_Type RepetitionNumber;
	DCISubframeRepetitionNumber_Type DCISubframeRepetitionNumber;
};

enum UL_TransRetransmission_Type_Sel {
	UL_TransRetransmission_Type_UNBOUND_VALUE = 0,
	UL_TransRetransmission_Type_NewTransmission = 1,
	UL_TransRetransmission_Type_ReTransmissionAdaptive = 2,
	UL_TransRetransmission_Type_ReTransmissionNonAdaptive = 3,
	UL_TransRetransmission_Type_ReTransmissionNonAdaptiveFixedRV = 4,
	UL_TransRetransmission_Type_RetransmissionAsynchronous = 5,
};

union UL_TransRetransmission_Type_Value {
	Null_Type NewTransmission;
	RedundancyVersion_Type ReTransmissionAdaptive;
	Null_Type ReTransmissionNonAdaptive;
	Null_Type ReTransmissionNonAdaptiveFixedRV;
	struct RetransmissionAsynchronous_Type RetransmissionAsynchronous;
};

struct UL_TransRetransmission_Type {
	enum UL_TransRetransmission_Type_Sel d;
	union UL_TransRetransmission_Type_Value v;
};

struct UL_TransRetransmission_Type_UL_TransRetransmissionList_Type_Dynamic {
	size_t d;
	struct UL_TransRetransmission_Type* v;
};

typedef struct UL_TransRetransmission_Type_UL_TransRetransmissionList_Type_Dynamic UL_TransRetransmissionList_Type;

enum NumSchedSubframes_Type_Sel {
	NumSchedSubframes_Type_UNBOUND_VALUE = 0,
	NumSchedSubframes_Type_MaxTwoSubframes = 1,
	NumSchedSubframes_Type_MaxFourSubframes = 2,
};

union NumSchedSubframes_Type_Value {
	B1_Type MaxTwoSubframes;
	B2_Type MaxFourSubframes;
};

struct NumSchedSubframes_Type {
	enum NumSchedSubframes_Type_Sel d;
	union NumSchedSubframes_Type_Value v;
};

struct PdcchDciFormat_Type_Format_Optional {
	bool d;
	PdcchDciFormat_Type v;
};

struct int32_t_HarqProcessId_Optional {
	bool d;
	int32_t v;
};

struct RepetitionNumber_Type_DciUlInfo_Type_RepetitionNumber_Optional {
	bool d;
	RepetitionNumber_Type v;
};

struct DCISubframeRepetitionNumber_Type_DciUlInfo_Type_DCISubframeRepetitionNumber_Optional {
	bool d;
	DCISubframeRepetitionNumber_Type v;
};

struct B1_Type_PUSCH_TriggerA_Optional {
	bool d;
	B1_Type v;
};

struct B4_Type_TimingOffset_Optional {
	bool d;
	B4_Type v;
};

struct NumSchedSubframes_Type_NumberOfScheduledSubframes_Optional {
	bool d;
	struct NumSchedSubframes_Type v;
};

struct DciUlInfo_Type {
	struct Imcs_Type Imcs;
	UL_TransRetransmissionList_Type TransRetransmissionList;
	struct FreqDomainSchedulExplicit_Type FreqDomainSchedul;
	struct PdcchDciFormat_Type_Format_Optional Format;
	struct int32_t_HarqProcessId_Optional HarqProcessId;
	struct RepetitionNumber_Type_DciUlInfo_Type_RepetitionNumber_Optional RepetitionNumber;
	struct DCISubframeRepetitionNumber_Type_DciUlInfo_Type_DCISubframeRepetitionNumber_Optional DCISubframeRepetitionNumber;
	struct B1_Type_PUSCH_TriggerA_Optional PUSCH_TriggerA;
	struct B4_Type_TimingOffset_Optional TimingOffset;
	struct NumSchedSubframes_Type_NumberOfScheduledSubframes_Optional NumberOfScheduledSubframes;
};

enum Modulation_Type {
	Modulation_unused = 0,
	Modulation_qpsk = 1,
	Modulation_qam16 = 2,
	Modulation_qam64 = 3,
};

typedef enum Modulation_Type Modulation_Type;

struct FreqDomainSchedulCommon_Type {
	int32_t FirstRbIndex;
	int32_t MaxRbCnt;
};

struct RepetitionNumber_Type_DciDlInfoCommon_Type_RepetitionNumber_Optional {
	bool d;
	RepetitionNumber_Type v;
};

struct DCISubframeRepetitionNumber_Type_DciDlInfoCommon_Type_DCISubframeRepetitionNumber_Optional {
	bool d;
	DCISubframeRepetitionNumber_Type v;
};

struct DciDlInfoCommon_Type {
	PdcchDciFormat_Type Format;
	PdcchResourceAllocation_Type ResourceAllocType;
	Modulation_Type Modulation_1stCW;
	Modulation_Type Modulation_2ndCW;
	struct FreqDomainSchedulCommon_Type FreqDomainSchedul;
	RedundancyVersionListDL_Type RedundancyVersionList;
	struct RepetitionNumber_Type_DciDlInfoCommon_Type_RepetitionNumber_Optional RepetitionNumber;
	struct DCISubframeRepetitionNumber_Type_DciDlInfoCommon_Type_DCISubframeRepetitionNumber_Optional DCISubframeRepetitionNumber;
};

enum MIMO_PrecodingBits_Type_Sel {
	MIMO_PrecodingBits_Type_UNBOUND_VALUE = 0,
	MIMO_PrecodingBits_Type_None = 1,
	MIMO_PrecodingBits_Type_Bit2 = 2,
	MIMO_PrecodingBits_Type_Bit3 = 3,
	MIMO_PrecodingBits_Type_Bit6 = 4,
};

union MIMO_PrecodingBits_Type_Value {
	Null_Type None;
	B2_Type Bit2;
	B3_Type Bit3;
	B6_Type Bit6;
};

struct MIMO_PrecodingBits_Type {
	enum MIMO_PrecodingBits_Type_Sel d;
	union MIMO_PrecodingBits_Type_Value v;
};

struct RedundancyVersionListDL_Type_RedundancyVersionList_2ndCW_Optional {
	bool d;
	RedundancyVersionListDL_Type v;
};

struct MIMO_DciDlInfo_Type {
	struct RedundancyVersionListDL_Type_RedundancyVersionList_2ndCW_Optional RedundancyVersionList_2ndCW;
	B1_Type CodeWordSwapFlag;
	struct MIMO_PrecodingBits_Type PrecodingBits;
};

struct MIMO_DciDlInfo_Type_MimoInfo_Optional {
	bool d;
	struct MIMO_DciDlInfo_Type v;
};

struct RepetitionNumber_Type_RepetitionNumber_Optional {
	bool d;
	RepetitionNumber_Type v;
};

struct DCISubframeRepetitionNumber_Type_DCISubframeRepetitionNumber_Optional {
	bool d;
	DCISubframeRepetitionNumber_Type v;
};

struct DciDlInfoExplicit_Type {
	struct Imcs_Type Imcs_1stCW;
	struct Imcs_Type Imcs_2ndCW;
	PdcchDciFormat_Type Format;
	PdcchResourceAllocation_Type ResourceAllocType;
	struct FreqDomainSchedulExplicit_Type FreqDomainSchedul;
	RedundancyVersionListDL_Type RedundancyVersionList;
	struct MIMO_DciDlInfo_Type_MimoInfo_Optional MimoInfo;
	struct RepetitionNumber_Type_RepetitionNumber_Optional RepetitionNumber;
	struct DCISubframeRepetitionNumber_Type_DCISubframeRepetitionNumber_Optional DCISubframeRepetitionNumber;
};

enum DciDlInfo_Type_Sel {
	DciDlInfo_Type_UNBOUND_VALUE = 0,
	DciDlInfo_Type_Auto = 1,
	DciDlInfo_Type_Explicit = 2,
};

union DciDlInfo_Type_Value {
	struct DciDlInfoCommon_Type Auto;
	struct DciDlInfoExplicit_Type Explicit;
};

struct DciDlInfo_Type {
	enum DciDlInfo_Type_Sel d;
	union DciDlInfo_Type_Value v;
};

enum PucchHoppingBits_Type_Sel {
	PucchHoppingBits_Type_UNBOUND_VALUE = 0,
	PucchHoppingBits_Type_OneBit = 1,
	PucchHoppingBits_Type_TwoBits = 2,
};

union PucchHoppingBits_Type_Value {
	B1_Type OneBit;
	B2_Type TwoBits;
};

struct PucchHoppingBits_Type {
	enum PucchHoppingBits_Type_Sel d;
	union PucchHoppingBits_Type_Value v;
};

struct UplinkHoppingResourceParameters_Type {
	struct PucchHoppingBits_Type PucchHopping;
};

enum UplinkHoppingControl_Type_Sel {
	UplinkHoppingControl_Type_UNBOUND_VALUE = 0,
	UplinkHoppingControl_Type_Deactivated = 1,
	UplinkHoppingControl_Type_Activated = 2,
};

union UplinkHoppingControl_Type_Value {
	Null_Type Deactivated;
	struct UplinkHoppingResourceParameters_Type Activated;
};

struct UplinkHoppingControl_Type {
	enum UplinkHoppingControl_Type_Sel d;
	union UplinkHoppingControl_Type_Value v;
};

typedef UE_EUTRA_Capability_ue_Category UE_Category_Type;

typedef UE_EUTRA_Capability_v1020_IEs_ue_Category_v1020 UE_Category_v1020_Type;

typedef UE_EUTRA_Capability_v1170_IEs_ue_Category_v1170 UE_Category_v1170_Type;

typedef UE_EUTRA_Capability_v11a0_IEs_ue_Category_v11a0 UE_Category_v11a0_Type;

typedef UE_RadioPagingInfo_r12_ue_Category_v1250 UE_Category_v1250_Type;

typedef UE_EUTRA_Capability_v1250_IEs_ue_CategoryDL_r12 UE_CategoryDL_r12_Type;

typedef UE_EUTRA_Capability_v1260_IEs_ue_CategoryDL_v1260 UE_CategoryDL_v1260_Type;

typedef UE_EUTRA_Capability_v1310_IEs_ue_CategoryDL_v1310_e UE_CategoryDL_v1310_Type;

typedef UE_EUTRA_Capability_v1330_IEs_ue_CategoryDL_v1330 UE_CategoryDL_v1330_Type;

typedef UE_EUTRA_Capability_v1350_IEs_ue_CategoryDL_v1350_e UE_CategoryDL_v1350_Type;

typedef UE_EUTRA_Capability_v1450_IEs_ue_CategoryDL_v1450 UE_CategoryDL_v1450_Type;

typedef UE_EUTRA_Capability_v1460_IEs_ue_CategoryDL_v1460 UE_CategoryDL_v1460_Type;

SIDL_END_C_INTERFACE
