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

typedef uint8_t HarqProcessId_Type;

typedef uint16_t TimingAdvance_Period_Type;

typedef struct MAC_MainConfig_drx_Config_r13 DRX_Config_r13_Type;

typedef struct MAC_MainConfig_eDRX_Config_CycleStartOffset_r13 EDRX_CycleStartOffset_r13_Type;

typedef MAC_MainConfig_ul_SCH_Config_ttiBundling TTI_Bundling_Type;

typedef MAC_MainConfig_e_HARQ_Pattern_r12 E_HARQ_Pattern_r12_Type;

struct bool_mgta_Optional {
	bool d;
	bool v;
};

struct MeasGapConfig_R8andLater_Type {
	struct MeasGapConfig measGapConfig;
	struct bool_mgta_Optional mgta;
};

enum MeasGapConfig_Type_Sel {
	MeasGapConfig_Type_UNBOUND_VALUE = 0,
	MeasGapConfig_Type_R8andLater = 1,
};

union MeasGapConfig_Type_Value {
	struct MeasGapConfig_R8andLater_Type R8andLater;
};

struct MeasGapConfig_Type {
	enum MeasGapConfig_Type_Sel d;
	union MeasGapConfig_Type_Value v;
};

enum MeasGapCtrl_Type_Sel {
	MeasGapCtrl_Type_UNBOUND_VALUE = 0,
	MeasGapCtrl_Type_None = 1,
	MeasGapCtrl_Type_Config = 2,
};

union MeasGapCtrl_Type_Value {
	Null_Type None;
	struct MeasGapConfig_Type Config;
};

struct MeasGapCtrl_Type {
	enum MeasGapCtrl_Type_Sel d;
	union MeasGapCtrl_Type_Value v;
};

struct AntennaInfoDedicated_v920_antennaInfo_v920_Optional {
	bool d;
	struct AntennaInfoDedicated_v920 v;
};

struct AntennaInfoDedicated_R8andLater_Type {
	struct AntennaInfoDedicated antennaInfo;
	struct AntennaInfoDedicated_v920_antennaInfo_v920_Optional antennaInfo_v920;
};

struct AntennaInfoUL_r10_antennaInfoUL_Optional {
	bool d;
	struct AntennaInfoUL_r10 v;
};

struct AntennaInfoDedicated_v10i0_antennaInfoDedicated_v10i0_Optional {
	bool d;
	struct AntennaInfoDedicated_v10i0 v;
};

struct AntennaInfoDedicated_v1250_antennaInfoDedicated_v1250_Optional {
	bool d;
	struct AntennaInfoDedicated_v1250 v;
};

struct AntennaInfoDedicated_v1430_antennaInfoDedicated_v1430_Optional {
	bool d;
	struct AntennaInfoDedicated_v1430 v;
};

struct AntennaInfoDedicated_R10andLater_Type {
	struct AntennaInfoDedicated_r10 antennaInfo;
	struct AntennaInfoUL_r10_antennaInfoUL_Optional antennaInfoUL;
	struct AntennaInfoDedicated_v10i0_antennaInfoDedicated_v10i0_Optional antennaInfoDedicated_v10i0;
	struct AntennaInfoDedicated_v1250_antennaInfoDedicated_v1250_Optional antennaInfoDedicated_v1250;
	struct AntennaInfoDedicated_v1430_antennaInfoDedicated_v1430_Optional antennaInfoDedicated_v1430;
};

enum AntennaInfoDedicated_Type_Sel {
	AntennaInfoDedicated_Type_UNBOUND_VALUE = 0,
	AntennaInfoDedicated_Type_R8andLater = 1,
	AntennaInfoDedicated_Type_R10andLater = 2,
};

union AntennaInfoDedicated_Type_Value {
	struct AntennaInfoDedicated_R8andLater_Type R8andLater;
	struct AntennaInfoDedicated_R10andLater_Type R10andLater;
};

struct AntennaInfoDedicated_Type {
	enum AntennaInfoDedicated_Type_Sel d;
	union AntennaInfoDedicated_Type_Value v;
};

struct HarqProcessId_Type_HarqProcessList_Type_Dynamic {
	size_t d;
	HarqProcessId_Type* v;
};

typedef struct HarqProcessId_Type_HarqProcessList_Type_Dynamic HarqProcessList_Type;

enum HarqProcessConfigDL_Type_Sel {
	HarqProcessConfigDL_Type_UNBOUND_VALUE = 0,
	HarqProcessConfigDL_Type_AllProcesses = 1,
	HarqProcessConfigDL_Type_SpecificSubset = 2,
};

