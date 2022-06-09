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

SIDL_BEGIN_C_INTERFACE

typedef struct PhysicalConfigDedicated_ce_Mode_r13 CE_Mode_r13_Type;

enum CA_CellInitialConfig_Type {
	CellInitialConfig_PCell = 0,
	CellInitialConfig_Scell_Active = 1,
	CellInitialConfig_Scell_Inactive = 2,
	CellInitialConfig_PScell = 3,
};

typedef enum CA_CellInitialConfig_Type CA_CellInitialConfig_Type;

struct DciDlInfoCommon_Type_PcchConfig_Type_DciInfo_Optional {
	bool d;
	struct DciDlInfoCommon_Type v;
};

struct PCCH_Config_v1310_PCCH_Config_v13_Optional {
	bool d;
	struct PCCH_Config_v1310 v;
};

struct PcchConfig_Type {
	struct DciDlInfoCommon_Type_PcchConfig_Type_DciInfo_Optional DciInfo;
	struct PCCH_Config_v1310_PCCH_Config_v13_Optional PCCH_Config_v13;
};

struct EIMTA_Config_Type {
	struct EIMTA_MainConfig_r12 EIMTA_MainConfig_r12;
	struct EIMTA_MainConfigServCell_r12 EIMTA_MainConfigServCell_r12;
};

struct CellConfigCapability_Type_ConfigCapability_Optional {
	bool d;
	CellConfigCapability_Type v;
};

struct StaticCellInfo_Type_StaticCellInfo_Optional {
	bool d;
	struct StaticCellInfo_Type v;
};

struct PhysicalLayerConfigDL_Type_PhysicalLayerConfigDL_Optional {
	bool d;
	struct PhysicalLayerConfigDL_Type v;
};

struct InitialCellPower_Type_InitialCellPower_Optional {
	bool d;
	struct InitialCellPower_Type v;
};

struct BcchConfig_Type_BcchConfig_Optional {
	bool d;
	struct BcchConfig_Type v;
};

struct PcchConfig_Type_PcchConfig_Optional {
	bool d;
	struct PcchConfig_Type v;
};

struct CA_CellInitialConfig_Type_CA_CellInitialConfig_Optional {
	bool d;
	CA_CellInitialConfig_Type v;
};

struct Bcch_BRConfig_Type_BRBcchConfig_Optional {
	bool d;
	struct Bcch_BRConfig_Type v;
};

struct LAA_SCellConfiguration_r13_LAA_SCellConfiguration_Optional {
	bool d;
	struct LAA_SCellConfiguration_r13 v;
};

struct LAA_SCellConfiguration_v1430_LAA_SCellConfiguration_v1430_Optional {
	bool d;
	struct LAA_SCellConfiguration_v1430 v;
};

struct BasicCellConfig_Type {
	struct CellConfigCapability_Type_ConfigCapability_Optional ConfigCapability;
	struct StaticCellInfo_Type_StaticCellInfo_Optional StaticCellInfo;
	struct PhysicalLayerConfigDL_Type_PhysicalLayerConfigDL_Optional PhysicalLayerConfigDL;
	struct InitialCellPower_Type_InitialCellPower_Optional InitialCellPower;
	struct BcchConfig_Type_BcchConfig_Optional BcchConfig;
	struct PcchConfig_Type_PcchConfig_Optional PcchConfig;
	struct CA_CellInitialConfig_Type_CA_CellInitialConfig_Optional CA_CellInitialConfig;
	struct Bcch_BRConfig_Type_BRBcchConfig_Optional BRBcchConfig;
	struct LAA_SCellConfiguration_r13_LAA_SCellConfiguration_Optional LAA_SCellConfiguration;
	struct LAA_SCellConfiguration_v1430_LAA_SCellConfiguration_v1430_Optional LAA_SCellConfiguration_v1430;
};

struct C_RNTI_C_RNTI_Optional {
	bool d;
	C_RNTI v;
};

struct PhysicalLayerConfigUL_Type_PhysicalLayerConfigUL_Optional {
	bool d;
	struct PhysicalLayerConfigUL_Type v;
};

struct RachProcedureConfig_Type_RachProcedureConfig_Optional {
	bool d;
	struct RachProcedureConfig_Type v;
};

struct CcchDcchDtchConfig_Type_CcchDcchDtchConfig_Optional {
	bool d;
	struct CcchDcchDtchConfig_Type v;
};

struct ServingCellConfig_Type_ServingCellConfig_Optional {
	bool d;
	struct ServingCellConfig_Type v;
};

struct SciSchedulingConfig_Type_SciSchedulingConfig_Optional {
	bool d;
	struct SciSchedulingConfig_Type v;
};

struct EIMTA_Config_Type_EIMTA_Config_Optional {
	bool d;
	struct EIMTA_Config_Type v;
};

struct CE_Mode_r13_Type_CE_Mode_Optional {
	bool d;
	CE_Mode_r13_Type v;
};

struct ActiveCellConfig_Type {
	struct C_RNTI_C_RNTI_Optional C_RNTI;
	struct PhysicalLayerConfigUL_Type_PhysicalLayerConfigUL_Optional PhysicalLayerConfigUL;
	struct RachProcedureConfig_Type_RachProcedureConfig_Optional RachProcedureConfig;
	struct CcchDcchDtchConfig_Type_CcchDcchDtchConfig_Optional CcchDcchDtchConfig;
	struct ServingCellConfig_Type_ServingCellConfig_Optional ServingCellConfig;
	struct SciSchedulingConfig_Type_SciSchedulingConfig_Optional SciSchedulingConfig;
	struct EIMTA_Config_Type_EIMTA_Config_Optional EIMTA_Config;
	struct CE_Mode_r13_Type_CE_Mode_Optional CE_Mode;
};

struct BasicCellConfig_Type_Basic_Optional {
	bool d;
	struct BasicCellConfig_Type v;
};

struct ActiveCellConfig_Type_Active_Optional {
	bool d;
	struct ActiveCellConfig_Type v;
};

struct CellConfigInfo_Type {
	struct BasicCellConfig_Type_Basic_Optional Basic;
	struct ActiveCellConfig_Type_Active_Optional Active;
};

enum CellConfigRequest_Type_Sel {
	CellConfigRequest_Type_UNBOUND_VALUE = 0,
	CellConfigRequest_Type_AddOrReconfigure = 1,
	CellConfigRequest_Type_Release = 2,
};

union CellConfigRequest_Type_Value {
	struct CellConfigInfo_Type AddOrReconfigure;
	Null_Type Release;
};

struct CellConfigRequest_Type {
	enum CellConfigRequest_Type_Sel d;
	union CellConfigRequest_Type_Value v;
};

SIDL_END_C_INTERFACE
