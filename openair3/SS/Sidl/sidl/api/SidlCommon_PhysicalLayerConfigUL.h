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

typedef UplinkPowerControlDedicatedSCell_r10_pathlossReferenceLinking_r10_e SCellPathlossReferenceLinking_Type;

struct PRACH_Config_v1310_prach_Config_v1310_Optional {
	bool d;
	struct PRACH_Config_v1310 v;
};

struct PRACH_Config_v1430_prach_Config_v1430_Optional {
	bool d;
	struct PRACH_Config_v1430 v;
};

struct PRACH_Config_R8andLater_Type {
	struct PRACH_Config prach_Config;
	struct PRACH_Config_v1310_prach_Config_v1310_Optional prach_Config_v1310;
	struct PRACH_Config_v1430_prach_Config_v1430_Optional prach_Config_v1430;
};

enum PRACH_Config_Type_Sel {
	PRACH_Config_Type_UNBOUND_VALUE = 0,
	PRACH_Config_Type_R8andLater = 1,
};

union PRACH_Config_Type_Value {
	struct PRACH_Config_R8andLater_Type R8andLater;
};

struct PRACH_Config_Type {
	enum PRACH_Config_Type_Sel d;
	union PRACH_Config_Type_Value v;
};

struct PUCCH_ConfigCommon_v1310_pucch_ConfigCommon_v1310_Optional {
	bool d;
	struct PUCCH_ConfigCommon_v1310 v;
};

struct PUCCH_ConfigCommon_R8andLater_Type {
	struct PUCCH_ConfigCommon pucch_ConfigCommon;
	struct PUCCH_ConfigCommon_v1310_pucch_ConfigCommon_v1310_Optional pucch_ConfigCommon_v1310;
};

enum PUCCH_ConfigCommon_Type_Sel {
	PUCCH_ConfigCommon_Type_UNBOUND_VALUE = 0,
	PUCCH_ConfigCommon_Type_R8andLater = 1,
};

union PUCCH_ConfigCommon_Type_Value {
	struct PUCCH_ConfigCommon_R8andLater_Type R8andLater;
};

struct PUCCH_ConfigCommon_Type {
	enum PUCCH_ConfigCommon_Type_Sel d;
	union PUCCH_ConfigCommon_Type_Value v;
};

struct PUCCH_ConfigDedicated_v1020_pucch_ConfigDedicated_v1020_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1020 v;
};

struct PUCCH_ConfigDedicated_v1130_pucch_ConfigDedicated_v1130_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1130 v;
};

struct PUCCH_ConfigDedicated_v1250_pucch_ConfigDedicated_v1250_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_v1250 v;
};

struct PUCCH_ConfigDedicated_R8andLater_Type {
	struct PUCCH_ConfigDedicated pucch_ConfigDedicated;
	struct PUCCH_ConfigDedicated_v1020_pucch_ConfigDedicated_v1020_Optional pucch_ConfigDedicated_v1020;
	struct PUCCH_ConfigDedicated_v1130_pucch_ConfigDedicated_v1130_Optional pucch_ConfigDedicated_v1130;
	struct PUCCH_ConfigDedicated_v1250_pucch_ConfigDedicated_v1250_Optional pucch_ConfigDedicated_v1250;
};

struct PUCCH_ConfigDedicated_R13andLater_Type {
	struct PUCCH_ConfigDedicated_r13 pucch_ConfigDedicated_r13;
};

enum PUCCH_ConfigDedicated_Type_Sel {
	PUCCH_ConfigDedicated_Type_UNBOUND_VALUE = 0,
	PUCCH_ConfigDedicated_Type_R8andLater = 1,
	PUCCH_ConfigDedicated_Type_R13andLater = 2,
};

union PUCCH_ConfigDedicated_Type_Value {
	struct PUCCH_ConfigDedicated_R8andLater_Type R8andLater;
	struct PUCCH_ConfigDedicated_R13andLater_Type R13andLater;
};

struct PUCCH_ConfigDedicated_Type {
	enum PUCCH_ConfigDedicated_Type_Sel d;
	union PUCCH_ConfigDedicated_Type_Value v;
};

