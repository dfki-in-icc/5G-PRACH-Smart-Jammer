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

#include "adbgTest.h"

void adbgTestHelloFromSSLogIn(acpCtx_t _ctx, size_t StrQty, const char* StrArray)
{
	adbgPrintLog(_ctx, "@TestHelloFromSS In Args : { ");

	adbgPrintLog(_ctx, "Str := '");
	for (size_t i1 = 0; i1 < StrQty; i1++) {
		adbgPrintLog(_ctx, "%02X", StrArray[i1]);
	}
	adbgPrintLog(_ctx, "'O");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestHelloToSSLogOut(acpCtx_t _ctx, size_t StrQty, const char* StrArray)
{
	adbgPrintLog(_ctx, "@TestHelloToSS Out Args : { ");

	adbgPrintLog(_ctx, "Str := '");
	for (size_t i1 = 0; i1 < StrQty; i1++) {
		adbgPrintLog(_ctx, "%02X", StrArray[i1]);
	}
	adbgPrintLog(_ctx, "'O");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestPingLogIn(acpCtx_t _ctx, uint32_t FromSS)
{
	adbgPrintLog(_ctx, "@TestPing In Args : { ");

	adbgPrintLog(_ctx, "FromSS := %u", (unsigned int)FromSS);

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestPingLogOut(acpCtx_t _ctx, uint32_t ToSS)
{
	adbgPrintLog(_ctx, "@TestPing Out Args : { ");

	adbgPrintLog(_ctx, "ToSS := %u", (unsigned int)ToSS);

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static void _adbgTest__Empty(acpCtx_t _ctx, const struct Empty* p)
{
	adbgPrintLog(_ctx, "dummy := %d", (int)p->dummy);
}

static void _adbgTest__EchoData(acpCtx_t _ctx, const struct EchoData* p)
{
	adbgPrintLog(_ctx, "str := \"%s\"", p->str);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "empty := { ");
	for (size_t i1 = 0; i1 < p->emptyQty; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->emptyArray[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->emptyQty - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "eee := { ");
	_adbgTest__Empty(_ctx, p->eee);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "sss := { ");
	_adbgTest__Empty(_ctx, &p->sss);
	adbgPrintLog(_ctx, " }");
}

void adbgTestEchoLogIn(acpCtx_t _ctx, const struct EchoData* FromSS)
{
	adbgPrintLog(_ctx, "@TestEcho In Args : { ");

	adbgPrintLog(_ctx, "FromSS := { ");
	_adbgTest__EchoData(_ctx, FromSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestEchoLogOut(acpCtx_t _ctx, const struct EchoData* ToSS)
{
	adbgPrintLog(_ctx, "@TestEcho Out Args : { ");

	adbgPrintLog(_ctx, "ToSS := { ");
	_adbgTest__EchoData(_ctx, ToSS);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static void _adbgTest__Char_Foo_DynamicOptional(acpCtx_t _ctx, const struct char_Foo_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__Char_Koo_ArrayOptional(acpCtx_t _ctx, const struct char_Koo_ArrayOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 25; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__Int_Bar_Optional(acpCtx_t _ctx, const struct int_Bar_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "%d", (int)p->v);
}

static void _adbgTest__Output(acpCtx_t _ctx, const struct Output* p)
{
	adbgPrintLog(_ctx, "Foo := ");
	_adbgTest__Char_Foo_DynamicOptional(_ctx, &p->Foo);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Koo := ");
	_adbgTest__Char_Koo_ArrayOptional(_ctx, &p->Koo);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Bar := ");
	_adbgTest__Int_Bar_Optional(_ctx, &p->Bar);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Zoo := '");
	for (size_t i1 = 0; i1 < p->Zoo.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->Zoo.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Z := '");
	for (size_t i1 = 0; i1 < p->ZQty; i1++) {
		adbgPrintLog(_ctx, "%02X", p->ZArray[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "Far := { ");
	for (size_t i1 = 0; i1 < p->Far.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->Far.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->Far.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

void adbgTestTest1LogIn(acpCtx_t _ctx, const struct Output* out)
{
	adbgPrintLog(_ctx, "@TestTest1 In Args : { ");

	adbgPrintLog(_ctx, "out := { ");
	_adbgTest__Output(_ctx, out);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestTest2LogOut(acpCtx_t _ctx, const struct Output* out)
{
	adbgPrintLog(_ctx, "@TestTest2 Out Args : { ");

	adbgPrintLog(_ctx, "out := { ");
	_adbgTest__Output(_ctx, out);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

static const char* adbgTest__SomeEnum__ToString(SomeEnum v)
{
	switch(v) {
		case SomeEnum_Zero: return "SomeEnum_Zero";
		case SomeEnum_One: return "SomeEnum_One";
		case SomeEnum_Two: return "SomeEnum_Two";
		case SomeEnum_Three: return "SomeEnum_Three";
		default: return "Unknown";
	}
}

static void _adbgTest__Complex(acpCtx_t _ctx, const struct Complex* p)
{
	adbgPrintLog(_ctx, "val := %d", (int)p->val);
}

static void _adbgTest__Empty2(acpCtx_t _ctx, const struct Empty2* p)
{
	adbgPrintLog(_ctx, "dummy := %d", (int)p->dummy);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "simple := '");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "%02X", p->simple[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "complex := { ");
	for (size_t i1 = 0; i1 < 3; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Complex(_ctx, &p->complex[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != 3 - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
}

static void _adbgTest__Empty_optional_struct_1_Optional(acpCtx_t _ctx, const struct Empty_optional_struct_1_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgTest__Empty(_ctx, &p->v);
}

static void _adbgTest__Empty_optional_struct_2_Optional(acpCtx_t _ctx, const struct Empty_optional_struct_2_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgTest__Empty(_ctx, &p->v);
}

static void _adbgTest__Int_optional_int_1_Optional(acpCtx_t _ctx, const struct int_optional_int_1_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "%d", (int)p->v);
}

static void _adbgTest__Int_optional_int_2_Optional(acpCtx_t _ctx, const struct int_optional_int_2_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "%d", (int)p->v);
}

static void _adbgTest__Char_optional_string_1_Optional(acpCtx_t _ctx, const struct char_optional_string_1_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "\"%s\"", p->v);
}

static void _adbgTest__Char_optional_string_2_Optional(acpCtx_t _ctx, const struct char_optional_string_2_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "\"%s\"", p->v);
}

static void _adbgTest__Empty_optional_struct_array_1_ArrayOptional(acpCtx_t _ctx, const struct Empty_optional_struct_array_1_ArrayOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < 2; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != 2 - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__Empty_optional_struct_array_2_ArrayOptional(acpCtx_t _ctx, const struct Empty_optional_struct_array_2_ArrayOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < 2; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != 2 - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__Int_optional_int_array_1_ArrayOptional(acpCtx_t _ctx, const struct int_optional_int_array_1_ArrayOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 2; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__Int_optional_int_array_2_ArrayOptional(acpCtx_t _ctx, const struct int_optional_int_array_2_ArrayOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < 2; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__Empty_dynamic_optional_struct_1_DynamicOptional(acpCtx_t _ctx, const struct Empty_dynamic_optional_struct_1_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->v.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__Empty_dynamic_optional_struct_2_DynamicOptional(acpCtx_t _ctx, const struct Empty_dynamic_optional_struct_2_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->v.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__Int_dynamic_optional_int_1_DynamicOptional(acpCtx_t _ctx, const struct int_dynamic_optional_int_1_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__Int_dynamic_optional_int_2_DynamicOptional(acpCtx_t _ctx, const struct int_dynamic_optional_int_2_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	adbgPrintLog(_ctx, "'");
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->v.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
}

static void _adbgTest__TestUnion_Value(acpCtx_t _ctx, const union TestUnion_Value* p, enum TestUnion_Sel d)
{
	if (d == TestUnion_Zero) {
		adbgPrintLog(_ctx, "Zero := %d", (int)p->Zero);
		return;
	}
	if (d == TestUnion_One) {
		adbgPrintLog(_ctx, "One := %d", (int)p->One);
		return;
	}
	if (d == TestUnion_Two) {
		adbgPrintLog(_ctx, "Two := { ");
		_adbgTest__Empty(_ctx, &p->Two);
		adbgPrintLog(_ctx, " }");
		return;
	}
	if (d == TestUnion_Three) {
		adbgPrintLog(_ctx, "Three := { ");
		_adbgTest__Empty(_ctx, &p->Three);
		adbgPrintLog(_ctx, " }");
		return;
	}
	adbgPrintLog(_ctx, "INVALID");
}

static void _adbgTest__TestUnion(acpCtx_t _ctx, const struct TestUnion* p)
{
	_adbgTest__TestUnion_Value(_ctx, &p->v, p->d);
}

static void _adbgTest__TestUnion_optional_union_1_Optional(acpCtx_t _ctx, const struct TestUnion_optional_union_1_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgTest__TestUnion(_ctx, &p->v);
}

static void _adbgTest__TestUnion_optional_union_pointer_1_Optional(acpCtx_t _ctx, const struct TestUnion_optional_union_pointer_1_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgTest__TestUnion(_ctx, p->v);
}

static void _adbgTest__TestUnion_optional_union_2_Optional(acpCtx_t _ctx, const struct TestUnion_optional_union_2_Optional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	_adbgTest__TestUnion(_ctx, &p->v);
}

static void _adbgTest__TestUnion_dynamic_optional_union_1_DynamicOptional(acpCtx_t _ctx, const struct TestUnion_dynamic_optional_union_1_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__TestUnion(_ctx, &p->v.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__TestUnion_dynamic_optional_union_2_DynamicOptional(acpCtx_t _ctx, const struct TestUnion_dynamic_optional_union_2_DynamicOptional* p)
{
	if (!p->d) { adbgPrintLog(_ctx, "omit"); return; }
	for (size_t i1 = 0; i1 < p->v.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__TestUnion(_ctx, &p->v.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->v.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
}

static void _adbgTest__New(acpCtx_t _ctx, const struct New* p)
{
	adbgPrintLog(_ctx, "dynamic_struct := { ");
	for (size_t i1 = 0; i1 < p->dynamic_struct.d; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &p->dynamic_struct.v[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != p->dynamic_struct.d - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_ints := '");
	for (size_t i1 = 0; i1 < p->dynamic_ints.d; i1++) {
		adbgPrintLog(_ctx, "%02X", p->dynamic_ints.v[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_struct_1 := ");
	if (p->optional_struct_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_optional_struct_1_Optional(_ctx, &p->optional_struct_1);
	if (p->optional_struct_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_struct_2 := ");
	if (p->optional_struct_2.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_optional_struct_2_Optional(_ctx, &p->optional_struct_2);
	if (p->optional_struct_2.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_int_1 := ");
	_adbgTest__Int_optional_int_1_Optional(_ctx, &p->optional_int_1);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_int_2 := ");
	_adbgTest__Int_optional_int_2_Optional(_ctx, &p->optional_int_2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_string_1 := ");
	_adbgTest__Char_optional_string_1_Optional(_ctx, &p->optional_string_1);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_string_2 := ");
	_adbgTest__Char_optional_string_2_Optional(_ctx, &p->optional_string_2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_struct_array_1 := ");
	if (p->optional_struct_array_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_optional_struct_array_1_ArrayOptional(_ctx, &p->optional_struct_array_1);
	if (p->optional_struct_array_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_struct_array_2 := ");
	if (p->optional_struct_array_2.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_optional_struct_array_2_ArrayOptional(_ctx, &p->optional_struct_array_2);
	if (p->optional_struct_array_2.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_int_array_1 := ");
	_adbgTest__Int_optional_int_array_1_ArrayOptional(_ctx, &p->optional_int_array_1);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_int_array_2 := ");
	_adbgTest__Int_optional_int_array_2_ArrayOptional(_ctx, &p->optional_int_array_2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_struct_1 := ");
	if (p->dynamic_optional_struct_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_dynamic_optional_struct_1_DynamicOptional(_ctx, &p->dynamic_optional_struct_1);
	if (p->dynamic_optional_struct_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_struct_2 := ");
	if (p->dynamic_optional_struct_2.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__Empty_dynamic_optional_struct_2_DynamicOptional(_ctx, &p->dynamic_optional_struct_2);
	if (p->dynamic_optional_struct_2.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_int_1 := ");
	_adbgTest__Int_dynamic_optional_int_1_DynamicOptional(_ctx, &p->dynamic_optional_int_1);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_int_2 := ");
	_adbgTest__Int_dynamic_optional_int_2_DynamicOptional(_ctx, &p->dynamic_optional_int_2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "union_test_pointer := { ");
	_adbgTest__TestUnion(_ctx, p->union_test_pointer);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "union_test := { ");
	_adbgTest__TestUnion(_ctx, &p->union_test);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_union_1 := ");
	if (p->optional_union_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__TestUnion_optional_union_1_Optional(_ctx, &p->optional_union_1);
	if (p->optional_union_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_union_pointer_1 := ");
	if (p->optional_union_pointer_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__TestUnion_optional_union_pointer_1_Optional(_ctx, &p->optional_union_pointer_1);
	if (p->optional_union_pointer_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "optional_union_2 := ");
	if (p->optional_union_2.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__TestUnion_optional_union_2_Optional(_ctx, &p->optional_union_2);
	if (p->optional_union_2.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_union_1 := ");
	if (p->dynamic_optional_union_1.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__TestUnion_dynamic_optional_union_1_DynamicOptional(_ctx, &p->dynamic_optional_union_1);
	if (p->dynamic_optional_union_1.d) { adbgPrintLog(_ctx, " }"); };
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "dynamic_optional_union_2 := ");
	if (p->dynamic_optional_union_2.d) { adbgPrintLog(_ctx, "{ "); };
	_adbgTest__TestUnion_dynamic_optional_union_2_DynamicOptional(_ctx, &p->dynamic_optional_union_2);
	if (p->dynamic_optional_union_2.d) { adbgPrintLog(_ctx, " }"); };
}

void adbgTestOtherLogIn(acpCtx_t _ctx, const struct Empty* in1, uint32_t in2, size_t in3Qty, const char* in3Array, const char* in4, bool in5, int in6, float in7, SomeEnum in8, size_t in9Qty, const struct Empty* in9Array, const struct Empty2* in10, const struct New* in11)
{
	adbgPrintLog(_ctx, "@TestOther In Args : { ");

	adbgPrintLog(_ctx, "in1 := { ");
	_adbgTest__Empty(_ctx, in1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in2 := %u", (unsigned int)in2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in3 := '");
	for (size_t i1 = 0; i1 < in3Qty; i1++) {
		adbgPrintLog(_ctx, "%02X", in3Array[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in4 := \"%s\"", in4);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in5 := %s", (in5 ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in6 := %d", (int)in6);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in7 := %.6f", (double)in7);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in8 := %s (%d)", adbgTest__SomeEnum__ToString(in8), (int)in8);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in9 := { ");
	for (size_t i1 = 0; i1 < in9Qty; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &in9Array[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != in9Qty - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in10 := { ");
	_adbgTest__Empty2(_ctx, in10);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "in11 := { ");
	_adbgTest__New(_ctx, in11);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}

void adbgTestOtherLogOut(acpCtx_t _ctx, const struct Empty* out1, uint32_t out2, size_t out3Qty, const char* out3Array, const char* out4, bool out5, int out6, float out7, SomeEnum out8, size_t out9Qty, const struct Empty* out9Array, const struct Empty2* out10, const struct New* out11)
{
	adbgPrintLog(_ctx, "@TestOther Out Args : { ");

	adbgPrintLog(_ctx, "out1 := { ");
	_adbgTest__Empty(_ctx, out1);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out2 := %u", (unsigned int)out2);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out3 := '");
	for (size_t i1 = 0; i1 < out3Qty; i1++) {
		adbgPrintLog(_ctx, "%02X", out3Array[i1]);
	}
	adbgPrintLog(_ctx, "'O");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out4 := \"%s\"", out4);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out5 := %s", (out5 ? "true" : "false"));
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out6 := %d", (int)out6);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out7 := %.6f", (double)out7);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out8 := %s (%d)", adbgTest__SomeEnum__ToString(out8), (int)out8);
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out9 := { ");
	for (size_t i1 = 0; i1 < out9Qty; i1++) {
		adbgPrintLog(_ctx, "{ ");
		_adbgTest__Empty(_ctx, &out9Array[i1]);
		adbgPrintLog(_ctx, " }");
		if (i1 != out9Qty - 1) { adbgPrintLog(_ctx, ", "); }
	}
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out10 := { ");
	_adbgTest__Empty2(_ctx, out10);
	adbgPrintLog(_ctx, " }");
	adbgPrintLog(_ctx, ", ");
	adbgPrintLog(_ctx, "out11 := { ");
	_adbgTest__New(_ctx, out11);
	adbgPrintLog(_ctx, " }");

	adbgPrintLog(_ctx, " }");
	adbgPrintFormatLog(_ctx);
}
