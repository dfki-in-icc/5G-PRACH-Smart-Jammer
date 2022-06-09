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

#include "SidlCompiler.h"
#include "SidlBase.h"

SIDL_BEGIN_C_INTERFACE

typedef bool Null_Type;

typedef bool Dummy_Type;

typedef BIT_STRING_ELEMENT B1_Type[1];

typedef BIT_STRING_ELEMENT B2_Type[2];

typedef BIT_STRING_ELEMENT B3_Type[3];

typedef BIT_STRING_ELEMENT B4_Type[4];

typedef BIT_STRING_ELEMENT B5_Type[5];

typedef BIT_STRING_ELEMENT B6_Type[6];

typedef BIT_STRING_ELEMENT B7_Type[7];

typedef BIT_STRING_ELEMENT B8_Type[8];

typedef BIT_STRING_ELEMENT B10_Type[10];

typedef BIT_STRING_ELEMENT B11_Type[11];

typedef BIT_STRING_ELEMENT B12_Type[12];

typedef BIT_STRING_ELEMENT B15_Type[15];

typedef BIT_STRING_ELEMENT B16_Type[16];

typedef BIT_STRING_ELEMENT B18_Type[18];

typedef BIT_STRING_ELEMENT B32_Type[32];

typedef BIT_STRING_ELEMENT B128_Type[128];

struct BIT_STRING_ELEMENT_B7_15_Type_Dynamic {
	size_t d;
	BIT_STRING_ELEMENT* v;
};

typedef struct BIT_STRING_ELEMENT_B7_15_Type_Dynamic B7_15_Type;

typedef OCTET_STRING_ELEMENT O1_Type[1];

typedef OCTET_STRING_ELEMENT O4_Type[4];

SIDL_END_C_INTERFACE
