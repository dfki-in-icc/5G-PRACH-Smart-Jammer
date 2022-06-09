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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Internal includes
#include "acp.h"
#include "acpMem.h"
#include "acpCtx.h"
#include "acpSocket.h"
#include "acpProto.h"
#include "acpMsgIds.h"
#include "acpInternal.h"
#include "adbg.h"
#include "adbgMsg.h"

#include "acpVer.h"
#include "acpHandshake.h"

// #define ACP_LOG_DEBUG(...) printf(__VA_ARGS__);
#define ACP_LOG_DEBUG(...)

// Static variables
static acpMalloc_t _alloc = NULL;
static acpFree_t _release = NULL;
static MSec_t _socketTimeout = 1000;
static bool _initialized = false;
bool acp_printPrettyMessages = false;

// Adds peer to the server context.
static void acpAddPeer(struct acpCtx* ctx, int peer)
{
	for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
		if (ctx->peers[i] == -1) {
			ctx->peers[i] = peer;
			ctx->peersHandshaked[i] = 0;
			ctx->peersSize++;
			return;
		}
	}
	SIDL_ASSERT(0);
}

// Removes peer from the server context.
static void acpRemovePeer(struct acpCtx* ctx, int peer)
{
	for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
		if (ctx->peers[i] == peer) {
			ctx->peers[i] = -1;
			ctx->peersHandshaked[i] = 0;
			ctx->peersSize--;
			return;
		}
	}
}

static bool acpPeerHandshaked(struct acpCtx* ctx, int peer)
{
	for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
		if (ctx->peers[i] == peer) {
			return (ctx->peersHandshaked[i] != 0)?true:false;
		}
	}

	return false;
}

static void acpPeerSetHandshaked(struct acpCtx* ctx, int peer, int flag)
{
	for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
		if (ctx->peers[i] == peer) {
			ctx->peersHandshaked[i] = flag;
			return;
		}
	}
}

// ___________________________ / Interface // ___________________________

void* acpMalloc(size_t size)
{
	if (_alloc) {
		return _alloc(size);
	}
	return NULL;
}

void acpFree(void* ptr)
{
	if (_release) {
		_release(ptr);
		return;
	}
}

void acpInit(acpMalloc_t alloc, acpFree_t release, MSec_t socketTimeout)
{
	ACP_DEBUG_ENTER_TRACE_LOG();

	if (_initialized) {
		ACP_DEBUG_EXIT_TRACE_LOG("FALSE");
		return;
	}

	SIDL_ASSERT(alloc && release);

	_alloc = alloc;

	_release = release;

	_socketTimeout = socketTimeout;

	acpCtxInit();

	_initialized = true;
	ACP_DEBUG_EXIT_TRACE_LOG("TRUE");
}

int acpUpdateCtx(acpCtx_t ctx, size_t size, const unsigned char* buffer)
{
	ACP_DEBUG_ENTER_TRACE_LOG();

	if (!acpCtxIsValid(ctx)) {
		ACP_DEBUG_EXIT_TRACE_LOG("ACP_ERR_INVALID_CTX");
		return -ACP_ERR_INVALID_CTX;
	}

	if (size < ACP_HEADER_SIZE) {
		ACP_DEBUG_EXIT_TRACE_LOG("ACP_ERR_INVALID_HEADER");
		return -ACP_ERR_INVALID_HEADER;
	}

	acpProcessPushMsg(ctx, size, buffer);

	ACP_DEBUG_EXIT_TRACE_LOG(NULL);
	return 0;
}

int acpCreateCtx(acpCtx_t* ctx)
{
	SIDL_ASSERT(ctx);

	*ctx = acpTakeCtx();
	if (!*ctx) {
		return -ACP_ERR_INVALID_CTX;
	}

	return 0;
}

int acpDeleteCtx(acpCtx_t ctx)
{
	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	acpGiveCtx(ctx);

	return 0;
}

bool acpIsConnected(acpCtx_t ctx)
{
	if (!acpCtxIsValid(ctx)) {
		return false;
	}

	struct acpCtx* c = ACP_CTX_CAST(ctx);
	return (c->sHandle != NULL);
}

