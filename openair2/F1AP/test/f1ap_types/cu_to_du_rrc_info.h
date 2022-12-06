#ifndef CU_TO_DU_RRC_INFORMATION_MIR_H
#define CU_TO_DU_RRC_INFORMATION_MIR_H 

#include "../byte_array.h"

// 9.3.1.25
typedef struct{

  // Optional
  // CG-ConfigInfo
  byte_array_t* cg_config_info;

  // Optional
  // UE-CapabilityRAT-ContainerList
  byte_array_t* ue_capability_rat_con_lst;

  // Optional
  // MeasConfig
  byte_array_t* meas_config;

  // Optional
  // Handover Preparation Information
  byte_array_t* handover_prep_info;

  // Optional
  // CellGroupConfig
  byte_array_t* cell_group_config;

  // Optional
  // Measurement Timing Configuration
  byte_array_t* meas_timing_conf;

  // Optional
  // UEAssistanceInformation
  byte_array_t* ue_assistance_info;

  // Optional
  // CG-Config
  byte_array_t* cg_config;

  // Optional
  // UEAssistanceInformationEUTRA
  byte_array_t* UE_assistance_info_eutra;

} cu_to_du_rrc_info_t ;

void free_cu_to_du_rrc_info(cu_to_du_rrc_info_t* src);

bool eq_cu_to_du_rrc_info(cu_to_du_rrc_info_t const* m0, cu_to_du_rrc_info_t const* m1);

#endif

