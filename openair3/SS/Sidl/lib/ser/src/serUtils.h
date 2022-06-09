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

// Macro definitions

#define HTON_8(dst, src, idx) \
	{                         \
		*dst = src;           \
		*idx += 1;            \
	}

#define NTOH_8(dst, src, idx)           \
	{                                   \
		dst = ((unsigned char*)src)[0]; \
		*idx += 1;                      \
	}

#define HTON_16(dst, src, idx)      \
	{                               \
		*(dst) = (src >> 8) & 0xFF; \
		*(dst + 1) = (src)&0xFF;    \
		*idx += 2;                  \
	}

#define NTOH_16(dst, src, idx)                   \
	{                                            \
		dst = ((((unsigned char*)src)[0] << 8) | \
			   ((unsigned char*)src)[1]);        \
		*idx += 2;                               \
	}

#define HTON_32(dst, src, idx)           \
	{                                    \
		*(dst) = (src >> 24) & 0xFF;     \
		*(dst + 1) = (src >> 16) & 0xFF; \
		*(dst + 2) = (src >> 8) & 0xFF;  \
		*(dst + 3) = (src)&0xFF;         \
		*idx += 4;                       \
	}

#define NTOH_32(dst, src, idx)                      \
	{                                               \
		(dst) = ((((unsigned char*)src)[0] << 24) | \
				 (((unsigned char*)src)[1] << 16) | \
				 (((unsigned char*)src)[2] << 8) |  \
				 ((unsigned char*)src)[3]);         \
		*(idx) += 4;                                \
	}

#define HTON_64(dst, src, idx)                  \
	{                                           \
		*(dst) = ((uint64_t)(src) >> 56) & 0xFF;     \
		*(dst + 1) = ((uint64_t)(src) >> 48) & 0xFF; \
		*(dst + 2) = ((uint64_t)(src) >> 40) & 0xFF; \
		*(dst + 3) = ((uint64_t)(src) >> 32) & 0xFF; \
		*(dst + 4) = ((uint64_t)(src) >> 24) & 0xFF; \
		*(dst + 5) = ((uint64_t)(src) >> 16) & 0xFF; \
		*(dst + 6) = ((uint64_t)(src) >> 8) & 0xFF;  \
		*(dst + 7) = ((uint64_t)(src) >> 0) & 0xFF;  \
		*idx += 8;                              \
	}

#define NTOH_64(dst, src, idx)                             \
	{                                                      \
		(dst) = (((uint64_t)(((unsigned char*)src)[0]) << 56) | \
				 ((uint64_t)(((unsigned char*)src)[1]) << 48) | \
				 ((uint64_t)(((unsigned char*)src)[2]) << 40) | \
				 ((uint64_t)(((unsigned char*)src)[3]) << 32) | \
				 ((uint64_t)(((unsigned char*)src)[4]) << 24) | \
				 ((uint64_t)(((unsigned char*)src)[5]) << 16) | \
				 ((uint64_t)(((unsigned char*)src)[6]) << 8) |  \
				 ((uint64_t)(((unsigned char*)src)[7]) << 0));  \
		*(idx) += 8;                                       \
	}

SIDL_END_C_INTERFACE
