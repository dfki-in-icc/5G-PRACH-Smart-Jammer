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

typedef C_RNTI MAC_CTRL_C_RNTI_Type;

typedef OCTET_STRING MAC_SDU_Type;

struct MAC_SDU_Type_MAC_SDUList_Type_Dynamic {
	size_t d;
	MAC_SDU_Type* v;
};

typedef struct MAC_SDU_Type_MAC_SDUList_Type_Dynamic MAC_SDUList_Type;

typedef ContentionResolutionId_Type MAC_CTRL_ContentionResolutionId_Type;

typedef B8_Type MAC_CTRL_TimingAdvance_Type;

typedef OCTET_STRING RLC_TMD_PDU_Type;

typedef B2_Type RLC_FramingInfo_Type;

typedef OCTET_STRING RLC_DataField_Type;

struct RLC_DataField_Type_RLC_DataFieldList_Type_Dynamic {
	size_t d;
	RLC_DataField_Type* v;
};

typedef struct RLC_DataField_Type_RLC_DataFieldList_Type_Dynamic RLC_DataFieldList_Type;

struct BIT_STRING_ELEMENT_RLC_Status_Padding_Type_Dynamic {
	size_t d;
	BIT_STRING_ELEMENT* v;
};

typedef struct BIT_STRING_ELEMENT_RLC_Status_Padding_Type_Dynamic RLC_Status_Padding_Type;

typedef OCTET_STRING PDCP_SDU_Type;

struct PDCP_SDU_Type_PDCP_SDUList_Type_Dynamic {
	size_t d;
	PDCP_SDU_Type* v;
};

typedef struct PDCP_SDU_Type_PDCP_SDUList_Type_Dynamic PDCP_SDUList_Type;

typedef OCTET_STRING RLC_SDU_Type;

struct RLC_SDU_Type_RLC_SDUList_Type_Dynamic {
	size_t d;
	RLC_SDU_Type* v;
};

typedef struct RLC_SDU_Type_RLC_SDUList_Type_Dynamic RLC_SDUList_Type;

struct MAC_PDU_Length_Type {
	B1_Type Format;
	B7_15_Type Value;
};

struct MAC_PDU_Length_Type_Length_Optional {
	bool d;
	struct MAC_PDU_Length_Type v;
};

struct MAC_PDU_SubHeader_Type {
	B2_Type Reserved;
	B1_Type Extension;
	B5_Type LCID;
	struct MAC_PDU_Length_Type_Length_Optional Length;
};

struct MAC_PDU_SubHeader_Type_MAC_Header_Type_Dynamic {
	size_t d;
	struct MAC_PDU_SubHeader_Type* v;
};

typedef struct MAC_PDU_SubHeader_Type_MAC_Header_Type_Dynamic MAC_Header_Type;

struct MAC_CTRL_ShortBSR_Type {
	B2_Type LCG;
	B6_Type Value;
};

struct MAC_CTRL_LongBSR_Type {
	B6_Type Value_LCG1;
	B6_Type Value_LCG2;
	B6_Type Value_LCG3;
	B6_Type Value_LCG4;
};

enum HarqProcessAssignment_Type_Sel {
	HarqProcessAssignment_Type_UNBOUND_VALUE = 0,
	HarqProcessAssignment_Type_Id = 1,
	HarqProcessAssignment_Type_Automatic = 2,
};

union HarqProcessAssignment_Type_Value {
	HarqProcessId_Type Id;
	Null_Type Automatic;
};

struct HarqProcessAssignment_Type {
	enum HarqProcessAssignment_Type_Sel d;
	union HarqProcessAssignment_Type_Value v;
};

struct MAC_CTRL_PowerHeadRoom_Type {
	B2_Type Reserved;
	B6_Type Value;
};

struct ScellBitMap_Type {
	B7_Type Value;
	B1_Type Reserved;
};

typedef struct ScellBitMap_Type MAC_CTRL_ScellActDeact_Type;

struct B2_Type_Reserved_Optional {
	bool d;
	B2_Type v;
};

struct B6_Type_PCMaxc_Optional {
	bool d;
	B6_Type v;
};

