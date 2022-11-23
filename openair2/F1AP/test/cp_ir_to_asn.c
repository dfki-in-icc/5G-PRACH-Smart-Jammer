#include "cp_ir_to_asn.h"

#include <assert.h>

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_InitiatingMessage.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SuccessfulOutcome.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Served-Cell-Information.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_F1SetupRequest.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolExtensionContainer.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SliceSupportList.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_NRFreqInfo.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_GNB-DU-Served-Cells-List.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_F1SetupResponse.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolIE-Field.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ServedPLMNs-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolExtensionField.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SliceSupportItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolIE-Field.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_TDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_FDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_FreqBandNrItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SupportedSULFreqBandItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SibtypetobeupdatedListItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_UnsuccessfulOutcome.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-List.h"


#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-Item.h"



#include "../../../openair3/UTILS/conversions.h"

#include "f1ap_types/fdd_info.h"
#include "f1ap_types/activate_cell.h"
#include "f1ap_types/gnb_du_srv_cell.h"
#include "f1ap_types/srv_cell_info.h"
#include "f1ap_types/gnb_rrc_ver.h"
#include "f1ap_types/nr_freq_info.h"


static inline
OCTET_STRING_t copy_ba_to_ostring(byte_array_t ba)
{
  OCTET_STRING_t os = { .size = ba.len }; 
  os.buf = malloc(ba.len);
  memcpy(os.buf, ba.buf, ba.len);
  return os;
}

static
BIT_STRING_t copy_bit_string(BIT_STRING_t const* src)
{
  assert(src != NULL);
  BIT_STRING_t dst = {.bits_unused = src->bits_unused, .size = src->size};
  dst.buf = calloc(src->size, sizeof(uint8_t));
  assert(dst.buf != NULL && "Memory exhausted");

  memcpy(dst.buf, src->buf, src->size);
  return dst;
}

static
F1AP_F1SetupRequestIEs_t* cp_trans_id(uint8_t src_trans_id)
{
  F1AP_F1SetupRequestIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupRequestIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_TransactionID;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupRequestIEs__value_PR_TransactionID;
  ie->value.choice.TransactionID = src_trans_id; 
  return ie;
}

static
F1AP_F1SetupRequestIEs_t* cp_gnb_du_id(uint64_t src_gnb_du_id)
{
  assert(src_gnb_du_id < (1UL << 36) && "Only 36 bits available"  );

  F1AP_F1SetupRequestIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupRequestIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_gNB_DU_ID;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupRequestIEs__value_PR_GNB_DU_ID;
  asn_uint642INTEGER(&ie->value.choice.GNB_DU_ID, src_gnb_du_id);
  return ie;
}

static
F1AP_NRFreqInfo_t cp_freq_info(nr_freq_info_f1ap_t const* src_frq_info)
  //, F1AP_NRFreqInfo_t* dst_frq_info)
{
  assert(src_frq_info != NULL);

  F1AP_NRFreqInfo_t dst = {0}; 

  // mandatory
  // 1.1 NR ARFCN 
  assert(src_frq_info-> nr_arfcn < (3279165 + 1) && "TS 38.473: 9.3.1.17 NR Frequency Info "); 

  dst.nRARFCN = src_frq_info->nr_arfcn;

  // optional 
  // 1.2 SUL Information
  assert(src_frq_info->sul_info == NULL && "Not implemented");

  // mandatory
  // 1.3 Frequency Band List
  assert(src_frq_info->sz_frq_band < 33);
  for(int i =0; i < src_frq_info->sz_frq_band; ++i){

    F1AP_FreqBandNrItem_t* dst_frq = calloc(1, sizeof(F1AP_FreqBandNrItem_t));
    assert(dst_frq != NULL && "Memory exhausted");

    // mandatory
    // 1.3.1 freqBandIndicatorNr 
    assert(src_frq_info->frq_band[i].frq_band < 1025);
    dst_frq->freqBandIndicatorNr = src_frq_info->frq_band[i].frq_band;

    // optional
    /* 1.3.2 supportedSULBandList*/
    if(src_frq_info->frq_band[i].sz_sul_band > 0){
      assert(src_frq_info->frq_band[i].sz_sul_band < 33);
      for(int j = 0; j < src_frq_info->frq_band[i].sz_sul_band; ++j ){
        F1AP_SupportedSULFreqBandItem_t* dst_sul_frq = calloc(1, sizeof(F1AP_SupportedSULFreqBandItem_t));
        assert(dst_sul_frq != NULL && "Memory exhausted");
        assert(src_frq_info->frq_band[i].sul_band[j] < 1025); 
        // mandatory
        // TDD.1.3.2.1 freqBandIndicatorNr 
        dst_sul_frq->freqBandIndicatorNr = src_frq_info->frq_band[i].sul_band[j];
        int const rc = ASN_SEQUENCE_ADD(&dst_frq->supportedSULBandList.list, dst_sul_frq);
        assert(rc == 0);
      }
    }
    // optional
    // Frequency Shift 7p5khz
    assert(src_frq_info->freq_shift == NULL && "NNot implemented"); 

    int const rc = ASN_SEQUENCE_ADD(&dst.freqBandListNr.list, dst_frq);
    assert(rc == 0);
  }
  return dst;
}

