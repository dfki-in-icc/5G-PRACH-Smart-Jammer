#include "srv_cell_info.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void free_srv_cell_info(srv_cell_info_t* src)
{
  // 3 5GS TAC
  free(src->five_gs_tac);

  // 4 Configured EPS TAC
  free(src->eps_tac);

  // 5 Served PLMN
  for(size_t i =0; i < src->sz_srv_plmn; ++i){
      free_srv_plmn_f1ap(&src->srv_plmn[i]); 
  }

  free(src->srv_plmn);

  // 6 CHOICE NR-Mode-Info
  if(src->nr_mode_info == FDD_NR_MODE_INFO_F1AP ){
    free_fdd_info_f1ap(&src->fdd);
  } else if (src->nr_mode_info == TDD_NR_MODE_INFO_F1AP ){
    free_tdd_info_f1ap(&src->tdd);
  } else {
    assert(0!=0 && "Unknown type");
  }
 
  // 7 Measurement Timing Configuration. Defined at 3gpp TS 38.331 (MeasurementTimingConfiguration)
  free_byte_array(src->meas_timing_conf);

  // 8 RANAC 
  free(src->ran_ac);

  // 9 Extended Served PLMN
  for(size_t i = 0; i < src->  sz_ext_srv_plmn; i++){
   free_srv_plmn_f1ap(&src->ext_srv_plmn[i]);
  }
  free(src->ext_srv_plmn);

  // 10 Cell Direction
  free(src->cell_dir);

  // 11 Cell Type
  free(src->cell_type);

  // 12 Broadcast PLMN Identity Info
  for(size_t i =0; i < src-> sz_brdcst_PLMN_id_info; ++i){
    free_brdcst_PLMN_id_info_f1ap(&src->brdcst_PLMN_id_info[i]); 
  }
  free(src->brdcst_PLMN_id_info); 

  // 13 Configured TAC Indication
  free(src->conf_tac_ind); // enum TRUE

  // 14 Aggressor gNB Set ID
  if(src->aggr_gnb_set_id != NULL){
    free_byte_array(*src->aggr_gnb_set_id);
    free(src->aggr_gnb_set_id);
  }

  // 15 Victim gNB Set ID
  if(src->victim_gnb_set_id != NULL){
    free_byte_array(*src->  victim_gnb_set_id);
    free(src->victim_gnb_set_id );
  }

  // 16 AB Info IAB-DU
  if(src-> ab_info_iab_du != NULL ){
    free_iab_info_iab_du_f1ap(src->ab_info_iab_du) ; 
    free(src->ab_info_iab_du);
  }

  // 17 SSB Positions In Burst
  if(src->ssb_pos_burst){
    free_ssb_pos_burst(src->ssb_pos_burst);
    free(src->ssb_pos_burst);
  }

  // 18 NR PRACH Configuration
  if(src->nr_prach_conf != NULL){
    free_nr_prach_conf(src->nr_prach_conf);
    free(src->nr_prach_conf);
  }
  
  // 19 SFN Offset
  if(src->sfn_offset != NULL ){
    free_byte_array(*src->sfn_offset);
    free(src->sfn_offset);
  }
}

