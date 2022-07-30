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

#include "adbgSysInd.h"

static const char* adbgSysInd__EUTRA_CellId_Type__ToString(EUTRA_CellId_Type v)
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

static void _adbgSysInd__PmchLogicalChannel_Type(acpCtx_t _ctx, const struct PmchLogicalChannel_Type* p)
{
	adbgPrintLog(_ctx, "Pmch_InfoIndex := %u", (unsigned int)p->Pmch_InfoIndex);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LogicalChannelIdentity := %u", (unsigned int)p->LogicalChannelIdentity);
}

static void _adbgSysInd__MRB_Identity_Type(acpCtx_t _ctx, const struct MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "Mbsfn_AreaId := %u", (unsigned int)p->Mbsfn_AreaId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PmchLogicalChannel := { ");
	_adbgSysInd__PmchLogicalChannel_Type(_ctx, &p->PmchLogicalChannel);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysInd__MCC_PLMN_Identity_mcc_Optional(acpCtx_t _ctx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__PLMN_Identity(acpCtx_t _ctx, const struct PLMN_Identity* p)
{
	adbgPrintLog(_ctx, "mcc := ");
	_adbgSysInd__MCC_PLMN_Identity_mcc_Optional(_ctx, &p->mcc);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "mnc := '");
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->mnc.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__TMGI_r9_plmn_Id_r9_Value(acpCtx_t _ctx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		adbgPrintLog(_ctx, "plmn_Index_r9 := %u", (unsigned int)p->plmn_Index_r9);
		return;
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		adbgPrintLog(_ctx, "explicitValue_r9 := { ");
		_adbgSysInd__PLMN_Identity(_ctx, &p->explicitValue_r9);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__TMGI_r9_plmn_Id_r9(acpCtx_t _ctx, const struct TMGI_r9_plmn_Id_r9* p)
{
	_adbgSysInd__TMGI_r9_plmn_Id_r9_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__TMGI_r9(acpCtx_t _ctx, const struct TMGI_r9* p)
{
	adbgPrintLog(_ctx, "plmn_Id_r9 := { ");
	_adbgSysInd__TMGI_r9_plmn_Id_r9(_ctx, &p->plmn_Id_r9);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "serviceId_r9 := '");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->serviceId_r9[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(acpCtx_t _ctx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__MBMSSessionInfo_r13(acpCtx_t _ctx, const struct MBMSSessionInfo_r13* p)
{
	adbgPrintLog(_ctx, "tmgi_r13 := { ");
	_adbgSysInd__TMGI_r9(_ctx, &p->tmgi_r13);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "sessionId_r13 := ");
	_adbgSysInd__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(_ctx, &p->sessionId_r13);
}

static void _adbgSysInd__SC_MRB_Identity_Type(acpCtx_t _ctx, const struct SC_MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "MbmsSessionInfo := { ");
	_adbgSysInd__MBMSSessionInfo_r13(_ctx, &p->MbmsSessionInfo);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysInd__RadioBearerId_Type_Value(acpCtx_t _ctx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
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
		_adbgSysInd__MRB_Identity_Type(_ctx, &p->Mrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RadioBearerId_Type_ScMrb) {
		adbgPrintLog(_ctx, "ScMrb := { ");
		_adbgSysInd__SC_MRB_Identity_Type(_ctx, &p->ScMrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__RadioBearerId_Type(acpCtx_t _ctx, const struct RadioBearerId_Type* p)
{
	_adbgSysInd__RadioBearerId_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__QosFlow_Identification_Type(acpCtx_t _ctx, const struct QosFlow_Identification_Type* p)
{
	adbgPrintLog(_ctx, "PDU_SessionId := %d", (int)p->PDU_SessionId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "QFI := %d", (int)p->QFI);
}

static void _adbgSysInd__RoutingInfo_Type_Value(acpCtx_t _ctx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		adbgPrintLog(_ctx, "RadioBearerId := { ");
		_adbgSysInd__RadioBearerId_Type(_ctx, &p->RadioBearerId);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RoutingInfo_Type_QosFlow) {
		adbgPrintLog(_ctx, "QosFlow := { ");
		_adbgSysInd__QosFlow_Identification_Type(_ctx, &p->QosFlow);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__RoutingInfo_Type(acpCtx_t _ctx, const struct RoutingInfo_Type* p)
{
	_adbgSysInd__RoutingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SystemFrameNumberInfo_Type_Value(acpCtx_t _ctx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static void _adbgSysInd__SystemFrameNumberInfo_Type(acpCtx_t _ctx, const struct SystemFrameNumberInfo_Type* p)
{
	_adbgSysInd__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SubFrameInfo_Type_Value(acpCtx_t _ctx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static void _adbgSysInd__SubFrameInfo_Type(acpCtx_t _ctx, const struct SubFrameInfo_Type* p)
{
	_adbgSysInd__SubFrameInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__HyperSystemFrameNumberInfo_Type(acpCtx_t _ctx, const HyperSystemFrameNumberInfo_Type* p)
{
	_adbgSysInd__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SlotOffset_Type_Value(acpCtx_t _ctx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static void _adbgSysInd__SlotOffset_Type(acpCtx_t _ctx, const struct SlotOffset_Type* p)
{
	_adbgSysInd__SlotOffset_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SlotTimingInfo_Type_Value(acpCtx_t _ctx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	if (d == SlotTimingInfo_Type_SlotOffset) {
		adbgPrintLog(_ctx, "SlotOffset := { ");
		_adbgSysInd__SlotOffset_Type(_ctx, &p->SlotOffset);
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

static void _adbgSysInd__SlotTimingInfo_Type(acpCtx_t _ctx, const struct SlotTimingInfo_Type* p)
{
	_adbgSysInd__SlotTimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SubFrameTiming_Type(acpCtx_t _ctx, const struct SubFrameTiming_Type* p)
{
	adbgPrintLog(_ctx, "SFN := { ");
	_adbgSysInd__SystemFrameNumberInfo_Type(_ctx, &p->SFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Subframe := { ");
	_adbgSysInd__SubFrameInfo_Type(_ctx, &p->Subframe);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HSFN := { ");
	_adbgSysInd__HyperSystemFrameNumberInfo_Type(_ctx, &p->HSFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Slot := { ");
	_adbgSysInd__SlotTimingInfo_Type(_ctx, &p->Slot);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysInd__TimingInfo_Type_Value(acpCtx_t _ctx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	if (d == TimingInfo_Type_SubFrame) {
		adbgPrintLog(_ctx, "SubFrame := { ");
		_adbgSysInd__SubFrameTiming_Type(_ctx, &p->SubFrame);
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

static void _adbgSysInd__TimingInfo_Type(acpCtx_t _ctx, const struct TimingInfo_Type* p)
{
	_adbgSysInd__TimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__IntegrityErrorIndication_Type(acpCtx_t _ctx, const struct IntegrityErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Nas := %s", (p->Nas ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Pdcp := %s", (p->Pdcp ? "true" : "false"));
}

static void _adbgSysInd__ErrorIndication_Type(acpCtx_t _ctx, const struct ErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Integrity := { ");
	_adbgSysInd__IntegrityErrorIndication_Type(_ctx, &p->Integrity);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "System := %d", (int)p->System);
}

static void _adbgSysInd__IndicationStatus_Type_Value(acpCtx_t _ctx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	if (d == IndicationStatus_Type_Ok) {
		adbgPrintLog(_ctx, "Ok := %s", (p->Ok ? "true" : "false"));
		return;
	}
	if (d == IndicationStatus_Type_Error) {
		adbgPrintLog(_ctx, "Error := { ");
		_adbgSysInd__ErrorIndication_Type(_ctx, &p->Error);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__IndicationStatus_Type(acpCtx_t _ctx, const struct IndicationStatus_Type* p)
{
	_adbgSysInd__IndicationStatus_Type_Value(_ctx, &p->v, p->d);
}

static const char* adbgSysInd__NR_CellId_Type__ToString(NR_CellId_Type v)
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

static void _adbgSysInd__RlcBearerRouting_Type_Value(acpCtx_t _ctx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
{
	if (d == RlcBearerRouting_Type_EUTRA) {
		adbgPrintLog(_ctx, "EUTRA := %s (%d)", adbgSysInd__EUTRA_CellId_Type__ToString(p->EUTRA), (int)p->EUTRA);
		return;
	}
	if (d == RlcBearerRouting_Type_NR) {
		adbgPrintLog(_ctx, "NR := %s (%d)", adbgSysInd__NR_CellId_Type__ToString(p->NR), (int)p->NR);
		return;
	}
	if (d == RlcBearerRouting_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__RlcBearerRouting_Type(acpCtx_t _ctx, const struct RlcBearerRouting_Type* p)
{
	_adbgSysInd__RlcBearerRouting_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(acpCtx_t _ctx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgSysInd__RlcBearerRouting_Type(_ctx, &p->v);
}

static void _adbgSysInd__IndAspCommonPart_Type(acpCtx_t _ctx, const struct IndAspCommonPart_Type* p)
{
	adbgPrintLog(_ctx, "CellId := %s (%d)", adbgSysInd__EUTRA_CellId_Type__ToString(p->CellId), (int)p->CellId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RoutingInfo := { ");
	_adbgSysInd__RoutingInfo_Type(_ctx, &p->RoutingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingInfo := { ");
	_adbgSysInd__TimingInfo_Type(_ctx, &p->TimingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Status := { ");
	_adbgSysInd__IndicationStatus_Type(_ctx, &p->Status);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RlcBearerRouting := ");
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgSysInd__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_ctx, &p->RlcBearerRouting);
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgSysInd__Int_RepetitionsPerPreambleAttempt_Optional(acpCtx_t _ctx, const struct int_RepetitionsPerPreambleAttempt_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "%d", (int)p->v);
}

static void _adbgSysInd__RachPreamble_Type(acpCtx_t _ctx, const struct RachPreamble_Type* p)
{
	adbgPrintLog(_ctx, "RAPID := %u", (unsigned int)p->RAPID);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PRTPower := %s", (p->PRTPower ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RepetitionsPerPreambleAttempt := ");
	_adbgSysInd__Int_RepetitionsPerPreambleAttempt_Optional(_ctx, &p->RepetitionsPerPreambleAttempt);
}

static void _adbgSysInd__Short_BSR_Type(acpCtx_t _ctx, const struct Short_BSR_Type* p)
{
	adbgPrintLog(_ctx, "LCG := %d", (int)p->LCG);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value := %d", (int)p->Value);
}

static void _adbgSysInd__Long_BSR_Type(acpCtx_t _ctx, const struct Long_BSR_Type* p)
{
	adbgPrintLog(_ctx, "Value_LCG1 := %d", (int)p->Value_LCG1);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG2 := %d", (int)p->Value_LCG2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG3 := %d", (int)p->Value_LCG3);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Value_LCG4 := %d", (int)p->Value_LCG4);
}

static void _adbgSysInd__SL_BSR_Value_Type(acpCtx_t _ctx, const struct SL_BSR_Value_Type* p)
{
	adbgPrintLog(_ctx, "DestinationIndex := '");
	for (size_t i2 = 0; i2 < 4; i2++) {
		adbgPrintLog(_ctx, "%02X", p->DestinationIndex[i2]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LCG_ID := '");
	for (size_t i2 = 0; i2 < 2; i2++) {
		adbgPrintLog(_ctx, "%02X", p->LCG_ID[i2]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "BufferSize := '");
	for (size_t i2 = 0; i2 < 6; i2++) {
		adbgPrintLog(_ctx, "%02X", p->BufferSize[i2]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__SL_BSR_MACHeader_Type(acpCtx_t _ctx, const struct SL_BSR_MACHeader_Type* p)
{
	adbgPrintLog(_ctx, "HeaderFieldLCID := '");
	for (size_t i1 = 0; i1 < 5; i1++) {
		adbgPrintLog(_ctx, "%02X", p->HeaderFieldLCID[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HeaderFieldE := '");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->HeaderFieldE[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HeaderFieldF := '");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->HeaderFieldF[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "SL_BSR_Value := { ");
	for (size_t i1 = 0; i1 < p->SL_BSR_Value.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgSysInd__SL_BSR_Value_Type(_ctx, &p->SL_BSR_Value.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->SL_BSR_Value.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysInd__BSR_Type_Value(acpCtx_t _ctx, const union BSR_Type_Value* p, enum BSR_Type_Sel d)
{
	if (d == BSR_Type_Short) {
		adbgPrintLog(_ctx, "Short := { ");
		_adbgSysInd__Short_BSR_Type(_ctx, &p->Short);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == BSR_Type_Truncated) {
		adbgPrintLog(_ctx, "Truncated := { ");
		_adbgSysInd__Short_BSR_Type(_ctx, &p->Truncated);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == BSR_Type_Long) {
		adbgPrintLog(_ctx, "Long := { ");
		_adbgSysInd__Long_BSR_Type(_ctx, &p->Long);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == BSR_Type_Sidelink) {
		adbgPrintLog(_ctx, "Sidelink := { ");
		_adbgSysInd__SL_BSR_MACHeader_Type(_ctx, &p->Sidelink);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__BSR_Type(acpCtx_t _ctx, const struct BSR_Type* p)
{
	_adbgSysInd__BSR_Type_Value(_ctx, &p->v, p->d);
}

static const char* adbgSysInd__HARQ_Type__ToString(HARQ_Type v)
{
	switch(v) {
		case HARQ_Type_ack: return "HARQ_Type_ack";
		case HARQ_Type_nack: return "HARQ_Type_nack";
		default: return "Unknown";
	}
}

static void _adbgSysInd__HarqProcessInfo_Type(acpCtx_t _ctx, const struct HarqProcessInfo_Type* p)
{
	adbgPrintLog(_ctx, "Id := %u", (unsigned int)p->Id);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "CURRENT_TX_NB := %d", (int)p->CURRENT_TX_NB);
}

static void _adbgSysInd__HarqError_Type_Value(acpCtx_t _ctx, const union HarqError_Type_Value* p, enum HarqError_Type_Sel d)
{
	if (d == HarqError_Type_UL) {
		adbgPrintLog(_ctx, "UL := { ");
		_adbgSysInd__HarqProcessInfo_Type(_ctx, &p->UL);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == HarqError_Type_DL) {
		adbgPrintLog(_ctx, "DL := { ");
		_adbgSysInd__HarqProcessInfo_Type(_ctx, &p->DL);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__HarqError_Type(acpCtx_t _ctx, const struct HarqError_Type* p)
{
	_adbgSysInd__HarqError_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__RlcDiscardInd_Type(acpCtx_t _ctx, const struct RlcDiscardInd_Type* p)
{
	adbgPrintLog(_ctx, "SequenceNumber := %d", (int)p->SequenceNumber);
}

static void _adbgSysInd__ScellBitMap_Type(acpCtx_t _ctx, const struct ScellBitMap_Type* p)
{
	adbgPrintLog(_ctx, "Value := '");
	for (size_t i1 = 0; i1 < 7; i1++) {
		adbgPrintLog(_ctx, "%02X", p->Value[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := '");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->Reserved[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__B2_Type_Reserved_Optional(acpCtx_t _ctx, const struct B2_Type_Reserved_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i2 = 0; i2 < 2; i2++) {
		adbgPrintLog(_ctx, "%02X", p->v[i2]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__B6_Type_PCMaxc_Optional(acpCtx_t _ctx, const struct B6_Type_PCMaxc_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i2 = 0; i2 < 6; i2++) {
		adbgPrintLog(_ctx, "%02X", p->v[i2]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysInd__PH_Record_Type(acpCtx_t _ctx, const struct PH_Record_Type* p)
{
	adbgPrintLog(_ctx, "P_Bit := '");
	for (size_t i2 = 0; i2 < 1; i2++) {
		adbgPrintLog(_ctx, "%02X", p->P_Bit[i2]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "V_Bit := '");
	for (size_t i2 = 0; i2 < 1; i2++) {
		adbgPrintLog(_ctx, "%02X", p->V_Bit[i2]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Valve := '");
	for (size_t i2 = 0; i2 < 6; i2++) {
		adbgPrintLog(_ctx, "%02X", p->Valve[i2]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Reserved := ");
	_adbgSysInd__B2_Type_Reserved_Optional(_ctx, &p->Reserved);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PCMaxc := ");
	_adbgSysInd__B6_Type_PCMaxc_Optional(_ctx, &p->PCMaxc);
}

static void _adbgSysInd__MAC_CTRL_ExtPowerHeadRoom_Type(acpCtx_t _ctx, const struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	adbgPrintLog(_ctx, "EPH_Octet1 := { ");
	_adbgSysInd__ScellBitMap_Type(_ctx, &p->EPH_Octet1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PH_RecordList := { ");
	for (size_t i1 = 0; i1 < p->PH_RecordList.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgSysInd__PH_Record_Type(_ctx, &p->PH_RecordList.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->PH_RecordList.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static const char* adbgSysInd__SRSInd_Type__ToString(SRSInd_Type v)
{
	switch(v) {
		case SRSInd_Type_SRS_Start: return "SRSInd_Type_SRS_Start";
		case SRSInd_Type_SRS_Stop: return "SRSInd_Type_SRS_Stop";
		default: return "Unknown";
	}
}

static void _adbgSysInd__MAC_CTRL_DC_PowerHeadRoom_Type(acpCtx_t _ctx, const struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	adbgPrintLog(_ctx, "DC_PH_Octet1 := { ");
	_adbgSysInd__ScellBitMap_Type(_ctx, &p->DC_PH_Octet1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "DC_PH_RecordList := { ");
	for (size_t i1 = 0; i1 < p->DC_PH_RecordList.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgSysInd__PH_Record_Type(_ctx, &p->DC_PH_RecordList.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->DC_PH_RecordList.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysInd__SystemIndication_Type_Value(acpCtx_t _ctx, const union SystemIndication_Type_Value* p, enum SystemIndication_Type_Sel d)
{
	if (d == SystemIndication_Type_Error) {
		adbgPrintLog(_ctx, "Error := '");
		for (size_t i1 = 0; i1 < p->Error.d; i1++) {
			adbgPrintLog(_ctx, "%02X", p->Error.v[i1]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	if (d == SystemIndication_Type_RachPreamble) {
		adbgPrintLog(_ctx, "RachPreamble := { ");
		_adbgSysInd__RachPreamble_Type(_ctx, &p->RachPreamble);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SystemIndication_Type_SchedReq) {
		adbgPrintLog(_ctx, "SchedReq := %s", (p->SchedReq ? "true" : "false"));
		return;
	}
	if (d == SystemIndication_Type_BSR) {
		adbgPrintLog(_ctx, "BSR := { ");
		_adbgSysInd__BSR_Type(_ctx, &p->BSR);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SystemIndication_Type_UL_HARQ) {
		adbgPrintLog(_ctx, "UL_HARQ := %s (%d)", adbgSysInd__HARQ_Type__ToString(p->UL_HARQ), (int)p->UL_HARQ);
		return;
	}
	if (d == SystemIndication_Type_C_RNTI) {
		adbgPrintLog(_ctx, "C_RNTI := '");
		for (size_t i1 = 0; i1 < 16; i1++) {
			adbgPrintLog(_ctx, "%02X", p->C_RNTI[i1]);
			return;
		}
	}
	adbgPrintLog(_ctx, "'O");
	if (d == SystemIndication_Type_PHR) {
		adbgPrintLog(_ctx, "PHR := %u", (unsigned int)p->PHR);
		return;
	}
	if (d == SystemIndication_Type_HarqError) {
		adbgPrintLog(_ctx, "HarqError := { ");
		_adbgSysInd__HarqError_Type(_ctx, &p->HarqError);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SystemIndication_Type_RlcDiscardInd) {
		adbgPrintLog(_ctx, "RlcDiscardInd := { ");
		_adbgSysInd__RlcDiscardInd_Type(_ctx, &p->RlcDiscardInd);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SystemIndication_Type_PeriodicRI) {
		adbgPrintLog(_ctx, "PeriodicRI := %u", (unsigned int)p->PeriodicRI);
		return;
	}
	if (d == SystemIndication_Type_EPHR) {
		adbgPrintLog(_ctx, "EPHR := { ");
		_adbgSysInd__MAC_CTRL_ExtPowerHeadRoom_Type(_ctx, &p->EPHR);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == SystemIndication_Type_CqiInd) {
		adbgPrintLog(_ctx, "CqiInd := %s", (p->CqiInd ? "true" : "false"));
		return;
	}
	if (d == SystemIndication_Type_SrsInd) {
		adbgPrintLog(_ctx, "SrsInd := %s (%d)", adbgSysInd__SRSInd_Type__ToString(p->SrsInd), (int)p->SrsInd);
		return;
	}
	if (d == SystemIndication_Type_DC_PHR) {
		adbgPrintLog(_ctx, "DC_PHR := { ");
		_adbgSysInd__MAC_CTRL_DC_PowerHeadRoom_Type(_ctx, &p->DC_PHR);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysInd__SystemIndication_Type(acpCtx_t _ctx, const struct SystemIndication_Type* p)
{
	_adbgSysInd__SystemIndication_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysInd__SYSTEM_IND(acpCtx_t _ctx, const struct SYSTEM_IND* p)
{
	adbgPrintLog(_ctx, "Common := { ");
	_adbgSysInd__IndAspCommonPart_Type(_ctx, &p->Common);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Indication := { ");
	_adbgSysInd__SystemIndication_Type(_ctx, &p->Indication);
	adbgPrintLog(_ctx, " }");
}

void adbgSysIndProcessToSSLogOut(acpCtx_t _ctx, const struct SYSTEM_IND* ToSS)
{
	adbgPrintLog(_ctx, "@SysIndProcessToSS Out Args : { ");

	adbgPrintLog(_ctx, "ToSS := { ");
	_adbgSysInd__SYSTEM_IND(_ctx, ToSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}
