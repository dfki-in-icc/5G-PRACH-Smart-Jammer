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
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_SibtypetobeupdatedListItem.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_DLUPTNLInformation-ToBeSetup-Item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Endpoint-IP-address-and-port.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRACells-List-item.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRA-FDD-Info.h"
#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_EUTRA-TDD-Info.h"


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

static
uint64_t cp_gnb_du_id_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->id == F1AP_ProtocolIE_ID_id_gNB_DU_ID);
  assert(ie->criticality == F1AP_Criticality_reject);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_ID);

  uint64_t tmp = 0;
  int const rc = asn_INTEGER2ulong(&ie->value.choice.GNB_DU_ID, &tmp);
  assert(rc == 0);
  assert(tmp < (1ul << 36) && "should not have more than 36 bits");

  return tmp;
}

static
byte_array_t* cp_gnb_du_name_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->criticality == F1AP_Criticality_ignore);
  assert(ie->value.present ==  F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Name);
  byte_array_t* dst = calloc(1, sizeof(byte_array_t));
  assert(dst != NULL && "memory exhausted");
  *dst = copy_ostring_to_ba(ie->value.choice.GNB_DU_Name);

  return dst;
}

static
s_nssai_arr_t cp_tai_slice_sup_lst_ir(F1AP_ServedPLMNs_ItemExtIEs_t const* src)
{
  s_nssai_arr_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_TAISliceSupportList);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->extensionValue.present == F1AP_ServedPLMNs_ItemExtIEs__extensionValue_PR_SliceSupportList);
  F1AP_SliceSupportList_t const* src_slice_sup_lst = &src->extensionValue.choice.SliceSupportList;

  size_t const sz = src_slice_sup_lst->list.count;
  dst.len = sz; 
  dst.tai_slice_sup_lst = calloc(sz, sizeof(s_nssai_t));
  assert( dst.tai_slice_sup_lst != NULL && "memory exhausted");

  for(size_t i = 0; i < sz; ++i){
    F1AP_SliceSupportItem_t const* src_tai_ss = src_slice_sup_lst->list.array[i]; 
    s_nssai_t* dst_tai_ss = &dst.tai_slice_sup_lst[i];   

    assert(src_tai_ss->sNSSAI.sST.size == 1);
    dst_tai_ss->sst = *src_tai_ss->sNSSAI.sST.buf;

    if(src_tai_ss->sNSSAI.sD != NULL){
      dst_tai_ss->sd = calloc(1, sizeof(byte_array_t) );
      assert(dst_tai_ss->sd != NULL && "memory exhausted");
      *dst_tai_ss->sd = copy_ostring_to_ba(*src_tai_ss->sNSSAI.sD);
    }
  }

  return dst;
}

static
srv_plmn_f1ap_t cp_srv_plmn_ir(F1AP_ServedPLMNs_Item_t const* src_srv_plmn)
{
  srv_plmn_f1ap_t dst = {0};

  // mandatory 
  // plmn identity 9.3.1.14 
  plmn_f1ap_t* dst_plmn_id = &dst.plmn_id;
  F1AP_PLMN_Identity_t const* src_plmn_id = &src_srv_plmn->pLMN_Identity; 
  TBCD_TO_MCC_MNC(src_plmn_id, dst_plmn_id->mcc,  dst_plmn_id->mnc,  dst_plmn_id->mnc_digit_len);

  // optional 9.3.1.37 
  // tai slice support list  
  if(src_srv_plmn->iE_Extensions != NULL){
    s_nssai_arr_t tmp = cp_tai_slice_sup_lst_ir( ((F1AP_ProtocolExtensionContainer_154P34_t const*)src_srv_plmn->iE_Extensions)->list.array[0]);
    dst.tai_slice_sup_lst = tmp.tai_slice_sup_lst;
    dst.sz_tai_slice_sup_lst = tmp.len;
  }

  // optional
  // npn support information

  // optional
  // extended tai slice support lis

  return dst;
}

static
nr_freq_info_f1ap_t cp_frq_info_ir(F1AP_NRFreqInfo_t const* src)
{
  assert(src != NULL);

  nr_freq_info_f1ap_t dst = {0}; 


  // mandatory
  // nr arfcn
  assert(src->nRARFCN < 3279165 + 1);
  dst.nr_arfcn = src->nRARFCN; 

  // optional
  // 1.2. sul information
  assert(src->sul_Information == NULL && "not implemented");

  // mandatory
  // 1.3 frequency band list
  assert(src->freqBandListNr.list.count < 33);
  dst.sz_frq_band = src->freqBandListNr.list.count; 

  dst.frq_band = calloc(dst.sz_frq_band, sizeof(freq_band_f1ap_t) );
  assert(dst.frq_band != NULL && "memory exhausted" );

  for(int i = 0; i < dst.sz_frq_band; ++i){
    F1AP_FreqBandNrItem_t const* src_frq = src->freqBandListNr.list.array[i];
    freq_band_f1ap_t* dst_frq = &dst.frq_band[i]; 

    // mandatory
    // 1.3.1 freqbandindicatornr 
    assert(src_frq->freqBandIndicatorNr < 1025);
    dst_frq->frq_band = src_frq->freqBandIndicatorNr;

    // optional
    /* 1.3.2 supportedsulbandlist*/
    if(src_frq->supportedSULBandList.list.count > 0){
      assert(src_frq->supportedSULBandList.list.count < 33);
      dst_frq->sz_sul_band = src_frq->supportedSULBandList.list.count; 
      dst_frq->sul_band = calloc(dst_frq->sz_sul_band, sizeof(uint16_t)); 
      assert( dst_frq->sul_band != NULL && "memory exhausted" );

      for(int j = 0; j < dst_frq->sz_sul_band; ++j ){
        assert(src_frq->supportedSULBandList.list.array[j]->freqBandIndicatorNr < 1025);
        // mandatory
        // tdd.1.3.2.1 freqbandindicatornr 
        dst_frq->sul_band[j] = src_frq->supportedSULBandList.list.array[j]->freqBandIndicatorNr; 
      }
    }

    // optional
    // frequency shift 7p5khz
    assert(src->iE_Extensions == NULL && "not implemented" );
  }

  return dst;
}

static
trans_bw_f1ap_t cp_trans_bw_ir(F1AP_Transmission_Bandwidth_t const* src)
{
  assert(src != NULL);

  trans_bw_f1ap_t dst = {0}; 

  // mandatory
  // nr scs
  dst.nr_scs = src->nRSCS;
  
  // mandatory
  // nrb
  dst.n_rb = src->nRNRB;

  return dst;
}

static
fdd_info_f1ap_t cp_fdd_info_ir(F1AP_FDD_Info_t const* src)
{
  fdd_info_f1ap_t dst = {0}; 

  // mandatory
  // ul freqinfo
  dst.ul_frq_info = cp_frq_info_ir(&src->uL_NRFreqInfo);

  // mandatory
  // dl freqinfo
  dst.dl_frq_info = cp_frq_info_ir(&src->dL_NRFreqInfo);

  // mandatory
  // ul transmission bandwidth
  dst.ul_bw = cp_trans_bw_ir(&src->uL_Transmission_Bandwidth);

  // mandatory
  // dl transmission bandwidth
  dst.dl_bw = cp_trans_bw_ir(&src->dL_Transmission_Bandwidth);

  // optional
  //ul carrier list
  assert(src->iE_Extensions == NULL && "not implemented");

  // optional
  // dl carrier list
  assert(src->iE_Extensions == NULL && "not implemented");

  return dst;
}

static
tdd_info_f1ap_t cp_tdd_info_ir(F1AP_TDD_Info_t const* src)
{
  tdd_info_f1ap_t dst = {0}; 

  // mandatory
  // nr freqinfo
  dst.freq_info = cp_frq_info_ir(&src->nRFreqInfo); 

  // mandatory
  // transmission bandwidth
  dst.bw = cp_trans_bw_ir(&src->transmission_Bandwidth);

  // optional
  // intended tdd dl-ul configuration
  assert(src->iE_Extensions == NULL && "not implemented");

  // optional
  // tdd ul-dl configuration common nr
  assert(src->iE_Extensions == NULL && "not implemented");

  // optional
  // carrier list
  assert(src->iE_Extensions == NULL && "not implemented");

  return dst;
}

static
srv_cell_info_t cp_srv_cell_info_ir(F1AP_Served_Cell_Information_t const* src_srv_cell_info)
{
  assert( src_srv_cell_info != NULL);

  srv_cell_info_t dst = {0};

  // mandatory
  // nr cgi
  TBCD_TO_MCC_MNC(&src_srv_cell_info->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_srv_cell_info->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1ul << 36) );

  // mandatory 
  // nr pci 
  assert(src_srv_cell_info->nRPCI < 1008); 
  dst.nr_pci = src_srv_cell_info->nRPCI;

  // optional 
  // - fivegs_tac 
  if(src_srv_cell_info->fiveGS_TAC != NULL) {
      assert(src_srv_cell_info->fiveGS_TAC->size == 3);
      dst.five_gs_tac = malloc(sizeof(uint32_t));
      assert(dst.five_gs_tac != NULL);
      *dst.five_gs_tac = cp_ostring_to_uint32(*src_srv_cell_info->fiveGS_TAC);
  }

    // optional 
    // - configured_EPS_TAC 
  if(src_srv_cell_info->configured_EPS_TAC != NULL){
    assert(src_srv_cell_info->configured_EPS_TAC->size == 2);
    dst.eps_tac = malloc(sizeof(uint16_t));
    assert(dst.eps_tac != NULL && "memory exhausted");
    *dst.eps_tac = cp_ostring_to_uint16(*src_srv_cell_info->configured_EPS_TAC);
  } 

    // mandatory 
    // servedplmn information 
    size_t const sz_srv_plmn = src_srv_cell_info->servedPLMNs.list.count;
    assert(sz_srv_plmn > 0);
    dst.sz_srv_plmn = sz_srv_plmn; 

    dst.srv_plmn = calloc(sz_srv_plmn, sizeof(srv_plmn_f1ap_t)); 
    assert(dst.srv_plmn != NULL && "memory exhausted");

    for(size_t i =0; i < sz_srv_plmn; ++i){
      dst.srv_plmn[i] = cp_srv_plmn_ir(src_srv_cell_info->servedPLMNs.list.array[i]);
    }

    // mandatory
    // choice nr-mode-info
  assert(src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_fDD ||  src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_tDD);

  dst.nr_mode_info = src_srv_cell_info->nR_Mode_Info.present == F1AP_NR_Mode_Info_PR_fDD ? FDD_NR_MODE_INFO_F1AP : TDD_NR_MODE_INFO_F1AP;
  assert(dst.nr_mode_info == FDD_NR_MODE_INFO_F1AP || dst.nr_mode_info == TDD_NR_MODE_INFO_F1AP);

  if(dst.nr_mode_info == FDD_NR_MODE_INFO_F1AP )
   dst.fdd = cp_fdd_info_ir(src_srv_cell_info->nR_Mode_Info.choice.fDD);
  else // tdd_nr_mode_info_f1ap
   dst.tdd = cp_tdd_info_ir(src_srv_cell_info->nR_Mode_Info.choice.tDD);

  // mandatory
  // measurement timing configuration
  dst.meas_timing_conf = copy_ostring_to_ba(src_srv_cell_info->measurementTimingConfiguration);

  // optional
  // ran area code
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // extended served plmns item
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // cell direction
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // cell type
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // broadcast plmn identity info list
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // configured tac indication
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  //  aggressor gnb set id
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  //  victim gnb set id
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  //  iab info iab-du
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  // optional
  // ssb positions in burst
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  //optional
  //nr prach configuration
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  //optional
  //sfn offset
  assert(src_srv_cell_info->iE_Extensions == NULL && "not implemented");

  return dst;
}