static
F1AP_NR_Mode_Info_t cp_tdd_info(tdd_info_f1ap_t const* src_tdd)
{
  assert(src_tdd != NULL);

  F1AP_NR_Mode_Info_t dst = {0}; //

  dst.present = F1AP_NR_Mode_Info_PR_tDD;
  dst.choice.tDD = calloc(1, sizeof(F1AP_TDD_Info_t) );
  assert(dst.choice.tDD != NULL && "Memory exhausted");
  F1AP_TDD_Info_t* dst_tdd = dst.choice.tDD; 

  // TDD.1 NR Frequency Info
  dst_tdd->nRFreqInfo = cp_freq_info(&src_tdd->freq_info); //, &dst_tdd->nRFreqInfo);

  // mandatory
  // TDD.2 transmission_Bandwidth 
  dst_tdd->transmission_Bandwidth.nRSCS = src_tdd->bw.nr_scs;
  dst_tdd->transmission_Bandwidth.nRNRB = src_tdd->bw.n_rb;

  // optional 
  //Intended TDD DL-UL Configuration
  assert(src_tdd->intended_tdd_dl_ul_conf == NULL && "Not Implemented");

  // optional
  //TDD UL-DL Configuration Common NR
  assert(src_tdd->tdd_ul_dl_conf_common == NULL && "Not Implemented" );

  // optional
  // Carrier List
  assert(src_tdd->carrier_lst == NULL && "Not implemented");

  return dst;
}

static
F1AP_NR_Mode_Info_t cp_fdd_info(fdd_info_f1ap_t const* src_fdd)
//, F1AP_Served_Cell_Information_t* dst_srv_cell_info)
{
  assert(src_fdd != NULL); 

  F1AP_NR_Mode_Info_t dst = {0};

  dst.present = F1AP_NR_Mode_Info_PR_fDD;
  dst.choice.fDD = calloc(1, sizeof(F1AP_FDD_Info_t) );
  assert(dst.choice.fDD != NULL && "Memory exhausted");
  F1AP_FDD_Info_t* dst_fdd = dst.choice.fDD; 

  // FDD.1 UL NR Frequency Info
  dst_fdd->uL_NRFreqInfo = cp_freq_info(&src_fdd->ul_frq_info); //, &dst_fdd->uL_NRFreqInfo);

  // FDD.2 DL NR Frequency Info
  dst_fdd->dL_NRFreqInfo = cp_freq_info(&src_fdd->dl_frq_info); //, &dst_fdd->dL_NRFreqInfo);

  // mandatory
  // FDD.3 transmission_Bandwidth 
  dst_fdd->uL_Transmission_Bandwidth.nRSCS = src_fdd->ul_bw.nr_scs;
  dst_fdd->uL_Transmission_Bandwidth.nRNRB = src_fdd->ul_bw.n_rb;

  // mandatory
  // FDD.4 transmission_Bandwidth 
  dst_fdd->dL_Transmission_Bandwidth.nRSCS = src_fdd->dl_bw.nr_scs;
  dst_fdd->dL_Transmission_Bandwidth.nRNRB = src_fdd->dl_bw.n_rb;

  // optional
  // UL Carrier List
  assert(src_fdd->ul_nr_carrier_lst == NULL && "Not implemented" );

  // optional
  // DL Carrier List
  assert(src_fdd->dl_nr_carrier_lst == NULL && "Not implemented" );

  return dst;
}

static
F1AP_SliceSupportItem_t* cp_slice_sup_lst(s_nssai_t const* src_ss)
{
  assert(src_ss != NULL);

  F1AP_SliceSupportItem_t* dst_tai_ss = calloc(1, sizeof(F1AP_SliceSupportItem_t)) ;
  assert(dst_tai_ss != NULL && "Memory exhausted");

  byte_array_t ba = {.len = sizeof(src_ss->sst), .buf = (uint8_t*)&src_ss->sst };
  dst_tai_ss->sNSSAI.sST = copy_ba_to_ostring(ba);

  if(src_ss->sd != NULL){
    dst_tai_ss->sNSSAI.sD = calloc(1, sizeof(OCTET_STRING_t));
    assert(dst_tai_ss->sNSSAI.sD != NULL && "Memory exhausted");
    *dst_tai_ss->sNSSAI.sD = copy_ba_to_ostring(*src_ss->sd);
  }

  return dst_tai_ss;
}

static
void cp_tai_slice_sup_lst(size_t len, s_nssai_t const tai_slice_sup_lst[len], F1AP_ProtocolExtensionContainer_154P34_t* dst_iE_Extensions)
{
  assert(len > 0);
  assert(tai_slice_sup_lst != NULL);
  assert(dst_iE_Extensions != NULL);

  F1AP_ServedPLMNs_ItemExtIEs_t* dst_slice_sup = calloc(1, sizeof(F1AP_ServedPLMNs_ItemExtIEs_t));
  assert(dst_slice_sup != NULL && "Memory exhausted");

  dst_slice_sup->id = F1AP_ProtocolIE_ID_id_TAISliceSupportList;
  dst_slice_sup->criticality = F1AP_Criticality_ignore;
  dst_slice_sup->extensionValue.present = F1AP_ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList;

  for(int k = 0; k < len; ++k ){
    F1AP_SliceSupportItem_t* ie = cp_slice_sup_lst(&tai_slice_sup_lst[k]); //, &dst_slice_sup->extensionValue.choice.SliceSupportList);
    int const rc = ASN_SEQUENCE_ADD(&dst_slice_sup->extensionValue.choice.SliceSupportList, ie);
    assert(rc == 0);
  }

  int const rc = ASN_SEQUENCE_ADD(&dst_iE_Extensions->list, dst_slice_sup);
  assert(rc == 0);
}

