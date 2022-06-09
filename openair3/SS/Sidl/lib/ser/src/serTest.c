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
#include "serTest.h"
#include "serMem.h"
#include "serUtils.h"

int serTestHelloFromSSEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, size_t StrQty, const char* StrArray)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], StrQty, _lidx);
	for (size_t i1 = 0; i1 < StrQty; i1++) {
		HTON_8(&_buffer[*_lidx], StrArray[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serTestHelloFromSSDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, size_t* StrQty, char** StrArray)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	NTOH_32(*StrQty, &_buffer[*_lidx], _lidx);
	*StrArray = serMalloc(_mem, *StrQty * sizeof(char));
	for (size_t i1 = 0; i1 < *StrQty; i1++) {
		NTOH_8((*StrArray)[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

void serTestHelloFromSSFreeSrv(char* StrArray)
{
	if (StrArray) {
		serFree(StrArray);
	}
}

int serTestHelloToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, size_t StrQty, const char* StrArray)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], StrQty, _lidx);
	for (size_t i1 = 0; i1 < StrQty; i1++) {
		HTON_8(&_buffer[*_lidx], StrArray[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serTestHelloToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, size_t* StrQty, char** StrArray)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	NTOH_32(*StrQty, &_buffer[*_lidx], _lidx);
	*StrArray = serMalloc(_mem, *StrQty * sizeof(char));
	for (size_t i1 = 0; i1 < *StrQty; i1++) {
		NTOH_8((*StrArray)[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

void serTestHelloToSSFreeClt(char* StrArray)
{
	if (StrArray) {
		serFree(StrArray);
	}
}

int serTestPingEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, uint32_t FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], FromSS, _lidx);

	return SIDL_STATUS_OK;
}

int serTestPingDecSrv(const unsigned char* _buffer, size_t _size, uint32_t* FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	NTOH_32(*FromSS, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

int serTestPingEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, uint32_t ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], ToSS, _lidx);

	return SIDL_STATUS_OK;
}

int serTestPingDecClt(const unsigned char* _buffer, size_t _size, uint32_t* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	NTOH_32(*ToSS, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->dummy, _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncEchoData(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EchoData* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmpLen = (strlen(p->str ? p->str : "")) + 1;
		memcpy(&_buffer[*_lidx], (p->str ? p->str : ""), _tmpLen);
		*_lidx += _tmpLen;
	}
	HTON_32(&_buffer[*_lidx], p->emptyQty, _lidx);
	for (size_t i1 = 0; i1 < p->emptyQty; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->emptyArray[i1]);
	}
	_serTestEncEmpty(_buffer, _size, _lidx, p->eee);
	_serTestEncEmpty(_buffer, _size, _lidx, &p->sss);

	return SIDL_STATUS_OK;
}

int serTestEchoEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EchoData* FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncEchoData(_buffer, _size, _lidx, FromSS);

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->dummy, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestDecEchoData(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct EchoData* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmpLen = strlen((const char*)&_buffer[*_lidx]) + 1;
		p->str = (char*)serMalloc(_mem, _tmpLen);
		memcpy(p->str, &_buffer[*_lidx], _tmpLen);
		*_lidx += _tmpLen;
	}
	NTOH_32(p->emptyQty, &_buffer[*_lidx], _lidx);
	p->emptyArray = serMalloc(_mem, p->emptyQty * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < p->emptyQty; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->emptyArray[i1]);
	}
	p->eee = serMalloc(_mem, sizeof(struct Empty));
	_serTestDecEmpty(_buffer, _size, _lidx, p->eee);
	_serTestDecEmpty(_buffer, _size, _lidx, &p->sss);

	return SIDL_STATUS_OK;
}

int serTestEchoDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EchoData** FromSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*FromSS = serMalloc(_mem, sizeof(struct EchoData));
	_serTestDecEchoData(_buffer, _size, _lidx, _mem, *FromSS);

	return SIDL_STATUS_OK;
}

static void _serTestFreeEchoData(struct EchoData* p)
{
	if (p->str) {
		serFree(p->str);
	}
	if (p->emptyArray) {
		for (size_t i1 = 0; i1 < p->emptyQty; i1++) {
		}
		serFree(p->emptyArray);
	}
}