struct PUCCH_ConfigCommon_Type_Common_Optional {
	bool d;
	struct PUCCH_ConfigCommon_Type v;
};

struct PUCCH_ConfigDedicated_Type_Dedicated_Optional {
	bool d;
	struct PUCCH_ConfigDedicated_Type v;
};

struct PUCCH_Configuration_Type {
	struct PUCCH_ConfigCommon_Type_Common_Optional Common;
	struct PUCCH_ConfigDedicated_Type_Dedicated_Optional Dedicated;
};

struct PUSCH_ConfigCommon_v1270_pusch_ConfigCommon_v1270_Optional {
	bool d;
	struct PUSCH_ConfigCommon_v1270 v;
};

struct PUSCH_ConfigCommon_v1310_pusch_ConfigCommon_v1310_Optional {
	bool d;
	struct PUSCH_ConfigCommon_v1310 v;
};

struct PUSCH_ConfigCommon_R8andLater_Type {
	struct PUSCH_ConfigCommon pusch_ConfigCommon;
	struct PUSCH_ConfigCommon_v1270_pusch_ConfigCommon_v1270_Optional pusch_ConfigCommon_v1270;
	struct PUSCH_ConfigCommon_v1310_pusch_ConfigCommon_v1310_Optional pusch_ConfigCommon_v1310;
};

enum PUSCH_ConfigCommon_Type_Sel {
	PUSCH_ConfigCommon_Type_UNBOUND_VALUE = 0,
	PUSCH_ConfigCommon_Type_R8andLater = 1,
};

union PUSCH_ConfigCommon_Type_Value {
	struct PUSCH_ConfigCommon_R8andLater_Type R8andLater;
};

struct PUSCH_ConfigCommon_Type {
	enum PUSCH_ConfigCommon_Type_Sel d;
	union PUSCH_ConfigCommon_Type_Value v;
};

struct PUSCH_ConfigDedicated_v1020_pusch_ConfigDedicated_v1020_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1020 v;
};

struct PUSCH_ConfigDedicated_v1130_pusch_ConfigDedicated_v1130_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1130 v;
};

struct PUSCH_ConfigDedicated_v1250_pusch_ConfigDedicated_v1250_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1250 v;
};

struct PUSCH_ConfigDedicated_R8andLater_Type {
	struct PUSCH_ConfigDedicated pusch_ConfigDedicated;
	struct PUSCH_ConfigDedicated_v1020_pusch_ConfigDedicated_v1020_Optional pusch_ConfigDedicated_v1020;
	struct PUSCH_ConfigDedicated_v1130_pusch_ConfigDedicated_v1130_Optional pusch_ConfigDedicated_v1130;
	struct PUSCH_ConfigDedicated_v1250_pusch_ConfigDedicated_v1250_Optional pusch_ConfigDedicated_v1250;
};

struct PUSCH_ConfigDedicated_v1430_pusch_ConfigDedicated_v1430_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_v1430 v;
};

struct PUSCH_ConfigDedicated_R13andLater_Type {
	struct PUSCH_ConfigDedicated_r13 pusch_ConfigDedicated_r13;
	struct PUSCH_ConfigDedicated_v1430_pusch_ConfigDedicated_v1430_Optional pusch_ConfigDedicated_v1430;
};

enum PUSCH_ConfigDedicated_Type_Sel {
	PUSCH_ConfigDedicated_Type_UNBOUND_VALUE = 0,
	PUSCH_ConfigDedicated_Type_R8andLater = 1,
	PUSCH_ConfigDedicated_Type_R13andLater = 2,
};

union PUSCH_ConfigDedicated_Type_Value {
	struct PUSCH_ConfigDedicated_R8andLater_Type R8andLater;
	struct PUSCH_ConfigDedicated_R13andLater_Type R13andLater;
};

struct PUSCH_ConfigDedicated_Type {
	enum PUSCH_ConfigDedicated_Type_Sel d;
	union PUSCH_ConfigDedicated_Type_Value v;
};

struct PUSCH_ConfigCommon_Type_Common_Optional {
	bool d;
	struct PUSCH_ConfigCommon_Type v;
};

