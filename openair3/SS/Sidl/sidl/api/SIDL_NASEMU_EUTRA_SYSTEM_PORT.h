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
#include "SidlParts.h"
#include "SidlVals.h"
#include "TtcnCommon.h"
#include "SidlCommon_BcchConfig.h"
#include "SidlCommon_Bcch_BRConfig.h"
#include "SidlCommon_CcchDcchDtchConfig.h"
#include "SidlCommon_InitialCellPower.h"
#include "SidlCommon_PhysicalLayerConfigDL.h"
#include "SidlCommon_PhysicalLayerConfigUL.h"
#include "SidlCommon_RachProcedureConfig.h"
#include "SidlCommon_SciSchedulingConfig.h"
#include "SidlCommon_ServingCellConfig.h"
#include "SidlCommon_StaticCellInfo.h"
#include "SidlCommon_CellConfigRequest.h"

SIDL_BEGIN_C_INTERFACE

enum RRC_MSG_Request_Type_Sel {
	RRC_MSG_Request_Type_UNBOUND_VALUE = 0,
	RRC_MSG_Request_Type_Ccch = 1,
	RRC_MSG_Request_Type_Dcch = 2,
};

struct uint8_t_RRC_MSG_Request_Type_Ccch_Dynamic {
	size_t d;
	uint8_t* v;
};

struct uint8_t_RRC_MSG_Request_Type_Dcch_Dynamic {
	size_t d;
	uint8_t* v;
};

union RRC_MSG_Request_Type_Value {
	struct uint8_t_RRC_MSG_Request_Type_Ccch_Dynamic Ccch;
	struct uint8_t_RRC_MSG_Request_Type_Dcch_Dynamic Dcch;
};

struct RRC_MSG_Request_Type {
	enum RRC_MSG_Request_Type_Sel d;
	union RRC_MSG_Request_Type_Value v;
};

enum RRC_MSG_Indication_Type_Sel {
	RRC_MSG_Indication_Type_UNBOUND_VALUE = 0,
	RRC_MSG_Indication_Type_Ccch = 1,
	RRC_MSG_Indication_Type_Dcch = 2,
};

struct uint8_t_RRC_MSG_Indication_Type_Ccch_Dynamic {
	size_t d;
	uint8_t* v;
};

struct uint8_t_RRC_MSG_Indication_Type_Dcch_Dynamic {
	size_t d;
	uint8_t* v;
};

union RRC_MSG_Indication_Type_Value {
	struct uint8_t_RRC_MSG_Indication_Type_Ccch_Dynamic Ccch;
	struct uint8_t_RRC_MSG_Indication_Type_Dcch_Dynamic Dcch;
};

struct RRC_MSG_Indication_Type {
	enum RRC_MSG_Indication_Type_Sel d;
	union RRC_MSG_Indication_Type_Value v;
};

struct EUTRA_RRC_PDU_REQ {
	struct ReqAspCommonPart_Type Common;
	struct RRC_MSG_Request_Type RrcPdu;
};

struct EUTRA_RRC_PDU_IND {
	struct IndAspCommonPart_Type Common;
	struct RRC_MSG_Indication_Type RrcPdu;
};

SIDL_END_C_INTERFACE
