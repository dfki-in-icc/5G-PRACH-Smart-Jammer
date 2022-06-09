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
#include "adbg.h"

SIDL_BEGIN_C_INTERFACE

void adbgTestHelloFromSSLogIn(acpCtx_t _ctx, size_t StrQty, const char* StrArray);

void adbgTestHelloToSSLogOut(acpCtx_t _ctx, size_t StrQty, const char* StrArray);

void adbgTestPingLogIn(acpCtx_t _ctx, uint32_t FromSS);

void adbgTestPingLogOut(acpCtx_t _ctx, uint32_t ToSS);

void adbgTestEchoLogIn(acpCtx_t _ctx, const struct EchoData* FromSS);

void adbgTestEchoLogOut(acpCtx_t _ctx, const struct EchoData* ToSS);

void adbgTestTest1LogIn(acpCtx_t _ctx, const struct Output* out);

void adbgTestTest2LogOut(acpCtx_t _ctx, const struct Output* out);

void adbgTestOtherLogIn(acpCtx_t _ctx, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11);

void adbgTestOtherLogOut(acpCtx_t _ctx, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11);

SIDL_END_C_INTERFACE
