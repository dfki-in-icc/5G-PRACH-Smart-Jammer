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
#include "acp.h"

SIDL_BEGIN_C_INTERFACE

// clang-format off

#ifdef ACP_DEBUG
#define ACP_DEBUG_PREFIX_CLOG(ctx, fmt, args...) do { adbgPrintLog(ctx, "=== ACP_DEBUG: [%s:%d] " fmt ": ", __FILE__, __LINE__, ##args); } while (false)
#define ACP_DEBUG_CLOG(ctx, fmt, args...) do { adbgPrintLog(ctx, "=== ACP_DEBUG: [%s:%d] " fmt, __FILE__, __LINE__, ##args); adbgPrintLog(ctx, NULL); } while (false)
#define ACP_DEBUG_ENTER_CLOG(ctx) do { adbgPrintLog(ctx, ">>> ACP_DEBUG: Entering %s at [%s:%d]", __FUNCTION__, __FILE__, __LINE__); adbgPrintLog(ctx, NULL); } while (false)
#define ACP_DEBUG_EXIT_CLOG(ctx, val) do { \
		if (val != NULL) adbgPrintLog(ctx, "<<< ACP_DEBUG: Exiting %s at [%s:%d] with return code '%s'", __FUNCTION__, __FILE__, __LINE__, val); \
		else adbgPrintLog(ctx, "<<< ACP_DEBUG: Exiting %s at %s:%d", __FUNCTION__, __FILE__, __LINE__); \
		adbgPrintLog(ctx, NULL); \
	} while (false)
#define ACP_DEBUG_LOG(fmt, args...) do { adbgCheckAndSetGlobalLogger(); adbgGlobalLogger("=== ACP_DEBUG: [%s:%d] " fmt, __FILE__, __LINE__, ##args); } while (false)
#define ACP_DEBUG_ENTER_LOG() do { adbgCheckAndSetGlobalLogger(); adbgGlobalLogger(">>> ACP_DEBUG: Entering %s at [%s:%d]", __FUNCTION__, __FILE__, __LINE__); } while (false)
#define ACP_DEBUG_EXIT_LOG(val) do { \
		adbgCheckAndSetGlobalLogger(); \
		if (val != NULL) adbgGlobalLogger("<<< ACP_DEBUG: Exiting %s at [%s:%d] with return code '%s'", __FUNCTION__, __FILE__, __LINE__, val); \
		else adbgGlobalLogger("<<< ACP_DEBUG: Exiting %s at [%s:%d]", __FUNCTION__, __FILE__, __LINE__); \
	} while (false)

#ifdef ACP_DEBUG_TRACE
#define ACP_DEBUG_PREFIX_TRACE_CLOG(ctx, fmt, args...) ACP_DEBUG_PREFIX_CLOG(ctx, fmt, args)
#define ACP_DEBUG_TRACE_CLOG(ctx, fmt, args...) ACP_DEBUG_CLOG(ctx, fmt, args)
#define ACP_DEBUG_ENTER_TRACE_CLOG(ctx) ACP_DEBUG_ENTER_CLOG(ctx)
#define ACP_DEBUG_EXIT_TRACE_CLOG(ctx, val) ACP_DEBUG_EXIT_CLOG(ctx, val)
#define ACP_DEBUG_TRACE_LOG(fmt, args...) ACP_DEBUG_LOG(fmt, args)
#define ACP_DEBUG_ENTER_TRACE_LOG() ACP_DEBUG_ENTER_LOG()
#define ACP_DEBUG_EXIT_TRACE_LOG(val) ACP_DEBUG_EXIT_LOG(val)
#else //!ACP_DEBUG_TRACE
#define ACP_DEBUG_PREFIX_TRACE_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_TRACE_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_TRACE_CLOG(ctx) do {} while (false)
#define ACP_DEBUG_EXIT_TRACE_CLOG(ctx, val) do {} while (false)
#define ACP_DEBUG_TRACE_LOG(fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_TRACE_LOG() do {} while (false)
#define ACP_DEBUG_EXIT_TRACE_LOG(val) do {} while (false)
#endif // ACP_DEBUG_TRACE

#else // !ACP_DEBUG
#define ACP_DEBUG_PREFIX_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_CLOG(ctx) do {} while (false)
#define ACP_DEBUG_EXIT_CLOG(ctx, val) do {} while (false)
#define ACP_DEBUG_LOG(fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_LOG() do {} while (false)
#define ACP_DEBUG_EXIT_LOG(val) do {} while (false)

#define ACP_DEBUG_PREFIX_TRACE_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_TRACE_CLOG(ctx, fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_TRACE_CLOG(ctx) do {} while (false)
#define ACP_DEBUG_EXIT_TRACE_CLOG(ctx, val) do {} while (false)
#define ACP_DEBUG_TRACE_LOG(fmt, args...) do {} while (false)
#define ACP_DEBUG_ENTER_TRACE_LOG() do {} while (false)
#define ACP_DEBUG_EXIT_TRACE_LOG(val) do {} while (false)
#endif // ACP_DEBUG

// clang-format on

/** Default size for log buffer. */
#define ADBG_DEFAULT_LOG_BUFFER_SIZE (1 * 1024 * 1024) // 1MB

/** Default global logger (printf). */
void adbgDefaultGlobalLogger(const char* fmt, ...);

/** Default logger (printf). */
void adbgDefaultLogger(const char* str);

/** Type of global logger. Syntax similar to printf. */
typedef void (*adbgGlobalLogger_t)(const char* fmt, ...);

/** Type of logger. Should print the string. */
typedef void (*adbgLogger_t)(const char* str);

/** Global logger. */
extern adbgGlobalLogger_t adbgGlobalLogger;

/** Global context logger. */
extern adbgLogger_t adbgLogger;

/** Sets the global loggers.
 * If any argument is NULL then sets it to the default.
 */
void adbgSetGlobalLoggers(adbgGlobalLogger_t globalLogger, adbgLogger_t logger);

/** Checks the global logger and sets it to the default if NULL. */
void adbgCheckAndSetGlobalLogger();

/** Sets the context logger and log buffer.
 * If any argument is NULL then sets it to the default.
 */
void adbgSetLogger(acpCtx_t ctx, adbgLogger_t logger, size_t size);

/** Prints to the log buffer,
 * if argument fmt is NULL then flushes the log buffer to the context logger.
 */
void adbgPrintLog(acpCtx_t ctx, const char* fmt, ...);

/** Flushes the log buffer to the context logger with formatting. */
void adbgPrintFormatLog(acpCtx_t ctx);

/** Enables/disables log formatting. */
void adbgSetPrintLogFormat(acpCtx_t ctx, bool format);

SIDL_END_C_INTERFACE