struct PUSCH_ConfigDedicated_Type_Dedicated_Optional {
	bool d;
	struct PUSCH_ConfigDedicated_Type v;
};

struct PUSCH_Configuration_Type {
	struct PUSCH_ConfigCommon_Type_Common_Optional Common;
	struct PUSCH_ConfigDedicated_Type_Dedicated_Optional Dedicated;
};

enum SS_TimingAdvanceConfig_Type_Sel {
	SS_TimingAdvanceConfig_Type_UNBOUND_VALUE = 0,
	SS_TimingAdvanceConfig_Type_InitialValue = 1,
	SS_TimingAdvanceConfig_Type_Relative = 2,
};

union SS_TimingAdvanceConfig_Type_Value {
	RACH_TimingAdvance_Type InitialValue;
	TimingAdvanceIndex_Type Relative;
};

struct SS_TimingAdvanceConfig_Type {
	enum SS_TimingAdvanceConfig_Type_Sel d;
	union SS_TimingAdvanceConfig_Type_Value v;
};

enum SoundingRS_UL_ConfigCommon_Type_Sel {
	SoundingRS_UL_ConfigCommon_Type_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigCommon_Type_R8 = 1,
};

union SoundingRS_UL_ConfigCommon_Type_Value {
	struct SoundingRS_UL_ConfigCommon R8;
};

struct SoundingRS_UL_ConfigCommon_Type {
	enum SoundingRS_UL_ConfigCommon_Type_Sel d;
	union SoundingRS_UL_ConfigCommon_Type_Value v;
};

struct SoundingRS_UL_ConfigDedicated_soundingRS_UL_ConfigDedicated_Optional {
	bool d;
	struct SoundingRS_UL_ConfigDedicated v;
};

struct SoundingRS_UL_ConfigDedicated_v1020_soundingRS_UL_ConfigDedicated_v1020_Optional {
	bool d;
	struct SoundingRS_UL_ConfigDedicated_v1020 v;
};

struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_soundingRS_UL_ConfigDedicatedAperiodic_r10_Optional {
	bool d;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10 v;
};

struct SoundingRS_UL_ConfigDedicated_R8andLater_Type {
	struct SoundingRS_UL_ConfigDedicated_soundingRS_UL_ConfigDedicated_Optional soundingRS_UL_ConfigDedicated;
	struct SoundingRS_UL_ConfigDedicated_v1020_soundingRS_UL_ConfigDedicated_v1020_Optional soundingRS_UL_ConfigDedicated_v1020;
	struct SoundingRS_UL_ConfigDedicatedAperiodic_r10_soundingRS_UL_ConfigDedicatedAperiodic_r10_Optional soundingRS_UL_ConfigDedicatedAperiodic_r10;
};

enum SoundingRS_UL_ConfigDedicated_Type_Sel {
	SoundingRS_UL_ConfigDedicated_Type_UNBOUND_VALUE = 0,
	SoundingRS_UL_ConfigDedicated_Type_R8andLater = 1,
};

union SoundingRS_UL_ConfigDedicated_Type_Value {
	struct SoundingRS_UL_ConfigDedicated_R8andLater_Type R8andLater;
};

struct SoundingRS_UL_ConfigDedicated_Type {
	enum SoundingRS_UL_ConfigDedicated_Type_Sel d;
	union SoundingRS_UL_ConfigDedicated_Type_Value v;
};

struct SRS_UL_Config_Type {
	struct SoundingRS_UL_ConfigCommon_Type Common;
	struct SoundingRS_UL_ConfigDedicated_Type Dedicated;
};

struct SchedulingRequestConfig_v1020_schedulingRequestConfig_v1020_Optional {
	bool d;
	struct SchedulingRequestConfig_v1020 v;
};

struct SchedulingRequestConfig_R8andLater_Type {
	struct SchedulingRequestConfig schedulingRequestConfig;
	struct SchedulingRequestConfig_v1020_schedulingRequestConfig_v1020_Optional schedulingRequestConfig_v1020;
};

