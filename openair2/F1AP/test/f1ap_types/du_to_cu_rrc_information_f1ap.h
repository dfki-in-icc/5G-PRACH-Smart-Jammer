#ifndef DU_TO_CU_RRC_INFORMATION_F1AP_MIR_H
#define DU_TO_CU_RRC_INFORMATION_F1AP_MIR_H

#include <stdbool.h>
#include <stdint.h>
#include "../byte_array.h"

typedef struct{

  // Mandatory
  // CellGroupConfig
  byte_array_t cell_group_config;

  // Optional
  // MeasGapConfig
  byte_array_t* meas_gap_info; 

  // Optional
  // Requested P-MaxFR1
  byte_array_t* req_p_max_fr1; 

  // Optional
  // DRX Long Cycle Start Offset
  uint32_t* drx_lng_cyc_str_off; // [0..10239]

  // Optional
  // Selected BandCombinationIndex
  byte_array_t* bnd_comb_idx; 

  // Optional
  // Selected FeatureSetEntryIndex
  byte_array_t* sel_feature_set_entry_idx; 

  // Optional
  // Ph-InfoSCG
  byte_array_t* ph_info_scg; 

  // Optional
  // Requested BandCombinationIndex
  byte_array_t* req_bnd_cmb_idx; 

  // Optional
  // Requested FeatureSetEntryIndex
  byte_array_t* req_feat_set_entry_idx; 

  // Optional
  // DRX Config
  byte_array_t* drx_cnfg; 

  // Optional
  // PDCCH BlindDetectionSCG
  byte_array_t* pdcch_blind_det_scg; 

  // Optional
  // Requested PDCCH BlindDetectionSCG
  byte_array_t* req_pdcch_blnd_det_scg; 

  // Optional
  // Ph-InfoMCG
  byte_array_t* ph_info_mcg; 

  // Optional
  // MeasGapSharingConfig 
  byte_array_t* meas_gap_shr_conf; 

  // Optional
  // SL-PHY-MAC-RLC-Config
  byte_array_t* sl_phy_mac_rlc_conf; 

  // Optional
  // SL-ConfigDedicatedEUTRA-Info 
  byte_array_t* sl_conf_dedica_eutra_info; 

  // Optional
  // Requested P-MaxFR2 
  byte_array_t* req_p_max_fr2; 

} du_to_cu_rrc_information_f1ap_t; 

void free_du_to_cu_rrc_information_f1ap(du_to_cu_rrc_information_f1ap_t* src);

bool eq_du_to_cu_rrc_information_f1ap(du_to_cu_rrc_information_f1ap_t const* m0, du_to_cu_rrc_information_f1ap_t const* m1);

#endif
