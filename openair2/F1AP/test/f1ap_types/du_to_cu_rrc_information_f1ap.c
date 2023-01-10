#include "du_to_cu_rrc_information_f1ap.h"

#include <assert.h>
#include <stdlib.h>


static
void free_ba_if_not_null(byte_array_t* ba)
{
  if(ba != NULL){
    free_byte_array(*ba);
    free(ba);
  }

}


void free_du_to_cu_rrc_information_f1ap(du_to_cu_rrc_information_f1ap_t* src)
{
  assert(src != NULL);

  // Mandatory
  // CellGroupConfig
  free_byte_array(src->cell_group_config);

  // Optional
  // MeasGapConfig
  free_ba_if_not_null(src->meas_gap_info); 

  // Optional
  // Requested P-MaxFR1
  free_ba_if_not_null(src->req_p_max_fr1); 

  // Optional
  // DRX Long Cycle Start Offset
  if(src->drx_lng_cyc_str_off != NULL)  // [0..10239]
    free(src->drx_lng_cyc_str_off); 

  // Optional
  // Selected BandCombinationIndex
  free_ba_if_not_null(src->bnd_comb_idx); 

  // Optional
  // Selected FeatureSetEntryIndex
  free_ba_if_not_null(src->sel_feature_set_entry_idx); 

  // Optional
  // Ph-InfoSCG
  free_ba_if_not_null(src->ph_info_scg); 

  // Optional
  // Requested BandCombinationIndex
  free_ba_if_not_null(src->req_bnd_cmb_idx); 

  // Optional
  // Requested FeatureSetEntryIndex
  free_ba_if_not_null(src->req_feat_set_entry_idx); 

  // Optional
  // DRX Config
  free_ba_if_not_null(src->drx_cnfg); 

  // Optional
  // PDCCH BlindDetectionSCG
  free_ba_if_not_null(src->pdcch_blind_det_scg); 

  // Optional
  // Requested PDCCH BlindDetectionSCG
  free_ba_if_not_null(src->req_pdcch_blnd_det_scg); 

  // Optional
  // Ph-InfoMCG
  free_ba_if_not_null(src->ph_info_mcg); 

  // Optional
  // MeasGapSharingConfig 
  free_ba_if_not_null(src->meas_gap_shr_conf); 

  // Optional
  // SL-PHY-MAC-RLC-Config
  free_ba_if_not_null(src->sl_phy_mac_rlc_conf); 

  // Optional
  // SL-ConfigDedicatedEUTRA-Info 
  free_ba_if_not_null(src->sl_conf_dedica_eutra_info); 

  // Optional
  // Requested P-MaxFR2 
  free_ba_if_not_null(src->req_p_max_fr2); 

}

bool eq_du_to_cu_rrc_information_f1ap(du_to_cu_rrc_information_f1ap_t const* m0, du_to_cu_rrc_information_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // CellGroupConfig
  if(eq_byte_array(&m0->cell_group_config, &m1->cell_group_config) == false)
    return false;

  // Optional
  // MeasGapConfig
  if(eq_byte_array(m0->meas_gap_info, m1->meas_gap_info ) == false)
    return false;

  // Optional
  // Requested P-MaxFR1
  if(eq_byte_array(m0->req_p_max_fr1, m1->req_p_max_fr1) == false)
    return false;

  // Optional
  // DRX Long Cycle Start Offset // [0..10239]
  if( (m0->drx_lng_cyc_str_off == NULL && m1->drx_lng_cyc_str_off != NULL) || (m0->drx_lng_cyc_str_off != NULL && m1->drx_lng_cyc_str_off == NULL) ) {
    return false;
  } else if(m0->drx_lng_cyc_str_off != NULL && m1->drx_lng_cyc_str_off != NULL) {
    assert(*m0->drx_lng_cyc_str_off < 10240);
    assert(*m1->drx_lng_cyc_str_off < 10240);

    if(*m0->drx_lng_cyc_str_off != *m1->drx_lng_cyc_str_off)
      return false;

  } // else m0 == NULL && m1 == NULL 

  // Optional
  // Selected BandCombinationIndex
  if(eq_byte_array(m0-> bnd_comb_idx , m1-> bnd_comb_idx ) == false )
    return false;

  // Optional
  // Selected FeatureSetEntryIndex
  if(eq_byte_array(m0->sel_feature_set_entry_idx, m1->sel_feature_set_entry_idx) == false)
    return false;

  // Optional
  // Ph-InfoSCG
  if(eq_byte_array(m0->ph_info_scg , m1->ph_info_scg ) == false)
    return false;

  // Optional
  // Requested BandCombinationIndex
  if(eq_byte_array(m0->req_bnd_cmb_idx, m1->req_bnd_cmb_idx) == false)
    return false;

  // Optional
  // Requested FeatureSetEntryIndex
  if(eq_byte_array(m0-> req_feat_set_entry_idx , m1-> req_feat_set_entry_idx ) == false )
    return false;

  // Optional
  // DRX Config
  if(eq_byte_array(m0-> drx_cnfg , m1-> drx_cnfg ) == false )
    return false;

  // Optional
  // PDCCH BlindDetectionSCG
  if(eq_byte_array(m0->pdcch_blind_det_scg , m1->pdcch_blind_det_scg ) == false )
    return false;

  // Optional
  // Requested PDCCH BlindDetectionSCG
  if(eq_byte_array(m0-> req_pdcch_blnd_det_scg , m1-> req_pdcch_blnd_det_scg ) == false )
    return false;

  // Optional
  // Ph-InfoMCG
  if(eq_byte_array(m0->ph_info_mcg , m1->ph_info_mcg ) == false )
    return false;

  // Optional
  // MeasGapSharingConfig 
  if(eq_byte_array(m0->meas_gap_shr_conf , m1->meas_gap_shr_conf ) == false )
    return false;

  // Optional
  // SL-PHY-MAC-RLC-Config
  if(eq_byte_array(m0-> sl_phy_mac_rlc_conf , m1-> sl_phy_mac_rlc_conf ) == false )
    return false;

  // Optional
  // SL-ConfigDedicatedEUTRA-Info 
  if(eq_byte_array(m0->sl_conf_dedica_eutra_info , m1->sl_conf_dedica_eutra_info ) == false )
    return false;

  // Optional
  // Requested P-MaxFR2 
  if(eq_byte_array(m0->req_p_max_fr2 , m1->req_p_max_fr2) == false )
    return false;

  return true;
}

