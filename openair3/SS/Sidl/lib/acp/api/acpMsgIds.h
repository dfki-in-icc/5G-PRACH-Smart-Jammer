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
#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

/** Defines message local IDs. */
enum acpMsgLocalId {
	ACP_LID_TestHelloFromSS = 0x90040100,
	ACP_LID_TestHelloToSS = 0x90040101,
	ACP_LID_TestPing = 0x90040102,
	ACP_LID_TestEcho = 0x90040103,
	ACP_LID_TestTest1 = 0x90040104,
	ACP_LID_TestTest2 = 0x90040105,
	ACP_LID_TestOther = 0x90040106,
	ACP_LID_SysProcess = 0x90040200,
	ACP_LID_SysVTEnquireTimingAck = 0x90040300,
	ACP_LID_SysVTEnquireTimingUpd = 0x90040301,
	ACP_LID_SysSrbProcessFromSS = 0x90040400,
	ACP_LID_SysSrbProcessToSS = 0x90040401,
	ACP_LID_VngProcess = 0x90040500,
	ACP_LID_DrbProcessFromSS = 0x90040600,
	ACP_LID_DrbProcessToSS = 0x90040601,
	ACP_LID_HandshakeHandleFromSS = 0x90040700,
	ACP_LID_HandshakeHandleToSS = 0x90040701,
};

SIDL_END_C_INTERFACE