static
gnb_du_srv_cell_f1ap_t cp_srv_cell_ir(F1AP_GNB_DU_Served_Cells_ItemIEs_t const* src_srv_cell)
{
  assert(src_srv_cell->id == F1AP_ProtocolIE_ID_id_GNB_DU_Served_Cells_Item);
  assert(src_srv_cell->criticality == F1AP_Criticality_reject);
  assert(src_srv_cell->value.present == F1AP_GNB_DU_Served_Cells_ItemIEs__value_PR_GNB_DU_Served_Cells_Item);

  gnb_du_srv_cell_f1ap_t dst_srv_cell = {0};
  
  // mandatory 
  // 4.1.1 served cell information 
  dst_srv_cell.srv_cell_info = cp_srv_cell_info_ir(&src_srv_cell->value.choice.GNB_DU_Served_Cells_Item.served_Cell_Information);

  // optional 
  // gnb-du system information

  return dst_srv_cell; 
}

static
srv_cell_span_t cp_srv_cell_array_ir(F1AP_F1SetupRequestIEs_t const* ie)
{
  assert(ie->criticality == F1AP_Criticality_reject);
  assert(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_GNB_DU_Served_Cells_List);

  size_t const sz = ie->value.choice.GNB_DU_Served_Cells_List.list.count;
  assert(sz > 0 && sz < 513);

  srv_cell_span_t dst = {0};
  dst.sz_srv_cell = sz;
  dst.srv_cell = calloc(sz, sizeof(gnb_du_srv_cell_f1ap_t));
  assert(dst.srv_cell != NULL && "memory exhausted");

  for(size_t i = 0; i < sz; ++i){
    // mandatory 
    // 4.1 served cells item 
    F1AP_GNB_DU_Served_Cells_ItemIEs_t const* src_srv_cell = (F1AP_GNB_DU_Served_Cells_ItemIEs_t const*)(ie->value.choice.GNB_DU_Served_Cells_List.list.array[i]);
    dst.srv_cell[i] = cp_srv_cell_ir(src_srv_cell);

    // optional
    // gnb-du system information
    // not implemented 
  }

  return dst; 
} 

static
gnb_rrc_ver_f1ap_t cp_ver_ir(F1AP_RRC_Version_t const* src)
{
  assert(src != NULL);
  gnb_rrc_ver_f1ap_t dst = {0}; 

  dst.latest_rrc_version = copy_bit_string(&src->latest_RRC_Version);

  // optional
  // latest rrc version enhanced

  return dst;
}

static
sib_t cp_sib_ir(F1AP_SibtypetobeupdatedListItem_t const* src)
{
  assert(src != NULL);

  sib_t dst = {0};

  // type: indicates a certain sib block,
  // e.g. 2 means sibtype2, 3 for
  // sibtype3, etc. values 6, 7, 8
  // and values 10 and higher are
  // not applicable in this version
  // of the specifications.
  
  assert(src->sIBtype > 1 && (src->sIBtype < 5 || src->sIBtype == 9));
  dst.type = src->sIBtype;

  // mandatory
  dst.msg = copy_ostring_to_ba(src->sIBmessage);

  // mandatory
  assert(src->valueTag < 32);
  dst.tag = src->valueTag; // integer (0..31, ...)

  // optional
  //sib_area_scope_t* area_scope;

  return dst;
}

static
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
  assert(src_info->sibtypetobeupdatedlist.list.count > 0 && src_info->sibtypetobeupdatedlist.list.count < 33 && "out of range" );

  dst.sz_sib = src_info->sibtypetobeupdatedlist.list.count;
  dst.sib = calloc(dst.sz_sib, sizeof(sib_t) );
  assert(dst.sib != NULL && "memory exhausted");

  for(int i = 0; i < dst.sz_sib; ++i){
     dst.sib[i] = cp_sib_ir(src_info->sibtypetobeupdatedlist.list.array[i] ); 
  }

  // optional
  //  assert(src->id == NULL && "not implemented");

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
  // nr cgi
  TBCD_TO_MCC_MNC(&src_it->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_it->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1ul << 36) );
  //dst.nr_cgi = cp_nr_cgi_ir(&src_it->nrcgi);

  // nr pci optional
  if(src_it->nRPCI != NULL){
    dst.nr_pci = malloc(sizeof(uint16_t));
    assert(dst.nr_pci != NULL);

    assert(*src_it->nRPCI < 1008);
    *dst.nr_pci = *src_it->nRPCI;
  }

  if(src_it->iE_Extensions != NULL){
    F1AP_ProtocolExtensionContainer_154P46_t const* src_cont = (F1AP_ProtocolExtensionContainer_154P46_t const*)src_it->iE_Extensions;
    assert(src_cont->list.count == 1 && "only sys information implemented" );

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

  F1AP_RRC_Version_t const* src_rrc = &src->value.choice.RRC_Version;

  // mandatory
  // latest rrc version
  assert(src_rrc->latest_RRC_Version.bits_unused == 8 - 3 && "only 3 bits used");
  dst.latest_rrc_version = copy_bit_string(&src_rrc->latest_RRC_Version); 

  // optional
  // latest rrc version enhanced
  assert(src_rrc->iE_Extensions == NULL && "not implemented");

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
/////////// f1ap setup ///////////
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
  // c1. transaction id (integer value) 
  dst.trans_id = cp_trans_id_ir(src_f1_setup->protocolIEs.list.array[0]);

  // mandatory 
  // c2. gnb_du_id (integer value) 
  dst.gnb_du_id = cp_gnb_du_id_ir(src_f1_setup->protocolIEs.list.array[1]);

  for(size_t i = 2; i < src_f1_setup->protocolIEs.list.count; ++i) {
    F1AP_F1SetupRequestIEs_t const* ie = src_f1_setup->protocolIEs.list.array[i]; 

    // optional 
    // c3. gnb_du_name   
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
      assert(0!=0 && "not implemented"); 
      // optional
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_BAPAddress){
      assert(0!=0 && "not implemented"); 
      // optional
    } else if(ie->value.present == F1AP_F1SetupRequestIEs__value_PR_Extended_GNB_CU_Name){
      assert(0!=0 && "not implemented"); 
    } else {
      assert(0 != 0 && "unknown type");
    }
  }

  return dst;
}

//////////////////////////////////////////
//////////////f1 setup response //////////
//////////////////////////////////////////

f1_setup_response_t cp_f1_setup_response_ir(F1AP_F1AP_PDU_t const* src)
{
  assert(src != NULL);

  f1_setup_response_t dst = {0};

  /* create */
  /* 0. pdu type */
  assert(src->present == F1AP_F1AP_PDU_PR_successfulOutcome);
  
  F1AP_SuccessfulOutcome_t* const src_out = src->choice.successfulOutcome;

  assert(src_out->procedureCode == F1AP_ProcedureCode_id_F1Setup);
  assert(src_out->criticality == F1AP_Criticality_reject);
  assert(src_out->value.present == F1AP_SuccessfulOutcome__value_PR_F1SetupResponse);

  F1AP_F1SetupResponse_t const* src_f1_sr = &src_out->value.choice.F1SetupResponse;
  assert(src_f1_sr->protocolIEs.list.count > 1 && src_f1_sr->protocolIEs.list.count < 9);

  // mandatory 
  // c1. transaction id (integer value)
  dst.trans_id = cp_trans_id_sr_ir(src_f1_sr->protocolIEs.list.array[0]);

  for(int i = 1; i < src_f1_sr->protocolIEs.list.count; ++i){
    F1AP_F1SetupResponseIEs_t const* ie = src_f1_sr->protocolIEs.list.array[i];
   
    assert(ie->id == F1AP_ProtocolIE_ID_id_gNB_CU_Name 
        || ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List 
        || ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_RRC_Version
        || !("unknown or unimplemented ie id") );
    
    if(ie->id == F1AP_ProtocolIE_ID_id_gNB_CU_Name){
       assert(ie->criticality == F1AP_Criticality_ignore);
       assert(ie->value.present == F1AP_F1SetupResponseIEs__value_PR_GNB_CU_Name);

       dst.gnb_cu_name = calloc(1, sizeof(byte_array_t));
       assert(dst.gnb_cu_name != NULL);

       *dst.gnb_cu_name = copy_ostring_to_ba(ie->value.choice.GNB_CU_Name); 

    } else if(ie->id ==  F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List){
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
      assert(0 != 0 && "unknown or unimplemented id");
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
  if(src->value.choice.Cause.present == F1AP_Cause_PR_radioNetwork  ){
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
    assert(0!=0 && "unknown type");
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
   assert(dst != NULL && "memory exhausted");

  //optional
  //procedure code
  if(src->procedureCode != NULL){
    dst->proc_code = malloc(sizeof(uint8_t));
    assert(dst->proc_code != NULL);

    assert(*src->procedureCode < 256);
    *dst->proc_code = *src->procedureCode; 
  }
  
  //optional
  //triggering message
  if(src->triggeringMessage != NULL){
    dst->trig_msg = malloc(sizeof(trig_msg_e));
    assert(dst->trig_msg != NULL && "memory exhausted");
 
    *dst->trig_msg = *src->triggeringMessage;
    assert(*dst->trig_msg < END_TRIGGERING_MESSAGE_F1AP);
  }

  //optional
  //procedure criticality
  if(src->procedureCriticality != NULL){
    dst->proc_crit = malloc(sizeof(proc_crit_e));
    assert(dst->proc_crit != NULL);
    
    *dst->proc_crit = *src->procedureCriticality;
    assert(*dst->proc_crit < END_PROCEDURE_CRITICALLITY_F1AP);
  }

  //optional
  //transaction id
  if(src->transactionID != NULL){
    dst->trans_id = malloc(sizeof(uint8_t));
    assert(dst->trans_id != NULL && "memory exhausted");

    assert(*src->transactionID < 256);
    *dst->trans_id = *src->transactionID;
  }

  // optional
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
//////////////f1 setup failure //////////
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

  // mandatory
  //transaction id
  dst.trans_id = cp_trans_id_sf(src_out->value.choice.F1SetupFailure.protocolIEs.list.array[0]);

  //mandatory
  //cause
  dst.cause = cp_cause_sf(src_out->value.choice.F1SetupFailure.protocolIEs.list.array[1]); 

  for(size_t i = 2; i < src_out->value.choice.F1SetupFailure.protocolIEs.list.count; ++i){
    F1AP_F1SetupFailureIEs_t const* ie = src_out->value.choice.F1SetupFailure.protocolIEs.list.array[i]; 

    // optional
    //time to wait
    if(ie->id == F1AP_ProtocolIE_ID_id_TimeToWait){
      assert(ie->criticality == F1AP_Criticality_ignore);
      assert(ie->value.present == F1AP_F1SetupFailureIEs__value_PR_TimeToWait);

      dst.time_to_wait = malloc(sizeof(time_to_wait_f1ap_e));
      assert(dst.time_to_wait != NULL && "memory exhausted");
      *dst.time_to_wait = ie->value.choice.TimeToWait;
      //optional
      //criticality diagnostics
    } else if(ie->id == F1AP_ProtocolIE_ID_id_CriticalityDiagnostics){
      assert(ie->criticality == F1AP_Criticality_ignore);
      assert(ie->value.present == F1AP_F1SetupFailureIEs__value_PR_CriticalityDiagnostics);
      F1AP_CriticalityDiagnostics_t	const * src_crit = &ie->value.choice.CriticalityDiagnostics;

      dst.diagnose = cp_crit_diagnose_sf(src_crit);
    } else { 
      assert(0!=0 && "incorrect id??" );
    }

  }

  return dst;
}

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
//////////////////////////////////////////

//////////////////////////////////////////
//////////////f1 setup failure //////////
//////////////////////////////////////////

static
uint32_t cp_gnb_cu_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID);	
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupRequestIEs__value_PR_GNB_CU_UE_F1AP_ID);

  assert(src->value.choice.GNB_CU_UE_F1AP_ID < (1ul << 32));
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
  // nr cgi
  TBCD_TO_MCC_MNC(& src->value.choice.NRCGI.pLMN_Identity, dst.plmn_id.mcc, dst.plmn_id.mnc, dst.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src->value.choice.NRCGI.nRCellIdentity, dst.nr_cell_id);
  assert(dst.nr_cell_id < (1ul << 36) );

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
  assert(dst != NULL && "memory exhausted");

  if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_ul){
    *dst = UL_CELL_UL_CONF;
  } else if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_sul){
    *dst = SUL_CELL_UL_CONF;
  } else if(src->value.choice.CellULConfigured == F1AP_CellULConfigured_ul_and_sul){
    *dst = UL_SUL_CELL_UL_CONF;
  } else {
    assert(0 != 0 && "unknown type");
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
    assert(dst.cg_config_info != NULL && "memory exhausted");
    *dst.cg_config_info = copy_ostring_to_ba(*src_cu->cG_ConfigInfo);
  }

  if(src_cu->uE_CapabilityRAT_ContainerList != NULL){
    dst.ue_capability_rat_con_lst = calloc(1, sizeof(byte_array_t)); 
    assert(dst.ue_capability_rat_con_lst != NULL && "memory exhausted");
    *dst.ue_capability_rat_con_lst = copy_ostring_to_ba(*src_cu->uE_CapabilityRAT_ContainerList);
  }

  if(src_cu->measConfig != NULL){
    dst.meas_config= calloc(1, sizeof(byte_array_t)); 
    assert(dst.meas_config!= NULL && "memory exhausted");
    *dst.meas_config= copy_ostring_to_ba(*src_cu->measConfig);
  }

  // optional
  // handover preparation information
  //assert(src->handover_prep_info == NULL && "not implemented");

  // optional
  // cellgroupconfig
  //assert(src->cell_group_config == NULL && "not implemented");

  // optional
  // measurement timing configuration
  //assert(src->meas_timing_conf == NULL && "not implemented");

  // optional
  // ueassistanceinformation
  //assert(src->ue_assistance_info == NULL && "not implemented");

  // optional
  // cg-config
  //assert(src->cg_config == NULL && "not implemented");

  // optional
  // ueassistanceinformationeutra
  //assert(src->ue_assistance_info_eutra == NULL && "not implemented");

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

  // nr cgi
  TBCD_TO_MCC_MNC(&src_nr_cgi->pLMN_Identity, dst.plmn_id.mcc, dst.plmn_id.mnc, dst.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_nr_cgi->nRCellIdentity, dst.nr_cell_id);
  assert(dst.nr_cell_id < (1ul << 36) );

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
  assert(dst.candidate_sp_cell_id != NULL && "memory exhausted");

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

  // mandatory 
  // long drx cycle length
  F1AP_DRXCycle_t const* src_drx = &src->value.choice.DRXCycle;
  dst->long_drx_cycle_len =  src_drx->longDRXCycleLength;
  // optional 
  // short drx cycle length
  assert(src_drx->shortDRXCycleLength == NULL && "not implemented");

  // optional
  // short drx cycle timer integer (1..16)
  assert(src_drx->shortDRXCycleTimer == NULL && "not implemented");

  return dst;
}