static
uint16_t cp_nr_pci(uint16_t src)
{
  assert(src < 1008); // int 0..1007
  return src;
}

static
F1AP_NRCGI_t cp_nr_cgi(nr_cgi_t const* src)
{
  assert(src != NULL);

  F1AP_NRCGI_t dst = {0};

  plmn_f1ap_t const* src_plmn_id = &src->plmn_id;
  MCC_MNC_TO_PLMNID(src_plmn_id->mcc, src_plmn_id->mnc, src_plmn_id->mnc_digit_len, &dst.pLMN_Identity);

  assert(src->nr_cell_id < (1UL << 36) );
  NR_CELL_ID_TO_BIT_STRING(src->nr_cell_id, &dst.nRCellIdentity);

  return dst;
}

static
F1AP_F1SetupRequestIEs_t* cp_gnb_du_name(byte_array_t ba)
{
  assert(ba.buf != NULL && ba.len > 0);

  F1AP_F1SetupRequestIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupRequestIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_gNB_DU_Name;
  ie->criticality               = F1AP_Criticality_ignore;
  ie->value.present             = F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Name;
  ie->value.choice.GNB_DU_Name  = copy_ba_to_ostring(ba);
  return ie;

}

static
F1AP_F1SetupResponseIEs_t*  cp_gnb_cu_name_sr(byte_array_t ba)
{
  assert(ba.buf != NULL && ba.len > 0);

  F1AP_F1SetupResponseIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupResponseIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_gNB_CU_Name;
  ie->criticality               = F1AP_Criticality_ignore;
  ie->value.present             = F1AP_F1SetupResponseIEs__value_PR_GNB_CU_Name;
  ie->value.choice.GNB_CU_Name  = copy_ba_to_ostring(ba);

  return ie;
}

static
F1AP_F1SetupResponseIEs_t* cp_trans_id_sr(uint8_t src_trans_id)
{
  assert(src_trans_id != 0);

  F1AP_F1SetupResponseIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupResponseIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_TransactionID;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupResponseIEs__value_PR_TransactionID;
  ie->value.choice.TransactionID = src_trans_id; 

  return ie;
}

static
F1AP_F1SetupResponseIEs_t* cp_gnb_rrc_sr(gnb_rrc_ver_f1ap_t const* ver)
{
  assert(ver != NULL);

  F1AP_F1SetupResponseIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupResponseIEs_t));
  assert(ie != NULL && "Memory exhausted");

  ie->id = F1AP_ProtocolIE_ID_id_GNB_CU_RRC_Version;
  ie->criticality = F1AP_Criticality_reject;
  ie->value.present = F1AP_F1SetupResponseIEs__value_PR_RRC_Version;

  // mandatory
  // Latest RRC Version
  assert(ver->latest_rrc_version.bits_unused == 8 - 3 && "Only 3 bits used");
  ie->value.choice.RRC_Version.latest_RRC_Version = copy_bit_string(&ver->latest_rrc_version); 

  // optional
  // Latest RRC Version Enhanced
  assert(ver->latest_rrc_version_enh == NULL);

  return ie;
} 

static
F1AP_ServedPLMNs_Item_t* cp_srv_plmn(srv_plmn_f1ap_t const *src_srv_plmn)
{
  assert(src_srv_plmn != NULL);

  F1AP_ServedPLMNs_Item_t* dst_srv_plmn = calloc(1, sizeof(F1AP_ServedPLMNs_Item_t ));
  assert(dst_srv_plmn != NULL && "Memory exhausted");

  // mandatory 
  // PLMN Identity 
  plmn_f1ap_t const* src_plmn_id = &src_srv_plmn->plmn_id;
  MCC_MNC_TO_PLMNID(src_plmn_id->mcc, src_plmn_id->mnc, src_plmn_id->mnc_digit_len, &dst_srv_plmn->pLMN_Identity );

  // optional 
  // TAI Slice support list  
  if(src_srv_plmn->sz_tai_slice_sup_lst > 0){
    dst_srv_plmn->iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P34_t));
    assert(dst_srv_plmn->iE_Extensions != NULL && "Memory exhausted");

    cp_tai_slice_sup_lst(src_srv_plmn->sz_tai_slice_sup_lst, src_srv_plmn->tai_slice_sup_lst, (F1AP_ProtocolExtensionContainer_154P34_t*)dst_srv_plmn->iE_Extensions);
  }

  return dst_srv_plmn;
}

static
F1AP_Configured_EPS_TAC_t* cp_eps_tac(uint16_t eps_tac)
{
  F1AP_Configured_EPS_TAC_t* dst = calloc(1, sizeof(F1AP_Configured_EPS_TAC_t));
  assert(dst != NULL && "Memory exhausted");

  byte_array_t ba = {.len = sizeof(eps_tac), .buf = (uint8_t*)&eps_tac};
  *dst = copy_ba_to_ostring(ba);
  return dst;
}

static
F1AP_FiveGS_TAC_t* cp_five_gs_tac(uint32_t five_gs_tac)
{
  assert(five_gs_tac < (1UL << 24) && "Max. 3 bytes" );

  F1AP_FiveGS_TAC_t* dst = calloc(1, sizeof(F1AP_FiveGS_TAC_t));
  assert(dst != NULL && "Memory exhausted");

//  uint32_t const tac = htonl(five_gs_tac);
  byte_array_t ba = {.len = sizeof(uint32_t) -1, .buf = (uint8_t*)&five_gs_tac};
//  ba.buf = ((uint8_t*)&tac+1);
  *dst = copy_ba_to_ostring(ba); 
  return dst;
}

