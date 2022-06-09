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

SIDL_BEGIN_C_INTERFACE

typedef uint8_t OCTET_STRING_ELEMENT;

typedef uint8_t BIT_STRING_ELEMENT;

struct OCTET_STRING_ELEMENT_OCTET_STRING_Dynamic {
	size_t d;
	OCTET_STRING_ELEMENT* v;
};

typedef struct OCTET_STRING_ELEMENT_OCTET_STRING_Dynamic OCTET_STRING;

struct BIT_STRING_ELEMENT_BIT_STRING_Dynamic {
	size_t d;
	BIT_STRING_ELEMENT* v;
};

typedef struct BIT_STRING_ELEMENT_BIT_STRING_Dynamic BIT_STRING;

struct int32_t_PREGEN_RECORD_OF_INTEGER_Dynamic {
	size_t d;
	int32_t* v;
};

typedef struct int32_t_PREGEN_RECORD_OF_INTEGER_Dynamic PREGEN_RECORD_OF_INTEGER;

SIDL_END_C_INTERFACE