struct PH_Record_Type {
	B1_Type P_Bit;
	B1_Type V_Bit;
	B6_Type Valve;
	struct B2_Type_Reserved_Optional Reserved;
	struct B6_Type_PCMaxc_Optional PCMaxc;
};

struct PH_Record_Type_PH_RecordList_Type_Dynamic {
	size_t d;
	struct PH_Record_Type* v;
};

typedef struct PH_Record_Type_PH_RecordList_Type_Dynamic PH_RecordList_Type;

struct PH_Record_Type_DC_PH_RecordList_Type_Dynamic {
	size_t d;
	struct PH_Record_Type* v;
};

typedef struct PH_Record_Type_DC_PH_RecordList_Type_Dynamic DC_PH_RecordList_Type;

struct MAC_CTRL_ExtPowerHeadRoom_Type {
	struct ScellBitMap_Type EPH_Octet1;
	PH_RecordList_Type PH_RecordList;
};

struct MAC_CTRL_DC_PowerHeadRoom_Type {
	struct ScellBitMap_Type DC_PH_Octet1;
	DC_PH_RecordList_Type DC_PH_RecordList;
};

struct MAC_CTRL_ShortBSR_Type_ShortBSR_Optional {
	bool d;
	struct MAC_CTRL_ShortBSR_Type v;
};

struct MAC_CTRL_LongBSR_Type_LongBSR_Optional {
	bool d;
	struct MAC_CTRL_LongBSR_Type v;
};

struct MAC_CTRL_C_RNTI_Type_C_RNTI_Optional {
	bool d;
	MAC_CTRL_C_RNTI_Type v;
};

struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional {
	bool d;
	MAC_CTRL_ContentionResolutionId_Type v;
};

struct MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional {
	bool d;
	MAC_CTRL_TimingAdvance_Type v;
};

struct MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional {
	bool d;
	struct MAC_CTRL_PowerHeadRoom_Type v;
};

struct MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional {
	bool d;
	MAC_CTRL_ScellActDeact_Type v;
};

struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional {
	bool d;
	struct MAC_CTRL_ExtPowerHeadRoom_Type v;
};

struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional {
	bool d;
	struct MAC_CTRL_DC_PowerHeadRoom_Type v;
};

struct MAC_CTRL_ElementList_Type {
	struct MAC_CTRL_ShortBSR_Type_ShortBSR_Optional ShortBSR;
	struct MAC_CTRL_LongBSR_Type_LongBSR_Optional LongBSR;
	struct MAC_CTRL_C_RNTI_Type_C_RNTI_Optional C_RNTI;
	struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional ContentionResolutionID;
	struct MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional TimingAdvance;
	struct MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional PowerHeadRoom;
	struct MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional ScellActDeact;
	struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional ExtPowerHeadRoom;
	struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional DC_PowerHeadRoom;
};

struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional {
	bool d;
	struct MAC_CTRL_ElementList_Type v;
};

struct MAC_SDUList_Type_SduList_Optional {
	bool d;
	MAC_SDUList_Type v;
};

struct OCTET_STRING_Padding_Optional {
	bool d;
	OCTET_STRING v;
};

struct MAC_PDU_Type {
	MAC_Header_Type Header;
	struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional CtrlElementList;
	struct MAC_SDUList_Type_SduList_Optional SduList;
	struct OCTET_STRING_Padding_Optional Padding;
};

struct MAC_PDU_Type_MAC_PDUList_Type_Dynamic {
	size_t d;
	struct MAC_PDU_Type* v;
};

typedef struct MAC_PDU_Type_MAC_PDUList_Type_Dynamic MAC_PDUList_Type;

struct RLC_UMD_Header_FixPartShortSN_Type {
	RLC_FramingInfo_Type FramingInfo;
	B1_Type Extension;
	B5_Type SequenceNumber;
};

struct RLC_LengthIndicator_LI11_Type {
	B1_Type Extension;
	B11_Type LengthIndicator;
};

struct RLC_LengthIndicator_LI11_Type_RLC_LI11_List_Type_Dynamic {
	size_t d;
	struct RLC_LengthIndicator_LI11_Type* v;
};