union HarqProcessConfigDL_Type_Value {
	Null_Type AllProcesses;
	HarqProcessList_Type SpecificSubset;
};

struct HarqProcessConfigDL_Type {
	enum HarqProcessConfigDL_Type_Sel d;
	union HarqProcessConfigDL_Type_Value v;
};

struct DciDlInfo_Type_DciInfo_Optional {
	bool d;
	struct DciDlInfo_Type v;
};

struct AntennaInfoDedicated_Type_AntennaInfo_Optional {
	bool d;
	struct AntennaInfoDedicated_Type v;
};

struct HarqProcessConfigDL_Type_HarqProcessConfig_Optional {
	bool d;
	struct HarqProcessConfigDL_Type v;
};

struct CcchDcchDtchConfigDL_Type {
	struct DciDlInfo_Type_DciInfo_Optional DciInfo;
	struct AntennaInfoDedicated_Type_AntennaInfo_Optional AntennaInfo;
	struct HarqProcessConfigDL_Type_HarqProcessConfig_Optional HarqProcessConfig;
};

enum TransmissionRepetition_Type_Sel {
	TransmissionRepetition_Type_UNBOUND_VALUE = 0,
	TransmissionRepetition_Type_Continuous = 1,
	TransmissionRepetition_Type_NumOfCycles = 2,
};

union TransmissionRepetition_Type_Value {
	Null_Type Continuous;
	int32_t NumOfCycles;
};

struct TransmissionRepetition_Type {
	enum TransmissionRepetition_Type_Sel d;
	union TransmissionRepetition_Type_Value v;
};

struct UplinkTimeAlignment_AutoSynch_Type {
	TimingAdvanceIndex_Type TimingAdvance;
	TimingAdvance_Period_Type TA_Period;
	struct TransmissionRepetition_Type TA_Repetition;
};

enum UplinkTimeAlignment_Synch_Type_Sel {
	UplinkTimeAlignment_Synch_Type_UNBOUND_VALUE = 0,
	UplinkTimeAlignment_Synch_Type_None = 1,
	UplinkTimeAlignment_Synch_Type_Auto = 2,
};

union UplinkTimeAlignment_Synch_Type_Value {
	Null_Type None;
	struct UplinkTimeAlignment_AutoSynch_Type Auto;
};

struct UplinkTimeAlignment_Synch_Type {
	enum UplinkTimeAlignment_Synch_Type_Sel d;
	union UplinkTimeAlignment_Synch_Type_Value v;
};

enum ULGrant_Period_Type_Sel {
	ULGrant_Period_Type_UNBOUND_VALUE = 0,
	ULGrant_Period_Type_OnlyOnce = 1,
	ULGrant_Period_Type_Duration = 2,
};

union ULGrant_Period_Type_Value {
	Null_Type OnlyOnce;
	int32_t Duration;
};

struct ULGrant_Period_Type {
	enum ULGrant_Period_Type_Sel d;
	union ULGrant_Period_Type_Value v;
};

struct PeriodicGrant_Type {
	struct ULGrant_Period_Type Period;
	struct TransmissionRepetition_Type NoOfRepetitions;
};

enum UL_GrantConfig_Type_Sel {
	UL_GrantConfig_Type_UNBOUND_VALUE = 0,
	UL_GrantConfig_Type_OnSR_Reception = 1,
	UL_GrantConfig_Type_Periodic = 2,
	UL_GrantConfig_Type_PeriodicOnSR_Reception = 3,
	UL_GrantConfig_Type_None = 4,
};

union UL_GrantConfig_Type_Value {
	Null_Type OnSR_Reception;
	struct PeriodicGrant_Type Periodic;
	struct PeriodicGrant_Type PeriodicOnSR_Reception;
	Null_Type None;
};

struct UL_GrantConfig_Type {
	enum UL_GrantConfig_Type_Sel d;
	union UL_GrantConfig_Type_Value v;
};

struct DciUlInfo_Type_DciInfo_Optional {
	bool d;
	struct DciUlInfo_Type v;
};

struct UplinkHoppingControl_Type_Hopping_Optional {
	bool d;
	struct UplinkHoppingControl_Type v;
};

struct UplinkTimeAlignment_Synch_Type_PUCCH_Synch_Optional {
	bool d;
	struct UplinkTimeAlignment_Synch_Type v;
};