static
F1AP_Served_Cell_Information_t cp_srv_cell_info(srv_cell_info_t const* src_srv_cell_info)
{
  assert(src_srv_cell_info != NULL);

  F1AP_Served_Cell_Information_t dst_srv_cell_info = {0};

  // mandatory
  // NR CGI
  dst_srv_cell_info.nRCGI = cp_nr_cgi(&src_srv_cell_info->nr_cgi);

  // mandatory 
  // NR PCI 
  dst_srv_cell_info.nRPCI = cp_nr_pci(src_srv_cell_info->nr_pci);

  // optional 
  // - fiveGS_TAC 
  if(src_srv_cell_info->five_gs_tac != NULL){
    dst_srv_cell_info.fiveGS_TAC = cp_five_gs_tac(*src_srv_cell_info->five_gs_tac);
  }

  // optional 
  // - Configured_EPS_TAC 
  if(src_srv_cell_info->eps_tac != NULL) {
    dst_srv_cell_info.configured_EPS_TAC = cp_eps_tac(*src_srv_cell_info->eps_tac);
  }
  // mandatory 
  // servedPLMN information 
  for(int i = 0; i < src_srv_cell_info->sz_srv_plmn; ++i){
    F1AP_ServedPLMNs_Item_t* dst_srv_plmn = cp_srv_plmn(&src_srv_cell_info->srv_plmn[i]);
    int rc = ASN_SEQUENCE_ADD(&dst_srv_cell_info.servedPLMNs, dst_srv_plmn);
    assert(rc == 0);
  }

  nr_mode_info_e const mode_info = src_srv_cell_info->nr_mode_info; 
  assert(mode_info == FDD_NR_MODE_INFO_F1AP || mode_info == TDD_NR_MODE_INFO_F1AP);

  if(src_srv_cell_info->nr_mode_info == FDD_NR_MODE_INFO_F1AP)
    dst_srv_cell_info.nR_Mode_Info = cp_fdd_info(&src_srv_cell_info->fdd); //, &dst_srv_cell_info);
  else // TDD_NR_MODE_INFO_F1AP
    dst_srv_cell_info.nR_Mode_Info = cp_tdd_info(&src_srv_cell_info->tdd); //, &dst_srv_cell_info);
  
  // mandatory
  // Measurement Timing Configuration
  dst_srv_cell_info.measurementTimingConfiguration = copy_ba_to_ostring(src_srv_cell_info->meas_timing_conf);

  // optional
  // RAN Area Code
  assert(src_srv_cell_info->ran_ac == NULL && "Not implemented");

  // optional
  // Extended Served PLMNs Item
  assert(src_srv_cell_info->sz_ext_srv_plmn == 0 && "Not implemented");

  // optional
  // Cell Direction
  assert(src_srv_cell_info->cell_dir == NULL && "Not implemented");

  // optional
  // Cell Type
  assert(src_srv_cell_info->cell_type == NULL && "Not implemented");

  // optional
  // Broadcast PLMN Identity Info List
  assert(src_srv_cell_info->brdcst_PLMN_id_info == NULL && "Not implemented" );

  // optional
  // Configured TAC Indication
  assert(src_srv_cell_info->conf_tac_ind  == NULL && "Not implemented");

  // optional
  //  Aggressor gNB Set ID
  assert(src_srv_cell_info->aggr_gnb_set_id== NULL && "Not implemented");

  // optional
  //  Victim gNB Set ID
  assert(src_srv_cell_info->victim_gnb_set_id == NULL && "Not implemented");

  // optional
  //  IAB Info IAB-DU
  assert(src_srv_cell_info->ab_info_iab_du == NULL && "Not implemented");

  // optional
  // SSB Positions In Burst
  assert(src_srv_cell_info->ssb_pos_burst == NULL && "Not implemented");

  //optional
  //NR PRACH Configuration
  assert(src_srv_cell_info->nr_prach_conf == NULL && "Not implemented");

  //optional
  //SFN Offset
  assert(src_srv_cell_info->sfn_offset == NULL && "Not implemented");

  return dst_srv_cell_info;
}

static
F1AP_GNB_DU_Served_Cells_ItemIEs_t* cp_srv_cell(gnb_du_srv_cell_f1ap_t const* src_srv_cell)
{
  assert(src_srv_cell != NULL);

  // mandatory 
  // 4.1 served cells item 
  F1AP_GNB_DU_Served_Cells_ItemIEs_t* dst_srv_cell = calloc(1, sizeof(F1AP_GNB_DU_Served_Cells_ItemIEs_t));
  assert(dst_srv_cell != NULL && "Memory exhausted");

  dst_srv_cell->id = F1AP_ProtocolIE_ID_id_GNB_DU_Served_Cells_Item;
  dst_srv_cell->criticality = F1AP_Criticality_reject;
  dst_srv_cell->value.present = F1AP_GNB_DU_Served_Cells_ItemIEs__value_PR_GNB_DU_Served_Cells_Item;

  // mandatory 
  // 4.1.1 served cell Information 
 dst_srv_cell->value.choice.GNB_DU_Served_Cells_Item.served_Cell_Information = cp_srv_cell_info(&src_srv_cell->srv_cell_info); 

  // optional
  // gNB-DU System Information
  assert(src_srv_cell->info == NULL && "Not Implemented" );

  return dst_srv_cell; 
}

