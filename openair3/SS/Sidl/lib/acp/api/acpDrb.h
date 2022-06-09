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

#include "SIDL_EUTRA_DRB_PORT.h"
#include "acp.h"

SIDL_BEGIN_C_INTERFACE

int acpDrbProcessFromSSEncClt(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct DRB_COMMON_REQ* FromSS);

int acpDrbProcessFromSSDecSrv(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct DRB_COMMON_REQ** FromSS);

void acpDrbProcessFromSSFreeSrv(struct DRB_COMMON_REQ* FromSS);

int acpDrbProcessToSSEncSrv(acpCtx_t _ctx, unsigned char* _buffer, size_t* _size, const struct DRB_COMMON_IND* ToSS);

int acpDrbProcessToSSDecClt(acpCtx_t _ctx, const unsigned char* _buffer, size_t _size, struct DRB_COMMON_IND** ToSS);

void acpDrbProcessToSSFreeClt(struct DRB_COMMON_IND* ToSS);

SIDL_END_C_INTERFACE
