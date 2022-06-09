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

#include "acpSysVT.h"
#include "acpCtx.h"
#include "acpProto.h"
#include "acpMsgIds.h"
#include "serSysVT.h"

int acpSysVTEnquireTimingUpdEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct VirtualTimeInfo_Type* TimingInfoToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serSysVTEnquireTimingUpdEncSrv(_buffer, *_size, &_lidx, TimingInfoToSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_SysVTEnquireTimingUpd, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpSysVTEnquireTimingUpdDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct VirtualTimeInfo_Type** TimingInfoToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serSysVTEnquireTimingUpdDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, TimingInfoToSS);
}

void acpSysVTEnquireTimingUpdFreeClt(struct VirtualTimeInfo_Type* TimingInfoToSS)
{
	serSysVTEnquireTimingUpdFreeClt(TimingInfoToSS);
}

int acpSysVTEnquireTimingAckEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct VirtualTimeInfo_Type* TimingInfoFromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serSysVTEnquireTimingAckEncClt(_buffer, *_size, &_lidx, TimingInfoFromSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_SysVTEnquireTimingAck, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpSysVTEnquireTimingAckDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct VirtualTimeInfo_Type** TimingInfoFromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serSysVTEnquireTimingAckDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, TimingInfoFromSS);
}

void acpSysVTEnquireTimingAckFreeSrv(struct VirtualTimeInfo_Type* TimingInfoFromSS)
{
	serSysVTEnquireTimingAckFreeSrv(TimingInfoFromSS);
}
