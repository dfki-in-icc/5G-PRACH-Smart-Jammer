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

// Internal includes
#include "acp.h"
#include "acpMsgIds.h"

SIDL_BEGIN_C_INTERFACE

void adbgMsgLogInArgs(acpCtx_t ctx, enum acpMsgLocalId id, size_t size, const unsigned char* buffer);
void adbgMsgLogOutArgs(acpCtx_t ctx, enum acpMsgLocalId id, size_t size, const unsigned char* buffer);

SIDL_END_C_INTERFACE
