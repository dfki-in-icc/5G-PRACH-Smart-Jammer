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

#pragma once

#include "SIDL_Test.h"
#include "acp.h"

SIDL_BEGIN_C_INTERFACE

int acpTestHelloFromSSEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, size_t StrQty, const char* StrArray);

int acpTestHelloFromSSDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, size_t* StrQty, char** StrArray);

void acpTestHelloFromSSFreeSrv(char* StrArray);

int acpTestHelloToSSEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, size_t StrQty, const char* StrArray);

int acpTestHelloToSSDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, size_t* StrQty, char** StrArray);

void acpTestHelloToSSFreeClt(char* StrArray);

int acpTestPingEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, uint32_t FromSS);

int acpTestPingDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, uint32_t* FromSS);

int acpTestPingEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, uint32_t ToSS);

int acpTestPingDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, uint32_t* ToSS);

int acpTestEchoEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EchoData* FromSS);

int acpTestEchoDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EchoData** FromSS);

void acpTestEchoFreeSrv(struct EchoData* FromSS);

int acpTestEchoEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct EchoData* ToSS);

int acpTestEchoDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct EchoData** ToSS);

void acpTestEchoFreeClt(struct EchoData* ToSS);

int acpTestTest1EncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Output* out);

int acpTestTest1DecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Output** out);

void acpTestTest1FreeSrv(struct Output* out);

int acpTestTest2EncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Output* out);

int acpTestTest2DecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Output** out);

void acpTestTest2FreeClt(struct Output* out);

int acpTestOtherEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11);

int acpTestOtherDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Empty** in1, uint32_t* in2, size_t* in3Qty, char** in3Array, char** in4, bool* in5, int* in6, float* in7, SomeEnum* in8, size_t* in9Qty, struct Empty** in9Array, struct Empty2** in10, struct New** in11);

void acpTestOtherFreeSrv(struct Empty* in1, char* in3Array, char* in4, struct Empty* in9Array, size_t in9Qty, struct Empty2* in10, struct New* in11);

int acpTestOtherEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11);

int acpTestOtherDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct Empty** out1, uint32_t* out2, size_t* out3Qty, char** out3Array, char** out4, bool* out5, int* out6, float* out7, SomeEnum* out8, size_t* out9Qty, struct Empty** out9Array, struct Empty2** out10, struct New** out11);

void acpTestOtherFreeClt(struct Empty* out1, char* out3Array, char* out4, struct Empty* out9Array, size_t out9Qty, struct Empty2* out10, struct New* out11);

SIDL_END_C_INTERFACE