void serTestEchoFreeSrv(struct EchoData* FromSS)
{
	if (FromSS) {
		_serTestFreeEchoData(FromSS);
		serFree(FromSS);
	}
}

int serTestEchoEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EchoData* ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncEchoData(_buffer, _size, _lidx, ToSS);

	return SIDL_STATUS_OK;
}

int serTestEchoDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EchoData** ToSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*ToSS = serMalloc(_mem, sizeof(struct EchoData));
	_serTestDecEchoData(_buffer, _size, _lidx, _mem, *ToSS);

	return SIDL_STATUS_OK;
}

void serTestEchoFreeClt(struct EchoData* ToSS)
{
	if (ToSS) {
		_serTestFreeEchoData(ToSS);
		serFree(ToSS);
	}
}

static int _serTestEncChar_Foo_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct char_Foo_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->v.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncChar_Koo_ArrayOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct char_Koo_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 25; i1++) {
		HTON_8(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_Bar_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_Bar_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v, _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncOutput(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Output* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncChar_Foo_DynamicOptional(_buffer, _size, _lidx, &p->Foo);
	_serTestEncChar_Koo_ArrayOptional(_buffer, _size, _lidx, &p->Koo);
	_serTestEncInt_Bar_Optional(_buffer, _size, _lidx, &p->Bar);
	HTON_32(&_buffer[*_lidx], p->Zoo.d, _lidx);
	for (size_t i1 = 0; i1 < p->Zoo.d; i1++) {
		HTON_8(&_buffer[*_lidx], p->Zoo.v[i1], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->ZQty, _lidx);
	for (size_t i1 = 0; i1 < p->ZQty; i1++) {
		HTON_8(&_buffer[*_lidx], p->ZArray[i1], _lidx);
	}
	HTON_32(&_buffer[*_lidx], p->Far.d, _lidx);
	for (size_t i1 = 0; i1 < p->Far.d; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->Far.v[i1]);
	}

	return SIDL_STATUS_OK;
}

int serTestTest1EncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Output* out)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncOutput(_buffer, _size, _lidx, out);

	return SIDL_STATUS_OK;
}

static int _serTestDecChar_Foo_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct char_Foo_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(char));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		NTOH_8(p->v.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecChar_Koo_ArrayOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct char_Koo_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 25; i1++) {
		NTOH_8(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_Bar_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_Bar_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestDecOutput(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct Output* p)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestDecChar_Foo_DynamicOptional(_buffer, _size, _lidx, _mem, &p->Foo);
	_serTestDecChar_Koo_ArrayOptional(_buffer, _size, _lidx, &p->Koo);
	_serTestDecInt_Bar_Optional(_buffer, _size, _lidx, &p->Bar);
	NTOH_32(p->Zoo.d, &_buffer[*_lidx], _lidx);
	p->Zoo.v = serMalloc(_mem, p->Zoo.d * sizeof(char));
	for (size_t i1 = 0; i1 < p->Zoo.d; i1++) {
		NTOH_8(p->Zoo.v[i1], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->ZQty, &_buffer[*_lidx], _lidx);
	p->ZArray = serMalloc(_mem, p->ZQty * sizeof(char));
	for (size_t i1 = 0; i1 < p->ZQty; i1++) {
		NTOH_8(p->ZArray[i1], &_buffer[*_lidx], _lidx);
	}
	NTOH_32(p->Far.d, &_buffer[*_lidx], _lidx);
	p->Far.v = serMalloc(_mem, p->Far.d * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < p->Far.d; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->Far.v[i1]);
	}

	return SIDL_STATUS_OK;
}

int serTestTest1DecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Output** out)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*out = serMalloc(_mem, sizeof(struct Output));
	_serTestDecOutput(_buffer, _size, _lidx, _mem, *out);

	return SIDL_STATUS_OK;
}

static void _serTestFreeChar_Foo_DynamicOptional(struct char_Foo_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serTestFreeOutput(struct Output* p)
{
	_serTestFreeChar_Foo_DynamicOptional(&p->Foo);
	if (p->Zoo.v) {
		serFree(p->Zoo.v);
	}
	if (p->ZArray) {
		serFree(p->ZArray);
	}
	if (p->Far.v) {
		for (size_t i1 = 0; i1 < p->Far.d; i1++) {
		}
		serFree(p->Far.v);
	}
}

void serTestTest1FreeSrv(struct Output* out)
{
	if (out) {
		_serTestFreeOutput(out);
		serFree(out);
	}
}

int serTestTest2EncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Output* out)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncOutput(_buffer, _size, _lidx, out);

	return SIDL_STATUS_OK;
}

