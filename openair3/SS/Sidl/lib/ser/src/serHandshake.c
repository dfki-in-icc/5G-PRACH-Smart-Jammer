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
#include "serHandshake.h"
#include "serMem.h"
#include "serUtils.h"

static int _serHandshakeEncAcpHandshake_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct AcpHandshake_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 32; i1++) {
		HTON_8(&_buffer[*_lidx], p->acpVersion[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 64; i1++) {
		HTON_8(&_buffer[*_lidx], p->acpVerCksm[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serHandshakeHandleFromSSEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct AcpHandshake_Type* fromSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serHandshakeEncAcpHandshake_Type(_buffer, _size, _lidx, fromSS);

	return SIDL_STATUS_OK;
}

static int _serHandshakeDecAcpHandshake_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct AcpHandshake_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	for (size_t i1 = 0; i1 < 32; i1++) {
		NTOH_8(p->acpVersion[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 64; i1++) {
		NTOH_8(p->acpVerCksm[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serHandshakeHandleFromSSDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct AcpHandshake_Type** fromSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*fromSS = serMalloc(_mem, sizeof(struct AcpHandshake_Type));
	_serHandshakeDecAcpHandshake_Type(_buffer, _size, _lidx, *fromSS);

	return SIDL_STATUS_OK;
}

void serHandshakeHandleFromSSFreeSrv(struct AcpHandshake_Type* fromSS)
{
	if (fromSS) {
		serFree(fromSS);
	}
}

static int _serHandshakeEncAcpHandshakeRsp_Type(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct AcpHandshakeRsp_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	HTON_8(&_buffer[*_lidx], p->acpVersionChk, _lidx);
	for (size_t i1 = 0; i1 < 32; i1++) {
		HTON_8(&_buffer[*_lidx], p->acpVersion[i1], _lidx);
	}
	for (size_t i1 = 0; i1 < 64; i1++) {
		HTON_8(&_buffer[*_lidx], p->acpVerCksm[i1], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serHandshakeHandleToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct AcpHandshakeRsp_Type* toSS)
{
	(void)_size; // TODO: generate boundaries checking

	_serHandshakeEncAcpHandshakeRsp_Type(_buffer, _size, _lidx, toSS);

	return SIDL_STATUS_OK;
}

static int _serHandshakeDecAcpHandshakeRsp_Type(const unsigned char* _buffer, size_t _size, size_t* _lidx, struct AcpHandshakeRsp_Type* p)
{
	(void)_size; // TODO: generate boundaries checking

	NTOH_8(p->acpVersionChk, &_buffer[*_lidx], _lidx);
	for (size_t i1 = 0; i1 < 32; i1++) {
		NTOH_8(p->acpVersion[i1], &_buffer[*_lidx], _lidx);
	}
	for (size_t i1 = 0; i1 < 64; i1++) {
		NTOH_8(p->acpVerCksm[i1], &_buffer[*_lidx], _lidx);
	}

	return SIDL_STATUS_OK;
}

int serHandshakeHandleToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct AcpHandshakeRsp_Type** toSS)
{
	(void)_size; // TODO: generate boundaries checking

	serMem_t _mem = serMemInit(_arena, _aSize);

	size_t __lidx = 0;
	size_t* _lidx = &__lidx;

	*toSS = serMalloc(_mem, sizeof(struct AcpHandshakeRsp_Type));
	_serHandshakeDecAcpHandshakeRsp_Type(_buffer, _size, _lidx, *toSS);

	return SIDL_STATUS_OK;
}

void serHandshakeHandleToSSFreeClt(struct AcpHandshakeRsp_Type* toSS)
{
	if (toSS) {
		serFree(toSS);
	}
}