typedef struct RLC_LengthIndicator_LI11_Type_RLC_LI11_List_Type_Dynamic RLC_LI11_List_Type;

struct RLC_LengthIndicator_LI15_Type {
	B1_Type Extension;
	B15_Type LengthIndicator;
};

struct RLC_LengthIndicator_LI15_Type_RLC_LI15_List_Type_Dynamic {
	size_t d;
	struct RLC_LengthIndicator_LI15_Type* v;
};

typedef struct RLC_LengthIndicator_LI15_Type_RLC_LI15_List_Type_Dynamic RLC_LI15_List_Type;

enum RLC_LI_List_Type_Sel {
	RLC_LI_List_Type_UNBOUND_VALUE = 0,
	RLC_LI_List_Type_LI11 = 1,
	RLC_LI_List_Type_LI15 = 2,
};

union RLC_LI_List_Type_Value {
	RLC_LI11_List_Type LI11;
	RLC_LI15_List_Type LI15;
};

struct RLC_LI_List_Type {
	enum RLC_LI_List_Type_Sel d;
	union RLC_LI_List_Type_Value v;
};

struct B4_Type_Padding_Optional {
	bool d;
	B4_Type v;
};

struct RLC_PDU_Header_FlexPart_Type {
	struct RLC_LI_List_Type LengthIndicator;
	struct B4_Type_Padding_Optional Padding;
};

struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional {
	bool d;
	struct RLC_PDU_Header_FlexPart_Type v;
};

struct RLC_UMD_HeaderShortSN_Type {
	struct RLC_UMD_Header_FixPartShortSN_Type FixPart;
	struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional FlexPart;
};

struct RLC_UMD_PDU_ShortSN_Type {
	struct RLC_UMD_HeaderShortSN_Type Header;
	RLC_DataFieldList_Type Data;
};

struct RLC_UMD_Header_FixPartLongSN_Type {
	B3_Type Reserved;
	RLC_FramingInfo_Type FramingInfo;
	B1_Type Extension;
	B10_Type SequenceNumber;
};

struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional {
	bool d;
	struct RLC_PDU_Header_FlexPart_Type v;
};

struct RLC_UMD_HeaderLongSN_Type {
	struct RLC_UMD_Header_FixPartLongSN_Type FixPart;
	struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional FlexPart;
};

struct RLC_UMD_PDU_LongSN_Type {
	struct RLC_UMD_HeaderLongSN_Type Header;
	RLC_DataFieldList_Type Data;
};

enum RLC_UMD_PDU_Type_Sel {
	RLC_UMD_PDU_Type_UNBOUND_VALUE = 0,
	RLC_UMD_PDU_Type_ShortSN = 1,
	RLC_UMD_PDU_Type_LongSN = 2,
};

union RLC_UMD_PDU_Type_Value {
	struct RLC_UMD_PDU_ShortSN_Type ShortSN;
	struct RLC_UMD_PDU_LongSN_Type LongSN;
};

struct RLC_UMD_PDU_Type {
	enum RLC_UMD_PDU_Type_Sel d;
	union RLC_UMD_PDU_Type_Value v;
};

struct RLC_AMD_Header_FixPart_Type {
	B1_Type D_C;
	B1_Type ReSeg;
	B1_Type Poll;
	RLC_FramingInfo_Type FramingInfo;
	B1_Type Extension;
	B10_Type SN;
};

struct RLC_AMD_Header_SegmentPart_Type {
	B1_Type LastSegmentFlag;
	B15_Type SegOffset;
};

struct RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional {
	bool d;
	struct RLC_AMD_Header_SegmentPart_Type v;
};

struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional {
	bool d;
	struct RLC_PDU_Header_FlexPart_Type v;
};

struct RLC_AMD_Header_Type {
	struct RLC_AMD_Header_FixPart_Type FixPart;
	struct RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional SegmentPart;
	struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional FlexPart;
};

struct RLC_AMD_PDU_Type {
	struct RLC_AMD_Header_Type Header;
	RLC_DataFieldList_Type Data;
};

