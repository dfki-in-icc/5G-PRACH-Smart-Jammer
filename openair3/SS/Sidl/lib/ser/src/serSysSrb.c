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
#include "serSysSrb.h"
#include "serMem.h"
#include "serUtils.h"

static int _serSysSrbEncPmchLogicalChannel_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Pmch_InfoIndex, _lidx);
	HTON_8(&_buffer[*_lidx], p->LogicalChannelIdentity, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncMRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Mbsfn_AreaId, _lidx);
	_serSysSrbEncPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncMCC_PLMN_Identity_mcc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncPLMN_Identity(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	HTON_32(&_buffer[*_lidx], p->mnc.d, _lidx);
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->mnc.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncTMGI_r9_plmn_Id_r9_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		HTON_8(&_buffer[*_lidx], p->plmn_Index_r9, _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysSrbEncPLMN_Identity(_buffer, _size, _lidx, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncTMGI_r9_plmn_Id_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncTMGI_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->serviceId_r9[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncMBMSSessionInfo_r13(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncTMGI_r9(_buffer, _size, _lidx, &p->tmgi_r13);
	_serSysSrbEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSC_MRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncMBMSSessionInfo_r13(_buffer, _size, _lidx, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRadioBearerId_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		HTON_8(&_buffer[*_lidx], p->Srb, _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		HTON_8(&_buffer[*_lidx], p->Drb, _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serSysSrbEncMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysSrbEncSC_MRB_Identity_Type(_buffer, _size, _lidx, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRadioBearerId_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRadioBearerId_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncQosFlow_Identification_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->PDU_SessionId, _lidx);
	HTON_32(&_buffer[*_lidx], p->QFI, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRoutingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysSrbEncRadioBearerId_Type(_buffer, _size, _lidx, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serSysSrbEncQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRoutingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRoutingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSystemFrameNumberInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serSysSrbEncSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSubFrameInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serSysSrbEncSubFrameInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncHyperSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSlotOffset_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serSysSrbEncSlotOffset_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSlotTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serSysSrbEncSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		HTON_8(&_buffer[*_lidx], p->FirstSlot, _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSlotTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncSubFrameTiming_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serSysSrbEncSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serSysSrbEncHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serSysSrbEncSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serSysSrbEncSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		HTON_8(&_buffer[*_lidx], p->Now, _lidx);
	}
	if (d == TimingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncIntegrityErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Nas, _lidx);
	HTON_8(&_buffer[*_lidx], p->Pdcp, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	HTON_32(&_buffer[*_lidx], p->System, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncIndicationStatus_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		HTON_8(&_buffer[*_lidx], p->Ok, _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serSysSrbEncErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncIndicationStatus_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRlcBearerRouting_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serSysSrbEncRlcBearerRouting_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysSrbEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncIndAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serSysSrbEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysSrbEncIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serSysSrbEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRRC_MSG_Indication_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RRC_MSG_Indication_Type_Value* p, enum RRC_MSG_Indication_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RRC_MSG_Indication_Type_Ccch) {
		HTON_32(&_buffer[*_lidx], p->Ccch.d, _lidx);
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			HTON_8(&_buffer[*_lidx], p->Ccch.v[i1], _lidx);
		}
	}
	if (d == RRC_MSG_Indication_Type_Dcch) {
		HTON_32(&_buffer[*_lidx], p->Dcch.d, _lidx);
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			HTON_8(&_buffer[*_lidx], p->Dcch.v[i1], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRRC_MSG_Indication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RRC_MSG_Indication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRRC_MSG_Indication_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncEUTRA_RRC_PDU_IND(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_RRC_PDU_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncIndAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serSysSrbEncRRC_MSG_Indication_Type(_buffer, _size, _lidx, &p->RrcPdu);

	return SIDL_STATUS_OK;
}

int serSysSrbProcessToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_RRC_PDU_IND* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncEUTRA_RRC_PDU_IND(_buffer, _size, _lidx, ToSS);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecPmchLogicalChannel_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Pmch_InfoIndex, &_buffer[*_lidx], _lidx);
	NTOH_8(p->LogicalChannelIdentity, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecMRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Mbsfn_AreaId, &_buffer[*_lidx], _lidx);
	_serSysSrbDecPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecMCC_PLMN_Identity_mcc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecPLMN_Identity(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	NTOH_32(p->mnc.d, &_buffer[*_lidx], _lidx);
	p->mnc.v = serMalloc(_mem, p->mnc.d * sizeof(MCC_MNC_Digit));
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		NTOH_8(p->mnc.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecTMGI_r9_plmn_Id_r9_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		NTOH_8(p->plmn_Index_r9, &_buffer[*_lidx], _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysSrbDecPLMN_Identity(_buffer, _size, _lidx, _mem, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecTMGI_r9_plmn_Id_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TMGI_r9_plmn_Id_r9_Sel)_tmp;
	}
	_serSysSrbDecTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecTMGI_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, _mem, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->serviceId_r9[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecMBMSSessionInfo_r13(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecTMGI_r9(_buffer, _size, _lidx, _mem, &p->tmgi_r13);
	_serSysSrbDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSC_MRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecMBMSSessionInfo_r13(_buffer, _size, _lidx, _mem, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRadioBearerId_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		NTOH_8(p->Srb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		NTOH_8(p->Drb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serSysSrbDecMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysSrbDecSC_MRB_Identity_Type(_buffer, _size, _lidx, _mem, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRadioBearerId_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RadioBearerId_Type_Sel)_tmp;
	}
	_serSysSrbDecRadioBearerId_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecQosFlow_Identification_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->PDU_SessionId, &_buffer[*_lidx], _lidx);
	NTOH_32(p->QFI, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRoutingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysSrbDecRadioBearerId_Type(_buffer, _size, _lidx, _mem, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serSysSrbDecQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRoutingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RoutingInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecRoutingInfo_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSystemFrameNumberInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serSysSrbDecSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSubFrameInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serSysSrbDecSubFrameInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SubFrameInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecHyperSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSlotOffset_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serSysSrbDecSlotOffset_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotOffset_Type_Sel)_tmp;
	}
	_serSysSrbDecSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSlotTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serSysSrbDecSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		NTOH_8(p->FirstSlot, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSlotTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotTimingInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecSubFrameTiming_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serSysSrbDecSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serSysSrbDecHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serSysSrbDecSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serSysSrbDecSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		NTOH_8(p->Now, &_buffer[*_lidx], _lidx);
	}
	if (d == TimingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TimingInfo_Type_Sel)_tmp;
	}
	_serSysSrbDecTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecIntegrityErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Nas, &_buffer[*_lidx], _lidx);
	NTOH_8(p->Pdcp, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	NTOH_32(p->System, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecIndicationStatus_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		NTOH_8(p->Ok, &_buffer[*_lidx], _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serSysSrbDecErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecIndicationStatus_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum IndicationStatus_Type_Sel)_tmp;
	}
	_serSysSrbDecIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRlcBearerRouting_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serSysSrbDecRlcBearerRouting_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RlcBearerRouting_Type_Sel)_tmp;
	}
	_serSysSrbDecRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysSrbDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecIndAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serSysSrbDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serSysSrbDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysSrbDecIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serSysSrbDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRRC_MSG_Indication_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RRC_MSG_Indication_Type_Value* p, enum RRC_MSG_Indication_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RRC_MSG_Indication_Type_Ccch) {
		NTOH_32(p->Ccch.d, &_buffer[*_lidx], _lidx);
		p->Ccch.v = serMalloc(_mem, p->Ccch.d * sizeof(uint8_t));
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			NTOH_8(p->Ccch.v[i1], &_buffer[*_lidx], _lidx);
		}
	}
	if (d == RRC_MSG_Indication_Type_Dcch) {
		NTOH_32(p->Dcch.d, &_buffer[*_lidx], _lidx);
		p->Dcch.v = serMalloc(_mem, p->Dcch.d * sizeof(uint8_t));
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			NTOH_8(p->Dcch.v[i1], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRRC_MSG_Indication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RRC_MSG_Indication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RRC_MSG_Indication_Type_Sel)_tmp;
	}
	_serSysSrbDecRRC_MSG_Indication_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecEUTRA_RRC_PDU_IND(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct EUTRA_RRC_PDU_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecIndAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serSysSrbDecRRC_MSG_Indication_Type(_buffer, _size, _lidx, _mem, &p->RrcPdu);

	return SIDL_STATUS_OK;
}

int serSysSrbProcessToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_RRC_PDU_IND** ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*ToSS = serMalloc(_mem, sizeof(struct EUTRA_RRC_PDU_IND));
	_serSysSrbDecEUTRA_RRC_PDU_IND(_buffer, _size, _lidx, _mem, *ToSS);

	return SIDL_STATUS_OK;
}

static void _serSysSrbFreePLMN_Identity(struct PLMN_Identity* p)
{
	if (p->mnc.v) {
		serFree(p->mnc.v);
	}
}

static void _serSysSrbFreeTMGI_r9_plmn_Id_r9_Value(union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysSrbFreePLMN_Identity(&p->explicitValue_r9);
	}
}

static void _serSysSrbFreeTMGI_r9_plmn_Id_r9(struct TMGI_r9_plmn_Id_r9* p)
{
	_serSysSrbFreeTMGI_r9_plmn_Id_r9_Value(&p->v, p->d);
}

static void _serSysSrbFreeTMGI_r9(struct TMGI_r9* p)
{
	_serSysSrbFreeTMGI_r9_plmn_Id_r9(&p->plmn_Id_r9);
}

static void _serSysSrbFreeMBMSSessionInfo_r13(struct MBMSSessionInfo_r13* p)
{
	_serSysSrbFreeTMGI_r9(&p->tmgi_r13);
}

static void _serSysSrbFreeSC_MRB_Identity_Type(struct SC_MRB_Identity_Type* p)
{
	_serSysSrbFreeMBMSSessionInfo_r13(&p->MbmsSessionInfo);
}

static void _serSysSrbFreeRadioBearerId_Type_Value(union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysSrbFreeSC_MRB_Identity_Type(&p->ScMrb);
	}
}

static void _serSysSrbFreeRadioBearerId_Type(struct RadioBearerId_Type* p)
{
	_serSysSrbFreeRadioBearerId_Type_Value(&p->v, p->d);
}

static void _serSysSrbFreeRoutingInfo_Type_Value(union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysSrbFreeRadioBearerId_Type(&p->RadioBearerId);
	}
}

static void _serSysSrbFreeRoutingInfo_Type(struct RoutingInfo_Type* p)
{
	_serSysSrbFreeRoutingInfo_Type_Value(&p->v, p->d);
}

static void _serSysSrbFreeIndAspCommonPart_Type(struct IndAspCommonPart_Type* p)
{
	_serSysSrbFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serSysSrbFreeRRC_MSG_Indication_Type_Value(union RRC_MSG_Indication_Type_Value* p, enum RRC_MSG_Indication_Type_Sel d)
{
	if (d == RRC_MSG_Indication_Type_Ccch) {
		if (p->Ccch.v) {
			serFree(p->Ccch.v);
		}
	}
	if (d == RRC_MSG_Indication_Type_Dcch) {
		if (p->Dcch.v) {
			serFree(p->Dcch.v);
		}
	}
}

static void _serSysSrbFreeRRC_MSG_Indication_Type(struct RRC_MSG_Indication_Type* p)
{
	_serSysSrbFreeRRC_MSG_Indication_Type_Value(&p->v, p->d);
}

static void _serSysSrbFreeEUTRA_RRC_PDU_IND(struct EUTRA_RRC_PDU_IND* p)
{
	_serSysSrbFreeIndAspCommonPart_Type(&p->Common);
	_serSysSrbFreeRRC_MSG_Indication_Type(&p->RrcPdu);
}

void serSysSrbProcessToSSFreeClt(struct EUTRA_RRC_PDU_IND* ToSS)
{
	if (ToSS) {
		_serSysSrbFreeEUTRA_RRC_PDU_IND(ToSS);
		serFree(ToSS);
	}
}

static int _serSysSrbEncReqAspControlInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->CnfFlag, _lidx);
	HTON_8(&_buffer[*_lidx], p->FollowOnFlag, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysSrbEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncReqAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serSysSrbEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysSrbEncReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serSysSrbEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRRC_MSG_Request_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RRC_MSG_Request_Type_Value* p, enum RRC_MSG_Request_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RRC_MSG_Request_Type_Ccch) {
		HTON_32(&_buffer[*_lidx], p->Ccch.d, _lidx);
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			HTON_8(&_buffer[*_lidx], p->Ccch.v[i1], _lidx);
		}
	}
	if (d == RRC_MSG_Request_Type_Dcch) {
		HTON_32(&_buffer[*_lidx], p->Dcch.d, _lidx);
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			HTON_8(&_buffer[*_lidx], p->Dcch.v[i1], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncRRC_MSG_Request_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RRC_MSG_Request_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysSrbEncRRC_MSG_Request_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbEncEUTRA_RRC_PDU_REQ(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_RRC_PDU_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncReqAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serSysSrbEncRRC_MSG_Request_Type(_buffer, _size, _lidx, &p->RrcPdu);

	return SIDL_STATUS_OK;
}

int serSysSrbProcessFromSSEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_RRC_PDU_REQ* FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbEncEUTRA_RRC_PDU_REQ(_buffer, _size, _lidx, FromSS);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecReqAspControlInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->CnfFlag, &_buffer[*_lidx], _lidx);
	NTOH_8(p->FollowOnFlag, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysSrbDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecReqAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serSysSrbDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serSysSrbDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysSrbDecReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serSysSrbDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRRC_MSG_Request_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RRC_MSG_Request_Type_Value* p, enum RRC_MSG_Request_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RRC_MSG_Request_Type_Ccch) {
		NTOH_32(p->Ccch.d, &_buffer[*_lidx], _lidx);
		p->Ccch.v = serMalloc(_mem, p->Ccch.d * sizeof(uint8_t));
		for (size_t i1 = 0; i1 < p->Ccch.d; i1++) {
			NTOH_8(p->Ccch.v[i1], &_buffer[*_lidx], _lidx);
		}
	}
	if (d == RRC_MSG_Request_Type_Dcch) {
		NTOH_32(p->Dcch.d, &_buffer[*_lidx], _lidx);
		p->Dcch.v = serMalloc(_mem, p->Dcch.d * sizeof(uint8_t));
		for (size_t i1 = 0; i1 < p->Dcch.d; i1++) {
			NTOH_8(p->Dcch.v[i1], &_buffer[*_lidx], _lidx);
		}
	}

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecRRC_MSG_Request_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RRC_MSG_Request_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RRC_MSG_Request_Type_Sel)_tmp;
	}
	_serSysSrbDecRRC_MSG_Request_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysSrbDecEUTRA_RRC_PDU_REQ(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct EUTRA_RRC_PDU_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysSrbDecReqAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serSysSrbDecRRC_MSG_Request_Type(_buffer, _size, _lidx, _mem, &p->RrcPdu);

	return SIDL_STATUS_OK;
}

int serSysSrbProcessFromSSDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_RRC_PDU_REQ** FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*FromSS = serMalloc(_mem, sizeof(struct EUTRA_RRC_PDU_REQ));
	_serSysSrbDecEUTRA_RRC_PDU_REQ(_buffer, _size, _lidx, _mem, *FromSS);

	return SIDL_STATUS_OK;
}

static void _serSysSrbFreeReqAspCommonPart_Type(struct ReqAspCommonPart_Type* p)
{
	_serSysSrbFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serSysSrbFreeRRC_MSG_Request_Type_Value(union RRC_MSG_Request_Type_Value* p, enum RRC_MSG_Request_Type_Sel d)
{
	if (d == RRC_MSG_Request_Type_Ccch) {
		if (p->Ccch.v) {
			serFree(p->Ccch.v);
		}
	}
	if (d == RRC_MSG_Request_Type_Dcch) {
		if (p->Dcch.v) {
			serFree(p->Dcch.v);
		}
	}
}

static void _serSysSrbFreeRRC_MSG_Request_Type(struct RRC_MSG_Request_Type* p)
{
	_serSysSrbFreeRRC_MSG_Request_Type_Value(&p->v, p->d);
}

static void _serSysSrbFreeEUTRA_RRC_PDU_REQ(struct EUTRA_RRC_PDU_REQ* p)
{
	_serSysSrbFreeReqAspCommonPart_Type(&p->Common);
	_serSysSrbFreeRRC_MSG_Request_Type(&p->RrcPdu);
}

void serSysSrbProcessFromSSFreeSrv(struct EUTRA_RRC_PDU_REQ* FromSS)
{
	if (FromSS) {
		_serSysSrbFreeEUTRA_RRC_PDU_REQ(FromSS);
		serFree(FromSS);
	}
}
