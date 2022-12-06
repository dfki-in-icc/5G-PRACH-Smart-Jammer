#include "cp_asn_to_ir.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "f1ap_types/activate_cell.h"
#include "f1ap_types/s_nssai.h"
#include "f1ap_types/gnb_cu_sys_info.h"
#include "f1ap_types/gnb_du_srv_cell.h"
#include "f1ap_types/gnb_rrc_ver.h"
#include "f1ap_types/sib.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SliceSupportItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_FreqBandNrItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_FreqBandNrItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SupportedSULFreqBandItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SibtypetobeupdatedListItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_InitiatingMessage.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SuccessfulOutcome.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolIE-Field.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolExtensionField.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ServedPLMNs-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_NRFreqInfo.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Transmission-Bandwidth.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_FDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_TDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ProtocolExtensionContainer.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Flows-Mapped-To-DRB-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ULUPTNLInformation-ToBeSetup-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_GTPTunnel.h" 
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_NonDynamic5QIDescriptor.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Dynamic5QIDescriptor.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_AdditionalPDCPDuplicationTNL-Item.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRANQoS.h"


#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_UnsuccessfulOutcome.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-List.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CriticalityDiagnostics-IE-Item.h"

#include "../../../openair3/UTILS/conversions.h"


typedef struct{
 activate_cell_t* cell; 
 size_t len;
} span_activate_cell_t; 

typedef struct{
  size_t len;
  s_nssai_t* tai_slice_sup_lst;
} s_nssai_arr_t ;

typedef struct{
  size_t sz_srv_cell; 
  gnb_du_srv_cell_f1ap_t* srv_cell;
} srv_cell_span_t;

static
uint32_t cp_ostring_to_uint32(OCTET_STRING_t src)
{
  assert(src.size < 5 && src.size > 0);
  uint32_t dst = {0};
  memcpy(&dst, src.buf, src.size);
  return dst;
//  return ntohl(dst);
}

static
uint16_t cp_ostring_to_uint16(OCTET_STRING_t src)
{
  assert(src.size < 3 && src.size > 0);
  uint32_t dst = {0};
  memcpy(&dst, src.buf, src.size);
  return dst;
//  return ntohs(dst);
}

static
byte_array_t copy_ostring_to_ba(OCTET_STRING_t src)
{
  byte_array_t dst = { .len = src.size};
  dst.buf = malloc(src.size);
  memcpy(dst.buf, src.buf, src.size);
  return dst;
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

uint8_t cp_trans_id_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->id == F1AP_ProtocolIE_ID_id_TransactionID);
  assert(ie->criticality == F1AP_Criticality_reject);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_TransactionID);
  
  assert(ie->value.choice.TransactionID < 256);
  return ie->value.choice.TransactionID;
}

uint64_t cp_gnb_du_id_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->id == F1AP_ProtocolIE_ID_id_gNB_DU_ID);
  assert(ie->criticality == F1AP_Criticality_reject);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_ID);

  uint64_t tmp = 0;
  int const rc = asn_INTEGER2ulong(&ie->value.choice.GNB_DU_ID, &tmp);
  assert(rc == 0);
  assert(tmp < (1UL << 36) && "Should not have more than 36 bits");

  return tmp;
}

byte_array_t* cp_gnb_du_name_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->criticality == F1AP_Criticality_ignore);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Name);
  byte_array_t* dst = calloc(1, sizeof(byte_array_t));
  assert(dst != NULL && "Memory exhausted");
  *dst = copy_ostring_to_ba(ie->value.choice.GNB_DU_Name);

  return dst;
}

s_nssai_arr_t cp_tai_slice_sup_lst_ir(F1AP_ServedPLMNs_ItemExtIEs_t  const* src)
{
  s_nssai_arr_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_TAISliceSupportList);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->extensionValue.present == F1AP_ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList);
  F1AP_SliceSupportList_t const* src_slice_sup_lst = &src->extensionValue.choice.SliceSupportList;

  size_t const sz = src_slice_sup_lst->list.count;
  dst.len = sz; 
  dst.tai_slice_sup_lst = calloc(sz, sizeof(s_nssai_t));
  assert( dst.tai_slice_sup_lst != NULL && "Memory exhausted");

  for(size_t i = 0; i < sz; ++i){
    F1AP_SliceSupportItem_t const* src_tai_ss = src_slice_sup_lst->list.array[i]; 
    s_nssai_t* dst_tai_ss = &dst.tai_slice_sup_lst[i];   

    assert(src_tai_ss->sNSSAI.sST.size == 1);
    dst_tai_ss->sst = *src_tai_ss->sNSSAI.sST.buf;

    if(src_tai_ss->sNSSAI.sD != NULL){
      dst_tai_ss->sd = calloc(1, sizeof(byte_array_t) );
      assert(dst_tai_ss->sd != NULL && "Memory exhausted");
      *dst_tai_ss->sd = copy_ostring_to_ba(*src_tai_ss->sNSSAI.sD);
    }
  }

  return dst;
}

srv_plmn_f1ap_t cp_srv_plmn_ir(F1AP_ServedPLMNs_Item_t const* src_srv_plmn)
{
  srv_plmn_f1ap_t dst = {0};

  // mandatory 
  // PLMN Identity 9.3.1.14 
  plmn_f1ap_t* dst_plmn_id = &dst.plmn_id;
  F1AP_PLMN_Identity_t const* src_plmn_id = &src_srv_plmn->pLMN_Identity; 
  TBCD_TO_MCC_MNC(src_plmn_id, dst_plmn_id->mcc,  dst_plmn_id->mnc,  dst_plmn_id->mnc_digit_len);

  // optional 9.3.1.37 
  // TAI Slice support list  
  if(src_srv_plmn->iE_Extensions != NULL){
    s_nssai_arr_t tmp = cp_tai_slice_sup_lst_ir( ((F1AP_ProtocolExtensionContainer_154P34_t const*)src_srv_plmn->iE_Extensions)->list.array[0]);
    dst.tai_slice_sup_lst = tmp.tai_slice_sup_lst;
    dst.sz_tai_slice_sup_lst = tmp.len;
  }

  // optional
  // NPN Support Information

  // optional
  // Extended TAI Slice Support Lis

  return dst;
}



nr_freq_info_f1ap_t cp_frq_info_ir(F1AP_NRFreqInfo_t	const* src)
{
  assert(src != NULL);

  nr_freq_info_f1ap_t dst = {0}; 


  // Mandatory
  // NR ARFCN
  assert(src->nRARFCN < 3279165 + 1);
  dst.nr_arfcn = src->nRARFCN; 

  // Optional
  // 1.2. SUL Information
  assert(src->sul_Information == NULL && "Not implemented");

  // mandatory
  // 1.3 Frequency Band List
  assert(src->freqBandListNr.list.count < 33);
  dst.sz_frq_band = src->freqBandListNr.list.count; 

  dst.frq_band = calloc(dst.sz_frq_band, sizeof(freq_band_f1ap_t) );
  assert(dst.frq_band != NULL && "Memory exhausted" );

  for(int i = 0; i < dst.sz_frq_band; ++i){
    F1AP_FreqBandNrItem_t const* src_frq = src->freqBandListNr.list.array[i];
    freq_band_f1ap_t* dst_frq = &dst.frq_band[i]; 

    // mandatory
    // 1.3.1 freqBandIndicatorNr 
    assert(src_frq->freqBandIndicatorNr < 1025);
    dst_frq->frq_band = src_frq->freqBandIndicatorNr;

    // optional
    /* 1.3.2 supportedSULBandList*/
    if(src_frq->supportedSULBandList.list.count > 0){
      assert(src_frq->supportedSULBandList.list.count < 33);
      dst_frq->sz_sul_band = src_frq->supportedSULBandList.list.count; 
      dst_frq->sul_band = calloc(dst_frq->sz_sul_band, sizeof(uint16_t)); 
      assert( dst_frq->sul_band != NULL && "Memory exhausted" );

      for(int j = 0; j < dst_frq->sz_sul_band; ++j ){
        assert(src_frq->supportedSULBandList.list.array[j]->freqBandIndicatorNr < 1025);
        // mandatory
        // TDD.1.3.2.1 freqBandIndicatorNr 
        dst_frq->sul_band[j] = src_frq->supportedSULBandList.list.array[j]->freqBandIndicatorNr; 
      }
    }

    // optional
    // Frequency Shift 7p5khz
    assert(src->iE_Extensions == NULL && "Not implemented" );
  }

  return dst;
}


trans_bw_f1ap_t cp_trans_bw_ir(F1AP_Transmission_Bandwidth_t const* src)
{
  assert(src != NULL);

  trans_bw_f1ap_t dst = {0}; 

  // Mandatory
  // NR SCS
  dst.nr_scs = src->nRSCS;
  
  // Mandatory
  // NRB
  dst.n_rb = src->nRNRB;

  return dst;
}