struct UL_GrantConfig_Type_UL_GrantConfig_Optional {
	bool d;
	struct UL_GrantConfig_Type v;
};

struct CcchDcchDtchConfigUL_Type {
	struct DciUlInfo_Type_DciInfo_Optional DciInfo;
	struct UplinkHoppingControl_Type_Hopping_Optional Hopping;
	struct UplinkTimeAlignment_Synch_Type_PUCCH_Synch_Optional PUCCH_Synch;
	struct UL_GrantConfig_Type_UL_GrantConfig_Optional UL_GrantConfig;
};

struct DRX_Config_R8_Optional {
	bool d;
	struct DRX_Config v;
};

struct DRX_Config_r13_Type_R13_Optional {
	bool d;
	DRX_Config_r13_Type v;
};

struct EDRX_CycleStartOffset_r13_Type_EDRX_CycleStartOffset_r13_Optional {
	bool d;
	EDRX_CycleStartOffset_r13_Type v;
};

struct DRX_Config_R8andLater_Type {
	struct DRX_Config_R8_Optional R8;
	struct DRX_Config_r13_Type_R13_Optional R13;
	struct EDRX_CycleStartOffset_r13_Type_EDRX_CycleStartOffset_r13_Optional EDRX_CycleStartOffset_r13;
};

enum DRX_Config_Type_Sel {
	DRX_Config_Type_UNBOUND_VALUE = 0,
	DRX_Config_Type_R8andLater = 1,
};

union DRX_Config_Type_Value {
	struct DRX_Config_R8andLater_Type R8andLater;
};

struct DRX_Config_Type {
	enum DRX_Config_Type_Sel d;
	union DRX_Config_Type_Value v;
};

enum DrxCtrl_Type_Sel {
	DrxCtrl_Type_UNBOUND_VALUE = 0,
	DrxCtrl_Type_None = 1,
	DrxCtrl_Type_Config = 2,
};

union DrxCtrl_Type_Value {
	Null_Type None;
	struct DRX_Config_Type Config;
};

struct DrxCtrl_Type {
	enum DrxCtrl_Type_Sel d;
	union DrxCtrl_Type_Value v;
};

struct E_HARQ_Pattern_r12_Type_E_HARQ_Pattern_Optional {
	bool d;
	E_HARQ_Pattern_r12_Type v;
};

struct TTI_BundlingConfig_R8andLater_Type {
	TTI_Bundling_Type TTI_BundlingEnabled;
	struct E_HARQ_Pattern_r12_Type_E_HARQ_Pattern_Optional E_HARQ_Pattern;
};

enum TTI_BundlingConfig_Type_Sel {
	TTI_BundlingConfig_Type_UNBOUND_VALUE = 0,
	TTI_BundlingConfig_Type_R8 = 1,
	TTI_BundlingConfig_Type_R8andLater = 2,
};

union TTI_BundlingConfig_Type_Value {
	bool R8;
	struct TTI_BundlingConfig_R8andLater_Type R8andLater;
};

struct TTI_BundlingConfig_Type {
	enum TTI_BundlingConfig_Type_Sel d;
	union TTI_BundlingConfig_Type_Value v;
};

struct MeasGapCtrl_Type_MeasGapCtrl_Optional {
	bool d;
	struct MeasGapCtrl_Type v;
};

struct CcchDcchDtchConfigDL_Type_DL_Optional {
	bool d;
	struct CcchDcchDtchConfigDL_Type v;
};

struct CcchDcchDtchConfigUL_Type_UL_Optional {
	bool d;
	struct CcchDcchDtchConfigUL_Type v;
};

struct DrxCtrl_Type_DrxCtrl_Optional {
	bool d;
	struct DrxCtrl_Type v;
};

struct TTI_BundlingConfig_Type_TtiBundling_Optional {
	bool d;
	struct TTI_BundlingConfig_Type v;
};

struct bool_CifPresence_Optional {
	bool d;
	bool v;
};

struct CcchDcchDtchConfig_Type {
	struct MeasGapCtrl_Type_MeasGapCtrl_Optional MeasGapCtrl;
	struct CcchDcchDtchConfigDL_Type_DL_Optional DL;
	struct CcchDcchDtchConfigUL_Type_UL_Optional UL;
	struct DrxCtrl_Type_DrxCtrl_Optional DrxCtrl;
	struct TTI_BundlingConfig_Type_TtiBundling_Optional TtiBundling;
	struct bool_CifPresence_Optional CifPresence;
};

SIDL_END_C_INTERFACE