int acpGetMsgSidlStatus(size_t size, const unsigned char* buffer, SidlStatus* sidlStatus)
{
	if (size < ACP_HEADER_SIZE) {
		return -ACP_ERR_INVALID_HEADER;
	}

	if (sidlStatus) {
		*sidlStatus = (SidlStatus)(uint32_t)((buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | (buffer[15]));
	}

	return 0;
}

int acpGetMsgLength(size_t size, unsigned char* buffer)
{
	if (size < ACP_HEADER_SIZE) {
		return -ACP_ERR_INVALID_HEADER;
	}

	int length = (int)((buffer[2] << 8) | buffer[3]);

	return length;
}

int acpGetMsgId(acpCtx_t ctx, size_t size, const unsigned char* buffer)
{
	if (size < ACP_HEADER_SIZE) {
		return -ACP_ERR_INVALID_HEADER;
	}

	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	int type = (int)((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]));

	if (type == (int)ACP_SERVICE_PUSH_TYPE) {
		acpUpdateCtx(ctx, size, buffer);
		return type;
	}

#if 0 // TODO
	if (type == ACP_LID_InternalConnectToMs) {
		SidlStatus sidlStatus;
		if (acpGetMsgSidlStatus(ACP_HEADER_SIZE, buffer, &sidlStatus) < 0) {
			return -ACP_ERR_INVALID_HEADER;
		}
		if (sidlStatus == SIDL_STATUS_OK) {
			uint32_t sHandle = 0;
			if (acpInternalConnectToMsDec(ctx, buffer, size, &sHandle) == 0) {
				if (sHandle) {
					ACP_CTX_CAST(ctx)->sHandle = (void*)(uintptr_t)sHandle;
				}
			}
		}
	}
#endif

	return acpCtxGetMsgUserId(ACP_CTX_CAST(ctx), type);
}

int acpGetMsgLocalId(size_t size, const unsigned char* buffer, enum acpMsgLocalId* localMsgId)
{
	if (size < ACP_HEADER_SIZE) {
		return -ACP_ERR_INVALID_HEADER;
	}

	int type = (int)((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]));

	if (localMsgId) {
		*localMsgId = (enum acpMsgLocalId)type;
	}

	if (type == (int)ACP_SERVICE_PUSH_TYPE) {
		return type;
	}

	return 0;
}

static int acpRecvMsgInternal(int sock, size_t size, unsigned char* buffer)
{
	int length = 0;
	SidlStatus sidlStatus;

	/* Read ACP message header */
	int sz = acpSocketReceive(sock, ACP_HEADER_SIZE, buffer, _socketTimeout, NULL);
	if (sz < 0) {
		return -ACP_ERR_SOCK_ERROR;
	}

	if (sz == 0) {
		return 0;
	}

	if (sz != ACP_HEADER_SIZE) {
		return -ACP_ERR_SOCK_ERROR;
	}

	/* Get payload length */
	length = acpGetMsgLength(ACP_HEADER_SIZE, buffer);
	if (length < 0) {
		return -ACP_ERR_INVALID_HEADER;
	}

	/* Check decode buffer is enough to read received message */
	if (length > (int)(size - ACP_HEADER_SIZE)) {
		return -ACP_ERR_INTERNAL;
	}

	if (length > 0) {
		/* Read ACP message payload */
		sz = acpSocketReceive(sock, length, &buffer[ACP_HEADER_SIZE], _socketTimeout, NULL);
		if (sz < 0) {
			return -ACP_ERR_SOCK_ERROR;
		}

		if (sz != length) {
			return -ACP_ERR_SOCK_TIMEOUT;
		}
	}

	if (acpGetMsgSidlStatus(ACP_HEADER_SIZE, buffer, &sidlStatus) < 0) {
		return -ACP_ERR_INVALID_HEADER;
	}

	if (sidlStatus != SIDL_STATUS_OK) {
		return -ACP_ERR_SIDL_FAILURE;
	}

	return length+ACP_HEADER_SIZE;
}