fdd_info_f1ap_t cp_fdd_info_ir(F1AP_FDD_Info_t	const* src)
{
  fdd_info_f1ap_t dst = {0}; 

  // Mandatory
  // UL FreqInfo
  dst.ul_frq_info = cp_frq_info_ir(&src->uL_NRFreqInfo);

  // Mandatory
  // DL FreqInfo
  dst.dl_frq_info = cp_frq_info_ir(&src->dL_NRFreqInfo);

  // Mandatory
  // UL Transmission Bandwidth
  dst.ul_bw = cp_trans_bw_ir(&src->uL_Transmission_Bandwidth);

  // Mandatory
  // DL Transmission Bandwidth
  dst.dl_bw = cp_trans_bw_ir(&src->dL_Transmission_Bandwidth);

  // Optional
  //UL Carrier List
  assert(src->iE_Extensions == NULL && "Not implemented");

  // Optional
  // DL Carrier List
  assert(src->iE_Extensions == NULL && "Not implemented");

  return dst;
}

tdd_info_f1ap_t cp_tdd_info_ir(F1AP_TDD_Info_t	const* src)
{
  tdd_info_f1ap_t dst = {0}; 

  // Mandatory
  // NR FreqInfo
  dst.freq_info = cp_frq_info_ir(&src->nRFreqInfo); 

  // Mandatory
  // Transmission Bandwidth
  dst.bw = cp_trans_bw_ir(&src->transmission_Bandwidth);

  // optional
  // Intended TDD DL-UL Configuration
  assert(src->iE_Extensions == NULL && "Not implemented");

  // optional
  // TDD UL-DL Configuration Common NR
  assert(src->iE_Extensions == NULL && "Not implemented");

  // optional
  // Carrier List
  assert(src->iE_Extensions == NULL && "Not implemented");

  return dst;
}


srv_cell_info_t cp_srv_cell_info_ir(F1AP_Served_Cell_Information_t const* src_srv_cell_info)
{
  assert( src_srv_cell_info != NULL);

  srv_cell_info_t dst = {0};

  // mandatory
  // NR CGI
  TBCD_TO_MCC_MNC(&src_srv_cell_info->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_srv_cell_info->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1UL << 36) );

  // mandatory 
  // NR PCI 
  assert(src_srv_cell_info->nRPCI < 1008); 
  dst.nr_pci = src_srv_cell_info->nRPCI;

  // optional 
  // - fiveGS_TAC 
  if(src_srv_cell_info->fiveGS_TAC != NULL) {
      assert(src_srv_cell_info->fiveGS_TAC->size == 3);
      dst.five_gs_tac = malloc(sizeof(uint32_t));
      assert(dst.five_gs_tac != NULL);
      *dst.five_gs_tac = cp_ostring_to_uint32(*src_srv_cell_info->fiveGS_TAC);
  }

    // optional 
    // - Configured_EPS_TAC 
  if(src_srv_cell_info->configured_EPS_TAC != NULL){
    assert(src_srv_cell_info->configured_EPS_TAC->size == 2);
    dst.eps_tac = malloc(sizeof(uint16_t));
    assert(dst.eps_tac != NULL && "Memory exhausted");
    *dst.eps_tac = cp_ostring_to_uint16(*src_srv_cell_info->configured_EPS_TAC);
  } 

    // mandatory 
    // servedPLMN information 
    size_t const sz_srv_plmn = src_srv_cell_info->servedPLMNs.list.count;
    assert(sz_srv_plmn > 0);
    dst.sz_srv_plmn = sz_srv_plmn; 

    dst.srv_plmn = calloc(sz_srv_plmn, sizeof(srv_plmn_f1ap_t)); 
    assert(dst.srv_plmn != NULL && "memory exhausted");

    for(size_t i =0; i < sz_srv_plmn; ++i){
      dst.srv_plmn[i] = cp_srv_plmn_ir(src_srv_cell_info->servedPLMNs.list.array[i]);
    }

    // mandatory
    // CHOICE NR-Mode-Info
  assert(src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_fDD ||  src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_tDD);

  dst.nr_mode_info = src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_fDD ? FDD_NR_MODE_INFO_F1AP : TDD_NR_MODE_INFO_F1AP;
  assert(dst.nr_mode_info == FDD_NR_MODE_INFO_F1AP || dst.nr_mode_info == TDD_NR_MODE_INFO_F1AP);

  if(dst.nr_mode_info == FDD_NR_MODE_INFO_F1AP )
   dst.fdd = cp_fdd_info_ir(src_srv_cell_info->nR_Mode_Info.choice.fDD);
  else // TDD_NR_MODE_INFO_F1AP
   dst.tdd = cp_tdd_info_ir(src_srv_cell_info->nR_Mode_Info.choice.tDD);

  // mandatory
  // Measurement Timing Configuration
  dst.meas_timing_conf = copy_ostring_to_ba(src_srv_cell_info->measurementTimingConfiguration);

  // optional
  // RAN Area Code
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // Extended Served PLMNs Item
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // Cell Direction
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // Cell Type
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // Broadcast PLMN Identity Info List
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // Configured TAC Indication
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  //  Aggressor gNB Set ID
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  //  Victim gNB Set ID
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  //  IAB Info IAB-DU
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  // optional
  // SSB Positions In Burst
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  //optional
  //NR PRACH Configuration
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  //optional
  //SFN Offset
  assert(src_srv_cell_info->iE_Extensions == NULL && "Not implemented");

  return dst;
}

gnb_du_srv_cell_f1ap_t cp_srv_cell_ir(F1AP_GNB_DU_Served_Cells_ItemIEs_t const* src_srv_cell)
{
  assert(src_srv_cell->id == F1AP_ProtocolIE_ID_id_GNB_DU_Served_Cells_Item);
  assert(src_srv_cell->criticality == F1AP_Criticality_reject);
  assert(src_srv_cell->value.present == F1AP_GNB_DU_Served_Cells_ItemIEs__value_PR_GNB_DU_Served_Cells_Item);

  gnb_du_srv_cell_f1ap_t dst_srv_cell = {0};
  
  // mandatory 
  // 4.1.1 served cell Information 
  dst_srv_cell.srv_cell_info = cp_srv_cell_info_ir(&src_srv_cell->value.choice.GNB_DU_Served_Cells_Item.served_Cell_Information);

  // optional 
  // gNB-DU System Information

  return dst_srv_cell; 
}




srv_cell_span_t cp_srv_cell_array_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->criticality == F1AP_Criticality_reject);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List);

  size_t const sz = ie->value.choice.GNB_DU_Served_Cells_List.list.count;
  assert(sz > 0 && sz < 513);

  srv_cell_span_t dst = {0};
  dst.sz_srv_cell = sz;
  dst.srv_cell = calloc(sz, sizeof(gnb_du_srv_cell_f1ap_t));
  assert(dst.srv_cell != NULL && "Memory exhausted");

  for(size_t i = 0; i < sz; ++i){
    // mandatory 
    // 4.1 served cells item 
    F1AP_GNB_DU_Served_Cells_ItemIEs_t const* src_srv_cell = (F1AP_GNB_DU_Served_Cells_ItemIEs_t const*)(ie->value.choice.GNB_DU_Served_Cells_List.list.array[i]);
    dst.srv_cell[i] = cp_srv_cell_ir(src_srv_cell);

    // optional
    // gNB-DU System Information
    // Not implemented 
  }

  return dst; 
} 

gnb_rrc_ver_f1ap_t cp_ver_ir(F1AP_RRC_Version_t const* src)
{
  assert(src != NULL);
  gnb_rrc_ver_f1ap_t dst = {0}; 

  dst.latest_rrc_version = copy_bit_string(&src->latest_RRC_Version);

  // optional
  // Latest RRC Version Enhanced

  return dst;
}

sib_t cp_sib_ir(struct F1AP_SibtypetobeupdatedListItem const* src)
{
  assert(src != NULL);

  sib_t dst = {0};

  // Type: indicates a certain SIB block,
  // e.g. 2 means sibType2, 3 for
  // sibType3, etc. Values 6, 7, 8
  // and values 10 and higher are
  // not applicable in this version
  // of the specifications.
  
  assert(src->sIBtype > 1 && (src->sIBtype < 5 || src->sIBtype == 9));
  dst.type = src->sIBtype;

  // mandatory
  dst.msg = copy_ostring_to_ba(src->sIBmessage);

  // mandatory
  assert(src->valueTag < 32);
  dst.tag = src->valueTag; // INTEGER (0..31, ...)

  // optional
  //sib_area_scope_t* area_scope;

  return dst;
}

