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

/** Allocates size bytes and returns a pointer to the allocated memory.
 *
 * @param[in]  size	Buffer size to allocate
 * @return   pointer to the allocation buffer or NULL on failure
 */
void* acpMalloc(size_t size);

/** Frees the memory space pointed to by ptr, which must have been returned
 * by a previous call acpMalloc.
 *
 * @param[in]  ptr Pointer to the allocated buffer
 */
void acpFree(void* ptr);

SIDL_END_C_INTERFACE
