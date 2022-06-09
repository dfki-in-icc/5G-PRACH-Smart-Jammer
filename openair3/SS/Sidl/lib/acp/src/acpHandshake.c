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

#include "acpHandshake.h"
#include "acpCtx.h"
#include "acpProto.h"
#include "acpMsgIds.h"
#include "serHandshake.h"

int acpHandshakeHandleFromSSEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct AcpHandshake_Type* fromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serHandshakeHandleFromSSEncClt(_buffer, *_size, &_lidx, fromSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_HandshakeHandleFromSS, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpHandshakeHandleFromSSDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct AcpHandshake_Type** fromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serHandshakeHandleFromSSDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, fromSS);
}

void acpHandshakeHandleFromSSFreeSrv(struct AcpHandshake_Type* fromSS)
{
	serHandshakeHandleFromSSFreeSrv(fromSS);
}

int acpHandshakeHandleToSSEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct AcpHandshakeRsp_Type* toSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serHandshakeHandleToSSEncSrv(_buffer, *_size, &_lidx, toSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_HandshakeHandleToSS, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpHandshakeHandleToSSDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct AcpHandshakeRsp_Type** toSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serHandshakeHandleToSSDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, toSS);
}

void acpHandshakeHandleToSSFreeClt(struct AcpHandshakeRsp_Type* toSS)
{
	serHandshakeHandleToSSFreeClt(toSS);
}
