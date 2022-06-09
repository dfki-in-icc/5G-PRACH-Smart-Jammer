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

#include "SIDL_VIRTUAL_TIME_PORT.h"
#include "acp.h"

SIDL_BEGIN_C_INTERFACE

int acpSysVTEnquireTimingUpdEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct VirtualTimeInfo_Type* TimingInfoToSS);

int acpSysVTEnquireTimingUpdDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct VirtualTimeInfo_Type** TimingInfoToSS);

void acpSysVTEnquireTimingUpdFreeClt(struct VirtualTimeInfo_Type* TimingInfoToSS);

int acpSysVTEnquireTimingAckEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct VirtualTimeInfo_Type* TimingInfoFromSS);

int acpSysVTEnquireTimingAckDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct VirtualTimeInfo_Type** TimingInfoFromSS);

void acpSysVTEnquireTimingAckFreeSrv(struct VirtualTimeInfo_Type* TimingInfoFromSS);

SIDL_END_C_INTERFACE
