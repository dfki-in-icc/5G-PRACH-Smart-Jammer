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

#include "SIDL_EUTRA_VNG_PORT.h"
#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

int serVngProcessEncClt(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_REQ* FromSS);

int serVngProcessDecSrv(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_VNG_CTRL_REQ** FromSS);

void serVngProcessFreeSrv(struct EUTRA_VNG_CTRL_REQ* FromSS);

int serVngProcessEncSrv(unsigned char* _buffer, size_t _size, size_t* _lidx, const struct EUTRA_VNG_CTRL_CNF* ToSS);

int serVngProcessDecClt(const unsigned char* _buffer, size_t _size, unsigned char* _arena, size_t _aSize, struct EUTRA_VNG_CTRL_CNF** ToSS);

void serVngProcessFreeClt(struct EUTRA_VNG_CTRL_CNF* ToSS);

SIDL_END_C_INTERFACE