static
byte_array_t* cp_res_coord_trans_ue_sr_ir(F1AP_UEContextSetupRequestIEs_t const* src)
{
  assert(src != NULL);

  byte_array_t* dst = calloc(1, sizeof(byte_array_t));
  assert(dst != NULL && "memory exhausted");

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
scell_to_be_setup_t cp_scell_to_be_setup_item_ue_sr_ir( F1AP_SCell_ToBeSetup_ItemIEs_t const* src)
{
  assert(src != NULL);

  scell_to_be_setup_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_SCell_ToBeSetup_ItemIEs__value_PR_SCell_ToBeSetup_Item);

  F1AP_SCell_ToBeSetup_Item_t const* src_item = &src->value.choice.SCell_ToBeSetup_Item;
  // mandatory
  // scell id 
  F1AP_NRCGI_t const* src_nr_cgi = &src_item->sCell_ID;

  // nr cgi
  TBCD_TO_MCC_MNC(&src_nr_cgi->pLMN_Identity, dst.scell_id.plmn_id.mcc, dst.scell_id.plmn_id.mnc, dst.scell_id.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_nr_cgi->nRCellIdentity, dst.scell_id.nr_cell_id);
  assert(dst.scell_id.nr_cell_id < (1ul << 36) );

  // mandatory
  // scellindex
  assert(src_item->sCellIndex < 32 &&  src_item->sCellIndex); // integer (1..31
  dst.scell_idx = src_item->sCellIndex;

  // optional
  // scell ul configured
  if(src_item->sCellULConfigured != NULL){
    dst.scell_ul_conf = calloc(1, sizeof(cell_ul_conf_e)); 
    assert(dst.scell_ul_conf != NULL && "memory exhausted" );

    if(*src_item->sCellULConfigured== F1AP_CellULConfigured_ul){
      *dst.scell_ul_conf = UL_CELL_UL_CONF;
    } else if(*src_item->sCellULConfigured == F1AP_CellULConfigured_sul) {
      *dst.scell_ul_conf = SUL_CELL_UL_CONF;
    } else if(*src_item->sCellULConfigured == F1AP_CellULConfigured_ul_and_sul) {
      *dst.scell_ul_conf = UL_SUL_CELL_UL_CONF;
    } else {
      assert( 0 != 0 && "unknown type");
    }
  }

  // optional
  // servingcellmo
  assert(src_item->iE_Extensions == NULL && "not implemented"); // integer(1..64) 

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
    assert(dst.scell_to_be_setup != NULL && "memory exhausted" );
  }

  for(size_t i = 0; i < dst.sz; ++i){
    F1AP_SCell_ToBeSetup_ItemIEs_t const* ie = (F1AP_SCell_ToBeSetup_ItemIEs_t const*)src->value.choice.SCell_ToBeSetup_List.list.array[i]; 
    dst.scell_to_be_setup[i] = cp_scell_to_be_setup_item_ue_sr_ir(ie);
  }

  return dst;
}

typedef struct{
  // srb to be setup item
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

  // mandatory
  //  srb id
  assert(src->value.choice.SRBs_ToBeSetup_Item.sRBID < 4); // [0,3]
  dst.srb_id = src->value.choice.SRBs_ToBeSetup_Item.sRBID;

  // optional
  // duplication indication
  assert(src->value.choice.SRBs_ToBeSetup_Item.duplicationIndication == NULL && "not implemented");

  // optional 
  // additional duplication indication
  assert(src->value.choice.SRBs_ToBeSetup_Item.iE_Extensions == NULL && "not implemented");

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
  assert(dst.srb_to_be_setup != NULL && "memory exhausted" );

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

  // mandatory
  // priority level
  assert(src->priorityLevel < 16);
  dst.prio_level = src->priorityLevel; // integer (0..15)
                      
  // mandatory
  // pre-emption capability
  if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption){
    dst.preempt_capability = SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_may_trigger_pre_emption){
    dst.preempt_capability = MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY; 
  } else {
    assert(0!=0 && "unknown type");
  }

  // mandatory
  // pre-emption vulnerability
  if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_not_pre_emptable){
    dst.preempt_vulnerability = NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY;  
  } else if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_pre_emptable){
    dst.preempt_vulnerability = PRE_EMPTABLE_PREEMPT_VULNERABILITY;  
  } else {
    assert(0!=0 && "unknown type");
  }

  return dst;
}

static
e_utran_qos_t cp_e_utran_qos(F1AP_EUTRANQoS_t const* src)
{
  assert(src != NULL);

  e_utran_qos_t dst = {0};

  // mandatory
  // qci
  assert(src->qCI < 256);
  dst.qci = src->qCI; 

  // mandatory
  // allocation and retention priority
  dst.alloc = cp_alloc_retention_prio(&src->allocationAndRetentionPriority);

  // optional
  // gbr qos information 9.3.1.21
  assert(src->gbrQosInformation == NULL && "not implemented" );

  return dst;
}

static
s_nssai_t cp_nssai(F1AP_SNSSAI_t const* src)
{
  assert(src != NULL);
  s_nssai_t dst = {0};

  // mandatory
  // octet string (size(1))
  assert(src->sST.size == 1);
  memcpy(&dst.sst, src->sST.buf, 1) ;

  // optional
  // octet string (size(3))
  assert(src->sD == NULL && "not implemented"); // size 3 bytes

  return dst;
}

static
non_dyn_5qi_descriptor_t cp_non_dyn_5qi(F1AP_NonDynamic5QIDescriptor_t const* src)
{
  assert(src != NULL);

  non_dyn_5qi_descriptor_t dst = {0}; 

  // mandatory
  //  5qi
  assert(src->fiveQI < 256);
  dst.five_qi = src->fiveQI;

  // optional
  // priority level
  assert(src->qoSPriorityLevel == NULL && "not implemented"); // [1,127]

  // optional
  // averaging window 9.3.1.53 integer (0..4095, ...)
  assert(src->averagingWindow == NULL && "not implemented");

  // optional 
  // maximum data burst volume 9.3.1.54
  assert(src->maxDataBurstVolume == NULL && "not implemented"); // integer (0..4095, ..., 4096..2000000)

  // optional 
  // cn packet delay budget downlink 9.3.1.145
  assert(src->iE_Extensions == NULL && "not implemented");
  //assert(src-> == NULL && "not implemented"); // integer (0..65535, …)

  // optional 
  // cn packet delay budget uplink // integer (0..65535, …)
  //assert(src->pkt_delay_budget_uplink == NULL && "not implemented"); // integer (0..65535, …)

  return dst;
}