enum SchedulingRequestConfig_Type_Sel {
	SchedulingRequestConfig_Type_UNBOUND_VALUE = 0,
	SchedulingRequestConfig_Type_R8andLater = 1,
};

union SchedulingRequestConfig_Type_Value {
	struct SchedulingRequestConfig_R8andLater_Type R8andLater;
};

struct SchedulingRequestConfig_Type {
	enum SchedulingRequestConfig_Type_Sel d;
	union SchedulingRequestConfig_Type_Value v;
};

struct CQI_ReportConfig_v920_cqi_ReportConfig_v920_Optional {
	bool d;
	struct CQI_ReportConfig_v920 v;
};

struct CQI_ReportConfig_R8andLater_Type {
	struct CQI_ReportConfig cqi_ReportConfig;
	struct CQI_ReportConfig_v920_cqi_ReportConfig_v920_Optional cqi_ReportConfig_v920;
};

struct CQI_ReportConfig_v1130_cqi_ReportConfig_v1130_Optional {
	bool d;
	struct CQI_ReportConfig_v1130 v;
};

struct CQI_ReportConfig_v1250_cqi_ReportConfig_v1250_Optional {
	bool d;
	struct CQI_ReportConfig_v1250 v;
};

struct CQI_ReportConfig_R10andLater_Type {
	struct CQI_ReportConfig_r10 cqi_ReportConfig_r10;
	struct CQI_ReportConfig_v1130_cqi_ReportConfig_v1130_Optional cqi_ReportConfig_v1130;
	struct CQI_ReportConfig_v1250_cqi_ReportConfig_v1250_Optional cqi_ReportConfig_v1250;
};

enum CQI_ReportConfig_Type_Sel {
	CQI_ReportConfig_Type_UNBOUND_VALUE = 0,
	CQI_ReportConfig_Type_R8andLater = 1,
	CQI_ReportConfig_Type_R10andLater = 2,
};

union CQI_ReportConfig_Type_Value {
	struct CQI_ReportConfig_R8andLater_Type R8andLater;
	struct CQI_ReportConfig_R10andLater_Type R10andLater;
};

struct CQI_ReportConfig_Type {
	enum CQI_ReportConfig_Type_Sel d;
	union CQI_ReportConfig_Type_Value v;
};

struct UplinkPowerControlCommon_v1020_uplinkPowerControlCommon_v1020_Optional {
	bool d;
	struct UplinkPowerControlCommon_v1020 v;
};

struct UplinkPowerControlCommonSCell_v1130_uplinkPowerControlCommonSCell_v1130_Optional {
	bool d;
	struct UplinkPowerControlCommonSCell_v1130 v;
};

struct UplinkPowerControlCommonPSCell_r12_uplinkPowerControlCommonPSCell_r12_Optional {
	bool d;
	struct UplinkPowerControlCommonPSCell_r12 v;
};

struct UplinkPowerControlCommon_R8andLater_Type {
	struct UplinkPowerControlCommon uplinkPowerControlCommon;
	struct UplinkPowerControlCommon_v1020_uplinkPowerControlCommon_v1020_Optional uplinkPowerControlCommon_v1020;
	struct UplinkPowerControlCommonSCell_v1130_uplinkPowerControlCommonSCell_v1130_Optional uplinkPowerControlCommonSCell_v1130;
	struct UplinkPowerControlCommonPSCell_r12_uplinkPowerControlCommonPSCell_r12_Optional uplinkPowerControlCommonPSCell_r12;
};

enum UplinkPowerControlCommon_Type_Sel {
	UplinkPowerControlCommon_Type_UNBOUND_VALUE = 0,
	UplinkPowerControlCommon_Type_R8andLater = 1,
};

union UplinkPowerControlCommon_Type_Value {
	struct UplinkPowerControlCommon_R8andLater_Type R8andLater;
};

struct UplinkPowerControlCommon_Type {
	enum UplinkPowerControlCommon_Type_Sel d;
	union UplinkPowerControlCommon_Type_Value v;
};

struct UplinkPowerControlDedicated_v1020_uplinkPowerControlDedicated_v1020_Optional {
	bool d;
	struct UplinkPowerControlDedicated_v1020 v;
};

