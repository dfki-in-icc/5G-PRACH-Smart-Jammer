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

#include <string.h>
#include <strings.h>

// Internal includes
#include "acpCtx.h"
#include "acpMem.h"
#include "acpIdMap.h"
#include "adbg.h"

// Definitions
#define ACP_USER_ID_DEFAULT_MAP_SIZE 8

// Use assert if the interface in ACP host package doesn't match the interface on server
// #define ACP_ASSERT_INTERFACE_MISMATCH

// Static variables
static struct acpCtx _contexts[ACP_MAX_CTX_QTY];

void acpCtxInit(void)
{
	ACP_DEBUG_ENTER_LOG();
	ACP_DEBUG_LOG("Clearing the context pool");
	memset(_contexts, 0, sizeof(_contexts));
	ACP_DEBUG_EXIT_LOG(NULL);
}

acpCtx_t acpTakeCtx(void)
{
	ACP_DEBUG_ENTER_LOG();
	for (int index = 0; index < ACP_MAX_CTX_QTY; index++) {
		if (!_contexts[index].ptr) {
			_contexts[index].ptr = &_contexts[index];
			_contexts[index].arena = NULL;
			_contexts[index].aSize = 0;
			_contexts[index].isServer = false;
			_contexts[index].handle = NULL;
			_contexts[index].sHandle = NULL;
			_contexts[index].sock = -1;
			for (int peer = 0; peer < ACP_MAX_PEER_QTY; peer++) {
				_contexts[index].peers[peer] = -1;
				_contexts[index].peersHandshaked[peer] = 0;
			}
			_contexts[index].peersSize = 0;
			_contexts[index].lastPeer = -1;
			_contexts[index].userIdMap = (struct acpUserService*)acpMalloc(ACP_USER_ID_DEFAULT_MAP_SIZE * sizeof(struct acpUserService));
			SIDL_ASSERT(_contexts[index].userIdMap);
			_contexts[index].userIdMapMaxSize = ACP_USER_ID_DEFAULT_MAP_SIZE;
			_contexts[index].userIdMapSize = 0;

			_contexts[index].logger = NULL;
			_contexts[index].logBuf = NULL;
			_contexts[index].logBufMaxSize = 0;
			_contexts[index].logBufSize = 0;
			_contexts[index].logFormat = false;
			_contexts[index].logFormatBuf = NULL;
			_contexts[index].logFormatBufMaxSize = 0;

			ACP_DEBUG_LOG("Adding a new context: index=%d, _contexts[%d].ptr=%p", index, index, _contexts[index].ptr);
			ACP_DEBUG_EXIT_LOG(NULL);
			return &_contexts[index];
		}
	}
	ACP_DEBUG_EXIT_LOG(NULL);
	return NULL;
}

void acpGiveCtx(acpCtx_t ctx)
{
	ACP_DEBUG_ENTER_LOG();
	for (int index = 0; index < ACP_MAX_CTX_QTY; index++) {
		if (_contexts[index].ptr == ctx) {
			ACP_DEBUG_LOG("Clearing the context: index=%d, _contexts[%d].ptr=%p", index, index, _contexts[index].ptr);
			_contexts[index].ptr = NULL;
			if (_contexts[index].arena) {
				acpFree(_contexts[index].arena);
			}
			if (_contexts[index].userIdMap) {
				acpFree(_contexts[index].userIdMap);
			}

			if (_contexts[index].logBuf) {
				acpFree(_contexts[index].logBuf);
			}
			if (_contexts[index].logFormatBuf) {
				acpFree(_contexts[index].logFormatBuf);
			}

			break;
		}
	}
	ACP_DEBUG_EXIT_LOG(NULL);
}

const char* acpCtxGetItfNameFrom_localId(int id)
{
	int itf_id = (id >> 8) & 0xFF;

	for (unsigned int i = 0; i < acpItfMapSize; i++) {
		if (acpItfMap[i].id == itf_id) {
			return acpItfMap[i].name;
		}
	}

	return NULL;
}

