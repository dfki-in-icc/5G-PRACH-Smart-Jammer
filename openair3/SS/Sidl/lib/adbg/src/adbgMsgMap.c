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

#include "adbgMsgMap.h"

#include "serTest.h"
#include "serSys.h"
#include "serSysSrb.h"
#include "serVng.h"
#include "serDrb.h"
#include "serHandshake.h"

#include "adbgTest.h"
#include "adbgSys.h"
#include "adbgSysSrb.h"
#include "adbgVng.h"
#include "adbgDrb.h"
#include "adbgHandshake.h"

void adbgMsgLogInArgs(acpCtx_t ctx, enum acpMsgLocalId id, size_t size, const unsigned char* buffer)
{
	if (id == 0x90040100 /* TestHelloFromSS */) {
		return;
	}
	if (id == 0x90040102 /* TestPing */) {
		return;
	}
	if (id == 0x90040103 /* TestEcho */) {
		return;
	}
	if (id == 0x90040104 /* TestTest1 */) {
		return;
	}
	if (id == 0x90040105 /* TestTest2 */) {
		return;
	}
	if (id == 0x90040106 /* TestOther */) {
		return;
	}
	if (id == 0x90040200 /* SysProcess */) {
		struct SYSTEM_CTRL_REQ* FromSS;
		if (serSysProcessDecSrv(buffer, size, NULL, 0, &FromSS) == 0) {
			adbgSysProcessLogIn(ctx, FromSS);
			serSysProcessFreeSrv(FromSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	if (id == 0x90040300 /* SysVTEnquireTimingAck */) {
		return;
	}
	if (id == 0x90040400 /* SysSrbProcessFromSS */) {
		struct EUTRA_RRC_PDU_REQ* FromSS;
		if (serSysSrbProcessFromSSDecSrv(buffer, size, NULL, 0, &FromSS) == 0) {
			adbgSysSrbProcessFromSSLogIn(ctx, FromSS);

			serSysSrbProcessFromSSFreeSrv(FromSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	if (id == 0x90040500 /* VngProcess */) {
		struct EUTRA_VNG_CTRL_REQ* FromSS;
		if (serVngProcessDecSrv(buffer, size, NULL, 0, &FromSS) == 0) {
			adbgVngProcessLogIn(ctx, FromSS);
			serVngProcessFreeSrv(FromSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	if (id == 0x90040600 /* DrbProcessFromSS */) {
		struct DRB_COMMON_REQ* FromSS;
		if (serDrbProcessFromSSDecSrv(buffer, size, NULL, 0, &FromSS) == 0) {
			adbgDrbProcessFromSSLogIn(ctx, FromSS);

			serDrbProcessFromSSFreeSrv(FromSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}

	/* Handshake */
	if (id == 0x90040700 /* HandshakeHandleFromSS */) {
		return;
	}
	SIDL_ASSERT(0);
}

void adbgMsgLogOutArgs(acpCtx_t ctx, enum acpMsgLocalId id, size_t size, const unsigned char* buffer)
{
	if (id == 0x90040101 /* TestHelloToSS */) {
		return;
	}
	if (id == 0x90040102 /* TestPing */) {
		return;
	}
	if (id == 0x90040103 /* TestEcho */) {
		return;
	}
	if (id == 0x90040104 /* TestTest1 */) {
		return;
	}
	if (id == 0x90040105 /* TestTest2 */) {
		return;
	}
	if (id == 0x90040106 /* TestOther */) {
		return;
	}
	if (id == 0x90040200 /* SysProcess */) {
		struct SYSTEM_CTRL_CNF* ToSS;
		if (serSysProcessDecClt(buffer, size, NULL, 0, &ToSS) == 0) {
			adbgSysProcessLogOut(ctx, ToSS);
			serSysProcessFreeClt(ToSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	if (id == 0x90040301 /* SysVTEnquireTimingUpd */) {
		return;
	}
	if (id == 0x90040401 /* SysSrbProcessToSS */) {
		struct EUTRA_RRC_PDU_IND* ToSS;
		if (serSysSrbProcessToSSDecClt(buffer, size, NULL, 0, &ToSS) == 0) {
			adbgSysSrbProcessToSSLogOut(ctx, ToSS);
			serSysSrbProcessToSSFreeClt(ToSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}

	if (id == 0x90040500 /* VngProcess */) {
		struct EUTRA_VNG_CTRL_CNF* ToSS;
		if (serVngProcessDecClt(buffer, size, NULL, 0, &ToSS) == 0) {
			adbgVngProcessLogOut(ctx, ToSS);
			serVngProcessFreeClt(ToSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	if (id == 0x90040601 /* DrbProcessToSS */) {
		struct DRB_COMMON_IND* ToSS;
		if (serDrbProcessToSSDecClt(buffer, size, NULL, 0, &ToSS) == 0) {
			adbgDrbProcessToSSLogOut(ctx, ToSS);
			serDrbProcessToSSFreeClt(ToSS);
		} else {
			adbgPrintLog(ctx, "cannot decode buffer");
			adbgPrintLog(ctx, NULL);
		}
		return;
	}
	/* Handshake */
	if (id == 0x90040701 /* HandshakeHandleToSS */) {
		return;
	}
	SIDL_ASSERT(0);
}