struct SCellPathlossReferenceLinking_Type_pathlossReferenceLinking_Optional {
	bool d;
	SCellPathlossReferenceLinking_Type v;
};

struct UplinkPowerControlDedicated_v1130_uplinkPowerControlDedicated_v1130_Optional {
	bool d;
	struct UplinkPowerControlDedicated_v1130 v;
};

struct UplinkPowerControlDedicated_v1250_uplinkPowerControlDedicated_v1250_Optional {
	bool d;
	struct UplinkPowerControlDedicated_v1250 v;
};

struct UplinkPowerControlDedicated_R8andLater_Type {
	struct UplinkPowerControlDedicated uplinkPowerControlDedicated;
	struct UplinkPowerControlDedicated_v1020_uplinkPowerControlDedicated_v1020_Optional uplinkPowerControlDedicated_v1020;
	struct SCellPathlossReferenceLinking_Type_pathlossReferenceLinking_Optional pathlossReferenceLinking;
	struct UplinkPowerControlDedicated_v1130_uplinkPowerControlDedicated_v1130_Optional uplinkPowerControlDedicated_v1130;
	struct UplinkPowerControlDedicated_v1250_uplinkPowerControlDedicated_v1250_Optional uplinkPowerControlDedicated_v1250;
};

enum UplinkPowerControlDedicated_Type_Sel {
	UplinkPowerControlDedicated_Type_UNBOUND_VALUE = 0,
	UplinkPowerControlDedicated_Type_R8andLater = 1,
};

union UplinkPowerControlDedicated_Type_Value {
	struct UplinkPowerControlDedicated_R8andLater_Type R8andLater;
};

struct UplinkPowerControlDedicated_Type {
	enum UplinkPowerControlDedicated_Type_Sel d;
	union UplinkPowerControlDedicated_Type_Value v;
};

struct PRACH_Config_Type_Prach_Optional {
	bool d;
	struct PRACH_Config_Type v;
};

struct PUCCH_Configuration_Type_Pucch_Optional {
	bool d;
	struct PUCCH_Configuration_Type v;
};

struct PUSCH_Configuration_Type_Pusch_Optional {
	bool d;
	struct PUSCH_Configuration_Type v;
};

struct SS_TimingAdvanceConfig_Type_TimingAdvance_Optional {
	bool d;
	struct SS_TimingAdvanceConfig_Type v;
};

struct SRS_UL_Config_Type_SRS_UL_Config_Optional {
	bool d;
	struct SRS_UL_Config_Type v;
};

struct SchedulingRequestConfig_Type_SR_Config_Optional {
	bool d;
	struct SchedulingRequestConfig_Type v;
};

struct CQI_ReportConfig_Type_CQI_ReportConfig_Optional {
	bool d;
	struct CQI_ReportConfig_Type v;
};

struct UplinkPowerControlCommon_Type_UplinkPowerControlCommon_Optional {
	bool d;
	struct UplinkPowerControlCommon_Type v;
};

struct UplinkPowerControlDedicated_Type_UplinkPowerControlDedicated_Optional {
	bool d;
	struct UplinkPowerControlDedicated_Type v;
};

struct PhysicalLayerConfigUL_Type {
	struct PRACH_Config_Type_Prach_Optional Prach;
	struct PUCCH_Configuration_Type_Pucch_Optional Pucch;
	struct PUSCH_Configuration_Type_Pusch_Optional Pusch;
	struct SS_TimingAdvanceConfig_Type_TimingAdvance_Optional TimingAdvance;
	struct SRS_UL_Config_Type_SRS_UL_Config_Optional SRS_UL_Config;
	struct SchedulingRequestConfig_Type_SR_Config_Optional SR_Config;
	struct CQI_ReportConfig_Type_CQI_ReportConfig_Optional CQI_ReportConfig;
	struct UplinkPowerControlCommon_Type_UplinkPowerControlCommon_Optional UplinkPowerControlCommon;
	struct UplinkPowerControlDedicated_Type_UplinkPowerControlDedicated_Optional UplinkPowerControlDedicated;
};

SIDL_END_C_INTERFACE