static
dyn_5qi_descriptor_t cp_dyn_5qi(F1AP_Dynamic5QIDescriptor_t const* src)
{
  assert(src != NULL);

  dyn_5qi_descriptor_t dst = {0}; 

  //mandatory
  //qos priority level
  assert(src->qoSPriorityLevel > 0 && src->qoSPriorityLevel < 128);
  dst.qos_prio_level = src->qoSPriorityLevel; // integer (1..127)

  // mandatory
  //packet delay budget 9.3.1.51
  assert(src->packetDelayBudget < 1024);
  dst.pkt_delay_budget = src->packetDelayBudget; // integer (0..1023, ...)

  // mandatory
  // packet error rate 9.3.1.52
  dst.pkt_error_rate.scalar = src->packetErrorRate.pER_Scalar;
  dst.pkt_error_rate.exponent = src->packetErrorRate.pER_Exponent;

  // optional
  // 5qi
  assert(src->fiveQI == NULL && "not implemented");

  // c-ifgbrflow
  // delay critical
  assert(src->delayCritical == NULL && "not implemented");

  // c-ifgbrflow
  // averaging window 9.3.1.53
  assert(src->averagingWindow == NULL && "not implemented"); // integer (0..4095, ...)

  // optional 
  // maximum data burst volume 9.3.1.54
  assert(src->maxDataBurstVolume == NULL && "not implemented"); //integer (0..4095, ..., 4096..2000000)

  // optional 
  // extended packet delay budget 9.3.1.145
  assert(src->iE_Extensions == NULL && "not implemented" );
  //assert(src->ext_pkt_delay_budget == NULL && "not implemented"); //integer (0..65535, …) 

  // optional 
  // cn packet delay budget downlink
  // assert(src->cn_pkt_dalay_budget_downlink == NULL && "not implemented"); 

  // optional
  // cn packet delay budget uplink
  // assert(src->cn_pkt_dalay_budget_uplink == NULL && "not implemented"); 

  return dst;
}

static
alloc_retention_prio_t cp_ng_ran_alloc_retention_prio(F1AP_NGRANAllocationAndRetentionPriority_t const* src)
{
  assert(src != NULL);

  alloc_retention_prio_t dst = {0}; 

  // mandatory
  // priority level
  assert(src->priorityLevel < 16);// integer (0..15)
  dst.prio_level = src->priorityLevel;

  // mandatory
  // pre-emption capability
  if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_shall_not_trigger_pre_emption){
    dst.preempt_capability = SHALL_NOT_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else if(src->pre_emptionCapability == F1AP_Pre_emptionCapability_may_trigger_pre_emption){
    dst.preempt_capability = MAY_TRIGGER_PRE_EMPTION_PREEMPT_CAPABILITY;
  } else {
    assert(0!=0 && "unknown type");
  } 

  // mandatory
  // pre-emption vulnerability
  //
  if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_not_pre_emptable){
    dst.preempt_vulnerability = NOT_PRE_EMPTABLE_PREEMPT_VULNERABILITY; 
  } else if(src->pre_emptionVulnerability == F1AP_Pre_emptionVulnerability_pre_emptable){
    dst.preempt_vulnerability = PRE_EMPTABLE_PREEMPT_VULNERABILITY;
  } else {
    assert(0!= 0 && "not known type");
  }

  return dst;
}

static
qos_flow_level_qos_parameter_t cp_qos_flow_level_qos_parameter(F1AP_QoSFlowLevelQoSParameters_t const* src)
{
  assert(src != NULL);

  qos_flow_level_qos_parameter_t dst = {0}; 
  //mandatory 
  // choice qos characteristics
  if(src->qoS_Characteristics.present == F1AP_QoS_Characteristics_PR_non_Dynamic_5QI ){
    // non dynamic 5qi descriptor 9.3.1.49
    dst.non_dyn = cp_non_dyn_5qi(src->qoS_Characteristics.choice.non_Dynamic_5QI); 
  } else if(src->qoS_Characteristics.present == F1AP_QoS_Characteristics_PR_dynamic_5QI){
    // dynamic 5qi descriptor 9.3.1.47
    dst.dyn = cp_dyn_5qi(src->qoS_Characteristics.choice.dynamic_5QI); 
  } else {
    assert(0!=0 && "unknown type");
  }

  // mandatory
  // ng-ran allocation and retention priority 9.3.1.48
  dst.alloc_retention_prio = cp_ng_ran_alloc_retention_prio(&src->nGRANallocationRetentionPriority);

  // optional 
  // gbr qos flow information 9.3.1.46
  assert(src->gBR_QoS_Flow_Information == NULL && "not implemented");

  // optional 
  // reflective qos attribute
  assert(src->reflective_QoS_Attribute == NULL && "not implemented");

  // optional 
  // pdu session id
  assert(src->iE_Extensions == NULL && "not implemented" );
  //uint8_t* pdu_session_id;

  // optional
  //ul pdu session aggregate maximum bit rate 9.3.1.22
  //uint64_t* ul_pdu_session_aggr_max_bit_rate; 

  // optional
  // qos monitoring request
  //qos_monitoring_request_e* qos_monitoring_request;

  return dst;
}

static
flows_mapped_to_drb_t cp_flows_mapped_to_drb_ue_sr_ir(F1AP_Flows_Mapped_To_DRB_Item_t const* src)
{
  assert(src != NULL);

  flows_mapped_to_drb_t dst = {0}; 

  // mandatory
  // qos flow identifier 9.3.1.63
  assert(src->qoSFlowIdentifier < 64);  // integer (0 ..63)
  dst.qfi = src->qoSFlowIdentifier; 

  // mandatory 
  // qos flow level qos parameters 9.3.1.45
  dst.qos_flow_level = cp_qos_flow_level_qos_parameter(&src->qoSFlowLevelQoSParameters); 

  // optional
  // qos flow mapping indication 9.3.1.72
  assert(src->iE_Extensions == NULL && "not implemented" );
  // qos_flow_mapping_ind 

  // optional
  // tsc traffic characteristics 9.3.1.141
  //assert(src->tsc_traffic_char == NULL && "not implemented");

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

  // mandatory
  // drb qos 9.3.1.45
  dst.drb_qos = cp_qos_flow_level_qos_parameter(&src_drb->dRB_QoS);

  // mandatory
  // s-nssai 9.3.1.38
  dst.nssai = cp_nssai(&src_drb->sNSSAI);

  // optional
  // notification control 9.3.1.56
  assert(src_drb->notificationControl == NULL && "not implemented");

  // flows mapped to drb
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

  // mandatory
  // ul up tnl information 9.3.2.1
  assert(src->uLUPTNLInformation.present == F1AP_UPTransportLayerInformation_PR_gTPTunnel );

  dst.tnl_info.tla = copy_bit_string(&src->uLUPTNLInformation.choice.gTPTunnel->transportLayerAddress);

  assert(src->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.size == 4);
  memcpy(dst.tnl_info.gtp_teid, src->uLUPTNLInformation.choice.gTPTunnel->gTP_TEID.buf, 4);

  // optional
  // bh information 9.3.1.114
  assert(src->iE_Extensions == NULL && "not implemented");

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
    assert(0!=0 && "unknown type");
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
  assert(dst.add_pdcp_dup_tnl != NULL && "memory exhausted");

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

  // mandatory
  // drb id 9.3.1.8
  assert(src_drb->dRBID < 33);;// integer [0,32]
  dst.drb_id = src_drb->dRBID;

  assert(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_eUTRANQoS 
      || src_drb->qoSInformation.present == F1AP_QoSInformation_PR_choice_extension);

  if(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_eUTRANQoS){
    dst.qos_info = E_UTRAN_QOS_INFORMATION;
    dst.e_utran_qos = cp_e_utran_qos(src_drb->qoSInformation.choice.eUTRANQoS);
  } else if(src_drb->qoSInformation.present == F1AP_QoSInformation_PR_choice_extension) {
    dst.qos_info = DRB_INFO_QOS_INFORMATION;
    dst.drb_info = cp_drb_info((F1AP_QoSInformation_ExtIEs_t const*)src_drb->qoSInformation.choice.choice_extension);
  } else {
    assert(0!= 0 && "not implemented");
  }

  // ul up tnl information to be setup list
  assert(src_drb->uLUPTNLInformation_ToBeSetup_List.list.count < 3);
  dst.sz_ul_up_tnl_info_lst = src_drb->uLUPTNLInformation_ToBeSetup_List.list.count; 
  if(dst.sz_ul_up_tnl_info_lst > 0){
    dst.ul_up_tnl_info = calloc(dst.sz_ul_up_tnl_info_lst, sizeof(ul_up_tnl_info_lst_t) );
    assert(dst.ul_up_tnl_info != NULL && "memory exhausted");

    for(size_t i = 0; i < dst.sz_ul_up_tnl_info_lst; ++i){
      dst.ul_up_tnl_info[i] = cp_ul_up_tnl_info_ue_sr_ir( src_drb->uLUPTNLInformation_ToBeSetup_List.list.array[i]);  
    }
  }

  // mandatory
  // rlc mode 9.3.1.27
  if(src_drb->rLCMode == F1AP_RLCMode_rlc_am){
    dst.rlc_mode = RLC_AM_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode ==   F1AP_RLCMode_rlc_um_bidirectional){
    dst.rlc_mode = RLC_UM_BIDIRECTIONAL_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode  == F1AP_RLCMode_rlc_um_unidirectional_ul){
    dst.rlc_mode = RLC_UM_UNIDIRECTIONAL_UL_RLC_MODE_F1AP;
  } else if(src_drb->rLCMode  == F1AP_RLCMode_rlc_um_unidirectional_dl){
    dst.rlc_mode = RLC_UM_UNIDIRECTIONAL_DL_RLC_MODE_F1AP;
  } else {
    assert( 0!= 0 && "unknown type");
  }

  // optional 
  // ul configuration 9.3.1.31
  assert(src_drb->uLConfiguration == NULL && "not implemented");

  // optional 
  // duplication activation 9.3.1.36
  assert(src_drb->duplicationActivation == NULL && "not implemented");

  // optional
  // dc based duplication configured
  // assert(src_drb->dc_dup_act_conf == NULL && "not implemented");

  // optional
  // dc based duplication activation 9.3.1.36
  // assert(src_drb->dc_dup_act == NULL && "not implemented");

  assert(((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.count > 0 );
  for(size_t i = 0; i < ((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.count; ++i){

    F1AP_DRBs_ToBeSetup_ItemExtIEs_t const* src_ie = ((F1AP_ProtocolExtensionContainer_154P82_t*)src_drb->iE_Extensions)->list.array[i];
    // mandatory
    // dl pdcp sn length
    if(src_ie->id == F1AP_ProtocolIE_ID_id_DLPDCPSNLength){
      dst.dl_pdcp_sn_len = cp_dl_pdcp_sn_len_ue_sr_ir( src_ie ) ;
      // additional pdcp duplication tnl list // [0,2]
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_AdditionalPDCPDuplicationTNL_List){
      add_pdcp_dup_tnl_span_t tmp = cp_add_pdcp_dup_tnl_ue_sr_ir(src_ie);
      assert(tmp.sz < 3);
      dst.sz_add_pdcp_dup_tnl = tmp.sz;
      dst.add_pdcp_dup_tnl = tmp.add_pdcp_dup_tnl;
    } else {
      assert(0!=0 && "unknown type");
    }

    // optional
    // rlc duplication information 9.3.1.146
    //assert(src->sz_rlc_dup_info < 4); // [0,3]
    //assert(src->sz_rlc_dup_info == 0 && "not implememted");
    //assert(src->rlc_dup_info == NULL && "not implememted");
    // for future implementation
    // f1ap_rlcduplicationinformation_t
    // f1ap_protocolie_id_id_rlcduplicationinformation	
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
  assert(dst.drb_to_be_setup != NULL && "memory exhausted");

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
  // mandatory
  //gnb-cu ue f1ap id 9.3.1.4
  dst.gnb_cu_ue_f1ap_id = cp_gnb_cu_ue_sr_ir(src->protocolIEs.list.array[0]);

  assert(src->protocolIEs.list.count < 37);
  for(size_t i = 1; i < src->protocolIEs.list.count; ++i){

    F1AP_UEContextSetupRequestIEs_t const* src_it = src->protocolIEs.list.array[i];

    // optional
    // gnb-du ue f1ap id 9.3.1.5
    if(src_it->id == F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID){
      dst.gnb_du_ue_f1ap_id = cp_gnb_du_ue_sr_ir(src_it);
      // mandatory
      // spcell id
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SpCell_ID){
      dst.sp_cell_id = cp_spcell_id_ue_sr_ir(src_it);
      // mandatory
      // servcellindex
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_ServCellIndex){
      dst.serv_cell_idx = cp_serv_cell_idx_ue_sr_ir(src_it);
      // optional
      // spcell ul configured 9.3.1.33
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SpCellULConfigured){
      dst.sp_cell_ul_conf = cp_sp_cell_ul_conf_ue_sr_ir(src_it);
      // mandatory
      // cu to du rrc information 9.3.1.25
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_CUtoDURRCInformation){
      dst.cu_to_du_rrc_info = cp_cu_to_du_rrc_info_ue_sr_ir(src_it);
      // optional 
      // candidate spcell id
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_Candidate_SpCell_List){
      candidate_sp_cell_id_span_t tmp = cp_candidate_sp_cell_id_ue_sr_ir(src_it);
      dst.candidate_sp_cell_id = tmp.candidate_sp_cell_id;
      dst.sz_candidate_sp_cell_id = tmp.sz;
      //optional 
      // drx cycle
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_DRXCycle){
      dst.drx_cycle = cp_drx_cycle_ue_sr_ir(src_it);
      // optional
      // resource coordination transfer container
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer){
      dst.resource_coord_transfer_container = cp_res_coord_trans_ue_sr_ir(src_it);
      // scell to be setup list
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SCell_ToBeSetup_List){
      scell_to_be_setup_span_t tmp = cp_scell_to_be_setup_ue_sr_ir(src_it);
      dst.scell_to_be_setup = tmp.scell_to_be_setup;
      dst.sz_scell_to_be_setup = tmp.sz;
      // srb to be setup item
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_SRBs_ToBeSetup_List){
      srb_to_be_setup_span_t tmp = cp_srb_to_be_setup_ue_sr_ir(src_it);
      dst.srb_to_be_setup = tmp.srb_to_be_setup; 
      dst.sz_srb_to_be_setup = tmp.sz;
      // drb to be setup item
    } else if(src_it->id == F1AP_ProtocolIE_ID_id_DRBs_ToBeSetup_List){
      drb_to_be_setup_span_t tmp = cp_drb_to_be_setup_ue_sr_ir(src_it);
      dst.drb_to_be_setup = tmp.drb_to_be_setup; 
      dst.sz_drb_to_be_setup = tmp.sz;
    } else{
      assert(0!=0 && "unknown or unimplemented");
    }


    // optional
    // inactivity monitoring request

    // optional 
    // rat-frequency priority information 9.3.1.34

    // optional
    // rrc-container 9.3.1.6

    // optional 
    // masked imeisv 9.3.1.55

    // optional
    // serving plmn 9.3.1.14

    // c-ifdrbsetup optional
    // gnb-du ue aggregate maximum bit rate uplink 9.3.1.22

    // optional
    // rrc delivery status request

    // optional 
    // resource coordination transfer information 9.3.1.73

    // optional
    // servingcellmo 

    // optional 
    // new gnb-cu ue f1ap id 9.3.1.4

    // optional
    // ran ue id

    // optional
    // trace activation 9.3.1.88

    // optional
    // additional rrm policy index 9.3.1.90

    // bh rlc channel to be setup item [0, 65536]
    // size_t sz_bh_rlc_chan_to_be_setup; 
    // bh_rlc_chan_to_be_setup_t* ; 

    // optional
    // configured bap address 9.3.1.111

    // optional
    // nr v2x services authorized 9.3.1.116

    // optional
    // lte v2x services authorized 9.3.1.117

    // optional
    // nr ue sidelink aggregate maximum bit rate 9.3.1.119

    // optional
    // lte ue sidelink aggregate maximum bit rate 9.3.1.118

    // optional
    // pc5 link aggregate bit rate 9.3.1.22

    // optional
    // sl drb to be setup list [0, 64]

    // optional
    // conditional inter-du mobility information 

    // optional
    // management based mdt plmn list 9.3.1.151

    // optional
    // serving nid 9.3.1.155

    // optional
    // f1-c transfer path 9.3.1.207 

  }

  return dst;
}

/////////
/////////
/////////


static
uint32_t cp_gnb_cu_ue_id_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID);	
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupResponseIEs__value_PR_GNB_CU_UE_F1AP_ID);

  assert(src->value.choice.GNB_CU_UE_F1AP_ID < (1ul << 32) );
  return src->value.choice.GNB_CU_UE_F1AP_ID; 
}

static
uint32_t cp_gnb_du_ue_id_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID);	
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_UEContextSetupResponseIEs__value_PR_GNB_DU_UE_F1AP_ID);

  assert(src->value.choice.GNB_DU_UE_F1AP_ID < (1ul << 32) );
  return src->value.choice.GNB_DU_UE_F1AP_ID;
}