gnb_cu_sys_info_t cp_gnb_cu_system_info_ir(F1AP_ProtocolExtensionContainer_154P46_t const* src)
{
  assert(src != NULL);

  gnb_cu_sys_info_t dst = {0};

  assert(src->list.count == 1);

  F1AP_Cells_to_be_Activated_List_ItemExtIEs_t const* src_it = src->list.array[0];

  assert(src_it->id == F1AP_ProtocolIE_ID_id_gNB_CUSystemInformation);
  assert(src_it->criticality == F1AP_Criticality_reject);
  assert(src_it->extensionValue.present == F1AP_Cells_to_be_Activated_List_ItemExtIEs__extensionValue_PR_GNB_CUSystemInformation);

  F1AP_GNB_CUSystemInformation_t const* src_info	= &src_it->extensionValue.choice.GNB_CUSystemInformation;
  assert(src_info->sibtypetobeupdatedlist.list.count > 0 && src_info->sibtypetobeupdatedlist.list.count < 33 && "Out of range" );

  dst.len = src_info->sibtypetobeupdatedlist.list.count;
  dst.sib = calloc(dst.len, sizeof(sib_t) );
  assert(dst.sib != NULL && "Memory exhausted");

  for(int i = 0; i < dst.len; ++i){
     dst.sib[i] = cp_sib_ir(src_info->sibtypetobeupdatedlist.list.array[i] ); 
  }

  // Optional
  //  assert(src->id == NULL && "Not implemented");

  return dst;
}


activate_cell_t cp_activate_cell_ir(F1AP_Cells_to_be_Activated_List_ItemIEs_t const* src)
{
  assert(src != NULL);

  activate_cell_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List_Item);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item);

  F1AP_Cells_to_be_Activated_List_Item_t const*	src_it = &src->value.choice.Cells_to_be_Activated_List_Item;

  // mandatory
  // NR CGI
  TBCD_TO_MCC_MNC(&src_it->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_it->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1UL << 36) );
  //dst.nr_cgi = cp_nr_cgi_ir(&src_it->nRCGI);

  // NR PCI optional
  if(src_it->nRPCI != NULL){
    dst.nr_pci = malloc(sizeof(uint16_t));
    assert(dst.nr_pci != NULL);

    assert(*src_it->nRPCI < 1008);
    *dst.nr_pci = *src_it->nRPCI;
  }

  if(src_it->iE_Extensions != NULL){
    F1AP_ProtocolExtensionContainer_154P46_t const* src_cont = (F1AP_ProtocolExtensionContainer_154P46_t const*)src_it->iE_Extensions;
    assert(src_cont->list.count == 1 && "Only sys information implemented" );

    dst.sys_info = calloc(1, sizeof(gnb_cu_sys_info_t)); 
    assert(dst.sys_info != NULL);
    *dst.sys_info = cp_gnb_cu_system_info_ir(((F1AP_ProtocolExtensionContainer_154P46_t*)src_it->iE_Extensions));
  }

  return dst;
}

span_activate_cell_t cp_cells_to_activate_ir(F1AP_F1SetupResponseIEs_t const* src)
{
  assert(src != NULL);

  span_activate_cell_t dst = {0};  

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List);


  assert(src->value.choice.Cells_to_be_Activated_List.list.count < 513);

  dst.len = src->value.choice.Cells_to_be_Activated_List.list.count;
  dst.cell = calloc(dst.len, sizeof(activate_cell_t));
  assert(dst.cell != NULL);

  for(size_t i =0; i < dst.len; ++i){
    dst.cell[i] = cp_activate_cell_ir((F1AP_Cells_to_be_Activated_List_ItemIEs_t*)src->value.choice.Cells_to_be_Activated_List.list.array[i]); 
  }

  return dst;
}

gnb_rrc_ver_f1ap_t cp_gnb_rrc_sr_ir(F1AP_F1SetupResponseIEs_t const* src)
{
  assert(src != NULL);

  gnb_rrc_ver_f1ap_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_RRC_Version);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_F1SetupResponseIEs__value_PR_RRC_Version);

  F1AP_RRC_Version_t const* src_rrc =  &src->value.choice.RRC_Version;

  // mandatory
  // Latest RRC Version
  assert(src_rrc->latest_RRC_Version.bits_unused == 8 - 3 && "Only 3 bits used");
  dst.latest_rrc_version = copy_bit_string(&src_rrc->latest_RRC_Version); 

  // optional
  // Latest RRC Version Enhanced
  assert(src_rrc->iE_Extensions == NULL && "Not implemented");

  return dst;
}

uint8_t cp_trans_id_sr_ir(F1AP_F1SetupResponseIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_TransactionID);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_F1SetupResponseIEs__value_PR_TransactionID);
  assert(src->value.choice.TransactionID < 256);

  return src->value.choice.TransactionID;
}

///////////////////////////////////////////
/////////// F1AP SETUP ///////////
///////////////////////////////////////////


f1_setup_t cp_f1_setup_ir(F1AP_F1AP_PDU_t const* src)
{
  assert(src != NULL);
  assert(src->present == F1AP_F1AP_PDU_PR_initiatingMessage);

  f1_setup_t dst = {0}; 

  F1AP_InitiatingMessage_t const* src_msg = src->choice.initiatingMessage;
  assert(src_msg->procedureCode == F1AP_ProcedureCode_id_F1Setup);
  assert(src_msg->criticality == F1AP_Criticality_reject);
  assert(src_msg->value.present == F1AP_InitiatingMessage__value_PR_F1SetupRequest);

  F1AP_F1SetupRequest_t const* src_f1_setup = &src_msg->value.choice.F1SetupRequest;
  assert(src_f1_setup->protocolIEs.list.count > 1);

  // mandatory 
  // c1. Transaction ID (integer value) 
  dst.trans_id = cp_trans_id_ir(src_f1_setup->protocolIEs.list.array[0]);

  // mandatory 
  // c2. GNB_DU_ID (integer value) 
  dst.gnb_du_id = cp_gnb_du_id_ir(src_f1_setup->protocolIEs.list.array[1]);

  for(size_t i = 2; i < src_f1_setup->protocolIEs.list.count; ++i) {
    F1AP_F1SetupRequestIEs_t const* ie = src_f1_setup->protocolIEs.list.array[i]; 

    // optional 
    // c3. GNB_DU_Name   
    if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Name){
      dst.gnb_du_name = cp_gnb_du_name_ir(ie);
      // optional 
      // c4. served cells list 
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List){
      srv_cell_span_t tmp = cp_srv_cell_array_ir(ie); 
      dst.srv_cell = tmp.srv_cell;
      dst.sz_srv_cell = tmp.sz_srv_cell;
      // mandatory
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_RRC_Version){
      dst.ver = cp_ver_ir(&ie->value.choice.RRC_Version); 
      // optional
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_Transport_Layer_Address_Info){
      assert(0!=0 && "Not implemented"); 
      // optional
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_BAPAddress){
      assert(0!=0 && "Not implemented"); 
      // optional
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_Extended_GNB_CU_Name){
      assert(0!=0 && "Not implemented"); 
    } else {
      assert(0 != 0 && "Unknown type");
    }
  }

  return dst;
}

//////////////////////////////////////////
//////////////F1 SETUP RESPONSE //////////
//////////////////////////////////////////

f1_setup_response_t cp_f1_setup_response_ir(F1AP_F1AP_PDU_t const* src)
{
  assert(src != NULL);

  f1_setup_response_t dst = {0};

  /* Create */
  /* 0. pdu Type */
  assert(src->present == F1AP_F1AP_PDU_PR_successfulOutcome);
  
  F1AP_SuccessfulOutcome_t* const src_out = src->choice.successfulOutcome;

  assert(src_out->procedureCode ==  F1AP_ProcedureCode_id_F1Setup);
  assert(src_out->criticality   ==  F1AP_Criticality_reject);
  assert(src_out->value.present ==  F1AP_SuccessfulOutcome__value_PR_F1SetupResponse);

  F1AP_F1SetupResponse_t *src_f1_sr = &src_out->value.choice.F1SetupResponse;
  assert(src_f1_sr->protocolIEs.list.count > 1 && src_f1_sr->protocolIEs.list.count < 9);

  // mandatory 
  // c1. Transaction ID (integer value)
  dst.trans_id = cp_trans_id_sr_ir(src_f1_sr->protocolIEs.list.array[0]);

  for(int i = 1; i < src_f1_sr->protocolIEs.list.count; ++i){
    F1AP_F1SetupResponseIEs_t const* ie = src_f1_sr->protocolIEs.list.array[i];
   
    assert(ie->id == F1AP_ProtocolIE_ID_id_gNB_CU_Name 
        || ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List 
        || ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_RRC_Version
        || !("Unknown or unimplemented ie ID") );
    
    if(ie->id == F1AP_ProtocolIE_ID_id_gNB_CU_Name){
       assert(ie->criticality == F1AP_Criticality_ignore);
       assert(ie->value.present == F1AP_F1SetupResponseIEs__value_PR_GNB_CU_Name);

       dst.gnb_cu_name = calloc(1, sizeof(byte_array_t));
       assert(dst.gnb_cu_name != NULL);

       *dst.gnb_cu_name = copy_ostring_to_ba(ie->value.choice.GNB_CU_Name); 

    } else if(ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List){
      assert(ie->criticality == F1AP_Criticality_reject);
      assert(ie->value.present == F1AP_F1SetupResponseIEs__value_PR_Cells_to_be_Activated_List);

      span_activate_cell_t tmp = cp_cells_to_activate_ir(ie);
      dst.sz_act_cell = tmp.len;
      dst.act_cell = tmp.cell; 

    } else if(ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_RRC_Version){
      assert(ie->criticality == F1AP_Criticality_reject);
      assert(ie->value.present == F1AP_F1SetupResponseIEs__value_PR_RRC_Version);
     
      dst.ver = cp_gnb_rrc_sr_ir(ie);

    } else {
      assert(0 != 0 && "Unknown or unimplemented id");
    }
  }
 
  return dst;
}


