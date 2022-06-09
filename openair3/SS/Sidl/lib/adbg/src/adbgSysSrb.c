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

#include "adbgSysSrb.h"

static const char* adbgSysSrb__EUTRA_CellId_Type__ToString(EUTRA_CellId_Type v)
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

static void _adbgSysSrb__PmchLogicalChannel_Type(acpCtx_t _ctx, const struct PmchLogicalChannel_Type* p)
{
	adbgPrintLog(_ctx, "Pmch_InfoIndex := %u", (unsigned int)p->Pmch_InfoIndex);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "LogicalChannelIdentity := %u", (unsigned int)p->LogicalChannelIdentity);
}

static void _adbgSysSrb__MRB_Identity_Type(acpCtx_t _ctx, const struct MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "Mbsfn_AreaId := %u", (unsigned int)p->Mbsfn_AreaId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "PmchLogicalChannel := { ");
	_adbgSysSrb__PmchLogicalChannel_Type(_ctx, &p->PmchLogicalChannel);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysSrb__MCC_PLMN_Identity_mcc_Optional(acpCtx_t _ctx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysSrb__PLMN_Identity(acpCtx_t _ctx, const struct PLMN_Identity* p)
{
	adbgPrintLog(_ctx, "mcc := ");
	_adbgSysSrb__MCC_PLMN_Identity_mcc_Optional(_ctx, &p->mcc);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "mnc := '");
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->mnc.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysSrb__TMGI_r9_plmn_Id_r9_Value(acpCtx_t _ctx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		adbgPrintLog(_ctx, "plmn_Index_r9 := %u", (unsigned int)p->plmn_Index_r9);
		return;
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		adbgPrintLog(_ctx, "explicitValue_r9 := { ");
		_adbgSysSrb__PLMN_Identity(_ctx, &p->explicitValue_r9);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__TMGI_r9_plmn_Id_r9(acpCtx_t _ctx, const struct TMGI_r9_plmn_Id_r9* p)
{
	_adbgSysSrb__TMGI_r9_plmn_Id_r9_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__TMGI_r9(acpCtx_t _ctx, const struct TMGI_r9* p)
{
	adbgPrintLog(_ctx, "plmn_Id_r9 := { ");
	_adbgSysSrb__TMGI_r9_plmn_Id_r9(_ctx, &p->plmn_Id_r9);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "serviceId_r9 := '");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->serviceId_r9[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysSrb__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(acpCtx_t _ctx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 1; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgSysSrb__MBMSSessionInfo_r13(acpCtx_t _ctx, const struct MBMSSessionInfo_r13* p)
{
	adbgPrintLog(_ctx, "tmgi_r13 := { ");
	_adbgSysSrb__TMGI_r9(_ctx, &p->tmgi_r13);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "sessionId_r13 := ");
	_adbgSysSrb__O1_MBMSSessionInfo_r13_sessionId_r13_Optional(_ctx, &p->sessionId_r13);
}

static void _adbgSysSrb__SC_MRB_Identity_Type(acpCtx_t _ctx, const struct SC_MRB_Identity_Type* p)
{
	adbgPrintLog(_ctx, "MbmsSessionInfo := { ");
	_adbgSysSrb__MBMSSessionInfo_r13(_ctx, &p->MbmsSessionInfo);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysSrb__RadioBearerId_Type_Value(acpCtx_t _ctx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
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
		_adbgSysSrb__MRB_Identity_Type(_ctx, &p->Mrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RadioBearerId_Type_ScMrb) {
		adbgPrintLog(_ctx, "ScMrb := { ");
		_adbgSysSrb__SC_MRB_Identity_Type(_ctx, &p->ScMrb);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__RadioBearerId_Type(acpCtx_t _ctx, const struct RadioBearerId_Type* p)
{
	_adbgSysSrb__RadioBearerId_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__QosFlow_Identification_Type(acpCtx_t _ctx, const struct QosFlow_Identification_Type* p)
{
	adbgPrintLog(_ctx, "PDU_SessionId := %d", (int)p->PDU_SessionId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "QFI := %d", (int)p->QFI);
}

static void _adbgSysSrb__RoutingInfo_Type_Value(acpCtx_t _ctx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		adbgPrintLog(_ctx, "RadioBearerId := { ");
		_adbgSysSrb__RadioBearerId_Type(_ctx, &p->RadioBearerId);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == RoutingInfo_Type_QosFlow) {
		adbgPrintLog(_ctx, "QosFlow := { ");
		_adbgSysSrb__QosFlow_Identification_Type(_ctx, &p->QosFlow);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__RoutingInfo_Type(acpCtx_t _ctx, const struct RoutingInfo_Type* p)
{
	_adbgSysSrb__RoutingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__SystemFrameNumberInfo_Type_Value(acpCtx_t _ctx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static void _adbgSysSrb__SystemFrameNumberInfo_Type(acpCtx_t _ctx, const struct SystemFrameNumberInfo_Type* p)
{
	_adbgSysSrb__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__SubFrameInfo_Type_Value(acpCtx_t _ctx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static void _adbgSysSrb__SubFrameInfo_Type(acpCtx_t _ctx, const struct SubFrameInfo_Type* p)
{
	_adbgSysSrb__SubFrameInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__HyperSystemFrameNumberInfo_Type(acpCtx_t _ctx, const HyperSystemFrameNumberInfo_Type* p)
{
	_adbgSysSrb__SystemFrameNumberInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__SlotOffset_Type_Value(acpCtx_t _ctx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static void _adbgSysSrb__SlotOffset_Type(acpCtx_t _ctx, const struct SlotOffset_Type* p)
{
	_adbgSysSrb__SlotOffset_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__SlotTimingInfo_Type_Value(acpCtx_t _ctx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	if (d == SlotTimingInfo_Type_SlotOffset) {
		adbgPrintLog(_ctx, "SlotOffset := { ");
		_adbgSysSrb__SlotOffset_Type(_ctx, &p->SlotOffset);
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

static void _adbgSysSrb__SlotTimingInfo_Type(acpCtx_t _ctx, const struct SlotTimingInfo_Type* p)
{
	_adbgSysSrb__SlotTimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__SubFrameTiming_Type(acpCtx_t _ctx, const struct SubFrameTiming_Type* p)
{
	adbgPrintLog(_ctx, "SFN := { ");
	_adbgSysSrb__SystemFrameNumberInfo_Type(_ctx, &p->SFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Subframe := { ");
	_adbgSysSrb__SubFrameInfo_Type(_ctx, &p->Subframe);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "HSFN := { ");
	_adbgSysSrb__HyperSystemFrameNumberInfo_Type(_ctx, &p->HSFN);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Slot := { ");
	_adbgSysSrb__SlotTimingInfo_Type(_ctx, &p->Slot);
	adbgPrintLog(_ctx, " }");
}

static void _adbgSysSrb__TimingInfo_Type_Value(acpCtx_t _ctx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	if (d == TimingInfo_Type_SubFrame) {
		adbgPrintLog(_ctx, "SubFrame := { ");
		_adbgSysSrb__SubFrameTiming_Type(_ctx, &p->SubFrame);
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

static void _adbgSysSrb__TimingInfo_Type(acpCtx_t _ctx, const struct TimingInfo_Type* p)
{
	_adbgSysSrb__TimingInfo_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__IntegrityErrorIndication_Type(acpCtx_t _ctx, const struct IntegrityErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Nas := %s", (p->Nas ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Pdcp := %s", (p->Pdcp ? "true" : "false"));
}

static void _adbgSysSrb__ErrorIndication_Type(acpCtx_t _ctx, const struct ErrorIndication_Type* p)
{
	adbgPrintLog(_ctx, "Integrity := { ");
	_adbgSysSrb__IntegrityErrorIndication_Type(_ctx, &p->Integrity);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "System := %d", (int)p->System);
}

static void _adbgSysSrb__IndicationStatus_Type_Value(acpCtx_t _ctx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	if (d == IndicationStatus_Type_Ok) {
		adbgPrintLog(_ctx, "Ok := %s", (p->Ok ? "true" : "false"));
		return;
	}
	if (d == IndicationStatus_Type_Error) {
		adbgPrintLog(_ctx, "Error := { ");
		_adbgSysSrb__ErrorIndication_Type(_ctx, &p->Error);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__IndicationStatus_Type(acpCtx_t _ctx, const struct IndicationStatus_Type* p)
{
	_adbgSysSrb__IndicationStatus_Type_Value(_ctx, &p->v, p->d);
}

static const char* adbgSysSrb__NR_CellId_Type__ToString(NR_CellId_Type v)
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

static void _adbgSysSrb__RlcBearerRouting_Type_Value(acpCtx_t _ctx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
{
	if (d == RlcBearerRouting_Type_EUTRA) {
		adbgPrintLog(_ctx, "EUTRA := %s (%d)", adbgSysSrb__EUTRA_CellId_Type__ToString(p->EUTRA), (int)p->EUTRA);
		return;
	}
	if (d == RlcBearerRouting_Type_NR) {
		adbgPrintLog(_ctx, "NR := %s (%d)", adbgSysSrb__NR_CellId_Type__ToString(p->NR), (int)p->NR);
		return;
	}
	if (d == RlcBearerRouting_Type_None) {
		adbgPrintLog(_ctx, "None := %s", (p->None ? "true" : "false"));
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__RlcBearerRouting_Type(acpCtx_t _ctx, const struct RlcBearerRouting_Type* p)
{
	_adbgSysSrb__RlcBearerRouting_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(acpCtx_t _ctx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgSysSrb__RlcBearerRouting_Type(_ctx, &p->v);
}

static void _adbgSysSrb__IndAspCommonPart_Type(acpCtx_t _ctx, const struct IndAspCommonPart_Type* p)
{
	adbgPrintLog(_ctx, "CellId := %s (%d)", adbgSysSrb__EUTRA_CellId_Type__ToString(p->CellId), (int)p->CellId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RoutingInfo := { ");
	_adbgSysSrb__RoutingInfo_Type(_ctx, &p->RoutingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingInfo := { ");
	_adbgSysSrb__TimingInfo_Type(_ctx, &p->TimingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Status := { ");
	_adbgSysSrb__IndicationStatus_Type(_ctx, &p->Status);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RlcBearerRouting := ");
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgSysSrb__RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_ctx, &p->RlcBearerRouting);
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgSysSrb__RRC_MSG_Indication_Type_Value(acpCtx_t _ctx, const union RRC_MSG_Indication_Type_Value* p, enum RRC_MSG_Indication_Type_Sel d)
{
	if (d == RRC_MSG_Indication_Type_Ccch) {
		adbgPrintLog(_ctx, "Ccch := '");
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			adbgPrintLog(_ctx, "%02X", p->Ccch.v[i1]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	if (d == RRC_MSG_Indication_Type_Dcch) {
		adbgPrintLog(_ctx, "Dcch := '");
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			adbgPrintLog(_ctx, "%02X", p->Dcch.v[i1]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__RRC_MSG_Indication_Type(acpCtx_t _ctx, const struct RRC_MSG_Indication_Type* p)
{
	_adbgSysSrb__RRC_MSG_Indication_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__EUTRA_RRC_PDU_IND(acpCtx_t _ctx, const struct EUTRA_RRC_PDU_IND* p)
{
	adbgPrintLog(_ctx, "Common := { ");
	_adbgSysSrb__IndAspCommonPart_Type(_ctx, &p->Common);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RrcPdu := { ");
	_adbgSysSrb__RRC_MSG_Indication_Type(_ctx, &p->RrcPdu);
	adbgPrintLog(_ctx, " }");
}

void adbgSysSrbProcessToSSLogOut(acpCtx_t _ctx, const struct EUTRA_RRC_PDU_IND* ToSS)
{
	adbgPrintLog(_ctx, "@SysSrbProcessToSS Out Args : { ");

	adbgPrintLog(_ctx, "ToSS := { ");
	_adbgSysSrb__EUTRA_RRC_PDU_IND(_ctx, ToSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static void _adbgSysSrb__ReqAspControlInfo_Type(acpCtx_t _ctx, const struct ReqAspControlInfo_Type* p)
{
	adbgPrintLog(_ctx, "CnfFlag := %s", (p->CnfFlag ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "FollowOnFlag := %s", (p->FollowOnFlag ? "true" : "false"));
}

static void _adbgSysSrb__RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(acpCtx_t _ctx, const struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgSysSrb__RlcBearerRouting_Type(_ctx, &p->v);
}

static void _adbgSysSrb__ReqAspCommonPart_Type(acpCtx_t _ctx, const struct ReqAspCommonPart_Type* p)
{
	adbgPrintLog(_ctx, "CellId := %s (%d)", adbgSysSrb__EUTRA_CellId_Type__ToString(p->CellId), (int)p->CellId);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RoutingInfo := { ");
	_adbgSysSrb__RoutingInfo_Type(_ctx, &p->RoutingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "TimingInfo := { ");
	_adbgSysSrb__TimingInfo_Type(_ctx, &p->TimingInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "ControlInfo := { ");
	_adbgSysSrb__ReqAspControlInfo_Type(_ctx, &p->ControlInfo);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RlcBearerRouting := ");
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgSysSrb__RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_ctx, &p->RlcBearerRouting);
	if (p->RlcBearerRouting.d) { adbgPrintLog(_ctx, " }"); };
}

static void _adbgSysSrb__RRC_MSG_Request_Type_Value(acpCtx_t _ctx, const union RRC_MSG_Request_Type_Value* p, enum RRC_MSG_Request_Type_Sel d)
{
	if (d == RRC_MSG_Request_Type_Ccch) {
		adbgPrintLog(_ctx, "Ccch := '");
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			adbgPrintLog(_ctx, "%02X", p->Ccch.v[i1]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	if (d == RRC_MSG_Request_Type_Dcch) {
		adbgPrintLog(_ctx, "Dcch := '");
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			adbgPrintLog(_ctx, "%02X", p->Dcch.v[i1]);
		}
		adbgPrintLog(_ctx, "'O");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgSysSrb__RRC_MSG_Request_Type(acpCtx_t _ctx, const struct RRC_MSG_Request_Type* p)
{
	_adbgSysSrb__RRC_MSG_Request_Type_Value(_ctx, &p->v, p->d);
}

static void _adbgSysSrb__EUTRA_RRC_PDU_REQ(acpCtx_t _ctx, const struct EUTRA_RRC_PDU_REQ* p)
{
	adbgPrintLog(_ctx, "Common := { ");
	_adbgSysSrb__ReqAspCommonPart_Type(_ctx, &p->Common);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "RrcPdu := { ");
	_adbgSysSrb__RRC_MSG_Request_Type(_ctx, &p->RrcPdu);
	adbgPrintLog(_ctx, " }");
}

void adbgSysSrbProcessFromSSLogIn(acpCtx_t _ctx, const struct EUTRA_RRC_PDU_REQ* FromSS)
{
	adbgPrintLog(_ctx, "@SysSrbProcessFromSS In Args : { ");

	adbgPrintLog(_ctx, "FromSS := { ");
	_adbgSysSrb__EUTRA_RRC_PDU_REQ(_ctx, FromSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}
