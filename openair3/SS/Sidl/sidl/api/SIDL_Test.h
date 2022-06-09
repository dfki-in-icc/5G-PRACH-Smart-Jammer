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

#include "SidlCompiler.h"

SIDL_BEGIN_C_INTERFACE

struct Complex {
	int val;
};

struct Empty {
	char dummy;
};

struct Empty2 {
	char dummy;
	int simple[3];
	struct Complex complex[3];
};

struct EchoData {
	char* str;
	size_t emptyQty;
	struct Empty* emptyArray;
	struct Empty* eee;
	struct Empty sss;
};

struct char_Foo_Dynamic {
	size_t d;
	char* v;
};

struct char_Foo_DynamicOptional {
	bool d;
	struct char_Foo_Dynamic v;
};

struct char_Koo_ArrayOptional {
	bool d;
	char v[25];
};

struct int_Bar_Optional {
	bool d;
	int v;
};

struct char_Zoo_Dynamic {
	size_t d;
	char* v;
};

struct Empty_Far_Dynamic {
	size_t d;
	struct Empty* v;
};

struct Output {
	struct char_Foo_DynamicOptional Foo;
	struct char_Koo_ArrayOptional Koo;
	struct int_Bar_Optional Bar;
	struct char_Zoo_Dynamic Zoo;
	size_t ZQty;
	char* ZArray;
	struct Empty_Far_Dynamic Far;
};

enum TestUnion_Sel {
	TestUnion_Zero = 0,
	TestUnion_One = 1,
	TestUnion_Two = 2,
	TestUnion_Three = 3,
};

union TestUnion_Value {
	int Zero;
	int One;
	struct Empty Two;
	struct Empty Three;
};

struct TestUnion {
	enum TestUnion_Sel d;
	union TestUnion_Value v;
};

struct Empty_dynamic_struct_Dynamic {
	size_t d;
	struct Empty* v;
};

struct int_dynamic_ints_Dynamic {
	size_t d;
	int* v;
};

struct Empty_optional_struct_1_Optional {
	bool d;
	struct Empty v;
};

struct Empty_optional_struct_2_Optional {
	bool d;
	struct Empty v;
};

struct int_optional_int_1_Optional {
	bool d;
	int v;
};

struct int_optional_int_2_Optional {
	bool d;
	int v;
};

struct char_optional_string_1_Optional {
	bool d;
	char* v;
};

struct char_optional_string_2_Optional {
	bool d;
	char* v;
};

struct Empty_optional_struct_array_1_ArrayOptional {
	bool d;
	struct Empty v[2];
};

struct Empty_optional_struct_array_2_ArrayOptional {
	bool d;
	struct Empty v[2];
};

struct int_optional_int_array_1_ArrayOptional {
	bool d;
	int v[2];
};

struct int_optional_int_array_2_ArrayOptional {
	bool d;
	int v[2];
};

struct Empty_dynamic_optional_struct_1_Dynamic {
	size_t d;
	struct Empty* v;
};

struct Empty_dynamic_optional_struct_1_DynamicOptional {
	bool d;
	struct Empty_dynamic_optional_struct_1_Dynamic v;
};

struct Empty_dynamic_optional_struct_2_Dynamic {
	size_t d;
	struct Empty* v;
};

struct Empty_dynamic_optional_struct_2_DynamicOptional {
	bool d;
	struct Empty_dynamic_optional_struct_2_Dynamic v;
};

struct int_dynamic_optional_int_1_Dynamic {
	size_t d;
	int* v;
};

struct int_dynamic_optional_int_1_DynamicOptional {
	bool d;
	struct int_dynamic_optional_int_1_Dynamic v;
};

struct int_dynamic_optional_int_2_Dynamic {
	size_t d;
	int* v;
};

struct int_dynamic_optional_int_2_DynamicOptional {
	bool d;
	struct int_dynamic_optional_int_2_Dynamic v;
};

struct TestUnion_optional_union_1_Optional {
	bool d;
	struct TestUnion v;
};

struct TestUnion_optional_union_pointer_1_Optional {
	bool d;
	struct TestUnion* v;
};

struct TestUnion_optional_union_2_Optional {
	bool d;
	struct TestUnion v;
};

struct TestUnion_dynamic_optional_union_1_Dynamic {
	size_t d;
	struct TestUnion* v;
};

struct TestUnion_dynamic_optional_union_1_DynamicOptional {
	bool d;
	struct TestUnion_dynamic_optional_union_1_Dynamic v;
};

struct TestUnion_dynamic_optional_union_2_Dynamic {
	size_t d;
	struct TestUnion* v;
};

struct TestUnion_dynamic_optional_union_2_DynamicOptional {
	bool d;
	struct TestUnion_dynamic_optional_union_2_Dynamic v;
};

struct New {
	struct Empty_dynamic_struct_Dynamic dynamic_struct;
	struct int_dynamic_ints_Dynamic dynamic_ints;
	struct Empty_optional_struct_1_Optional optional_struct_1;
	struct Empty_optional_struct_2_Optional optional_struct_2;
	struct int_optional_int_1_Optional optional_int_1;
	struct int_optional_int_2_Optional optional_int_2;
	struct char_optional_string_1_Optional optional_string_1;
	struct char_optional_string_2_Optional optional_string_2;
	struct Empty_optional_struct_array_1_ArrayOptional optional_struct_array_1;
	struct Empty_optional_struct_array_2_ArrayOptional optional_struct_array_2;
	struct int_optional_int_array_1_ArrayOptional optional_int_array_1;
	struct int_optional_int_array_2_ArrayOptional optional_int_array_2;
	struct Empty_dynamic_optional_struct_1_DynamicOptional dynamic_optional_struct_1;
	struct Empty_dynamic_optional_struct_2_DynamicOptional dynamic_optional_struct_2;
	struct int_dynamic_optional_int_1_DynamicOptional dynamic_optional_int_1;
	struct int_dynamic_optional_int_2_DynamicOptional dynamic_optional_int_2;
	struct TestUnion* union_test_pointer;
	struct TestUnion union_test;
	struct TestUnion_optional_union_1_Optional optional_union_1;
	struct TestUnion_optional_union_pointer_1_Optional optional_union_pointer_1;
	struct TestUnion_optional_union_2_Optional optional_union_2;
	struct TestUnion_dynamic_optional_union_1_DynamicOptional dynamic_optional_union_1;
	struct TestUnion_dynamic_optional_union_2_DynamicOptional dynamic_optional_union_2;
};

enum SomeEnum {
	SomeEnum_Zero = 0,
	SomeEnum_One = 1,
	SomeEnum_Two = 2,
	SomeEnum_Three = 3,
};

typedef enum SomeEnum SomeEnum;

SIDL_END_C_INTERFACE
