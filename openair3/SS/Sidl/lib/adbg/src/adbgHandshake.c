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

#include "adbgHandshake.h"

static void _adbgHandshake__AcpHandshake_Type(acpCtx_t _ctx, const struct AcpHandshake_Type* p)
{
	adbgPrintLog(_ctx, "acpVersion := '");
	for (size_t i1 = 0; i1 < 32; i1++) {
		adbgPrintLog(_ctx, "%02X", p->acpVersion[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "acpVerCksm := '");
	for (size_t i1 = 0; i1 < 64; i1++) {
		adbgPrintLog(_ctx, "%02X", p->acpVerCksm[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

void adbgHandshakeHandleFromSSLogIn(acpCtx_t _ctx, const struct AcpHandshake_Type* fromSS)
{
	adbgPrintLog(_ctx, "@HandshakeHandleFromSS In Args : { ");

	adbgPrintLog(_ctx, "fromSS := { ");
	_adbgHandshake__AcpHandshake_Type(_ctx, fromSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static void _adbgHandshake__AcpHandshakeRsp_Type(acpCtx_t _ctx, const struct AcpHandshakeRsp_Type* p)
{
	adbgPrintLog(_ctx, "acpVersionChk := %s", (p->acpVersionChk ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "acpVersion := '");
	for (size_t i1 = 0; i1 < 32; i1++) {
		adbgPrintLog(_ctx, "%02X", p->acpVersion[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "acpVerCksm := '");
	for (size_t i1 = 0; i1 < 64; i1++) {
		adbgPrintLog(_ctx, "%02X", p->acpVerCksm[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

void adbgHandshakeHandleToSSLogOut(acpCtx_t _ctx, const struct AcpHandshakeRsp_Type* toSS)
{
	adbgPrintLog(_ctx, "@HandshakeHandleToSS Out Args : { ");

	adbgPrintLog(_ctx, "toSS := { ");
	_adbgHandshake__AcpHandshakeRsp_Type(_ctx, toSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}