int acpGetIndexFrom_localId_name(int id, const char* name)
{
	static int last_itf_id_index = 0;

	// FIXME: how to handle SIDL_PUBLIC_SERVICES
	if ((id & 0xF0000000) != 0x90000000) {
		return -ACP_ERR_UNKNOWN_SERVICE_NAME;
	}

	int itf_id = (id >> 8) & 0xFF;

	if (acpItfMap[last_itf_id_index].id != itf_id) {
		for (unsigned int i = 0; i < acpItfMapSize; i++) {
			if (acpItfMap[i].id == itf_id) {
				last_itf_id_index = i;
				break;
			}
		}
	}

	if (last_itf_id_index == (int)acpItfMapSize) {
		last_itf_id_index = 0;
		return -ACP_ERR_UNKNOWN_SERVICE_NAME;
	}

	int service_id_index = acpItfMap[last_itf_id_index].startIndex + (id & 0xFF);

	// This is just to check that the current server service matches the generated one
	if (name) {
		size_t remoteNameOffset = strlen(acpItfMap[last_itf_id_index].name);

		do {
			if (service_id_index >= (int)acpIdMapSize) {
#ifndef ACP_ASSERT_INTERFACE_MISMATCH
				service_id_index = -ACP_ERR_UNKNOWN_SERVICE_NAME;
#endif
				break;
			}

#ifndef ACP_ASSERT_INTERFACE_MISMATCH
			if (remoteNameOffset >= strlen(acpIdMap[service_id_index].name)) {
				service_id_index = -ACP_ERR_UNKNOWN_SERVICE_NAME;
				break;
			}
#else
			SIDL_ASSERT(remoteNameOffset < strlen(acpIdMap[service_id_index].name));
#endif

			const char* remoteName = acpIdMap[service_id_index].name + remoteNameOffset;
			int ret = strcasecmp(remoteName, name);
#ifndef ACP_ASSERT_INTERFACE_MISMATCH
			if (ret != 0) {
				service_id_index = -ACP_ERR_UNKNOWN_SERVICE_NAME;
				break;
			}
#else
			SIDL_ASSERT(ret == 0);
#endif
		} while (false);

		// iterate interface to find the name
		if (service_id_index < 0) {
			unsigned int start = acpItfMap[last_itf_id_index].startIndex;
			unsigned int end = start + acpItfMap[last_itf_id_index].servicesQty;
			for (unsigned int i = start; i < end; i++) {
				if (remoteNameOffset >= strlen(acpIdMap[i].name)) {
					continue;
				}
				const char* remoteName = acpIdMap[i].name + remoteNameOffset;
				int ret = strcasecmp(remoteName, name);
				if (ret == 0) {
					service_id_index = i;
					break;
				}
			}
		}
	} else {
#ifndef ACP_ASSERT_INTERFACE_MISMATCH
		if (service_id_index >= (int)acpIdMapSize) {
			service_id_index = -ACP_ERR_UNKNOWN_SERVICE_NAME;
		}
#endif
	}

	return service_id_index;
}

static int acpGetIndexFrom_name(const char* name)
{
	// TODO: better search
	for (unsigned int i = 0; i < acpIdMapSize; i++) {
		if (strcasecmp(acpIdMap[i].name, name) == 0) {
			return (int)i;
		}
	}
	return -ACP_ERR_UNKNOWN_SERVICE_NAME;
}

int acpCtxGetMsgUserId(struct acpCtx* ctx, unsigned int type)
{
	for (unsigned int i = 0; i < ctx->userIdMapSize; i++) {
		if (acpIdMap[ctx->userIdMap[i].id_index].remote_id == type) {
			return ctx->userIdMap[i].user_id;
		}
	}
	return -ACP_ERR_SERVICE_NOT_MAPPED;
}

static int acpCtxSetMsgUserIdFromIndex(struct acpCtx* ctx, int index, int userId)
{
	if (ctx->userIdMapSize == ctx->userIdMapMaxSize) {
		struct acpUserService* userIdMap = (struct acpUserService*)acpMalloc((ctx->userIdMapMaxSize + ACP_USER_ID_DEFAULT_MAP_SIZE) * sizeof(struct acpUserService));
		SIDL_ASSERT(userIdMap);

		memcpy(userIdMap, ctx->userIdMap, ctx->userIdMapSize * sizeof(struct acpUserService));

		acpFree(ctx->userIdMap);

		ctx->userIdMap = userIdMap;
		ctx->userIdMapMaxSize += ACP_USER_ID_DEFAULT_MAP_SIZE;
	}

	int userIdIndex = ctx->userIdMapSize++;

	ctx->userIdMap[userIdIndex].user_id = userId;
	ctx->userIdMap[userIdIndex].id_index = index;
	acpIdMap[index].remote_id = acpIdMap[index].local_id; // TODO: implement

	return 0;
}

int acpCtxSetMsgUserId(struct acpCtx* ctx, const char* name, int userId)
{
	int index = acpGetIndexFrom_name(name);
	if (index < 0) return index;
	SIDL_ASSERT(index < (int)acpIdMapSize);

	return acpCtxSetMsgUserIdFromIndex(ctx, index, userId);
}

int acpCtxSetMsgUserIdById(struct acpCtx* ctx, enum acpMsgLocalId lid, int userId)
{
	int index = acpGetIndexFrom_localId_name(lid, NULL);
	if (index < 0) return index;
	SIDL_ASSERT(index < (int)acpIdMapSize);

	return acpCtxSetMsgUserIdFromIndex(ctx, index, userId);
}

bool acpCtxIsValid(acpCtx_t ctx)
{
	struct acpCtx* c = ACP_CTX_CAST(ctx);
	if (c && (c->ptr == ctx)) {
		return true;
	}
	return false;
}

int acpCtxResolveId(int id, const char* name)
{
	int index = acpGetIndexFrom_localId_name(id, name);
	if (index < 0) return index;
	SIDL_ASSERT(index < (int)acpIdMapSize);

	acpIdMap[index].remote_id = id;

	return index;
}

const char* acpCtxGetMsgNameFromId(int id)
{
	if (id == (int)ACP_SERVICE_PUSH_TYPE) return "SERVICE_PUSH";
	int index = acpGetIndexFrom_localId_name(id, NULL);
	if (index < 0 || index >= (int)acpIdMapSize) return "UNKNOWN";
	return acpIdMap[index].name;
}

int acpCtxGetMsgKindFromId(int id)
{
	if (id == (int)ACP_SERVICE_PUSH_TYPE) return -1;
	int index = acpGetIndexFrom_localId_name(id, NULL);
	if (index < 0 || index >= (int)acpIdMapSize) return -ACP_ERR_SIDL_FAILURE;
	return acpIdMap[index].kind;
}