///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////


static
uint8_t cp_trans_id_sf(F1AP_F1SetupFailureIEs_t const* src)
{
  assert(src->id == F1AP_ProtocolIE_ID_id_TransactionID);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_F1SetupFailureIEs__value_PR_TransactionID);
  assert(src->value.choice.TransactionID  < 256);

  return src->value.choice.TransactionID;
}

static
cause_f1ap_t cp_cause_sf(F1AP_F1SetupFailureIEs_t const* src)
{
  assert(src->id == F1AP_ProtocolIE_ID_id_Cause);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_F1SetupFailureIEs__value_PR_Cause);

  cause_f1ap_t dst = {.type = END_CAUSE_F1AP}; 
  if(src->value.choice.Cause.present == F1AP_Cause_PR_radioNetwork){
    dst.type = RADIO_NETWORK_CAUSE_F1AP; 
    dst.radio = src->value.choice.Cause.choice.radioNetwork; 
  } else if(src->value.choice.Cause.present == F1AP_Cause_PR_transport){
    dst.type = TRANSPORT_CAUSE_F1AP; 
    dst.trans = src->value.choice.Cause.choice.transport; 
  } else if(src->value.choice.Cause.present == F1AP_Cause_PR_protocol){
    dst.type = PROTOCOL_CAUSE_F1AP; 
    dst.proto = src->value.choice.Cause.choice.protocol; 
  } else if(src->value.choice.Cause.present == F1AP_Cause_PR_misc){
    dst.type = MISC_CAUSE_F1AP; 
    dst.misc = src->value.choice.Cause.choice.misc; 
  } else {
    assert(0!=0 && "Unknown type");
  }
  return dst;
}

static
crit_diagn_ie_t cp_crit_diagnose_ie_sf(F1AP_CriticalityDiagnostics_IE_Item_t const* src)
{
  assert(src != NULL);

  crit_diagn_ie_t dst = {0}; 
  //mandatory 
  dst.proc_crit = src->iECriticality;
  assert(dst.proc_crit < END_PROCEDURE_CRITICALLITY_F1AP);

  //mandatory 
  dst.id = src->iE_ID;

  //mandatory 
  dst.type_of_error = src->typeOfError;
  assert(dst.type_of_error < END_CRITICALLITY_DIAGNOSTIC_IE_F1AP);

  return dst;
}

static
criticallity_diagnostic_f1ap_t* cp_crit_diagnose_sf(F1AP_CriticalityDiagnostics_t const* src)
{
  assert(src != NULL);

  criticallity_diagnostic_f1ap_t* dst = calloc(1, sizeof(criticallity_diagnostic_f1ap_t));    
   assert(dst != NULL && "Memory exhausted");

  //optional
  //Procedure Code
  if(src->procedureCode != NULL){
    dst->proc_code = malloc(sizeof(uint8_t));
    assert(dst->proc_code != NULL);

    assert(*src->procedureCode < 256);
    *dst->proc_code = *src->procedureCode; 
  }
  
  //optional
  //Triggering Message
  if(src->triggeringMessage != NULL){
    dst->trig_msg = malloc(sizeof(trig_msg_e));
    assert(dst->trig_msg != NULL && "Memory exhausted");
 
    *dst->trig_msg = *src->triggeringMessage;
    assert(*dst->trig_msg < END_TRIGGERING_MESSAGE_F1AP);
  }

  //optional
  //Procedure Criticality
  if(src->procedureCriticality != NULL){
    dst->proc_crit = malloc(sizeof(proc_crit_e));
    assert(dst->proc_crit != NULL);
    
    *dst->proc_crit = *src->procedureCriticality;
    assert(*dst->proc_crit < END_PROCEDURE_CRITICALLITY_F1AP);
  }

  //optional
  //Transaction ID
  if(src->transactionID != NULL){
    dst->trans_id = malloc(sizeof(uint8_t));
    assert(dst->trans_id != NULL && "Memory exhausted");

    assert(*src->transactionID < 256);
    *dst->trans_id = *src->transactionID;
  }

  // Optional
  if(src->iEsCriticalityDiagnostics != NULL){
    const size_t sz = src->iEsCriticalityDiagnostics->list.count; 
    assert(sz > 0 && sz < 257);
    dst->sz_crit_diagn_ie = sz; 
    dst->crit_diagn_ie = calloc(sz, sizeof(crit_diagn_ie_t));
    for(size_t i = 0; i < sz; ++i){
      dst->crit_diagn_ie[i] = cp_crit_diagnose_ie_sf(src->iEsCriticalityDiagnostics->list.array[i]);
    }
     
  }

  return dst;
}


//////////////////////////////////////////
//////////////F1 SETUP FAILURE //////////
//////////////////////////////////////////


f1_setup_failure_t cp_f1_setup_failure_ir(F1AP_F1AP_PDU_t const* src)
{
  assert(src != NULL);
  assert(src->present == F1AP_F1AP_PDU_PR_unsuccessfulOutcome);

  F1AP_UnsuccessfulOutcome_t const* src_out = src->choice.unsuccessfulOutcome;
  assert(src_out->procedureCode ==  F1AP_ProcedureCode_id_F1Setup);
  assert(src_out->criticality   ==  F1AP_Criticality_reject);
  assert(src_out->value.present ==  F1AP_UnsuccessfulOutcome__value_PR_F1SetupFailure);

  assert(src_out->value.choice.F1SetupFailure.protocolIEs.list.count > 1);
  assert(src_out->value.choice.F1SetupFailure.protocolIEs.list.count < 5);

  f1_setup_failure_t dst = {0}; 

  // Mandatory
  //Transaction ID
  dst.trans_id = cp_trans_id_sf(src_out->value.choice.F1SetupFailure.protocolIEs.list.array[0]);

  //Mandatory
  //Cause
  dst.cause = cp_cause_sf(src_out->value.choice.F1SetupFailure.protocolIEs.list.array[1]); 

  for(size_t i = 2; i < src_out->value.choice.F1SetupFailure.protocolIEs.list.count; ++i){
    F1AP_F1SetupFailureIEs_t const* ie = src_out->value.choice.F1SetupFailure.protocolIEs.list.array[i]; 

    // Optional
    //Time to wait
    if(ie->id == F1AP_ProtocolIE_ID_id_TimeToWait){
      assert(ie->criticality == F1AP_Criticality_ignore);
      assert(ie->value.present == F1AP_F1SetupFailureIEs__value_PR_TimeToWait);

      dst.time_to_wait = malloc(sizeof(time_to_wait_f1ap_e));
      assert(dst.time_to_wait != NULL && "Memory exhausted");
      *dst.time_to_wait = ie->value.choice.TimeToWait;
      //Optional
      //Criticality Diagnostics
    } else if(ie->id == F1AP_ProtocolIE_ID_id_CriticalityDiagnostics){
      assert(ie->criticality == F1AP_Criticality_ignore);
      assert(ie->value.present == F1AP_F1SetupFailureIEs__value_PR_CriticalityDiagnostics);
      F1AP_CriticalityDiagnostics_t	const * src_crit = &ie->value.choice.CriticalityDiagnostics;

      dst.diagnose = cp_crit_diagnose_sf(src_crit);
    } else { 
      assert(0!=0 && "Incorrect id??" );
    }

  }

  return dst;
}

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
//////////////F1 SETUP FAILURE //////////
//////////////////////////////////////////

static
uint32_t cp_gnb_cu_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID);	
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID);

  assert(src->value.choice.GNB_CU_UE_F1AP_ID < (1UL << 32));
  return src->value.choice.GNB_CU_UE_F1AP_ID;
}

static
uint32_t* cp_gnb_du_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID);	
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_GNB_DU_UE_F1AP_ID);

  uint32_t* dst = malloc(sizeof(uint32_t));
  assert(dst != NULL);

  *dst = src->value.choice.GNB_DU_UE_F1AP_ID;
  return dst;
}

