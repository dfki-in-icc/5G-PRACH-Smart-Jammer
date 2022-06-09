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

#include "acpTest.h"
#include "acpCtx.h"
#include "acpProto.h"
#include "acpMsgIds.h"
#include "serTest.h"

int acpTestHelloFromSSEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, size_t StrQty, const char* StrArray)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestHelloFromSSEncClt(_buffer, *_size, &_lidx, StrQty, StrArray);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestHelloFromSS, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestHelloFromSSDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, size_t* StrQty, char** StrArray)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestHelloFromSSDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, StrQty, StrArray);
}

void acpTestHelloFromSSFreeSrv(char* StrArray)
{
	serTestHelloFromSSFreeSrv(StrArray);
}

int acpTestHelloToSSEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, size_t StrQty, const char* StrArray)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestHelloToSSEncSrv(_buffer, *_size, &_lidx, StrQty, StrArray);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestHelloToSS, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestHelloToSSDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, size_t* StrQty, char** StrArray)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestHelloToSSDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, StrQty, StrArray);
}

void acpTestHelloToSSFreeClt(char* StrArray)
{
	serTestHelloToSSFreeClt(StrArray);
}

int acpTestPingEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, uint32_t FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestPingEncClt(_buffer, *_size, &_lidx, FromSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestPing, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestPingDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, uint32_t* FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestPingDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, FromSS);
}

int acpTestPingEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, uint32_t ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestPingEncSrv(_buffer, *_size, &_lidx, ToSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestPing, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestPingDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, uint32_t* ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestPingDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ToSS);
}

int acpTestEchoEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EchoData* FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestEchoEncClt(_buffer, *_size, &_lidx, FromSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestEcho, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestEchoDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EchoData** FromSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestEchoDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, FromSS);
}

void acpTestEchoFreeSrv(struct EchoData* FromSS)
{
	serTestEchoFreeSrv(FromSS);
}

int acpTestEchoEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EchoData* ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestEchoEncSrv(_buffer, *_size, &_lidx, ToSS);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestEcho, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestEchoDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EchoData** ToSS)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestEchoDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, ToSS);
}

void acpTestEchoFreeClt(struct EchoData* ToSS)
{
	serTestEchoFreeClt(ToSS);
}

int acpTestTest1EncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Output* out)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestTest1EncClt(_buffer, *_size, &_lidx, out);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestTest1, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestTest1DecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Output** out)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestTest1DecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, out);
}

void acpTestTest1FreeSrv(struct Output* out)
{
	serTestTest1FreeSrv(out);
}

int acpTestTest2EncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Output* out)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestTest2EncSrv(_buffer, *_size, &_lidx, out);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestTest2, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestTest2DecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Output** out)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestTest2DecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, out);
}

void acpTestTest2FreeClt(struct Output* out)
{
	serTestTest2FreeClt(out);
}

int acpTestOtherEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestOtherEncClt(_buffer, *_size, &_lidx, in1, in2, in3Qty, in3Array, in4, in5, in6, in7, in8, in9Qty, in9Array, in10, in11);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestOther, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestOtherDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Empty** in1, uint32_t* in2, size_t* in3Qty, char** in3Array, char** in4, bool* in5, int* in6, float* in7, SomeEnum* in8, size_t* in9Qty, struct Empty** in9Array, struct Empty2** in10, struct New** in11)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestOtherDecSrv(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, in1, in2, in3Qty, in3Array, in4, in5, in6, in7, in8, in9Qty, in9Array, in10, in11);
}

void acpTestOtherFreeSrv(struct Empty* in1, char* in3Array, char* in4, struct Empty* in9Array, size_t in9Qty, struct Empty2* in10, struct New* in11)
{
	serTestOtherFreeSrv(in1, in3Array, in4, in9Array, in9Qty, in10, in11);
}

int acpTestOtherEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	size_t _lidx = ACP_HEADER_SIZE;
	int _ret = serTestOtherEncSrv(_buffer, *_size, &_lidx, out1, out2, out3Qty, out3Array, out4, out5, out6, out7, out8, out9Qty, out9Array, out10, out11);
	if (_ret == SIDL_STATUS_OK) {
		acpBuildHeader(_ctx, ACP_LID_TestOther, _lidx, _buffer);
	}
	*_size = _lidx;
	return _ret;
}

int acpTestOtherDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Empty** out1, uint32_t* out2, size_t* out3Qty, char** out3Array, char** out4, bool* out5, int* out6, float* out7, SomeEnum* out8, size_t* out9Qty, struct Empty** out9Array, struct Empty2** out10, struct New** out11)
{
	if (!acpCtxIsValid(_ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return serTestOtherDecClt(_buffer + ACP_HEADER_SIZE, _size - ACP_HEADER_SIZE, ACP_CTX_CAST(_ctx)->arena, ACP_CTX_CAST(_ctx)->aSize, out1, out2, out3Qty, out3Array, out4, out5, out6, out7, out8, out9Qty, out9Array, out10, out11);
}

void acpTestOtherFreeClt(struct Empty* out1, char* out3Array, char* out4, struct Empty* out9Array, size_t out9Qty, struct Empty2* out10, struct New* out11)
{
	serTestOtherFreeClt(out1, out3Array, out4, out9Array, out9Qty, out10, out11);
}
