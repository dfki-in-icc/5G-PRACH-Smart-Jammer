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

/** Defines ACP Context. */
struct acpIdMapService {
	/** Service name. */
	const char* name;
	/** Service static local id. */
	unsigned int local_id;
	/** Service remote id. */
	unsigned int remote_id;
	/** Service kind (0 - NTF, 1 - ONEWAY, 2 - CMD). */
	int kind;
};

struct acpIdMapItf {
	/** SIDL interface name. */
	const char* name;
	/** interface id. */
	int id;
	/** Start index. */
	int startIndex;
	/** Service quantity. */
	int servicesQty;
};

/** Interface IDs mapping. */
extern const struct acpIdMapItf acpItfMap[];
extern const unsigned int acpItfMapSize;

/** Service IDs mapping. */
extern struct acpIdMapService acpIdMap[];
extern const unsigned int acpIdMapSize;

SIDL_END_C_INTERFACE