static
nr_cgi_t cp_spcell_id_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_SpCell_ID);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_NRCGI);

  nr_cgi_t dst = {0};
  // NR CGI
  TBCD_TO_MCC_MNC(& src->value.choice.NRCGI.pLMN_Identity, dst.plmn_id.mcc, dst.plmn_id.mnc, dst.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src->value.choice.NRCGI.nRCellIdentity, dst.nr_cell_id);
  assert(dst.nr_cell_id < (1UL << 36) );

  return dst;
}

static
uint8_t cp_serv_cell_idx_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_ServCellIndex);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_ServCellIndex);

  assert(src->value.choice.ServCellIndex < 256);

  return src->value.choice.ServCellIndex; 
}

static
cell_ul_conf_e* cp_sp_cell_ul_conf_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_SpCellULConfigured);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_CellULConfigured);

  cell_ul_conf_e* dst = malloc(sizeof(cell_ul_conf_e));
  assert(dst != NULL && "Memory exhausted");

  if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_ul){
    *dst =  UL_CELL_UL_CONF;
  } else if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_sul){
    *dst = SUL_CELL_UL_CONF;
  } else if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_ul_and_sul){
    *dst = UL_SUL_CELL_UL_CONF;
  } else {
    assert(0 != 0 && "Unknown type");
  }

  return dst;
}

static
cu_to_du_rrc_info_t cp_cu_to_du_rrc_info_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  cu_to_du_rrc_info_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_CUtoDURRCInformation);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_CUtoDURRCInformation);

  F1AP_CUtoDURRCInformation_t const* src_cu = &src->value.choice.CUtoDURRCInformation;

  if(src_cu->cG_ConfigInfo != NULL){
    dst.cg_config_info = calloc(1, sizeof(byte_array_t)); 
    assert(dst.cg_config_info != NULL && "Memory exhausted");
    *dst.cg_config_info = copy_ostring_to_ba(*src_cu->cG_ConfigInfo);
  }

  if(src_cu->uE_CapabilityRAT_ContainerList != NULL){
    dst.ue_capability_rat_con_lst = calloc(1, sizeof(byte_array_t)); 
    assert(dst.ue_capability_rat_con_lst != NULL && "Memory exhausted");
    *dst.ue_capability_rat_con_lst = copy_ostring_to_ba(*src_cu->uE_CapabilityRAT_ContainerList);
  }

  if(src_cu->measConfig != NULL){
    dst.meas_config= calloc(1, sizeof(byte_array_t)); 
    assert(dst.meas_config!= NULL && "Memory exhausted");
    *dst.meas_config= copy_ostring_to_ba(*src_cu->measConfig);
  }

  // Optional
  // Handover Preparation Information
  //assert(src->handover_prep_info == NULL && "Not implemented");

  // Optional
  // CellGroupConfig
  //assert(src->cell_group_config == NULL && "Not implemented");

  // Optional
  // Measurement Timing Configuration
  //assert(src->meas_timing_conf == NULL && "Not implemented");

  // Optional
  // UEAssistanceInformation
  //assert(src->ue_assistance_info == NULL && "Not implemented");

  // Optional
  // CG-Config
  //assert(src->cg_config == NULL && "Not implemented");

  // Optional
  // UEAssistanceInformationEUTRA
  //assert(src->UE_assistance_info_eutra == NULL && "Not implemented");

  return dst;
}

typedef struct{
  size_t sz; // max. 64
  nr_cgi_t* candidate_sp_cell_id;
} candidate_sp_cell_id_span_t;

static
nr_cgi_t cp_candidate_sp_cell_item_ue_sr_ir(F1AP_Candidate_SpCell_ItemIEs_t const* src)
{
  assert(src != NULL);

  nr_cgi_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Candidate_SpCell_Item); 
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_Candidate_SpCell_ItemIEs__value_PR_Candidate_SpCell_Item);


  F1AP_NRCGI_t const* src_nr_cgi = &src->value.choice.Candidate_SpCell_Item.candidate_SpCell_ID; 

  // NR CGI
  TBCD_TO_MCC_MNC(&src_nr_cgi->pLMN_Identity, dst.plmn_id.mcc, dst.plmn_id.mnc, dst.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_nr_cgi->nRCellIdentity, dst.nr_cell_id);
  assert(dst.nr_cell_id < (1UL << 36) );

  return dst;
}


static
candidate_sp_cell_id_span_t cp_candidate_sp_cell_id_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  candidate_sp_cell_id_span_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_Candidate_SpCell_List);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_Candidate_SpCell_List);

  assert(src->value.choice.Candidate_SpCell_List.list.count < 65);
  dst.sz = src->value.choice.Candidate_SpCell_List.list.count; 
  dst.candidate_sp_cell_id = calloc(dst.sz, sizeof(nr_cgi_t));
  assert(dst.candidate_sp_cell_id != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
   dst.candidate_sp_cell_id[i] = cp_candidate_sp_cell_item_ue_sr_ir((F1AP_Candidate_SpCell_ItemIEs_t const*)src->value.choice.Candidate_SpCell_List.list.array[i]);   
  }

  return dst;
}

static
drx_cycle_t* cp_drx_cycle_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  drx_cycle_t* dst = calloc(1, sizeof( drx_cycle_t ));
  assert(dst != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_DRXCycle);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_DRXCycle);

  // Mandatory 
  // Long DRX Cycle Length
  F1AP_DRXCycle_t const* src_drx = &src->value.choice.DRXCycle;
  dst->long_drx_cycle_len =  src_drx->longDRXCycleLength;
  // Optional 
  // Short DRX Cycle Length
  assert(src_drx->shortDRXCycleLength == NULL && "Not implemented");

  // Optional
  // Short DRX Cycle Timer INTEGER (1..16)
  assert(src_drx->shortDRXCycleLength == NULL && "Not implemented");

  return dst;
}

static
byte_array_t* cp_res_coord_trans_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  byte_array_t* dst = calloc(1, sizeof(byte_array_t));
  assert(dst != NULL && "Memory exhausted");

  assert(src->id == F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_ResourceCoordinationTransferContainer);

  *dst = copy_ostring_to_ba(src->value.choice.ResourceCoordinationTransferContainer);
  return dst;
}


typedef struct{
  size_t sz; // [0,32]
  scell_to_be_setup_t* scell_to_be_setup;
} scell_to_be_setup_span_t;

static
scell_to_be_setup_t cp_scell_to_be_setup_item_ue_sr_ir(F1AP_SCell_ToBeSetup_ItemIEs_t const* src)
{
  assert(src != NULL);

  scell_to_be_setup_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item);

  F1AP_SCell_ToBeSetup_Item_t const* src_item = &src->value.choice.SCell_ToBeSetup_Item;
  // Mandatory
  // SCell ID 
  F1AP_NRCGI_t const* src_nr_cgi = &src_item->sCell_ID;

  // NR CGI
  TBCD_TO_MCC_MNC(&src_nr_cgi->pLMN_Identity, dst.scell_id.plmn_id.mcc, dst.scell_id.plmn_id.mnc, dst.scell_id.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_nr_cgi->nRCellIdentity, dst.scell_id.nr_cell_id);
  assert(dst.scell_id.nr_cell_id < (1UL << 36) );

  // Mandatory
  // SCellIndex
  assert(src_item->sCellIndex < 32 &&  src_item->sCellIndex); // INTEGER (1..31
  dst.scell_idx = src_item->sCellIndex;

  // Optional
  // SCell UL Configured
  if(src_item->sCellULConfigured != NULL){
    dst.scell_ul_conf = calloc(1, sizeof(cell_ul_conf_e)); 
    assert(dst.scell_ul_conf != NULL && "Memory exhausted" );

    if(*src_item->sCellULConfigured == F1AP_CellULConfigured_ul){
      *dst.scell_ul_conf = UL_CELL_UL_CONF;
    } else if(*src_item->sCellULConfigured == F1AP_CellULConfigured_sul) {
      *dst.scell_ul_conf = SUL_CELL_UL_CONF;
    } else if(*src_item->sCellULConfigured == F1AP_CellULConfigured_ul_and_sul) {
      *dst.scell_ul_conf = UL_SUL_CELL_UL_CONF;
    } else {
      assert( 0 != 0 && "Unknown type");
    }
  }

  // Optional
  // servingCellMO
  assert(src_item->iE_Extensions == NULL && "Not implemented"); // INTEGER(1..64) 

  return dst;
}

static
scell_to_be_setup_span_t cp_scell_to_be_setup_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  scell_to_be_setup_span_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_List);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_SCell_ToBeSetup_List);

  dst.sz = src->value.choice.SCell_ToBeSetup_List.list.count;
  assert(dst.sz > 0 && dst.sz < 33);
  if(dst.sz > 0){
    dst.scell_to_be_setup = calloc(dst.sz, sizeof(scell_to_be_setup_t) );
    assert(dst.scell_to_be_setup != NULL && "Memory exhausted" );
  }

  for(size_t i = 0; i < dst.sz; ++i){
    F1AP_SCell_ToBeSetup_ItemIEs_t const* ie = (F1AP_SCell_ToBeSetup_ItemIEs_t const*)src->value.choice.SCell_ToBeSetup_List.list.array[i]; 
    dst.scell_to_be_setup[i] = cp_scell_to_be_setup_item_ue_sr_ir(ie);
  }

  return dst;
}