static
F1AP_F1SetupRequestIEs_t* cp_srv_cell_array(size_t len, gnb_du_srv_cell_f1ap_t srv_cell[len])
{
  assert(len > 0);

  F1AP_F1SetupRequestIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupRequestIEs_t));
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_gNB_DU_Served_Cells_List;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List;
  for(size_t i = 0; i < len; ++i){
    F1AP_GNB_DU_Served_Cells_ItemIEs_t* dst_srv_cell = cp_srv_cell(&srv_cell[i]);
    int const rc = ASN_SEQUENCE_ADD(&ie->value.choice.GNB_DU_Served_Cells_List, dst_srv_cell);
    assert(rc == 0);
  }
  return ie;
}

static
F1AP_F1SetupRequestIEs_t* cp_gnb_rrc(gnb_rrc_ver_f1ap_t const* ver)
{
  assert(ver != NULL);

  F1AP_F1SetupRequestIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupRequestIEs_t ));
  assert(ie != NULL && "Memory exhausted");

  ie->id = F1AP_ProtocolIE_ID_id_GNB_DU_RRC_Version;
  ie->criticality = F1AP_Criticality_reject;
  ie->value.present = F1AP_F1SetupRequestIEs__value_PR_RRC_Version;

  // mandatory
  // Latest RRC Version
  assert(ver->latest_rrc_version.bits_unused == 8 - 3 && "Only 3 bits used");
  ie->value.choice.RRC_Version.latest_RRC_Version = copy_bit_string(&ver->latest_rrc_version); 

  // optional
  // Latest RRC Version Enhanced
  assert(ver->latest_rrc_version_enh == NULL);

  return ie;
} 

static
void cp_sib(sib_t const* src, void* dst_lst)
{
  F1AP_SibtypetobeupdatedListItem_t* dst = calloc(1, sizeof(F1AP_SibtypetobeupdatedListItem_t)); ;
  assert(dst != NULL &&  "Memory exhausted" );

  // Type. Mandatory
  assert(src->type > 1 && (src->type < 6 || src->type == 9) && "Invalid SIB type 9.3.1.42");
  dst->sIBtype = src->type;

  // Message. Mandatory
  dst->sIBmessage = copy_ba_to_ostring(src->msg);

  // Value tag. Mandatory
  assert(src->tag < 32);
  dst->valueTag = src->tag; 

  // Area scope. Optional
  assert(src->area_scope == NULL && "Not implemented");

  int const rc = ASN_SEQUENCE_ADD(dst_lst, dst);
  assert(rc == 0);
}

static
void cp_gnb_cu_system_info(gnb_cu_sys_info_t const* src, F1AP_ProtocolExtensionContainer_154P46_t* dst_lst)
{
  assert(src != NULL);
  assert(dst_lst != NULL);

  /* 3.1.2 gNB-CUSystem Information */
  F1AP_Cells_to_be_Activated_List_ItemExtIEs_t* dst = calloc(1, sizeof(F1AP_Cells_to_be_Activated_List_ItemExtIEs_t)); 
  assert(dst != NULL && "Memory exhasuted");

  dst->id = F1AP_ProtocolIE_ID_id_gNB_CUSystemInformation;
  dst->criticality = F1AP_Criticality_reject;
  dst->extensionValue.present = F1AP_Cells_to_be_Activated_List_ItemExtIEs__extensionValue_PR_GNB_CUSystemInformation;

  assert(src->len > 0 && src->len < 33 && "Out of range" );
  for(int i = 0; i < src->len; ++i){
    cp_sib(&src->sib[i], &dst->extensionValue.choice.GNB_CUSystemInformation.sibtypetobeupdatedlist.list); 
  }

  // Optional
  assert(src->id == NULL && "Not implemented");

  int const rc = ASN_SEQUENCE_ADD(&dst_lst->list, dst);
  assert(rc == 0);
}

static
void cp_activate_cell(activate_cell_t const* src, F1AP_Cells_to_be_Activated_List_t	* dst_lst)
{
  F1AP_Cells_to_be_Activated_List_ItemIEs_t* dst_lst_it = calloc(1, sizeof(F1AP_Cells_to_be_Activated_List_ItemIEs_t));
  assert(dst_lst_it != NULL);

  dst_lst_it->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List_Item;
  dst_lst_it->criticality = F1AP_Criticality_reject;
  dst_lst_it->value.present = F1AP_Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item;

  /* 3.1 cells to be Activated list item */
  F1AP_Cells_to_be_Activated_List_Item_t *dst = &dst_lst_it->value.choice.Cells_to_be_Activated_List_Item;

  // NR CGI 
  dst->nRCGI = cp_nr_cgi(&src->nr_cgi);

  // NR PCI optional
  if(src->nr_pci != NULL){
    dst->nRPCI = calloc(1, sizeof(F1AP_NRPCI_t));
    assert(dst->nRPCI != NULL);
    *dst->nRPCI = cp_nr_pci(*src->nr_pci);
  }

  // gNB-CU System Information
  if(src->sys_info != NULL){

    dst->iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P46_t));
    assert(dst->iE_Extensions != NULL && "Memory exhausted");

    cp_gnb_cu_system_info(src->sys_info, ((F1AP_ProtocolExtensionContainer_154P46_t*)dst->iE_Extensions));
  }

  // Available PLMN List. optional
  assert(src->avail_plmn == NULL && "Not implemented");
  assert(src->sz_avail_plmn_lst == 0 && "Not implemented");

  // Extended Available PLMN List. optional
  assert(src->ext_avail_plmn == NULL && "Not implemented");
  assert(src->sz_ext_avail_plmn_lst == 0);

  // IAB Info IAB-donor-CU. optional
  assert(src->ab_info == NULL && "Not implemented");

  // Available SNPN ID List. optional
  assert(src->av_snpn == NULL && "Not implemented");
  assert(src->sz_av_snpn_id_lst == 0 && "Not implemented");


  int const rc = ASN_SEQUENCE_ADD(dst_lst, dst_lst_it);
  assert(rc == 0);
}