static int acpHandleHandshakeFromClient(struct acpCtx* ctx, int sock, struct AcpHandshake_Type *hs)
{
	struct AcpHandshakeRsp_Type hs_rsp;

	memset(hs_rsp.acpVersion, 0, 32);
	snprintf(hs_rsp.acpVersion, 31, "%s", ACP_VERSION);
	memset(hs_rsp.acpVerCksm, 0, 64);
	snprintf(hs_rsp.acpVerCksm, 63, "%s", ACP_VERSION_CKSM);

	/* ACP version matched */
	if(strncmp(ACP_VERSION, hs->acpVersion, strlen(ACP_VERSION)) == 0 && strncmp(ACP_VERSION_CKSM, hs->acpVerCksm, strlen(ACP_VERSION)) == 0) {
		hs_rsp.acpVersionChk = true;
	} else {
		hs_rsp.acpVersionChk = false;
	}

	ACP_DEBUG_CLOG(ctx, "ACP version: %s", hs_rsp.acpVersionChk?"matched":"not matched");

	int ret = 0;
	const size_t size = 16*1024;
	size_t bufSize = size;
	unsigned char* buffer = (unsigned char*)acpMalloc(size);
	SIDL_ASSERT(buffer);

	ret = acpHandshakeHandleToSSEncSrv(ctx, buffer, &bufSize, &hs_rsp);
	if( ret == 0) {
		if (acpSocketSend(sock, bufSize, buffer) != (int)bufSize) {
			ACP_DEBUG_CLOG(ctx, "Error: Failed to send HandshakeToSS message");
			acpPeerSetHandshaked(ctx, sock, 0);
			ret = -ACP_ERR_INTERNAL;
		} else {
			ACP_DEBUG_CLOG(ctx, "HandshakeToSS message sent");
			//No fatal to be generated if handshake not well processed
			acpPeerSetHandshaked(ctx, sock, 1);
			//acpPeerSetHandshaked(ctx, sock, (hs_rsp.acpVersionChk?1:0));
			ret = 0;
		}
	} else {
		ACP_DEBUG_CLOG(ctx, "ERROR: Failed to encode HandshakeToSS message");
		acpPeerSetHandshaked(ctx, sock, 0);
		ret = -ACP_ERR_INTERNAL;
	}

	acpFree(buffer);

	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return ret;
}

static int acpHandleHandshakeToServer(struct acpCtx* ctx, int sock)
{
	ACP_DEBUG_ENTER_CLOG(ctx);
	const size_t size = 16*1024;
	unsigned char* buffer = (unsigned char*)acpMalloc(size);
	SIDL_ASSERT(buffer);

	int length;
	enum acpMsgLocalId localId;

	/* Send Handshake to Server */
	struct AcpHandshake_Type hs;
	struct AcpHandshakeRsp_Type *hs_rsp;
	size_t bufSize = size;
	int    ret = 0;
	memset(hs.acpVersion, 0, 32);
	snprintf(hs.acpVersion, 31, "%s", ACP_VERSION);
	memset(hs.acpVerCksm, 0, 64);
	snprintf(hs.acpVerCksm, 63, "%s", ACP_VERSION_CKSM);
	ret = acpHandshakeHandleFromSSEncClt(ctx, buffer, &bufSize, &hs);
	if(ret == 0) {
		int retry = 0;

		if (acpSocketSend(sock, bufSize, buffer) != (int)bufSize) {
			ACP_DEBUG_EXIT_CLOG(ctx, "Failed to send HandshakeFromSS message");
			acpFree(buffer);
			return -ACP_ERR_INTERNAL;
		}

		/* Now wait for handshake rsp from Server side */
		/* Loop */
		for (;;) {
			length = acpRecvMsgInternal(sock, size, buffer);
			if (length < 0) {
				ACP_DEBUG_CLOG(ctx, "ERROR: Failed to receive ACP message, error=%d", length);
				ret = -ACP_ERR_SOCK_ERROR;
				break;
			}

			/* FIXME: No message (timeout on socket), retry */
			if (length == 0) {
				retry++;
				if(retry == 10) {
					ret = -ACP_ERR_SOCK_TIMEOUT;
					ACP_DEBUG_CLOG(ctx, "ERROR: No HandshakeToSSToSS message for ACP version handshake");
					break;
				}
				continue;
			}

			/* Get local message ID */
			if (acpGetMsgLocalId(ACP_HEADER_SIZE, buffer, &localId) < -1) {
				ret = -ACP_ERR_INVALID_HEADER;
				break;
			}

			ACP_DEBUG_CLOG(ctx, "<<< Receive '%s' service message [localId=%08X]", acpGetMsgName(ACP_HEADER_SIZE, buffer), localId);

			if (localId != ACP_LID_HandshakeHandleToSS) {
				/* Discard message other than handshake rsp */
				retry++;
				if(retry == 10) {
					ret = -ACP_ERR_SOCK_TIMEOUT;
					break;
				}
				continue;
			}

			if (acpHandshakeHandleToSSDecClt(ctx, buffer, length, &hs_rsp) != 0) {
				ACP_DEBUG_CLOG(ctx, "ERROR: Failed to decode HandshakeToSS message");
				ret = -ACP_ERR_INTERNAL;
				break;
			}

			if(!hs_rsp->acpVersionChk) {
				ACP_DEBUG_CLOG(ctx, "ERROR: ACP version Not Matched: cli_version=[%s, cksm: %s] srv_version=[%s, chksm: %s].", ACP_VERSION, ACP_VERSION_CKSM, hs_rsp->acpVersion, hs_rsp->acpVerCksm);
				acpHandshakeHandleToSSFreeClt(hs_rsp);
				ret = 0;//-ACP_ERR_INVALID_VERSION;
				break;
			}

			ACP_DEBUG_CLOG(ctx, "ACP version Matched: [%s], checksum: [%s]", hs_rsp->acpVersion, hs_rsp->acpVerCksm);
			acpHandshakeHandleToSSFreeClt(hs_rsp);
			ret = 0;
			break;
		}
	} else {
		ACP_DEBUG_CLOG(ctx, "ERROR: Failed to encode HandshakeFromSS message");
		ret = -ACP_ERR_INTERNAL;
	}

	acpFree(buffer);
	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return ret;
}