typedef struct{
  // SRB to Be Setup Item
  size_t sz; // [0,8]
  srb_to_be_setup_t* srb_to_be_setup;
} srb_to_be_setup_span_t;

static
srb_to_be_setup_t cp_srb_to_be_setup_item_ue_sr_ir(F1AP_SRBs_ToBeSetup_ItemIEs_t const* src)
{
  assert(src != NULL);

  srb_to_be_setup_t  dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_Item); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_SRBs_ToBeSetup_ItemIEs__value_PR_SRBs_ToBeSetup_Item);

  // Mandatory
  //  SRB ID
  assert(src->value.choice.SRBs_ToBeSetup_Item.sRBID < 4); // [0,3]
  dst.srb_id = src->value.choice.SRBs_ToBeSetup_Item.sRBID;

  // Optional
  // Duplication Indication
  assert(src->value.choice.SRBs_ToBeSetup_Item.duplicationIndication == NULL && "Not implemented");

  // Optional 
  // Additional Duplication Indication
  assert(src->value.choice.SRBs_ToBeSetup_Item.duplicationIndication == NULL && "not implemented");

  return dst;
}

static
srb_to_be_setup_span_t cp_srb_to_be_setup_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  srb_to_be_setup_span_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_List);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_SRBs_ToBeSetup_List);

  assert(src->value.choice.SRBs_ToBeSetup_List.list.count < 9); // [0,8]
  dst.sz = src->value.choice.SRBs_ToBeSetup_List.list.count;
  
  dst.srb_to_be_setup = calloc(dst.sz, sizeof(srb_to_be_setup_t));
  assert(dst.srb_to_be_setup != NULL && "Memory exhausted" );

  for(size_t i = 0; i < dst.sz; ++i){
    dst.srb_to_be_setup[i] = cp_srb_to_be_setup_item_ue_sr_ir((F1AP_SRBs_ToBeSetup_ItemIEs_t const*)src->value.choice.SRBs_ToBeSetup_List.list.array[i]); 
  }

  return dst;
}

typedef struct{
  size_t sz;
  drb_to_be_setup_t*  drb_to_be_setup;
} drb_to_be_setup_span_t ; 

static
alloc_retention_prio_t cp_alloc_retention_prio(F1AP_AllocationAndRetentionPriority_t const* src)
{
  assert(src != NULL);

  alloc_retention_prio_t dst = {0}; 

  // Mandatory
  // Priority Level
  assert(src->priorityLevel < 16);
  dst.prio_level = src->priorityLevel; // INTEGER (0..15)
                      
  // Mandatory
  // Pre-emption Capability
  if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption){
    dst.preempt_capability = SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_may_trigger_pre_emption){
    dst.preempt_capability = MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY; 
  } else {
    assert(0!=0 && "Unknown type");
  }

  // Mandatory
  // Pre-emption Vulnerability
  if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_not_pre_emptable){
    dst.preempt_vulnerability = NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY;  
  } else if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_pre_emptable){
    dst.preempt_vulnerability = PRE_EMPTABLE_PREEMPT_VULNERABILITY;  
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

static
e_utran_qos_t cp_e_utran_qos(F1AP_EUTRANQoS_t const* src)
{
  assert(src != NULL);

  e_utran_qos_t dst = {0};

  // Mandatory
  // QCI
  assert(src->qCI < 256);
  dst.qci = src->qCI; 

  // Mandatory
  // Allocation and Retention Priority
  dst.alloc = cp_alloc_retention_prio(&src->allocationAndRetentionPriority);

  // Optional
  // GBR QoS Information 9.3.1.21
  assert(src->gbrQosInformation == NULL && "not implemented" );

  return dst;
}

static
s_nssai_t cp_nssai(F1AP_SNSSAI_t const* src)
{
  assert(src != NULL);
  s_nssai_t dst = {0};

  // Mandatory
  // OCTET STRING (SIZE(1))
  assert(src->sST.size == 1);
  memcpy(&dst.sst, src->sST.buf, 1) ;

  // Optional
  // OCTET STRING (SIZE(3))
  assert(src->sD == NULL && "Not implemented"); // size 3 bytes

  return dst;
}

static
non_dyn_5qi_descriptor_t cp_non_dyn_5qi(F1AP_NonDynamic5QIDescriptor_t const* src)
{
  assert(src != NULL);

  non_dyn_5qi_descriptor_t dst = {0}; 

  // Mandatory
  //  5QI
  assert(src->fiveQI < 256);
  dst.five_qi = src->fiveQI;

  // Optional
  // Priority Level
  assert(src->qoSPriorityLevel == NULL && "Not implemented"); // [1,127]

  // Optional
  // Averaging Window 9.3.1.53 INTEGER (0..4095, ...)
  assert(src->averagingWindow == NULL && "Not implemented");

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  assert(src->maxDataBurstVolume == NULL && "Not implemented"); // INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // CN Packet Delay Budget Downlink 9.3.1.145
  assert(src->iE_Extensions == NULL && "not implemented");
  //assert(src-> == NULL && "Not implemented"); // INTEGER (0..65535, …)

  // Optional 
  // CN Packet Delay Budget Uplink // INTEGER (0..65535, …)
  //assert(src->pkt_delay_budget_uplink == NULL && "Not implemented"); // INTEGER (0..65535, …)

  return dst;
}

static
dyn_5qi_descriptor_t cp_dyn_5qi(F1AP_Dynamic5QIDescriptor_t const* src)
{
  assert(src != NULL);

  dyn_5qi_descriptor_t dst = {0}; 

  //Mandatory
  //QoS Priority Level
  assert(src->qoSPriorityLevel > 0 && src->qoSPriorityLevel < 128);
  dst.qos_prio_level = src->qoSPriorityLevel; // INTEGER (1..127)

  // Mandatory
  //Packet Delay Budget 9.3.1.51
  assert(src->packetDelayBudget < 1024);
  dst.pkt_delay_budget = src->packetDelayBudget; // INTEGER (0..1023, ...)

  // Mandatory
  // Packet Error Rate 9.3.1.52
  dst.pkt_error_rate.scalar = src->packetErrorRate.pER_Scalar;
  dst.pkt_error_rate.exponent = src->packetErrorRate.pER_Exponent;

  // Optional
  // 5QI
  assert(src->fiveQI == NULL && "Not implemented");

  // C-ifGBRflow
  // Delay Critical
  assert(src->delayCritical == NULL && "Not implemented");

  // C-ifGBRflow
  // Averaging Window 9.3.1.53
  assert(src->averagingWindow == NULL && "Not implemented"); // INTEGER (0..4095, ...)

  // Optional 
  // Maximum Data Burst Volume 9.3.1.54
  assert(src->maxDataBurstVolume == NULL && "Not implemented"); //INTEGER (0..4095, ..., 4096..2000000)

  // Optional 
  // Extended Packet Delay Budget 9.3.1.145
  assert(src->iE_Extensions == NULL && "Not implemented" );
  //assert(src->ext_pkt_delay_budget == NULL && "Not implemented"); //INTEGER (0..65535, …) 

  // Optional 
  // CN Packet Delay Budget Downlink
  // assert(src->cn_pkt_dalay_budget_downlink == NULL && "Not implemented"); 

  // Optional
  // CN Packet Delay Budget Uplink
  // assert(src->cn_pkt_dalay_budget_uplink == NULL && "Not implemented"); 

  return dst;
}

static
alloc_retention_prio_t cp_ng_ran_alloc_retention_prio(F1AP_NGRANAllocationAndRetentionPriority_t const* src)
{
  assert(src != NULL);

  alloc_retention_prio_t dst = {0}; 

  // Mandatory
  // Priority Level
  assert(src->priorityLevel   < 16);// INTEGER (0..15)
  dst.prio_level = src->priorityLevel;

  // Mandatory
  // Pre-emption Capability
  if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption){
    dst.preempt_capability = SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_may_trigger_pre_emption){
    dst.preempt_capability = MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else {
    assert(0!=0 && "Unknown type");
  } 

  // Mandatory
  // Pre-emption Vulnerability
  //
  if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_not_pre_emptable){
    dst.preempt_vulnerability = NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY; 
  } else if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_pre_emptable){
    dst.preempt_vulnerability = PRE_EMPTABLE_PREEMPT_VULNERABILITY;
  } else {
    assert(0!= 0 && "Not known type");
  }

  return dst;
}