static
F1AP_F1SetupResponseIEs_t* cp_cells_to_activate(size_t len, activate_cell_t act_cell[len])
{
  assert(len > 0);
  assert(act_cell != NULL);

  F1AP_F1SetupResponseIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupResponseIEs_t) );
  assert(ie != NULL && "Memory exhausted");
  ie->id                        = F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List;

  for(size_t i = 0; i < len; ++i){
    cp_activate_cell(&act_cell[i], &ie->value.choice.Cells_to_be_Activated_List ); 
  }

  return ie;
}



///////////////////////////////////////////
/////////// F1AP SETUP ////////////////////
///////////////////////////////////////////

F1AP_F1AP_PDU_t cp_f1_setup_asn(f1_setup_t* const f1_setup)
{
  F1AP_F1AP_PDU_t pdu = {0};

  /* Create */
  /* 0. pdu Type */
  pdu.present = F1AP_F1AP_PDU_PR_initiatingMessage;
  pdu.choice.initiatingMessage = calloc(1, sizeof(struct F1AP_InitiatingMessage));
  assert(pdu.choice.initiatingMessage != NULL && "Memory exahusted");
  F1AP_InitiatingMessage_t* initMsg = pdu.choice.initiatingMessage;

  initMsg->procedureCode = F1AP_ProcedureCode_id_F1Setup;
  initMsg->criticality   = F1AP_Criticality_reject;
  initMsg->value.present = F1AP_InitiatingMessage__value_PR_F1SetupRequest;
  F1AP_F1SetupRequest_t *dst_f1_setup = &initMsg->value.choice.F1SetupRequest;

  // mandatory 
  // c1. Transaction ID (integer value) 
  F1AP_F1SetupRequestIEs_t* trans_id = cp_trans_id(f1_setup->trans_id);
  int rc = ASN_SEQUENCE_ADD(&dst_f1_setup->protocolIEs.list, trans_id);
  assert(rc == 0);

  // mandatory 
  // c2. GNB_DU_ID (integer value) 
  F1AP_F1SetupRequestIEs_t* gnb_du_id = cp_gnb_du_id(f1_setup->gnb_du_id);
  rc = ASN_SEQUENCE_ADD(&dst_f1_setup->protocolIEs.list, gnb_du_id);
  assert(rc == 0);

  // optional 
  // c3. GNB_DU_Name 
  if (f1_setup->gnb_du_name != NULL){ 
    F1AP_F1SetupRequestIEs_t* gnb_du_name = cp_gnb_du_name(*f1_setup->gnb_du_name);
    rc = ASN_SEQUENCE_ADD(&dst_f1_setup->protocolIEs.list, gnb_du_name);
    assert(rc == 0);
  }

  // optional 
  // c4. served cells list 
  if(f1_setup->sz_srv_cell > 0){
    F1AP_F1SetupRequestIEs_t* dst_cells = cp_srv_cell_array(f1_setup->sz_srv_cell, f1_setup->srv_cell);
    rc = ASN_SEQUENCE_ADD(&dst_f1_setup->protocolIEs.list, dst_cells);
    assert(rc == 0);
  }

  // mandatory
  // gNB-DU RRC version
  // 9.3.1.70  This IE is not used in this release.
  F1AP_F1SetupRequestIEs_t* gnb_du_rrc = cp_gnb_rrc(&f1_setup->ver);
  rc = ASN_SEQUENCE_ADD(&dst_f1_setup->protocolIEs.list, gnb_du_rrc);
  assert(rc == 0);

  // optional 
  // Transport Layer Address Info
  assert(f1_setup->tlai == NULL && "Not implemented");

  // optional 
  // BAP Address
  assert(f1_setup->bad_add == NULL && "Not Implemented" ); 

  // optional 
  // Extended gNB-DU Name
  assert(f1_setup->egdn == NULL && "Not implemented" );

//  xer_fprint(stdout, &asn_DEF_F1AP_F1AP_PDU, &pdu);
//  fflush(stdout);
  return pdu;
}

///////////////////////////////////////////
/////////// F1AP SETUP RESPONSE ///////////
///////////////////////////////////////////

