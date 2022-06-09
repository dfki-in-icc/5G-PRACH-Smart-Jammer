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

#include <string.h>
#include "serDrb.h"
#include "serMem.h"
#include "serUtils.h"

static int _serDrbEncPmchLogicalChannel_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Pmch_InfoIndex, _lidx);
	HTON_8(&_buffer[*_lidx], p->LogicalChannelIdentity, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Mbsfn_AreaId, _lidx);
	_serDrbEncPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMCC_PLMN_Identity_mcc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPLMN_Identity(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	HTON_32(&_buffer[*_lidx], p->mnc.d, _lidx);
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->mnc.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncTMGI_r9_plmn_Id_r9_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		HTON_8(&_buffer[*_lidx], p->plmn_Index_r9, _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serDrbEncPLMN_Identity(_buffer, _size, _lidx, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncTMGI_r9_plmn_Id_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncTMGI_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->serviceId_r9[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMBMSSessionInfo_r13(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncTMGI_r9(_buffer, _size, _lidx, &p->tmgi_r13);
	_serDrbEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSC_MRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncMBMSSessionInfo_r13(_buffer, _size, _lidx, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRadioBearerId_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		HTON_8(&_buffer[*_lidx], p->Srb, _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		HTON_8(&_buffer[*_lidx], p->Drb, _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serDrbEncMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serDrbEncSC_MRB_Identity_Type(_buffer, _size, _lidx, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRadioBearerId_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRadioBearerId_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncQosFlow_Identification_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->PDU_SessionId, _lidx);
	HTON_32(&_buffer[*_lidx], p->QFI, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRoutingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serDrbEncRadioBearerId_Type(_buffer, _size, _lidx, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serDrbEncQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRoutingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRoutingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSystemFrameNumberInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SystemFrameNumberInfo_Type_Number) {
		HTON_16(&_buffer[*_lidx], p->Number, _lidx);
	}
	if (d == SystemFrameNumberInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSubFrameInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SubFrameInfo_Type_Number) {
		HTON_8(&_buffer[*_lidx], p->Number, _lidx);
	}
	if (d == SubFrameInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncSubFrameInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncHyperSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSlotOffset_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotOffset_Type_Numerology0) {
		HTON_8(&_buffer[*_lidx], p->Numerology0, _lidx);
	}
	if (d == SlotOffset_Type_Numerology1) {
		HTON_8(&_buffer[*_lidx], p->Numerology1, _lidx);
	}
	if (d == SlotOffset_Type_Numerology2) {
		HTON_8(&_buffer[*_lidx], p->Numerology2, _lidx);
	}
	if (d == SlotOffset_Type_Numerology3) {
		HTON_8(&_buffer[*_lidx], p->Numerology3, _lidx);
	}
	if (d == SlotOffset_Type_Numerology4) {
		HTON_8(&_buffer[*_lidx], p->Numerology4, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncSlotOffset_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSlotTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serDrbEncSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		HTON_8(&_buffer[*_lidx], p->FirstSlot, _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncSlotTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncSubFrameTiming_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serDrbEncSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serDrbEncHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serDrbEncSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serDrbEncTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serDrbEncSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		HTON_8(&_buffer[*_lidx], p->Now, _lidx);
	}
	if (d == TimingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncReqAspControlInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->CnfFlag, _lidx);
	HTON_8(&_buffer[*_lidx], p->FollowOnFlag, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRlcBearerRouting_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RlcBearerRouting_Type_EUTRA) {
		{
			size_t _tmp = (size_t)p->EUTRA;
			HTON_32(&_buffer[*_lidx], _tmp, _lidx);
		}
	}
	if (d == RlcBearerRouting_Type_NR) {
		{
			size_t _tmp = (size_t)p->NR;
			HTON_32(&_buffer[*_lidx], _tmp, _lidx);
		}
	}
	if (d == RlcBearerRouting_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRlcBearerRouting_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncReqAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serDrbEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serDrbEncReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serDrbEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serDrbEncHarqProcessAssignment_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union HarqProcessAssignment_Type_Value* p, enum HarqProcessAssignment_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == HarqProcessAssignment_Type_Id) {
		HTON_8(&_buffer[*_lidx], p->Id, _lidx);
	}
	if (d == HarqProcessAssignment_Type_Automatic) {
		HTON_8(&_buffer[*_lidx], p->Automatic, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncHarqProcessAssignment_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct HarqProcessAssignment_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncHarqProcessAssignment_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncHarqProcessAssignment_Type_HarqProcess_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct HarqProcessAssignment_Type_HarqProcess_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncHarqProcessAssignment_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_PDU_Length_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_PDU_Length_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Format[i4], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->Value.d, _lidx);
	for (size_t i4 = 0; i4 < p->Value.d; i4++) {
		HTON_8(&_buffer[*_lidx], p->Value.v[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_PDU_Length_Type_Length_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_PDU_Length_Type_Length_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_PDU_Length_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_PDU_SubHeader_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_PDU_SubHeader_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 2; i4++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 5; i4++) {
		HTON_8(&_buffer[*_lidx], p->LCID[i4], _lidx);
	}
	_serDrbEncMAC_PDU_Length_Type_Length_Optional(_buffer, _size, _lidx, &p->Length);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ShortBSR_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ShortBSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->LCG[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ShortBSR_Type_ShortBSR_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ShortBSR_Type_ShortBSR_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_ShortBSR_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_LongBSR_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_LongBSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value_LCG1[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value_LCG2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value_LCG3[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value_LCG4[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_LongBSR_Type_LongBSR_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_LongBSR_Type_LongBSR_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_LongBSR_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_C_RNTI_Type_C_RNTI_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_C_RNTI_Type_C_RNTI_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 8; i3++) {
		HTON_8(&_buffer[*_lidx], p->v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_PowerHeadRoom_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_PowerHeadRoom_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ScellActDeact_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const MAC_CTRL_ScellActDeact_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 7; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_ScellActDeact_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncScellBitMap_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ScellBitMap_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 7; i3++) {
		HTON_8(&_buffer[*_lidx], p->Value[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncB2_Type_Reserved_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct B2_Type_Reserved_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i4 = 0; i4 < 2; i4++) {
		HTON_8(&_buffer[*_lidx], p->v[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncB6_Type_PCMaxc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct B6_Type_PCMaxc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i4 = 0; i4 < 6; i4++) {
		HTON_8(&_buffer[*_lidx], p->v[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPH_Record_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PH_Record_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->P_Bit[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->V_Bit[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 6; i4++) {
		HTON_8(&_buffer[*_lidx], p->Valve[i4], _lidx);
	}
	_serDrbEncB2_Type_Reserved_Optional(_buffer, _size, _lidx, &p->Reserved);
	_serDrbEncB6_Type_PCMaxc_Optional(_buffer, _size, _lidx, &p->PCMaxc);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ExtPowerHeadRoom_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncScellBitMap_Type(_buffer, _size, _lidx, &p->EPH_Octet1);
	HTON_32(&_buffer[*_lidx], p->PH_RecordList.d, _lidx);
	for (size_t i3 = 0; i3 < p->PH_RecordList.d; i3++) {
		_serDrbEncPH_Record_Type(_buffer, _size, _lidx, &p->PH_RecordList.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_ExtPowerHeadRoom_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_DC_PowerHeadRoom_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncScellBitMap_Type(_buffer, _size, _lidx, &p->DC_PH_Octet1);
	HTON_32(&_buffer[*_lidx], p->DC_PH_RecordList.d, _lidx);
	for (size_t i3 = 0; i3 < p->DC_PH_RecordList.d; i3++) {
		_serDrbEncPH_Record_Type(_buffer, _size, _lidx, &p->DC_PH_RecordList.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_DC_PowerHeadRoom_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ElementList_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ElementList_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncMAC_CTRL_ShortBSR_Type_ShortBSR_Optional(_buffer, _size, _lidx, &p->ShortBSR);
	_serDrbEncMAC_CTRL_LongBSR_Type_LongBSR_Optional(_buffer, _size, _lidx, &p->LongBSR);
	_serDrbEncMAC_CTRL_C_RNTI_Type_C_RNTI_Optional(_buffer, _size, _lidx, &p->C_RNTI);
	_serDrbEncMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(_buffer, _size, _lidx, &p->ContentionResolutionID);
	_serDrbEncMAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(_buffer, _size, _lidx, &p->TimingAdvance);
	_serDrbEncMAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(_buffer, _size, _lidx, &p->PowerHeadRoom);
	_serDrbEncMAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(_buffer, _size, _lidx, &p->ScellActDeact);
	_serDrbEncMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(_buffer, _size, _lidx, &p->ExtPowerHeadRoom);
	_serDrbEncMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(_buffer, _size, _lidx, &p->DC_PowerHeadRoom);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_CTRL_ElementList_Type_CtrlElementList_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncMAC_CTRL_ElementList_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_SDUList_Type_SduList_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_SDUList_Type_SduList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->v.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->v.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->v.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncOCTET_STRING_Padding_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct OCTET_STRING_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncMAC_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->Header.d, _lidx);
	for (size_t i3 = 0; i3 < p->Header.d; i3++) {
		_serDrbEncMAC_PDU_SubHeader_Type(_buffer, _size, _lidx, &p->Header.v[i3]);
	}
	_serDrbEncMAC_CTRL_ElementList_Type_CtrlElementList_Optional(_buffer, _size, _lidx, &p->CtrlElementList);
	_serDrbEncMAC_SDUList_Type_SduList_Optional(_buffer, _size, _lidx, &p->SduList);
	_serDrbEncOCTET_STRING_Padding_Optional(_buffer, _size, _lidx, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_Header_FixPartShortSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_Header_FixPartShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->FramingInfo[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_LengthIndicator_LI11_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_LengthIndicator_LI11_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 11; i4++) {
		HTON_8(&_buffer[*_lidx], p->LengthIndicator[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_LengthIndicator_LI15_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_LengthIndicator_LI15_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 15; i4++) {
		HTON_8(&_buffer[*_lidx], p->LengthIndicator[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_LI_List_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RLC_LI_List_Type_Value* p, enum RLC_LI_List_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_LI_List_Type_LI11) {
		HTON_32(&_buffer[*_lidx], p->LI11.d, _lidx);
		for (size_t i3 = 0; i3 < p->LI11.d; i3++) {
			_serDrbEncRLC_LengthIndicator_LI11_Type(_buffer, _size, _lidx, &p->LI11.v[i3]);
		}
	}
	if (d == RLC_LI_List_Type_LI15) {
		HTON_32(&_buffer[*_lidx], p->LI15.d, _lidx);
		for (size_t i3 = 0; i3 < p->LI15.d; i3++) {
			_serDrbEncRLC_LengthIndicator_LI15_Type(_buffer, _size, _lidx, &p->LI15.v[i3]);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_LI_List_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_LI_List_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRLC_LI_List_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncB4_Type_Padding_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct B4_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_LI_List_Type(_buffer, _size, _lidx, &p->LengthIndicator);
	_serDrbEncB4_Type_Padding_Optional(_buffer, _size, _lidx, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_HeaderShortSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_HeaderShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_UMD_Header_FixPartShortSN_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(_buffer, _size, _lidx, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_PDU_ShortSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_PDU_ShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_UMD_HeaderShortSN_Type(_buffer, _size, _lidx, &p->Header);
	HTON_32(&_buffer[*_lidx], p->Data.d, _lidx);
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->Data.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->Data.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_Header_FixPartLongSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_Header_FixPartLongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->FramingInfo[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 10; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_HeaderLongSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_HeaderLongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_UMD_Header_FixPartLongSN_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(_buffer, _size, _lidx, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_PDU_LongSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_PDU_LongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_UMD_HeaderLongSN_Type(_buffer, _size, _lidx, &p->Header);
	HTON_32(&_buffer[*_lidx], p->Data.d, _lidx);
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->Data.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->Data.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_PDU_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RLC_UMD_PDU_Type_Value* p, enum RLC_UMD_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_UMD_PDU_Type_ShortSN) {
		_serDrbEncRLC_UMD_PDU_ShortSN_Type(_buffer, _size, _lidx, &p->ShortSN);
	}
	if (d == RLC_UMD_PDU_Type_LongSN) {
		_serDrbEncRLC_UMD_PDU_LongSN_Type(_buffer, _size, _lidx, &p->LongSN);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_UMD_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_UMD_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRLC_UMD_PDU_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_FixPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_FixPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->ReSeg[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Poll[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->FramingInfo[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 10; i3++) {
		HTON_8(&_buffer[*_lidx], p->SN[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_SegmentPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_SegmentPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->LastSegmentFlag[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->SegOffset[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_AMD_Header_SegmentPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_Header_FixPart_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbEncRLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(_buffer, _size, _lidx, &p->SegmentPart);
	_serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(_buffer, _size, _lidx, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_Header_Type(_buffer, _size, _lidx, &p->Header);
	HTON_32(&_buffer[*_lidx], p->Data.d, _lidx);
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->Data.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->Data.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_FixPartExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_FixPartExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->ReSeg[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Poll[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->FramingInfo[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->SN_Ext[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_HeaderExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_HeaderExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_Header_FixPartExt_Type(_buffer, _size, _lidx, &p->FixPartExt);
	_serDrbEncRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(_buffer, _size, _lidx, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_PDU_Ext_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_PDU_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_HeaderExt_Type(_buffer, _size, _lidx, &p->HeaderExt);
	HTON_32(&_buffer[*_lidx], p->Data.d, _lidx);
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->Data.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->Data.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_FixPartSegExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_FixPartSegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->ReSeg[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Poll[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->FramingInfo[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extension[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->LastSegmentFlag[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->SN_Ext[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_SegmentPartExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_SegmentPartExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->SegOffset[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_AMD_Header_SegmentPartExt_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_HeaderSegExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_HeaderSegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_Header_FixPartSegExt_Type(_buffer, _size, _lidx, &p->FixPartSegExt);
	_serDrbEncRLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(_buffer, _size, _lidx, &p->SegmentPart);
	_serDrbEncRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(_buffer, _size, _lidx, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AMD_PDU_SegExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AMD_PDU_SegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncRLC_AMD_HeaderSegExt_Type(_buffer, _size, _lidx, &p->HeaderSegExt);
	HTON_32(&_buffer[*_lidx], p->Data.d, _lidx);
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		HTON_32(&_buffer[*_lidx], p->Data.v[i3].d, _lidx);
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			HTON_8(&_buffer[*_lidx], p->Data.v[i3].v[i4], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_ACK_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_ACK_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 10; i3++) {
		HTON_8(&_buffer[*_lidx], p->ACK_SN[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extn1[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_SegOffset_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_SegOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 15; i4++) {
		HTON_8(&_buffer[*_lidx], p->Start[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 15; i4++) {
		HTON_8(&_buffer[*_lidx], p->End[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_SegOffset_Type_SO_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_SegOffset_Type_SO_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_Status_SegOffset_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_NACK_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_NACK_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 10; i4++) {
		HTON_8(&_buffer[*_lidx], p->NACK_SN[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extn1[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extn2[i4], _lidx);
	}
	_serDrbEncRLC_Status_SegOffset_Type_SO_Optional(_buffer, _size, _lidx, &p->SO);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_NACK_List_Type_NackList_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_NACK_List_Type_NackList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_serDrbEncRLC_Status_NACK_Type(_buffer, _size, _lidx, &p->v.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AM_StatusPDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AM_StatusPDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->CP_Type[i3], _lidx);
	}
	_serDrbEncRLC_Status_ACK_Type(_buffer, _size, _lidx, &p->Ack);
	_serDrbEncRLC_Status_NACK_List_Type_NackList_Optional(_buffer, _size, _lidx, &p->NackList);
	_serDrbEncRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(_buffer, _size, _lidx, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_ACK_Ext_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_ACK_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->ACK_SN_Ext[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Extn1[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_SegOffset_Ext_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_SegOffset_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 16; i4++) {
		HTON_8(&_buffer[*_lidx], p->Start[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 16; i4++) {
		HTON_8(&_buffer[*_lidx], p->End[i4], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_SegOffset_Ext_Type_SO_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_SegOffset_Ext_Type_SO_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRLC_Status_SegOffset_Ext_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_NACK_Ext_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_NACK_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 16; i4++) {
		HTON_8(&_buffer[*_lidx], p->NACK_SN_Ext[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extn1[i4], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		HTON_8(&_buffer[*_lidx], p->Extn2[i4], _lidx);
	}
	_serDrbEncRLC_Status_SegOffset_Ext_Type_SO_Optional(_buffer, _size, _lidx, &p->SO);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_serDrbEncRLC_Status_NACK_Ext_Type(_buffer, _size, _lidx, &p->v.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_AM_StatusPDU_Ext_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_AM_StatusPDU_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->CP_Type[i3], _lidx);
	}
	_serDrbEncRLC_Status_ACK_Ext_Type(_buffer, _size, _lidx, &p->Ack_Ext);
	_serDrbEncRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(_buffer, _size, _lidx, &p->Nack_Ext_List);
	_serDrbEncRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(_buffer, _size, _lidx, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RLC_PDU_Type_Value* p, enum RLC_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_PDU_Type_TMD) {
		HTON_32(&_buffer[*_lidx], p->TMD.d, _lidx);
		for (size_t i3 = 0; i3 < p->TMD.d; i3++) {
			HTON_8(&_buffer[*_lidx], p->TMD.v[i3], _lidx);
		}
	}
	if (d == RLC_PDU_Type_UMD) {
		_serDrbEncRLC_UMD_PDU_Type(_buffer, _size, _lidx, &p->UMD);
	}
	if (d == RLC_PDU_Type_AMD) {
		_serDrbEncRLC_AMD_PDU_Type(_buffer, _size, _lidx, &p->AMD);
	}
	if (d == RLC_PDU_Type_AMD_Ext) {
		_serDrbEncRLC_AMD_PDU_Ext_Type(_buffer, _size, _lidx, &p->AMD_Ext);
	}
	if (d == RLC_PDU_Type_AMD_SegExt) {
		_serDrbEncRLC_AMD_PDU_SegExt_Type(_buffer, _size, _lidx, &p->AMD_SegExt);
	}
	if (d == RLC_PDU_Type_Status) {
		_serDrbEncRLC_AM_StatusPDU_Type(_buffer, _size, _lidx, &p->Status);
	}
	if (d == RLC_PDU_Type_Status_Ext) {
		_serDrbEncRLC_AM_StatusPDU_Ext_Type(_buffer, _size, _lidx, &p->Status_Ext);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncRLC_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RLC_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRLC_PDU_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_LongSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_LongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_ShortSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_ShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 7; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_ExtSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_ExtSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_18bitSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->PollingBit[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_ROHC_FB_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_ROHC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->ROHC_FB.d, _lidx);
	for (size_t i3 = 0; i3 < p->ROHC_FB.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->ROHC_FB.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_StatusReport_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_StatusReport_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS[i3], _lidx);
	}
	_serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(_buffer, _size, _lidx, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_StatusReportExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_StatusReportExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS_Ext[i3], _lidx);
	}
	_serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(_buffer, _size, _lidx, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_StatusReport_18bitSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_StatusReport_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS_18bitSN[i3], _lidx);
	}
	_serDrbEncOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(_buffer, _size, _lidx, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_StatusReport_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_StatusReport_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->HRW[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->NMP[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_StatusReportExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_StatusReportExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS_Ext[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->HRW_Ext[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved3[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->NMP_Ext[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_StatusReport_18bitSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_StatusReport_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->FMS_18bitSN[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->HRW_18bitSN[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->NMP_18bitSN[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_SLRB_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_SLRB_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->PGK_Index[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->PTK_Identity[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_SLRB_1to1_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_SLRB_1to1_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->KD_sess_ID[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->MAC_I[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_EndMarker_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_EndMarker_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->LSN[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_EndMarkerExt_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_EndMarkerExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->LSN_Ext[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_LWA_EndMarker_18bitSN_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_LWA_EndMarker_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->LSN_18bitSN[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_LongSN_UDC_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_LongSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FU[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FR[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Checksum[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_ExtSN_UDC_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_ExtSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FU[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FR[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Checksum[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_DataPdu_18bitSN_UDC_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_DataPdu_18bitSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->PollingBit[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		HTON_8(&_buffer[*_lidx], p->SequenceNumber[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FU[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FR[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved2[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Checksum[i3], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SDU.d, _lidx);
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		HTON_8(&_buffer[*_lidx], p->SDU.v[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_UDC_FB_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_UDC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->FE[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_Ctrl_EHC_FB_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_Ctrl_EHC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->D_C[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		HTON_8(&_buffer[*_lidx], p->PDU_Type[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i3], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		HTON_8(&_buffer[*_lidx], p->EHC_FB[i3], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_PDU_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union PDCP_PDU_Type_Value* p, enum PDCP_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == PDCP_PDU_Type_DataLongSN) {
		_serDrbEncPDCP_DataPdu_LongSN_Type(_buffer, _size, _lidx, &p->DataLongSN);
	}
	if (d == PDCP_PDU_Type_DataShortSN) {
		_serDrbEncPDCP_DataPdu_ShortSN_Type(_buffer, _size, _lidx, &p->DataShortSN);
	}
	if (d == PDCP_PDU_Type_DataExtSN) {
		_serDrbEncPDCP_DataPdu_ExtSN_Type(_buffer, _size, _lidx, &p->DataExtSN);
	}
	if (d == PDCP_PDU_Type_Data_18bitSN) {
		_serDrbEncPDCP_DataPdu_18bitSN_Type(_buffer, _size, _lidx, &p->Data_18bitSN);
	}
	if (d == PDCP_PDU_Type_RohcFeedback) {
		_serDrbEncPDCP_Ctrl_ROHC_FB_PDU_Type(_buffer, _size, _lidx, &p->RohcFeedback);
	}
	if (d == PDCP_PDU_Type_StatusReport) {
		_serDrbEncPDCP_Ctrl_StatusReport_Type(_buffer, _size, _lidx, &p->StatusReport);
	}
	if (d == PDCP_PDU_Type_StatusReportExt) {
		_serDrbEncPDCP_Ctrl_StatusReportExt_Type(_buffer, _size, _lidx, &p->StatusReportExt);
	}
	if (d == PDCP_PDU_Type_StatusReport_18bitSN) {
		_serDrbEncPDCP_Ctrl_StatusReport_18bitSN_Type(_buffer, _size, _lidx, &p->StatusReport_18bitSN);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport) {
		_serDrbEncPDCP_Ctrl_LWA_StatusReport_Type(_buffer, _size, _lidx, &p->LWA_StatusReport);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReportExt) {
		_serDrbEncPDCP_Ctrl_LWA_StatusReportExt_Type(_buffer, _size, _lidx, &p->LWA_StatusReportExt);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport_18bitSN) {
		_serDrbEncPDCP_Ctrl_LWA_StatusReport_18bitSN_Type(_buffer, _size, _lidx, &p->LWA_StatusReport_18bitSN);
	}
	if (d == PDCP_PDU_Type_DataSLRB) {
		_serDrbEncPDCP_DataPdu_SLRB_Type(_buffer, _size, _lidx, &p->DataSLRB);
	}
	if (d == PDCP_PDU_Type_DataSLRB_1to1) {
		_serDrbEncPDCP_DataPdu_SLRB_1to1_Type(_buffer, _size, _lidx, &p->DataSLRB_1to1);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker) {
		_serDrbEncPDCP_Ctrl_LWA_EndMarker_Type(_buffer, _size, _lidx, &p->LWA_EndMarker);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarkerExt) {
		_serDrbEncPDCP_Ctrl_LWA_EndMarkerExt_Type(_buffer, _size, _lidx, &p->LWA_EndMarkerExt);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker_18bitSN) {
		_serDrbEncPDCP_Ctrl_LWA_EndMarker_18bitSN_Type(_buffer, _size, _lidx, &p->LWA_EndMarker_18bitSN);
	}
	if (d == PDCP_PDU_Type_DataLongSN_UDC) {
		_serDrbEncPDCP_DataPdu_LongSN_UDC_Type(_buffer, _size, _lidx, &p->DataLongSN_UDC);
	}
	if (d == PDCP_PDU_Type_DataExtSN_UDC) {
		_serDrbEncPDCP_DataPdu_ExtSN_UDC_Type(_buffer, _size, _lidx, &p->DataExtSN_UDC);
	}
	if (d == PDCP_PDU_Type_Data18bitSN_UDC) {
		_serDrbEncPDCP_DataPdu_18bitSN_UDC_Type(_buffer, _size, _lidx, &p->Data18bitSN_UDC);
	}
	if (d == PDCP_PDU_Type_UdcFeedback) {
		_serDrbEncPDCP_Ctrl_UDC_FB_PDU_Type(_buffer, _size, _lidx, &p->UdcFeedback);
	}
	if (d == PDCP_PDU_Type_EhcFeedback) {
		_serDrbEncPDCP_Ctrl_EHC_FB_PDU_Type(_buffer, _size, _lidx, &p->EhcFeedback);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncPDCP_PDU_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PDCP_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncPDCP_PDU_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncL2DataList_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union L2DataList_Type_Value* p, enum L2DataList_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == L2DataList_Type_MacPdu) {
		HTON_32(&_buffer[*_lidx], p->MacPdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->MacPdu.d; i2++) {
			_serDrbEncMAC_PDU_Type(_buffer, _size, _lidx, &p->MacPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_RlcPdu) {
		HTON_32(&_buffer[*_lidx], p->RlcPdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->RlcPdu.d; i2++) {
			_serDrbEncRLC_PDU_Type(_buffer, _size, _lidx, &p->RlcPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_PdcpPdu) {
		HTON_32(&_buffer[*_lidx], p->PdcpPdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->PdcpPdu.d; i2++) {
			_serDrbEncPDCP_PDU_Type(_buffer, _size, _lidx, &p->PdcpPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_PdcpSdu) {
		HTON_32(&_buffer[*_lidx], p->PdcpSdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->PdcpSdu.d; i2++) {
			HTON_32(&_buffer[*_lidx], p->PdcpSdu.v[i2].d, _lidx);
			for (size_t i3 = 0; i3 < p->PdcpSdu.v[i2].d; i3++) {
				HTON_8(&_buffer[*_lidx], p->PdcpSdu.v[i2].v[i3], _lidx);
			}
		}
	}
	if (d == L2DataList_Type_NrPdcpSdu) {
		HTON_32(&_buffer[*_lidx], p->NrPdcpSdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->NrPdcpSdu.d; i2++) {
			HTON_32(&_buffer[*_lidx], p->NrPdcpSdu.v[i2].d, _lidx);
			for (size_t i3 = 0; i3 < p->NrPdcpSdu.v[i2].d; i3++) {
				HTON_8(&_buffer[*_lidx], p->NrPdcpSdu.v[i2].v[i3], _lidx);
			}
		}
	}
	if (d == L2DataList_Type_RlcSdu) {
		HTON_32(&_buffer[*_lidx], p->RlcSdu.d, _lidx);
		for (size_t i2 = 0; i2 < p->RlcSdu.d; i2++) {
			HTON_32(&_buffer[*_lidx], p->RlcSdu.v[i2].d, _lidx);
			for (size_t i3 = 0; i3 < p->RlcSdu.v[i2].d; i3++) {
				HTON_8(&_buffer[*_lidx], p->RlcSdu.v[i2].v[i3], _lidx);
			}
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncL2DataList_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct L2DataList_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncL2DataList_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncDRB_DataPerSubframe_DL_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_DataPerSubframe_DL_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->SubframeOffset, _lidx);
	_serDrbEncHarqProcessAssignment_Type_HarqProcess_Optional(_buffer, _size, _lidx, &p->HarqProcess);
	_serDrbEncL2DataList_Type(_buffer, _size, _lidx, &p->PduSduList);

	return SIDL_STATUS_OK;
}

static int _serDrbEncL2Data_Request_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct L2Data_Request_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->SubframeDataList.d, _lidx);
	for (size_t i1 = 0; i1 < p->SubframeDataList.d; i1++) {
		_serDrbEncDRB_DataPerSubframe_DL_Type(_buffer, _size, _lidx, &p->SubframeDataList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncNull_Type_SuppressPdcchForC_RNTI_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Null_Type_SuppressPdcchForC_RNTI_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_8(&_buffer[*_lidx], p->v, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncDRB_COMMON_REQ(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_COMMON_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncReqAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serDrbEncL2Data_Request_Type(_buffer, _size, _lidx, &p->U_Plane);
	_serDrbEncNull_Type_SuppressPdcchForC_RNTI_Optional(_buffer, _size, _lidx, &p->SuppressPdcchForC_RNTI);

	return SIDL_STATUS_OK;
}

int serDrbProcessFromSSEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_COMMON_REQ* FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncDRB_COMMON_REQ(_buffer, _size, _lidx, FromSS);

	return SIDL_STATUS_OK;
}

static int _serDrbDecPmchLogicalChannel_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Pmch_InfoIndex, &_buffer[*_lidx], _lidx);
	NTOH_8(p->LogicalChannelIdentity, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Mbsfn_AreaId, &_buffer[*_lidx], _lidx);
	_serDrbDecPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMCC_PLMN_Identity_mcc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPLMN_Identity(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	NTOH_32(p->mnc.d, &_buffer[*_lidx], _lidx);
	p->mnc.v = serMalloc(_mem, p->mnc.d * sizeof(MCC_MNC_Digit));
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		NTOH_8(p->mnc.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecTMGI_r9_plmn_Id_r9_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		NTOH_8(p->plmn_Index_r9, &_buffer[*_lidx], _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serDrbDecPLMN_Identity(_buffer, _size, _lidx, _mem, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecTMGI_r9_plmn_Id_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TMGI_r9_plmn_Id_r9_Sel)_tmp;
	}
	_serDrbDecTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecTMGI_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, _mem, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->serviceId_r9[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMBMSSessionInfo_r13(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecTMGI_r9(_buffer, _size, _lidx, _mem, &p->tmgi_r13);
	_serDrbDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSC_MRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecMBMSSessionInfo_r13(_buffer, _size, _lidx, _mem, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRadioBearerId_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		NTOH_8(p->Srb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		NTOH_8(p->Drb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serDrbDecMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serDrbDecSC_MRB_Identity_Type(_buffer, _size, _lidx, _mem, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRadioBearerId_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RadioBearerId_Type_Sel)_tmp;
	}
	_serDrbDecRadioBearerId_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecQosFlow_Identification_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->PDU_SessionId, &_buffer[*_lidx], _lidx);
	NTOH_32(p->QFI, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRoutingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serDrbDecRadioBearerId_Type(_buffer, _size, _lidx, _mem, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serDrbDecQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRoutingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RoutingInfo_Type_Sel)_tmp;
	}
	_serDrbDecRoutingInfo_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSystemFrameNumberInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SystemFrameNumberInfo_Type_Number) {
		NTOH_16(p->Number, &_buffer[*_lidx], _lidx);
	}
	if (d == SystemFrameNumberInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serDrbDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSubFrameInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SubFrameInfo_Type_Number) {
		NTOH_8(p->Number, &_buffer[*_lidx], _lidx);
	}
	if (d == SubFrameInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecSubFrameInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SubFrameInfo_Type_Sel)_tmp;
	}
	_serDrbDecSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecHyperSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serDrbDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSlotOffset_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotOffset_Type_Numerology0) {
		NTOH_8(p->Numerology0, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotOffset_Type_Numerology1) {
		NTOH_8(p->Numerology1, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotOffset_Type_Numerology2) {
		NTOH_8(p->Numerology2, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotOffset_Type_Numerology3) {
		NTOH_8(p->Numerology3, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotOffset_Type_Numerology4) {
		NTOH_8(p->Numerology4, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecSlotOffset_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotOffset_Type_Sel)_tmp;
	}
	_serDrbDecSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSlotTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serDrbDecSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		NTOH_8(p->FirstSlot, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecSlotTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotTimingInfo_Type_Sel)_tmp;
	}
	_serDrbDecSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecSubFrameTiming_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serDrbDecSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serDrbDecHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serDrbDecSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serDrbDecTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serDrbDecSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		NTOH_8(p->Now, &_buffer[*_lidx], _lidx);
	}
	if (d == TimingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TimingInfo_Type_Sel)_tmp;
	}
	_serDrbDecTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecReqAspControlInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->CnfFlag, &_buffer[*_lidx], _lidx);
	NTOH_8(p->FollowOnFlag, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRlcBearerRouting_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RlcBearerRouting_Type_EUTRA) {
		{
			size_t _tmp;
			NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
			p->EUTRA = (EUTRA_CellId_Type)_tmp;
		}
	}
	if (d == RlcBearerRouting_Type_NR) {
		{
			size_t _tmp;
			NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
			p->NR = (NR_CellId_Type)_tmp;
		}
	}
	if (d == RlcBearerRouting_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRlcBearerRouting_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RlcBearerRouting_Type_Sel)_tmp;
	}
	_serDrbDecRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecReqAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serDrbDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serDrbDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serDrbDecReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serDrbDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serDrbDecHarqProcessAssignment_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union HarqProcessAssignment_Type_Value* p, enum HarqProcessAssignment_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == HarqProcessAssignment_Type_Id) {
		NTOH_8(p->Id, &_buffer[*_lidx], _lidx);
	}
	if (d == HarqProcessAssignment_Type_Automatic) {
		NTOH_8(p->Automatic, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecHarqProcessAssignment_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct HarqProcessAssignment_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum HarqProcessAssignment_Type_Sel)_tmp;
	}
	_serDrbDecHarqProcessAssignment_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecHarqProcessAssignment_Type_HarqProcess_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct HarqProcessAssignment_Type_HarqProcess_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecHarqProcessAssignment_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_PDU_Length_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_PDU_Length_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Format[i4], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->Value.d, &_buffer[*_lidx], _lidx);
	p->Value.v = serMalloc(_mem, p->Value.d * sizeof(BIT_STRING_ELEMENT));
	for (size_t i4 = 0; i4 < p->Value.d; i4++) {
		NTOH_8(p->Value.v[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_PDU_Length_Type_Length_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_PDU_Length_Type_Length_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_PDU_Length_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_PDU_SubHeader_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_PDU_SubHeader_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 2; i4++) {
		NTOH_8(p->Reserved[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extension[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 5; i4++) {
		NTOH_8(p->LCID[i4], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecMAC_PDU_Length_Type_Length_Optional(_buffer, _size, _lidx, _mem, &p->Length);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ShortBSR_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_ShortBSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->LCG[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ShortBSR_Type_ShortBSR_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_ShortBSR_Type_ShortBSR_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_ShortBSR_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_LongBSR_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_LongBSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value_LCG1[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value_LCG2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value_LCG3[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value_LCG4[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_LongBSR_Type_LongBSR_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_LongBSR_Type_LongBSR_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_LongBSR_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_C_RNTI_Type_C_RNTI_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_C_RNTI_Type_C_RNTI_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(BIT_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 8; i3++) {
		NTOH_8(p->v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_PowerHeadRoom_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 6; i3++) {
		NTOH_8(p->Value[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_PowerHeadRoom_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ScellActDeact_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, MAC_CTRL_ScellActDeact_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 7; i3++) {
		NTOH_8(p->Value[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_ScellActDeact_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecScellBitMap_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ScellBitMap_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 7; i3++) {
		NTOH_8(p->Value[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecB2_Type_Reserved_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct B2_Type_Reserved_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i4 = 0; i4 < 2; i4++) {
		NTOH_8(p->v[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecB6_Type_PCMaxc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct B6_Type_PCMaxc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i4 = 0; i4 < 6; i4++) {
		NTOH_8(p->v[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPH_Record_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PH_Record_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->P_Bit[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->V_Bit[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 6; i4++) {
		NTOH_8(p->Valve[i4], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecB2_Type_Reserved_Optional(_buffer, _size, _lidx, &p->Reserved);
	_serDrbDecB6_Type_PCMaxc_Optional(_buffer, _size, _lidx, &p->PCMaxc);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ExtPowerHeadRoom_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecScellBitMap_Type(_buffer, _size, _lidx, &p->EPH_Octet1);
	NTOH_32(p->PH_RecordList.d, &_buffer[*_lidx], _lidx);
	p->PH_RecordList.v = serMalloc(_mem, p->PH_RecordList.d * sizeof(struct PH_Record_Type));
	for (size_t i3 = 0; i3 < p->PH_RecordList.d; i3++) {
		_serDrbDecPH_Record_Type(_buffer, _size, _lidx, &p->PH_RecordList.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_ExtPowerHeadRoom_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_DC_PowerHeadRoom_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecScellBitMap_Type(_buffer, _size, _lidx, &p->DC_PH_Octet1);
	NTOH_32(p->DC_PH_RecordList.d, &_buffer[*_lidx], _lidx);
	p->DC_PH_RecordList.v = serMalloc(_mem, p->DC_PH_RecordList.d * sizeof(struct PH_Record_Type));
	for (size_t i3 = 0; i3 < p->DC_PH_RecordList.d; i3++) {
		_serDrbDecPH_Record_Type(_buffer, _size, _lidx, &p->DC_PH_RecordList.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_DC_PowerHeadRoom_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ElementList_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ElementList_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecMAC_CTRL_ShortBSR_Type_ShortBSR_Optional(_buffer, _size, _lidx, &p->ShortBSR);
	_serDrbDecMAC_CTRL_LongBSR_Type_LongBSR_Optional(_buffer, _size, _lidx, &p->LongBSR);
	_serDrbDecMAC_CTRL_C_RNTI_Type_C_RNTI_Optional(_buffer, _size, _lidx, &p->C_RNTI);
	_serDrbDecMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(_buffer, _size, _lidx, _mem, &p->ContentionResolutionID);
	_serDrbDecMAC_CTRL_TimingAdvance_Type_TimingAdvance_Optional(_buffer, _size, _lidx, &p->TimingAdvance);
	_serDrbDecMAC_CTRL_PowerHeadRoom_Type_PowerHeadRoom_Optional(_buffer, _size, _lidx, &p->PowerHeadRoom);
	_serDrbDecMAC_CTRL_ScellActDeact_Type_ScellActDeact_Optional(_buffer, _size, _lidx, &p->ScellActDeact);
	_serDrbDecMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(_buffer, _size, _lidx, _mem, &p->ExtPowerHeadRoom);
	_serDrbDecMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(_buffer, _size, _lidx, _mem, &p->DC_PowerHeadRoom);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_CTRL_ElementList_Type_CtrlElementList_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecMAC_CTRL_ElementList_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_SDUList_Type_SduList_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_SDUList_Type_SduList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(MAC_SDU_Type));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_32(p->v.v[i3].d, &_buffer[*_lidx], _lidx);
		p->v.v[i3].v = serMalloc(_mem, p->v.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->v.v[i3].d; i4++) {
			NTOH_8(p->v.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecOCTET_STRING_Padding_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct OCTET_STRING_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecMAC_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->Header.d, &_buffer[*_lidx], _lidx);
	p->Header.v = serMalloc(_mem, p->Header.d * sizeof(struct MAC_PDU_SubHeader_Type));
	for (size_t i3 = 0; i3 < p->Header.d; i3++) {
		_serDrbDecMAC_PDU_SubHeader_Type(_buffer, _size, _lidx, _mem, &p->Header.v[i3]);
	}
	_serDrbDecMAC_CTRL_ElementList_Type_CtrlElementList_Optional(_buffer, _size, _lidx, _mem, &p->CtrlElementList);
	_serDrbDecMAC_SDUList_Type_SduList_Optional(_buffer, _size, _lidx, _mem, &p->SduList);
	_serDrbDecOCTET_STRING_Padding_Optional(_buffer, _size, _lidx, _mem, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_Header_FixPartShortSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_UMD_Header_FixPartShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->FramingInfo[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extension[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_LengthIndicator_LI11_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_LengthIndicator_LI11_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extension[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 11; i4++) {
		NTOH_8(p->LengthIndicator[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_LengthIndicator_LI15_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_LengthIndicator_LI15_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extension[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 15; i4++) {
		NTOH_8(p->LengthIndicator[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_LI_List_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RLC_LI_List_Type_Value* p, enum RLC_LI_List_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_LI_List_Type_LI11) {
		NTOH_32(p->LI11.d, &_buffer[*_lidx], _lidx);
		p->LI11.v = serMalloc(_mem, p->LI11.d * sizeof(struct RLC_LengthIndicator_LI11_Type));
		for (size_t i3 = 0; i3 < p->LI11.d; i3++) {
			_serDrbDecRLC_LengthIndicator_LI11_Type(_buffer, _size, _lidx, &p->LI11.v[i3]);
		}
	}
	if (d == RLC_LI_List_Type_LI15) {
		NTOH_32(p->LI15.d, &_buffer[*_lidx], _lidx);
		p->LI15.v = serMalloc(_mem, p->LI15.d * sizeof(struct RLC_LengthIndicator_LI15_Type));
		for (size_t i3 = 0; i3 < p->LI15.d; i3++) {
			_serDrbDecRLC_LengthIndicator_LI15_Type(_buffer, _size, _lidx, &p->LI15.v[i3]);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_LI_List_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_LI_List_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RLC_LI_List_Type_Sel)_tmp;
	}
	_serDrbDecRLC_LI_List_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecB4_Type_Padding_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct B4_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_LI_List_Type(_buffer, _size, _lidx, _mem, &p->LengthIndicator);
	_serDrbDecB4_Type_Padding_Optional(_buffer, _size, _lidx, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_HeaderShortSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_UMD_HeaderShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_UMD_Header_FixPartShortSN_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(_buffer, _size, _lidx, _mem, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_PDU_ShortSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_UMD_PDU_ShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_UMD_HeaderShortSN_Type(_buffer, _size, _lidx, _mem, &p->Header);
	NTOH_32(p->Data.d, &_buffer[*_lidx], _lidx);
	p->Data.v = serMalloc(_mem, p->Data.d * sizeof(RLC_DataField_Type));
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		NTOH_32(p->Data.v[i3].d, &_buffer[*_lidx], _lidx);
		p->Data.v[i3].v = serMalloc(_mem, p->Data.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			NTOH_8(p->Data.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_Header_FixPartLongSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_UMD_Header_FixPartLongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->FramingInfo[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extension[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 10; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_HeaderLongSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_UMD_HeaderLongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_UMD_Header_FixPartLongSN_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(_buffer, _size, _lidx, _mem, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_PDU_LongSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_UMD_PDU_LongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_UMD_HeaderLongSN_Type(_buffer, _size, _lidx, _mem, &p->Header);
	NTOH_32(p->Data.d, &_buffer[*_lidx], _lidx);
	p->Data.v = serMalloc(_mem, p->Data.d * sizeof(RLC_DataField_Type));
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		NTOH_32(p->Data.v[i3].d, &_buffer[*_lidx], _lidx);
		p->Data.v[i3].v = serMalloc(_mem, p->Data.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			NTOH_8(p->Data.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_PDU_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RLC_UMD_PDU_Type_Value* p, enum RLC_UMD_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_UMD_PDU_Type_ShortSN) {
		_serDrbDecRLC_UMD_PDU_ShortSN_Type(_buffer, _size, _lidx, _mem, &p->ShortSN);
	}
	if (d == RLC_UMD_PDU_Type_LongSN) {
		_serDrbDecRLC_UMD_PDU_LongSN_Type(_buffer, _size, _lidx, _mem, &p->LongSN);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_UMD_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_UMD_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RLC_UMD_PDU_Type_Sel)_tmp;
	}
	_serDrbDecRLC_UMD_PDU_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_FixPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_FixPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->ReSeg[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Poll[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->FramingInfo[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extension[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 10; i3++) {
		NTOH_8(p->SN[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_SegmentPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_SegmentPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->LastSegmentFlag[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->SegOffset[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_AMD_Header_SegmentPart_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_Header_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_Header_FixPart_Type(_buffer, _size, _lidx, &p->FixPart);
	_serDrbDecRLC_AMD_Header_SegmentPart_Type_SegmentPart_Optional(_buffer, _size, _lidx, &p->SegmentPart);
	_serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(_buffer, _size, _lidx, _mem, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_Header_Type(_buffer, _size, _lidx, _mem, &p->Header);
	NTOH_32(p->Data.d, &_buffer[*_lidx], _lidx);
	p->Data.v = serMalloc(_mem, p->Data.d * sizeof(RLC_DataField_Type));
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		NTOH_32(p->Data.v[i3].d, &_buffer[*_lidx], _lidx);
		p->Data.v[i3].v = serMalloc(_mem, p->Data.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			NTOH_8(p->Data.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_FixPartExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_FixPartExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->ReSeg[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Poll[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->FramingInfo[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extension[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->SN_Ext[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_HeaderExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_HeaderExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_Header_FixPartExt_Type(_buffer, _size, _lidx, &p->FixPartExt);
	_serDrbDecRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(_buffer, _size, _lidx, _mem, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_PDU_Ext_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_PDU_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_HeaderExt_Type(_buffer, _size, _lidx, _mem, &p->HeaderExt);
	NTOH_32(p->Data.d, &_buffer[*_lidx], _lidx);
	p->Data.v = serMalloc(_mem, p->Data.d * sizeof(RLC_DataField_Type));
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		NTOH_32(p->Data.v[i3].d, &_buffer[*_lidx], _lidx);
		p->Data.v[i3].v = serMalloc(_mem, p->Data.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			NTOH_8(p->Data.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_FixPartSegExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_FixPartSegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->ReSeg[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Poll[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->FramingInfo[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extension[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->LastSegmentFlag[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->SN_Ext[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_SegmentPartExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_SegmentPartExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->SegOffset[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_AMD_Header_SegmentPartExt_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_PDU_Header_FlexPart_Type(_buffer, _size, _lidx, _mem, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_HeaderSegExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_HeaderSegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_Header_FixPartSegExt_Type(_buffer, _size, _lidx, &p->FixPartSegExt);
	_serDrbDecRLC_AMD_Header_SegmentPartExt_Type_SegmentPart_Optional(_buffer, _size, _lidx, &p->SegmentPart);
	_serDrbDecRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(_buffer, _size, _lidx, _mem, &p->FlexPart);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AMD_PDU_SegExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AMD_PDU_SegExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecRLC_AMD_HeaderSegExt_Type(_buffer, _size, _lidx, _mem, &p->HeaderSegExt);
	NTOH_32(p->Data.d, &_buffer[*_lidx], _lidx);
	p->Data.v = serMalloc(_mem, p->Data.d * sizeof(RLC_DataField_Type));
	for (size_t i3 = 0; i3 < p->Data.d; i3++) {
		NTOH_32(p->Data.v[i3].d, &_buffer[*_lidx], _lidx);
		p->Data.v[i3].v = serMalloc(_mem, p->Data.v[i3].d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i4 = 0; i4 < p->Data.v[i3].d; i4++) {
			NTOH_8(p->Data.v[i3].v[i4], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_ACK_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_ACK_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 10; i3++) {
		NTOH_8(p->ACK_SN[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extn1[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_SegOffset_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_SegOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 15; i4++) {
		NTOH_8(p->Start[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 15; i4++) {
		NTOH_8(p->End[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_SegOffset_Type_SO_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_SegOffset_Type_SO_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_Status_SegOffset_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_NACK_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_NACK_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 10; i4++) {
		NTOH_8(p->NACK_SN[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extn1[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extn2[i4], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecRLC_Status_SegOffset_Type_SO_Optional(_buffer, _size, _lidx, &p->SO);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_NACK_List_Type_NackList_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_Status_NACK_List_Type_NackList_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct RLC_Status_NACK_Type));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_serDrbDecRLC_Status_NACK_Type(_buffer, _size, _lidx, &p->v.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(BIT_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AM_StatusPDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AM_StatusPDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->CP_Type[i3], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecRLC_Status_ACK_Type(_buffer, _size, _lidx, &p->Ack);
	_serDrbDecRLC_Status_NACK_List_Type_NackList_Optional(_buffer, _size, _lidx, _mem, &p->NackList);
	_serDrbDecRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(_buffer, _size, _lidx, _mem, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_ACK_Ext_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_ACK_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->ACK_SN_Ext[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Extn1[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_SegOffset_Ext_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_SegOffset_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 16; i4++) {
		NTOH_8(p->Start[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 16; i4++) {
		NTOH_8(p->End[i4], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_SegOffset_Ext_Type_SO_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_SegOffset_Ext_Type_SO_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRLC_Status_SegOffset_Ext_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_NACK_Ext_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RLC_Status_NACK_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i4 = 0; i4 < 16; i4++) {
		NTOH_8(p->NACK_SN_Ext[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extn1[i4], &_buffer[*_lidx], _lidx);
	}
	for (size_t i4 = 0; i4 < 1; i4++) {
		NTOH_8(p->Extn2[i4], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecRLC_Status_SegOffset_Ext_Type_SO_Optional(_buffer, _size, _lidx, &p->SO);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct RLC_Status_NACK_Ext_Type));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		_serDrbDecRLC_Status_NACK_Ext_Type(_buffer, _size, _lidx, &p->v.v[i3]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(BIT_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_AM_StatusPDU_Ext_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_AM_StatusPDU_Ext_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->CP_Type[i3], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecRLC_Status_ACK_Ext_Type(_buffer, _size, _lidx, &p->Ack_Ext);
	_serDrbDecRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(_buffer, _size, _lidx, _mem, &p->Nack_Ext_List);
	_serDrbDecRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(_buffer, _size, _lidx, _mem, &p->Padding);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RLC_PDU_Type_Value* p, enum RLC_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RLC_PDU_Type_TMD) {
		NTOH_32(p->TMD.d, &_buffer[*_lidx], _lidx);
		p->TMD.v = serMalloc(_mem, p->TMD.d * sizeof(OCTET_STRING_ELEMENT));
		for (size_t i3 = 0; i3 < p->TMD.d; i3++) {
			NTOH_8(p->TMD.v[i3], &_buffer[*_lidx], _lidx);
		}
	}
	if (d == RLC_PDU_Type_UMD) {
		_serDrbDecRLC_UMD_PDU_Type(_buffer, _size, _lidx, _mem, &p->UMD);
	}
	if (d == RLC_PDU_Type_AMD) {
		_serDrbDecRLC_AMD_PDU_Type(_buffer, _size, _lidx, _mem, &p->AMD);
	}
	if (d == RLC_PDU_Type_AMD_Ext) {
		_serDrbDecRLC_AMD_PDU_Ext_Type(_buffer, _size, _lidx, _mem, &p->AMD_Ext);
	}
	if (d == RLC_PDU_Type_AMD_SegExt) {
		_serDrbDecRLC_AMD_PDU_SegExt_Type(_buffer, _size, _lidx, _mem, &p->AMD_SegExt);
	}
	if (d == RLC_PDU_Type_Status) {
		_serDrbDecRLC_AM_StatusPDU_Type(_buffer, _size, _lidx, _mem, &p->Status);
	}
	if (d == RLC_PDU_Type_Status_Ext) {
		_serDrbDecRLC_AM_StatusPDU_Ext_Type(_buffer, _size, _lidx, _mem, &p->Status_Ext);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecRLC_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RLC_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RLC_PDU_Type_Sel)_tmp;
	}
	_serDrbDecRLC_PDU_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_LongSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_LongSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_ShortSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_ShortSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 7; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_ExtSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_ExtSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_18bitSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->PollingBit[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_ROHC_FB_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_Ctrl_ROHC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->ROHC_FB.d, &_buffer[*_lidx], _lidx);
	p->ROHC_FB.v = serMalloc(_mem, p->ROHC_FB.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->ROHC_FB.d; i3++) {
		NTOH_8(p->ROHC_FB.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_StatusReport_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_Ctrl_StatusReport_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->FMS[i3], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(_buffer, _size, _lidx, _mem, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_StatusReportExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_Ctrl_StatusReportExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->FMS_Ext[i3], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(_buffer, _size, _lidx, _mem, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->v.d; i3++) {
		NTOH_8(p->v.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_StatusReport_18bitSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_Ctrl_StatusReport_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->FMS_18bitSN[i3], &_buffer[*_lidx], _lidx);
	}
	_serDrbDecOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(_buffer, _size, _lidx, _mem, &p->Bitmap);

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_StatusReport_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_StatusReport_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->FMS[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->HRW[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->NMP[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_StatusReportExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_StatusReportExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->FMS_Ext[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Reserved2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->HRW_Ext[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->Reserved3[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->NMP_Ext[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_StatusReport_18bitSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_StatusReport_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->FMS_18bitSN[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->HRW_18bitSN[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Reserved2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->NMP_18bitSN[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_SLRB_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_SLRB_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->SDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->PGK_Index[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->PTK_Identity[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_SLRB_1to1_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_SLRB_1to1_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->SDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->KD_sess_ID[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 16; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->MAC_I[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_EndMarker_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_EndMarker_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->LSN[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_EndMarkerExt_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_EndMarkerExt_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 5; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->LSN_Ext[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_LWA_EndMarker_18bitSN_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_LWA_EndMarker_18bitSN_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->LSN_18bitSN[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_LongSN_UDC_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_LongSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 12; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FU[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FR[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Checksum[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_ExtSN_UDC_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_ExtSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 15; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FU[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FR[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Checksum[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_DataPdu_18bitSN_UDC_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_DataPdu_18bitSN_UDC_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->PollingBit[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 18; i3++) {
		NTOH_8(p->SequenceNumber[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FU[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FR[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 2; i3++) {
		NTOH_8(p->Reserved2[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Checksum[i3], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SDU.d, &_buffer[*_lidx], _lidx);
	p->SDU.v = serMalloc(_mem, p->SDU.d * sizeof(OCTET_STRING_ELEMENT));
	for (size_t i3 = 0; i3 < p->SDU.d; i3++) {
		NTOH_8(p->SDU.v[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_UDC_FB_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_UDC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->FE[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_Ctrl_EHC_FB_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PDCP_Ctrl_EHC_FB_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->D_C[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 3; i3++) {
		NTOH_8(p->PDU_Type[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 4; i3++) {
		NTOH_8(p->Reserved[i3], &_buffer[*_lidx], _lidx);
	}
	for (size_t i3 = 0; i3 < 1; i3++) {
		NTOH_8(p->EHC_FB[i3], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_PDU_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union PDCP_PDU_Type_Value* p, enum PDCP_PDU_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == PDCP_PDU_Type_DataLongSN) {
		_serDrbDecPDCP_DataPdu_LongSN_Type(_buffer, _size, _lidx, _mem, &p->DataLongSN);
	}
	if (d == PDCP_PDU_Type_DataShortSN) {
		_serDrbDecPDCP_DataPdu_ShortSN_Type(_buffer, _size, _lidx, _mem, &p->DataShortSN);
	}
	if (d == PDCP_PDU_Type_DataExtSN) {
		_serDrbDecPDCP_DataPdu_ExtSN_Type(_buffer, _size, _lidx, _mem, &p->DataExtSN);
	}
	if (d == PDCP_PDU_Type_Data_18bitSN) {
		_serDrbDecPDCP_DataPdu_18bitSN_Type(_buffer, _size, _lidx, _mem, &p->Data_18bitSN);
	}
	if (d == PDCP_PDU_Type_RohcFeedback) {
		_serDrbDecPDCP_Ctrl_ROHC_FB_PDU_Type(_buffer, _size, _lidx, _mem, &p->RohcFeedback);
	}
	if (d == PDCP_PDU_Type_StatusReport) {
		_serDrbDecPDCP_Ctrl_StatusReport_Type(_buffer, _size, _lidx, _mem, &p->StatusReport);
	}
	if (d == PDCP_PDU_Type_StatusReportExt) {
		_serDrbDecPDCP_Ctrl_StatusReportExt_Type(_buffer, _size, _lidx, _mem, &p->StatusReportExt);
	}
	if (d == PDCP_PDU_Type_StatusReport_18bitSN) {
		_serDrbDecPDCP_Ctrl_StatusReport_18bitSN_Type(_buffer, _size, _lidx, _mem, &p->StatusReport_18bitSN);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport) {
		_serDrbDecPDCP_Ctrl_LWA_StatusReport_Type(_buffer, _size, _lidx, &p->LWA_StatusReport);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReportExt) {
		_serDrbDecPDCP_Ctrl_LWA_StatusReportExt_Type(_buffer, _size, _lidx, &p->LWA_StatusReportExt);
	}
	if (d == PDCP_PDU_Type_LWA_StatusReport_18bitSN) {
		_serDrbDecPDCP_Ctrl_LWA_StatusReport_18bitSN_Type(_buffer, _size, _lidx, &p->LWA_StatusReport_18bitSN);
	}
	if (d == PDCP_PDU_Type_DataSLRB) {
		_serDrbDecPDCP_DataPdu_SLRB_Type(_buffer, _size, _lidx, _mem, &p->DataSLRB);
	}
	if (d == PDCP_PDU_Type_DataSLRB_1to1) {
		_serDrbDecPDCP_DataPdu_SLRB_1to1_Type(_buffer, _size, _lidx, _mem, &p->DataSLRB_1to1);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker) {
		_serDrbDecPDCP_Ctrl_LWA_EndMarker_Type(_buffer, _size, _lidx, &p->LWA_EndMarker);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarkerExt) {
		_serDrbDecPDCP_Ctrl_LWA_EndMarkerExt_Type(_buffer, _size, _lidx, &p->LWA_EndMarkerExt);
	}
	if (d == PDCP_PDU_Type_LWA_EndMarker_18bitSN) {
		_serDrbDecPDCP_Ctrl_LWA_EndMarker_18bitSN_Type(_buffer, _size, _lidx, &p->LWA_EndMarker_18bitSN);
	}
	if (d == PDCP_PDU_Type_DataLongSN_UDC) {
		_serDrbDecPDCP_DataPdu_LongSN_UDC_Type(_buffer, _size, _lidx, _mem, &p->DataLongSN_UDC);
	}
	if (d == PDCP_PDU_Type_DataExtSN_UDC) {
		_serDrbDecPDCP_DataPdu_ExtSN_UDC_Type(_buffer, _size, _lidx, _mem, &p->DataExtSN_UDC);
	}
	if (d == PDCP_PDU_Type_Data18bitSN_UDC) {
		_serDrbDecPDCP_DataPdu_18bitSN_UDC_Type(_buffer, _size, _lidx, _mem, &p->Data18bitSN_UDC);
	}
	if (d == PDCP_PDU_Type_UdcFeedback) {
		_serDrbDecPDCP_Ctrl_UDC_FB_PDU_Type(_buffer, _size, _lidx, &p->UdcFeedback);
	}
	if (d == PDCP_PDU_Type_EhcFeedback) {
		_serDrbDecPDCP_Ctrl_EHC_FB_PDU_Type(_buffer, _size, _lidx, &p->EhcFeedback);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecPDCP_PDU_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PDCP_PDU_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum PDCP_PDU_Type_Sel)_tmp;
	}
	_serDrbDecPDCP_PDU_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecL2DataList_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union L2DataList_Type_Value* p, enum L2DataList_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == L2DataList_Type_MacPdu) {
		NTOH_32(p->MacPdu.d, &_buffer[*_lidx], _lidx);
		p->MacPdu.v = serMalloc(_mem, p->MacPdu.d * sizeof(struct MAC_PDU_Type));
		for (size_t i2 = 0; i2 < p->MacPdu.d; i2++) {
			_serDrbDecMAC_PDU_Type(_buffer, _size, _lidx, _mem, &p->MacPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_RlcPdu) {
		NTOH_32(p->RlcPdu.d, &_buffer[*_lidx], _lidx);
		p->RlcPdu.v = serMalloc(_mem, p->RlcPdu.d * sizeof(struct RLC_PDU_Type));
		for (size_t i2 = 0; i2 < p->RlcPdu.d; i2++) {
			_serDrbDecRLC_PDU_Type(_buffer, _size, _lidx, _mem, &p->RlcPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_PdcpPdu) {
		NTOH_32(p->PdcpPdu.d, &_buffer[*_lidx], _lidx);
		p->PdcpPdu.v = serMalloc(_mem, p->PdcpPdu.d * sizeof(struct PDCP_PDU_Type));
		for (size_t i2 = 0; i2 < p->PdcpPdu.d; i2++) {
			_serDrbDecPDCP_PDU_Type(_buffer, _size, _lidx, _mem, &p->PdcpPdu.v[i2]);
		}
	}
	if (d == L2DataList_Type_PdcpSdu) {
		NTOH_32(p->PdcpSdu.d, &_buffer[*_lidx], _lidx);
		p->PdcpSdu.v = serMalloc(_mem, p->PdcpSdu.d * sizeof(PDCP_SDU_Type));
		for (size_t i2 = 0; i2 < p->PdcpSdu.d; i2++) {
			NTOH_32(p->PdcpSdu.v[i2].d, &_buffer[*_lidx], _lidx);
			p->PdcpSdu.v[i2].v = serMalloc(_mem, p->PdcpSdu.v[i2].d * sizeof(OCTET_STRING_ELEMENT));
			for (size_t i3 = 0; i3 < p->PdcpSdu.v[i2].d; i3++) {
				NTOH_8(p->PdcpSdu.v[i2].v[i3], &_buffer[*_lidx], _lidx);
			}
		}
	}
	if (d == L2DataList_Type_NrPdcpSdu) {
		NTOH_32(p->NrPdcpSdu.d, &_buffer[*_lidx], _lidx);
		p->NrPdcpSdu.v = serMalloc(_mem, p->NrPdcpSdu.d * sizeof(PDCP_SDU_Type));
		for (size_t i2 = 0; i2 < p->NrPdcpSdu.d; i2++) {
			NTOH_32(p->NrPdcpSdu.v[i2].d, &_buffer[*_lidx], _lidx);
			p->NrPdcpSdu.v[i2].v = serMalloc(_mem, p->NrPdcpSdu.v[i2].d * sizeof(OCTET_STRING_ELEMENT));
			for (size_t i3 = 0; i3 < p->NrPdcpSdu.v[i2].d; i3++) {
				NTOH_8(p->NrPdcpSdu.v[i2].v[i3], &_buffer[*_lidx], _lidx);
			}
		}
	}
	if (d == L2DataList_Type_RlcSdu) {
		NTOH_32(p->RlcSdu.d, &_buffer[*_lidx], _lidx);
		p->RlcSdu.v = serMalloc(_mem, p->RlcSdu.d * sizeof(RLC_SDU_Type));
		for (size_t i2 = 0; i2 < p->RlcSdu.d; i2++) {
			NTOH_32(p->RlcSdu.v[i2].d, &_buffer[*_lidx], _lidx);
			p->RlcSdu.v[i2].v = serMalloc(_mem, p->RlcSdu.v[i2].d * sizeof(OCTET_STRING_ELEMENT));
			for (size_t i3 = 0; i3 < p->RlcSdu.v[i2].d; i3++) {
				NTOH_8(p->RlcSdu.v[i2].v[i3], &_buffer[*_lidx], _lidx);
			}
		}
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecL2DataList_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct L2DataList_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum L2DataList_Type_Sel)_tmp;
	}
	_serDrbDecL2DataList_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecDRB_DataPerSubframe_DL_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct DRB_DataPerSubframe_DL_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->SubframeOffset, &_buffer[*_lidx], _lidx);
	_serDrbDecHarqProcessAssignment_Type_HarqProcess_Optional(_buffer, _size, _lidx, &p->HarqProcess);
	_serDrbDecL2DataList_Type(_buffer, _size, _lidx, _mem, &p->PduSduList);

	return SIDL_STATUS_OK;
}

static int _serDrbDecL2Data_Request_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct L2Data_Request_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->SubframeDataList.d, &_buffer[*_lidx], _lidx);
	p->SubframeDataList.v = serMalloc(_mem, p->SubframeDataList.d * sizeof(struct DRB_DataPerSubframe_DL_Type));
	for (size_t i1 = 0; i1 < p->SubframeDataList.d; i1++) {
		_serDrbDecDRB_DataPerSubframe_DL_Type(_buffer, _size, _lidx, _mem, &p->SubframeDataList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecNull_Type_SuppressPdcchForC_RNTI_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Null_Type_SuppressPdcchForC_RNTI_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_8(p->v, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecDRB_COMMON_REQ(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct DRB_COMMON_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecReqAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serDrbDecL2Data_Request_Type(_buffer, _size, _lidx, _mem, &p->U_Plane);
	_serDrbDecNull_Type_SuppressPdcchForC_RNTI_Optional(_buffer, _size, _lidx, &p->SuppressPdcchForC_RNTI);

	return SIDL_STATUS_OK;
}

int serDrbProcessFromSSDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct DRB_COMMON_REQ** FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*FromSS = serMalloc(_mem, sizeof(struct DRB_COMMON_REQ));
	_serDrbDecDRB_COMMON_REQ(_buffer, _size, _lidx, _mem, *FromSS);

	return SIDL_STATUS_OK;
}

static void _serDrbFreePLMN_Identity(struct PLMN_Identity* p)
{
	if (p->mnc.v) {
		serFree(p->mnc.v);
	}
}

static void _serDrbFreeTMGI_r9_plmn_Id_r9_Value(union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serDrbFreePLMN_Identity(&p->explicitValue_r9);
	}
}

static void _serDrbFreeTMGI_r9_plmn_Id_r9(struct TMGI_r9_plmn_Id_r9* p)
{
	_serDrbFreeTMGI_r9_plmn_Id_r9_Value(&p->v, p->d);
}

static void _serDrbFreeTMGI_r9(struct TMGI_r9* p)
{
	_serDrbFreeTMGI_r9_plmn_Id_r9(&p->plmn_Id_r9);
}

static void _serDrbFreeMBMSSessionInfo_r13(struct MBMSSessionInfo_r13* p)
{
	_serDrbFreeTMGI_r9(&p->tmgi_r13);
}

static void _serDrbFreeSC_MRB_Identity_Type(struct SC_MRB_Identity_Type* p)
{
	_serDrbFreeMBMSSessionInfo_r13(&p->MbmsSessionInfo);
}

static void _serDrbFreeRadioBearerId_Type_Value(union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	if (d == RadioBearerId_Type_ScMrb) {
		_serDrbFreeSC_MRB_Identity_Type(&p->ScMrb);
	}
}

static void _serDrbFreeRadioBearerId_Type(struct RadioBearerId_Type* p)
{
	_serDrbFreeRadioBearerId_Type_Value(&p->v, p->d);
}

static void _serDrbFreeRoutingInfo_Type_Value(union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serDrbFreeRadioBearerId_Type(&p->RadioBearerId);
	}
}

static void _serDrbFreeRoutingInfo_Type(struct RoutingInfo_Type* p)
{
	_serDrbFreeRoutingInfo_Type_Value(&p->v, p->d);
}

static void _serDrbFreeReqAspCommonPart_Type(struct ReqAspCommonPart_Type* p)
{
	_serDrbFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serDrbFreeMAC_PDU_Length_Type(struct MAC_PDU_Length_Type* p)
{
	if (p->Value.v) {
		serFree(p->Value.v);
	}
}

static void _serDrbFreeMAC_PDU_Length_Type_Length_Optional(struct MAC_PDU_Length_Type_Length_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeMAC_PDU_Length_Type(&p->v);
}

static void _serDrbFreeMAC_PDU_SubHeader_Type(struct MAC_PDU_SubHeader_Type* p)
{
	_serDrbFreeMAC_PDU_Length_Type_Length_Optional(&p->Length);
}

static void _serDrbFreeMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(struct MAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreeMAC_CTRL_ExtPowerHeadRoom_Type(struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	if (p->PH_RecordList.v) {
		for (size_t i3 = 0; i3 < p->PH_RecordList.d; i3++) {
		}
		serFree(p->PH_RecordList.v);
	}
}

static void _serDrbFreeMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(struct MAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeMAC_CTRL_ExtPowerHeadRoom_Type(&p->v);
}

static void _serDrbFreeMAC_CTRL_DC_PowerHeadRoom_Type(struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	if (p->DC_PH_RecordList.v) {
		for (size_t i3 = 0; i3 < p->DC_PH_RecordList.d; i3++) {
		}
		serFree(p->DC_PH_RecordList.v);
	}
}

static void _serDrbFreeMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(struct MAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeMAC_CTRL_DC_PowerHeadRoom_Type(&p->v);
}

static void _serDrbFreeMAC_CTRL_ElementList_Type(struct MAC_CTRL_ElementList_Type* p)
{
	_serDrbFreeMAC_CTRL_ContentionResolutionId_Type_ContentionResolutionID_Optional(&p->ContentionResolutionID);
	_serDrbFreeMAC_CTRL_ExtPowerHeadRoom_Type_ExtPowerHeadRoom_Optional(&p->ExtPowerHeadRoom);
	_serDrbFreeMAC_CTRL_DC_PowerHeadRoom_Type_DC_PowerHeadRoom_Optional(&p->DC_PowerHeadRoom);
}

static void _serDrbFreeMAC_CTRL_ElementList_Type_CtrlElementList_Optional(struct MAC_CTRL_ElementList_Type_CtrlElementList_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeMAC_CTRL_ElementList_Type(&p->v);
}

static void _serDrbFreeMAC_SDUList_Type_SduList_Optional(struct MAC_SDUList_Type_SduList_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i3 = 0; i3 < p->v.d; i3++) {
			if (p->v.v[i3].v) {
				serFree(p->v.v[i3].v);
			}
		}
		serFree(p->v.v);
	}
}

static void _serDrbFreeOCTET_STRING_Padding_Optional(struct OCTET_STRING_Padding_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreeMAC_PDU_Type(struct MAC_PDU_Type* p)
{
	if (p->Header.v) {
		for (size_t i3 = 0; i3 < p->Header.d; i3++) {
			_serDrbFreeMAC_PDU_SubHeader_Type(&p->Header.v[i3]);
		}
		serFree(p->Header.v);
	}
	_serDrbFreeMAC_CTRL_ElementList_Type_CtrlElementList_Optional(&p->CtrlElementList);
	_serDrbFreeMAC_SDUList_Type_SduList_Optional(&p->SduList);
	_serDrbFreeOCTET_STRING_Padding_Optional(&p->Padding);
}

static void _serDrbFreeRLC_LI_List_Type_Value(union RLC_LI_List_Type_Value* p, enum RLC_LI_List_Type_Sel d)
{
	if (d == RLC_LI_List_Type_LI11) {
		if (p->LI11.v) {
			for (size_t i3 = 0; i3 < p->LI11.d; i3++) {
			}
			serFree(p->LI11.v);
		}
	}
	if (d == RLC_LI_List_Type_LI15) {
		if (p->LI15.v) {
			for (size_t i3 = 0; i3 < p->LI15.d; i3++) {
			}
			serFree(p->LI15.v);
		}
	}
}

static void _serDrbFreeRLC_LI_List_Type(struct RLC_LI_List_Type* p)
{
	_serDrbFreeRLC_LI_List_Type_Value(&p->v, p->d);
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type(struct RLC_PDU_Header_FlexPart_Type* p)
{
	_serDrbFreeRLC_LI_List_Type(&p->LengthIndicator);
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeRLC_PDU_Header_FlexPart_Type(&p->v);
}

static void _serDrbFreeRLC_UMD_HeaderShortSN_Type(struct RLC_UMD_HeaderShortSN_Type* p)
{
	_serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderShortSN_Type_FlexPart_Optional(&p->FlexPart);
}

static void _serDrbFreeRLC_UMD_PDU_ShortSN_Type(struct RLC_UMD_PDU_ShortSN_Type* p)
{
	_serDrbFreeRLC_UMD_HeaderShortSN_Type(&p->Header);
	if (p->Data.v) {
		for (size_t i3 = 0; i3 < p->Data.d; i3++) {
			if (p->Data.v[i3].v) {
				serFree(p->Data.v[i3].v);
			}
		}
		serFree(p->Data.v);
	}
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(struct RLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeRLC_PDU_Header_FlexPart_Type(&p->v);
}

static void _serDrbFreeRLC_UMD_HeaderLongSN_Type(struct RLC_UMD_HeaderLongSN_Type* p)
{
	_serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_UMD_HeaderLongSN_Type_FlexPart_Optional(&p->FlexPart);
}

static void _serDrbFreeRLC_UMD_PDU_LongSN_Type(struct RLC_UMD_PDU_LongSN_Type* p)
{
	_serDrbFreeRLC_UMD_HeaderLongSN_Type(&p->Header);
	if (p->Data.v) {
		for (size_t i3 = 0; i3 < p->Data.d; i3++) {
			if (p->Data.v[i3].v) {
				serFree(p->Data.v[i3].v);
			}
		}
		serFree(p->Data.v);
	}
}

static void _serDrbFreeRLC_UMD_PDU_Type_Value(union RLC_UMD_PDU_Type_Value* p, enum RLC_UMD_PDU_Type_Sel d)
{
	if (d == RLC_UMD_PDU_Type_ShortSN) {
		_serDrbFreeRLC_UMD_PDU_ShortSN_Type(&p->ShortSN);
	}
	if (d == RLC_UMD_PDU_Type_LongSN) {
		_serDrbFreeRLC_UMD_PDU_LongSN_Type(&p->LongSN);
	}
}

static void _serDrbFreeRLC_UMD_PDU_Type(struct RLC_UMD_PDU_Type* p)
{
	_serDrbFreeRLC_UMD_PDU_Type_Value(&p->v, p->d);
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeRLC_PDU_Header_FlexPart_Type(&p->v);
}

static void _serDrbFreeRLC_AMD_Header_Type(struct RLC_AMD_Header_Type* p)
{
	_serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_AMD_Header_Type_FlexPart_Optional(&p->FlexPart);
}

static void _serDrbFreeRLC_AMD_PDU_Type(struct RLC_AMD_PDU_Type* p)
{
	_serDrbFreeRLC_AMD_Header_Type(&p->Header);
	if (p->Data.v) {
		for (size_t i3 = 0; i3 < p->Data.d; i3++) {
			if (p->Data.v[i3].v) {
				serFree(p->Data.v[i3].v);
			}
		}
		serFree(p->Data.v);
	}
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(struct RLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeRLC_PDU_Header_FlexPart_Type(&p->v);
}

static void _serDrbFreeRLC_AMD_HeaderExt_Type(struct RLC_AMD_HeaderExt_Type* p)
{
	_serDrbFreeRLC_PDU_Header_FlexPart_Type_RLC_AMD_HeaderExt_Type_FlexPart_Optional(&p->FlexPart);
}

static void _serDrbFreeRLC_AMD_PDU_Ext_Type(struct RLC_AMD_PDU_Ext_Type* p)
{
	_serDrbFreeRLC_AMD_HeaderExt_Type(&p->HeaderExt);
	if (p->Data.v) {
		for (size_t i3 = 0; i3 < p->Data.d; i3++) {
			if (p->Data.v[i3].v) {
				serFree(p->Data.v[i3].v);
			}
		}
		serFree(p->Data.v);
	}
}

static void _serDrbFreeRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(struct RLC_PDU_Header_FlexPart_Type_FlexPart_Optional* p)
{
	if (!p->d) return;
	_serDrbFreeRLC_PDU_Header_FlexPart_Type(&p->v);
}

static void _serDrbFreeRLC_AMD_HeaderSegExt_Type(struct RLC_AMD_HeaderSegExt_Type* p)
{
	_serDrbFreeRLC_PDU_Header_FlexPart_Type_FlexPart_Optional(&p->FlexPart);
}

static void _serDrbFreeRLC_AMD_PDU_SegExt_Type(struct RLC_AMD_PDU_SegExt_Type* p)
{
	_serDrbFreeRLC_AMD_HeaderSegExt_Type(&p->HeaderSegExt);
	if (p->Data.v) {
		for (size_t i3 = 0; i3 < p->Data.d; i3++) {
			if (p->Data.v[i3].v) {
				serFree(p->Data.v[i3].v);
			}
		}
		serFree(p->Data.v);
	}
}

static void _serDrbFreeRLC_Status_NACK_List_Type_NackList_Optional(struct RLC_Status_NACK_List_Type_NackList_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i3 = 0; i3 < p->v.d; i3++) {
		}
		serFree(p->v.v);
	}
}

static void _serDrbFreeRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreeRLC_AM_StatusPDU_Type(struct RLC_AM_StatusPDU_Type* p)
{
	_serDrbFreeRLC_Status_NACK_List_Type_NackList_Optional(&p->NackList);
	_serDrbFreeRLC_Status_Padding_Type_RLC_AM_StatusPDU_Type_Padding_Optional(&p->Padding);
}

static void _serDrbFreeRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(struct RLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i3 = 0; i3 < p->v.d; i3++) {
		}
		serFree(p->v.v);
	}
}

static void _serDrbFreeRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(struct RLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreeRLC_AM_StatusPDU_Ext_Type(struct RLC_AM_StatusPDU_Ext_Type* p)
{
	_serDrbFreeRLC_Status_NACK_Ext_List_Type_Nack_Ext_List_Optional(&p->Nack_Ext_List);
	_serDrbFreeRLC_Status_Padding_Type_RLC_AM_StatusPDU_Ext_Type_Padding_Optional(&p->Padding);
}

static void _serDrbFreeRLC_PDU_Type_Value(union RLC_PDU_Type_Value* p, enum RLC_PDU_Type_Sel d)
{
	if (d == RLC_PDU_Type_TMD) {
		if (p->TMD.v) {
			serFree(p->TMD.v);
		}
	}
	if (d == RLC_PDU_Type_UMD) {
		_serDrbFreeRLC_UMD_PDU_Type(&p->UMD);
	}
	if (d == RLC_PDU_Type_AMD) {
		_serDrbFreeRLC_AMD_PDU_Type(&p->AMD);
	}
	if (d == RLC_PDU_Type_AMD_Ext) {
		_serDrbFreeRLC_AMD_PDU_Ext_Type(&p->AMD_Ext);
	}
	if (d == RLC_PDU_Type_AMD_SegExt) {
		_serDrbFreeRLC_AMD_PDU_SegExt_Type(&p->AMD_SegExt);
	}
	if (d == RLC_PDU_Type_Status) {
		_serDrbFreeRLC_AM_StatusPDU_Type(&p->Status);
	}
	if (d == RLC_PDU_Type_Status_Ext) {
		_serDrbFreeRLC_AM_StatusPDU_Ext_Type(&p->Status_Ext);
	}
}

static void _serDrbFreeRLC_PDU_Type(struct RLC_PDU_Type* p)
{
	_serDrbFreeRLC_PDU_Type_Value(&p->v, p->d);
}

static void _serDrbFreePDCP_DataPdu_LongSN_Type(struct PDCP_DataPdu_LongSN_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_ShortSN_Type(struct PDCP_DataPdu_ShortSN_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_ExtSN_Type(struct PDCP_DataPdu_ExtSN_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_18bitSN_Type(struct PDCP_DataPdu_18bitSN_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_Ctrl_ROHC_FB_PDU_Type(struct PDCP_Ctrl_ROHC_FB_PDU_Type* p)
{
	if (p->ROHC_FB.v) {
		serFree(p->ROHC_FB.v);
	}
}

static void _serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(struct OCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreePDCP_Ctrl_StatusReport_Type(struct PDCP_Ctrl_StatusReport_Type* p)
{
	_serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReport_Type_Bitmap_Optional(&p->Bitmap);
}

static void _serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(struct OCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreePDCP_Ctrl_StatusReportExt_Type(struct PDCP_Ctrl_StatusReportExt_Type* p)
{
	_serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReportExt_Type_Bitmap_Optional(&p->Bitmap);
}

static void _serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(struct OCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serDrbFreePDCP_Ctrl_StatusReport_18bitSN_Type(struct PDCP_Ctrl_StatusReport_18bitSN_Type* p)
{
	_serDrbFreeOCTET_STRING_PDCP_Ctrl_StatusReport_18bitSN_Type_Bitmap_Optional(&p->Bitmap);
}

static void _serDrbFreePDCP_DataPdu_SLRB_Type(struct PDCP_DataPdu_SLRB_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_SLRB_1to1_Type(struct PDCP_DataPdu_SLRB_1to1_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_LongSN_UDC_Type(struct PDCP_DataPdu_LongSN_UDC_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_ExtSN_UDC_Type(struct PDCP_DataPdu_ExtSN_UDC_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_DataPdu_18bitSN_UDC_Type(struct PDCP_DataPdu_18bitSN_UDC_Type* p)
{
	if (p->SDU.v) {
		serFree(p->SDU.v);
	}
}

static void _serDrbFreePDCP_PDU_Type_Value(union PDCP_PDU_Type_Value* p, enum PDCP_PDU_Type_Sel d)
{
	if (d == PDCP_PDU_Type_DataLongSN) {
		_serDrbFreePDCP_DataPdu_LongSN_Type(&p->DataLongSN);
	}
	if (d == PDCP_PDU_Type_DataShortSN) {
		_serDrbFreePDCP_DataPdu_ShortSN_Type(&p->DataShortSN);
	}
	if (d == PDCP_PDU_Type_DataExtSN) {
		_serDrbFreePDCP_DataPdu_ExtSN_Type(&p->DataExtSN);
	}
	if (d == PDCP_PDU_Type_Data_18bitSN) {
		_serDrbFreePDCP_DataPdu_18bitSN_Type(&p->Data_18bitSN);
	}
	if (d == PDCP_PDU_Type_RohcFeedback) {
		_serDrbFreePDCP_Ctrl_ROHC_FB_PDU_Type(&p->RohcFeedback);
	}
	if (d == PDCP_PDU_Type_StatusReport) {
		_serDrbFreePDCP_Ctrl_StatusReport_Type(&p->StatusReport);
	}
	if (d == PDCP_PDU_Type_StatusReportExt) {
		_serDrbFreePDCP_Ctrl_StatusReportExt_Type(&p->StatusReportExt);
	}
	if (d == PDCP_PDU_Type_StatusReport_18bitSN) {
		_serDrbFreePDCP_Ctrl_StatusReport_18bitSN_Type(&p->StatusReport_18bitSN);
	}
	if (d == PDCP_PDU_Type_DataSLRB) {
		_serDrbFreePDCP_DataPdu_SLRB_Type(&p->DataSLRB);
	}
	if (d == PDCP_PDU_Type_DataSLRB_1to1) {
		_serDrbFreePDCP_DataPdu_SLRB_1to1_Type(&p->DataSLRB_1to1);
	}
	if (d == PDCP_PDU_Type_DataLongSN_UDC) {
		_serDrbFreePDCP_DataPdu_LongSN_UDC_Type(&p->DataLongSN_UDC);
	}
	if (d == PDCP_PDU_Type_DataExtSN_UDC) {
		_serDrbFreePDCP_DataPdu_ExtSN_UDC_Type(&p->DataExtSN_UDC);
	}
	if (d == PDCP_PDU_Type_Data18bitSN_UDC) {
		_serDrbFreePDCP_DataPdu_18bitSN_UDC_Type(&p->Data18bitSN_UDC);
	}
}

static void _serDrbFreePDCP_PDU_Type(struct PDCP_PDU_Type* p)
{
	_serDrbFreePDCP_PDU_Type_Value(&p->v, p->d);
}

static void _serDrbFreeL2DataList_Type_Value(union L2DataList_Type_Value* p, enum L2DataList_Type_Sel d)
{
	if (d == L2DataList_Type_MacPdu) {
		if (p->MacPdu.v) {
			for (size_t i2 = 0; i2 < p->MacPdu.d; i2++) {
				_serDrbFreeMAC_PDU_Type(&p->MacPdu.v[i2]);
			}
			serFree(p->MacPdu.v);
		}
	}
	if (d == L2DataList_Type_RlcPdu) {
		if (p->RlcPdu.v) {
			for (size_t i2 = 0; i2 < p->RlcPdu.d; i2++) {
				_serDrbFreeRLC_PDU_Type(&p->RlcPdu.v[i2]);
			}
			serFree(p->RlcPdu.v);
		}
	}
	if (d == L2DataList_Type_PdcpPdu) {
		if (p->PdcpPdu.v) {
			for (size_t i2 = 0; i2 < p->PdcpPdu.d; i2++) {
				_serDrbFreePDCP_PDU_Type(&p->PdcpPdu.v[i2]);
			}
			serFree(p->PdcpPdu.v);
		}
	}
	if (d == L2DataList_Type_PdcpSdu) {
		if (p->PdcpSdu.v) {
			for (size_t i2 = 0; i2 < p->PdcpSdu.d; i2++) {
				if (p->PdcpSdu.v[i2].v) {
					serFree(p->PdcpSdu.v[i2].v);
				}
			}
			serFree(p->PdcpSdu.v);
		}
	}
	if (d == L2DataList_Type_NrPdcpSdu) {
		if (p->NrPdcpSdu.v) {
			for (size_t i2 = 0; i2 < p->NrPdcpSdu.d; i2++) {
				if (p->NrPdcpSdu.v[i2].v) {
					serFree(p->NrPdcpSdu.v[i2].v);
				}
			}
			serFree(p->NrPdcpSdu.v);
		}
	}
	if (d == L2DataList_Type_RlcSdu) {
		if (p->RlcSdu.v) {
			for (size_t i2 = 0; i2 < p->RlcSdu.d; i2++) {
				if (p->RlcSdu.v[i2].v) {
					serFree(p->RlcSdu.v[i2].v);
				}
			}
			serFree(p->RlcSdu.v);
		}
	}
}

static void _serDrbFreeL2DataList_Type(struct L2DataList_Type* p)
{
	_serDrbFreeL2DataList_Type_Value(&p->v, p->d);
}

static void _serDrbFreeDRB_DataPerSubframe_DL_Type(struct DRB_DataPerSubframe_DL_Type* p)
{
	_serDrbFreeL2DataList_Type(&p->PduSduList);
}

static void _serDrbFreeL2Data_Request_Type(struct L2Data_Request_Type* p)
{
	if (p->SubframeDataList.v) {
		for (size_t i1 = 0; i1 < p->SubframeDataList.d; i1++) {
			_serDrbFreeDRB_DataPerSubframe_DL_Type(&p->SubframeDataList.v[i1]);
		}
		serFree(p->SubframeDataList.v);
	}
}

static void _serDrbFreeDRB_COMMON_REQ(struct DRB_COMMON_REQ* p)
{
	_serDrbFreeReqAspCommonPart_Type(&p->Common);
	_serDrbFreeL2Data_Request_Type(&p->U_Plane);
}

void serDrbProcessFromSSFreeSrv(struct DRB_COMMON_REQ* FromSS)
{
	if (FromSS) {
		_serDrbFreeDRB_COMMON_REQ(FromSS);
		serFree(FromSS);
	}
}

static int _serDrbEncIntegrityErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Nas, _lidx);
	HTON_8(&_buffer[*_lidx], p->Pdcp, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	HTON_32(&_buffer[*_lidx], p->System, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncIndicationStatus_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		HTON_8(&_buffer[*_lidx], p->Ok, _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serDrbEncErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbEncIndicationStatus_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbEncIndAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serDrbEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serDrbEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serDrbEncIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serDrbEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serDrbEncDRB_DataPerSubframe_UL_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_DataPerSubframe_UL_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncL2DataList_Type(_buffer, _size, _lidx, &p->PduSduList);
	HTON_32(&_buffer[*_lidx], p->NoOfTTIs, _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbEncL2Data_Indication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct L2Data_Indication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncDRB_DataPerSubframe_UL_Type(_buffer, _size, _lidx, &p->SubframeData);

	return SIDL_STATUS_OK;
}

static int _serDrbEncDRB_COMMON_IND(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_COMMON_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncIndAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serDrbEncL2Data_Indication_Type(_buffer, _size, _lidx, &p->U_Plane);

	return SIDL_STATUS_OK;
}

int serDrbProcessToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct DRB_COMMON_IND* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbEncDRB_COMMON_IND(_buffer, _size, _lidx, ToSS);

	return SIDL_STATUS_OK;
}

static int _serDrbDecIntegrityErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Nas, &_buffer[*_lidx], _lidx);
	NTOH_8(p->Pdcp, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	NTOH_32(p->System, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecIndicationStatus_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		NTOH_8(p->Ok, &_buffer[*_lidx], _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serDrbDecErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serDrbDecIndicationStatus_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum IndicationStatus_Type_Sel)_tmp;
	}
	_serDrbDecIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serDrbDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serDrbDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serDrbDecIndAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serDrbDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serDrbDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serDrbDecIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serDrbDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serDrbDecDRB_DataPerSubframe_UL_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct DRB_DataPerSubframe_UL_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecL2DataList_Type(_buffer, _size, _lidx, _mem, &p->PduSduList);
	NTOH_32(p->NoOfTTIs, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serDrbDecL2Data_Indication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct L2Data_Indication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecDRB_DataPerSubframe_UL_Type(_buffer, _size, _lidx, _mem, &p->SubframeData);

	return SIDL_STATUS_OK;
}

static int _serDrbDecDRB_COMMON_IND(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct DRB_COMMON_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serDrbDecIndAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serDrbDecL2Data_Indication_Type(_buffer, _size, _lidx, _mem, &p->U_Plane);

	return SIDL_STATUS_OK;
}

int serDrbProcessToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct DRB_COMMON_IND** ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*ToSS = serMalloc(_mem, sizeof(struct DRB_COMMON_IND));
	_serDrbDecDRB_COMMON_IND(_buffer, _size, _lidx, _mem, *ToSS);

	return SIDL_STATUS_OK;
}

static void _serDrbFreeIndAspCommonPart_Type(struct IndAspCommonPart_Type* p)
{
	_serDrbFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serDrbFreeDRB_DataPerSubframe_UL_Type(struct DRB_DataPerSubframe_UL_Type* p)
{
	_serDrbFreeL2DataList_Type(&p->PduSduList);
}

static void _serDrbFreeL2Data_Indication_Type(struct L2Data_Indication_Type* p)
{
	_serDrbFreeDRB_DataPerSubframe_UL_Type(&p->SubframeData);
}

static void _serDrbFreeDRB_COMMON_IND(struct DRB_COMMON_IND* p)
{
	_serDrbFreeIndAspCommonPart_Type(&p->Common);
	_serDrbFreeL2Data_Indication_Type(&p->U_Plane);
}

void serDrbProcessToSSFreeClt(struct DRB_COMMON_IND* ToSS)
{
	if (ToSS) {
		_serDrbFreeDRB_COMMON_IND(ToSS);
		serFree(ToSS);
	}
}
