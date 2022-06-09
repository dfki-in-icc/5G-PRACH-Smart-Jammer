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

#include <stdint.h>
#include <arpa/inet.h>

// Internal includes
#include "acpProto.h"
#include "acpCtx.h"

void acpProcessPushMsg(acpCtx_t ctx, size_t size, const unsigned char* buffer)
{
	struct acpCtx* c = ACP_CTX_CAST(ctx);

	c->handle = (void*)(uintptr_t)((buffer[8] << 24) | (buffer[9] << 16) | (buffer[10] << 8) | (buffer[11]));

	size -= ACP_HEADER_SIZE;

	struct acpWklmServicePushMessage* m = (struct acpWklmServicePushMessage*)&buffer[ACP_HEADER_SIZE];

	size_t serviceQty = ntohl(m->serviceQty);
	const uint8_t* p = (uint8_t*)m + 4;
	size_t offset = 0;
	size -= 4;
	for (size_t i = 0; i < serviceQty; i++) {
		const struct acpWklmService* s = (struct acpWklmService*)&p[offset];
		const char* name = (char*)&p[offset + 4];

		offset += 4;
		while ((offset < size) && p[offset]) {
			offset++;
		}
		offset++;
		if (offset >= size) {
			break;
		}
		acpCtxResolveId(ntohl(s->id), name);
	}
}

void acpBuildHeader(acpCtx_t ctx, unsigned int type, size_t size, unsigned char* buffer)
{
	struct acpCtx* c = ACP_CTX_CAST(ctx);

	uint32_t handle = (uint32_t)(uintptr_t)(c->sHandle ? c->sHandle : c->handle);
	uint32_t status = 0;

	size_t payloadSize = size - ACP_HEADER_SIZE; // payload size

	buffer[0] = 0x01;
	buffer[1] = 0x06;
	buffer[2] = (payloadSize >> 8) & 0xFF;
	buffer[3] = payloadSize & 0xFF;
	buffer[4] = (type >> 24) & 0xFF;
	buffer[5] = (type >> 16) & 0xFF;
	buffer[6] = (type >> 8) & 0xFF;
	buffer[7] = type & 0xFF;
	buffer[8] = (handle >> 24) & 0xFF;
	buffer[9] = (handle >> 16) & 0xFF;
	buffer[10] = (handle >> 8) & 0xFF;
	buffer[11] = handle & 0xFF;
	buffer[12] = (status >> 24) & 0xFF;
	buffer[13] = (status >> 16) & 0xFF;
	buffer[14] = (status >> 8) & 0xFF;
	buffer[15] = status & 0xFF;
}