F1AP_F1AP_PDU_t cp_f1_setup_response_asn(f1_setup_response_t* const f1_sr)
{
  assert(f1_sr != NULL);

  F1AP_F1AP_PDU_t pdu = {0};

  /* Create */
  /* 0. pdu Type */
  pdu.present = F1AP_F1AP_PDU_PR_successfulOutcome;
  pdu.choice.successfulOutcome = calloc(1, sizeof(struct F1AP_SuccessfulOutcome));
  assert(pdu.choice.successfulOutcome != NULL && "Memory exahusted");

  F1AP_SuccessfulOutcome_t* dst_out = pdu.choice.successfulOutcome;

  dst_out->procedureCode =  F1AP_ProcedureCode_id_F1Setup;
  dst_out->criticality   =  F1AP_Criticality_reject;
  dst_out->value.present =  F1AP_SuccessfulOutcome__value_PR_F1SetupResponse;
  F1AP_F1SetupResponse_t *dst_f1_sr = &dst_out->value.choice.F1SetupResponse;

  // mandatory 
  // c1. Transaction ID (integer value)
  F1AP_F1SetupResponseIEs_t* trans_id = cp_trans_id_sr(f1_sr->trans_id);
  int rc = ASN_SEQUENCE_ADD(&dst_f1_sr->protocolIEs.list, trans_id );
  assert(rc == 0);

   // optional 
  // c2. GNB_CU_Name 
  if (f1_sr->gnb_cu_name != NULL){ 
    F1AP_F1SetupResponseIEs_t* ie  = cp_gnb_cu_name_sr(*f1_sr->gnb_cu_name); // , dst_f1_sr);
    int const rc = ASN_SEQUENCE_ADD(&dst_f1_sr->protocolIEs.list, ie);
    assert(rc == 0);
  }

  // optional 
  // c3. cells to be Activated list 
  if(f1_sr->sz_act_cell > 0){
    F1AP_F1SetupResponseIEs_t* ie = cp_cells_to_activate(f1_sr->sz_act_cell, f1_sr->act_cell); //, dst_f1_sr);
    int const rc = ASN_SEQUENCE_ADD(& dst_f1_sr->protocolIEs.list, ie);
    assert(rc == 0);
  }
    
  // mandatory
  // gNB-DU RRC version
  // 9.3.1.70  This IE is not used in this release.
  F1AP_F1SetupResponseIEs_t* rrc_ver = cp_gnb_rrc_sr(&f1_sr->ver); //, dst_f1_sr); 
  rc = ASN_SEQUENCE_ADD(&dst_f1_sr->protocolIEs.list, rrc_ver);
  assert(rc == 0);

  // optional 
  // Transport Layer Address Info
  assert(f1_sr->tlai == NULL && "Not implemented");

  // optional
  // Uplink BH Non-UP Traffic Mapping
  assert(f1_sr->ubh_nup_traff == NULL && "Not implemented ");

  // optional 
  // BAP Address
  assert(f1_sr->bad_add == NULL && "Not Implemented" ); 

  // optional 
  // Extended gNB-CU Name
  assert(f1_sr->egcn == NULL && "Not implemented" );

//  xer_fprint(stdout, &asn_DEF_F1AP_F1AP_PDU, &pdu);
//  fflush(stdout);
  return pdu;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


F1AP_F1SetupFailureIEs_t* cp_trans_id_sf(uint8_t trans_id)
{
  F1AP_F1SetupFailureIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupFailureIEs_t));
  assert(ie != NULL && "Memory exahusted");

  ie->id                        = F1AP_ProtocolIE_ID_id_TransactionID;
  ie->criticality               = F1AP_Criticality_reject;
  ie->value.present             = F1AP_F1SetupFailureIEs__value_PR_TransactionID;
  ie->value.choice.TransactionID = trans_id; 
  
  return ie;
}

F1AP_F1SetupFailureIEs_t* cp_cause_sf(cause_f1ap_t src)
{
  F1AP_F1SetupFailureIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupFailureIEs_t));
  assert(ie != NULL && "Memory exahusted");

  ie->id                        = F1AP_ProtocolIE_ID_id_Cause;
  ie->criticality               = F1AP_Criticality_ignore;
  ie->value.present             = F1AP_F1SetupFailureIEs__value_PR_Cause;

  if(src.type == RADIO_NETWORK_CAUSE_F1AP){
    ie->value.choice.Cause.present = F1AP_Cause_PR_radioNetwork;
    assert(src.radio < END_RADIO_NETWORK_CAUSE_F1AP);
    ie->value.choice.Cause.choice.radioNetwork = src.radio;
  } else if(src.type == TRANSPORT_CAUSE_F1AP){
    ie->value.choice.Cause.present = F1AP_Cause_PR_transport;
    assert(src.trans < END_TRANSPORT_CAUSE_F1AP);
    ie->value.choice.Cause.choice.transport = src.trans;
  } else if(src.type == PROTOCOL_CAUSE_F1AP){
    ie->value.choice.Cause.present = F1AP_Cause_PR_protocol;
    assert(src.proto < END_PROTOCOL_CAUSE_F1AP);
    ie->value.choice.Cause.choice.protocol = src.proto;
  } else if(src.type == MISC_CAUSE_F1AP){
    ie->value.choice.Cause.present = F1AP_Cause_PR_misc;
    assert(src.misc < END_MISC_CAUSE_F1AP);
    ie->value.choice.Cause.choice.misc = src.misc;
  } else{
    assert(0!= 0 && "Unknown type");
  }

  return ie;
}


F1AP_F1SetupFailureIEs_t* cp_time_to_wait_sf(time_to_wait_f1ap_e src)
{
  F1AP_F1SetupFailureIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupFailureIEs_t));
  assert(ie != NULL && "Memory exahusted");

  ie->id                        = F1AP_ProtocolIE_ID_id_TimeToWait;
  ie->criticality               = F1AP_Criticality_ignore;
  ie->value.present             = F1AP_F1SetupFailureIEs__value_PR_TimeToWait;

  assert(src < END_TIME_TO_WAIT_F1AP);

  ie->value.choice.TimeToWait = src;

  return ie;
}