static
du_to_cu_rrc_information_f1ap_t cp_du_to_cu_rrc_info_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  du_to_cu_rrc_information_f1ap_t dst = {0}; 

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_DUtoCURRCInformation);	
  assert(src_ie->criticality == F1AP_Criticality_reject);
  assert(src_ie->value.present == F1AP_UEContextSetupResponseIEs__value_PR_DUtoCURRCInformation);

 F1AP_DUtoCURRCInformation_t const* src = &src_ie->value.choice.DUtoCURRCInformation;

  // mandatory
  // cellgroupconfig
  dst.cell_group_config = copy_ostring_to_ba(src->cellGroupConfig);

  // optional
  // measgapconfig
  assert(src->measGapConfig == NULL && "not implemented"); 

  // optional
  // requested p-maxfr1
  assert(src->requestedP_MaxFR1 == NULL && "not implemented"); 


  assert(src->iE_Extensions == NULL && "not implemented");

  // optional
  // drx long cycle start offset
  // assert(src->drx_lng_cyc_str_off== NULL && "not implemented"); // [0..10239]

  // optional
  // selected bandcombinationindex
  // assert(src->bnd_comb_idx== NULL && "not implemented"); 

  // optional
  // selected featuresetentryindex
  // assert(src->sel_feature_set_entry_idx== NULL && "not implemented"); 

  // optional
  // ph-infoscg
  //  assert(src->ph_info_scg== NULL && "not implemented"); 

  // optional
  // requested bandcombinationindex
  // assert(src->req_bnd_cmb_idx== NULL && "not implemented"); 

  // optional
  // requested featuresetentryindex
  // assert(src->req_feat_set_entry_idx== NULL && "not implemented"); 

  // optional
  // drx config
  // assert(src->drx_cnfg== NULL && "not implemented"); 

  // optional
  // pdcch blinddetectionscg
  // assert(src->pdcch_blind_det_scg== NULL && "not implemented"); 

  // optional
  // requested pdcch blinddetectionscg
  // assert(src->req_pdcch_blnd_det_scg== NULL && "not implemented"); 

  // optional
  // ph-infomcg
  // assert(src->ph_info_mcg== NULL && "not implemented"); 

  // optional
  // measgapsharingconfig 
  // assert(src->meas_gap_shr_conf== NULL && "not implemented"); 

  // optional
  // sl-phy-mac-rlc-config
  // assert(src->sl_phy_mac_rlc_conf== NULL && "not implemented"); 

  // optional
  // sl-configdedicatedeutra-info 
  // assert(src->sl_conf_dedica_eutra_info== NULL && "not implemented"); 

  // optional
  // requested p-maxfr2 
  // assert(src->req_p_max_fr2== NULL && "not implemented"); 

  return dst;
}

static
uint16_t* cp_c_rnti_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  uint16_t* dst = malloc(sizeof(uint16_t));
  assert(dst != NULL && "memory exhausted");

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_C_RNTI);	
  assert(src_ie->criticality == F1AP_Criticality_ignore);
  assert(src_ie->value.present == F1AP_UEContextSetupResponseIEs__value_PR_C_RNTI);

  // 9.3.1.32 [0, 65535]
  assert(src_ie->value.choice.C_RNTI <  65536);
  *dst = src_ie->value.choice.C_RNTI;

  return dst; 
}

static
byte_array_t* cp_res_coord_trans_cont_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  byte_array_t* dst = calloc(1, sizeof(byte_array_t)); 
  assert(dst != NULL && "memory exhausted");

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer);	
  assert(src_ie->criticality == F1AP_Criticality_ignore);
  assert(src_ie->value.present == F1AP_UEContextSetupResponseIEs__value_PR_ResourceCoordinationTransferContainer);

  *dst = copy_ostring_to_ba(src_ie->value.choice.ResourceCoordinationTransferContainer);

  return dst;
}

typedef struct{
  size_t sz;
  drb_setup_item_f1ap_t* drb_setup_item;
} drb_setup_item_arr_t;

typedef struct{
  size_t sz; // [1, 2]
  dl_up_trans_layer_info_t* dl_up_tnl_info_tbs;
} dl_up_tnl_info_tbs_arr_t;

static
dl_up_trans_layer_info_t cp_dl_up_trans_layer_info(struct F1AP_DLUPTNLInformation_ToBeSetup_Item const* src)
{
  assert(src != NULL);

  dl_up_trans_layer_info_t dst = {0};

  // 9.3.2.1
  assert(src->dLUPTNLInformation.present == F1AP_UPTransportLayerInformation_PR_gTPTunnel); 

  byte_array_t tmp = copy_ostring_to_ba(src->dLUPTNLInformation.choice.gTPTunnel->gTP_TEID);
  assert(tmp.len == 4);
  memcpy(dst.gtp_teid, tmp.buf, 4);
  free_byte_array(tmp);

  dst.trans_layer_addr = copy_bit_string(&src->dLUPTNLInformation.choice.gTPTunnel->transportLayerAddress);

  return dst;
}

static
dl_up_tnl_info_tbs_arr_t cp_dl_up_tnl_info_tbs_arr(F1AP_DLUPTNLInformation_ToBeSetup_List_t const* src)
{
  assert(src != NULL);

  dl_up_tnl_info_tbs_arr_t dst = {0}; 

  assert(src->list.count == 1 || src->list.count == 2);

  dst.sz = src-> list.count;

  dst.dl_up_tnl_info_tbs = calloc(dst.sz, sizeof(dl_up_trans_layer_info_t));
  assert(dst.dl_up_tnl_info_tbs != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.dl_up_tnl_info_tbs[i] = cp_dl_up_trans_layer_info(src->list.array[i]);
  }

  return dst;
}