/* Performs full connection to server. */
static int acpConnectToSrv(struct acpCtx* ctx, int sock)
{
	ACP_DEBUG_ENTER_CLOG(ctx);

	/* Perform Handshake to Server */
	int ret = acpHandleHandshakeToServer(ctx, sock);

	if(ret != 0) {
		ACP_DEBUG_EXIT_CLOG(ctx, "Error: Handshake failure");
		return ret;
	}

	/* TODO: Other operations to communicate with server */

	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return ret;
}


int acpRecvMsg(acpCtx_t ctx, size_t* size, unsigned char* buffer)
{
	SIDL_ASSERT(size && buffer);

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_ENTER_TRACE_CLOG(ctx);
#endif

	if (!acpCtxIsValid(ctx)) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_INVALID_CTX");
		return -ACP_ERR_INVALID_CTX;
	}

	int err, sz, length, userId;
	unsigned int handle;
	SidlStatus sidlStatus;

	int sock = ACP_CTX_CAST(ctx)->sock;
	size_t peersSize = 0;
	int* peers = NULL;

	bool disconnected = false;

	if (ACP_CTX_CAST(ctx)->isServer) {
		peersSize = ACP_CTX_CAST(ctx)->peersSize;
		peers = ACP_CTX_CAST(ctx)->peers;
	}

	err = acpSocketSelectMulti(&sock, _socketTimeout, peersSize, peers);
	if (err == 0) {
#ifdef ACP_DEBUG_TRACE_FLOOD
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, NULL);
#endif
		return 0;
	}
	if (err < 0) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "acpSocketSelectMulti failed");
		return -ACP_ERR_SOCK_ERROR;
	}

	if (ACP_CTX_CAST(ctx)->isServer) {
		if (ACP_CTX_CAST(ctx)->sock == sock) {
			int peerSock = acpSocketAccept(sock);
			if (peerSock < 0) {
				ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "acpSocketAccept failed");
				return -ACP_ERR_SOCK_ERROR;
			}
			acpAddPeer(ACP_CTX_CAST(ctx), peerSock);
			ACP_DEBUG_CLOG(ctx, "Peer connected");
			ACP_DEBUG_EXIT_TRACE_CLOG(ctx, NULL);
			return 0;
		}

		ACP_CTX_CAST(ctx)->lastPeer = sock;
	}

	// Check decode buffer is enough to read message header
	if (ACP_HEADER_SIZE > *size) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SMALL_BUFFER");
		return -ACP_ERR_SMALL_BUFFER;
	}

	// Read SIDL message header
	sz = acpSocketReceive(sock, ACP_HEADER_SIZE, buffer, _socketTimeout, (ACP_CTX_CAST(ctx)->isServer ? &disconnected : NULL));
	if (sz == 0) {
		if (ACP_CTX_CAST(ctx)->isServer && disconnected) {
			acpRemovePeer(ACP_CTX_CAST(ctx), sock);
			acpSocketClose(sock);
			ACP_DEBUG_CLOG(ctx, "Peer disconnected");
		}
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, NULL);
		return -ACP_ERR_SOCKCONN_ABORTED;
	}
	if (sz < 0) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SOCK_ERROR");
		return -ACP_ERR_SOCK_ERROR;
	}
	if (sz != ACP_HEADER_SIZE) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SOCK_TIMEOUT");
		return -ACP_ERR_SOCK_TIMEOUT;
	}

	// Get payload length
	length = acpGetMsgLength(ACP_HEADER_SIZE, buffer);
	if (length < 0) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_INVALID_HEADER");
		return -ACP_ERR_INVALID_HEADER;
	}

	// Check decode buffer is enough to read received message
	if (length > (int)(*size - ACP_HEADER_SIZE)) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SMALL_BUFFER");
		return -ACP_ERR_SMALL_BUFFER;
	}

	if (length > 0) {
		// Read ACP message payload
		sz = acpSocketReceive(sock, length, &buffer[ACP_HEADER_SIZE], _socketTimeout, NULL);
		if (sz < 0) {
			ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SOCK_ERROR");
			return -ACP_ERR_SOCK_ERROR;
		}
		if (sz != length) {
			ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SOCK_TIMEOUT");
			return -ACP_ERR_SOCK_TIMEOUT;
		}
	}

	if (acpGetMsgSidlStatus(ACP_HEADER_SIZE, buffer, &sidlStatus) < 0) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_INVALID_HEADER");
		return -ACP_ERR_INVALID_HEADER;
	}
	if (sidlStatus != SIDL_STATUS_OK) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_SIDL_FAILURE");
		return -ACP_ERR_SIDL_FAILURE;
	}

	// Get message ID
	userId = acpGetMsgId(ctx, length + ACP_HEADER_SIZE, buffer);

	if (userId == (int)ACP_SERVICE_PUSH_TYPE) {
		userId = 0;
	}

	handle = (unsigned int)((buffer[8] << 24) | (buffer[9] << 16) | (buffer[10] << 8) | (buffer[11]));

	if (handle == ACP_KEEPALIVE_HANDLE) {
		userId = 0;
	}

	*size = length + ACP_HEADER_SIZE;
	int type = (int)((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]));
	ACP_DEBUG_PREFIX_CLOG(ctx, "Receiving message '%s' (userId: %d, type: 0x%x)\r\n", acpGetMsgName(*size, buffer), userId, type);

	if (ACP_CTX_CAST(ctx)->isServer) {
		bool handshaked = acpPeerHandshaked(ctx, sock);
		if(!handshaked && type != (int)ACP_LID_HandshakeHandleFromSS) {
			/* The first message should be HandshakeHandleFromSS from client */
			ACP_DEBUG_PREFIX_CLOG(ctx, "Wrong message, should be HandshakeHandleFromSS\r\n");
			userId = -ACP_ERR_INTERNAL;
		} else if(type == (int)ACP_LID_HandshakeHandleFromSS) {
			/* Handle Handshake from Client */
			/* FIXME: How about duplicate Handshake? */
			struct AcpHandshake_Type *hs;

			if(acpHandshakeHandleFromSSDecSrv(ctx, buffer, length + ACP_HEADER_SIZE, &hs) == 0) {
				ACP_DEBUG_CLOG(ctx, "ACP version: cli_version=[%s, cksm: %s] srv_version=[%s, chksm: %s].", hs->acpVersion, hs->acpVerCksm, ACP_VERSION, ACP_VERSION_CKSM);
				userId = acpHandleHandshakeFromClient(ctx, sock, hs);
				acpHandshakeHandleFromSSFreeSrv(hs);
			} else {
				ACP_DEBUG_CLOG(ctx, "ERROR: Failed to decode HandshakeFromSS message");
				userId = -ACP_ERR_INTERNAL;
			}
		}
	}

	if (userId >= 0) {
#ifdef ACP_DEBUG
		adbgMsgLog(ctx, ADBG_MSG_LOG_RECV_DIR, *size, buffer);
#endif
	}
	ACP_DEBUG_EXIT_TRACE_CLOG(ctx, NULL);
	return userId;
}

