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
#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

int serTestHelloFromSSEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, size_t StrQty, const char* StrArray);

int serTestHelloFromSSDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, size_t* StrQty, char** StrArray);

void serTestHelloFromSSFreeSrv(char* StrArray);

int serTestHelloToSSEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, size_t StrQty, const char* StrArray);

int serTestHelloToSSDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, size_t* StrQty, char** StrArray);

void serTestHelloToSSFreeClt(char* StrArray);

int serTestPingEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, uint32_t FromSS);

int serTestPingDecSrv(const unsigned char* _buffer, size_t _size, uint32_t* FromSS);

int serTestPingEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, uint32_t ToSS);

int serTestPingDecClt(const unsigned char* _buffer, size_t _size, uint32_t* ToSS);

int serTestEchoEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EchoData* FromSS);

int serTestEchoDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EchoData** FromSS);

void serTestEchoFreeSrv(struct EchoData* FromSS);

int serTestEchoEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EchoData* ToSS);

int serTestEchoDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EchoData** ToSS);

void serTestEchoFreeClt(struct EchoData* ToSS);

int serTestTest1EncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Output* out);

int serTestTest1DecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Output** out);

void serTestTest1FreeSrv(struct Output* out);

int serTestTest2EncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Output* out);

int serTestTest2DecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Output** out);

void serTestTest2FreeClt(struct Output* out);

int serTestOtherEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11);

int serTestOtherDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Empty** in1, uint32_t* in2, size_t* in3Qty, char** in3Array, char** in4, bool* in5, int* in6, float* in7, SomeEnum* in8, size_t* in9Qty, struct Empty** in9Array, struct Empty2** in10, struct New** in11);

void serTestOtherFreeSrv(struct Empty* in1, char* in3Array, char* in4, struct Empty* in9Array, size_t in9Qty, struct Empty2* in10, struct New* in11);

int serTestOtherEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11);

int serTestOtherDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct Empty** out1, uint32_t* out2, size_t* out3Qty, char** out3Array, char** out4, bool* out5, int* out6, float* out7, SomeEnum* out8, size_t* out9Qty, struct Empty** out9Array, struct Empty2** out10, struct New** out11);

void serTestOtherFreeClt(struct Empty* out1, char* out3Array, char* out4, struct Empty* out9Array, size_t out9Qty, struct Empty2* out10, struct New* out11);

SIDL_END_C_INTERFACE