static
drb_setup_item_f1ap_t cp_drb_setup_item_ie_setup_resp(F1AP_DRBs_Setup_ItemIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  drb_setup_item_f1ap_t dst = {0}; 

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_DRBs_Setup_Item);	
  assert(src_ie->criticality == F1AP_Criticality_ignore);
  assert(src_ie->value.present == F1AP_DRBs_Setup_ItemIEs__value_PR_DRBs_Setup_Item);

  F1AP_DRBs_Setup_Item_t const* src = &src_ie->value.choice.DRBs_Setup_Item;

  // Mandatory
  // DRB ID 9.3.1.8
  assert(src->dRBID > 0 && src->dRBID < 33);
  dst.drb_id = src->dRBID;

  // Optional
  // LCID 9.3.1.35
  if(src->lCID != NULL){ // [1, 32]
    dst.lc_id = malloc(sizeof(uint8_t)); 
    assert(dst.lc_id != NULL && "Memory exhausted");
    
    assert(*src->lCID > 0 && *src->lCID < 33);
    *dst.lc_id = *src->lCID; 
  }


  // DL UP TNL Information to be setup
  dl_up_tnl_info_tbs_arr_t tmp = cp_dl_up_tnl_info_tbs_arr(&src->dLUPTNLInformation_ToBeSetup_List);

  dst.sz_dl_up_tnl_info_tbs = tmp.sz;
  dst.dl_up_tnl_info_tbs = tmp.dl_up_tnl_info_tbs;

	  assert(src->iE_Extensions == NULL && "Not implemented");

  // Additional PDCP Duplication TNL List [0,1] 9.3.2.1 
  // iup_trans_layer_info_t* add_pdcp_dup_tnl_lst;

  // Optional
  // Current QoS Parameters Set Index 9.3.1.123 [1,8]
  // uint8_t* cur_qos_par_set_idx;

  return dst;
}

static
drb_setup_item_arr_t cp_drb_setup_item_setup_resp(F1AP_UEContextSetupResponseIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  drb_setup_item_arr_t dst = {0};

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_DRBs_Setup_List);	
  assert(src_ie->criticality == F1AP_Criticality_ignore);
  assert(src_ie->value.present == F1AP_UEContextSetupResponseIEs__value_PR_DRBs_Setup_List);

  dst.sz = src_ie->value.choice.DRBs_Setup_List.list.count;

  if(dst.sz > 0){
    dst.drb_setup_item = calloc(dst.sz, sizeof(drb_setup_item_f1ap_t));
    assert(dst.drb_setup_item != NULL && "Memory exhausted" );
  }

  for(size_t i = 0; i < dst.sz; ++i){
    dst.drb_setup_item[i] = cp_drb_setup_item_ie_setup_resp((F1AP_DRBs_Setup_ItemIEs_t*)src_ie->value.choice.DRBs_Setup_List.list.array[i]);
  }

  return dst;
}

typedef struct{
  size_t sz;
  srb_setup_item_t* srb_setup_item;
} srb_setup_item_arr_t; 

static
srb_setup_item_t cp_srb_setup_item_ie_stp_resp(F1AP_SRBs_Setup_ItemIEs_t const* src)
{
  assert(src != NULL);

  srb_setup_item_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_SRBs_Setup_Item);	
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_SRBs_Setup_ItemIEs__value_PR_SRBs_Setup_Item);

  // Mandatory
  // SRB ID 9.3.1.7
  assert(src->value.choice.SRBs_Setup_Item.sRBID < 4); // [0,3]
  dst.srb_id = src->value.choice.SRBs_Setup_Item.sRBID ;

  // Mandatory
  // LCID 9.3.1.35
  assert(src->value.choice.SRBs_Setup_Item.lCID > 0 && src->value.choice.SRBs_Setup_Item.lCID < 33); // [1,32]

  dst.lc_id = src->value.choice.SRBs_Setup_Item.lCID; 

  return dst;
}


static
srb_setup_item_arr_t cp_srb_setup_item_stp_resp(F1AP_UEContextSetupResponseIEs_t const* src_ie)
{
  assert(src_ie != NULL);

  srb_setup_item_arr_t dst = {0}; 

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_SRBs_Setup_List);	
  assert(src_ie->criticality == F1AP_Criticality_ignore);
  assert(src_ie->value.present == F1AP_UEContextSetupResponseIEs__value_PR_SRBs_Setup_List);

  F1AP_SRBs_Setup_List_t const* src = &src_ie->value.choice.SRBs_Setup_List;
 
  dst.sz = src->list.count; 
  if(src->list.count > 0){
    dst.srb_setup_item = calloc(dst.sz, sizeof( srb_setup_item_t));
    assert(dst.srb_setup_item != NULL && "Memory exhasuted");
  }

  for(size_t i = 0; i < dst.sz; ++i){
    dst.srb_setup_item[i] = cp_srb_setup_item_ie_stp_resp((F1AP_SRBs_Setup_ItemIEs_t*)  src->list.array[i]);
  }

  return dst;
}


ue_ctx_setup_response_t cp_ue_ctx_setup_response_ir(F1AP_F1AP_PDU_t const* src_pdu)
{
  assert(src_pdu != NULL);

  ue_ctx_setup_response_t dst = {0};

  assert(src_pdu->present == F1AP_F1AP_PDU_PR_successfulOutcome);

  F1AP_SuccessfulOutcome_t const* src_out = src_pdu->choice.successfulOutcome;

  assert(src_out->procedureCode == F1AP_ProcedureCode_id_UEContextSetup);
  assert(src_out->criticality == F1AP_Criticality_reject);
  assert(src_out->value.present == F1AP_SuccessfulOutcome__value_PR_UEContextSetupResponse);
  F1AP_UEContextSetupResponse_t const* src = &src_out->value.choice.UEContextSetupResponse;


  assert(src->protocolIEs.list.size > 2 && "At least three IE are mandatory");
  
  for(size_t i = 0; i < src->protocolIEs.list.count; ++i){
    F1AP_UEContextSetupResponseIEs_t const* src_ie = src->protocolIEs.list.array[i]; 

    // Mandatory
    //gNB-CU UE F1AP ID 9.3.1.4
    if(src_ie->id == F1AP_ProtocolIE_ID_id_gNB_CU_UE_F1AP_ID){
      dst.gnb_cu_ue_id = cp_gnb_cu_ue_id_setup_resp(src_ie);

    // Mandatory
    // gNB-DU UE F1AP ID 9.3.1.5
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_gNB_DU_UE_F1AP_ID){
      dst.gnb_du_ue_id = cp_gnb_du_ue_id_setup_resp(src_ie);

    // Mandatory 
    // DU To CU RRC Information 9.3.1.26
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_DUtoCURRCInformation){
      dst.du_to_cu_rrc_info = cp_du_to_cu_rrc_info_setup_resp(src_ie);  

    // Optional
    // C-RNTI 9.3.1.32
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_C_RNTI){
      dst.c_rnti = cp_c_rnti_setup_resp(src_ie);

    // Optional
    // Resource Coordination Transfer Container 
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_ResourceCoordinationTransferContainer) {
       dst.res_coord_trans_cont = cp_res_coord_trans_cont_setup_resp(src_ie);

    // DRB Setup Item List [0, 64]
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_DRBs_Setup_List){
      drb_setup_item_arr_t tmp = cp_drb_setup_item_setup_resp(src_ie);    
      dst.sz_drb_setup_item = tmp.sz; 
      dst.drb_setup_item = tmp.drb_setup_item; 

    // SRB Setup List [0, 8]
    } else if(src_ie->id == F1AP_ProtocolIE_ID_id_SRBs_Setup_List){
        srb_setup_item_arr_t tmp = cp_srb_setup_item_stp_resp(src_ie);
        dst.sz_srb_setup_item = tmp.sz;
        dst.srb_setup_item = tmp.srb_setup_item;
    } else {
      assert(0 != 0 && "Not implemented IEs");
    } 
  }

  return dst;
}

/////////////////////////
/////////////////////////
/////////////////////////

static
uint8_t cp_trans_id_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  assert(src->id == F1AP_ProtocolIE_ID_id_TransactionID);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_TransactionID);
 
  assert(src->value.choice.TransactionID < 256);

  return src->value.choice.TransactionID; 
}

static
activate_cell_t cp_act_cell_cu_conf_up_ir(F1AP_Cells_to_be_Activated_List_ItemIEs_t const* src)
{
  assert(src != NULL);

  activate_cell_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List_Item);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_Cells_to_be_Activated_List_ItemIEs__value_PR_Cells_to_be_Activated_List_Item);

  F1AP_Cells_to_be_Activated_List_Item_t const* src_it = &src->value.choice.Cells_to_be_Activated_List_Item;

  // Mandatory
  // NR CGI 
  TBCD_TO_MCC_MNC(&src_it->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_it->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1ul << 36) );

  // Optional
  // NR PCI
  if(src_it->nRPCI != NULL){
    dst.nr_pci = malloc(sizeof(uint16_t));
    assert(dst.nr_pci != NULL);
    assert(*src_it->nRPCI < ( 1UL << 16));
    *dst.nr_pci = *src_it->nRPCI;
  }

  if(src_it->iE_Extensions == NULL)
    return dst;

  for(size_t i = 0; i < ((F1AP_ProtocolExtensionContainer_154P46_t*)src_it->iE_Extensions)->list.count; ++i){
    F1AP_Cells_to_be_Activated_List_ItemExtIEs_t const* ie = ((F1AP_ProtocolExtensionContainer_154P46_t*)src_it->iE_Extensions)->list.array[i];  

    // gNB-CU System Information
    if(ie->id == F1AP_ProtocolIE_ID_id_gNB_CUSystemInformation){
      dst.sys_info = calloc(1, sizeof(gnb_cu_sys_info_t)); 
      assert(dst.sys_info != NULL && "Memory exhausted");
     
      *dst.sys_info = cp_gnb_cu_system_info_ir(((F1AP_ProtocolExtensionContainer_154P46_t*)src_it->iE_Extensions));
    } else {
      // Available PLMN List. optional
      // Extended Available PLMN List. optional
      // IAB Info IAB-donor-CU. optional
      // Available SNPN ID List. optional
      assert(0 != 0 && "Not implemented or Unknown ID");
    }
  } 

  return dst;
}

typedef struct{
  size_t sz;
  activate_cell_t* cell;
} span_cell_act_cu_conf_up_t;

static
span_cell_act_cu_conf_up_t cp_cell_act_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  span_cell_act_cu_conf_up_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Activated_List);

  assert(src->value.choice.Cells_to_be_Activated_List.list.count > 0);
  assert(src->value.choice.Cells_to_be_Activated_List.list.count < 513);
  dst.sz = src->value.choice.Cells_to_be_Activated_List.list.count;

  dst.cell = calloc(dst.sz, sizeof(activate_cell_t));
  assert(dst.cell != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
   dst.cell[i] = cp_act_cell_cu_conf_up_ir((F1AP_Cells_to_be_Activated_List_ItemIEs_t*)src->value.choice.Cells_to_be_Activated_List.list.array[i]); 
  }

  return dst;
}

static
cells_to_be_deact_t cp_cell_deact_it_cu_conf_up_ir(F1AP_Cells_to_be_Deactivated_List_ItemIEs_t const* src)
{
  assert(src != NULL);

  cells_to_be_deact_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Deactivated_List_Item); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_Cells_to_be_Deactivated_List_ItemIEs__value_PR_Cells_to_be_Deactivated_List_Item);

  // mandatory
  // NR CGI
  TBCD_TO_MCC_MNC(&src->value.choice.Cells_to_be_Deactivated_List_Item.nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src->value.choice.Cells_to_be_Deactivated_List_Item.nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1ul << 36) );

  return dst;
}

typedef struct{
  size_t sz;
  cells_to_be_deact_t* cell; 
} span_cell_deact_cu_conf_up_t ;

