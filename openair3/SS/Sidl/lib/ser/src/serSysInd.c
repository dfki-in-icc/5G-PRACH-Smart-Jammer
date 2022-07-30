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
#include "serSysInd.h"
#include "serMem.h"
#include "serUtils.h"

static int _serSysIndEncPmchLogicalChannel_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Pmch_InfoIndex, _lidx);
	HTON_8(&_buffer[*_lidx], p->LogicalChannelIdentity, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncMRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Mbsfn_AreaId, _lidx);
	_serSysIndEncPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncMCC_PLMN_Identity_mcc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncPLMN_Identity(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	HTON_32(&_buffer[*_lidx], p->mnc.d, _lidx);
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->mnc.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncTMGI_r9_plmn_Id_r9_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		HTON_8(&_buffer[*_lidx], p->plmn_Index_r9, _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysIndEncPLMN_Identity(_buffer, _size, _lidx, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncTMGI_r9_plmn_Id_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncTMGI_r9(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_8(&_buffer[*_lidx], p->serviceId_r9[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncMBMSSessionInfo_r13(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncTMGI_r9(_buffer, _size, _lidx, &p->tmgi_r13);
	_serSysIndEncO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSC_MRB_Identity_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncMBMSSessionInfo_r13(_buffer, _size, _lidx, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRadioBearerId_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		HTON_8(&_buffer[*_lidx], p->Srb, _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		HTON_8(&_buffer[*_lidx], p->Drb, _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serSysIndEncMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysIndEncSC_MRB_Identity_Type(_buffer, _size, _lidx, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRadioBearerId_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncRadioBearerId_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncQosFlow_Identification_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->PDU_SessionId, _lidx);
	HTON_32(&_buffer[*_lidx], p->QFI, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRoutingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysIndEncRadioBearerId_Type(_buffer, _size, _lidx, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serSysIndEncQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRoutingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncRoutingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSystemFrameNumberInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serSysIndEncSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSubFrameInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serSysIndEncSubFrameInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncHyperSystemFrameNumberInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSlotOffset_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serSysIndEncSlotOffset_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSlotTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serSysIndEncSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		HTON_8(&_buffer[*_lidx], p->FirstSlot, _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		HTON_8(&_buffer[*_lidx], p->Any, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSlotTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSubFrameTiming_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serSysIndEncSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serSysIndEncHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serSysIndEncSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncTimingInfo_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serSysIndEncSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		HTON_8(&_buffer[*_lidx], p->Now, _lidx);
	}
	if (d == TimingInfo_Type_None) {
		HTON_8(&_buffer[*_lidx], p->None, _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncTimingInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncIntegrityErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Nas, _lidx);
	HTON_8(&_buffer[*_lidx], p->Pdcp, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncErrorIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	HTON_32(&_buffer[*_lidx], p->System, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncIndicationStatus_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		HTON_8(&_buffer[*_lidx], p->Ok, _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serSysIndEncErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncIndicationStatus_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRlcBearerRouting_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serSysIndEncRlcBearerRouting_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysIndEncRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncIndAspCommonPart_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->CellId;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncRoutingInfo_Type(_buffer, _size, _lidx, &p->RoutingInfo);
	_serSysIndEncTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysIndEncIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serSysIndEncRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncInt_RepetitionsPerPreambleAttempt_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_RepetitionsPerPreambleAttempt_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRachPreamble_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RachPreamble_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->RAPID, _lidx);
	HTON_8(&_buffer[*_lidx], p->PRTPower, _lidx);
	_serSysIndEncInt_RepetitionsPerPreambleAttempt_Optional(_buffer, _size, _lidx, &p->RepetitionsPerPreambleAttempt);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncShort_BSR_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Short_BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->LCG, _lidx);
	HTON_32(&_buffer[*_lidx], p->Value, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncLong_BSR_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Long_BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->Value_LCG1, _lidx);
	HTON_32(&_buffer[*_lidx], p->Value_LCG2, _lidx);
	HTON_32(&_buffer[*_lidx], p->Value_LCG3, _lidx);
	HTON_32(&_buffer[*_lidx], p->Value_LCG4, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSL_BSR_Value_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SL_BSR_Value_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i2 = 0; i2 < 4; i2++) {
		HTON_8(&_buffer[*_lidx], p->DestinationIndex[i2], _lidx);
	}
	for (size_t i2 = 0; i2 < 2; i2++) {
		HTON_8(&_buffer[*_lidx], p->LCG_ID[i2], _lidx);
	}
	for (size_t i2 = 0; i2 < 6; i2++) {
		HTON_8(&_buffer[*_lidx], p->BufferSize[i2], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSL_BSR_MACHeader_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SL_BSR_MACHeader_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 5; i1++) {
		HTON_8(&_buffer[*_lidx], p->HeaderFieldLCID[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->HeaderFieldE[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->HeaderFieldF[i1], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->SL_BSR_Value.d, _lidx);
	for (size_t i1 = 0; i1 < p->SL_BSR_Value.d; i1++) {
		_serSysIndEncSL_BSR_Value_Type(_buffer, _size, _lidx, &p->SL_BSR_Value.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncBSR_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union BSR_Type_Value* p, enum BSR_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == BSR_Type_Short) {
		_serSysIndEncShort_BSR_Type(_buffer, _size, _lidx, &p->Short);
	}
	if (d == BSR_Type_Truncated) {
		_serSysIndEncShort_BSR_Type(_buffer, _size, _lidx, &p->Truncated);
	}
	if (d == BSR_Type_Long) {
		_serSysIndEncLong_BSR_Type(_buffer, _size, _lidx, &p->Long);
	}
	if (d == BSR_Type_Sidelink) {
		_serSysIndEncSL_BSR_MACHeader_Type(_buffer, _size, _lidx, &p->Sidelink);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncBSR_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncBSR_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncHarqProcessInfo_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct HarqProcessInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->Id, _lidx);
	HTON_32(&_buffer[*_lidx], p->CURRENT_TX_NB, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncHarqError_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union HarqError_Type_Value* p, enum HarqError_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == HarqError_Type_UL) {
		_serSysIndEncHarqProcessInfo_Type(_buffer, _size, _lidx, &p->UL);
	}
	if (d == HarqError_Type_DL) {
		_serSysIndEncHarqProcessInfo_Type(_buffer, _size, _lidx, &p->DL);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncHarqError_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct HarqError_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncHarqError_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncRlcDiscardInd_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct RlcDiscardInd_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->SequenceNumber, _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncScellBitMap_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct ScellBitMap_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 7; i1++) {
		HTON_8(&_buffer[*_lidx], p->Value[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		HTON_8(&_buffer[*_lidx], p->Reserved[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncB2_Type_Reserved_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct B2_Type_Reserved_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i2 = 0; i2 < 2; i2++) {
		HTON_8(&_buffer[*_lidx], p->v[i2], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncB6_Type_PCMaxc_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct B6_Type_PCMaxc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i2 = 0; i2 < 6; i2++) {
		HTON_8(&_buffer[*_lidx], p->v[i2], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncPH_Record_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct PH_Record_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i2 = 0; i2 < 1; i2++) {
		HTON_8(&_buffer[*_lidx], p->P_Bit[i2], _lidx);
	}
	for (size_t i2 = 0; i2 < 1; i2++) {
		HTON_8(&_buffer[*_lidx], p->V_Bit[i2], _lidx);
	}
	for (size_t i2 = 0; i2 < 6; i2++) {
		HTON_8(&_buffer[*_lidx], p->Valve[i2], _lidx);
	}
	_serSysIndEncB2_Type_Reserved_Optional(_buffer, _size, _lidx, &p->Reserved);
	_serSysIndEncB6_Type_PCMaxc_Optional(_buffer, _size, _lidx, &p->PCMaxc);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncMAC_CTRL_ExtPowerHeadRoom_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncScellBitMap_Type(_buffer, _size, _lidx, &p->EPH_Octet1);
	HTON_32(&_buffer[*_lidx], p->PH_RecordList.d, _lidx);
	for (size_t i1 = 0; i1 < p->PH_RecordList.d; i1++) {
		_serSysIndEncPH_Record_Type(_buffer, _size, _lidx, &p->PH_RecordList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncMAC_CTRL_DC_PowerHeadRoom_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncScellBitMap_Type(_buffer, _size, _lidx, &p->DC_PH_Octet1);
	HTON_32(&_buffer[*_lidx], p->DC_PH_RecordList.d, _lidx);
	for (size_t i1 = 0; i1 < p->DC_PH_RecordList.d; i1++) {
		_serSysIndEncPH_Record_Type(_buffer, _size, _lidx, &p->DC_PH_RecordList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSystemIndication_Type_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union SystemIndication_Type_Value* p, enum SystemIndication_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SystemIndication_Type_Error) {
		HTON_32(&_buffer[*_lidx], p->Error.d, _lidx);
		for (size_t i1 = 0; i1 < p->Error.d; i1++) {
			HTON_8(&_buffer[*_lidx], p->Error.v[i1], _lidx);
		}
	}
	if (d == SystemIndication_Type_RachPreamble) {
		_serSysIndEncRachPreamble_Type(_buffer, _size, _lidx, &p->RachPreamble);
	}
	if (d == SystemIndication_Type_SchedReq) {
		HTON_8(&_buffer[*_lidx], p->SchedReq, _lidx);
	}
	if (d == SystemIndication_Type_BSR) {
		_serSysIndEncBSR_Type(_buffer, _size, _lidx, &p->BSR);
	}
	if (d == SystemIndication_Type_UL_HARQ) {
		{
			size_t _tmp = (size_t)p->UL_HARQ;
			HTON_32(&_buffer[*_lidx], _tmp, _lidx);
		}
	}
	if (d == SystemIndication_Type_C_RNTI) {
		for (size_t i1 = 0; i1 < 16; i1++) {
			HTON_8(&_buffer[*_lidx], p->C_RNTI[i1], _lidx);
		}
	}
	if (d == SystemIndication_Type_PHR) {
		HTON_8(&_buffer[*_lidx], p->PHR, _lidx);
	}
	if (d == SystemIndication_Type_HarqError) {
		_serSysIndEncHarqError_Type(_buffer, _size, _lidx, &p->HarqError);
	}
	if (d == SystemIndication_Type_RlcDiscardInd) {
		_serSysIndEncRlcDiscardInd_Type(_buffer, _size, _lidx, &p->RlcDiscardInd);
	}
	if (d == SystemIndication_Type_PeriodicRI) {
		HTON_8(&_buffer[*_lidx], p->PeriodicRI, _lidx);
	}
	if (d == SystemIndication_Type_EPHR) {
		_serSysIndEncMAC_CTRL_ExtPowerHeadRoom_Type(_buffer, _size, _lidx, &p->EPHR);
	}
	if (d == SystemIndication_Type_CqiInd) {
		HTON_8(&_buffer[*_lidx], p->CqiInd, _lidx);
	}
	if (d == SystemIndication_Type_SrsInd) {
		{
			size_t _tmp = (size_t)p->SrsInd;
			HTON_32(&_buffer[*_lidx], _tmp, _lidx);
		}
	}
	if (d == SystemIndication_Type_DC_PHR) {
		_serSysIndEncMAC_CTRL_DC_PowerHeadRoom_Type(_buffer, _size, _lidx, &p->DC_PHR);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSystemIndication_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SystemIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serSysIndEncSystemIndication_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndEncSYSTEM_IND(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SYSTEM_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncIndAspCommonPart_Type(_buffer, _size, _lidx, &p->Common);
	_serSysIndEncSystemIndication_Type(_buffer, _size, _lidx, &p->Indication);

	return SIDL_STATUS_OK;
}

int serSysIndProcessToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct SYSTEM_IND* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndEncSYSTEM_IND(_buffer, _size, _lidx, ToSS);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecPmchLogicalChannel_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PmchLogicalChannel_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Pmch_InfoIndex, &_buffer[*_lidx], _lidx);
	NTOH_8(p->LogicalChannelIdentity, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecMRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Mbsfn_AreaId, &_buffer[*_lidx], _lidx);
	_serSysIndDecPmchLogicalChannel_Type(_buffer, _size, _lidx, &p->PmchLogicalChannel);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecMCC_PLMN_Identity_mcc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct MCC_PLMN_Identity_mcc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecPLMN_Identity(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct PLMN_Identity* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecMCC_PLMN_Identity_mcc_Optional(_buffer, _size, _lidx, &p->mcc);
	NTOH_32(p->mnc.d, &_buffer[*_lidx], _lidx);
	p->mnc.v = serMalloc(_mem, p->mnc.d * sizeof(MCC_MNC_Digit));
	for (size_t i1 = 0; i1 < p->mnc.d; i1++) {
		NTOH_8(p->mnc.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecTMGI_r9_plmn_Id_r9_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TMGI_r9_plmn_Id_r9_plmn_Index_r9) {
		NTOH_8(p->plmn_Index_r9, &_buffer[*_lidx], _lidx);
	}
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysIndDecPLMN_Identity(_buffer, _size, _lidx, _mem, &p->explicitValue_r9);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecTMGI_r9_plmn_Id_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9_plmn_Id_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TMGI_r9_plmn_Id_r9_Sel)_tmp;
	}
	_serSysIndDecTMGI_r9_plmn_Id_r9_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecTMGI_r9(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TMGI_r9* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecTMGI_r9_plmn_Id_r9(_buffer, _size, _lidx, _mem, &p->plmn_Id_r9);
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_8(p->serviceId_r9[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct O1_MBMSSessionInfo_r13_sessionId_r13_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecMBMSSessionInfo_r13(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MBMSSessionInfo_r13* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecTMGI_r9(_buffer, _size, _lidx, _mem, &p->tmgi_r13);
	_serSysIndDecO1_MBMSSessionInfo_r13_sessionId_r13_Optional(_buffer, _size, _lidx, &p->sessionId_r13);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSC_MRB_Identity_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SC_MRB_Identity_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecMBMSSessionInfo_r13(_buffer, _size, _lidx, _mem, &p->MbmsSessionInfo);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRadioBearerId_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RadioBearerId_Type_Srb) {
		NTOH_8(p->Srb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Drb) {
		NTOH_8(p->Drb, &_buffer[*_lidx], _lidx);
	}
	if (d == RadioBearerId_Type_Mrb) {
		_serSysIndDecMRB_Identity_Type(_buffer, _size, _lidx, &p->Mrb);
	}
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysIndDecSC_MRB_Identity_Type(_buffer, _size, _lidx, _mem, &p->ScMrb);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRadioBearerId_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RadioBearerId_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RadioBearerId_Type_Sel)_tmp;
	}
	_serSysIndDecRadioBearerId_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecQosFlow_Identification_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct QosFlow_Identification_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->PDU_SessionId, &_buffer[*_lidx], _lidx);
	NTOH_32(p->QFI, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRoutingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == RoutingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysIndDecRadioBearerId_Type(_buffer, _size, _lidx, _mem, &p->RadioBearerId);
	}
	if (d == RoutingInfo_Type_QosFlow) {
		_serSysIndDecQosFlow_Identification_Type(_buffer, _size, _lidx, &p->QosFlow);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRoutingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct RoutingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RoutingInfo_Type_Sel)_tmp;
	}
	_serSysIndDecRoutingInfo_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSystemFrameNumberInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SystemFrameNumberInfo_Type_Value* p, enum SystemFrameNumberInfo_Type_Sel d)
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

static int _serSysIndDecSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serSysIndDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSubFrameInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SubFrameInfo_Type_Value* p, enum SubFrameInfo_Type_Sel d)
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

static int _serSysIndDecSubFrameInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SubFrameInfo_Type_Sel)_tmp;
	}
	_serSysIndDecSubFrameInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecHyperSystemFrameNumberInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, HyperSystemFrameNumberInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemFrameNumberInfo_Type_Sel)_tmp;
	}
	_serSysIndDecSystemFrameNumberInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSlotOffset_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotOffset_Type_Value* p, enum SlotOffset_Type_Sel d)
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

static int _serSysIndDecSlotOffset_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotOffset_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotOffset_Type_Sel)_tmp;
	}
	_serSysIndDecSlotOffset_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSlotTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union SlotTimingInfo_Type_Value* p, enum SlotTimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SlotTimingInfo_Type_SlotOffset) {
		_serSysIndDecSlotOffset_Type(_buffer, _size, _lidx, &p->SlotOffset);
	}
	if (d == SlotTimingInfo_Type_FirstSlot) {
		NTOH_8(p->FirstSlot, &_buffer[*_lidx], _lidx);
	}
	if (d == SlotTimingInfo_Type_Any) {
		NTOH_8(p->Any, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSlotTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SlotTimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SlotTimingInfo_Type_Sel)_tmp;
	}
	_serSysIndDecSlotTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSubFrameTiming_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SubFrameTiming_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->SFN);
	_serSysIndDecSubFrameInfo_Type(_buffer, _size, _lidx, &p->Subframe);
	_serSysIndDecHyperSystemFrameNumberInfo_Type(_buffer, _size, _lidx, &p->HSFN);
	_serSysIndDecSlotTimingInfo_Type(_buffer, _size, _lidx, &p->Slot);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecTimingInfo_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union TimingInfo_Type_Value* p, enum TimingInfo_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TimingInfo_Type_SubFrame) {
		_serSysIndDecSubFrameTiming_Type(_buffer, _size, _lidx, &p->SubFrame);
	}
	if (d == TimingInfo_Type_Now) {
		NTOH_8(p->Now, &_buffer[*_lidx], _lidx);
	}
	if (d == TimingInfo_Type_None) {
		NTOH_8(p->None, &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecTimingInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TimingInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TimingInfo_Type_Sel)_tmp;
	}
	_serSysIndDecTimingInfo_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecIntegrityErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IntegrityErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Nas, &_buffer[*_lidx], _lidx);
	NTOH_8(p->Pdcp, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecErrorIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ErrorIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecIntegrityErrorIndication_Type(_buffer, _size, _lidx, &p->Integrity);
	NTOH_32(p->System, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecIndicationStatus_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union IndicationStatus_Type_Value* p, enum IndicationStatus_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == IndicationStatus_Type_Ok) {
		NTOH_8(p->Ok, &_buffer[*_lidx], _lidx);
	}
	if (d == IndicationStatus_Type_Error) {
		_serSysIndDecErrorIndication_Type(_buffer, _size, _lidx, &p->Error);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecIndicationStatus_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct IndicationStatus_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum IndicationStatus_Type_Sel)_tmp;
	}
	_serSysIndDecIndicationStatus_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRlcBearerRouting_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union RlcBearerRouting_Type_Value* p, enum RlcBearerRouting_Type_Sel d)
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

static int _serSysIndDecRlcBearerRouting_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum RlcBearerRouting_Type_Sel)_tmp;
	}
	_serSysIndDecRlcBearerRouting_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serSysIndDecRlcBearerRouting_Type(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecIndAspCommonPart_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct IndAspCommonPart_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->CellId = (EUTRA_CellId_Type)_tmp;
	}
	_serSysIndDecRoutingInfo_Type(_buffer, _size, _lidx, _mem, &p->RoutingInfo);
	_serSysIndDecTimingInfo_Type(_buffer, _size, _lidx, &p->TimingInfo);
	_serSysIndDecIndicationStatus_Type(_buffer, _size, _lidx, &p->Status);
	_serSysIndDecRlcBearerRouting_Type_IndAspCommonPart_Type_RlcBearerRouting_Optional(_buffer, _size, _lidx, &p->RlcBearerRouting);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecInt_RepetitionsPerPreambleAttempt_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_RepetitionsPerPreambleAttempt_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRachPreamble_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RachPreamble_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->RAPID, &_buffer[*_lidx], _lidx);
	NTOH_8(p->PRTPower, &_buffer[*_lidx], _lidx);
	_serSysIndDecInt_RepetitionsPerPreambleAttempt_Optional(_buffer, _size, _lidx, &p->RepetitionsPerPreambleAttempt);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecShort_BSR_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Short_BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->LCG, &_buffer[*_lidx], _lidx);
	NTOH_32(p->Value, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecLong_BSR_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Long_BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->Value_LCG1, &_buffer[*_lidx], _lidx);
	NTOH_32(p->Value_LCG2, &_buffer[*_lidx], _lidx);
	NTOH_32(p->Value_LCG3, &_buffer[*_lidx], _lidx);
	NTOH_32(p->Value_LCG4, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSL_BSR_Value_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct SL_BSR_Value_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i2 = 0; i2 < 4; i2++) {
		NTOH_8(p->DestinationIndex[i2], &_buffer[*_lidx], _lidx);
	}
	for (size_t i2 = 0; i2 < 2; i2++) {
		NTOH_8(p->LCG_ID[i2], &_buffer[*_lidx], _lidx);
	}
	for (size_t i2 = 0; i2 < 6; i2++) {
		NTOH_8(p->BufferSize[i2], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSL_BSR_MACHeader_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SL_BSR_MACHeader_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 5; i1++) {
		NTOH_8(p->HeaderFieldLCID[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->HeaderFieldE[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->HeaderFieldF[i1], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->SL_BSR_Value.d, &_buffer[*_lidx], _lidx);
	p->SL_BSR_Value.v = serMalloc(_mem, p->SL_BSR_Value.d * sizeof(struct SL_BSR_Value_Type));
	for (size_t i1 = 0; i1 < p->SL_BSR_Value.d; i1++) {
		_serSysIndDecSL_BSR_Value_Type(_buffer, _size, _lidx, &p->SL_BSR_Value.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecBSR_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union BSR_Type_Value* p, enum BSR_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == BSR_Type_Short) {
		_serSysIndDecShort_BSR_Type(_buffer, _size, _lidx, &p->Short);
	}
	if (d == BSR_Type_Truncated) {
		_serSysIndDecShort_BSR_Type(_buffer, _size, _lidx, &p->Truncated);
	}
	if (d == BSR_Type_Long) {
		_serSysIndDecLong_BSR_Type(_buffer, _size, _lidx, &p->Long);
	}
	if (d == BSR_Type_Sidelink) {
		_serSysIndDecSL_BSR_MACHeader_Type(_buffer, _size, _lidx, _mem, &p->Sidelink);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecBSR_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct BSR_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum BSR_Type_Sel)_tmp;
	}
	_serSysIndDecBSR_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecHarqProcessInfo_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct HarqProcessInfo_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->Id, &_buffer[*_lidx], _lidx);
	NTOH_32(p->CURRENT_TX_NB, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecHarqError_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union HarqError_Type_Value* p, enum HarqError_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == HarqError_Type_UL) {
		_serSysIndDecHarqProcessInfo_Type(_buffer, _size, _lidx, &p->UL);
	}
	if (d == HarqError_Type_DL) {
		_serSysIndDecHarqProcessInfo_Type(_buffer, _size, _lidx, &p->DL);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecHarqError_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct HarqError_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum HarqError_Type_Sel)_tmp;
	}
	_serSysIndDecHarqError_Type_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecRlcDiscardInd_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct RlcDiscardInd_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->SequenceNumber, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecScellBitMap_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct ScellBitMap_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 7; i1++) {
		NTOH_8(p->Value[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 1; i1++) {
		NTOH_8(p->Reserved[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecB2_Type_Reserved_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct B2_Type_Reserved_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i2 = 0; i2 < 2; i2++) {
		NTOH_8(p->v[i2], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecB6_Type_PCMaxc_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct B6_Type_PCMaxc_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i2 = 0; i2 < 6; i2++) {
		NTOH_8(p->v[i2], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecPH_Record_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct PH_Record_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i2 = 0; i2 < 1; i2++) {
		NTOH_8(p->P_Bit[i2], &_buffer[*_lidx], _lidx);
	}
	for (size_t i2 = 0; i2 < 1; i2++) {
		NTOH_8(p->V_Bit[i2], &_buffer[*_lidx], _lidx);
	}
	for (size_t i2 = 0; i2 < 6; i2++) {
		NTOH_8(p->Valve[i2], &_buffer[*_lidx], _lidx);
	}
	_serSysIndDecB2_Type_Reserved_Optional(_buffer, _size, _lidx, &p->Reserved);
	_serSysIndDecB6_Type_PCMaxc_Optional(_buffer, _size, _lidx, &p->PCMaxc);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecMAC_CTRL_ExtPowerHeadRoom_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecScellBitMap_Type(_buffer, _size, _lidx, &p->EPH_Octet1);
	NTOH_32(p->PH_RecordList.d, &_buffer[*_lidx], _lidx);
	p->PH_RecordList.v = serMalloc(_mem, p->PH_RecordList.d * sizeof(struct PH_Record_Type));
	for (size_t i1 = 0; i1 < p->PH_RecordList.d; i1++) {
		_serSysIndDecPH_Record_Type(_buffer, _size, _lidx, &p->PH_RecordList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecMAC_CTRL_DC_PowerHeadRoom_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecScellBitMap_Type(_buffer, _size, _lidx, &p->DC_PH_Octet1);
	NTOH_32(p->DC_PH_RecordList.d, &_buffer[*_lidx], _lidx);
	p->DC_PH_RecordList.v = serMalloc(_mem, p->DC_PH_RecordList.d * sizeof(struct PH_Record_Type));
	for (size_t i1 = 0; i1 < p->DC_PH_RecordList.d; i1++) {
		_serSysIndDecPH_Record_Type(_buffer, _size, _lidx, &p->DC_PH_RecordList.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSystemIndication_Type_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, union SystemIndication_Type_Value* p, enum SystemIndication_Type_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == SystemIndication_Type_Error) {
		NTOH_32(p->Error.d, &_buffer[*_lidx], _lidx);
		p->Error.v = serMalloc(_mem, p->Error.d * sizeof(CHAR_STRING_ELEMENT));
		for (size_t i1 = 0; i1 < p->Error.d; i1++) {
			NTOH_8(p->Error.v[i1], &_buffer[*_lidx], _lidx);
		}
	}
	if (d == SystemIndication_Type_RachPreamble) {
		_serSysIndDecRachPreamble_Type(_buffer, _size, _lidx, &p->RachPreamble);
	}
	if (d == SystemIndication_Type_SchedReq) {
		NTOH_8(p->SchedReq, &_buffer[*_lidx], _lidx);
	}
	if (d == SystemIndication_Type_BSR) {
		_serSysIndDecBSR_Type(_buffer, _size, _lidx, _mem, &p->BSR);
	}
	if (d == SystemIndication_Type_UL_HARQ) {
		{
			size_t _tmp;
			NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
			p->UL_HARQ = (HARQ_Type)_tmp;
		}
	}
	if (d == SystemIndication_Type_C_RNTI) {
		for (size_t i1 = 0; i1 < 16; i1++) {
			NTOH_8(p->C_RNTI[i1], &_buffer[*_lidx], _lidx);
		}
	}
	if (d == SystemIndication_Type_PHR) {
		NTOH_8(p->PHR, &_buffer[*_lidx], _lidx);
	}
	if (d == SystemIndication_Type_HarqError) {
		_serSysIndDecHarqError_Type(_buffer, _size, _lidx, &p->HarqError);
	}
	if (d == SystemIndication_Type_RlcDiscardInd) {
		_serSysIndDecRlcDiscardInd_Type(_buffer, _size, _lidx, &p->RlcDiscardInd);
	}
	if (d == SystemIndication_Type_PeriodicRI) {
		NTOH_8(p->PeriodicRI, &_buffer[*_lidx], _lidx);
	}
	if (d == SystemIndication_Type_EPHR) {
		_serSysIndDecMAC_CTRL_ExtPowerHeadRoom_Type(_buffer, _size, _lidx, _mem, &p->EPHR);
	}
	if (d == SystemIndication_Type_CqiInd) {
		NTOH_8(p->CqiInd, &_buffer[*_lidx], _lidx);
	}
	if (d == SystemIndication_Type_SrsInd) {
		{
			size_t _tmp;
			NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
			p->SrsInd = (SRSInd_Type)_tmp;
		}
	}
	if (d == SystemIndication_Type_DC_PHR) {
		_serSysIndDecMAC_CTRL_DC_PowerHeadRoom_Type(_buffer, _size, _lidx, _mem, &p->DC_PHR);
	}

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSystemIndication_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SystemIndication_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum SystemIndication_Type_Sel)_tmp;
	}
	_serSysIndDecSystemIndication_Type_Value(_buffer, _size, _lidx, _mem, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serSysIndDecSYSTEM_IND(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct SYSTEM_IND* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serSysIndDecIndAspCommonPart_Type(_buffer, _size, _lidx, _mem, &p->Common);
	_serSysIndDecSystemIndication_Type(_buffer, _size, _lidx, _mem, &p->Indication);

	return SIDL_STATUS_OK;
}

int serSysIndProcessToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct SYSTEM_IND** ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*ToSS = serMalloc(_mem, sizeof(struct SYSTEM_IND));
	_serSysIndDecSYSTEM_IND(_buffer, _size, _lidx, _mem, *ToSS);

	return SIDL_STATUS_OK;
}

static void _serSysIndFreePLMN_Identity(struct PLMN_Identity* p)
{
	if (p->mnc.v) {
		serFree(p->mnc.v);
	}
}

static void _serSysIndFreeTMGI_r9_plmn_Id_r9_Value(union TMGI_r9_plmn_Id_r9_Value* p, enum TMGI_r9_plmn_Id_r9_Sel d)
{
	if (d == TMGI_r9_plmn_Id_r9_explicitValue_r9) {
		_serSysIndFreePLMN_Identity(&p->explicitValue_r9);
	}
}

static void _serSysIndFreeTMGI_r9_plmn_Id_r9(struct TMGI_r9_plmn_Id_r9* p)
{
	_serSysIndFreeTMGI_r9_plmn_Id_r9_Value(&p->v, p->d);
}

static void _serSysIndFreeTMGI_r9(struct TMGI_r9* p)
{
	_serSysIndFreeTMGI_r9_plmn_Id_r9(&p->plmn_Id_r9);
}

static void _serSysIndFreeMBMSSessionInfo_r13(struct MBMSSessionInfo_r13* p)
{
	_serSysIndFreeTMGI_r9(&p->tmgi_r13);
}

static void _serSysIndFreeSC_MRB_Identity_Type(struct SC_MRB_Identity_Type* p)
{
	_serSysIndFreeMBMSSessionInfo_r13(&p->MbmsSessionInfo);
}

static void _serSysIndFreeRadioBearerId_Type_Value(union RadioBearerId_Type_Value* p, enum RadioBearerId_Type_Sel d)
{
	if (d == RadioBearerId_Type_ScMrb) {
		_serSysIndFreeSC_MRB_Identity_Type(&p->ScMrb);
	}
}

static void _serSysIndFreeRadioBearerId_Type(struct RadioBearerId_Type* p)
{
	_serSysIndFreeRadioBearerId_Type_Value(&p->v, p->d);
}

static void _serSysIndFreeRoutingInfo_Type_Value(union RoutingInfo_Type_Value* p, enum RoutingInfo_Type_Sel d)
{
	if (d == RoutingInfo_Type_RadioBearerId) {
		_serSysIndFreeRadioBearerId_Type(&p->RadioBearerId);
	}
}

static void _serSysIndFreeRoutingInfo_Type(struct RoutingInfo_Type* p)
{
	_serSysIndFreeRoutingInfo_Type_Value(&p->v, p->d);
}

static void _serSysIndFreeIndAspCommonPart_Type(struct IndAspCommonPart_Type* p)
{
	_serSysIndFreeRoutingInfo_Type(&p->RoutingInfo);
}

static void _serSysIndFreeSL_BSR_MACHeader_Type(struct SL_BSR_MACHeader_Type* p)
{
	if (p->SL_BSR_Value.v) {
		for (size_t i1 = 0; i1 < p->SL_BSR_Value.d; i1++) {
		}
		serFree(p->SL_BSR_Value.v);
	}
}

static void _serSysIndFreeBSR_Type_Value(union BSR_Type_Value* p, enum BSR_Type_Sel d)
{
	if (d == BSR_Type_Sidelink) {
		_serSysIndFreeSL_BSR_MACHeader_Type(&p->Sidelink);
	}
}

static void _serSysIndFreeBSR_Type(struct BSR_Type* p)
{
	_serSysIndFreeBSR_Type_Value(&p->v, p->d);
}

static void _serSysIndFreeMAC_CTRL_ExtPowerHeadRoom_Type(struct MAC_CTRL_ExtPowerHeadRoom_Type* p)
{
	if (p->PH_RecordList.v) {
		for (size_t i1 = 0; i1 < p->PH_RecordList.d; i1++) {
		}
		serFree(p->PH_RecordList.v);
	}
}

static void _serSysIndFreeMAC_CTRL_DC_PowerHeadRoom_Type(struct MAC_CTRL_DC_PowerHeadRoom_Type* p)
{
	if (p->DC_PH_RecordList.v) {
		for (size_t i1 = 0; i1 < p->DC_PH_RecordList.d; i1++) {
		}
		serFree(p->DC_PH_RecordList.v);
	}
}

static void _serSysIndFreeSystemIndication_Type_Value(union SystemIndication_Type_Value* p, enum SystemIndication_Type_Sel d)
{
	if (d == SystemIndication_Type_Error) {
		if (p->Error.v) {
			serFree(p->Error.v);
		}
	}
	if (d == SystemIndication_Type_BSR) {
		_serSysIndFreeBSR_Type(&p->BSR);
	}
	if (d == SystemIndication_Type_EPHR) {
		_serSysIndFreeMAC_CTRL_ExtPowerHeadRoom_Type(&p->EPHR);
	}
	if (d == SystemIndication_Type_DC_PHR) {
		_serSysIndFreeMAC_CTRL_DC_PowerHeadRoom_Type(&p->DC_PHR);
	}
}

static void _serSysIndFreeSystemIndication_Type(struct SystemIndication_Type* p)
{
	_serSysIndFreeSystemIndication_Type_Value(&p->v, p->d);
}

static void _serSysIndFreeSYSTEM_IND(struct SYSTEM_IND* p)
{
	_serSysIndFreeIndAspCommonPart_Type(&p->Common);
	_serSysIndFreeSystemIndication_Type(&p->Indication);
}

void serSysIndProcessToSSFreeClt(struct SYSTEM_IND* ToSS)
{
	if (ToSS) {
		_serSysIndFreeSYSTEM_IND(ToSS);
		serFree(ToSS);
	}
}