struct RLC_AMD_Header_FixPartExt_Type {
	B1_Type D_C;
	B1_Type ReSeg;
	B1_Type Poll;
	RLC_FramingInfo_Type FramingInfo;
	B1_Type Extension;
	B2_Type Reserved;
	B16_Type SN_Ext;
};

struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional {
	bool d;
	struct RLC_PDU_Header_FlexPart_Type v;
};

struct RLC_AMD_HeaderExt_Type {
	struct RLC_AMD_Header_FixPartExt_Type FixPartExt;
	struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional FlexPart;
};

struct RLC_AMD_PDU_Ext_Type {
	struct RLC_AMD_HeaderExt_Type HeaderExt;
	RLC_DataFieldList_Type Data;
};

struct RLC_AMD_Header_FixPartSegExt_Type {
	B1_Type D_C;
	B1_Type ReSeg;
	B1_Type Poll;
	RLC_FramingInfo_Type FramingInfo;
	B1_Type Extension;
	B1_Type LastSegmentFlag;
	B1_Type Reserved;
	B16_Type SN_Ext;
};

struct RLC_AMD_Header_SegmentPartExt_Type {
	B16_Type SegOffset;
};

struct RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional {
	bool d;
	struct RLC_AMD_Header_SegmentPartExt_Type v;
};

struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional {
	bool d;
	struct RLC_PDU_Header_FlexPart_Type v;
};

struct RLC_AMD_HeaderSegExt_Type {
	struct RLC_AMD_Header_FixPartSegExt_Type FixPartSegExt;
	struct RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional SegmentPart;
	struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional FlexPart;
};

struct RLC_AMD_PDU_SegExt_Type {
	struct RLC_AMD_HeaderSegExt_Type HeaderSegExt;
	RLC_DataFieldList_Type Data;
};

struct RLC_Status_ACK_Type {
	B10_Type ACK_SN;
	B1_Type Extn1;
};

struct RLC_Status_SegOffset_Type {
	B15_Type Start;
	B15_Type End;
};

struct RLC_Status_SegOffset_Type_SO_Optional {
	bool d;
	struct RLC_Status_SegOffset_Type v;
};

struct RLC_Status_NACK_Type {
	B10_Type NACK_SN;
	B1_Type Extn1;
	B1_Type Extn2;
	struct RLC_Status_SegOffset_Type_SO_Optional SO;
};

struct RLC_Status_NACK_Type_RLC_Status_NACK_List_Type_Dynamic {
	size_t d;
	struct RLC_Status_NACK_Type* v;
};

typedef struct RLC_Status_NACK_Type_RLC_Status_NACK_List_Type_Dynamic RLC_Status_NACK_List_Type;

struct RLC_Status_NACK_List_Type_NackList_Optional {
	bool d;
	RLC_Status_NACK_List_Type v;
};

struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional {
	bool d;
	RLC_Status_Padding_Type v;
};

struct RLC_AM_StatusPDU_Type {
	B1_Type D_C;
	B3_Type CP_Type;
	struct RLC_Status_ACK_Type Ack;
	struct RLC_Status_NACK_List_Type_NackList_Optional NackList;
	struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional Padding;
};

struct RLC_Status_ACK_Ext_Type {
	B16_Type ACK_SN_Ext;
	B1_Type Extn1;
};

struct RLC_Status_SegOffset_Ext_Type {
	B16_Type Start;
	B16_Type End;
};

struct RLC_Status_SegOffset_Ext_Type_SO_Optional {
	bool d;
	struct RLC_Status_SegOffset_Ext_Type v;
};

struct RLC_Status_NACK_Ext_Type {
	B16_Type NACK_SN_Ext;
	B1_Type Extn1;
	B1_Type Extn2;
	struct RLC_Status_SegOffset_Ext_Type_SO_Optional SO;
};

struct RLC_Status_NACK_Ext_Type_RLC_Status_NACK_Ext_List_Type_Dynamic {
	size_t d;
	struct RLC_Status_NACK_Ext_Type* v;
};

typedef struct RLC_Status_NACK_Ext_Type_RLC_Status_NACK_Ext_List_Type_Dynamic RLC_Status_NACK_Ext_List_Type;

struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional {
	bool d;
	RLC_Status_NACK_Ext_List_Type v;
};

struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional {
	bool d;
	RLC_Status_Padding_Type v;
};

struct RLC_AM_StatusPDU_Ext_Type {
	B1_Type D_C;
	B3_Type CP_Type;
	struct RLC_Status_ACK_Ext_Type Ack_Ext;
	struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional Nack_Ext_List;
	struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional Padding;
};

enum RLC_PDU_Type_Sel {
	RLC_PDU_Type_UNBOUND_VALUE = 0,
	RLC_PDU_Type_TMD = 1,
	RLC_PDU_Type_UMD = 2,
	RLC_PDU_Type_AMD = 3,
	RLC_PDU_Type_AMD_Ext = 4,
	RLC_PDU_Type_AMD_SegExt = 5,
	RLC_PDU_Type_Status = 6,
	RLC_PDU_Type_Status_Ext = 7,
};

union RLC_PDU_Type_Value {
	RLC_TMD_PDU_Type TMD;
	struct RLC_UMD_PDU_Type UMD;
	struct RLC_AMD_PDU_Type AMD;
	struct RLC_AMD_PDU_Ext_Type AMD_Ext;
	struct RLC_AMD_PDU_SegExt_Type AMD_SegExt;
	struct RLC_AM_StatusPDU_Type Status;
	struct RLC_AM_StatusPDU_Ext_Type Status_Ext;
};

struct RLC_PDU_Type {
	enum RLC_PDU_Type_Sel d;
	union RLC_PDU_Type_Value v;
};

struct RLC_PDU_Type_RLC_PDUList_Type_Dynamic {
	size_t d;
	struct RLC_PDU_Type* v;
};

typedef struct RLC_PDU_Type_RLC_PDUList_Type_Dynamic RLC_PDUList_Type;

struct PDCP_DataPdu_LongSN_Type {
	B1_Type D_C;
	B3_Type Reserved;
	B12_Type SequenceNumber;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_ShortSN_Type {
	B1_Type D_C;
	B7_Type SequenceNumber;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_ExtSN_Type {
	B1_Type D_C;
	B15_Type SequenceNumber;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_18bitSN_Type {
	B1_Type D_C;
	B1_Type PollingBit;
	B4_Type Reserved;
	B18_Type SequenceNumber;
	PDCP_SDU_Type SDU;
};

struct PDCP_Ctrl_ROHC_FB_PDU_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B4_Type Reserved;
	OCTET_STRING ROHC_FB;
};

struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional {
	bool d;
	OCTET_STRING v;
};

struct PDCP_Ctrl_StatusReport_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B12_Type FMS;
	struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional Bitmap;
};

struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional {
	bool d;
	OCTET_STRING v;
};

struct PDCP_Ctrl_StatusReportExt_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B5_Type Reserved;
	B15_Type FMS_Ext;
	struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional Bitmap;
};

struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional {
	bool d;
	OCTET_STRING v;
};

struct PDCP_Ctrl_StatusReport_18bitSN_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B2_Type Reserved;
	B18_Type FMS_18bitSN;
	struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional Bitmap;
};

struct PDCP_Ctrl_LWA_StatusReport_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B12_Type FMS;
	B12_Type HRW;
	B12_Type NMP;
};

struct PDCP_Ctrl_LWA_StatusReportExt_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B5_Type Reserved;
	B15_Type FMS_Ext;
	B1_Type Reserved2;
	B15_Type HRW_Ext;
	B1_Type Reserved3;
	B15_Type NMP_Ext;
};

struct PDCP_Ctrl_LWA_StatusReport_18bitSN_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B2_Type Reserved;
	B18_Type FMS_18bitSN;
	B18_Type HRW_18bitSN;
	B4_Type Reserved2;
	B18_Type NMP_18bitSN;
};

struct PDCP_DataPdu_SLRB_Type {
	B3_Type SDU_Type;
	B5_Type PGK_Index;
	B16_Type PTK_Identity;
	B16_Type SequenceNumber;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_SLRB_1to1_Type {
	B3_Type SDU_Type;
	B5_Type Reserved;
	B16_Type KD_sess_ID;
	B16_Type SequenceNumber;
	PDCP_SDU_Type SDU;
	O4_Type MAC_I;
};

struct PDCP_Ctrl_LWA_EndMarker_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B12_Type LSN;
};