F1AP_CriticalityDiagnostics_IE_Item_t* cp_crit_diagnose_ie_sf(crit_diagn_ie_t const* src)
{
  assert(src != NULL);
  F1AP_CriticalityDiagnostics_IE_Item_t* ie = calloc(1, sizeof(F1AP_CriticalityDiagnostics_IE_Item_t));
  assert(ie != NULL && "Memory exhausted");

  //mandatory 
  assert(src->proc_crit < END_PROCEDURE_CRITICALLITY_F1AP);
  ie->iECriticality = src->proc_crit;

  //mandatory 
  ie->iE_ID = src->id; 

  //mandatory 
  assert(src->type_of_error < END_CRITICALLITY_DIAGNOSTIC_IE_F1AP);
  ie->typeOfError = src->type_of_error;

  return ie;
} 

F1AP_F1SetupFailureIEs_t* cp_crit_diagnose_sf(criticallity_diagnostic_f1ap_t const* src)
{
  assert(src != NULL);

  F1AP_F1SetupFailureIEs_t* ie = calloc(1, sizeof(F1AP_F1SetupFailureIEs_t));
  assert(ie != NULL && "Memory exahusted");

  ie->id                        = F1AP_ProtocolIE_ID_id_CriticalityDiagnostics;
  ie->criticality               = F1AP_Criticality_ignore;
  ie->value.present             = F1AP_F1SetupFailureIEs__value_PR_CriticalityDiagnostics;
  F1AP_CriticalityDiagnostics_t	*dst = &ie->value.choice.CriticalityDiagnostics;

  //optional
  //Procedure Code
  if(src->proc_code != NULL){
    dst->procedureCode = calloc(1, sizeof(F1AP_ProcedureCode_t));
    assert(dst->procedureCode != NULL);
    *dst->procedureCode = *src->proc_code;
  }

  //optional
  //Triggering Message
  if(src->trig_msg != NULL){
    dst->triggeringMessage = calloc(1, sizeof(F1AP_TriggeringMessage_t));
    assert(dst->triggeringMessage != NULL);
    assert(*src->trig_msg < END_TRIGGERING_MESSAGE_F1AP);
    *dst->triggeringMessage = *src->trig_msg;
  }

  //optional
  //Procedure Criticality
  if(src->proc_crit != NULL){
    dst->procedureCriticality = calloc(1, sizeof(F1AP_Criticality_t));
    assert(dst->procedureCriticality != NULL && "Memory exhausted");
    *dst->procedureCriticality = *src->proc_crit;
  }

  //optional
  //Transaction ID
  if(src->trans_id != NULL){
    dst->transactionID = calloc(1, sizeof(F1AP_TransactionID_t));
    assert(dst->transactionID != NULL && "Memory exhausted" );
    *dst->transactionID = *src->trans_id;
  }

  // Optional
  if(src->sz_crit_diagn_ie > 0){
    dst->iEsCriticalityDiagnostics = calloc(1, sizeof(F1AP_CriticalityDiagnostics_IE_List_t));
    assert( dst->iEsCriticalityDiagnostics != NULL && "Memory exhausted");

    for(size_t i = 0; i < src->sz_crit_diagn_ie; ++i){
        F1AP_CriticalityDiagnostics_IE_Item_t* it = cp_crit_diagnose_ie_sf(&src->crit_diagn_ie[i]); 
        const int rc = ASN_SEQUENCE_ADD(&dst->iEsCriticalityDiagnostics->list, it);
        assert(rc == 0);
    }
  }

  return ie;
}

F1AP_F1AP_PDU_t cp_f1_setup_failure_asn(f1_setup_failure_t* const src)
{
  assert(src != NULL);

  F1AP_F1AP_PDU_t pdu = {0};

  /* Create */
  /* 0. pdu Type */
  pdu.present = F1AP_F1AP_PDU_PR_unsuccessfulOutcome;
  pdu.choice.unsuccessfulOutcome = calloc(1, sizeof(F1AP_UnsuccessfulOutcome_t));
  assert(pdu.choice.unsuccessfulOutcome != NULL && "Memory exahusted");

  F1AP_UnsuccessfulOutcome_t* dst_out = pdu.choice.unsuccessfulOutcome;

  dst_out->procedureCode =  F1AP_ProcedureCode_id_F1Setup;
  dst_out->criticality   =  F1AP_Criticality_reject;
  dst_out->value.present =  F1AP_UnsuccessfulOutcome__value_PR_F1SetupFailure;
  F1AP_F1SetupFailure_t *dst_f1_sf = &dst_out->value.choice.F1SetupFailure;

  // Mandatory
  //Transaction ID
  F1AP_F1SetupFailureIEs_t* id = cp_trans_id_sf(src->trans_id);
  int rc = ASN_SEQUENCE_ADD(&dst_f1_sf->protocolIEs.list, id);
  assert(rc == 0);

  //Mandatory
  //Cause
  F1AP_F1SetupFailureIEs_t* cause = cp_cause_sf(src->cause);
  rc = ASN_SEQUENCE_ADD(&dst_f1_sf->protocolIEs.list, cause);
  assert(rc == 0);

  // Optional
  //Time to wait
  if(src->time_to_wait != NULL){
    F1AP_F1SetupFailureIEs_t* time = cp_time_to_wait_sf(*src->time_to_wait);
    rc = ASN_SEQUENCE_ADD(&dst_f1_sf->protocolIEs.list, time);
    assert(rc == 0);
  }

  //Optional
  //Criticality Diagnostics
  if(src->diagnose != NULL){
    F1AP_F1SetupFailureIEs_t* diagnose = cp_crit_diagnose_sf(src->diagnose);
    rc = ASN_SEQUENCE_ADD(&dst_f1_sf->protocolIEs.list, diagnose);
    assert(rc == 0);
  }

  return pdu;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


