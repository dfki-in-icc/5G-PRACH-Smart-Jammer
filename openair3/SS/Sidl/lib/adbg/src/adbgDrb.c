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

#include "adbgDrb.h"

static const char* adbgDrb__EUTRA_CellId_Type__ToString(EUTRA_CellId_Type v)
{
	switch(v) {
		case eutra_Cell_NonSpecific: return "eutra_Cell_NonSpecific";
		case eutra_Cell1: return "eutra_Cell1";
		case eutra_Cell2: return "eutra_Cell2";
		case eutra_Cell3: return "eutra_Cell3";
		case eutra_Cell4: return "eutra_Cell4";
		case eutra_Cell6: return "eutra_Cell6";
		case eutra_Cell10: return "eutra_Cell10";
		case eutra_Cell11: return "eutra_Cell11";
		case eutra_Cell12: return "eutra_Cell12";
		case eutra_Cell13: return "eutra_Cell13";
		case eutra_Cell14: return "eutra_Cell14";
		case eutra_Cell23: return "eutra_Cell23";
		case eutra_Cell28: return "eutra_Cell28";
		case eutra_Cell29: return "eutra_Cell29";
		case eutra_Cell30: return "eutra_Cell30";
		case eutra_Cell31: return "eutra_Cell31";
		case eutra_CellA: return "eutra_CellA";
		case eutra_CellB: return "eutra_CellB";
		case eutra_CellC: return "eutra_CellC";
		case eutra_CellD: return "eutra_CellD";
		case eutra_CellE: return "eutra_CellE";
		case eutra_CellG: return "eutra_CellG";
		case eutra_CellH: return "eutra_CellH";
		case eutra_CellI: return "eutra_CellI";
		case eutra_CellJ: return "eutra_CellJ";
		case eutra_CellK: return "eutra_CellK";
		case eutra_CellL: return "eutra_CellL";
		case eutra_CellM: return "eutra_CellM";
		default: return "Unknown";
	}
}

static void _adbgDrb__PmchLogicalChannel_Type(acpCtx_t _ctx, const struct PmchLogicalChannel_Type* p)
{
	adbgPrintLog(_ctx, "Pmch_InfoIndex := %u", (unsigned int)p->Pmch_InfoIndex);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LogicalChannelIdentity := %u", (unsigned int)p->LogicalChannelIdentity);
}