span_cell_deact_cu_conf_up_t cp_cell_deact_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  span_cell_deact_cu_conf_up_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Deactivated_List); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Deactivated_List);

  F1AP_Cells_to_be_Deactivated_List_t	const* src_cell = &src->value.choice.Cells_to_be_Deactivated_List;
  assert(src_cell->list.count > 0);
  assert(src_cell->list.count < 513);

  dst.sz = src_cell->list.count; 
  dst.cell = calloc(dst.sz, sizeof(cells_to_be_deact_t));
  assert(dst.cell != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    F1AP_Cells_to_be_Deactivated_List_ItemIEs_t const* ie = (F1AP_Cells_to_be_Deactivated_List_ItemIEs_t*)src_cell->list.array[i];
    dst.cell[i] = cp_cell_deact_it_cu_conf_up_ir(ie); 
  }

  return dst;
}

static
endpoint_ip_addr_port_t cp_ip_addr_port_ir(F1AP_Endpoint_IP_address_and_port_t* const src)
{
  assert(src != NULL);

  endpoint_ip_addr_port_t dst = {0}; 

  dst.endpoint_ip_addr.trans_layer_add = copy_bit_string(&src->endpointIPAddress);

  assert(src->iE_Extensions != NULL && "Port needed" );

  assert(((F1AP_ProtocolExtensionContainer_154P93_t*)src->iE_Extensions)->list.count == 1);

  F1AP_Endpoint_IP_address_and_port_ExtIEs_t const* src_ie = ((F1AP_ProtocolExtensionContainer_154P93_t*)src->iE_Extensions)->list.array[0];

  assert(src_ie->id == F1AP_ProtocolIE_ID_id_portNumber); 
  assert(src_ie->criticality == F1AP_Criticality_reject); 
  assert(src_ie->extensionValue.present == F1AP_Endpoint_IP_address_and_port_ExtIEs__extensionValue_PR_PortNumber); 

  dst.port = copy_bit_string(&src_ie->extensionValue.choice.PortNumber);

  return dst;
}

static
cp_trans_layer_info_t cp_tnl_assoc_tran_layer_addr_ir(F1AP_CP_TransportLayerAddress_t	const* src)
{
  assert(src != NULL);

  cp_trans_layer_info_t dst = {0}; 

  if(src->present ==  F1AP_CP_TransportLayerAddress_PR_endpoint_IP_address) {
    dst.type = IP_ADDRESS_CP_TRANS_LAYER_INFO; 
    dst.ep_ip_addr.trans_layer_add = copy_bit_string(&src->choice.endpoint_IP_address); 
  } else if (src->present == F1AP_CP_TransportLayerAddress_PR_endpoint_IP_address_and_port){
      dst.type = IP_ADDRESS_PORT_CP_TRANS_LAYER_INFO;
      dst.ep_ip_addr_port = cp_ip_addr_port_ir(src->choice.endpoint_IP_address_and_port);
  } else {
    assert(0!=0 && "Unknown type");
  } 

  return dst;
}

static
tnl_assoc_usage_e cp_tnl_usage(F1AP_TNLAssociationUsage_t	src) 
{
  tnl_assoc_usage_e dst = {0}; 

  if(src == F1AP_TNLAssociationUsage_ue	 ){
    dst = UE_TNL_ASSOC_USAGE; 
  } else if(src == F1AP_TNLAssociationUsage_non_ue){
    dst = NON_UE_TNL_ASSOC_USAGE ; 
  } else if (src == F1AP_TNLAssociationUsage_both){
    dst = BOTH_TNL_ASSOC_USAGE; 
  } else {
    assert(0!=0 && "Unknown type" );
  }

  return dst;
}

static
gnb_cu_tnl_asso_to_add_t cp_tnl_assoc_add_it_cu_conf_up_ir(F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t const* src)
{
  assert(src != NULL);

  gnb_cu_tnl_asso_to_add_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Add_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present ==  F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Add_Item);

 F1AP_GNB_CU_TNL_Association_To_Add_Item_t const* src_ie = &src->value.choice.GNB_CU_TNL_Association_To_Add_Item;
  
  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst.cp_trans_layer_info = cp_tnl_assoc_tran_layer_addr_ir(&src_ie->tNLAssociationTransportLayerAddress); 

  // TNL Association Usage
  //  Mandatory
  dst.tnl_assoc_usage = cp_tnl_usage(src_ie->tNLAssociationUsage) ;

  return dst;
}


typedef struct{
  size_t sz;
  gnb_cu_tnl_asso_to_add_t* tnl; 
} span_tnl_assoc_add_t; 

static
span_tnl_assoc_add_t cp_tnl_assoc_add_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);
  span_tnl_assoc_add_t dst = {0};

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Add_List); 
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Add_List);

 F1AP_GNB_CU_TNL_Association_To_Add_List_t const* src_tnl = &src->value.choice.GNB_CU_TNL_Association_To_Add_List;

  assert(src_tnl->list.count > 0);
  assert(src_tnl->list.count < 33);
  dst.sz =src_tnl->list.count; 

  dst.tnl = calloc(dst.sz, sizeof(gnb_cu_tnl_asso_to_add_t));
  assert(dst.tnl != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.tnl[i] = cp_tnl_assoc_add_it_cu_conf_up_ir((F1AP_GNB_CU_TNL_Association_To_Add_ItemIEs_t*)src_tnl->list.array[i]);
  }

  return dst;
}

static
gnb_cu_tnl_assoc_to_rem_t cp_tnl_assoc_rm_it_cu_conf_up_ir( F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t const* src)
{
  assert(src != NULL);

  gnb_cu_tnl_assoc_to_rem_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Remove_Item);


 F1AP_GNB_CU_TNL_Association_To_Remove_Item_t	const* src_it = &src->value.choice.GNB_CU_TNL_Association_To_Remove_Item;

  // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst.tnl_assoc_trans = cp_tnl_assoc_tran_layer_addr_ir(&src_it->tNLAssociationTransportLayerAddress);

  // TNL Association Transport Layer Address gNB-DU
  // Optional
  if(src_it->iE_Extensions != NULL){

  assert(((struct F1AP_ProtocolExtensionContainer_154P116*)src_it->iE_Extensions)->list.count == 1);

  F1AP_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs_t const* src_ie =  ((struct F1AP_ProtocolExtensionContainer_154P116*)src_it->iE_Extensions)->list.array[0];


  assert(src_ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_Item);
   assert(src_ie->criticality == F1AP_Criticality_reject);
   assert(src_ie->extensionValue.present == F1AP_GNB_CU_TNL_Association_To_Remove_Item_ExtIEs__extensionValue_PR_CP_TransportLayerAddress);

   dst.tnl_assoc_trans_du = calloc(1, sizeof(cp_trans_layer_info_t));
   assert(dst.tnl_assoc_trans_du != NULL && "Memory exhausted");

    *dst.tnl_assoc_trans_du =  cp_tnl_assoc_tran_layer_addr_ir(&src_ie->extensionValue.choice.CP_TransportLayerAddress);
  } 

  return dst;
}

typedef struct{
  size_t sz;
  gnb_cu_tnl_assoc_to_rem_t* tnl; 
} span_tnl_assoc_rm_t; 

static
span_tnl_assoc_rm_t cp_tnl_assoc_rm_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  span_tnl_assoc_rm_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_List); 
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Remove_List);

 F1AP_GNB_CU_TNL_Association_To_Remove_List_t const* src_lst = &src->value.choice.GNB_CU_TNL_Association_To_Remove_List;

 assert(src_lst->list.count > 0);
 assert(src_lst->list.count < 33);

 dst.sz = src_lst->list.count;
 dst.tnl = calloc(dst.sz, sizeof(gnb_cu_tnl_assoc_to_rem_t));
 assert(dst.tnl != NULL && "Memory exhausted");

 for(size_t i = 0; i < dst.sz; ++i){
    dst.tnl[i] =  cp_tnl_assoc_rm_it_cu_conf_up_ir((F1AP_GNB_CU_TNL_Association_To_Remove_ItemIEs_t*)src_lst->list.array[i]);
 }

  return dst;
}

static
gnb_cu_tnl_assoc_to_upd_t cp_tnl_assoc_to_upd_it_cu_conf_up_ir(F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t const* src)
{
  assert(src != NULL);

  gnb_cu_tnl_assoc_to_upd_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Update_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs__value_PR_GNB_CU_TNL_Association_To_Update_Item);

 F1AP_GNB_CU_TNL_Association_To_Update_Item_t const* src_it = &src->value.choice.GNB_CU_TNL_Association_To_Update_Item;

 // TNL Association Transport Layer Address 9.3.2.4
  // Mandatory
  dst.tnl_assoc_trans_addr = cp_tnl_assoc_tran_layer_addr_ir(&src_it->tNLAssociationTransportLayerAddress);

  // TNL Association Usage
  // Optional
  if(src_it->tNLAssociationUsage != NULL){
    dst.tnl_association_usage = calloc(1, sizeof(tnl_assoc_usage_e));
    assert(dst.tnl_association_usage != NULL && "Memory exhausted");

    *dst.tnl_association_usage = cp_tnl_usage(*src_it->tNLAssociationUsage) ; 
  }

  return dst;
}

typedef struct{
  size_t sz;
  gnb_cu_tnl_assoc_to_upd_t* tnl; 
} span_tnl_assoc_upd_t;

span_tnl_assoc_upd_t cp_tnl_assoc_upd_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src) 
{
  assert(src != NULL);

  span_tnl_assoc_upd_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Update_List); 
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_GNB_CU_TNL_Association_To_Update_List);

  F1AP_GNB_CU_TNL_Association_To_Update_List_t	const* src_lst = &src->value.choice.GNB_CU_TNL_Association_To_Update_List;

  assert(src_lst->list.count > 0);
  assert(src_lst->list.count < 33);

  dst.sz = src_lst->list.count; 
  dst.tnl = calloc(dst.sz, sizeof(gnb_cu_tnl_assoc_to_upd_t));
  assert(dst.tnl != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.tnl[i] = cp_tnl_assoc_to_upd_it_cu_conf_up_ir((F1AP_GNB_CU_TNL_Association_To_Update_ItemIEs_t*)src_lst->list.array[i]);
  }

  return dst;
}

static
cell_barred_e cp_cell_barred_ir(F1AP_CellBarred_t	src)
{
	cell_barred_e dst = {0}; 

  if(src == F1AP_CellBarred_barred){
    dst = BARRED_CELL_BARRED;
  } else if(src == F1AP_CellBarred_not_barred){
    dst = NOT_BARRED_CELL_BARRED;
  } else {
    assert( 0!= 0 && "Unknown type" );
  }
  
  return dst;
}

static
cells_to_be_barred_t cp_cell_to_be_barred_it_cu_conf_up(F1AP_Cells_to_be_Barred_ItemIEs_t const* src)
{
  assert(src != NULL);

  cells_to_be_barred_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Barred_Item);
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_Cells_to_be_Barred_ItemIEs__value_PR_Cells_to_be_Barred_Item);


 F1AP_Cells_to_be_Barred_Item_t	const* src_it = &src->value.choice.Cells_to_be_Barred_Item;

  // NR CGI 9.3.1.12
  // Mandatory
  TBCD_TO_MCC_MNC(&src_it->nRCGI.pLMN_Identity, dst.nr_cgi.plmn_id.mcc, dst.nr_cgi.plmn_id.mnc, dst.nr_cgi.plmn_id.mnc_digit_len);
  BIT_STRING_TO_NR_CELL_IDENTITY(&src_it->nRCGI.nRCellIdentity, dst.nr_cgi.nr_cell_id);
  assert(dst.nr_cgi.nr_cell_id < (1ul << 36) );

  // Cell Barred 
  // Mandatory
  dst.cell_barred = cp_cell_barred_ir(src_it->cellBarred);

  // IAB Barred 
  // Optional
  if(src_it->iE_Extensions != NULL){

    assert(((F1AP_ProtocolExtensionContainer_154P48_t*)src_it->iE_Extensions)->list.count == 1);

    F1AP_Cells_to_be_Barred_Item_ExtIEs_t const* src_ie = ((F1AP_ProtocolExtensionContainer_154P48_t*)src_it->iE_Extensions)->list.array[0];

    assert(src_ie->id == F1AP_ProtocolIE_ID_id_IAB_Barred);
    assert(src_ie->criticality == F1AP_Criticality_ignore);
    assert(src_ie->extensionValue.present == F1AP_Cells_to_be_Barred_Item_ExtIEs__extensionValue_PR_IAB_Barred);

    dst.iab_barred = calloc(1, sizeof(cell_barred_e) );
    assert(dst.iab_barred != NULL && "Memory exhausted"); 

    *dst.iab_barred = cp_cell_barred_ir(src_ie->extensionValue.choice.IAB_Barred); 
  }

  return dst;
}