static
qos_flow_level_qos_parameter_t cp_qos_flow_level_qos_parameter(F1AP_QoSFlowLevelQoSParameters_t const* src)
{
  assert(src != NULL);

  qos_flow_level_qos_parameter_t dst = {0}; 
  //Mandatory 
  // CHOICE QoS Characteristics
  if(src->qoS_Characteristics.present == F1AP_QoS_Characteristics_PR_non_Dynamic_5QI){
    // Non Dynamic 5QI Descriptor 9.3.1.49
    dst.non_dyn = cp_non_dyn_5qi(src->qoS_Characteristics.choice.non_Dynamic_5QI); 
  } else if(src->qoS_Characteristics.present == F1AP_QoS_Characteristics_PR_dynamic_5QI){
    // Dynamic 5QI Descriptor 9.3.1.47
    dst.dyn = cp_dyn_5qi(src->qoS_Characteristics.choice.dynamic_5QI); 
  } else {
    assert(0!=0 && "Unknown type");
  }

  // Mandatory
  // NG-RAN Allocation and Retention Priority 9.3.1.48
  dst.alloc_retention_prio = cp_ng_ran_alloc_retention_prio(&src->nGRANallocationRetentionPriority);

  // Optional 
  // GBR QoS Flow Information 9.3.1.46
  assert(src->gBR_QoS_Flow_Information == NULL && "Not implemented");

  // Optional 
  // Reflective QoS Attribute
  assert(src->reflective_QoS_Attribute == NULL && "Not implemented");

  // Optional 
  // PDU Session ID
  assert(src->iE_Extensions == NULL && "Not implemented" );
  //uint8_t* pdu_session_id;

  // Optional
  //UL PDU Session Aggregate Maximum Bit Rate 9.3.1.22
  //uint64_t* ul_pdu_session_aggr_max_bit_rate; 

  // Optional
  // QoS Monitoring Request
  //qos_monitoring_request_e* qos_monitoring_request;

  return dst;
}

static
flows_mapped_to_drb_t cp_flows_mapped_to_drb_ue_sr_ir(F1AP_Flows_Mapped_To_DRB_Item_t const* src)
{
  assert(src != NULL);

  flows_mapped_to_drb_t dst = {0}; 

  // Mandatory
  // QoS Flow Identifier 9.3.1.63
  assert(src->qoSFlowIdentifier < 64);  // INTEGER (0 ..63)
  dst.qfi = src->qoSFlowIdentifier; 

  // Mandatory 
  // QoS Flow Level QoS Parameters 9.3.1.45
  dst.qos_flow_level = cp_qos_flow_level_qos_parameter(&src->qoSFlowLevelQoSParameters); 

  // Optional
  // QoS Flow Mapping Indication 9.3.1.72
  assert(src->iE_Extensions == NULL && "Not implemented" );
  // qos_flow_mapping_ind 

  // Optional
  // TSC Traffic Characteristics 9.3.1.141
  //assert(src->tsc_traffic_char == NULL && "Not implemented");

  return dst;
}

static
drb_info_t cp_drb_info(F1AP_QoSInformation_ExtIEs_t const* src)
{
  assert(src != NULL);

  drb_info_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_DRB_Information);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_QoSInformation_ExtIEs__value_PR_DRB_Information);

  F1AP_DRB_Information_t	const* src_drb = &src->value.choice.DRB_Information;

  // Mandatory
  // DRB QoS 9.3.1.45
  dst.drb_qos = cp_qos_flow_level_qos_parameter(&src_drb->dRB_QoS);

  // Mandatory
  // S-NSSAI 9.3.1.38
  dst.nssai = cp_nssai(&src_drb->sNSSAI);

  // Optional
  // Notification Control 9.3.1.56
  assert(src_drb->notificationControl == NULL && "Not implemented");

  // Flows Mapped to DRB
  assert(src_drb->flows_Mapped_To_DRB_List.list.count > 0 && src_drb->flows_Mapped_To_DRB_List.list.count  < 65);// [1,64]
  dst.sz_flows_mapped_to_drb =  src_drb->flows_Mapped_To_DRB_List.list.count;
  dst.flows_mapped_to_drb = calloc(dst.sz_flows_mapped_to_drb, sizeof(flows_mapped_to_drb_t));

  for(size_t i = 0; i < dst.sz_flows_mapped_to_drb; ++i){
   dst.flows_mapped_to_drb[i] = cp_flows_mapped_to_drb_ue_sr_ir(src_drb->flows_Mapped_To_DRB_List.list.array[i]);
  }

  return dst;
}

static
ul_up_tnl_info_lst_t cp_ul_up_tnl_info_ue_sr_ir(F1AP_ULUPTNLInformation_ToBeSetup_Item_t const* src)
{
  assert(src != NULL);

  ul_up_tnl_info_lst_t dst = {0}; 

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  assert(src->uLUPTNLInformation.present == F1AP_UPTransportLayerInformation_PR_gTPTunnel);

  dst.tnl_info.tla = copy_bit_string(&src->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress );

  assert(src->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size == 4);
  memcpy(dst.tnl_info.gtp_teid, src->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf, 4);

  // Optional
  // BH Information 9.3.1.114
  assert(src->iE_Extensions == NULL && "Not implemented");

  return dst;
}

static
pdcp_sn_len_e cp_dl_pdcp_sn_len_ue_sr_ir(F1AP_DRBs_ToBeSetup_ItemExtIEs_t const* src) 
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_DLPDCPSNLength);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->extensionValue.present == F1AP_DRBs_ToBeSetup_ItemExtIEs__extensionValue_PR_PDCPSNLength);

  pdcp_sn_len_e dst = {0}; 

  if(src->extensionValue.choice.PDCPSNLength == F1AP_PDCPSNLength_twelve_bits){
   dst = TWELVE_12BITS_PDCP_SN_LEN;
  } else if(src->extensionValue.choice.PDCPSNLength == F1AP_PDCPSNLength_eighteen_bits){
    dst = EIGHTEEN_18BITS_PDCP_SN_LEN;
  } else {
    assert(0!=0 && "Unknown type");
  }

  return dst;
}

typedef struct{
  add_pdcp_dup_tnl_t* add_pdcp_dup_tnl;
  size_t sz;
} add_pdcp_dup_tnl_span_t;

static
add_pdcp_dup_tnl_t cp_add_pdcp_dup_tnl_ue_sr_item_ir(F1AP_AdditionalPDCPDuplicationTNL_Item_t const* src)
{
  assert(src != NULL);

  add_pdcp_dup_tnl_t dst = {0}; 

  assert(src->additionalPDCPDuplicationUPTNLInformation.present == F1AP_UPTransportLayerInformation_PR_gTPTunnel);

  byte_array_t ba = copy_ostring_to_ba(src->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel->gTP_TEID); 
  assert(ba.len == 4);

  memcpy(dst.gtp_teid, ba.buf, 4);
  free_byte_array(ba); 

  dst.trans_layer_add = copy_bit_string(&src->additionalPDCPDuplicationUPTNLInformation.choice.gTPTunnel->transportLayerAddress);

  return dst;
}

static
add_pdcp_dup_tnl_span_t cp_add_pdcp_dup_tnl_ue_sr_ir(F1AP_DRBs_ToBeSetup_ItemExtIEs_t const* src)
{
  assert(src != NULL);

  add_pdcp_dup_tnl_span_t dst = {0}; 
  assert(src->id == F1AP_ProtocolIE_ID_id_AdditionalPDCPDuplicationTNL_List);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->extensionValue.present == F1AP_DRBs_ToBeSetup_ItemExtIEs__extensionValue_PR_AdditionalPDCPDuplicationTNL_List);

  dst.sz = src->extensionValue.choice.AdditionalPDCPDuplicationTNL_List.list.count;
  dst.add_pdcp_dup_tnl = calloc(dst.sz, sizeof( add_pdcp_dup_tnl_t ) );
  assert(dst.add_pdcp_dup_tnl != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i ){
    F1AP_AdditionalPDCPDuplicationTNL_Item_t const* ie = src->extensionValue.choice.AdditionalPDCPDuplicationTNL_List.list.array[i];
    dst.add_pdcp_dup_tnl[i] = cp_add_pdcp_dup_tnl_ue_sr_item_ir(ie); 
  }

  return dst;
}

