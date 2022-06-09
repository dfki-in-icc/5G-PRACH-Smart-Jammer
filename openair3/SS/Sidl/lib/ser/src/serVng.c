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
#include "serVng.h"
#include "serMem.h"
#include "serUtils.h"

static int _serVngEncPmchLogicalChannel_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Pmch_InfoIndex, _lidx);
	HTON_8(&_buffer[*_lidx], p->LogicalChannelIdentity, _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngEncMRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Mbsfn_AreaId, _lidx);
	_serVngEncPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serVngEncMCC_PLMN_Identity_mcc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncPLMN_Identity(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	HTON_32(&_buffer[*_lidx], p->mnc.d, _lidx);
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->mnc.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncTMGI_r9_plmn_Id_r9_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		HTON_8(&_buffer[*_lidx], p->plmn_Index_r9, _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serVngEncPLMN_Identity(_buffer, _size, _lidx, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncTMGI_r9_plmn_Id_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncTMGI_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->serviceId_r9[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncMBMSSessionInfo_r13(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncTMGI_r9(_buffer, _size, _lidx, &p->tmgi_r13);
	_serVngEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serVngEncSC_MRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncMBMSSessionInfo_r13(_buffer, _size, _lidx, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serVngEncRadioBearerId_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		HTON_8(&_buffer[*_lidx], p->Srb, _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		HTON_8(&_buffer[*_lidx], p->Drb, _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serVngEncMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serVngEncSC_MRB_Identity_Type(_buffer, _size, _lidx, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncRadioBearerId_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncRadioBearerId_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncQosFlow_Identification_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->PDU_SessionId, _lidx);
	HTON_32(&_buffer[*_lidx], p->QFI, _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngEncRoutingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serVngEncRadioBearerId_Type(_buffer, _size, _lidx, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serVngEncQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncRoutingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncRoutingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncSystemFrameNumberInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serVngEncSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncSubFrameInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serVngEncSubFrameInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncHyperSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncSlotOffset_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serVngEncSlotOffset_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncSlotTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serVngEncSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		HTON_8(&_buffer[*_lidx], p->FirstSlot, _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncSlotTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncSubFrameTiming_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serVngEncSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serVngEncHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serVngEncSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serVngEncTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serVngEncSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		HTON_8(&_buffer[*_lidx], p->Now, _lidx);
	}
	if (d == TimingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncReqAspControlInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->CnfFlag, _lidx);
	HTON_8(&_buffer[*_lidx], p->FollowOnFlag, _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngEncRlcBearerRouting_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serVngEncRlcBearerRouting_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serVngEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serVngEncReqAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serVngEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serVngEncReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serVngEncRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serVngEncEUTRA_VngConfigInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VngConfigInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->Bandwidth;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->NocLevel, _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngEncEUTRA_VngConfigRequest_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union EUTRA_VngConfigRequest_Type_Value* p, enum EUTRA_VngConfigRequest_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == EUTRA_VngConfigRequest_Type_Configure) {
		_serVngEncEUTRA_VngConfigInfo_Type(_buffer, _size, _lidx, &p->Configure);
	}
	if (d == EUTRA_VngConfigRequest_Type_Activate) {
		HTON_8(&_buffer[*_lidx], p->Activate, _lidx);
	}
	if (d == EUTRA_VngConfigRequest_Type_Deactivate) {
		HTON_8(&_buffer[*_lidx], p->Deactivate, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncEUTRA_VngConfigRequest_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VngConfigRequest_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncEUTRA_VngConfigRequest_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncEUTRA_VNG_CTRL_REQ(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncReqAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serVngEncEUTRA_VngConfigRequest_Type(_buffer, _size, _lidx, &p->Request);

	return SIDL_STATUS_OK;
}

int serVngProcessEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_REQ* FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncEUTRA_VNG_CTRL_REQ(_buffer, _size, _lidx, FromSS);

	return SIDL_STATUS_OK;
}

static int _serVngDecPmchLogicalChannel_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Pmch_InfoIndex, &_buffer[*_lidx], _lidx);
	NTOH_8(p->LogicalChannelIdentity, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngDecMRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Mbsfn_AreaId, &_buffer[*_lidx], _lidx);
	_serVngDecPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serVngDecMCC_PLMN_Identity_mcc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecPLMN_Identity(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	NTOH_32(p->mnc.d, &_buffer[*_lidx], _lidx);
	p->mnc.v = serMalloc(_mem, p->mnc.d * sizeof(MCC_MNC_Digit));
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		NTOH_8(p->mnc.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecTMGI_r9_plmn_Id_r9_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		NTOH_8(p->plmn_Index_r9, &_buffer[*_lidx], _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serVngDecPLMN_Identity(_buffer, _size, _lidx, _mem, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecTMGI_r9_plmn_Id_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TMGI_r9_plmn_Id_r9_Sel)_tmp;
	}
	_serVngDecTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecTMGI_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, _mem, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->serviceId_r9[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecMBMSSessionInfo_r13(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecTMGI_r9(_buffer, _size, _lidx, _mem, &p->tmgi_r13);
	_serVngDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serVngDecSC_MRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecMBMSSessionInfo_r13(_buffer, _size, _lidx, _mem, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serVngDecRadioBearerId_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		NTOH_8(p->Srb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		NTOH_8(p->Drb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serVngDecMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serVngDecSC_MRB_Identity_Type(_buffer, _size, _lidx, _mem, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecRadioBearerId_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RadioBearerId_Type_Sel)_tmp;
	}
	_serVngDecRadioBearerId_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecQosFlow_Identification_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->PDU_SessionId, &_buffer[*_lidx], _lidx);
	NTOH_32(p->QFI, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngDecRoutingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serVngDecRadioBearerId_Type(_buffer, _size, _lidx, _mem, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serVngDecQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecRoutingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RoutingInfo_Type_Sel)_tmp;
	}
	_serVngDecRoutingInfo_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecSystemFrameNumberInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serVngDecSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serVngDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecSubFrameInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serVngDecSubFrameInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SubFrameInfo_Type_Sel)_tmp;
	}
	_serVngDecSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecHyperSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serVngDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecSlotOffset_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serVngDecSlotOffset_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotOffset_Type_Sel)_tmp;
	}
	_serVngDecSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecSlotTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serVngDecSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		NTOH_8(p->FirstSlot, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecSlotTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotTimingInfo_Type_Sel)_tmp;
	}
	_serVngDecSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecSubFrameTiming_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serVngDecSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serVngDecHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serVngDecSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serVngDecTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serVngDecSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		NTOH_8(p->Now, &_buffer[*_lidx], _lidx);
	}
	if (d == TimingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TimingInfo_Type_Sel)_tmp;
	}
	_serVngDecTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecReqAspControlInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ReqAspControlInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->CnfFlag, &_buffer[*_lidx], _lidx);
	NTOH_8(p->FollowOnFlag, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngDecRlcBearerRouting_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serVngDecRlcBearerRouting_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RlcBearerRouting_Type_Sel)_tmp;
	}
	_serVngDecRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serVngDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serVngDecReqAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct ReqAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serVngDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serVngDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serVngDecReqAspControlInfo_Type(_buffer, _size, _lidx, &p->ControlInfo);
	_serVngDecRlcBearerRouting_Type_ReqAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serVngDecEUTRA_VngConfigInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct EUTRA_VngConfigInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->Bandwidth = (Dl_Bandwidth_Type)_tmp;
	}
	NTOH_32(p->NocLevel, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serVngDecEUTRA_VngConfigRequest_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union EUTRA_VngConfigRequest_Type_Value* p, enum EUTRA_VngConfigRequest_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == EUTRA_VngConfigRequest_Type_Configure) {
		_serVngDecEUTRA_VngConfigInfo_Type(_buffer, _size, _lidx, &p->Configure);
	}
	if (d == EUTRA_VngConfigRequest_Type_Activate) {
		NTOH_8(p->Activate, &_buffer[*_lidx], _lidx);
	}
	if (d == EUTRA_VngConfigRequest_Type_Deactivate) {
		NTOH_8(p->Deactivate, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecEUTRA_VngConfigRequest_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct EUTRA_VngConfigRequest_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum EUTRA_VngConfigRequest_Type_Sel)_tmp;
	}
	_serVngDecEUTRA_VngConfigRequest_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecEUTRA_VNG_CTRL_REQ(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct EUTRA_VNG_CTRL_REQ* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecReqAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serVngDecEUTRA_VngConfigRequest_Type(_buffer, _size, _lidx, &p->Request);

	return SIDL_STATUS_OK;
}

int serVngProcessDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_VNG_CTRL_REQ** FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*FromSS = serMalloc(_mem, sizeof(struct EUTRA_VNG_CTRL_REQ));
	_serVngDecEUTRA_VNG_CTRL_REQ(_buffer, _size, _lidx, _mem, *FromSS);

	return SIDL_STATUS_OK;
}

static void _serVngFreePLMN_Identity(struct PLMN_Identity* p)
{
	if (p->mnc.v) {
		serFree(p->mnc.v);
	}
}

static void _serVngFreeTMGI_r9_plmn_Id_r9_Value(union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serVngFreePLMN_Identity(&p->explicitValue_r9);
	}
}

static void _serVngFreeTMGI_r9_plmn_Id_r9(struct TMGI_r9_plmn_Id_r9* p)
{
	_serVngFreeTMGI_r9_plmn_Id_r9_Value(&p->v, p->d);
}

static void _serVngFreeTMGI_r9(struct TMGI_r9* p)
{
	_serVngFreeTMGI_r9_plmn_Id_r9(&p->plmn_Id_r9);
}

static void _serVngFreeMBMSSessionInfo_r13(struct MBMSSessionInfo_r13* p)
{
	_serVngFreeTMGI_r9(&p->tmgi_r13);
}

static void _serVngFreeSC_MRB_Identity_Type(struct SC_MRB_Identity_Type* p)
{
	_serVngFreeMBMSSessionInfo_r13(&p->MbmsSessionInfo);
}

static void _serVngFreeRadioBearerId_Type_Value(union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	if (d == RadioBearerId_Type_ScMrb) {
		_serVngFreeSC_MRB_Identity_Type(&p->ScMrb);
	}
}

static void _serVngFreeRadioBearerId_Type(struct RadioBearerId_Type* p)
{
	_serVngFreeRadioBearerId_Type_Value(&p->v, p->d);
}

static void _serVngFreeRoutingInfo_Type_Value(union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serVngFreeRadioBearerId_Type(&p->RadioBearerId);
	}
}

static void _serVngFreeRoutingInfo_Type(struct RoutingInfo_Type* p)
{
	_serVngFreeRoutingInfo_Type_Value(&p->v, p->d);
}

static void _serVngFreeReqAspCommonPart_Type(struct ReqAspCommonPart_Type* p)
{
	_serVngFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serVngFreeEUTRA_VNG_CTRL_REQ(struct EUTRA_VNG_CTRL_REQ* p)
{
	_serVngFreeReqAspCommonPart_Type(&p->Common);
}

void serVngProcessFreeSrv(struct EUTRA_VNG_CTRL_REQ* FromSS)
{
	if (FromSS) {
		_serVngFreeEUTRA_VNG_CTRL_REQ(FromSS);
		serFree(FromSS);
	}
}

static int _serVngEncConfirmationResult_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union ConfirmationResult_Type_Value* p, enum ConfirmationResult_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == ConfirmationResult_Type_Success) {
		HTON_8(&_buffer[*_lidx], p->Success, _lidx);
	}
	if (d == ConfirmationResult_Type_Error) {
		HTON_32(&_buffer[*_lidx], p->Error, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngEncConfirmationResult_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ConfirmationResult_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncConfirmationResult_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngEncCnfAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct CnfAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serVngEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serVngEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serVngEncConfirmationResult_Type(_buffer, _size, _lidx, &p->Result);

	return SIDL_STATUS_OK;
}

static int _serVngEncEUTRA_VNG_CTRL_CNF(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_CNF* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncCnfAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	HTON_8(&_buffer[*_lidx], p->Confirm, _lidx);

	return SIDL_STATUS_OK;
}

int serVngProcessEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_CNF* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngEncEUTRA_VNG_CTRL_CNF(_buffer, _size, _lidx, ToSS);

	return SIDL_STATUS_OK;
}

static int _serVngDecConfirmationResult_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union ConfirmationResult_Type_Value* p, enum ConfirmationResult_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == ConfirmationResult_Type_Success) {
		NTOH_8(p->Success, &_buffer[*_lidx], _lidx);
	}
	if (d == ConfirmationResult_Type_Error) {
		NTOH_32(p->Error, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serVngDecConfirmationResult_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ConfirmationResult_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum ConfirmationResult_Type_Sel)_tmp;
	}
	_serVngDecConfirmationResult_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serVngDecCnfAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct CnfAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serVngDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serVngDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serVngDecConfirmationResult_Type(_buffer, _size, _lidx, &p->Result);

	return SIDL_STATUS_OK;
}

static int _serVngDecEUTRA_VNG_CTRL_CNF(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct EUTRA_VNG_CTRL_CNF* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serVngDecCnfAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	NTOH_8(p->Confirm, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

int serVngProcessDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_VNG_CTRL_CNF** ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*ToSS = serMalloc(_mem, sizeof(struct EUTRA_VNG_CTRL_CNF));
	_serVngDecEUTRA_VNG_CTRL_CNF(_buffer, _size, _lidx, _mem, *ToSS);

	return SIDL_STATUS_OK;
}

static void _serVngFreeCnfAspCommonPart_Type(struct CnfAspCommonPart_Type* p)
{
	_serVngFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serVngFreeEUTRA_VNG_CTRL_CNF(struct EUTRA_VNG_CTRL_CNF* p)
{
	_serVngFreeCnfAspCommonPart_Type(&p->Common);
}

void serVngProcessFreeClt(struct EUTRA_VNG_CTRL_CNF* ToSS)
{
	if (ToSS) {
		_serVngFreeEUTRA_VNG_CTRL_CNF(ToSS);
		serFree(ToSS);
	}
}