typedef struct{
  size_t sz;
  cells_to_be_barred_t* cells;
} span_cells_to_be_barred_t; 

static
span_cells_to_be_barred_t cp_cells_to_be_barred_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  span_cells_to_be_barred_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Barred_List); 
  assert(src->criticality == F1AP_Criticality_ignore);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_Cells_to_be_Barred_List);

  F1AP_Cells_to_be_Barred_List_t	const* src_lst = &src->value.choice.Cells_to_be_Barred_List; 

  assert(src_lst->list.count > 0);
  assert(src_lst->list.count < 513);

  dst.sz = src_lst->list.count;
  dst.cells = calloc(dst.sz, sizeof(cells_to_be_barred_t));
  assert(dst.cells != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.cells[i] = cp_cell_to_be_barred_it_cu_conf_up((F1AP_Cells_to_be_Barred_ItemIEs_t*)src_lst->list.array[i]); 
  } 

  return dst;
}

static
fdd_info_serv_eutra_cell_info_t cp_eutra_cell_info_fdd(F1AP_EUTRA_FDD_Info_t const* src)
{
  assert(src != NULL);

  fdd_info_serv_eutra_cell_info_t dst = {0};

// [0,2199]
  assert(src->uL_offsetToPointA < 2200);
  dst.ul_offset_to_point_a = src->uL_offsetToPointA;

  // [0,2199]
  assert(src->dL_offsetToPointA < 2200);
  dst.dl_offset_to_point_a = src->dL_offsetToPointA;

  return dst;
}

static
tdd_info_serv_eutra_cell_info_t cp_eutra_cell_info_tdd(F1AP_EUTRA_TDD_Info_t const* src)
{
  assert(src != NULL);

  tdd_info_serv_eutra_cell_info_t dst = {0}; 

  // [0,2199]
  assert(src->offsetToPointA < 2200);
  dst.offset_to_point_a = src->offsetToPointA; 

  return dst;
}

static
serv_eutra_cell_info_t cp_serv_eutra_cell_info_ir(F1AP_Served_EUTRA_Cells_Information_t const* src)
{
  assert(src != NULL);

  serv_eutra_cell_info_t dst = {0}; 

  if(src->eUTRA_Mode_Info.present == F1AP_EUTRA_Mode_Info_PR_eUTRAFDD){
    dst.type = FDD_INFO_SERV_EUTRA_CELL_INFO;
    dst.fdd = cp_eutra_cell_info_fdd(src->eUTRA_Mode_Info.choice.eUTRAFDD);
  } else if(src->eUTRA_Mode_Info.present == F1AP_EUTRA_Mode_Info_PR_eUTRATDD){
    dst.type = TDD_INFO_SERV_EUTRA_CELL_INFO;
    dst.tdd = cp_eutra_cell_info_tdd(src->eUTRA_Mode_Info.choice.eUTRATDD);
  } else {
    assert(0 !=0 && "Unknown type" );
  }

  return dst;
}

static
eutra_cell_t cp_eutra_cell_it_cu_conf_up(F1AP_EUTRACells_List_item_t const* src)
{
  assert(src != NULL);

  eutra_cell_t dst = {0}; 

  // EUTRA Cell ID
  // Mandatory // [28]
  assert(src->eUTRA_Cell_ID.size == 4 && src->eUTRA_Cell_ID.bits_unused == 4);
  dst.eutra_cell_id = copy_bit_string(&src->eUTRA_Cell_ID); 

  // Served E-UTRA CellInformation 9.3.1.64
  // Mandatory
 
  dst.serv_eutra_cell_info = cp_serv_eutra_cell_info_ir(&src->served_EUTRA_Cells_Information); 


  return dst;
}

static
prot_eutra_resources_t cp_prot_eutra_res_it_cu_conf_up_ir(F1AP_Protected_EUTRA_Resources_ItemIEs_t const* src)
{
  assert(src != NULL);

  prot_eutra_resources_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Protected_EUTRA_Resources_Item);
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_Protected_EUTRA_Resources_ItemIEs__value_PR_Protected_EUTRA_Resources_Item);

 F1AP_Protected_EUTRA_Resources_Item_t const* src_it = &src->value.choice.Protected_EUTRA_Resources_Item;
  // Spectrum sharing group [1, 256]
  assert(src_it->spectrumSharingGroupID > 0);
  assert(src_it->spectrumSharingGroupID < 257);
  dst.spec_sharing_group_id = src_it->spectrumSharingGroupID;

  // EUTRA Cell                                
  assert(src_it->eUTRACells_List.list.count > 0); 
  assert(src_it->eUTRACells_List.list.count < 257); 

  dst.sz_eutra_cell = src_it->eUTRACells_List.list.count; 
  dst.eutra_cell = calloc( dst.sz_eutra_cell, sizeof(eutra_cell_t) );
  assert(dst.eutra_cell != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz_eutra_cell; ++i){
    dst.eutra_cell[i] = cp_eutra_cell_it_cu_conf_up(src_it->eUTRACells_List.list.array[i]); 
  }

  return dst;
}


typedef struct{
  size_t sz;
  prot_eutra_resources_t* eutra;
} span_prot_eutra_res_lst_t;

static
span_prot_eutra_res_lst_t cp_prot_eutra_res_cu_conf_up_ir(F1AP_GNBCUConfigurationUpdateIEs_t const* src)
{
  assert(src != NULL);

  span_prot_eutra_res_lst_t dst = {0}; 

  assert(src->id == F1AP_ProtocolIE_ID_id_Protected_EUTRA_Resources_List); 
  assert(src->criticality == F1AP_Criticality_reject);
  assert(src->value.present == F1AP_GNBCUConfigurationUpdateIEs__value_PR_Protected_EUTRA_Resources_List);

 F1AP_Protected_EUTRA_Resources_List_t const* src_lst = &src->value.choice.Protected_EUTRA_Resources_List;

  assert(src_lst->list.count > 0);
  assert(src_lst->list.count < 257);

  dst.sz = src_lst->list.count;
  dst.eutra = calloc(dst.sz, sizeof(prot_eutra_resources_t));
  assert(dst.eutra != NULL && "Memory exhausted");

  for(size_t i = 0; i < dst.sz; ++i){
    dst.eutra[i] = cp_prot_eutra_res_it_cu_conf_up_ir((F1AP_Protected_EUTRA_Resources_ItemIEs_t*)src_lst->list.array[i]);
  }

  return dst;
}

gnb_cu_conf_update_t cp_gnb_cu_conf_update_ir(F1AP_F1AP_PDU_t const* src_pdu)
{
  assert(src_pdu != NULL);

  gnb_cu_conf_update_t dst = {0};

  assert(src_pdu->present == F1AP_F1AP_PDU_PR_initiatingMessage);

  F1AP_InitiatingMessage_t const* src_out = src_pdu->choice.initiatingMessage;

  assert(src_out->procedureCode == F1AP_ProcedureCode_id_gNBCUConfigurationUpdate);
  assert(src_out->criticality == F1AP_Criticality_reject);
  assert(src_out->value.present == F1AP_InitiatingMessage__value_PR_GNBCUConfigurationUpdate);

  F1AP_GNBCUConfigurationUpdate_t const* src = & src_out->value.choice.GNBCUConfigurationUpdate;

  assert(src->protocolIEs.list.count > 1 && "At least 2 are needed");
  for(size_t i = 0; i < src->protocolIEs.list.count; ++i){
    F1AP_GNBCUConfigurationUpdateIEs_t const* ie = src->protocolIEs.list.array[i]; 
    // Mandatory
    // Transaction ID 9.3.1.23
    if(ie->id == F1AP_ProtocolIE_ID_id_TransactionID){
      dst.trans_id = cp_trans_id_cu_conf_up_ir(ie);

      // Cells to be Activated List [0,512]
    } else if(ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Activated_List){
      span_cell_act_cu_conf_up_t tmp = cp_cell_act_cu_conf_up_ir(ie);
      dst.sz_cells_to_be_act = tmp.sz;
      dst.cells_to_be_act = tmp.cell;

      // Cells to be Deactivated List [0,512]
    }else if(ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Deactivated_List){
      span_cell_deact_cu_conf_up_t tmp = cp_cell_deact_cu_conf_up_ir(ie);
      dst.sz_cells_to_be_deact = tmp.sz;
      dst.cells_to_be_deact = tmp.cell;

      // TNL Association To Add List
    }else if(ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Add_List){
      span_tnl_assoc_add_t tmp = cp_tnl_assoc_add_cu_conf_up_ir(ie);
      dst.sz_gnb_cu_tnl_asso_to_add = tmp.sz;
      dst.gnb_cu_tnl_asso_to_add = tmp.tnl;

      // TNL Association To Remove List // [0,32] 
    }else if(ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Remove_List ){
      span_tnl_assoc_rm_t tmp = cp_tnl_assoc_rm_cu_conf_up_ir(ie);
      dst.sz_gnb_cu_tnl_assoc_to_rem = tmp.sz;
      dst.gnb_cu_tnl_assoc_to_rem = tmp.tnl;

      // TNL Association To Update List // [0,32] 
    } else if(ie->id == F1AP_ProtocolIE_ID_id_GNB_CU_TNL_Association_To_Update_List){
      span_tnl_assoc_upd_t tmp = cp_tnl_assoc_upd_cu_conf_up_ir(ie);
      dst.sz_gnb_cu_tnl_assoc_to_upd = tmp.sz;
      dst.gnb_cu_tnl_assoc_to_upd = tmp.tnl;

      // Cells to be Barred // [0, 512] 
    } else if(ie->id == F1AP_ProtocolIE_ID_id_Cells_to_be_Barred_List){
      span_cells_to_be_barred_t tmp = cp_cells_to_be_barred_cu_conf_up_ir(ie);
      dst.sz_cells_to_be_barred = tmp.sz;
      dst.cells_to_be_barred = tmp.cells;

      // Protected EUTRA Resources [0, 257] 
    } else if(ie->id == F1AP_ProtocolIE_ID_id_Protected_EUTRA_Resources_List){
      span_prot_eutra_res_lst_t tmp = cp_prot_eutra_res_cu_conf_up_ir(ie);
      dst.sz_prot_eutra_resources = tmp.sz;
      dst.prot_eutra_resources = tmp.eutra;

    } else {
      assert(0!=0 && "Not implemented or Unknown ie ID");
    }
  }
  return dst;
}