int serTestTest2DecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Output** out)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*out = serMalloc(_mem, sizeof(struct Output));
	_serTestDecOutput(_buffer, _size, _lidx, _mem, *out);

	return SIDL_STATUS_OK;
}

void serTestTest2FreeClt(struct Output* out)
{
	if (out) {
		_serTestFreeOutput(out);
		serFree(out);
	}
}

static int _serTestEncComplex(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Complex* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->val, _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty2(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty2* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->dummy, _lidx);
	for (size_t i1 = 0; i1 < 3; i1++) {
		HTON_32(&_buffer[*_lidx], p->simple[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 3; i1++) {
		_serTestEncComplex(_buffer, _size, _lidx, &p->complex[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_optional_struct_1_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_optional_struct_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestEncEmpty(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_optional_struct_2_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_optional_struct_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestEncEmpty(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_optional_int_1_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_optional_int_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v, _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_optional_int_2_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_optional_int_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v, _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestEncChar_optional_string_1_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct char_optional_string_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	{
		size_t _tmpLen = (strlen(p->v ? p->v : "")) + 1;
		memcpy(&_buffer[*_lidx], (p->v ? p->v : ""), _tmpLen);
		*_lidx += _tmpLen;
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncChar_optional_string_2_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct char_optional_string_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	{
		size_t _tmpLen = (strlen(p->v ? p->v : "")) + 1;
		memcpy(&_buffer[*_lidx], (p->v ? p->v : ""), _tmpLen);
		*_lidx += _tmpLen;
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_optional_struct_array_1_ArrayOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_optional_struct_array_1_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_optional_struct_array_2_ArrayOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_optional_struct_array_2_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_optional_int_array_1_ArrayOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_optional_int_array_1_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		HTON_32(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_optional_int_array_2_ArrayOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_optional_int_array_2_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		HTON_32(&_buffer[*_lidx], p->v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_dynamic_optional_struct_1_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_dynamic_optional_struct_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncEmpty_dynamic_optional_struct_2_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty_dynamic_optional_struct_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_dynamic_optional_int_1_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_dynamic_optional_int_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		HTON_32(&_buffer[*_lidx], p->v.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncInt_dynamic_optional_int_2_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct int_dynamic_optional_int_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		HTON_32(&_buffer[*_lidx], p->v.v[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_Value(unsigned char* _buffer, size_t _size, size_t* _lidx, const union TestUnion_Value* p, enum TestUnion_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TestUnion_Zero) {
		HTON_32(&_buffer[*_lidx], p->Zero, _lidx);
	}
	if (d == TestUnion_One) {
		HTON_32(&_buffer[*_lidx], p->One, _lidx);
	}
	if (d == TestUnion_Two) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->Two);
	}
	if (d == TestUnion_Three) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->Three);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp = (size_t)p->d;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	_serTestEncTestUnion_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_optional_union_1_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion_optional_union_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestEncTestUnion(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_optional_union_pointer_1_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion_optional_union_pointer_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestEncTestUnion(_buffer, _size, _lidx, p->v);

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_optional_union_2_Optional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion_optional_union_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestEncTestUnion(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_dynamic_optional_union_1_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion_dynamic_optional_union_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestEncTestUnion(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncTestUnion_dynamic_optional_union_2_DynamicOptional(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct TestUnion_dynamic_optional_union_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->d, _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	HTON_32(&_buffer[*_lidx], p->v.d, _lidx);
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestEncTestUnion(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestEncNew(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct New* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_32(&_buffer[*_lidx], p->dynamic_struct.d, _lidx);
	for (size_t i1 = 0; i1 < p->dynamic_struct.d; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &p->dynamic_struct.v[i1]);
	}
	HTON_32(&_buffer[*_lidx], p->dynamic_ints.d, _lidx);
	for (size_t i1 = 0; i1 < p->dynamic_ints.d; i1++) {
		HTON_32(&_buffer[*_lidx], p->dynamic_ints.v[i1], _lidx);
	}
	_serTestEncEmpty_optional_struct_1_Optional(_buffer, _size, _lidx, &p->optional_struct_1);
	_serTestEncEmpty_optional_struct_2_Optional(_buffer, _size, _lidx, &p->optional_struct_2);
	_serTestEncInt_optional_int_1_Optional(_buffer, _size, _lidx, &p->optional_int_1);
	_serTestEncInt_optional_int_2_Optional(_buffer, _size, _lidx, &p->optional_int_2);
	_serTestEncChar_optional_string_1_Optional(_buffer, _size, _lidx, &p->optional_string_1);
	_serTestEncChar_optional_string_2_Optional(_buffer, _size, _lidx, &p->optional_string_2);
	_serTestEncEmpty_optional_struct_array_1_ArrayOptional(_buffer, _size, _lidx, &p->optional_struct_array_1);
	_serTestEncEmpty_optional_struct_array_2_ArrayOptional(_buffer, _size, _lidx, &p->optional_struct_array_2);
	_serTestEncInt_optional_int_array_1_ArrayOptional(_buffer, _size, _lidx, &p->optional_int_array_1);
	_serTestEncInt_optional_int_array_2_ArrayOptional(_buffer, _size, _lidx, &p->optional_int_array_2);
	_serTestEncEmpty_dynamic_optional_struct_1_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_struct_1);
	_serTestEncEmpty_dynamic_optional_struct_2_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_struct_2);
	_serTestEncInt_dynamic_optional_int_1_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_int_1);
	_serTestEncInt_dynamic_optional_int_2_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_int_2);
	_serTestEncTestUnion(_buffer, _size, _lidx, p->union_test_pointer);
	_serTestEncTestUnion(_buffer, _size, _lidx, &p->union_test);
	_serTestEncTestUnion_optional_union_1_Optional(_buffer, _size, _lidx, &p->optional_union_1);
	_serTestEncTestUnion_optional_union_pointer_1_Optional(_buffer, _size, _lidx, &p->optional_union_pointer_1);
	_serTestEncTestUnion_optional_union_2_Optional(_buffer, _size, _lidx, &p->optional_union_2);
	_serTestEncTestUnion_dynamic_optional_union_1_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_union_1);
	_serTestEncTestUnion_dynamic_optional_union_2_DynamicOptional(_buffer, _size, _lidx, &p->dynamic_optional_union_2);

	return SIDL_STATUS_OK;
}

int serTestOtherEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncEmpty(_buffer, _size, _lidx, in1);
	HTON_32(&_buffer[*_lidx], in2, _lidx);
	HTON_32(&_buffer[*_lidx], in3Qty, _lidx);
	for (size_t i1 = 0; i1 < in3Qty; i1++) {
		HTON_8(&_buffer[*_lidx], in3Array[i1], _lidx);
	}
	{
		size_t _tmpLen = (strlen(in4 ? in4 : "")) + 1;
		memcpy(&_buffer[*_lidx], (in4 ? in4 : ""), _tmpLen);
		*_lidx += _tmpLen;
	}
	HTON_8(&_buffer[*_lidx], in5, _lidx);
	HTON_32(&_buffer[*_lidx], in6, _lidx);
	{
		size_t _tmp = (size_t)in7;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	{
		size_t _tmp = (size_t)in8;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	HTON_32(&_buffer[*_lidx], in9Qty, _lidx);
	for (size_t i1 = 0; i1 < in9Qty; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &in9Array[i1]);
	}
	_serTestEncEmpty2(_buffer, _size, _lidx, in10);
	_serTestEncNew(_buffer, _size, _lidx, in11);

	return SIDL_STATUS_OK;
}

static int _serTestDecComplex(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Complex* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->val, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty2(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty2* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->dummy, &_buffer[*_lidx], _lidx);
	for (size_t i1 = 0; i1 < 3; i1++) {
		NTOH_32(p->simple[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 3; i1++) {
		_serTestDecComplex(_buffer, _size, _lidx, &p->complex[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_optional_struct_1_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty_optional_struct_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestDecEmpty(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_optional_struct_2_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty_optional_struct_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestDecEmpty(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_optional_int_1_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_optional_int_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_optional_int_2_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_optional_int_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v, &_buffer[*_lidx], _lidx);

	return SIDL_STATUS_OK;
}

static int _serTestDecChar_optional_string_1_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct char_optional_string_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	{
		size_t _tmpLen = strlen((const char*)&_buffer[*_lidx]) + 1;
		p->v = (char*)serMalloc(_mem, _tmpLen);
		memcpy(p->v, &_buffer[*_lidx], _tmpLen);
		*_lidx += _tmpLen;
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecChar_optional_string_2_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct char_optional_string_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	{
		size_t _tmpLen = strlen((const char*)&_buffer[*_lidx]) + 1;
		p->v = (char*)serMalloc(_mem, _tmpLen);
		memcpy(p->v, &_buffer[*_lidx], _tmpLen);
		*_lidx += _tmpLen;
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_optional_struct_array_1_ArrayOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty_optional_struct_array_1_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_optional_struct_array_2_ArrayOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct Empty_optional_struct_array_2_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_optional_int_array_1_ArrayOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_optional_int_array_1_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		NTOH_32(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_optional_int_array_2_ArrayOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct int_optional_int_array_2_ArrayOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	for (size_t i1 = 0; i1 < 2; i1++) {
		NTOH_32(p->v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_dynamic_optional_struct_1_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct Empty_dynamic_optional_struct_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecEmpty_dynamic_optional_struct_2_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct Empty_dynamic_optional_struct_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_dynamic_optional_int_1_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct int_dynamic_optional_int_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(int));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		NTOH_32(p->v.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecInt_dynamic_optional_int_2_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct int_dynamic_optional_int_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(int));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		NTOH_32(p->v.v[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_Value(const unsigned char* _buffer, size_t _size, size_t* _lidx, union TestUnion_Value* p, enum TestUnion_Sel d)
{
	(void)_size; // TODO: generate boundaries checking

	if (d == TestUnion_Zero) {
		NTOH_32(p->Zero, &_buffer[*_lidx], _lidx);
	}
	if (d == TestUnion_One) {
		NTOH_32(p->One, &_buffer[*_lidx], _lidx);
	}
	if (d == TestUnion_Two) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->Two);
	}
	if (d == TestUnion_Three) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->Three);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TestUnion* p)
{
	(void)_size; // TODO: generate boundaries checking

	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		p->d = (enum TestUnion_Sel)_tmp;
	}
	_serTestDecTestUnion_Value(_buffer, _size, _lidx, &p->v, p->d);

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_optional_union_1_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TestUnion_optional_union_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestDecTestUnion(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_optional_union_pointer_1_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TestUnion_optional_union_pointer_1_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	p->v = serMalloc(_mem, sizeof(struct TestUnion));
	_serTestDecTestUnion(_buffer, _size, _lidx, p->v);

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_optional_union_2_Optional(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct TestUnion_optional_union_2_Optional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	_serTestDecTestUnion(_buffer, _size, _lidx, &p->v);

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_dynamic_optional_union_1_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TestUnion_dynamic_optional_union_1_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct TestUnion));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestDecTestUnion(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecTestUnion_dynamic_optional_union_2_DynamicOptional(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct TestUnion_dynamic_optional_union_2_DynamicOptional* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->d, &_buffer[*_lidx], _lidx);
	if (!p->d) return SIDL_STATUS_OK;
	NTOH_32(p->v.d, &_buffer[*_lidx], _lidx);
	p->v.v = serMalloc(_mem, p->v.d * sizeof(struct TestUnion));
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		_serTestDecTestUnion(_buffer, _size, _lidx, &p->v.v[i1]);
	}

	return SIDL_STATUS_OK;
}

static int _serTestDecNew(const unsigned char* _buffer, size_t _size, size_t* _lidx, serMem_t _mem, struct New* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_32(p->dynamic_struct.d, &_buffer[*_lidx], _lidx);
	p->dynamic_struct.v = serMalloc(_mem, p->dynamic_struct.d * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < p->dynamic_struct.d; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, &p->dynamic_struct.v[i1]);
	}
	NTOH_32(p->dynamic_ints.d, &_buffer[*_lidx], _lidx);
	p->dynamic_ints.v = serMalloc(_mem, p->dynamic_ints.d * sizeof(int));
	for (size_t i1 = 0; i1 < p->dynamic_ints.d; i1++) {
		NTOH_32(p->dynamic_ints.v[i1], &_buffer[*_lidx], _lidx);
	}
	_serTestDecEmpty_optional_struct_1_Optional(_buffer, _size, _lidx, &p->optional_struct_1);
	_serTestDecEmpty_optional_struct_2_Optional(_buffer, _size, _lidx, &p->optional_struct_2);
	_serTestDecInt_optional_int_1_Optional(_buffer, _size, _lidx, &p->optional_int_1);
	_serTestDecInt_optional_int_2_Optional(_buffer, _size, _lidx, &p->optional_int_2);
	_serTestDecChar_optional_string_1_Optional(_buffer, _size, _lidx, _mem, &p->optional_string_1);
	_serTestDecChar_optional_string_2_Optional(_buffer, _size, _lidx, _mem, &p->optional_string_2);
	_serTestDecEmpty_optional_struct_array_1_ArrayOptional(_buffer, _size, _lidx, &p->optional_struct_array_1);
	_serTestDecEmpty_optional_struct_array_2_ArrayOptional(_buffer, _size, _lidx, &p->optional_struct_array_2);
	_serTestDecInt_optional_int_array_1_ArrayOptional(_buffer, _size, _lidx, &p->optional_int_array_1);
	_serTestDecInt_optional_int_array_2_ArrayOptional(_buffer, _size, _lidx, &p->optional_int_array_2);
	_serTestDecEmpty_dynamic_optional_struct_1_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_struct_1);
	_serTestDecEmpty_dynamic_optional_struct_2_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_struct_2);
	_serTestDecInt_dynamic_optional_int_1_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_int_1);
	_serTestDecInt_dynamic_optional_int_2_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_int_2);
	p->union_test_pointer = serMalloc(_mem, sizeof(struct TestUnion));
	_serTestDecTestUnion(_buffer, _size, _lidx, p->union_test_pointer);
	_serTestDecTestUnion(_buffer, _size, _lidx, &p->union_test);
	_serTestDecTestUnion_optional_union_1_Optional(_buffer, _size, _lidx, &p->optional_union_1);
	_serTestDecTestUnion_optional_union_pointer_1_Optional(_buffer, _size, _lidx, _mem, &p->optional_union_pointer_1);
	_serTestDecTestUnion_optional_union_2_Optional(_buffer, _size, _lidx, &p->optional_union_2);
	_serTestDecTestUnion_dynamic_optional_union_1_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_union_1);
	_serTestDecTestUnion_dynamic_optional_union_2_DynamicOptional(_buffer, _size, _lidx, _mem, &p->dynamic_optional_union_2);

	return SIDL_STATUS_OK;
}

int serTestOtherDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Empty** in1, uint32_t* in2, size_t* in3Qty, char** in3Array, char** in4, bool* in5, int* in6, float* in7, SomeEnum* in8, size_t* in9Qty, struct Empty** in9Array, struct Empty2** in10, struct New** in11)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*in1 = serMalloc(_mem, sizeof(struct Empty));
	_serTestDecEmpty(_buffer, _size, _lidx, *in1);
	NTOH_32(*in2, &_buffer[*_lidx], _lidx);
	NTOH_32(*in3Qty, &_buffer[*_lidx], _lidx);
	*in3Array = serMalloc(_mem, *in3Qty * sizeof(char));
	for (size_t i1 = 0; i1 < *in3Qty; i1++) {
		NTOH_8((*in3Array)[i1], &_buffer[*_lidx], _lidx);
	}
	{
		size_t _tmpLen = strlen((const char*)&_buffer[*_lidx]) + 1;
		*in4 = (char*)serMalloc(_mem, _tmpLen);
		memcpy(*in4, &_buffer[*_lidx], _tmpLen);
		*_lidx += _tmpLen;
	}
	NTOH_8(*in5, &_buffer[*_lidx], _lidx);
	NTOH_32(*in6, &_buffer[*_lidx], _lidx);
	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		*in7 = (float)_tmp;
	}
	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		*in8 = (SomeEnum)_tmp;
	}
	NTOH_32(*in9Qty, &_buffer[*_lidx], _lidx);
	*in9Array = serMalloc(_mem, *in9Qty * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < *in9Qty; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, (*&in9Array)[i1]);
	}
	*in10 = serMalloc(_mem, sizeof(struct Empty2));
	_serTestDecEmpty2(_buffer, _size, _lidx, *in10);
	*in11 = serMalloc(_mem, sizeof(struct New));
	_serTestDecNew(_buffer, _size, _lidx, _mem, *in11);

	return SIDL_STATUS_OK;
}

static void _serTestFreeChar_optional_string_1_Optional(struct char_optional_string_1_Optional* p)
{
	if (p->v) {
		serFree(p->v);
	}
}

static void _serTestFreeChar_optional_string_2_Optional(struct char_optional_string_2_Optional* p)
{
	if (p->v) {
		serFree(p->v);
	}
}

static void _serTestFreeEmpty_dynamic_optional_struct_1_DynamicOptional(struct Empty_dynamic_optional_struct_1_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i1 = 0; i1 < p->v.d; i1++) {
		}
		serFree(p->v.v);
	}
}

static void _serTestFreeEmpty_dynamic_optional_struct_2_DynamicOptional(struct Empty_dynamic_optional_struct_2_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i1 = 0; i1 < p->v.d; i1++) {
		}
		serFree(p->v.v);
	}
}

static void _serTestFreeInt_dynamic_optional_int_1_DynamicOptional(struct int_dynamic_optional_int_1_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serTestFreeInt_dynamic_optional_int_2_DynamicOptional(struct int_dynamic_optional_int_2_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		serFree(p->v.v);
	}
}

static void _serTestFreeTestUnion_optional_union_pointer_1_Optional(struct TestUnion_optional_union_pointer_1_Optional* p)
{
	if (!p->d) return;
}

static void _serTestFreeTestUnion_dynamic_optional_union_1_DynamicOptional(struct TestUnion_dynamic_optional_union_1_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i1 = 0; i1 < p->v.d; i1++) {
		}
		serFree(p->v.v);
	}
}

static void _serTestFreeTestUnion_dynamic_optional_union_2_DynamicOptional(struct TestUnion_dynamic_optional_union_2_DynamicOptional* p)
{
	if (!p->d) return;
	if (p->v.v) {
		for (size_t i1 = 0; i1 < p->v.d; i1++) {
		}
		serFree(p->v.v);
	}
}

static void _serTestFreeNew(struct New* p)
{
	if (p->dynamic_struct.v) {
		for (size_t i1 = 0; i1 < p->dynamic_struct.d; i1++) {
		}
		serFree(p->dynamic_struct.v);
	}
	if (p->dynamic_ints.v) {
		serFree(p->dynamic_ints.v);
	}
	_serTestFreeChar_optional_string_1_Optional(&p->optional_string_1);
	_serTestFreeChar_optional_string_2_Optional(&p->optional_string_2);
	_serTestFreeEmpty_dynamic_optional_struct_1_DynamicOptional(&p->dynamic_optional_struct_1);
	_serTestFreeEmpty_dynamic_optional_struct_2_DynamicOptional(&p->dynamic_optional_struct_2);
	_serTestFreeInt_dynamic_optional_int_1_DynamicOptional(&p->dynamic_optional_int_1);
	_serTestFreeInt_dynamic_optional_int_2_DynamicOptional(&p->dynamic_optional_int_2);
	_serTestFreeTestUnion_optional_union_pointer_1_Optional(&p->optional_union_pointer_1);
	_serTestFreeTestUnion_dynamic_optional_union_1_DynamicOptional(&p->dynamic_optional_union_1);
	_serTestFreeTestUnion_dynamic_optional_union_2_DynamicOptional(&p->dynamic_optional_union_2);
}

void serTestOtherFreeSrv(struct Empty* in1, char* in3Array, char* in4, struct Empty* in9Array, size_t in9Qty, struct Empty2* in10, struct New* in11)
{
	if (in1) {
		serFree(in1);
	}
	if (in3Array) {
		serFree(in3Array);
	}
	if (in4) {
		serFree(in4);
	}
	if (in9Array) {
		for (size_t i1 = 0; i1 < in9Qty; i1++) {
		}
		serFree(in9Array);
	}
	if (in10) {
		serFree(in10);
	}
	if (in11) {
		_serTestFreeNew(in11);
		serFree(in11);
	}
}

int serTestOtherEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11)
{
	(void)_size; // TODO: generate boundaries checking

	_serTestEncEmpty(_buffer, _size, _lidx, out1);
	HTON_32(&_buffer[*_lidx], out2, _lidx);
	HTON_32(&_buffer[*_lidx], out3Qty, _lidx);
	for (size_t i1 = 0; i1 < out3Qty; i1++) {
		HTON_8(&_buffer[*_lidx], out3Array[i1], _lidx);
	}
	{
		size_t _tmpLen = (strlen(out4 ? out4 : "")) + 1;
		memcpy(&_buffer[*_lidx], (out4 ? out4 : ""), _tmpLen);
		*_lidx += _tmpLen;
	}
	HTON_8(&_buffer[*_lidx], out5, _lidx);
	HTON_32(&_buffer[*_lidx], out6, _lidx);
	{
		size_t _tmp = (size_t)out7;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	{
		size_t _tmp = (size_t)out8;
		HTON_32(&_buffer[*_lidx], _tmp, _lidx);
	}
	HTON_32(&_buffer[*_lidx], out9Qty, _lidx);
	for (size_t i1 = 0; i1 < out9Qty; i1++) {
		_serTestEncEmpty(_buffer, _size, _lidx, &out9Array[i1]);
	}
	_serTestEncEmpty2(_buffer, _size, _lidx, out10);
	_serTestEncNew(_buffer, _size, _lidx, out11);

	return SIDL_STATUS_OK;
}

int serTestOtherDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Empty** out1, uint32_t* out2, size_t* out3Qty, char** out3Array, char** out4, bool* out5, int* out6, float* out7, SomeEnum* out8, size_t* out9Qty, struct Empty** out9Array, struct Empty2** out10, struct New** out11)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*out1 = serMalloc(_mem, sizeof(struct Empty));
	_serTestDecEmpty(_buffer, _size, _lidx, *out1);
	NTOH_32(*out2, &_buffer[*_lidx], _lidx);
	NTOH_32(*out3Qty, &_buffer[*_lidx], _lidx);
	*out3Array = serMalloc(_mem, *out3Qty * sizeof(char));
	for (size_t i1 = 0; i1 < *out3Qty; i1++) {
		NTOH_8((*out3Array)[i1], &_buffer[*_lidx], _lidx);
	}
	{
		size_t _tmpLen = strlen((const char*)&_buffer[*_lidx]) + 1;
		*out4 = (char*)serMalloc(_mem, _tmpLen);
		memcpy(*out4, &_buffer[*_lidx], _tmpLen);
		*_lidx += _tmpLen;
	}
	NTOH_8(*out5, &_buffer[*_lidx], _lidx);
	NTOH_32(*out6, &_buffer[*_lidx], _lidx);
	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		*out7 = (float)_tmp;
	}
	{
		size_t _tmp;
		NTOH_32(_tmp, &_buffer[*_lidx], _lidx);
		*out8 = (SomeEnum)_tmp;
	}
	NTOH_32(*out9Qty, &_buffer[*_lidx], _lidx);
	*out9Array = serMalloc(_mem, *out9Qty * sizeof(struct Empty));
	for (size_t i1 = 0; i1 < *out9Qty; i1++) {
		_serTestDecEmpty(_buffer, _size, _lidx, (*&out9Array)[i1]);
	}
	*out10 = serMalloc(_mem, sizeof(struct Empty2));
	_serTestDecEmpty2(_buffer, _size, _lidx, *out10);
	*out11 = serMalloc(_mem, sizeof(struct New));
	_serTestDecNew(_buffer, _size, _lidx, _mem, *out11);

	return SIDL_STATUS_OK;
}

void serTestOtherFreeClt(struct Empty* out1, char* out3Array, char* out4, struct Empty* out9Array, size_t out9Qty, struct Empty2* out10, struct New* out11)
{
	if (out1) {
		serFree(out1);
	}
	if (out3Array) {
		serFree(out3Array);
	}
	if (out4) {
		serFree(out4);
	}
	if (out9Array) {
		for (size_t i1 = 0; i1 < out9Qty; i1++) {
		}
		serFree(out9Array);
	}
	if (out10) {
		serFree(out10);
	}
	if (out11) {
		_serTestFreeNew(out11);
		serFree(out11);
	}
}