bool eq_srv_cell_info( srv_cell_info_t const* m0, srv_cell_info_t const* m1)
{
  assert(m0 != NULL);
  assert(m1 != NULL);

  // 1 NR CGI
  if(eq_nr_cgi(&m0->nr_cgi, &m1->nr_cgi) == false)
    return false;

  // 2 NR PCI
  assert(m0->nr_pci < 1008);
  assert(m1->nr_pci < 1008);
  if(m0->nr_pci != m1->nr_pci) // [0, 1007]
    return false;

  // 3 5GS TAC
  if(m0->five_gs_tac != m1->five_gs_tac){
    if(m0->five_gs_tac == NULL) 
      return false;
    if(m1->five_gs_tac == NULL) 
      return false;
    if(*m0->five_gs_tac != *m1->five_gs_tac) 
      return false;
  }

  // 4 Configured EPS TAC
  if(m0->eps_tac != m1->eps_tac){
    if(m0->eps_tac == NULL)
      return false;
    if(m1->eps_tac == NULL)
      return false;
    if(*m0->eps_tac != *m1->eps_tac)
      return false;
  }

  // 5 Served PLMN
  if(m0->sz_srv_plmn != m1->sz_srv_plmn)
    return false;

  for(size_t i = 0; i < m0->sz_srv_plmn; ++i ){
    if(eq_srv_plmn_f1ap(&m0->srv_plmn[i], &m1->srv_plmn[i]) == false)
      return false;
  }

  // 6 CHOICE NR-Mode-Info
  assert(m0->nr_mode_info == FDD_NR_MODE_INFO_F1AP || m0->nr_mode_info == TDD_NR_MODE_INFO_F1AP );
  assert(m1->nr_mode_info == FDD_NR_MODE_INFO_F1AP || m1->nr_mode_info == TDD_NR_MODE_INFO_F1AP );
  if (m0->nr_mode_info != m1->nr_mode_info)
    return false;

  if(m0->nr_mode_info == FDD_NR_MODE_INFO_F1AP){
    if(eq_fdd_info_f1ap(&m0->fdd, &m1->fdd) == false)
      return false;
  } else if(m0->nr_mode_info == TDD_NR_MODE_INFO_F1AP){
    if(eq_tdd_info_f1ap(&m0->tdd, &m1->tdd) == false)
      return false;
  } else {
    assert(0 != 0 && "Not supported type");
  }

  // 7 Measurement Timing Configuration. Defined at 3gpp TS 38.331 (MeasurementTimingConfiguration)
  if(eq_byte_array(&m0->meas_timing_conf, &m1->meas_timing_conf) == false )
    return false;

  // 8 RANAC 
  if(m0->ran_ac != m1->ran_ac){
    if(m0->ran_ac == NULL)
      return false;
    if(m1->ran_ac == NULL)
      return false;
    if(*m0->ran_ac != *m1->ran_ac)
      return false;
  }

  // 9 Extended Served PLMN
  if(m0->sz_ext_srv_plmn != m1->sz_ext_srv_plmn)
    return false;
  for(size_t i = 0; i < m0->sz_ext_srv_plmn; ++i){
    if(eq_srv_plmn_f1ap(&m0->ext_srv_plmn[i], &m1->ext_srv_plmn[i] ) == false)
      return false;
  }

  // 10 Cell Direction
  assert(m0->cell_dir == NULL && "Not implemented" );
  assert(m1->cell_dir == NULL && "Not implemented" );

  // 11 Cell Type
  assert(m0->cell_type == NULL && "Not implemented" );
  assert(m1->cell_type == NULL && "Not implemented" );

  // 12 Broadcast PLMN Identity Info
  if(m0->sz_brdcst_PLMN_id_info != m1->sz_brdcst_PLMN_id_info)
    return false;
  for(size_t i = 0; i < m0->sz_brdcst_PLMN_id_info; ++i){
    if(eq_brdcst_PLMN_id_info_f1ap(m0->brdcst_PLMN_id_info, m1->brdcst_PLMN_id_info  ) == false)
      return false;
  }

  // 13 Configured TAC Indication
  //e_F1AP_ConfiguredTACIndication* conf_tac_ind; // enum TRUE
  assert(m0->conf_tac_ind == NULL && "Not implemented" );
  assert(m1->conf_tac_ind == NULL && "Not implemented" );

  // 14 Aggressor gNB Set ID
  if(eq_byte_array(m0->aggr_gnb_set_id, m1->aggr_gnb_set_id) == false)
    return false;

  // 15 Victim gNB Set ID
  if(eq_byte_array(m0->victim_gnb_set_id, m1->victim_gnb_set_id) == false) 
    return false;
  
  // 16 AB Info IAB-DU
  if(eq_iab_info_iab_du_f1ap(m0->ab_info_iab_du, m1->ab_info_iab_du) == false)
    return false;

  // 17 SSB Positions In Burst
  if(eq_ssb_pos_burst(m0->ssb_pos_burst, m1->ssb_pos_burst) == false)
    return false;

  // 18 NR PRACH Configuration
  if(eq_nr_prach_conf(m0->nr_prach_conf, m1->nr_prach_conf) == false)
    return false;
  
  // 19 SFN Offset
  if(eq_byte_array(m0->sfn_offset, m1->sfn_offset) == false) 
    return false;

  return true;
}

