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

#include "acpIdMap.h"


const struct acpIdMapItf acpItfMap[] = {
	{ "Test", 1, 0, 7 },
	{ "Sys", 2, 7, 1 },
	{ "SysVT", 3, 8, 2 },
	{ "Srb", 4, 10, 2 },
	{ "Vng", 5, 12, 1 },
	{ "Drb", 6, 13, 2 },
	{ "Handshake", 7, 15, 2 },
};

const unsigned int acpItfMapSize = sizeof(acpItfMap) / sizeof(acpItfMap[0]);

// local_id (second field) has the following format:
// 0x9004XXYY,
// where XX is the interface number (from acpItfMap second field),
// and YY is the number of service (sequence of 00..NN) in the interface.
struct acpIdMapService acpIdMap[] = {
	// Test part
	{ "TestHelloFromSS", 0x90040100, (unsigned int)-1, 1 },
	{ "TestHelloToSS", 0x90040101, (unsigned int)-1, 0 },
	{ "TestPing", 0x90040102, (unsigned int)-1, 2 },
	{ "TestEcho", 0x90040103, (unsigned int)-1, 2 },
	{ "TestTest1", 0x90040104, (unsigned int)-1, 2 },
	{ "TestTest2", 0x90040105, (unsigned int)-1, 2 },
	{ "TestOther", 0x90040106, (unsigned int)-1, 2 },

	// Sys part
	{ "SysProcess", 0x90040200, (unsigned int)-1, 2 },

	// SysVT (internal) part
	{ "SysVTEnquireTimingAck", 0x90040300, (unsigned int)-1, 1 },
	{ "SysVTEnquireTimingUpd", 0x90040301, (unsigned int)-1, 0 },

	// SysSrb part
	{ "SysSrbProcessFromSS", 0x90040400, (unsigned int)-1, 1 },
	{ "SysSrbProcessToSS", 0x90040401, (unsigned int)-1, 0 },

	// Vng part
	{ "VngProcess", 0x90040500, (unsigned int)-1, 2 },

	// Drb part
	{ "DrbProcessFromSS", 0x90040600, (unsigned int)-1, 1 },
	{ "DrbProcessToSS", 0x90040601, (unsigned int)-1, 0 },

	// Handshake (internale) part
	{ "HandshakeHandleFromSS", 0x90040700, (unsigned int)-1, 1 },
	{ "HandshakeHandleToSS", 0x90040701, (unsigned int)-1, 0 }
};

const unsigned int acpIdMapSize = sizeof(acpIdMap) / sizeof(acpIdMap[0]);
