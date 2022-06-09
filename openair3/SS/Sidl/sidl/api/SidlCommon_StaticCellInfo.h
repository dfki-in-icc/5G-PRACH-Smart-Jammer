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

typedef CarrierBandwidthEUTRA_dl_Bandwidth_e Dl_Bandwidth_Type;

typedef CarrierBandwidthEUTRA_ul_Bandwidth_e Ul_Bandwidth_Type;

typedef Null EUTRA_FDD_Info_Type;

enum EUTRA_RBSize_Type {
	n_RB_SC_12 = 0,
	n_RB_SC_24 = 1,
};

typedef enum EUTRA_RBSize_Type EUTRA_RBSize_Type;

enum EUTRA_CyclicPrefix_Type {
	CyclicPrefix_normal = 0,
	CyclicPrefix_extended = 1,
};

typedef enum EUTRA_CyclicPrefix_Type EUTRA_CyclicPrefix_Type;

struct TDD_Config_v1130_tdd_Config_v1130_Optional {
	bool d;
	struct TDD_Config_v1130 v;
};

struct TDD_Config_R8andLater_Type {
	struct TDD_Config R8;
	struct TDD_Config_v1130_tdd_Config_v1130_Optional tdd_Config_v1130;
};

enum TDD_Config_Type_Sel {
	TDD_Config_Type_UNBOUND_VALUE = 0,
	TDD_Config_Type_R8andLater = 1,
};

union TDD_Config_Type_Value {
	struct TDD_Config_R8andLater_Type R8andLater;
};

struct TDD_Config_Type {
	enum TDD_Config_Type_Sel d;
	union TDD_Config_Type_Value v;
};

struct EUTRA_TDD_Info_Type {
	struct TDD_Config_Type Configuration;
};

struct EUTRA_HalfDuplexFDD_Info_Type {
};

enum EUTRA_RAT_Type_Sel {
	EUTRA_RAT_Type_UNBOUND_VALUE = 0,
	EUTRA_RAT_Type_FDD = 1,
	EUTRA_RAT_Type_TDD = 2,
	EUTRA_RAT_Type_HalfDuplexFDD = 3,
};

union EUTRA_RAT_Type_Value {
	EUTRA_FDD_Info_Type FDD;
	struct EUTRA_TDD_Info_Type TDD;
	struct EUTRA_HalfDuplexFDD_Info_Type HalfDuplexFDD;
};

struct EUTRA_RAT_Type {
	enum EUTRA_RAT_Type_Sel d;
	union EUTRA_RAT_Type_Value v;
};

struct uint8_t_TcOffset_Optional {
	bool d;
	uint8_t v;
};

struct CellTimingInfo_Type {
	struct uint8_t_TcOffset_Optional TcOffset;
	uint32_t Tcell;
	uint16_t SfnOffset;
	uint16_t HsfnOffset;
};

struct CellIdentity_eNB_CellId_Optional {
	bool d;
	CellIdentity v;
};

struct CommonStaticCellInfo_Type {
	struct EUTRA_RAT_Type RAT;
	PhysCellId PhysicalCellId;
	struct CellIdentity_eNB_CellId_Optional eNB_CellId;
	int32_t EutraBand;
	struct CellTimingInfo_Type CellTimingInfo;
};

struct DownlinkStaticCellInfo_Type {
	int32_t Earfcn;
	Dl_Bandwidth_Type Bandwidth;
	EUTRA_RBSize_Type RBSize;
	EUTRA_CyclicPrefix_Type CyclicPrefix;
};

struct UplinkStaticCellInfo_Type {
	int32_t Earfcn;
	Ul_Bandwidth_Type Bandwidth;
	EUTRA_CyclicPrefix_Type CyclicPrefix;
};

struct UplinkStaticCellInfo_Type_Uplink_Optional {
	bool d;
	struct UplinkStaticCellInfo_Type v;
};

struct StaticCellInfo_Type {
	struct CommonStaticCellInfo_Type Common;
	struct DownlinkStaticCellInfo_Type Downlink;
	struct UplinkStaticCellInfo_Type_Uplink_Optional Uplink;
};

SIDL_END_C_INTERFACE