int acpSendMsg(acpCtx_t ctx, size_t size, const unsigned char* buffer)
{
	SIDL_ASSERT(buffer);

	ACP_DEBUG_ENTER_TRACE_CLOG(ctx);

	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	int sock = ACP_CTX_CAST(ctx)->sock;
	if (ACP_CTX_CAST(ctx)->isServer) {
		sock = ACP_CTX_CAST(ctx)->lastPeer;
		if (sock == -1) {
			ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_INVALID_CTX");
			return -ACP_ERR_INVALID_CTX;
		}
	}

	ACP_DEBUG_PREFIX_CLOG(ctx, "Sending message '%s'", acpGetMsgName(size, buffer));
#ifdef ACP_DEBUG
	adbgMsgLog(ctx, ADBG_MSG_LOG_SEND_DIR, size, buffer);
#endif

	if (acpSocketSend(sock, size, buffer) != (int)size) {
		ACP_DEBUG_EXIT_TRACE_CLOG(ctx, "ACP_ERR_INTERNAL");
		return -ACP_ERR_INTERNAL;
	}

	/*
	if (ACP_CTX_CAST(ctx)->isServer) {
		ACP_CTX_CAST(ctx)->lastPeer = -1;
	}
	*/

	ACP_DEBUG_EXIT_TRACE_CLOG(ctx, NULL);
	return 0;
}

