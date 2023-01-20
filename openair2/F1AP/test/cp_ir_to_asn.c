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
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_NonDynamic5QIDescriptor.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-List.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRANQoS.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Dynamic5QIDescriptor.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Flows-Mapped-To-DRB-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ULUPTNLInformation-ToBeSetup-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_GTPTunnel.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_AdditionalPDCPDuplicationTNL-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_DLUPTNLInformation-ToBeSetup-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Endpoint-IP-address-and-port.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRACells-List-item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRA-FDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRA-TDD-Info.h"

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

  assert(src->sz_sib > 0 && src->sz_sib < 33 && "Out of range" );
  for(int i = 0; i < src->sz_sib; ++i){
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

F1AP_F1AP_PDU_t cp_f1_setup_asn(f1_setup_t const* f1_setup)
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

F1AP_F1AP_PDU_t cp_f1_setup_response_asn(f1_setup_response_t const* f1_sr)
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

F1AP_F1AP_PDU_t cp_f1_setup_failure_asn(f1_setup_failure_t const* src)
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

static
F1AP_UEContextSetupRequestIEs_t* cp_gnb_cu_ue_sr(uint32_t src)
{
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;	
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID;
  dst->value.choice.GNB_CU_UE_F1AP_ID = src;

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_gnb_du_ue_sr(uint32_t src)
{
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID;
  dst->value.choice.GNB_DU_UE_F1AP_ID = src;

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_spcell_id_ue_sr(nr_cgi_t const* src)
{
  assert(src != NULL);
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_SpCell_ID;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_NRCGI;
  dst->value.choice.NRCGI = cp_nr_cgi(src); 

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_serv_cell_idx_ue_sr(uint8_t src)
{
  assert(src < 32);
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_ServCellIndex;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_ServCellIndex;
  dst->value.choice.ServCellIndex = src;

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_sp_cell_ul_conf_ue_sr(cell_ul_conf_e src)
{
  assert(src < END_CELL_UL_CONF);
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_SpCellULConfigured;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_CellULConfigured;

  if(src == UL_CELL_UL_CONF){
    dst->value.choice.CellULConfigured = F1AP_CellULConfigured_ul;
  } else if(src == SUL_CELL_UL_CONF){
    dst->value.choice.CellULConfigured = F1AP_CellULConfigured_sul	;
  } else if(src == UL_SUL_CELL_UL_CONF){
    dst->value.choice.CellULConfigured = F1AP_CellULConfigured_ul_and_sul	;
  } else {
    assert(0 != 0 && "Unknown type");
  }

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_cu_to_du_rrc_info_ue_sr(cu_to_du_rrc_info_t const * src)
{
  assert(src != NULL);
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_CUtoDURRCInformation;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_CUtoDURRCInformation;

 F1AP_CUtoDURRCInformation_t* dst_cu = &dst->value.choice.CUtoDURRCInformation;
  if(src->cg_config_info != NULL){
    dst_cu->cG_ConfigInfo = calloc(1, sizeof(F1AP_CG_ConfigInfo_t));
    assert(dst_cu->cG_ConfigInfo != NULL && "Memory exhasuted");
    *dst_cu->cG_ConfigInfo = copy_ba_to_ostring(*src->cg_config_info); 
  }

  if(src->ue_capability_rat_con_lst != NULL){
    dst_cu->uE_CapabilityRAT_ContainerList = calloc(1, sizeof(F1AP_UE_CapabilityRAT_ContainerList_t));
    assert(dst_cu->uE_CapabilityRAT_ContainerList != NULL && "Memory exhasuted");
    *dst_cu->uE_CapabilityRAT_ContainerList = copy_ba_to_ostring(*src->ue_capability_rat_con_lst); 
  }

  if(src->meas_config != NULL ){
    dst_cu->measConfig = calloc(1, sizeof(F1AP_MeasConfig_t));
    assert(dst_cu->measConfig != NULL && "Memory exhasuted");
    *dst_cu->measConfig = copy_ba_to_ostring(*src->meas_config); 
  }

  // Optional
  // Handover Preparation Information
  assert(src->handover_prep_info == NULL && "Not implemented");

  // Optional
  // CellGroupConfig
  assert(src->cell_group_config == NULL && "Not implemented");

  // Optional
  // Measurement Timing Configuration
  assert(src->meas_timing_conf == NULL && "Not implemented");

  // Optional
  // UEAssistanceInformation
  assert(src->ue_assistance_info == NULL && "Not implemented");

  // Optional
  // CG-Config
  assert(src->cg_config == NULL && "Not implemented");

  // Optional
  // UEAssistanceInformationEUTRA
  assert(src->UE_assistance_info_eutra == NULL && "Not implemented");

  return dst;
}

static
F1AP_Candidate_SpCell_ItemIEs_t* cp_candidate_sp_cell_item_ue_sr(nr_cgi_t const* src)
{
  assert(src != NULL);

  F1AP_Candidate_SpCell_ItemIEs_t* dst = calloc(1, sizeof(F1AP_Candidate_SpCell_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_Candidate_SpCell_Item; 
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_Candidate_SpCell_ItemIEs__value_PR_Candidate_SpCell_Item;
  dst->value.choice.Candidate_SpCell_Item.candidate_SpCell_ID = cp_nr_cgi(src); 

  return dst;
}  

static
F1AP_UEContextSetupRequestIEs_t* cp_candidate_sp_cell_id_ue_sr(size_t len, nr_cgi_t arr[len])
{
  assert(len < 65);
  assert(arr != NULL);

  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_Candidate_SpCell_List;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_Candidate_SpCell_List;

  for(size_t i = 0; i < len; ++i){
    F1AP_Candidate_SpCell_ItemIEs_t* ie = cp_candidate_sp_cell_item_ue_sr(&arr[i]);  
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Candidate_SpCell_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_drx_cycle_ue_sr(drx_cycle_t const* src)
{
  assert(src != NULL);

  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_DRXCycle;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_DRXCycle;

  // Mandatory 
  // Long DRX Cycle Length
  F1AP_DRXCycle_t* dst_drx = &dst->value.choice.DRXCycle;
  assert(src->long_drx_cycle_len < END_LONG_DRX_CYCLE_LEN);
  dst_drx->longDRXCycleLength = src->long_drx_cycle_len;

  // Optional 
  // Short DRX Cycle Length
  assert(src->short_drx_cycle_len == NULL && "Not implemented");

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  assert(src-> short_drx_cycle_timer == NULL && "Not implemented");

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_res_coord_trans_ue_sr(byte_array_t src)
{
  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_ResourceCoordinationTransferContainer;

  dst->value.choice.ResourceCoordinationTransferContainer = copy_ba_to_ostring(src);

  return dst;
}

static
F1AP_SCell_ToBeSetup_ItemIEs_t* cp_scell_to_be_setup_item_ue_sr(scell_to_be_setup_t const* src)
{
  assert(src != NULL);
  F1AP_SCell_ToBeSetup_ItemIEs_t* dst = calloc(1, sizeof(F1AP_SCell_ToBeSetup_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_Item;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item;

  F1AP_SCell_ToBeSetup_Item_t* dst_item = &dst->value.choice.SCell_ToBeSetup_Item;
  // Mandatory
  // SCell ID 
  dst_item->sCell_ID = cp_nr_cgi(&src->scell_id);

  // Mandatory
  // SCellIndex
  assert(src->scell_idx < 32 && src->scell_idx > 0); // INTEGER (1..31
  dst_item->sCellIndex = src->scell_idx; 

  // Optional
  // SCell UL Configured
  if(src->scell_ul_conf != NULL){
    dst_item->sCellULConfigured = calloc(1, sizeof(F1AP_CellULConfigured_t)); 
    assert(dst_item->sCellULConfigured != NULL && "Memory exhausted");
  
    assert(*src->scell_ul_conf < END_CELL_UL_CONF);
    if(*src->scell_ul_conf == UL_CELL_UL_CONF){
      *dst_item->sCellULConfigured = F1AP_CellULConfigured_ul;	
    } else if(*src->scell_ul_conf == SUL_CELL_UL_CONF){
      *dst_item->sCellULConfigured = F1AP_CellULConfigured_sul;	
    } else if(*src->scell_ul_conf == UL_SUL_CELL_UL_CONF){
      *dst_item->sCellULConfigured = F1AP_CellULConfigured_ul_and_sul;	
    } else {
      assert(0 !=0 && "Unknown type");
    }
  }

  // Optional
  // servingCellMO
  assert(src->serv_cell_mo == NULL && "Not implemented"); // INTEGER(1..64) 

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_scell_to_be_setup_ue_sr(size_t len, scell_to_be_setup_t arr[len])
{
  assert(len > 0);
  assert(arr != NULL);

  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_List;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_SCell_ToBeSetup_List;

  for(size_t i = 0; i < len; ++i){
    F1AP_SCell_ToBeSetup_ItemIEs_t* ie = cp_scell_to_be_setup_item_ue_sr(&arr[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.SCell_ToBeSetup_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_SRBs_ToBeSetup_ItemIEs_t* cp_srb_to_be_setup_item_ue_sr(srb_to_be_setup_t const* src)
{
  assert(src != NULL);

  F1AP_SRBs_ToBeSetup_ItemIEs_t* dst = calloc(1, sizeof(F1AP_SRBs_ToBeSetup_ItemIEs_t)); 
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_Item; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item;

  // Mandatory
  //  SRB ID
  assert(src->srb_id < 4); // [0,3]
  dst->value.choice.SRBs_ToBeSetup_Item.sRBID = src->srb_id; 

  // Optional
  // Duplication Indication
  assert(src-> dup_ind == NULL && "Not implemented");

  // Optional 
  // Additional Duplication Indication
  assert(src->add_dup_ind == NULL && "not implemented");

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_srb_to_be_setup_ue_sr(size_t len, srb_to_be_setup_t arr[len])
{
  assert(len > 0);
  assert(arr != NULL);

  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_List;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List;

  for(size_t i = 0; i < len; ++i){
    F1AP_SRBs_ToBeSetup_ItemIEs_t * ie = cp_srb_to_be_setup_item_ue_sr(&arr[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.SRBs_ToBeSetup_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_AllocationAndRetentionPriority_t	cp_alloc_retention_prio(alloc_retention_prio_t const* src)
{
  assert(src != NULL);

  F1AP_AllocationAndRetentionPriority_t	dst = {0};

  // Mandatory
  // Priority Level
  assert(src->prio_level < 16);// INTEGER (0..15)
  dst.priorityLevel = src->prio_level;

  // Mandatory
  // Pre-emption Capability
  assert(src->preempt_capability < END_PREEMPT_CAPABILITY);
  if(src->preempt_capability == SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY){
    dst.pre_emptionCapability = F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption;
  } else if(src->preempt_capability == MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY){
    dst.pre_emptionCapability = F1AP_Pre_emptionCapability_may_trigger_pre_emption;
  } else {
    assert(0!= 0 && "Not known type");
  }

  // Mandatory
  // Pre-emption Vulnerability
  assert(src->preempt_vulnerability < END_PREEMPT_VULNERABILITY);
  if(src->preempt_vulnerability == NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY ){
    dst.pre_emptionVulnerability = F1AP_Pre_emptionVulnerability_not_pre_emptable	;
  } else if(src->preempt_vulnerability == PRE_EMPTABLE_PREEMPT_VULNERABILITY ){
    dst.pre_emptionVulnerability = F1AP_Pre_emptionVulnerability_pre_emptable	;
  } else {
    assert(0!= 0 && "Not known type");
  }

  return dst;
}

static
F1AP_NGRANAllocationAndRetentionPriority_t cp_ng_ran_alloc_retention_prio(alloc_retention_prio_t const* src)
{
  assert(src != NULL);

  F1AP_NGRANAllocationAndRetentionPriority_t dst = {0};

  // Mandatory
  // Priority Level
  assert(src->prio_level < 16);// INTEGER (0..15)
  dst.priorityLevel = src->prio_level;

  // Mandatory
  // Pre-emption Capability
  assert(src->preempt_capability < END_PREEMPT_CAPABILITY);
  if(src->preempt_capability == SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY){
    dst.pre_emptionCapability = F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption;
  } else if(src->preempt_capability == MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY){
    dst.pre_emptionCapability = F1AP_Pre_emptionCapability_may_trigger_pre_emption;
  } else {
    assert(0!= 0 && "Not known type");
  }

  // Mandatory
  // Pre-emption Vulnerability
  assert(src->preempt_vulnerability < END_PREEMPT_VULNERABILITY);
  if(src->preempt_vulnerability == NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY ){
    dst.pre_emptionVulnerability = F1AP_Pre_emptionVulnerability_not_pre_emptable	;
  } else if(src->preempt_vulnerability == PRE_EMPTABLE_PREEMPT_VULNERABILITY ){
    dst.pre_emptionVulnerability = F1AP_Pre_emptionVulnerability_pre_emptable	;
  } else {
    assert(0!= 0 && "Not known type");
  }

  return dst;
}

static
F1AP_NonDynamic5QIDescriptor_t* cp_non_dyn_5qi(non_dyn_5qi_descriptor_t const* src)
{
  assert(src != NULL);
  F1AP_NonDynamic5QIDescriptor_t* dst = calloc(1, sizeof(F1AP_NonDynamic5QIDescriptor_t));
  assert(dst != NULL && "Memory exhausted");

  // Mandatory
  //  5QI
  dst->fiveQI = src->five_qi;

  // Optional
  // Priority Level
  assert(src->prio_level == NULL && "Not implemented"); // [1,127]

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  assert(src->av_window == NULL && "Not implemented");

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  assert(src->max_data_burst_vol == NULL && "Not implemented"); // INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  assert(src->pkt_delay_budget_downlink == NULL && "Not implemented"); // INTEGER (0..65535, …)

  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  assert(src->pkt_delay_budget_uplink == NULL && "Not implemented"); // INTEGER (0..65535, …)

  return dst;
}

static
F1AP_Dynamic5QIDescriptor_t* cp_dyn_5qi(dyn_5qi_descriptor_t const* src)
{
  assert(src != NULL);

  F1AP_Dynamic5QIDescriptor_t* dst = calloc(1, sizeof(F1AP_Dynamic5QIDescriptor_t));
  assert(dst != NULL && "Memory exhausted");

  //Mandatory
  //QoS Priority Level
  assert(src->qos_prio_level > 0 && src->qos_prio_level < 128);
  dst->qoSPriorityLevel = src->qos_prio_level; // INTEGER (1..127)

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  assert(src->pkt_delay_budget < 1024);
  dst->packetDelayBudget = src->pkt_delay_budget; // INTEGER (0..1023, ...)

  // Mandatory
  // Packet Error Rate 9.3.1.52
  dst->packetErrorRate.pER_Scalar = src->pkt_error_rate.scalar; 
  dst->packetErrorRate.pER_Exponent = src->pkt_error_rate.exponent; 

  // Optional
  // 5QI
  assert(src->five_qi == NULL && "Not implemented");

  // C-ifGBRflow
  // Delay Critical
  assert(src->delay_critical == NULL && "Not implemented");

  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  assert(src->av_window == NULL && "Not implemented"); // INTEGER (0..4095, ...)

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  assert(src->max_data_burst_vol == NULL && "Not implemented"); //INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  assert(src->ext_pkt_delay_budget == NULL && "Not implemented"); //INTEGER (0..65535, …) 

  // Optional 
  // CN Packet Delay Budget Downlink
  assert(src->cn_pkt_dalay_budget_downlink == NULL && "Not implemented"); 

  // Optional
  // CN Packet Delay Budget Uplink
  assert(src->cn_pkt_dalay_budget_uplink == NULL && "Not implemented"); 

  return dst;
}

static
F1AP_QoSFlowLevelQoSParameters_t cp_qos_flow_level_qos_parameter(qos_flow_level_qos_parameter_t const* src)
{
  assert(src != NULL);

  F1AP_QoSFlowLevelQoSParameters_t dst = {0}; 

  //Mandatory 
  // CHOICE QoS Characteristics
  assert(src->qos_characteristic < END_5QI_DESCRIPTOR); 
  if(src->qos_characteristic == NON_DYN_5QI_DESCRIPTOR){
    // Non Dynamic 5QI Descriptor 9.3.1.49
    dst.qoS_Characteristics.present = F1AP_QoS_Characteristics_PR_non_Dynamic_5QI;
    dst.qoS_Characteristics.choice.non_Dynamic_5QI = cp_non_dyn_5qi(&src->non_dyn);
  } else if(src->qos_characteristic == DYN_5QI_DESCRIPTOR){
    // Dynamic 5QI Descriptor 9.3.1.47
    dst.qoS_Characteristics.present = F1AP_QoS_Characteristics_PR_dynamic_5QI;
    dst.qoS_Characteristics.choice.dynamic_5QI = cp_dyn_5qi(&src->dyn);
  } else {
    assert(0!=0 && "Unknown type");
  }

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
  dst.nGRANallocationRetentionPriority = cp_ng_ran_alloc_retention_prio(&src->alloc_retention_prio);

  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  assert(src->gbr_qos_flow_info == NULL && "Not implemented");

  // Optional 
  // Reflective QoS Attribute
  assert(src->reflective_qos_attr == NULL && "Not implemented");

  // Optional 
  // PDU Session ID
  assert(src->pdu_session_id == NULL && "Not implemented");

  // Optional
  //UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  assert(src->ul_pdu_session_aggr_max_bit_rate == NULL && "Not implemented"); 

  // Optional
  // QoS Monitoring Request
  assert(src->qos_monitoring_request == NULL && "Not implemented");

  return dst;
}

static
F1AP_SNSSAI_t cp_nssai(s_nssai_t const* src)
{
  assert(src != NULL);
  F1AP_SNSSAI_t dst = {0};

  // Mandatory
  // OCTET STRING (SIZE(1))
  dst.sST.size = 1;
  dst.sST.buf = malloc(1);
  assert(dst.sST.buf != NULL && "Memory exhausted" );
  memcpy(dst.sST.buf, &src->sst, 1);

  // Optional
  // OCTET STRING (SIZE(3))
  assert(src->sd == NULL && "Not implemented"); // size 3 bytes

  return dst;
}

static
F1AP_Flows_Mapped_To_DRB_Item_t* cp_flows_mapped_to_drb_ue_sr(flows_mapped_to_drb_t const* src)
{
  assert(src != NULL);

  F1AP_Flows_Mapped_To_DRB_Item_t* dst = calloc(1, sizeof(F1AP_Flows_Mapped_To_DRB_Item_t));
  assert(dst != NULL && "Memory exhausted");

  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  assert(src->qfi < 64);  // INTEGER (0 ..63)
  dst->qoSFlowIdentifier = src->qfi; 

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  dst->qoSFlowLevelQoSParameters = cp_qos_flow_level_qos_parameter(&src->qos_flow_level); 

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  assert(src->  qos_flow_mapping_ind == NULL && "Not implemented" );

  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  assert(src->tsc_traffic_char == NULL && "Not implemented");

  return dst;
}

static
F1AP_DRB_Information_t cp_drb_info(drb_info_t const* src) 
{
  assert(src != NULL);

  F1AP_DRB_Information_t dst = {0};

  // Mandatory
  // DRB QoS 9.3.1.45
  dst.dRB_QoS = cp_qos_flow_level_qos_parameter(&src->drb_qos);

  // Mandatory
  // S-NSSAI 9.3.1.38
  dst.sNSSAI = cp_nssai(&src->nssai);

  // Optional
  // Notification Control 9.3.1.56
  assert(src->notification_ctrl == NULL && "Not implemented");

  // Flows Mapped to DRB
  assert(src->sz_flows_mapped_to_drb > 0 && src->sz_flows_mapped_to_drb < 65);// [1,64]
  for(size_t i = 0; i < src-> sz_flows_mapped_to_drb; ++i){
    F1AP_Flows_Mapped_To_DRB_Item_t* ie = cp_flows_mapped_to_drb_ue_sr(&src->flows_mapped_to_drb[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst.flows_Mapped_To_DRB_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_ULUPTNLInformation_ToBeSetup_Item_t* cp_ul_up_tnl_info_ue_sr(ul_up_tnl_info_lst_t const* src)
{
  assert(src != NULL);

  F1AP_ULUPTNLInformation_ToBeSetup_Item_t* dst = calloc(1,sizeof(F1AP_ULUPTNLInformation_ToBeSetup_Item_t));
  assert(dst != NULL && "Memory exhausted");

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  dst->uLUPTNLInformation.present = F1AP_UPTransportLayerInformation_PR_gTPTunnel;
  dst->uLUPTNLInformation.choice.gTPTunnel = calloc(1, sizeof(F1AP_GTPTunnel_t));
  assert(dst->uLUPTNLInformation.choice.gTPTunnel != NULL && "Memory exhausted");

  dst->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress = copy_bit_string(&src->tnl_info.tla); 
  byte_array_t ba = {.buf = (uint8_t*)&src->tnl_info.gtp_teid, .len = 4 };
  dst->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID = copy_ba_to_ostring(ba);

  // Optional
  // BH Information 9.3.1.114
  assert(src->bh_info == NULL && "Memory exhausted");

  return dst;
} 

static
F1AP_DRBs_ToBeSetup_ItemExtIEs_t* cp_dl_pdcp_sn_len_ue_sr(const pdcp_sn_len_e src)
{
  assert(src < END_PDCP_SN_LEN);

  F1AP_DRBs_ToBeSetup_ItemExtIEs_t* dst = calloc(1, sizeof(F1AP_DRBs_ToBeSetup_ItemExtIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_DLPDCPSNLength;
  dst->criticality = F1AP_Criticality_ignore;
  dst->extensionValue.present = F1AP_DRBs_ToBeSetup_ItemExtIEs__extensionValue_PR_PDCPSNLength;

  if(src == TWELVE_12BITS_PDCP_SN_LEN){
    dst->extensionValue.choice.PDCPSNLength = F1AP_PDCPSNLength_twelve_bits	;
  } else if(src == EIGHTEEN_18BITS_PDCP_SN_LEN ){
    dst->extensionValue.choice.PDCPSNLength =  F1AP_PDCPSNLength_eighteen_bits;
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

static
F1AP_AdditionalPDCPDuplicationTNL_Item_t* cp_add_pdcp_dup_tnl_item_ue_sr(add_pdcp_dup_tnl_t const* src)
{
  assert(src != NULL);

  F1AP_AdditionalPDCPDuplicationTNL_Item_t* dst = calloc(1, sizeof(F1AP_AdditionalPDCPDuplicationTNL_Item_t));
  assert(dst != NULL && "Memory exhausted");

  dst->additionalPDCPDuplicationUPTNLInformation.present = F1AP_UPTransportLayerInformation_PR_gTPTunnel;
  dst->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel = calloc(1, sizeof(F1AP_GTPTunnel_t));
  assert(dst->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel != NULL && "Memory exhausted");

  byte_array_t ba = {.len = 4, .buf = (uint8_t*)&src->gtp_teid};
  dst->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel->gTP_TEID = copy_ba_to_ostring(ba);

  dst->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel->transportLayerAddress = copy_bit_string(&src->trans_layer_add);

  return dst;
}

static
F1AP_DRBs_ToBeSetup_ItemExtIEs_t* cp_add_pdcp_dup_tnl_ue_sr(size_t len , add_pdcp_dup_tnl_t arr[len])
{
  assert(len > 0);
  assert(arr != NULL);

  F1AP_DRBs_ToBeSetup_ItemExtIEs_t* dst = calloc(1, sizeof(F1AP_DRBs_ToBeSetup_ItemExtIEs_t));
  assert(dst != NULL);

  dst->id = F1AP_ProtocolIE_ID_id_AdditionalPDCPDuplicationTNL_List;
  dst->criticality = F1AP_Criticality_ignore;
  dst->extensionValue.present = F1AP_DRBs_ToBeSetup_ItemExtIEs__extensionValue_PR_AdditionalPDCPDuplicationTNL_List;

  for(size_t i = 0; i < len; ++i ){
    F1AP_AdditionalPDCPDuplicationTNL_Item_t* ie = cp_add_pdcp_dup_tnl_item_ue_sr(&arr[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->extensionValue.choice.AdditionalPDCPDuplicationTNL_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_DRBs_ToBeSetup_ItemIEs_t* cp_drb_to_be_setup_item_ue_sr(drb_to_be_setup_t const* src)
{
  assert(src != NULL);

  F1AP_DRBs_ToBeSetup_ItemIEs_t* dst = calloc(1, sizeof(F1AP_DRBs_ToBeSetup_ItemIEs_t)); 
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_Item; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_DRBs_ToBeSetup_ItemIEs__value_PR_DRBs_ToBeSetup_Item;

  F1AP_DRBs_ToBeSetup_Item_t* dst_drb = &dst->value.choice.DRBs_ToBeSetup_Item;

  // Mandatory
  // DRB ID 9.3.1.8
  assert(src->drb_id < 33); // INTEGER [0,32]
  dst_drb->dRBID = src->drb_id;

  F1AP_QoSInformation_t* dst_qos = &dst_drb->qoSInformation;

  assert(src->qos_info < END_QOS_INFORMATION);
  if(src->qos_info == E_UTRAN_QOS_INFORMATION){

    dst_qos->present = F1AP_QoSInformation_PR_eUTRANQoS;
    dst_qos->choice.eUTRANQoS = calloc(1, sizeof(F1AP_EUTRANQoS_t)) ; 
    assert(dst_qos->choice.eUTRANQoS != NULL && "Memory exhausted");
     e_utran_qos_t const* src_qos = &src->e_utran_qos;
    // Mandatory
    // QCI
    dst_qos->choice.eUTRANQoS->qCI = src_qos->qci; 

    // Mandatory
    // Allocation and Retention Priority
    dst_qos->choice.eUTRANQoS->allocationAndRetentionPriority = cp_alloc_retention_prio(&src_qos->alloc);

    // Optional
    // GBR QoS Information 9.3.1.21
    assert(src_qos->gbr_qos_info == NULL && "not implemented" );

  } else if(src->qos_info == DRB_INFO_QOS_INFORMATION){
    dst_qos->present = F1AP_QoSInformation_PR_choice_extension;
    dst_qos->choice.choice_extension = calloc(1, sizeof(F1AP_QoSInformation_ExtIEs_t));
    assert(dst_qos->choice.choice_extension != NULL && "Memory exhausted"); 

    F1AP_QoSInformation_ExtIEs_t* dst_qos_info = (F1AP_QoSInformation_ExtIEs_t*)dst_qos->choice.choice_extension;
    dst_qos_info->id = F1AP_ProtocolIE_ID_id_DRB_Information;
    dst_qos_info->criticality = F1AP_Criticality_ignore;
    dst_qos_info->value.present = F1AP_QoSInformation_ExtIEs__value_PR_DRB_Information;

    dst_qos_info->value.choice.DRB_Information = cp_drb_info(&src->drb_info);
  } else {
    assert("Unknown type");
  }


  // UL UP TNL Information to be setup List
  if(src->sz_ul_up_tnl_info_lst > 0){
    assert(src->sz_ul_up_tnl_info_lst < 3);
    assert(src-> ul_up_tnl_info != NULL);
  }
  for(size_t i = 0; i < src->sz_ul_up_tnl_info_lst; ++i){
    F1AP_ULUPTNLInformation_ToBeSetup_Item_t* ie = cp_ul_up_tnl_info_ue_sr(&src->ul_up_tnl_info[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst_drb->uLUPTNLInformation_ToBeSetup_List.list, ie);
    assert(rc == 0);
  }

  // Mandatory
  // RLC Mode 9.3.1.27
  assert(src->rlc_mode < END_RLC_MODE_F1AP);
  if(src->rlc_mode == RLC_AM_RLC_MODE_F1AP){
    dst_drb->rLCMode = F1AP_RLCMode_rlc_am	;
  } else if(src->rlc_mode == RLC_UM_BIDIRECTIONAL_RLC_MODE_F1AP){
    dst_drb->rLCMode = F1AP_RLCMode_rlc_um_bidirectional;
  } else if(src->rlc_mode == RLC_UM_UNIDIRECTIONAL_UL_RLC_MODE_F1AP){
    dst_drb->rLCMode = F1AP_RLCMode_rlc_um_unidirectional_ul;
  } else if(src->rlc_mode == RLC_UM_UNIDIRECTIONAL_DL_RLC_MODE_F1AP){
    dst_drb->rLCMode = F1AP_RLCMode_rlc_um_unidirectional_dl;
  } else {
    assert( 0!= 0 && "Unknown type");
  }

  // Optional 
  // UL Configuration 9.3.1.31
  assert(src-> ul_conf == NULL && "Not implemented");

  // Optional 
  // Duplication Activation 9.3.1.36
  assert(src->dup_act == NULL && "Not implemented");

  // Optional
  // DC Based Duplication Configured
  assert(src->dc_dup_act_conf == NULL && "Not implemented");

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  assert(src->dc_dup_act == NULL && "Not implemented");

  // Mandatory
  // DL PDCP SN length
  F1AP_DRBs_ToBeSetup_ItemExtIEs_t* ie = cp_dl_pdcp_sn_len_ue_sr(src->dl_pdcp_sn_len); 
  if(dst_drb->iE_Extensions == NULL){
    dst_drb->iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P82_t)); 
    assert(dst_drb->iE_Extensions != NULL && "Memory exhausted" );
  }
  int rc = ASN_SEQUENCE_ADD(&((F1AP_ProtocolExtensionContainer_154P82_t*)dst_drb->iE_Extensions)->list, ie);
  assert(rc == 0);

  // Optional
  // UL PDCP SN length
  assert(src->ul_pdcp_sn_len == NULL && "Not implemented");

  // Additional PDCP Duplication TNL List
  assert(src->sz_add_pdcp_dup_tnl < 3); // [0,2]

  if(src->sz_add_pdcp_dup_tnl > 0) {
    F1AP_DRBs_ToBeSetup_ItemExtIEs_t* ie = cp_add_pdcp_dup_tnl_ue_sr(src->sz_add_pdcp_dup_tnl, src->add_pdcp_dup_tnl); 
    int rc = ASN_SEQUENCE_ADD(&((F1AP_ProtocolExtensionContainer_154P82_t*)dst_drb->iE_Extensions)->list, ie);
    assert(rc == 0);
  }

  // Optional
  // RLC Duplication Information 9.3.1.146
  assert(src->sz_rlc_dup_info < 4); // [0,3]
  assert(src->sz_rlc_dup_info == 0 && "Not implememted");
  assert(src->rlc_dup_info == NULL && "Not implememted");
  // For future implementation
  // F1AP_RLCDuplicationInformation_t
  // F1AP_ProtocolIE_ID_id_RLCDuplicationInformation	
  

  return dst;
}

static
F1AP_UEContextSetupRequestIEs_t* cp_drb_to_be_setup_ue_sr(size_t len, drb_to_be_setup_t arr[len])
{
  assert(len > 0);
  assert(arr != NULL);

  F1AP_UEContextSetupRequestIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupRequestIEs_t) );
  assert(dst != NULL && "memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_List;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List;

  for(size_t i = 0; i < len; ++i){
    F1AP_DRBs_ToBeSetup_ItemIEs_t * ie = cp_drb_to_be_setup_item_ue_sr(&arr[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.DRBs_ToBeSetup_List.list, ie);
    assert(rc == 0);
  }

  return dst;
}

F1AP_F1AP_PDU_t cp_ue_ctx_setup_request_asn(ue_ctx_setup_request_t const* src)
{
  assert(src != NULL);

  F1AP_F1AP_PDU_t pdu = {0};

  /* Create */
  /* 0. pdu Type */
  pdu.present = F1AP_F1AP_PDU_PR_initiatingMessage;
  pdu.choice.initiatingMessage = calloc(1, sizeof(struct F1AP_InitiatingMessage));
  assert(pdu.choice.initiatingMessage != NULL && "Memory exahusted");
  F1AP_InitiatingMessage_t* initMsg = pdu.choice.initiatingMessage;

  initMsg->procedureCode = F1AP_ProcedureCode_id_UEContextSetup;
  initMsg->criticality = F1AP_Criticality_reject;
  initMsg->value.present = F1AP_InitiatingMessage__value_PR_UEContextSetupRequest;
  F1AP_UEContextSetupRequest_t *dst = &initMsg->value.choice.UEContextSetupRequest;

  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  F1AP_UEContextSetupRequestIEs_t* id = cp_gnb_cu_ue_sr(src->gnb_cu_ue_f1ap_id);
  int rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
  assert(rc == 0);

  // Optional
  // gNB-DU UE F1AP ID 9.3.1.5
  if(src->gnb_du_ue_f1ap_id != NULL){
    F1AP_UEContextSetupRequestIEs_t* id = cp_gnb_du_ue_sr(*src->gnb_du_ue_f1ap_id);
    int rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // Mandatory
  // SpCell ID
  F1AP_UEContextSetupRequestIEs_t* spcell_id = cp_spcell_id_ue_sr(&src->sp_cell_id);
  rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, spcell_id);
  assert(rc == 0);

  // Mandatory
  // ServCellIndex
  F1AP_UEContextSetupRequestIEs_t* served_cell_id = cp_serv_cell_idx_ue_sr(src->serv_cell_idx);
  rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, served_cell_id);
  assert(rc == 0);

  // Optional
  // SpCell UL Configured 9.3.1.33
  if(src->sp_cell_ul_conf != NULL){
    F1AP_UEContextSetupRequestIEs_t* ie = cp_sp_cell_ul_conf_ue_sr(*src->sp_cell_ul_conf);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  }

  // Mandatory
  // CU to DU RRC Information 9.3.1.25
  F1AP_UEContextSetupRequestIEs_t* cu_to_du_rrc_info = cp_cu_to_du_rrc_info_ue_sr(&src->cu_to_du_rrc_info);
  rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, cu_to_du_rrc_info);
  assert(rc == 0);

  // Optional 
  // Candidate SpCell ID
  if(src->sz_candidate_sp_cell_id > 0){
    assert(src->sz_candidate_sp_cell_id < 65); // max. 64
    assert(src->candidate_sp_cell_id != NULL); 
    F1AP_UEContextSetupRequestIEs_t* ie = cp_candidate_sp_cell_id_ue_sr(src->sz_candidate_sp_cell_id, src->candidate_sp_cell_id);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  } else {
    assert(src->candidate_sp_cell_id == NULL);
  }

  //Optional 
  // DRX Cycle
  if(src->drx_cycle != NULL){
    F1AP_UEContextSetupRequestIEs_t* ie = cp_drx_cycle_ue_sr(src->drx_cycle);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  }

  // Optional
  // Resource Coordination Transfer Container
  if(src->resource_coord_transfer_container != NULL){
    F1AP_UEContextSetupRequestIEs_t* ie = cp_res_coord_trans_ue_sr(*src->resource_coord_transfer_container);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  }

  // SCell To Be Setup List
  if(src->sz_scell_to_be_setup > 0){
    assert(src->sz_scell_to_be_setup < 33);
    assert(src->scell_to_be_setup != NULL);
    F1AP_UEContextSetupRequestIEs_t* ie = cp_scell_to_be_setup_ue_sr(src->sz_scell_to_be_setup, src->scell_to_be_setup);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  } else {
    assert(src->scell_to_be_setup == NULL);
  }

  // SRB to Be Setup Item
  if(src->sz_srb_to_be_setup > 0){ // [0,8]
    assert(src->sz_srb_to_be_setup < 9);
    assert(src->srb_to_be_setup != NULL);
    F1AP_UEContextSetupRequestIEs_t* ie = cp_srb_to_be_setup_ue_sr(src->sz_srb_to_be_setup, src->srb_to_be_setup);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  } else {
    assert(src->srb_to_be_setup == NULL);
  }

  // DRB to Be Setup Item
  if(src->sz_drb_to_be_setup > 0){
    assert(src->sz_drb_to_be_setup < 65); // [0,64]
    assert(src->drb_to_be_setup != NULL);
    F1AP_UEContextSetupRequestIEs_t* ie = cp_drb_to_be_setup_ue_sr(src->sz_drb_to_be_setup, src->drb_to_be_setup);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, ie);
    assert(rc == 0);
  } else {
    assert(src->drb_to_be_setup == NULL);
  }

  // Optional
  // Inactivity Monitoring Request
 
  // Optional 
  // RAT-Frequency Priority Information 9.3.1.34

  // Optional
  // RRC-Container 9.3.1.6

  // Optional 
  // Masked IMEISV 9.3.1.55

  // Optional
  // Serving PLMN 9.3.1.14
  
  // C-ifDRBSetup Optional
  // gNB-DU UE Aggregate Maximum Bit Rate Uplink 9.3.1.22

  // Optional
  // RRC Delivery Status Request

  // Optional 
  // Resource Coordination Transfer Information 9.3.1.73

  // Optional
  // servingCellMO 

  // Optional 
  // New gNB-CU UE F1AP ID 9.3.1.4
 
  // Optional
  // RAN UE ID

  // Optional
  // Trace Activation 9.3.1.88

  // Optional
  // Additional RRM Policy Index 9.3.1.90

  // BH RLC Channel to be Setup Item [0, 65536]
  // size_t sz_bh_rlc_chan_to_be_setup; 
  // bh_rlc_chan_to_be_setup_t* ; 

  // Optional
  // Configured BAP Address 9.3.1.111

  // Optional
  // NR V2X Services Authorized 9.3.1.116

  // Optional
  // LTE V2X Services Authorized 9.3.1.117

  // Optional
  // NR UE Sidelink Aggregate Maximum Bit Rate 9.3.1.119

  // Optional
  // LTE UE Sidelink Aggregate Maximum Bit Rate 9.3.1.118

  // Optional
  // PC5 Link Aggregate Bit Rate 9.3.1.22

  // Optional
  // SL DRB to Be Setup List [0, 64]

  // Optional
  // Conditional Inter-DU Mobility Information 

  // Optional
  // Management Based MDT PLMN List 9.3.1.151

  // Optional
  // Serving NID 9.3.1.155

  // Optional
  // F1-C Transfer Path 9.3.1.207 

  return pdu;
}



///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


static
F1AP_UEContextSetupResponseIEs_t* cp_gnb_cu_ue_stp_rsp(uint32_t src)
{
  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID;	
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_GNB_CU_UE_F1AP_ID;
  dst->value.choice.GNB_CU_UE_F1AP_ID = src;

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_gnb_du_ue_stp_rsp(uint32_t src)
{
  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID;	
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_GNB_DU_UE_F1AP_ID;
  dst->value.choice.GNB_DU_UE_F1AP_ID = src;

  return dst;
}

static
F1AP_DUtoCURRCInformation_t cp_du_to_cu_rrc_info(du_to_cu_rrc_information_f1ap_t const* src)
{
  assert(src != NULL);

  F1AP_DUtoCURRCInformation_t dst = {0}; 

  // Mandatory
  // CellGroupConfig
  dst.cellGroupConfig = copy_ba_to_ostring(src->cell_group_config); 

  // Optional
  // MeasGapConfig
  assert(src->meas_gap_info == NULL && "Not implemented"); 

  // Optional
  // Requested P-MaxFR1
  assert(src->req_p_max_fr1== NULL && "Not implemented"); 

  // Optional
  // DRX Long Cycle Start Offset
  assert(src->drx_lng_cyc_str_off== NULL && "Not implemented"); // [0..10239]

  // Optional
  // Selected BandCombinationIndex
  assert(src->bnd_comb_idx== NULL && "Not implemented"); 

  // Optional
  // Selected FeatureSetEntryIndex
  assert(src->sel_feature_set_entry_idx== NULL && "Not implemented"); 

  // Optional
  // Ph-InfoSCG
  assert(src->ph_info_scg== NULL && "Not implemented"); 

  // Optional
  // Requested BandCombinationIndex
  assert(src->req_bnd_cmb_idx== NULL && "Not implemented"); 

  // Optional
  // Requested FeatureSetEntryIndex
  assert(src->req_feat_set_entry_idx== NULL && "Not implemented"); 

  // Optional
  // DRX Config
  assert(src->drx_cnfg== NULL && "Not implemented"); 

  // Optional
  // PDCCH BlindDetectionSCG
  assert(src->pdcch_blind_det_scg== NULL && "Not implemented"); 

  // Optional
  // Requested PDCCH BlindDetectionSCG
  assert(src->req_pdcch_blnd_det_scg== NULL && "Not implemented"); 

  // Optional
  // Ph-InfoMCG
  assert(src->ph_info_mcg== NULL && "Not implemented"); 

  // Optional
  // MeasGapSharingConfig 
  assert(src->meas_gap_shr_conf== NULL && "Not implemented"); 

  // Optional
  // SL-PHY-MAC-RLC-Config
  assert(src->sl_phy_mac_rlc_conf== NULL && "Not implemented"); 

  // Optional
  // SL-ConfigDedicatedEUTRA-Info 
  assert(src->sl_conf_dedica_eutra_info== NULL && "Not implemented"); 

  // Optional
  // Requested P-MaxFR2 
  assert(src->req_p_max_fr2== NULL && "Not implemented"); 

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_du_to_cu_rrc_info_stp_rsp(du_to_cu_rrc_information_f1ap_t const* src)
{
  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_DUtoCURRCInformation;	
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_DUtoCURRCInformation;

  dst->value.choice.DUtoCURRCInformation = cp_du_to_cu_rrc_info(src);

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_c_rnti_stp_rsp(uint16_t c_rnti)
{
  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_C_RNTI;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_C_RNTI;

  // 9.3.1.32 [0, 65535]
  dst->value.choice.C_RNTI = c_rnti;

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_res_coord_trans_cont(byte_array_t ba)
{
  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present =  F1AP_UEContextSetupResponseIEs__value_PR_ResourceCoordinationTransferContainer;

  dst->value.choice.ResourceCoordinationTransferContainer = copy_ba_to_ostring(ba);

  return dst;
}

static
F1AP_DLUPTNLInformation_ToBeSetup_Item_t* cp_dl_up_tnl_info_tbs(dl_up_trans_layer_info_t const* src)
{
  assert(src != NULL);

  F1AP_DLUPTNLInformation_ToBeSetup_Item_t* dst = calloc(1, sizeof(F1AP_DLUPTNLInformation_ToBeSetup_Item_t));
  assert(dst != NULL && "Memory exhausted");
 
  // 9.3.2.1
  dst->dLUPTNLInformation.present = F1AP_UPTransportLayerInformation_PR_gTPTunnel; 
  dst->dLUPTNLInformation.choice.gTPTunnel = calloc(1, sizeof(F1AP_GTPTunnel_t) );
  assert(dst->dLUPTNLInformation.choice.gTPTunnel != NULL && "Memory exhausted");

  byte_array_t tmp = {.len = 4, .buf = (uint8_t*)src->gtp_teid };
  dst->dLUPTNLInformation.choice.gTPTunnel->gTP_TEID = copy_ba_to_ostring(tmp); 
  dst->dLUPTNLInformation.choice.gTPTunnel->transportLayerAddress = copy_bit_string(&src->trans_layer_addr);

  return dst;
}

static
F1AP_DRBs_Setup_ItemIEs_t* cp_drb_stp_lst_it_stp_rsp(drb_setup_item_f1ap_t const* src)
{
  assert(src != NULL);
  F1AP_DRBs_Setup_ItemIEs_t* dst = calloc(1, sizeof(F1AP_DRBs_Setup_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_DRBs_Setup_Item;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_DRBs_Setup_ItemIEs__value_PR_DRBs_Setup_Item;

  // Mandatory
  // DRB ID 9.3.1.8
  assert(src->drb_id > 0 && src->drb_id < 33);
  dst->value.choice.DRBs_Setup_Item.dRBID = src->drb_id;

  // Optional
  // LCID 9.3.1.35
  if(src->lc_id != NULL){ // [1, 32]
    assert(*src->lc_id > 0 && *src->lc_id < 33);
    dst->value.choice.DRBs_Setup_Item.lCID = calloc(1, sizeof(F1AP_LCID_t));
    assert(dst->value.choice.DRBs_Setup_Item.lCID != NULL && "Memory exhausted");
    *dst->value.choice.DRBs_Setup_Item.lCID = *src->lc_id; 
  }

  // DL UP TNL Information to be setup // [1, 2]
  assert(src->sz_dl_up_tnl_info_tbs == 1 || src-> sz_dl_up_tnl_info_tbs == 2);
  for(size_t i = 0; i < src->sz_dl_up_tnl_info_tbs; ++i){
    F1AP_DLUPTNLInformation_ToBeSetup_Item_t* dl_up_tnl = cp_dl_up_tnl_info_tbs(&src->dl_up_tnl_info_tbs[i]); // src->sz_drb_setup_item, src->drb_setup_item);  
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.DRBs_Setup_Item.dLUPTNLInformation_ToBeSetup_List.list, dl_up_tnl);
    assert(rc == 0);
  } 

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  assert(src-> add_pdcp_dup_tnl_lst == NULL && "Not implemented");

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  assert(src-> cur_qos_par_set_idx == NULL && "Not implemented");

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_drb_stp_lst_stp_rsp(size_t sz, drb_setup_item_f1ap_t item[sz])
{
  assert(sz > 0);

  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_DRBs_Setup_List;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_DRBs_Setup_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_DRBs_Setup_ItemIEs_t* id = cp_drb_stp_lst_it_stp_rsp(&item[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.DRBs_Setup_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

      










static
F1AP_SRBs_Setup_ItemIEs_t* cp_srb_stp_lst_it_stp_rsp(srb_setup_item_t const* src)
{
  assert(src != NULL);
  F1AP_SRBs_Setup_ItemIEs_t* dst = calloc(1, sizeof(F1AP_SRBs_Setup_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_SRBs_Setup_Item;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_SRBs_Setup_ItemIEs__value_PR_SRBs_Setup_Item;

  // Mandatory
  // SRB ID 9.3.1.7
  assert(src->srb_id < 4); // [0,3]
  dst->value.choice.SRBs_Setup_Item.sRBID = src->srb_id;

  // Mandatory
  // LCID 9.3.1.35
  assert(src->lc_id > 0 && src->lc_id < 33); // [1,32]
  dst->value.choice.SRBs_Setup_Item.lCID = src->lc_id; 

  return dst;
}

static
F1AP_UEContextSetupResponseIEs_t* cp_srb_stp_lst_stp_rsp(size_t sz, srb_setup_item_t item[sz])
{
  assert(sz > 0);

  F1AP_UEContextSetupResponseIEs_t* dst = calloc(1, sizeof(F1AP_UEContextSetupResponseIEs_t));
  assert(dst != NULL && "memory exhausted");
  dst->id = F1AP_ProtocolIE_ID_id_SRBs_Setup_List;	
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_UEContextSetupResponseIEs__value_PR_SRBs_Setup_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_SRBs_Setup_ItemIEs_t* id = cp_srb_stp_lst_it_stp_rsp(&item[i]); 
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.SRBs_Setup_List.list, id);
    assert(rc == 0);
  }

  return dst;
}


F1AP_F1AP_PDU_t cp_ue_ctx_setup_response_asn(ue_ctx_setup_response_t const* src)
{
  assert(src != NULL);

  F1AP_F1AP_PDU_t pdu = {0}; 

  /* Create */
  /* 0. pdu Type */
  pdu.present =  F1AP_F1AP_PDU_PR_successfulOutcome;
  pdu.choice.successfulOutcome = calloc(1, sizeof(struct F1AP_SuccessfulOutcome));
  assert(pdu.choice.successfulOutcome != NULL && "Memory exahusted");

  F1AP_SuccessfulOutcome_t* dst_out = pdu.choice.successfulOutcome;

  dst_out->procedureCode = F1AP_ProcedureCode_id_UEContextSetup;
  dst_out->criticality = F1AP_Criticality_reject;
  dst_out->value.present = F1AP_SuccessfulOutcome__value_PR_UEContextSetupResponse;
  F1AP_UEContextSetupResponse_t *dst = & dst_out->value.choice.UEContextSetupResponse;

  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  F1AP_UEContextSetupResponseIEs_t* id = cp_gnb_cu_ue_stp_rsp(src->gnb_cu_ue_id);
  int rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
  assert(rc == 0);

    // Mandatory
  // gNB-DU UE F1AP ID 9.3.1.5
  id = cp_gnb_du_ue_stp_rsp(src->gnb_du_ue_id);
  rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
  assert(rc == 0);

  // Mandatory 
  // DU To CU RRC Information 9.3.1.26
  id = cp_du_to_cu_rrc_info_stp_rsp(&src->du_to_cu_rrc_info);  
  rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
  assert(rc == 0);

  // Optional
  // C-RNTI 9.3.1.32
  if(src->c_rnti != NULL){
    id = cp_c_rnti_stp_rsp(*src->c_rnti);  
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // Optional
  // Resource Coordination Transfer Container 
  if(src->res_coord_trans_cont != NULL){
    id = cp_res_coord_trans_cont(*src->res_coord_trans_cont);  
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // Optional
  // Full Configuration 
  assert(src->full_config == NULL && "Not implemented");

  // DRB Setup Item List [0, 64]
  if(src->sz_drb_setup_item > 0){
    assert(src->sz_drb_setup_item < 65);
    id = cp_drb_stp_lst_stp_rsp(src->sz_drb_setup_item, src->drb_setup_item);  
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // SRB Failed to Setup List [0,8]
  assert(src->sz_srb_failed_setup_item == 0 && "Not implemented");
  assert(src->srb_failed_setup_item == NULL && "Not implemented");

  // DRB Failed to Setup List [0, 64]
  assert(src->sz_drb_failed_setup_item == 0 && "Not implemented");
  assert(src->drb_failed_setup_item == NULL && "Not implemented");

  // SCell Failed To Setup List [0, 32]
  assert(src->scell_failed_setup_item == NULL && "Not implemented");

  // Optional
  // Inactivity Monitoring Response
  assert(src->inactivity_mon_response == NULL && "Not implemented");

  // Optional
  // Criticality Diagnostics 9.3.1.3
  assert(src->crit_diagn == NULL && "Not implemented");

  // SRB Setup List [0, 8]
  if(src->sz_srb_setup_item > 0){
    assert(src->sz_srb_setup_item < 9);

    id = cp_srb_stp_lst_stp_rsp(src->sz_srb_setup_item, src->srb_setup_item);  
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // BH RLC Channel Setup List [0,1]
  // 9.3.1.113
  assert(src->bh_rlc_setup_lst == NULL && "Not implemented");

  // BH RLC Channel Failed to be Setup List
  assert(src->bh_rlc_chn_failed_tbs_lst == NULL && "Not implemented");

  // SL DRB Setup List
  // 9.3.1.120
  assert(src->sl_drb_setup_lst == NULL && "Not implemented");// [1, 512]

  // SL DRB Failed To Setup List
  assert(src->sl_drb_failed_to_setup_lst == NULL && "Not implemented");  

  // Optional
  // Requested Target Cell ID 9.3.1.12 
  assert(src->req_target_cell_id == NULL && "Not implemented");

  return pdu;
}


/////
/////
/////

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_trans_id_cu_conf_up(uint8_t trans_id)
{
  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
  
  dst->id = F1AP_ProtocolIE_ID_id_TransactionID;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_TransactionID;
  dst->value.choice.TransactionID = trans_id;

  return dst;
}

/*
static
F1AP_GNB_CUSystemInformation_ExtIEs_t* cp_gnb_cu_system_info_cu_conf_up(gnb_cu_sys_info_t const* src)
{
  assert(src != NULL);

  F1AP_GNB_CUSystemInformation_ExtIEs_t

  F1AP_GNB_CUSystemInformation_ExtIEs_t* dst = calloc(1, sizeof(F1AP_GNB_CUSystemInformation_ExtIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = ; 

  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_TransactionID;
  dst->extensionValue.choice.
  dst->extensionValue.choice.SystemInformationAreaID.

  dst->value.choice.TransactionID = trans_id;

  assert(src->sz_sib > 0 && src->sz_sib < 33);

  size_t sz_sib; // [1,32]
  sib_t* sib;

  // Optional. Bit String of 24
  sys_info_area_id_t* id;

}
*/

static
F1AP_Cells_to_be_Activated_List_ItemIEs_t* cp_act_cell_cu_conf_up(activate_cell_t const* src)
{
  F1AP_Cells_to_be_Activated_List_ItemIEs_t* dst = calloc(1, sizeof(F1AP_Cells_to_be_Activated_List_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List_Item;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item;

  F1AP_Cells_to_be_Activated_List_Item_t* dst_it = &dst->value.choice.Cells_to_be_Activated_List_Item;
  // Mandatory
  // NR CGI 
  dst_it->nRCGI = cp_nr_cgi(&src->nr_cgi);

  // Optional
  // NR PCI
  if(src->nr_pci != NULL){
   dst_it->nRPCI = calloc(1, sizeof(F1AP_NRPCI_t));
   assert(dst_it->nRPCI != NULL);
   *dst_it->nRPCI = cp_nr_pci(*src->nr_pci);
  }

  if(src->sys_info != NULL || src->avail_plmn != NULL 
      || src->ext_avail_plmn != NULL || src->ab_info != NULL 
      || src->av_snpn != NULL){
    dst_it->iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P46_t ));
    assert(dst_it->iE_Extensions != NULL && "Memory exhausted");
  }

  if(src->sys_info != NULL){
    cp_gnb_cu_system_info(src->sys_info, ((F1AP_ProtocolExtensionContainer_154P46_t*)dst_it->iE_Extensions));
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

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_cell_act_cu_conf_up(size_t sz, activate_cell_t act[sz])
{
  assert(sz < 513);
  assert(act != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Activated_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_Cells_to_be_Activated_List_ItemIEs_t* id = cp_act_cell_cu_conf_up(&act[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Cells_to_be_Activated_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_Cells_to_be_Deactivated_List_ItemIEs_t* cp_deact_cell_cu_conf_up(cells_to_be_deact_t const* deact)
{
  F1AP_Cells_to_be_Deactivated_List_ItemIEs_t* dst = calloc(1, sizeof(F1AP_Cells_to_be_Deactivated_List_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Deactivated_List_Item; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_Cells_to_be_Deactivated_List_ItemIEs__value_PR_Cells_to_be_Deactivated_List_Item;

  dst->value.choice.Cells_to_be_Deactivated_List_Item.nRCGI = cp_nr_cgi(&deact->nr_cgi);

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_cell_deact_cu_conf_up(size_t sz, cells_to_be_deact_t deact[sz])
{
  assert(sz < 513);
  assert(deact != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Deactivated_List; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Deactivated_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_Cells_to_be_Deactivated_List_ItemIEs_t* id = cp_deact_cell_cu_conf_up(&deact[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Cells_to_be_Deactivated_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_TNLAssociationUsage_t cp_tnl_assoc_usage(tnl_assoc_usage_e src)
{
  F1AP_TNLAssociationUsage_t dst = {0}; 

  if(src == UE_TNL_ASSOC_USAGE)
    dst = F1AP_TNLAssociationUsage_ue;	
  else if(src == NON_UE_TNL_ASSOC_USAGE )
    dst = F1AP_TNLAssociationUsage_non_ue;	
  else if(src == BOTH_TNL_ASSOC_USAGE)
    dst = F1AP_TNLAssociationUsage_both;	
  else
    assert(0!=0 && "Unknown enum type");
 
  return dst;
}

static
struct F1AP_Endpoint_IP_address_and_port* cp_ip_addr_port(endpoint_ip_addr_port_t const* src)
{
  assert(src != NULL);

 struct F1AP_Endpoint_IP_address_and_port* dst = calloc(1, sizeof(struct F1AP_Endpoint_IP_address_and_port));
  assert(dst != NULL && "memory exhausted");

  dst->endpointIPAddress = copy_bit_string(&src->endpoint_ip_addr.trans_layer_add);

  dst->iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P93_t));
  assert(dst->iE_Extensions != NULL && "Memory exhausted");

  F1AP_Endpoint_IP_address_and_port_ExtIEs_t* ip_addr_port = calloc(1, sizeof(F1AP_Endpoint_IP_address_and_port_ExtIEs_t));
  assert(ip_addr_port != NULL && "Memory exhausted");

  ip_addr_port->id = F1AP_ProtocolIE_ID_id_portNumber; 
  ip_addr_port->criticality = F1AP_Criticality_reject; 
  ip_addr_port->extensionValue.present = F1AP_Endpoint_IP_address_and_port_ExtIEs__extensionValue_PR_PortNumber; 

  ip_addr_port->extensionValue.choice.PortNumber = copy_bit_string(&src->port);

  int const rc = ASN_SEQUENCE_ADD(&((F1AP_ProtocolExtensionContainer_154P93_t*)dst->iE_Extensions)->list, ip_addr_port);
  assert(rc == 0);

  return dst;
}

static
F1AP_CP_TransportLayerAddress_t cp_tnl_assoc_tran_layer_addr(cp_trans_layer_info_t const* src)
{
  assert(src != NULL);

  F1AP_CP_TransportLayerAddress_t dst = {0}; 

  if(src->type == IP_ADDRESS_CP_TRANS_LAYER_INFO ){
    dst.present = F1AP_CP_TransportLayerAddress_PR_endpoint_IP_address;
    dst.choice.endpoint_IP_address = copy_bit_string(&src->ep_ip_addr.trans_layer_add);
  } else if (src->type == IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO){
    dst.present = F1AP_CP_TransportLayerAddress_PR_endpoint_IP_address_and_port;
    dst.choice.endpoint_IP_address_and_port = cp_ip_addr_port(&src->ep_ip_addr_port);
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

static
F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t* cp_tnl_assoc_to_add_it_cu_conf_up(gnb_cu_tnl_asso_to_add_t const* src)
{
  assert(src != NULL);

  F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t* dst = calloc(1, sizeof(F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Add_Item;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Add_Item;


  // TNL Association Transport Layer Information 9.3.2.4
  // Mandatory
  dst->value.choice.GNB_CU_TNL_Association_To_Add_Item.tNLAssociationTransportLayerAddress = cp_tnl_assoc_tran_layer_addr( &src->cp_trans_layer_info ); 

  // TNL Association Usage  
  // Mandatory
  dst->value.choice.GNB_CU_TNL_Association_To_Add_Item.tNLAssociationUsage = cp_tnl_assoc_usage(src->tnl_assoc_usage)  ;

    return dst;
}


static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_tnl_assoc_to_add_cu_conf_up(size_t sz, gnb_cu_tnl_asso_to_add_t tnl[sz])
{
  assert(sz < 33);
  assert(tnl != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Add_List; 
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present =  F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Add_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t* id = cp_tnl_assoc_to_add_it_cu_conf_up(&tnl[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.GNB_CU_TNL_Association_To_Add_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t* cp_tnl_assoc_to_rm_it_cu_conf_up(gnb_cu_tnl_assoc_to_rem_t const* src)
{
  assert(src != NULL);

  F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t* dst = calloc(1, sizeof(F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_Item;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Remove_Item;

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst->value.choice.GNB_CU_TNL_Association_To_Remove_Item.tNLAssociationTransportLayerAddress =  cp_tnl_assoc_tran_layer_addr(&src->tnl_assoc_trans);

  // TNL Association Transport Layer Address gNB-DU
  // Optional
  if(src->tnl_assoc_trans_du != NULL){
    dst->value.choice.GNB_CU_TNL_Association_To_Remove_Item.iE_Extensions = calloc(1, sizeof(struct F1AP_ProtocolExtensionContainer_154P116));
    assert(dst->value.choice.GNB_CU_TNL_Association_To_Remove_Item.iE_Extensions != NULL && "Memory exhausted");

    F1AP_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs_t* ie = calloc(1, sizeof(F1AP_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs_t));
    assert(ie != NULL && "Memory exhausted");

    // It should be something similar to F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs, but could
    // not find the IE id in the standard 
    ie->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_Item;
    ie->criticality = F1AP_Criticality_reject;
    ie->extensionValue.present = F1AP_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs__extensionValue_PR_CP_TransportLayerAddress;
    ie->extensionValue.choice.CP_TransportLayerAddress = cp_tnl_assoc_tran_layer_addr(src->tnl_assoc_trans_du);

    int rc = ASN_SEQUENCE_ADD(&((struct F1AP_ProtocolExtensionContainer_154P116*)dst->value.choice.GNB_CU_TNL_Association_To_Remove_Item.iE_Extensions)->list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_tnl_assoc_to_rm_cu_conf_up(size_t sz, gnb_cu_tnl_assoc_to_rem_t tnl[sz])
{
  assert(sz < 33);
  assert(tnl != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_List; 
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present =  F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Remove_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t* id = cp_tnl_assoc_to_rm_it_cu_conf_up(&tnl[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.GNB_CU_TNL_Association_To_Remove_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t* cp_tnl_assoc_to_upd_it_cu_conf_up(gnb_cu_tnl_assoc_to_upd_t const* src)
{
  assert(src != NULL);

  F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t* dst = calloc(1, sizeof(F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Update_Item;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Update_Item;

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst->value.choice.GNB_CU_TNL_Association_To_Update_Item.tNLAssociationTransportLayerAddress = cp_tnl_assoc_tran_layer_addr(&src->tnl_assoc_trans_addr);

  // TNL Association Usage
  // Optional
  if(src->tnl_association_usage != NULL){
    dst->value.choice.GNB_CU_TNL_Association_To_Update_Item.tNLAssociationUsage = calloc(1, sizeof(F1AP_TNLAssociationUsage_t)); 
    assert(dst->value.choice.GNB_CU_TNL_Association_To_Update_Item.tNLAssociationUsage != NULL && "Memory exhausted");
    *dst->value.choice.GNB_CU_TNL_Association_To_Update_Item.tNLAssociationUsage = cp_tnl_assoc_usage(*src->tnl_association_usage);
  }

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_tnl_assoc_to_upd_cu_conf_up(size_t sz, gnb_cu_tnl_assoc_to_upd_t tnl[sz])
{
  assert(sz < 33);
  assert(tnl != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Update_List; 
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Update_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t* id = cp_tnl_assoc_to_upd_it_cu_conf_up(&tnl[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.GNB_CU_TNL_Association_To_Update_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_CellBarred_t cp_cell_barred(cell_barred_e src)
{
  assert(src == BARRED_CELL_BARRED || src == NOT_BARRED_CELL_BARRED);

  F1AP_CellBarred_t dst = {0}; 

  if(src == BARRED_CELL_BARRED){
    dst = F1AP_CellBarred_barred;
  } else if(src == NOT_BARRED_CELL_BARRED){
    dst = F1AP_CellBarred_not_barred;
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

static
F1AP_IAB_Barred_t	cp_cell_iab_barred(cell_barred_e src)
{
  assert(src == BARRED_CELL_BARRED || src == NOT_BARRED_CELL_BARRED);

 F1AP_IAB_Barred_t dst = {0}; 

  if(src == BARRED_CELL_BARRED){
    dst = F1AP_IAB_Barred_barred;
  } else if(src == NOT_BARRED_CELL_BARRED){
    dst = F1AP_IAB_Barred_not_barred;
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

static
F1AP_Cells_to_be_Barred_ItemIEs_t* cp_cell_to_be_barred_it_cu_conf_up(cells_to_be_barred_t const* src)
{
  assert(src != NULL);

  F1AP_Cells_to_be_Barred_ItemIEs_t* dst = calloc(1, sizeof(F1AP_Cells_to_be_Barred_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Barred_Item;
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_Cells_to_be_Barred_ItemIEs__value_PR_Cells_to_be_Barred_Item;

  // NR CGI 9.3.1.12
  // Mandatory
  dst->value.choice.Cells_to_be_Barred_Item.nRCGI = cp_nr_cgi(&src->nr_cgi);

  // Cell Barred 
  // Mandatory
 dst->value.choice.Cells_to_be_Barred_Item.cellBarred = cp_cell_barred(src->cell_barred);

  // IAB Barred 
  // Optional
  if(src-> iab_barred != NULL){
    dst->value.choice.Cells_to_be_Barred_Item.iE_Extensions = calloc(1, sizeof(F1AP_ProtocolExtensionContainer_154P48_t)); 
    assert( dst->value.choice.Cells_to_be_Barred_Item.iE_Extensions != NULL && "Memory exhausted");

    F1AP_Cells_to_be_Barred_Item_ExtIEs_t * ie = calloc(1, sizeof(F1AP_Cells_to_be_Barred_Item_ExtIEs_t)); 
    assert(ie != NULL && "Memory exhausted");
    
    ie->id = F1AP_ProtocolIE_ID_id_IAB_Barred;
    ie->criticality = F1AP_Criticality_ignore;
    ie->extensionValue.present = F1AP_Cells_to_be_Barred_Item_ExtIEs__extensionValue_PR_IAB_Barred;

    ie->extensionValue.choice.IAB_Barred = cp_cell_iab_barred(*src->iab_barred);
;
    int const rc = ASN_SEQUENCE_ADD(& ((F1AP_ProtocolExtensionContainer_154P48_t*)dst->value.choice.Cells_to_be_Barred_Item.iE_Extensions)->list, ie);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_cell_to_be_barred_cu_conf_up(size_t sz, cells_to_be_barred_t src[sz])
{
  assert(sz < 513);
  assert(src != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");
 
  dst->id = F1AP_ProtocolIE_ID_id_Cells_to_be_Barred_List; 
  dst->criticality = F1AP_Criticality_ignore;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Barred_List;

  for(size_t i = 0; i < sz; ++i){
  F1AP_Cells_to_be_Barred_ItemIEs_t* id = cp_cell_to_be_barred_it_cu_conf_up(&src[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Cells_to_be_Barred_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_EUTRA_FDD_Info_t* cp_eutra_cell_info_fdd(fdd_info_serv_eutra_cell_info_t src)
{
  F1AP_EUTRA_FDD_Info_t* dst = calloc(1, sizeof(F1AP_EUTRA_FDD_Info_t));

  // [0,2199]
  assert(src.ul_offset_to_point_a < 2200);
  dst->uL_offsetToPointA = src.ul_offset_to_point_a;

  // [0,2199]
  assert(src.dl_offset_to_point_a < 2200);
  dst->dL_offsetToPointA = src.dl_offset_to_point_a;

  return dst;
}

static
F1AP_EUTRA_TDD_Info_t* cp_eutra_cell_info_tdd(tdd_info_serv_eutra_cell_info_t src)
{

  F1AP_EUTRA_TDD_Info_t* dst = calloc(1, sizeof(F1AP_EUTRA_TDD_Info_t));
  assert(dst != NULL && "Memory exhausted");

  // [0,2199]
  assert(src.offset_to_point_a < 2200);
  dst->offsetToPointA = src.offset_to_point_a;

  return dst;
}

static
F1AP_Served_EUTRA_Cells_Information_t cp_serv_eutra_cell_info(serv_eutra_cell_info_t const* src)
{
  assert(src != NULL);

  F1AP_Served_EUTRA_Cells_Information_t dst = {0}; 

  assert(src->type == FDD_INFO_SERV_EUTRA_CELL_INFO || src->type == TDD_INFO_SERV_EUTRA_CELL_INFO);

  if(src->type == FDD_INFO_SERV_EUTRA_CELL_INFO){
    dst.eUTRA_Mode_Info.present = F1AP_EUTRA_Mode_Info_PR_eUTRAFDD;
    dst.eUTRA_Mode_Info.choice.eUTRAFDD = cp_eutra_cell_info_fdd(src->fdd);
  } else if(src->type == TDD_INFO_SERV_EUTRA_CELL_INFO){
    dst.eUTRA_Mode_Info.present = F1AP_EUTRA_Mode_Info_PR_eUTRATDD;
    dst.eUTRA_Mode_Info.choice.eUTRATDD = cp_eutra_cell_info_tdd(src->tdd);
  } else {
    assert(0 !=0 && "Unknown type");
  }

  //Optional
  assert(src->prot_eutra_resource_ind == NULL && "Not implemented" );

  return dst;
}

static
F1AP_EUTRACells_List_item_t* cp_eutra_cell_it_cu_conf_up(eutra_cell_t const* src)
{
  assert(src != NULL);

  F1AP_EUTRACells_List_item_t* dst = calloc(1, sizeof(F1AP_EUTRACells_List_item_t));
  assert(dst != NULL && "Memory exhausted");

  // EUTRA Cell ID
  // Mandatory // [28]
  assert(src->eutra_cell_id.size == 4 && src->eutra_cell_id.bits_unused == 4);
  dst->eUTRA_Cell_ID = copy_bit_string(&src->eutra_cell_id);

  // Served E-UTRA CellInformation 9.3.1.64
  // Mandatory
  dst->served_EUTRA_Cells_Information = cp_serv_eutra_cell_info(&src->serv_eutra_cell_info); 

  return dst;
}

static
F1AP_Protected_EUTRA_Resources_ItemIEs_t* cp_prot_eutra_res_it_cu_conf_up(prot_eutra_resources_t const* src)
{
  assert(src != NULL);

  F1AP_Protected_EUTRA_Resources_ItemIEs_t* dst = calloc(1, sizeof(F1AP_Protected_EUTRA_Resources_ItemIEs_t));
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_Protected_EUTRA_Resources_Item;
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_Protected_EUTRA_Resources_ItemIEs__value_PR_Protected_EUTRA_Resources_Item;

  // Spectrum sharing group [1, 256]
  assert(src->spec_sharing_group_id > 0 && src->spec_sharing_group_id < 257);
  dst->value.choice.Protected_EUTRA_Resources_Item.spectrumSharingGroupID = src->spec_sharing_group_id;

  // EUTRA Cell                                
  assert(src->sz_eutra_cell > 0 && src->sz_eutra_cell < 257);
  for(size_t i =0; i < src->sz_eutra_cell ; ++i){
    F1AP_EUTRACells_List_item_t* id = cp_eutra_cell_it_cu_conf_up(&src->eutra_cell[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Protected_EUTRA_Resources_Item.eUTRACells_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

static
F1AP_GNBCUConfigurationUpdateIEs_t* cp_prot_eutra_res_cu_conf_up(size_t sz,prot_eutra_resources_t prot[sz])
{
  assert(sz < 257);
  assert(prot != NULL);

  F1AP_GNBCUConfigurationUpdateIEs_t* dst = calloc(1, sizeof(F1AP_GNBCUConfigurationUpdateIEs_t)); 
  assert(dst != NULL && "Memory exhausted");

  dst->id = F1AP_ProtocolIE_ID_id_Protected_EUTRA_Resources_List; 
  dst->criticality = F1AP_Criticality_reject;
  dst->value.present = F1AP_GNBCUConfigurationUpdateIEs__value_PR_Protected_EUTRA_Resources_List;

  for(size_t i = 0; i < sz; ++i){
    F1AP_Protected_EUTRA_Resources_ItemIEs_t* id = cp_prot_eutra_res_it_cu_conf_up(&prot[i]);
    int const rc = ASN_SEQUENCE_ADD(&dst->value.choice.Protected_EUTRA_Resources_List.list, id);
    assert(rc == 0);
  }

  return dst;
}

F1AP_F1AP_PDU_t cp_gnb_cu_conf_update_asn(gnb_cu_conf_update_t const* src)
{
  assert(src != NULL);

  F1AP_F1AP_PDU_t pdu = {0}; 

  /* Create */
  /* 0. pdu Type */
  pdu.present =  F1AP_F1AP_PDU_PR_initiatingMessage;
  pdu.choice.initiatingMessage= calloc(1, sizeof(F1AP_InitiatingMessage_t));
  assert(pdu.choice.initiatingMessage != NULL && "Memory exahusted");

  F1AP_InitiatingMessage_t* dst_out = pdu.choice.initiatingMessage;

  dst_out->procedureCode = F1AP_ProcedureCode_id_gNBCUConfigurationUpdate;
  dst_out->criticality = F1AP_Criticality_reject;
  dst_out->value.present = F1AP_InitiatingMessage__value_PR_GNBCUConfigurationUpdate;

  F1AP_GNBCUConfigurationUpdate_t *dst = & dst_out->value.choice.GNBCUConfigurationUpdate;

  // Mandatory
  // Transaction ID 9.3.1.23
  F1AP_GNBCUConfigurationUpdateIEs_t* id = cp_trans_id_cu_conf_up(src->trans_id);
  int rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
  assert(rc == 0);

  // Cells to be Activated List [0,512]
  assert(src->sz_cells_to_be_act < 513);
  if(src->sz_cells_to_be_act > 0){
    id = cp_cell_act_cu_conf_up(src->sz_cells_to_be_act, src->cells_to_be_act);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }
  
  // Cells to be Deactivated List [0,512]
  assert(src->sz_cells_to_be_deact < 513);
  if(src->sz_cells_to_be_deact > 0){
    id = cp_cell_deact_cu_conf_up(src->sz_cells_to_be_deact, src->cells_to_be_deact);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // TNL Association To Add List
  assert(src->sz_gnb_cu_tnl_asso_to_add < 33);
  if(src->sz_gnb_cu_tnl_asso_to_add > 0){
    id = cp_tnl_assoc_to_add_cu_conf_up(src->sz_gnb_cu_tnl_asso_to_add, src->gnb_cu_tnl_asso_to_add);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // TNL Association To Remove List // [0,32] 
  assert(src->sz_gnb_cu_tnl_assoc_to_rem < 33);
  if(src->sz_gnb_cu_tnl_assoc_to_rem > 0){
    id = cp_tnl_assoc_to_rm_cu_conf_up(src->sz_gnb_cu_tnl_assoc_to_rem, src->gnb_cu_tnl_assoc_to_rem);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // TNL Association To Update List // [0,32] 
  assert(src->sz_gnb_cu_tnl_assoc_to_upd < 33);
  if(src->sz_gnb_cu_tnl_assoc_to_upd > 0){
    id = cp_tnl_assoc_to_upd_cu_conf_up(src->sz_gnb_cu_tnl_assoc_to_upd, src->gnb_cu_tnl_assoc_to_upd);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // Cells to be Barred // [0, 512] 
  assert(src->sz_cells_to_be_barred < 513);
  if(src->sz_cells_to_be_barred > 0){
    id = cp_cell_to_be_barred_cu_conf_up(src->sz_cells_to_be_barred, src->cells_to_be_barred);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // Protected EUTRA Resources 
  assert(src->sz_prot_eutra_resources < 257);
  if(src->sz_prot_eutra_resources > 0){
    id = cp_prot_eutra_res_cu_conf_up(src->sz_prot_eutra_resources, src->prot_eutra_resources);
    rc = ASN_SEQUENCE_ADD(&dst->protocolIEs.list, id);
    assert(rc == 0);
  }

  // [0, 512]
  assert(src->sz_neighbour_cell_info == 0 && "Not implemented");
  assert(src->neighbour_cell_info == NULL && "Not implemented");

  // Transport Layer Address Info 9.3.2.5
  // Optional
  assert(src->trans_layer_add_info == NULL && "Not implemented");


  // Uplink BH Non-UP Traffic Mapping 9.3.1.103
  // Optional
  assert(src->up_bh_non_up_traff_map == NULL && "Not implemented"); 

  // BAP Address 9.3.1.111
  // Optional
  assert(src->bap_address == NULL && "Not implemented");

  return pdu;
}

