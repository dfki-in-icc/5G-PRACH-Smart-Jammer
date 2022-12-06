#include "cu_to_du_rrc_info.h"

#include <assert.h>

#include <stdlib.h>

static
void free_byte_array_if_not_null(byte_array_t* ba)
{
  if(ba == NULL)
    return;

  free_byte_array(*ba);
  free(ba);
}


void free_cu_to_du_rrc_info(cu_to_du_rrc_info_t* src)
{
  assert(src != NULL);

  // Optional
  // CG-ConfigInfo
  free_byte_array_if_not_null(src->cg_config_info);

  // Optional
  // UE-CapabilityRAT-ContainerList
  free_byte_array_if_not_null(src->ue_capability_rat_con_lst);

  // Optional
  // MeasConfig
  free_byte_array_if_not_null(src->meas_config);

  // Optional
  // Handover Preparation Information
  free_byte_array_if_not_null(src->handover_prep_info);

  // Optional
  // CellGroupConfig
  free_byte_array_if_not_null(src->cell_group_config);

  // Optional
  // Measurement Timing Configuration
  free_byte_array_if_not_null(src->meas_timing_conf);

  // Optional
  // UEAssistanceInformation
 free_byte_array_if_not_null(src-> ue_assistance_info);

  // Optional
  // CG-Config
  free_byte_array_if_not_null(src->cg_config);

  // Optional
  // UEAssistanceInformationEUTRA
  free_byte_array_if_not_null(src->UE_assistance_info_eutra);

}

bool eq_cu_to_du_rrc_info(cu_to_du_rrc_info_t const* m0, cu_to_du_rrc_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  // Optional
  // CG-ConfigInfo
  if(eq_byte_array(m0->cg_config_info, m1->cg_config_info) == false)
    return false;

  // Optional
  // UE-CapabilityRAT-ContainerList
  if(eq_byte_array(m0->ue_capability_rat_con_lst, m1->ue_capability_rat_con_lst) == false)
    return false;

  // Optional
  // MeasConfig
  if(eq_byte_array(m0->meas_config, m1->meas_config) == false)
    return false;

  // Optional
  // Handover Preparation Information
  if(eq_byte_array(m0->handover_prep_info, m1->handover_prep_info) == false)
    return false;

  // Optional
  // CellGroupConfig
 if(eq_byte_array(m0->cell_group_config, m1->cell_group_config) == false)
    return false;


  // Optional
  // Measurement Timing Configuration
 if(eq_byte_array(m0->meas_timing_conf, m1->meas_timing_conf) == false)
    return false;


  // Optional
  // UEAssistanceInformation
 if(eq_byte_array(m0->ue_assistance_info, m1->ue_assistance_info) == false)
    return false;


  // Optional
  // CG-Config
 if(eq_byte_array(m0->cg_config, m1->cg_config) == false)
    return false;


  // Optional
  // UEAssistanceInformationEUTRA
 if(eq_byte_array(m0->UE_assistance_info_eutra, m1->UE_assistance_info_eutra) == false)
    return false;

  return true;
}