int acpClientInit(acpCtx_t ctx, IpAddress_t ipaddr, int port, size_t aSize)
{
	ACP_DEBUG_ENTER_CLOG(ctx);

	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	int sock = acpSocketConnect(ipaddr, port);
	if (sock < 0) {
		ACP_DEBUG_EXIT_CLOG(ctx, "ACP_ERR_SOCKCONN_ABORTED");
		return -ACP_ERR_SOCKCONN_ABORTED;
	}

	int ret = acpConnectToSrv(ACP_CTX_CAST(ctx), sock);
	if (ret < 0) {
		acpSocketClose(sock);
		ACP_DEBUG_CLOG(ctx, "ERROR: Failed to connect to Server (toSS) error=%d", ret);
		ACP_DEBUG_EXIT_CLOG(ctx, NULL);
		return ret;
	}

#if 0
	// Should not happen
	if (!acpIsConnected(ctx)) {
		acpSocketClose(sock);
		return -ACP_ERR_NOT_CONNECTED;
	}
#endif

	ACP_CTX_CAST(ctx)->sock = sock;
	ACP_CTX_CAST(ctx)->isServer = false;
	ACP_CTX_CAST(ctx)->aSize = aSize;
	ACP_CTX_CAST(ctx)->arena = (unsigned char*)acpMalloc(aSize);
	SIDL_ASSERT(ACP_CTX_CAST(ctx)->arena);

	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return 0;
}

int acpClientInitWithCtx(IpAddress_t ipaddr, int port, const struct acpMsgTable* msgTable, size_t aSize, acpCtx_t* ctx)
{
	SIDL_ASSERT(ctx);

	ACP_DEBUG_ENTER_LOG();

	acpCtx_t _ctx = NULL;
	if (acpCreateCtx(&_ctx) < 0) {
		ACP_DEBUG_EXIT_LOG("ACP_ERR_INVALID_CTX");
		return -ACP_ERR_INVALID_CTX;
	}

	if (msgTable) {
		while (msgTable->name) {
			int err = acpSetMsgId(_ctx, msgTable->name, msgTable->userId);
			if (err < 0) {
				acpDeleteCtx(_ctx);
				ACP_DEBUG_EXIT_LOG("acpSetMsgId failed");
				return err;
			}
			msgTable++;
		}
	}

	int ret = acpClientInit(_ctx, ipaddr, port, aSize);
	if (ret < 0) {
		acpDeleteCtx(_ctx);
		ACP_DEBUG_EXIT_LOG("acpClientInit failed");
		return ret;
	}

	if (acp_printPrettyMessages) {
		adbgSetPrintLogFormat(_ctx, true);
	}
	*ctx = _ctx;

	ACP_DEBUG_EXIT_LOG(NULL);
	return 0;
}

int acpServerInit(acpCtx_t ctx, IpAddress_t ipaddr, int port, size_t aSize)
{
	ACP_DEBUG_ENTER_CLOG(ctx);

	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	int sock = acpSocketListen(ipaddr, port);
	if (sock < 0) {
		ACP_DEBUG_EXIT_CLOG(ctx, "ACP_ERR_SOCKCONN_ABORTED");
		return -ACP_ERR_SOCKCONN_ABORTED;
	}

	ACP_CTX_CAST(ctx)->sock = sock;
	ACP_CTX_CAST(ctx)->isServer = true;
	ACP_CTX_CAST(ctx)->aSize = aSize;
	ACP_CTX_CAST(ctx)->arena = (unsigned char*)acpMalloc(aSize);
	SIDL_ASSERT(ACP_CTX_CAST(ctx)->arena);

	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return 0;
}