static
drb_to_be_setup_t cp_drb_to_be_setup_item_ue_sr_ir(F1AP_DRBs_ToBeSetup_ItemIEs_t const* src)
{
  assert(src != NULL);

  drb_to_be_setup_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_Item); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_DRBs_ToBeSetup_ItemIEs__value_PR_DRBs_ToBeSetup_Item);

  F1AP_DRBs_ToBeSetup_Item_t const* src_drb = &src->value.choice.DRBs_ToBeSetup_Item;

  // Mandatory
  // DRB ID 9.3.1.8
  assert(src_drb->dRBID < 33);;// INTEGER [0,32]
  dst.drb_id = src_drb->dRBID;

  assert(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_eUTRANQoS 
      || src_drb->qoSInformation.present == F1AP_QoSInformation_PR_choice_extension);

  if(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_eUTRANQoS){
    dst.qos_info = E_UTRAN_QOS_INFORMATION;
    dst.e_utran_qos = cp_e_utran_qos(src_drb->qoSInformation.choice.eUTRANQoS );
  } else if(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_choice_extension) {
    dst.qos_info = DRB_INFO_QOS_INFORMATION;
    dst.drb_info = cp_drb_info((F1AP_QoSInformation_ExtIEs_t const*)src_drb->qoSInformation.choice.choice_extension);
  } else {
    assert(0!= 0 && "Not implemented");
  }

  // UL UP TNL Information to be setup List
  assert(src_drb->uLUPTNLInformation_ToBeSetup_List.list.count < 3);
  dst.sz_ul_up_tnl_info_lst = src_drb->uLUPTNLInformation_ToBeSetup_List.list.count; 
  if(dst.sz_ul_up_tnl_info_lst > 0){
    dst.ul_up_tnl_info = calloc(dst.sz_ul_up_tnl_info_lst, sizeof(ul_up_tnl_info_lst_t) );
    assert(dst.ul_up_tnl_info != NULL && "Memory exhausted");

    for(size_t i = 0; i < dst.sz_ul_up_tnl_info_lst; ++i){
      dst.ul_up_tnl_info[i] = cp_ul_up_tnl_info_ue_sr_ir( src_drb->uLUPTNLInformation_ToBeSetup_List.list.array[i]);  
    }
  }

  // Mandatory
  // RLC Mode 9.3.1.27
  if(src_drb->rLCMode == F1AP_RLCMode_rlc_am){
    dst.rlc_mode = RLC_AM_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode ==   F1AP_RLCMode_rlc_um_bidirectional){
    dst.rlc_mode = RLC_UM_BIDIRECTIONAL_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode  == F1AP_RLCMode_rlc_um_unidirectional_ul){
    dst.rlc_mode = RLC_UM_UNIDIRECTIONAL_UL_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode  == F1AP_RLCMode_rlc_um_unidirectional_dl){
    dst.rlc_mode = RLC_UM_UNIDIRECTIONAL_DL_RLC_MODE_F1AP;
  } else {
    assert( 0!= 0 && "Unknown type");
  }

  // Optional 
  // UL Configuration 9.3.1.31
  assert(src_drb->uLConfiguration == NULL && "Not implemented");

  // Optional 
  // Duplication Activation 9.3.1.36
  assert(src_drb->duplicationActivation == NULL && "Not implemented");

  // Optional
  // DC Based Duplication Configured
  // assert(src_drb->dc_dup_act_conf == NULL && "Not implemented");

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  // assert(src_drb->dc_dup_act == NULL && "Not implemented");

  assert(((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.count > 0 );
  for(size_t i = 0; i < ((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.count; ++i){

    F1AP_DRBs_ToBeSetup_ItemExtIEs_t const* src_ie = ((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.array[i];
    // Mandatory
    // DL PDCP SN length
    if(src_ie->id == F1AP_ProtocolIE_ID_id_DLPDCPSNLength){
      dst.dl_pdcp_sn_len = cp_dl_pdcp_sn_len_ue_sr_ir( src_ie ) ;
      // Additional PDCP Duplication TNL List // [0,2]
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_AdditionalPDCPDuplicationTNL_List){
      add_pdcp_dup_tnl_span_t tmp = cp_add_pdcp_dup_tnl_ue_sr_ir(src_ie);
      assert(tmp.sz < 3);
      dst.sz_add_pdcp_dup_tnl = tmp.sz;
      dst.add_pdcp_dup_tnl = tmp.add_pdcp_dup_tnl;
    } else {
      assert(0!=0 && "Unknown type");
    }

    // Optional
    // RLC Duplication Information 9.3.1.146
    //assert(src->sz_rlc_dup_info < 4); // [0,3]
    //assert(src->sz_rlc_dup_info == 0 && "Not implememted");
    //assert(src->rlc_dup_info == NULL && "Not implememted");
    // For future implementation
    // F1AP_RLCDuplicationInformation_t
    // F1AP_ProtocolIE_ID_id_RLCDuplicationInformation	
  }

  return dst;
}

static
drb_to_be_setup_span_t cp_drb_to_be_setup_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  drb_to_be_setup_span_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_List);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_DRBs_ToBeSetup_List);

  dst.sz = src->value.choice.DRBs_ToBeSetup_List.list.count;
  assert(dst.sz < 65);
  dst.drb_to_be_setup = calloc(dst.sz, sizeof(drb_to_be_setup_t) );
  assert(dst.drb_to_be_setup != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.drb_to_be_setup[i] = cp_drb_to_be_setup_item_ue_sr_ir((F1AP_DRBs_ToBeSetup_ItemIEs_t const*)src->value.choice.DRBs_ToBeSetup_List.list.array[i]) ;
  }

  return dst;
}

ue_ctx_setup_request_t cp_ue_ctx_setup_request_ir(F1AP_F1AP_PDU_t const* src_pdu)
{
  assert(src_pdu != NULL);

  ue_ctx_setup_request_t dst = {0};

  assert(src_pdu->present == F1AP_F1AP_PDU_PR_initiatingMessage);

  F1AP_InitiatingMessage_t const* src_init = src_pdu->choice.initiatingMessage;
  assert(src_init->procedureCode == F1AP_ProcedureCode_id_UEContextSetup);
  assert(src_init->criticality == F1AP_Criticality_reject);
  assert(src_init->value.present == F1AP_InitiatingMessage__value_PR_UEContextSetupRequest);

  F1AP_UEContextSetupRequest_t const* src = &src_init->value.choice.UEContextSetupRequest;
  // Mandatory
  //gNB-CU UE F1AP ID 9.3.1.4
  dst.gnb_cu_ue_f1ap_id = cp_gnb_cu_ue_sr_ir(src->protocolIEs.list.array[0]);

  assert(src->protocolIEs.list.count < 37);
  for(size_t i = 1; i < src->protocolIEs.list.count; ++i){

    F1AP_UEContextSetupRequestIEs_t const* src_it = src->protocolIEs.list.array[i];

    // Optional
    // gNB-DU UE F1AP ID 9.3.1.5
    if(src_it->id == F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID){
      dst.gnb_du_ue_f1ap_id = cp_gnb_du_ue_sr_ir(src_it);
      // Mandatory
      // SpCell ID
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SpCell_ID){
      dst.sp_cell_id = cp_spcell_id_ue_sr_ir(src_it);
      // Mandatory
      // ServCellIndex
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_ServCellIndex){
      dst.serv_cell_idx = cp_serv_cell_idx_ue_sr_ir(src_it);
      // Optional
      // SpCell UL Configured 9.3.1.33
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SpCellULConfigured){
      dst.sp_cell_ul_conf = cp_sp_cell_ul_conf_ue_sr_ir(src_it);
      // Mandatory
      // CU to DU RRC Information 9.3.1.25
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_CUtoDURRCInformation){
      dst.cu_to_du_rrc_info = cp_cu_to_du_rrc_info_ue_sr_ir(src_it);
      // Optional 
      // Candidate SpCell ID
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_Candidate_SpCell_List){
      candidate_sp_cell_id_span_t tmp = cp_candidate_sp_cell_id_ue_sr_ir(src_it);
      dst.candidate_sp_cell_id = tmp.candidate_sp_cell_id;
      dst.sz_candidate_sp_cell_id = tmp.sz;
      //Optional 
      // DRX Cycle
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_DRXCycle){
      dst.drx_cycle = cp_drx_cycle_ue_sr_ir(src_it);
      // Optional
      // Resource Coordination Transfer Container
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer){
      dst.resource_coord_transfer_container = cp_res_coord_trans_ue_sr_ir(src_it);
      // SCell To Be Setup List
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_List){
      scell_to_be_setup_span_t tmp = cp_scell_to_be_setup_ue_sr_ir(src_it);
      dst.scell_to_be_setup = tmp.scell_to_be_setup;
      dst.sz_scell_to_be_setup = tmp.sz;
      // SRB to Be Setup Item
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_List){
      srb_to_be_setup_span_t tmp = cp_srb_to_be_setup_ue_sr_ir(src_it);
      dst.srb_to_be_setup = tmp.srb_to_be_setup; 
      dst.sz_srb_to_be_setup = tmp.sz;
      // DRB to Be Setup Item
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_List){
      drb_to_be_setup_span_t tmp = cp_drb_to_be_setup_ue_sr_ir(src_it);
      dst.drb_to_be_setup = tmp.drb_to_be_setup; 
      dst.sz_drb_to_be_setup = tmp.sz;
    } else{
      assert(0!=0 && "Unknown or unimplemented");
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

  }

  return dst;
}

