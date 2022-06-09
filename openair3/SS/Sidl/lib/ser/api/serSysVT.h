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
#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

int serSysVTEnquireTimingUpdEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct VirtualTimeInfo_Type* TimingInfoToSS);

int serSysVTEnquireTimingUpdDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct VirtualTimeInfo_Type** TimingInfoToSS);

void serSysVTEnquireTimingUpdFreeClt(struct VirtualTimeInfo_Type* TimingInfoToSS);

int serSysVTEnquireTimingAckEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct VirtualTimeInfo_Type* TimingInfoFromSS);

int serSysVTEnquireTimingAckDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct VirtualTimeInfo_Type** TimingInfoFromSS);

void serSysVTEnquireTimingAckFreeSrv(struct VirtualTimeInfo_Type* TimingInfoFromSS);

SIDL_END_C_INTERFACE
