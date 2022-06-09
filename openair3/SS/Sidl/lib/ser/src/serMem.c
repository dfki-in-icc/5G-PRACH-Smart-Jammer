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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Internal includes
#include "serMem.h"

// Enable capability for dynamic memory allocation
#define SER_MEM_SUPPORT_DYNAMIC

// Define ser memory header size
#define SER_MEM_HEADER_SIZE 4

// If SER_MEM_SUPPORT_DYNAMIC is defined serMem will allow dynamic memory allocation,
// otherwise ONLY arena allocation is supported (no malloc/free will be ever called)
#ifdef SER_MEM_SUPPORT_DYNAMIC
static struct serMemCtx serMemDynCtx = { SER_MEM_DYNAMIC_MAGIC, 0, 0 };

/** Defines preinitialized memory context for dynamic memory allocation. */
serMem_t serMemDyn = &serMemDynCtx;
#endif

serMem_t serMemInit(unsigned char* arena, unsigned int aSize)
{
	if (!arena && (aSize == 0)) {
#ifdef SER_MEM_SUPPORT_DYNAMIC
		return serMemDyn;
#else
		SIDL_ASSERT(0);
#endif
	}

	if (!arena || (aSize < sizeof(struct serMemCtx))) {
		return NULL;
	}

	serMem_t mem = (serMem_t)arena;
	mem->magic = SER_MEM_MAGIC;
	mem->size = aSize;
	mem->index = sizeof(struct serMemCtx);

	return mem;
}

void* serMalloc(serMem_t mem, size_t size)
{
	SIDL_ASSERT(mem);
	void* ptr = NULL;

#ifdef SER_MEM_SUPPORT_DYNAMIC
	if (mem->magic == SER_MEM_DYNAMIC_MAGIC) {
		// add ser memory header,
		// since serFree dereferences the pointer to check for magic
		unsigned char* buf = (unsigned char*)malloc(size + SER_MEM_HEADER_SIZE);
		if (buf) {
			memset(buf, 0, SER_MEM_HEADER_SIZE);
			ptr = &buf[SER_MEM_HEADER_SIZE];
		}
		return ptr;
	}
#endif
	if (mem->magic == SER_MEM_MAGIC) {
		// check buffer alignment
		mem->index = (mem->index + 3) & (~((int)0x03));

		// +4 to store header to retrieve mem context with serFree
		if ((mem->index + size + SER_MEM_HEADER_SIZE) <= mem->size) {
			unsigned char* arena = (unsigned char*)mem;
			unsigned int offset = mem->index + SER_MEM_HEADER_SIZE;
			ptr = &arena[offset];
			arena[mem->index] = 0x0A;
			arena[mem->index + 1] = 0xC9;
			arena[mem->index + 2] = (offset >> 8) & 0xFF;
			arena[mem->index + 3] = offset & 0xFF;
			mem->index += (size + SER_MEM_HEADER_SIZE);
		}

		return ptr;
	} else {
		SIDL_ASSERT(0);
	}

	return ptr;
}

void serFree(void* ptr)
{
	SIDL_ASSERT(ptr);

	// get pointer to the allocated data header
	unsigned char* origin = (unsigned char*)ptr - SER_MEM_HEADER_SIZE;
	if ((origin[0] == 0x0A) && (origin[1] == 0xC9)) {
		unsigned int offset = (unsigned int)((origin[2] << 8) | origin[3]);
		// get origin pointer to the arena
		origin = (unsigned char*)ptr - offset;

		serMem_t mem = (serMem_t)origin;
		if (mem && (mem->magic == SER_MEM_MAGIC)) {
			return;
		}
	}
#ifdef SER_MEM_SUPPORT_DYNAMIC
	// there is ser mem header, see serMalloc comment
	free((unsigned char*)ptr - SER_MEM_HEADER_SIZE);
#else
	SIDL_ASSERT(0);
#endif
}