int acpServerInitWithCtx(IpAddress_t ipaddr, int port, const struct acpMsgTable* msgTable, size_t aSize, acpCtx_t* ctx)
{
	SIDL_ASSERT(ctx);

	ACP_DEBUG_ENTER_LOG();

	acpCtx_t _ctx = NULL;
	if (acpCreateCtx(&_ctx) < 0) {
		ACP_DEBUG_EXIT_LOG("ACP_ERR_INVALID_CTX");
		return -ACP_ERR_INVALID_CTX;
	}

	if (msgTable) {
		while (msgTable->name) {
			int err = acpSetMsgId(_ctx, msgTable->name, msgTable->userId);
			if (err < 0) {
				acpDeleteCtx(_ctx);
				ACP_DEBUG_EXIT_LOG("acpSetMsgId failed");
				return err;
			}
			msgTable++;
		}
	}

	int ret = acpServerInit(_ctx, ipaddr, port, aSize);
	if (ret < 0) {
		acpDeleteCtx(_ctx);
		ACP_DEBUG_EXIT_LOG("acpServerInit failed");
		return ret;
	}

	*ctx = _ctx;

	ACP_DEBUG_EXIT_LOG(NULL);
	return 0;
}

int acpClose2(acpCtx_t ctx)
{
	ACP_DEBUG_ENTER_CLOG(ctx);

	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	acpSocketClose(ACP_CTX_CAST(ctx)->sock);

	if (ACP_CTX_CAST(ctx)->isServer) {
		for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
			if (ACP_CTX_CAST(ctx)->peers[i] != -1) {
				acpSocketClose(ACP_CTX_CAST(ctx)->peers[i]);
			}
		}
	}

	ACP_DEBUG_EXIT_CLOG(ctx, NULL);
	return 0;
}

int acpClose(acpCtx_t ctx)
{
	ACP_DEBUG_ENTER_LOG();

	if (!acpCtxIsValid(ctx)) {
		ACP_DEBUG_EXIT_LOG("ACP_ERR_INVALID_CTX");
		return -ACP_ERR_INVALID_CTX;
	}

	acpSocketClose(ACP_CTX_CAST(ctx)->sock);

	if (ACP_CTX_CAST(ctx)->isServer) {
		for (int i = 0; i < ACP_MAX_PEER_QTY; i++) {
			if (ACP_CTX_CAST(ctx)->peers[i] != -1) {
				acpSocketClose(ACP_CTX_CAST(ctx)->peers[i]);
			}
		}
	}

	acpDeleteCtx(ctx);

	ACP_DEBUG_EXIT_LOG(NULL);
	return 0;
}

int acpGetSocketFd(acpCtx_t ctx)
{
	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	return ACP_CTX_CAST(ctx)->sock;
}

void acpSetSocketFd(acpCtx_t ctx, int socketfd)
{
	if (acpCtxIsValid(ctx)) {
		ACP_CTX_CAST(ctx)->sock = socketfd;
	}
}

int acpGetLastPeerSocketFd(acpCtx_t ctx)
{
	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	return ACP_CTX_CAST(ctx)->lastPeer;
}

void acpSetLastPeerSocketFd(acpCtx_t ctx, int socketfd)
{
	if (acpCtxIsValid(ctx)) {
		ACP_CTX_CAST(ctx)->lastPeer = socketfd;
	}
}

MSec_t acpGetSocketTimeout(void)
{
	return _socketTimeout;
}

int acpSetMsgId(acpCtx_t ctx, const char* name, int userMsgId)
{
	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}
	return acpCtxSetMsgUserId(ACP_CTX_CAST(ctx), name, userMsgId);
}

int acpSetMsgIdFromLocalId(acpCtx_t ctx, enum acpMsgLocalId localMsgId, int userMsgId)
{
	if (!acpCtxIsValid(ctx)) {
		return -ACP_ERR_INVALID_CTX;
	}

	return acpCtxSetMsgUserIdById(ACP_CTX_CAST(ctx), localMsgId, userMsgId);
}

const char* acpGetMsgName(size_t size, const unsigned char* buffer)
{
	if (size < ACP_HEADER_SIZE) {
		return "INVALID_HEADER";
	}
	int type = (int)((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]));
	return acpCtxGetMsgNameFromId(type);
}
