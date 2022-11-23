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



