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

// Internal includes
#include "acp.h"
#include "acpIdMap.h"

SIDL_BEGIN_C_INTERFACE

#define ACP_CTX_CAST(pTR) ((struct acpCtx*)(pTR))

/** Defines user ID. */
struct acpUserService {
	/** User ID */
	int user_id;
	/** Offset in acpIdMap table */
	int id_index;
};

/** Defines ACP Context. */
struct acpCtx {
	/** Context pointer. */
	acpCtx_t ptr;
	/** Arena to devode received message. */
	unsigned char* arena;
	/** Arena size to decode received message. */
	size_t aSize;
	/** If context is server side. */
	bool isServer;
	/** ACP handle. */
	void* handle;
	/** ACP server handle. */
	void* sHandle;
	/** Socket descriptor (on client side, or master socket on server side). */
	int sock;
	/** Peers socket descriptors on server side. */
	int peers[ACP_MAX_PEER_QTY];
	/** Peers socket handshake state on server side. */
	int peersHandshaked[ACP_MAX_PEER_QTY];
	/** Peers socket descriptors size. */
	size_t peersSize;
	/** Last peer socket descriptor who sent the message. */
	int lastPeer;
	/** User ID map. */
	struct acpUserService* userIdMap;
	/** User ID map max size. */
	size_t userIdMapMaxSize;
	/** ID map size. */
	size_t userIdMapSize;

	/** Debug logger callback. */
	void (*logger)(const char*);
	/** Debug logger buffer. */
	unsigned char* logBuf;
	/** Debug logger buffer max size. */
	size_t logBufMaxSize;
	/** Debug logger buffer size. */
	size_t logBufSize;
	/** Debug logger use formatting. */
	bool logFormat;
	/** Debug logger formatting buffer. */
	unsigned char* logFormatBuf;
	/** Debug logger formatting buffer max size. */
	size_t logFormatBufMaxSize;
};

/** Init. */
void acpCtxInit(void);

/** Returns new available context. */
acpCtx_t acpTakeCtx(void);

/** Makes the context available. */
void acpGiveCtx(acpCtx_t ctx);

/** Gets message user id. */
int acpCtxGetMsgUserId(struct acpCtx* ctx, unsigned int type);

/** Sets message user id. */
int acpCtxSetMsgUserId(struct acpCtx* ctx, const char* name, int userId);

/** Sets message user id. */
int acpCtxSetMsgUserIdById(struct acpCtx* ctx, enum acpMsgLocalId lid, int userId);

/** Checks if the context is valid. */
bool acpCtxIsValid(acpCtx_t ctx);

/** Resolves message id. */
int acpCtxResolveId(int id, const char* name);

/** Gets message name from id (for debug). */
const char* acpCtxGetMsgNameFromId(int id);

/** Gets message kind from id (for debug). */
int acpCtxGetMsgKindFromId(int id);

/** Gets sidl name from local id. */
const char* acpCtxGetItfNameFrom_localId(int id);

SIDL_END_C_INTERFACE