struct PDCP_Ctrl_LWA_EndMarkerExt_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B5_Type Reserved;
	B15_Type LSN_Ext;
};

struct PDCP_Ctrl_LWA_EndMarker_18bitSN_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B2_Type Reserved;
	B18_Type LSN_18bitSN;
};

struct PDCP_DataPdu_LongSN_UDC_Type {
	B1_Type D_C;
	B3_Type Reserved;
	B12_Type SequenceNumber;
	B1_Type FU;
	B1_Type FR;
	B2_Type Reserved2;
	B4_Type Checksum;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_ExtSN_UDC_Type {
	B1_Type D_C;
	B15_Type SequenceNumber;
	B1_Type FU;
	B1_Type FR;
	B2_Type Reserved2;
	B4_Type Checksum;
	PDCP_SDU_Type SDU;
};

struct PDCP_DataPdu_18bitSN_UDC_Type {
	B1_Type D_C;
	B1_Type PollingBit;
	B4_Type Reserved;
	B18_Type SequenceNumber;
	B1_Type FU;
	B1_Type FR;
	B2_Type Reserved2;
	B4_Type Checksum;
	PDCP_SDU_Type SDU;
};

struct PDCP_Ctrl_UDC_FB_PDU_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B1_Type FE;
	B3_Type Reserved;
};

struct PDCP_Ctrl_EHC_FB_PDU_Type {
	B1_Type D_C;
	B3_Type PDU_Type;
	B4_Type Reserved;
	O1_Type EHC_FB;
};

enum PDCP_PDU_Type_Sel {
	PDCP_PDU_Type_UNBOUND_VALUE = 0,
	PDCP_PDU_Type_DataLongSN = 1,
	PDCP_PDU_Type_DataShortSN = 2,
	PDCP_PDU_Type_DataExtSN = 3,
	PDCP_PDU_Type_Data_18bitSN = 4,
	PDCP_PDU_Type_RohcFeedback = 5,
	PDCP_PDU_Type_StatusReport = 6,
	PDCP_PDU_Type_StatusReportExt = 7,
	PDCP_PDU_Type_StatusReport_18bitSN = 8,
	PDCP_PDU_Type_LWA_StatusReport = 9,
	PDCP_PDU_Type_LWA_StatusReportExt = 10,
	PDCP_PDU_Type_LWA_StatusReport_18bitSN = 11,
	PDCP_PDU_Type_DataSLRB = 12,
	PDCP_PDU_Type_DataSLRB_1to1 = 13,
	PDCP_PDU_Type_LWA_EndMarker = 14,
	PDCP_PDU_Type_LWA_EndMarkerExt = 15,
	PDCP_PDU_Type_LWA_EndMarker_18bitSN = 16,
	PDCP_PDU_Type_DataLongSN_UDC = 17,
	PDCP_PDU_Type_DataExtSN_UDC = 18,
	PDCP_PDU_Type_Data18bitSN_UDC = 19,
	PDCP_PDU_Type_UdcFeedback = 20,
	PDCP_PDU_Type_EhcFeedback = 21,
};

