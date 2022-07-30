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
#include "SidlASN1.h"
#include "SidlASN1Base.h"
#include "SidlBase.h"
#include "SidlCommon.h"
#include "SidlCommonBase.h"

SIDL_BEGIN_C_INTERFACE

typedef int32_t DRB_Identity;

enum NR_RadioBearerId_Type_Sel {
	NR_RadioBearerId_Type_UNBOUND_VALUE = 0,
	NR_RadioBearerId_Type_Srb = 1,
	NR_RadioBearerId_Type_Drb = 2,
};

union NR_RadioBearerId_Type_Value {
	SRB_Identity_Type Srb;
	DRB_Identity Drb;
};

struct NR_RadioBearerId_Type {
	enum NR_RadioBearerId_Type_Sel d;
	union NR_RadioBearerId_Type_Value v;
};

enum NR_RoutingInfo_Type_Sel {
	NR_RoutingInfo_Type_UNBOUND_VALUE = 0,
	NR_RoutingInfo_Type_None = 1,
	NR_RoutingInfo_Type_RadioBearerId = 2,
	NR_RoutingInfo_Type_QosFlow = 3,
};

union NR_RoutingInfo_Type_Value {
	Null_Type None;
	struct NR_RadioBearerId_Type RadioBearerId;
	struct QosFlow_Identification_Type QosFlow;
};

struct NR_RoutingInfo_Type {
	enum NR_RoutingInfo_Type_Sel d;
	union NR_RoutingInfo_Type_Value v;
};

typedef Null_Type NR_RoutingInfoSUL_Type;

struct NR_RoutingInfoSUL_Type_RoutingInfoSUL_Optional {
	bool d;
	NR_RoutingInfoSUL_Type v;
};

struct NR_IndAspCommonPart_Type {
	NR_CellId_Type CellId;
	struct NR_RoutingInfo_Type RoutingInfo;
	struct NR_RoutingInfoSUL_Type_RoutingInfoSUL_Optional RoutingInfoSUL;
	struct RlcBearerRouting_Type RlcBearerRouting;
	struct TimingInfo_Type TimingInfo;
	struct IndicationStatus_Type Status;
};

struct NR_ReqAspCommonPart_Type {
	NR_CellId_Type CellId;
	struct NR_RoutingInfo_Type RoutingInfo;
	struct RlcBearerRouting_Type RlcBearerRouting;
	struct TimingInfo_Type TimingInfo;
	struct ReqAspControlInfo_Type ControlInfo;
};

SIDL_END_C_INTERFACE
