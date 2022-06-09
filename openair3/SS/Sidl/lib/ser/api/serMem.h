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

// System includes
#include <stdbool.h>
#include <stdint.h>

// Internal includes
#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

/** Defines magic value for memory context.*/
#define SER_MEM_MAGIC 0x0AC90AC9

/** Defines magic value for dynamic memory context.*/
#define SER_MEM_DYNAMIC_MAGIC 0x1AC91AC9

/** Defines memory context. */
struct serMemCtx {
	uint32_t magic;
	size_t size;
	size_t index;
};

/** Memory context handle. */
typedef struct serMemCtx* serMem_t;

/** Initialize memory context. */
serMem_t serMemInit(unsigned char* arena, unsigned int aSize);

/** Allocates size bytes and returns a pointer to the allocated memory. */
void* serMalloc(serMem_t mem, size_t size);

/** frees the memory space pointed to by ptr, which must have been returned
 * by a previous call serMalloc.*/
void serFree(void* ptr);

/** Defines preinitialized memory context for dynamic memory allocation. */
extern serMem_t serMemDyn;

/** Defines convenient way to allocate memory using dynamic allocation. */
#define SER_DYN_ALLOC(sIZE) serMalloc(serMemDyn, (sIZE))

SIDL_END_C_INTERFACE