union PDCP_PDU_Type_Value {
	struct PDCP_DataPdu_LongSN_Type DataLongSN;
	struct PDCP_DataPdu_ShortSN_Type DataShortSN;
	struct PDCP_DataPdu_ExtSN_Type DataExtSN;
	struct PDCP_DataPdu_18bitSN_Type Data_18bitSN;
	struct PDCP_Ctrl_ROHC_FB_PDU_Type RohcFeedback;
	struct PDCP_Ctrl_StatusReport_Type StatusReport;
	struct PDCP_Ctrl_StatusReportExt_Type StatusReportExt;
	struct PDCP_Ctrl_StatusReport_18bitSN_Type StatusReport_18bitSN;
	struct PDCP_Ctrl_LWA_StatusReport_Type LWA_StatusReport;
	struct PDCP_Ctrl_LWA_StatusReportExt_Type LWA_StatusReportExt;
	struct PDCP_Ctrl_LWA_StatusReport_18bitSN_Type LWA_StatusReport_18bitSN;
	struct PDCP_DataPdu_SLRB_Type DataSLRB;
	struct PDCP_DataPdu_SLRB_1to1_Type DataSLRB_1to1;
	struct PDCP_Ctrl_LWA_EndMarker_Type LWA_EndMarker;
	struct PDCP_Ctrl_LWA_EndMarkerExt_Type LWA_EndMarkerExt;
	struct PDCP_Ctrl_LWA_EndMarker_18bitSN_Type LWA_EndMarker_18bitSN;
	struct PDCP_DataPdu_LongSN_UDC_Type DataLongSN_UDC;
	struct PDCP_DataPdu_ExtSN_UDC_Type DataExtSN_UDC;
	struct PDCP_DataPdu_18bitSN_UDC_Type Data18bitSN_UDC;
	struct PDCP_Ctrl_UDC_FB_PDU_Type UdcFeedback;
	struct PDCP_Ctrl_EHC_FB_PDU_Type EhcFeedback;
};

struct PDCP_PDU_Type {
	enum PDCP_PDU_Type_Sel d;
	union PDCP_PDU_Type_Value v;
};

struct PDCP_PDU_Type_PDCP_PDUList_Type_Dynamic {
	size_t d;
	struct PDCP_PDU_Type* v;
};

typedef struct PDCP_PDU_Type_PDCP_PDUList_Type_Dynamic PDCP_PDUList_Type;

enum L2DataList_Type_Sel {
	L2DataList_Type_UNBOUND_VALUE = 0,
	L2DataList_Type_MacPdu = 1,
	L2DataList_Type_RlcPdu = 2,
	L2DataList_Type_PdcpPdu = 3,
	L2DataList_Type_PdcpSdu = 4,
	L2DataList_Type_NrPdcpSdu = 5,
	L2DataList_Type_RlcSdu = 6,
};

union L2DataList_Type_Value {
	MAC_PDUList_Type MacPdu;
	RLC_PDUList_Type RlcPdu;
	PDCP_PDUList_Type PdcpPdu;
	PDCP_SDUList_Type PdcpSdu;
	PDCP_SDUList_Type NrPdcpSdu;
	RLC_SDUList_Type RlcSdu;
};

struct L2DataList_Type {
	enum L2DataList_Type_Sel d;
	union L2DataList_Type_Value v;
};

struct HarqProcessAssignment_Type_HarqProcess_Optional {
	bool d;
	struct HarqProcessAssignment_Type v;
};

struct DRB_DataPerSubframe_DL_Type {
	int32_t SubframeOffset;
	struct HarqProcessAssignment_Type_HarqProcess_Optional HarqProcess;
	struct L2DataList_Type PduSduList;
};

struct DRB_DataPerSubframe_DL_Type_DRB_DataPerSubframeList_DL_Type_Dynamic {
	size_t d;
	struct DRB_DataPerSubframe_DL_Type* v;
};

typedef struct DRB_DataPerSubframe_DL_Type_DRB_DataPerSubframeList_DL_Type_Dynamic DRB_DataPerSubframeList_DL_Type;

struct L2Data_Request_Type {
	DRB_DataPerSubframeList_DL_Type SubframeDataList;
};

struct DRB_DataPerSubframe_UL_Type {
	struct L2DataList_Type PduSduList;
	int32_t NoOfTTIs;
};

struct L2Data_Indication_Type {
	struct DRB_DataPerSubframe_UL_Type SubframeData;
};

struct Null_Type_SuppressPdcchForC_RNTI_Optional {
	bool d;
	Null_Type v;
};

struct DRB_COMMON_REQ {
	struct ReqAspCommonPart_Type Common;
	struct L2Data_Request_Type U_Plane;
	struct Null_Type_SuppressPdcchForC_RNTI_Optional SuppressPdcchForC_RNTI;
};

struct DRB_COMMON_IND {
	struct IndAspCommonPart_Type Common;
	struct L2Data_Indication_Type U_Plane;
};

SIDL_END_C_INTERFACE
