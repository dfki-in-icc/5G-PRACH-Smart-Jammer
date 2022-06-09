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

#include "acpVng.h"
#include "acpCtx.h"
#include "acpProto.h"
#include "acpMsgIds.h"
#include "serVng.h"

int acpVngProcessEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EUTRA_VNG_CTRL_REQ* FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serVngProcessEncClt(_buffer, *_size, &_lidx, FromSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_VngProcess, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpVngProcessDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EUTRA_VNG_CTRL_REQ** FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serVngProcessDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, FromSS);
}

void acpVngProcessFreeSrv(struct EUTRA_VNG_CTRL_REQ* FromSS)
{
	serVngProcessFreeSrv(FromSS);
}

int acpVngProcessEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EUTRA_VNG_CTRL_CNF* ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serVngProcessEncSrv(_buffer, *_size, &_lidx, ToSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_VngProcess, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpVngProcessDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EUTRA_VNG_CTRL_CNF** ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serVngProcessDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, ToSS);
}

void acpVngProcessFreeClt(struct EUTRA_VNG_CTRL_CNF* ToSS)
{
	serVngProcessFreeClt(ToSS);
}