static void _adbgDrb__MRB_Identity_Type(acpCtx_t _ctx, const struct MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "Mbsfn_AreaId := %u", (unsigned int)p->Mbsfn_AreaId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PmchLogicalChannel := { ");
	_adbgDrb__PmchLogicalChannel_Type(_ctx, &p->PmchLogicalChannel);
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__MCC_PLMN_Identity_mcc_Optional(acpCtx_t _ctx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PLMN_Identity(acpCtx_t _ctx, const struct PLMN_Identity* p)
{
	adbgPrintLog(_ctx, "mcc := ");
	_adbgDrb__MCC_PLMN_Identity_mcc_Optional(_ctx, &p->mcc);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "mnc := '");
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->mnc.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__TMGI_r9_plmn_Id_r9_Value(acpCtx_t _ctx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		adbgPrintLog(_ctx, "plmn_Index_r9 := %u", (unsigned int)p->plmn_Index_r9);
		return;
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		adbgPrintLog(_ctx, "explicitValue_r9 := { ");
		_adbgDrb__PLMN_Identity(_ctx, &p->explicitValue_r9);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__TMGI_r9_plmn_Id_r9(acpCtx_t _ctx, const struct TMGI_r9_plmn_Id_r9* p)
{
	_adbgDrb__TMGI_r9_plmn_Id_r9_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__TMGI_r9(acpCtx_t _ctx, const struct TMGI_r9* p)
{
	adbgPrintLog(_ctx, "plmn_Id_r9 := { ");
	_adbgDrb__TMGI_r9_plmn_Id_r9(_ctx, &p->plmn_Id_r9);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "serviceId_r9 := '");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->serviceId_r9[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(acpCtx_t _ctx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MBMSSessionInfo_r13(acpCtx_t _ctx, const struct MBMSSessionInfo_r13* p)
{
	adbgPrintLog(_ctx, "tmgi_r13 := { ");
	_adbgDrb__TMGI_r9(_ctx, &p->tmgi_r13);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "sessionId_r13 := ");
	_adbgDrb__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(_ctx, &p->sessionId_r13);
}

static void _adbgDrb__SC_MRB_Identity_Type(acpCtx_t _ctx, const struct SC_MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "MbmsSessionInfo := { ");
	_adbgDrb__MBMSSessionInfo_r13(_ctx, &p->MbmsSessionInfo);
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RadioBearerId_Type_Value(acpCtx_t _ctx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	if (d == RadioBearerId_Type_Srb) {
		adbgPrintLog(_ctx, "Srb := %u", (unsigned int)p->Srb);
		return;
	}
	if (d == RadioBearerId_Type_Drb) {
		adbgPrintLog(_ctx, "Drb := %u", (unsigned int)p->Drb);
		return;
	}
	if (d == RadioBearerId_Type_Mrb) {
		adbgPrintLog(_ctx, "Mrb := { ");
		_adbgDrb__MRB_Identity_Type(_ctx, &p->Mrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RadioBearerId_Type_ScMrb) {
		adbgPrintLog(_ctx, "ScMrb := { ");
		_adbgDrb__SC_MRB_Identity_Type(_ctx, &p->ScMrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RadioBearerId_Type(acpCtx_t _ctx, const struct RadioBearerId_Type* p)
{
	_adbgDrb__RadioBearerId_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__QosFlow_Identification_Type(acpCtx_t _ctx, const struct QosFlow_Identification_Type* p)
{
	adbgPrintLog(_ctx, "PDU_SessionId := %d", (int)p->PDU_SessionId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "QFI := %d", (int)p->QFI);
}

static void _adbgDrb__RoutingInfo_Type_Value(acpCtx_t _ctx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		adbgPrintLog(_ctx, "RadioBearerId := { ");
		_adbgDrb__RadioBearerId_Type(_ctx, &p->RadioBearerId);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RoutingInfo_Type_QosFlow) {
		adbgPrintLog(_ctx, "QosFlow := { ");
		_adbgDrb__QosFlow_Identification_Type(_ctx, &p->QosFlow);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RoutingInfo_Type(acpCtx_t _ctx, const struct RoutingInfo_Type* p)
{
	_adbgDrb__RoutingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__SystemFrameNumberInfo_Type_Value(acpCtx_t _ctx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
{
	if (d == SystemFrameNumberInfo_Type_Number) {
		adbgPrintLog(_ctx, "Number := %u", (unsigned int)p->Number);
		return;
	}
	if (d == SystemFrameNumberInfo_Type_Any) {
		adbgPrintLog(_ctx, "Any := %s", (p->Any ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__SystemFrameNumberInfo_Type(acpCtx_t _ctx, const struct SystemFrameNumberInfo_Type* p)
{
	_adbgDrb__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__SubFrameInfo_Type_Value(acpCtx_t _ctx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
{
	if (d == SubFrameInfo_Type_Number) {
		adbgPrintLog(_ctx, "Number := %u", (unsigned int)p->Number);
		return;
	}
	if (d == SubFrameInfo_Type_Any) {
		adbgPrintLog(_ctx, "Any := %s", (p->Any ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__SubFrameInfo_Type(acpCtx_t _ctx, const struct SubFrameInfo_Type* p)
{
	_adbgDrb__SubFrameInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__HyperSystemFrameNumberInfo_Type(acpCtx_t _ctx, const HyperSystemFrameNumberInfo_Type* p)
{
	_adbgDrb__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__SlotOffset_Type_Value(acpCtx_t _ctx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
{
	if (d == SlotOffset_Type_Numerology0) {
		adbgPrintLog(_ctx, "Numerology0 := %s", (p->Numerology0 ? "true" : "false"));
		return;
	}
	if (d == SlotOffset_Type_Numerology1) {
		adbgPrintLog(_ctx, "Numerology1 := %u", (unsigned int)p->Numerology1);
		return;
	}
	if (d == SlotOffset_Type_Numerology2) {
		adbgPrintLog(_ctx, "Numerology2 := %u", (unsigned int)p->Numerology2);
		return;
	}
	if (d == SlotOffset_Type_Numerology3) {
		adbgPrintLog(_ctx, "Numerology3 := %u", (unsigned int)p->Numerology3);
		return;
	}
	if (d == SlotOffset_Type_Numerology4) {
		adbgPrintLog(_ctx, "Numerology4 := %u", (unsigned int)p->Numerology4);
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__SlotOffset_Type(acpCtx_t _ctx, const struct SlotOffset_Type* p)
{
	_adbgDrb__SlotOffset_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__SlotTimingInfo_Type_Value(acpCtx_t _ctx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	if (d == SlotTimingInfo_Type_SlotOffset) {
		adbgPrintLog(_ctx, "SlotOffset := { ");
		_adbgDrb__SlotOffset_Type(_ctx, &p->SlotOffset);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		adbgPrintLog(_ctx, "FirstSlot := %s", (p->FirstSlot ? "true" : "false"));
		return;
	}
	if (d == SlotTimingInfo_Type_Any) {
		adbgPrintLog(_ctx, "Any := %s", (p->Any ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__SlotTimingInfo_Type(acpCtx_t _ctx, const struct SlotTimingInfo_Type* p)
{
	_adbgDrb__SlotTimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__SubFrameTiming_Type(acpCtx_t _ctx, const struct SubFrameTiming_Type* p)
{
	adbgPrintLog(_ctx, "SFN := { ");
	_adbgDrb__SystemFrameNumberInfo_Type(_ctx, &p->SFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Subframe := { ");
	_adbgDrb__SubFrameInfo_Type(_ctx, &p->Subframe);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HSFN := { ");
	_adbgDrb__HyperSystemFrameNumberInfo_Type(_ctx, &p->HSFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Slot := { ");
	_adbgDrb__SlotTimingInfo_Type(_ctx, &p->Slot);
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__TimingInfo_Type_Value(acpCtx_t _ctx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	if (d == TimingInfo_Type_SubFrame) {
		adbgPrintLog(_ctx, "SubFrame := { ");
		_adbgDrb__SubFrameTiming_Type(_ctx, &p->SubFrame);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == TimingInfo_Type_Now) {
		adbgPrintLog(_ctx, "Now := %s", (p->Now ? "true" : "false"));
		return;
	}
	if (d == TimingInfo_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__TimingInfo_Type(acpCtx_t _ctx, const struct TimingInfo_Type* p)
{
	_adbgDrb__TimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__ReqAspControlInfo_Type(acpCtx_t _ctx, const struct ReqAspControlInfo_Type* p)
{
	adbgPrintLog(_ctx, "CnfFlag := %s", (p->CnfFlag ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FollowOnFlag := %s", (p->FollowOnFlag ? "true" : "false"));
}

static const char* adbgDrb__NR_CellId_Type__ToString(NR_CellId_Type v)
{
	switch(v) {
		case nr_Cell_NonSpecific: return "nr_Cell_NonSpecific";
		case nr_Cell1: return "nr_Cell1";
		case nr_Cell2: return "nr_Cell2";
		case nr_Cell3: return "nr_Cell3";
		case nr_Cell4: return "nr_Cell4";
		case nr_Cell6: return "nr_Cell6";
		case nr_Cell10: return "nr_Cell10";
		case nr_Cell11: return "nr_Cell11";
		case nr_Cell12: return "nr_Cell12";
		case nr_Cell13: return "nr_Cell13";
		case nr_Cell14: return "nr_Cell14";
		case nr_Cell23: return "nr_Cell23";
		case nr_Cell28: return "nr_Cell28";
		case nr_Cell29: return "nr_Cell29";
		case nr_Cell30: return "nr_Cell30";
		case nr_Cell31: return "nr_Cell31";
		default: return "Unknown";
	}
}

static void _adbgDrb__RlcBearerRouting_Type_Value(acpCtx_t _ctx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
{
	if (d == RlcBearerRouting_Type_EUTRA) {
		adbgPrintLog(_ctx, "EUTRA := %s (%d)", adbgDrb__EUTRA_CellId_Type__ToString(p->EUTRA), (int)p->EUTRA);
		return;
	}
	if (d == RlcBearerRouting_Type_NR) {
		adbgPrintLog(_ctx, "NR := %s (%d)", adbgDrb__NR_CellId_Type__ToString(p->NR), (int)p->NR);
		return;
	}
	if (d == RlcBearerRouting_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RlcBearerRouting_Type(acpCtx_t _ctx, const struct RlcBearerRouting_Type* p)
{
	_adbgDrb__RlcBearerRouting_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(acpCtx_t _ctx, const struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RlcBearerRouting_Type(_ctx, &p->v);
}

static void _adbgDrb__ReqAspCommonPart_Type(acpCtx_t _ctx, const struct ReqAspCommonPart_Type* p)
{
	adbgPrintLog(_ctx, "CellId := %s (%d)", adbgDrb__EUTRA_CellId_Type__ToString(p->CellId), (int)p->CellId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RoutingInfo := { ");
	_adbgDrb__RoutingInfo_Type(_ctx, &p->RoutingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingInfo := { ");
	_adbgDrb__TimingInfo_Type(_ctx, &p->TimingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ControlInfo := { ");
	_adbgDrb__ReqAspControlInfo_Type(_ctx, &p->ControlInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RlcBearerRouting := ");
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_ctx, &p->RlcBearerRouting);
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__HarqProcessAssignment_Type_Value(acpCtx_t _ctx, const union HarqProcessAssignment_Type_Value* p, enum HarqProcessAssignment_Type_Sel d)
{
	if (d == HarqProcessAssignment_Type_Id) {
		adbgPrintLog(_ctx, "Id := %u", (unsigned int)p->Id);
		return;
	}
	if (d == HarqProcessAssignment_Type_Automatic) {
		adbgPrintLog(_ctx, "Automatic := %s", (p->Automatic ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__HarqProcessAssignment_Type(acpCtx_t _ctx, const struct HarqProcessAssignment_Type* p)
{
	_adbgDrb__HarqProcessAssignment_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__HarqProcessAssignment_Type_HarqProcess_Optional(acpCtx_t _ctx, const struct HarqProcessAssignment_Type_HarqProcess_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__HarqProcessAssignment_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_PDU_Length_Type(acpCtx_t _ctx, const struct MAC_PDU_Length_Type* p)
{
	adbgPrintLog(_ctx, "Format := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Format[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i4 = 0; i4 < p->Value.d; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Value.v[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_PDU_Length_Type_Length_Optional(acpCtx_t _ctx, const struct MAC_PDU_Length_Type_Length_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_PDU_Length_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_PDU_SubHeader_Type(acpCtx_t _ctx, const struct MAC_PDU_SubHeader_Type* p)
{
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i4 = 0; i4 < 2; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LCID := '");
	for (size_t i4 = 0; i4 < 5; i4++) {
		adbgPrintLog(_ctx, "%02X", p->LCID[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Length := ");
	if (p->Length.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_PDU_Length_Type_Length_Optional(_ctx, &p->Length);
	if (p->Length.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__MAC_CTRL_ShortBSR_Type(acpCtx_t _ctx, const struct MAC_CTRL_ShortBSR_Type* p)
{
	adbgPrintLog(_ctx, "LCG := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LCG[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_ShortBSR_Type_ShortBSR_Optional(acpCtx_t _ctx, const struct MAC_CTRL_ShortBSR_Type_ShortBSR_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_ShortBSR_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_CTRL_LongBSR_Type(acpCtx_t _ctx, const struct MAC_CTRL_LongBSR_Type* p)
{
	adbgPrintLog(_ctx, "Value_LCG1 := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value_LCG1[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG2 := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value_LCG2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG3 := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value_LCG3[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG4 := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value_LCG4[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_LongBSR_Type_LongBSR_Optional(acpCtx_t _ctx, const struct MAC_CTRL_LongBSR_Type_LongBSR_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_LongBSR_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_CTRL_C_RNTI_Type_C_RNTI_Optional(acpCtx_t _ctx, const struct MAC_CTRL_C_RNTI_Type_C_RNTI_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(acpCtx_t _ctx, const struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(acpCtx_t _ctx, const struct MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < 8; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_PowerHeadRoom_Type(acpCtx_t _ctx, const struct MAC_CTRL_PowerHeadRoom_Type* p)
{
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i3 = 0; i3 < 6; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(acpCtx_t _ctx, const struct MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_PowerHeadRoom_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_CTRL_ScellActDeact_Type(acpCtx_t _ctx, const MAC_CTRL_ScellActDeact_Type* p)
{
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i3 = 0; i3 < 7; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(acpCtx_t _ctx, const struct MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_ScellActDeact_Type(_ctx, &p->v);
}

static void _adbgDrb__ScellBitMap_Type(acpCtx_t _ctx, const struct ScellBitMap_Type* p)
{
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i3 = 0; i3 < 7; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Value[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__B2_Type_Reserved_Optional(acpCtx_t _ctx, const struct B2_Type_Reserved_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i4 = 0; i4 < 2; i4++) {
		adbgPrintLog(_ctx, "%02X", p->v[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__B6_Type_PCMaxc_Optional(acpCtx_t _ctx, const struct B6_Type_PCMaxc_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i4 = 0; i4 < 6; i4++) {
		adbgPrintLog(_ctx, "%02X", p->v[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PH_Record_Type(acpCtx_t _ctx, const struct PH_Record_Type* p)
{
	adbgPrintLog(_ctx, "P_Bit := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->P_Bit[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "V_Bit := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->V_Bit[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Valve := '");
	for (size_t i4 = 0; i4 < 6; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Valve[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := ");
	_adbgDrb__B2_Type_Reserved_Optional(_ctx, &p->Reserved);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PCMaxc := ");
	_adbgDrb__B6_Type_PCMaxc_Optional(_ctx, &p->PCMaxc);
}

static void _adbgDrb__MAC_CTRL_ExtPowerHeadRoom_Type(acpCtx_t _ctx, const struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	adbgPrintLog(_ctx, "EPH_Octet1 := { ");
	_adbgDrb__ScellBitMap_Type(_ctx, &p->EPH_Octet1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PH_RecordList := { ");
	for (size_t i3 = 0; i3 < p->PH_RecordList.d; i3++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgDrb__PH_Record_Type(_ctx, &p->PH_RecordList.v[i3]);
		adbgPrintLog(_ctx, " }");
		if (i3 != p->PH_RecordList.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(acpCtx_t _ctx, const struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_ExtPowerHeadRoom_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_CTRL_DC_PowerHeadRoom_Type(acpCtx_t _ctx, const struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	adbgPrintLog(_ctx, "DC_PH_Octet1 := { ");
	_adbgDrb__ScellBitMap_Type(_ctx, &p->DC_PH_Octet1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "DC_PH_RecordList := { ");
	for (size_t i3 = 0; i3 < p->DC_PH_RecordList.d; i3++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgDrb__PH_Record_Type(_ctx, &p->DC_PH_RecordList.v[i3]);
		adbgPrintLog(_ctx, " }");
		if (i3 != p->DC_PH_RecordList.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(acpCtx_t _ctx, const struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_DC_PowerHeadRoom_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_CTRL_ElementList_Type(acpCtx_t _ctx, const struct MAC_CTRL_ElementList_Type* p)
{
	adbgPrintLog(_ctx, "ShortBSR := ");
	if (p->ShortBSR.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_ShortBSR_Type_ShortBSR_Optional(_ctx, &p->ShortBSR);
	if (p->ShortBSR.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LongBSR := ");
	if (p->LongBSR.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_LongBSR_Type_LongBSR_Optional(_ctx, &p->LongBSR);
	if (p->LongBSR.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "C_RNTI := ");
	_adbgDrb__MAC_CTRL_C_RNTI_Type_C_RNTI_Optional(_ctx, &p->C_RNTI);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ContentionResolutionID := ");
	if (p->ContentionResolutionID.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(_ctx, &p->ContentionResolutionID);
	if (p->ContentionResolutionID.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingAdvance := ");
	_adbgDrb__MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(_ctx, &p->TimingAdvance);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PowerHeadRoom := ");
	if (p->PowerHeadRoom.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(_ctx, &p->PowerHeadRoom);
	if (p->PowerHeadRoom.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ScellActDeact := ");
	if (p->ScellActDeact.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(_ctx, &p->ScellActDeact);
	if (p->ScellActDeact.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ExtPowerHeadRoom := ");
	if (p->ExtPowerHeadRoom.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(_ctx, &p->ExtPowerHeadRoom);
	if (p->ExtPowerHeadRoom.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "DC_PowerHeadRoom := ");
	if (p->DC_PowerHeadRoom.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(_ctx, &p->DC_PowerHeadRoom);
	if (p->DC_PowerHeadRoom.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__MAC_CTRL_ElementList_Type_CtrlElementList_Optional(acpCtx_t _ctx, const struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__MAC_CTRL_ElementList_Type(_ctx, &p->v);
}

static void _adbgDrb__MAC_SDUList_Type_SduList_Optional(acpCtx_t _ctx, const struct MAC_SDUList_Type_SduList_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->v.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->v.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgDrb__OCTET_STRING_Padding_Optional(acpCtx_t _ctx, const struct OCTET_STRING_Padding_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__MAC_PDU_Type(acpCtx_t _ctx, const struct MAC_PDU_Type* p)
{
	adbgPrintLog(_ctx, "Header := { ");
	for (size_t i3 = 0; i3 < p->Header.d; i3++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgDrb__MAC_PDU_SubHeader_Type(_ctx, &p->Header.v[i3]);
		adbgPrintLog(_ctx, " }");
		if (i3 != p->Header.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "CtrlElementList := ");
	if (p->CtrlElementList.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_CTRL_ElementList_Type_CtrlElementList_Optional(_ctx, &p->CtrlElementList);
	if (p->CtrlElementList.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SduList := ");
	if (p->SduList.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__MAC_SDUList_Type_SduList_Optional(_ctx, &p->SduList);
	if (p->SduList.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Padding := ");
	if (p->Padding.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__OCTET_STRING_Padding_Optional(_ctx, &p->Padding);
	if (p->Padding.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_UMD_Header_FixPartShortSN_Type(acpCtx_t _ctx, const struct RLC_UMD_Header_FixPartShortSN_Type* p)
{
	adbgPrintLog(_ctx, "FramingInfo := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FramingInfo[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_LengthIndicator_LI11_Type(acpCtx_t _ctx, const struct RLC_LengthIndicator_LI11_Type* p)
{
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LengthIndicator := '");
	for (size_t i4 = 0; i4 < 11; i4++) {
		adbgPrintLog(_ctx, "%02X", p->LengthIndicator[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_LengthIndicator_LI15_Type(acpCtx_t _ctx, const struct RLC_LengthIndicator_LI15_Type* p)
{
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LengthIndicator := '");
	for (size_t i4 = 0; i4 < 15; i4++) {
		adbgPrintLog(_ctx, "%02X", p->LengthIndicator[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_LI_List_Type_Value(acpCtx_t _ctx, const union RLC_LI_List_Type_Value* p, enum RLC_LI_List_Type_Sel d)
{
	if (d == RLC_LI_List_Type_LI11) {
		adbgPrintLog(_ctx, "LI11 := { ");
		for (size_t i3 = 0; i3 < p->LI11.d; i3++) {
			adbgPrintLog(_ctx, "{ ");
			_adbgDrb__RLC_LengthIndicator_LI11_Type(_ctx, &p->LI11.v[i3]);
			adbgPrintLog(_ctx, " }");
			if (i3 != p->LI11.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_LI_List_Type_LI15) {
		adbgPrintLog(_ctx, "LI15 := { ");
		for (size_t i3 = 0; i3 < p->LI15.d; i3++) {
			adbgPrintLog(_ctx, "{ ");
			_adbgDrb__RLC_LengthIndicator_LI15_Type(_ctx, &p->LI15.v[i3]);
			adbgPrintLog(_ctx, " }");
			if (i3 != p->LI15.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RLC_LI_List_Type(acpCtx_t _ctx, const struct RLC_LI_List_Type* p)
{
	_adbgDrb__RLC_LI_List_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__B4_Type_Padding_Optional(acpCtx_t _ctx, const struct B4_Type_Padding_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type* p)
{
	adbgPrintLog(_ctx, "LengthIndicator := { ");
	_adbgDrb__RLC_LI_List_Type(_ctx, &p->LengthIndicator);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Padding := ");
	_adbgDrb__B4_Type_Padding_Optional(_ctx, &p->Padding);
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_PDU_Header_FlexPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_UMD_HeaderShortSN_Type(acpCtx_t _ctx, const struct RLC_UMD_HeaderShortSN_Type* p)
{
	adbgPrintLog(_ctx, "FixPart := { ");
	_adbgDrb__RLC_UMD_Header_FixPartShortSN_Type(_ctx, &p->FixPart);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FlexPart := ");
	if (p->FlexPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(_ctx, &p->FlexPart);
	if (p->FlexPart.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_UMD_PDU_ShortSN_Type(acpCtx_t _ctx, const struct RLC_UMD_PDU_ShortSN_Type* p)
{
	adbgPrintLog(_ctx, "Header := { ");
	_adbgDrb__RLC_UMD_HeaderShortSN_Type(_ctx, &p->Header);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Data := { ");
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->Data.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->Data.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RLC_UMD_Header_FixPartLongSN_Type(acpCtx_t _ctx, const struct RLC_UMD_Header_FixPartLongSN_Type* p)
{
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FramingInfo := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FramingInfo[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 10; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_PDU_Header_FlexPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_UMD_HeaderLongSN_Type(acpCtx_t _ctx, const struct RLC_UMD_HeaderLongSN_Type* p)
{
	adbgPrintLog(_ctx, "FixPart := { ");
	_adbgDrb__RLC_UMD_Header_FixPartLongSN_Type(_ctx, &p->FixPart);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FlexPart := ");
	if (p->FlexPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(_ctx, &p->FlexPart);
	if (p->FlexPart.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_UMD_PDU_LongSN_Type(acpCtx_t _ctx, const struct RLC_UMD_PDU_LongSN_Type* p)
{
	adbgPrintLog(_ctx, "Header := { ");
	_adbgDrb__RLC_UMD_HeaderLongSN_Type(_ctx, &p->Header);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Data := { ");
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->Data.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->Data.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RLC_UMD_PDU_Type_Value(acpCtx_t _ctx, const union RLC_UMD_PDU_Type_Value* p, enum RLC_UMD_PDU_Type_Sel d)
{
	if (d == RLC_UMD_PDU_Type_ShortSN) {
		adbgPrintLog(_ctx, "ShortSN := { ");
		_adbgDrb__RLC_UMD_PDU_ShortSN_Type(_ctx, &p->ShortSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_UMD_PDU_Type_LongSN) {
		adbgPrintLog(_ctx, "LongSN := { ");
		_adbgDrb__RLC_UMD_PDU_LongSN_Type(_ctx, &p->LongSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RLC_UMD_PDU_Type(acpCtx_t _ctx, const struct RLC_UMD_PDU_Type* p)
{
	_adbgDrb__RLC_UMD_PDU_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__RLC_AMD_Header_FixPart_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_FixPart_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ReSeg := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ReSeg[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Poll := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Poll[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FramingInfo := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FramingInfo[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SN := '");
	for (size_t i3 = 0; i3 < 10; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AMD_Header_SegmentPart_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_SegmentPart_Type* p)
{
	adbgPrintLog(_ctx, "LastSegmentFlag := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LastSegmentFlag[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SegOffset := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SegOffset[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(acpCtx_t _ctx, const struct RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_AMD_Header_SegmentPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_PDU_Header_FlexPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_AMD_Header_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_Type* p)
{
	adbgPrintLog(_ctx, "FixPart := { ");
	_adbgDrb__RLC_AMD_Header_FixPart_Type(_ctx, &p->FixPart);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SegmentPart := ");
	if (p->SegmentPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(_ctx, &p->SegmentPart);
	if (p->SegmentPart.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FlexPart := ");
	if (p->FlexPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(_ctx, &p->FlexPart);
	if (p->FlexPart.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_AMD_PDU_Type(acpCtx_t _ctx, const struct RLC_AMD_PDU_Type* p)
{
	adbgPrintLog(_ctx, "Header := { ");
	_adbgDrb__RLC_AMD_Header_Type(_ctx, &p->Header);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Data := { ");
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->Data.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->Data.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RLC_AMD_Header_FixPartExt_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_FixPartExt_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ReSeg := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ReSeg[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Poll := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Poll[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FramingInfo := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FramingInfo[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SN_Ext := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SN_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_PDU_Header_FlexPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_AMD_HeaderExt_Type(acpCtx_t _ctx, const struct RLC_AMD_HeaderExt_Type* p)
{
	adbgPrintLog(_ctx, "FixPartExt := { ");
	_adbgDrb__RLC_AMD_Header_FixPartExt_Type(_ctx, &p->FixPartExt);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FlexPart := ");
	if (p->FlexPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(_ctx, &p->FlexPart);
	if (p->FlexPart.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_AMD_PDU_Ext_Type(acpCtx_t _ctx, const struct RLC_AMD_PDU_Ext_Type* p)
{
	adbgPrintLog(_ctx, "HeaderExt := { ");
	_adbgDrb__RLC_AMD_HeaderExt_Type(_ctx, &p->HeaderExt);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Data := { ");
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->Data.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->Data.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RLC_AMD_Header_FixPartSegExt_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_FixPartSegExt_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ReSeg := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ReSeg[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Poll := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Poll[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FramingInfo := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FramingInfo[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extension := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extension[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LastSegmentFlag := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LastSegmentFlag[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SN_Ext := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SN_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AMD_Header_SegmentPartExt_Type(acpCtx_t _ctx, const struct RLC_AMD_Header_SegmentPartExt_Type* p)
{
	adbgPrintLog(_ctx, "SegOffset := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SegOffset[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(acpCtx_t _ctx, const struct RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_AMD_Header_SegmentPartExt_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_PDU_Header_FlexPart_Type_FlexPart_Optional(acpCtx_t _ctx, const struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_PDU_Header_FlexPart_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_AMD_HeaderSegExt_Type(acpCtx_t _ctx, const struct RLC_AMD_HeaderSegExt_Type* p)
{
	adbgPrintLog(_ctx, "FixPartSegExt := { ");
	_adbgDrb__RLC_AMD_Header_FixPartSegExt_Type(_ctx, &p->FixPartSegExt);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SegmentPart := ");
	if (p->SegmentPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(_ctx, &p->SegmentPart);
	if (p->SegmentPart.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FlexPart := ");
	if (p->FlexPart.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_PDU_Header_FlexPart_Type_FlexPart_Optional(_ctx, &p->FlexPart);
	if (p->FlexPart.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_AMD_PDU_SegExt_Type(acpCtx_t _ctx, const struct RLC_AMD_PDU_SegExt_Type* p)
{
	adbgPrintLog(_ctx, "HeaderSegExt := { ");
	_adbgDrb__RLC_AMD_HeaderSegExt_Type(_ctx, &p->HeaderSegExt);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Data := { ");
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		adbgPrintLog(_ctx, "'");
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			adbgPrintLog(_ctx, "%02X", p->Data.v[i3].v[i4]);
		}
		adbgPrintLog(_ctx, "'O");
		if (i3 != p->Data.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__RLC_Status_ACK_Type(acpCtx_t _ctx, const struct RLC_Status_ACK_Type* p)
{
	adbgPrintLog(_ctx, "ACK_SN := '");
	for (size_t i3 = 0; i3 < 10; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ACK_SN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn1 := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extn1[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_Status_SegOffset_Type(acpCtx_t _ctx, const struct RLC_Status_SegOffset_Type* p)
{
	adbgPrintLog(_ctx, "Start := '");
	for (size_t i4 = 0; i4 < 15; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Start[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "End := '");
	for (size_t i4 = 0; i4 < 15; i4++) {
		adbgPrintLog(_ctx, "%02X", p->End[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_Status_SegOffset_Type_SO_Optional(acpCtx_t _ctx, const struct RLC_Status_SegOffset_Type_SO_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_Status_SegOffset_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_Status_NACK_Type(acpCtx_t _ctx, const struct RLC_Status_NACK_Type* p)
{
	adbgPrintLog(_ctx, "NACK_SN := '");
	for (size_t i4 = 0; i4 < 10; i4++) {
		adbgPrintLog(_ctx, "%02X", p->NACK_SN[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn1 := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extn1[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn2 := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extn2[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SO := ");
	if (p->SO.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_SegOffset_Type_SO_Optional(_ctx, &p->SO);
	if (p->SO.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_Status_NACK_List_Type_NackList_Optional(acpCtx_t _ctx, const struct RLC_Status_NACK_List_Type_NackList_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_adbgDrb__RLC_Status_NACK_Type(_ctx, &p->v.v[i3]);
		if (i3 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgDrb__RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(acpCtx_t _ctx, const struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AM_StatusPDU_Type(acpCtx_t _ctx, const struct RLC_AM_StatusPDU_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "CP_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->CP_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Ack := { ");
	_adbgDrb__RLC_Status_ACK_Type(_ctx, &p->Ack);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "NackList := ");
	if (p->NackList.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_NACK_List_Type_NackList_Optional(_ctx, &p->NackList);
	if (p->NackList.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Padding := ");
	if (p->Padding.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(_ctx, &p->Padding);
	if (p->Padding.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_Status_ACK_Ext_Type(acpCtx_t _ctx, const struct RLC_Status_ACK_Ext_Type* p)
{
	adbgPrintLog(_ctx, "ACK_SN_Ext := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ACK_SN_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn1 := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Extn1[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_Status_SegOffset_Ext_Type(acpCtx_t _ctx, const struct RLC_Status_SegOffset_Ext_Type* p)
{
	adbgPrintLog(_ctx, "Start := '");
	for (size_t i4 = 0; i4 < 16; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Start[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "End := '");
	for (size_t i4 = 0; i4 < 16; i4++) {
		adbgPrintLog(_ctx, "%02X", p->End[i4]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_Status_SegOffset_Ext_Type_SO_Optional(acpCtx_t _ctx, const struct RLC_Status_SegOffset_Ext_Type_SO_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RLC_Status_SegOffset_Ext_Type(_ctx, &p->v);
}

static void _adbgDrb__RLC_Status_NACK_Ext_Type(acpCtx_t _ctx, const struct RLC_Status_NACK_Ext_Type* p)
{
	adbgPrintLog(_ctx, "NACK_SN_Ext := '");
	for (size_t i4 = 0; i4 < 16; i4++) {
		adbgPrintLog(_ctx, "%02X", p->NACK_SN_Ext[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn1 := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extn1[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Extn2 := '");
	for (size_t i4 = 0; i4 < 1; i4++) {
		adbgPrintLog(_ctx, "%02X", p->Extn2[i4]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SO := ");
	if (p->SO.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_SegOffset_Ext_Type_SO_Optional(_ctx, &p->SO);
	if (p->SO.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(acpCtx_t _ctx, const struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_adbgDrb__RLC_Status_NACK_Ext_Type(_ctx, &p->v.v[i3]);
		if (i3 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgDrb__RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(acpCtx_t _ctx, const struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__RLC_AM_StatusPDU_Ext_Type(acpCtx_t _ctx, const struct RLC_AM_StatusPDU_Ext_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "CP_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->CP_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Ack_Ext := { ");
	_adbgDrb__RLC_Status_ACK_Ext_Type(_ctx, &p->Ack_Ext);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Nack_Ext_List := ");
	if (p->Nack_Ext_List.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(_ctx, &p->Nack_Ext_List);
	if (p->Nack_Ext_List.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Padding := ");
	if (p->Padding.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(_ctx, &p->Padding);
	if (p->Padding.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__RLC_PDU_Type_Value(acpCtx_t _ctx, const union RLC_PDU_Type_Value* p, enum RLC_PDU_Type_Sel d)
{
	if (d == RLC_PDU_Type_TMD) {
		adbgPrintLog(_ctx, "TMD := '");
		for (size_t i3 = 0; i3 < p->TMD.d; i3++) {
			adbgPrintLog(_ctx, "%02X", p->TMD.v[i3]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	if (d == RLC_PDU_Type_UMD) {
		adbgPrintLog(_ctx, "UMD := { ");
		_adbgDrb__RLC_UMD_PDU_Type(_ctx, &p->UMD);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_PDU_Type_AMD) {
		adbgPrintLog(_ctx, "AMD := { ");
		_adbgDrb__RLC_AMD_PDU_Type(_ctx, &p->AMD);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_PDU_Type_AMD_Ext) {
		adbgPrintLog(_ctx, "AMD_Ext := { ");
		_adbgDrb__RLC_AMD_PDU_Ext_Type(_ctx, &p->AMD_Ext);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_PDU_Type_AMD_SegExt) {
		adbgPrintLog(_ctx, "AMD_SegExt := { ");
		_adbgDrb__RLC_AMD_PDU_SegExt_Type(_ctx, &p->AMD_SegExt);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_PDU_Type_Status) {
		adbgPrintLog(_ctx, "Status := { ");
		_adbgDrb__RLC_AM_StatusPDU_Type(_ctx, &p->Status);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RLC_PDU_Type_Status_Ext) {
		adbgPrintLog(_ctx, "Status_Ext := { ");
		_adbgDrb__RLC_AM_StatusPDU_Ext_Type(_ctx, &p->Status_Ext);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__RLC_PDU_Type(acpCtx_t _ctx, const struct RLC_PDU_Type* p)
{
	_adbgDrb__RLC_PDU_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__PDCP_DataPdu_LongSN_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_LongSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_ShortSN_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_ShortSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 7; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_ExtSN_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_ExtSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_18bitSN_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_18bitSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PollingBit := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PollingBit[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_ROHC_FB_PDU_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_ROHC_FB_PDU_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ROHC_FB := '");
	for (size_t i3 = 0; i3 < p->ROHC_FB.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->ROHC_FB.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(acpCtx_t _ctx, const struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_StatusReport_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_StatusReport_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Bitmap := ");
	if (p->Bitmap.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(_ctx, &p->Bitmap);
	if (p->Bitmap.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(acpCtx_t _ctx, const struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_StatusReportExt_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_StatusReportExt_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS_Ext := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Bitmap := ");
	if (p->Bitmap.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(_ctx, &p->Bitmap);
	if (p->Bitmap.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(acpCtx_t _ctx, const struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_StatusReport_18bitSN_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_StatusReport_18bitSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS_18bitSN := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS_18bitSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Bitmap := ");
	if (p->Bitmap.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(_ctx, &p->Bitmap);
	if (p->Bitmap.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__PDCP_Ctrl_LWA_StatusReport_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_StatusReport_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HRW := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->HRW[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "NMP := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->NMP[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_LWA_StatusReportExt_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_StatusReportExt_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS_Ext := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved2 := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HRW_Ext := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->HRW_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved3 := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved3[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "NMP_Ext := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->NMP_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_LWA_StatusReport_18bitSN_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_StatusReport_18bitSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FMS_18bitSN := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FMS_18bitSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HRW_18bitSN := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->HRW_18bitSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved2 := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "NMP_18bitSN := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->NMP_18bitSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_SLRB_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_SLRB_Type* p)
{
	adbgPrintLog(_ctx, "SDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PGK_Index := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PGK_Index[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PTK_Identity := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PTK_Identity[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_SLRB_1to1_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_SLRB_1to1_Type* p)
{
	adbgPrintLog(_ctx, "SDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "KD_sess_ID := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->KD_sess_ID[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 16; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "MAC_I := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->MAC_I[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_LWA_EndMarker_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_EndMarker_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LSN := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_LWA_EndMarkerExt_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_EndMarkerExt_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 5; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LSN_Ext := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LSN_Ext[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_LWA_EndMarker_18bitSN_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_LWA_EndMarker_18bitSN_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LSN_18bitSN := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->LSN_18bitSN[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_LongSN_UDC_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_LongSN_UDC_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 12; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FU := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FU[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FR := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FR[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved2 := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Checksum := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Checksum[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_ExtSN_UDC_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_ExtSN_UDC_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 15; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FU := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FU[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FR := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FR[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved2 := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Checksum := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Checksum[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_DataPdu_18bitSN_UDC_Type(acpCtx_t _ctx, const struct PDCP_DataPdu_18bitSN_UDC_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PollingBit := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PollingBit[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SequenceNumber := '");
	for (size_t i3 = 0; i3 < 18; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SequenceNumber[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FU := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FU[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FR := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FR[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved2 := '");
	for (size_t i3 = 0; i3 < 2; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved2[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Checksum := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Checksum[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SDU := '");
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		adbgPrintLog(_ctx, "%02X", p->SDU.v[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_UDC_FB_PDU_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_UDC_FB_PDU_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FE := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->FE[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_Ctrl_EHC_FB_PDU_Type(acpCtx_t _ctx, const struct PDCP_Ctrl_EHC_FB_PDU_Type* p)
{
	adbgPrintLog(_ctx, "D_C := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->D_C[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PDU_Type := '");
	for (size_t i3 = 0; i3 < 3; i3++) {
		adbgPrintLog(_ctx, "%02X", p->PDU_Type[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i3 = 0; i3 < 4; i3++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i3]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "EHC_FB := '");
	for (size_t i3 = 0; i3 < 1; i3++) {
		adbgPrintLog(_ctx, "%02X", p->EHC_FB[i3]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgDrb__PDCP_PDU_Type_Value(acpCtx_t _ctx, const union PDCP_PDU_Type_Value* p, enum PDCP_PDU_Type_Sel d)
{
	if (d == PDCP_PDU_Type_DataLongSN) {
		adbgPrintLog(_ctx, "DataLongSN := { ");
		_adbgDrb__PDCP_DataPdu_LongSN_Type(_ctx, &p->DataLongSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataShortSN) {
		adbgPrintLog(_ctx, "DataShortSN := { ");
		_adbgDrb__PDCP_DataPdu_ShortSN_Type(_ctx, &p->DataShortSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataExtSN) {
		adbgPrintLog(_ctx, "DataExtSN := { ");
		_adbgDrb__PDCP_DataPdu_ExtSN_Type(_ctx, &p->DataExtSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_Data_18bitSN) {
		adbgPrintLog(_ctx, "Data_18bitSN := { ");
		_adbgDrb__PDCP_DataPdu_18bitSN_Type(_ctx, &p->Data_18bitSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_RohcFeedback) {
		adbgPrintLog(_ctx, "RohcFeedback := { ");
		_adbgDrb__PDCP_Ctrl_ROHC_FB_PDU_Type(_ctx, &p->RohcFeedback);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_StatusReport) {
		adbgPrintLog(_ctx, "StatusReport := { ");
		_adbgDrb__PDCP_Ctrl_StatusReport_Type(_ctx, &p->StatusReport);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_StatusReportExt) {
		adbgPrintLog(_ctx, "StatusReportExt := { ");
		_adbgDrb__PDCP_Ctrl_StatusReportExt_Type(_ctx, &p->StatusReportExt);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_StatusReport_18bitSN) {
		adbgPrintLog(_ctx, "StatusReport_18bitSN := { ");
		_adbgDrb__PDCP_Ctrl_StatusReport_18bitSN_Type(_ctx, &p->StatusReport_18bitSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport) {
		adbgPrintLog(_ctx, "LWA_StatusReport := { ");
		_adbgDrb__PDCP_Ctrl_LWA_StatusReport_Type(_ctx, &p->LWA_StatusReport);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_StatusReportExt) {
		adbgPrintLog(_ctx, "LWA_StatusReportExt := { ");
		_adbgDrb__PDCP_Ctrl_LWA_StatusReportExt_Type(_ctx, &p->LWA_StatusReportExt);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport_18bitSN) {
		adbgPrintLog(_ctx, "LWA_StatusReport_18bitSN := { ");
		_adbgDrb__PDCP_Ctrl_LWA_StatusReport_18bitSN_Type(_ctx, &p->LWA_StatusReport_18bitSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataSLRB) {
		adbgPrintLog(_ctx, "DataSLRB := { ");
		_adbgDrb__PDCP_DataPdu_SLRB_Type(_ctx, &p->DataSLRB);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataSLRB_1to1) {
		adbgPrintLog(_ctx, "DataSLRB_1to1 := { ");
		_adbgDrb__PDCP_DataPdu_SLRB_1to1_Type(_ctx, &p->DataSLRB_1to1);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker) {
		adbgPrintLog(_ctx, "LWA_EndMarker := { ");
		_adbgDrb__PDCP_Ctrl_LWA_EndMarker_Type(_ctx, &p->LWA_EndMarker);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_EndMarkerExt) {
		adbgPrintLog(_ctx, "LWA_EndMarkerExt := { ");
		_adbgDrb__PDCP_Ctrl_LWA_EndMarkerExt_Type(_ctx, &p->LWA_EndMarkerExt);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker_18bitSN) {
		adbgPrintLog(_ctx, "LWA_EndMarker_18bitSN := { ");
		_adbgDrb__PDCP_Ctrl_LWA_EndMarker_18bitSN_Type(_ctx, &p->LWA_EndMarker_18bitSN);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataLongSN_UDC) {
		adbgPrintLog(_ctx, "DataLongSN_UDC := { ");
		_adbgDrb__PDCP_DataPdu_LongSN_UDC_Type(_ctx, &p->DataLongSN_UDC);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_DataExtSN_UDC) {
		adbgPrintLog(_ctx, "DataExtSN_UDC := { ");
		_adbgDrb__PDCP_DataPdu_ExtSN_UDC_Type(_ctx, &p->DataExtSN_UDC);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_Data18bitSN_UDC) {
		adbgPrintLog(_ctx, "Data18bitSN_UDC := { ");
		_adbgDrb__PDCP_DataPdu_18bitSN_UDC_Type(_ctx, &p->Data18bitSN_UDC);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_UdcFeedback) {
		adbgPrintLog(_ctx, "UdcFeedback := { ");
		_adbgDrb__PDCP_Ctrl_UDC_FB_PDU_Type(_ctx, &p->UdcFeedback);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == PDCP_PDU_Type_EhcFeedback) {
		adbgPrintLog(_ctx, "EhcFeedback := { ");
		_adbgDrb__PDCP_Ctrl_EHC_FB_PDU_Type(_ctx, &p->EhcFeedback);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__PDCP_PDU_Type(acpCtx_t _ctx, const struct PDCP_PDU_Type* p)
{
	_adbgDrb__PDCP_PDU_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__L2DataList_Type_Value(acpCtx_t _ctx, const union L2DataList_Type_Value* p, enum L2DataList_Type_Sel d)
{
	if (d == L2DataList_Type_MacPdu) {
		adbgPrintLog(_ctx, "MacPdu := { ");
		for (size_t i2 = 0; i2 < p->MacPdu.d; i2++) {
			adbgPrintLog(_ctx, "{ ");
			_adbgDrb__MAC_PDU_Type(_ctx, &p->MacPdu.v[i2]);
			adbgPrintLog(_ctx, " }");
			if (i2 != p->MacPdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == L2DataList_Type_RlcPdu) {
		adbgPrintLog(_ctx, "RlcPdu := { ");
		for (size_t i2 = 0; i2 < p->RlcPdu.d; i2++) {
			adbgPrintLog(_ctx, "{ ");
			_adbgDrb__RLC_PDU_Type(_ctx, &p->RlcPdu.v[i2]);
			adbgPrintLog(_ctx, " }");
			if (i2 != p->RlcPdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == L2DataList_Type_PdcpPdu) {
		adbgPrintLog(_ctx, "PdcpPdu := { ");
		for (size_t i2 = 0; i2 < p->PdcpPdu.d; i2++) {
			adbgPrintLog(_ctx, "{ ");
			_adbgDrb__PDCP_PDU_Type(_ctx, &p->PdcpPdu.v[i2]);
			adbgPrintLog(_ctx, " }");
			if (i2 != p->PdcpPdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == L2DataList_Type_PdcpSdu) {
		adbgPrintLog(_ctx, "PdcpSdu := { ");
		for (size_t i2 = 0; i2 < p->PdcpSdu.d; i2++) {
			adbgPrintLog(_ctx, "'");
			for (size_t i3 = 0; i3 < p->PdcpSdu.v[i2].d; i3++) {
				adbgPrintLog(_ctx, "%02X", p->PdcpSdu.v[i2].v[i3]);
			}
			adbgPrintLog(_ctx, "'O");
			if (i2 != p->PdcpSdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == L2DataList_Type_NrPdcpSdu) {
		adbgPrintLog(_ctx, "NrPdcpSdu := { ");
		for (size_t i2 = 0; i2 < p->NrPdcpSdu.d; i2++) {
			adbgPrintLog(_ctx, "'");
			for (size_t i3 = 0; i3 < p->NrPdcpSdu.v[i2].d; i3++) {
				adbgPrintLog(_ctx, "%02X", p->NrPdcpSdu.v[i2].v[i3]);
			}
			adbgPrintLog(_ctx, "'O");
			if (i2 != p->NrPdcpSdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == L2DataList_Type_RlcSdu) {
		adbgPrintLog(_ctx, "RlcSdu := { ");
		for (size_t i2 = 0; i2 < p->RlcSdu.d; i2++) {
			adbgPrintLog(_ctx, "'");
			for (size_t i3 = 0; i3 < p->RlcSdu.v[i2].d; i3++) {
				adbgPrintLog(_ctx, "%02X", p->RlcSdu.v[i2].v[i3]);
			}
			adbgPrintLog(_ctx, "'O");
			if (i2 != p->RlcSdu.d - 1) { adbgPrintLog(_ctx, ", "); }
		}
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__L2DataList_Type(acpCtx_t _ctx, const struct L2DataList_Type* p)
{
	_adbgDrb__L2DataList_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__DRB_DataPerSubframe_DL_Type(acpCtx_t _ctx, const struct DRB_DataPerSubframe_DL_Type* p)
{
	adbgPrintLog(_ctx, "SubframeOffset := %d", (int)p->SubframeOffset);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HarqProcess := ");
	if (p->HarqProcess.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__HarqProcessAssignment_Type_HarqProcess_Optional(_ctx, &p->HarqProcess);
	if (p->HarqProcess.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PduSduList := { ");
	_adbgDrb__L2DataList_Type(_ctx, &p->PduSduList);
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__L2Data_Request_Type(acpCtx_t _ctx, const struct L2Data_Request_Type* p)
{
	adbgPrintLog(_ctx, "SubframeDataList := { ");
	for (size_t i1 = 0; i1 < p->SubframeDataList.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgDrb__DRB_DataPerSubframe_DL_Type(_ctx, &p->SubframeDataList.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->SubframeDataList.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__Null_Type_SuppressPdcchForC_RNTI_Optional(acpCtx_t _ctx, const struct Null_Type_SuppressPdcchForC_RNTI_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "%s", (p->v ? "true" : "false"));
}

static void _adbgDrb__DRB_COMMON_REQ(acpCtx_t _ctx, const struct DRB_COMMON_REQ* p)
{
	adbgPrintLog(_ctx, "Common := { ");
	_adbgDrb__ReqAspCommonPart_Type(_ctx, &p->Common);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "U_Plane := { ");
	_adbgDrb__L2Data_Request_Type(_ctx, &p->U_Plane);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SuppressPdcchForC_RNTI := ");
	_adbgDrb__Null_Type_SuppressPdcchForC_RNTI_Optional(_ctx, &p->SuppressPdcchForC_RNTI);
}

void adbgDrbProcessFromSSLogIn(acpCtx_t _ctx, const struct DRB_COMMON_REQ* FromSS)
{
	adbgPrintLog(_ctx, "@DrbProcessFromSS In Args : { ");

	adbgPrintLog(_ctx, "FromSS := { ");
	_adbgDrb__DRB_COMMON_REQ(_ctx, FromSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static void _adbgDrb__IntegrityErrorIndication_Type(acpCtx_t _ctx, const struct IntegrityErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Nas := %s", (p->Nas ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Pdcp := %s", (p->Pdcp ? "true" : "false"));
}

static void _adbgDrb__ErrorIndication_Type(acpCtx_t _ctx, const struct ErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Integrity := { ");
	_adbgDrb__IntegrityErrorIndication_Type(_ctx, &p->Integrity);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "System := %d", (int)p->System);
}

static void _adbgDrb__IndicationStatus_Type_Value(acpCtx_t _ctx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	if (d == IndicationStatus_Type_Ok) {
		adbgPrintLog(_ctx, "Ok := %s", (p->Ok ? "true" : "false"));
		return;
	}
	if (d == IndicationStatus_Type_Error) {
		adbgPrintLog(_ctx, "Error := { ");
		_adbgDrb__ErrorIndication_Type(_ctx, &p->Error);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgDrb__IndicationStatus_Type(acpCtx_t _ctx, const struct IndicationStatus_Type* p)
{
	_adbgDrb__IndicationStatus_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgDrb__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(acpCtx_t _ctx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgDrb__RlcBearerRouting_Type(_ctx, &p->v);
}

static void _adbgDrb__IndAspCommonPart_Type(acpCtx_t _ctx, const struct IndAspCommonPart_Type* p)
{
	adbgPrintLog(_ctx, "CellId := %s (%d)", adbgDrb__EUTRA_CellId_Type__ToString(p->CellId), (int)p->CellId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RoutingInfo := { ");
	_adbgDrb__RoutingInfo_Type(_ctx, &p->RoutingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingInfo := { ");
	_adbgDrb__TimingInfo_Type(_ctx, &p->TimingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Status := { ");
	_adbgDrb__IndicationStatus_Type(_ctx, &p->Status);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RlcBearerRouting := ");
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgDrb__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_ctx, &p->RlcBearerRouting);
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgDrb__DRB_DataPerSubframe_UL_Type(acpCtx_t _ctx, const struct DRB_DataPerSubframe_UL_Type* p)
{
	adbgPrintLog(_ctx, "PduSduList := { ");
	_adbgDrb__L2DataList_Type(_ctx, &p->PduSduList);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "NoOfTTIs := %d", (int)p->NoOfTTIs);
}

static void _adbgDrb__L2Data_Indication_Type(acpCtx_t _ctx, const struct L2Data_Indication_Type* p)
{
	adbgPrintLog(_ctx, "SubframeData := { ");
	_adbgDrb__DRB_DataPerSubframe_UL_Type(_ctx, &p->SubframeData);
	adbgPrintLog(_ctx, " }");
}

static void _adbgDrb__DRB_COMMON_IND(acpCtx_t _ctx, const struct DRB_COMMON_IND* p)
{
	adbgPrintLog(_ctx, "Common := { ");
	_adbgDrb__IndAspCommonPart_Type(_ctx, &p->Common);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "U_Plane := { ");
	_adbgDrb__L2Data_Indication_Type(_ctx, &p->U_Plane);
	adbgPrintLog(_ctx, " }");
}

void adbgDrbProcessToSSLogOut(acpCtx_t _ctx, const struct DRB_COMMON_IND* ToSS)
{
	adbgPrintLog(_ctx, "@DrbProcessToSS Out Args : { ");

	adbgPrintLog(_ctx, "ToSS := { ");
	_adbgDrb__DRB_COMMON_IND(_ctx, ToSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}
