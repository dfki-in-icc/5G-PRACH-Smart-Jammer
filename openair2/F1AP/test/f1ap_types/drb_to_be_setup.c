#include "drb_to_be_setup.h"

#include <assert.h>
#include <stdlib.h>

void free_drb_to_be_setup(drb_to_be_setup_t* src)
{
  assert(src != NULL);

  // Mandatory
  // DRB ID 9.3.1.8
  assert(src->drb_id < 33); // INTEGER [0,32]

  assert(src->qos_info < END_QOS_INFORMATION);

  if(src->qos_info == E_UTRAN_QOS_INFORMATION){
    free_e_utran_qos(&src->e_utran_qos);
  } else if (src->qos_info == DRB_INFO_QOS_INFORMATION) {
    free_drb_info(&src->drb_info);
  } else {
    assert(0!=0 && "Unknown type");
  }

  // UL UP TNL Information to be setup List
  assert(src->sz_ul_up_tnl_info_lst > 0 && src->sz_ul_up_tnl_info_lst < 3); // [1, 2]
  for(size_t i = 0; i < src->sz_ul_up_tnl_info_lst; ++i){
    free_ul_up_tnl_info_lst(&src->ul_up_tnl_info[i]);
  }
  if(src->ul_up_tnl_info != NULL)
    free(src->ul_up_tnl_info);

  // Mandatory
  // RLC Mode 9.3.1.27
  // rlc_mode_f1ap_e rlc_mode;

  // Optional 
  // UL Configuration 9.3.1.31
  if(src->ul_conf != NULL)
    free(src->ul_conf);

  // Optional 
  // Duplication Activation 9.3.1.36
  if(src->dup_act != NULL)
    free(src->dup_act);
//  duplication_act_f1ap_e* dup_act;

  // Optional
  // DC Based Duplication Configured
  if(src->dc_dup_act_conf != NULL )
    free(src-> dc_dup_act_conf);
//  bool* dc_dup_act_conf;

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  if(src->dc_dup_act != NULL)
    free(src->dc_dup_act);
//  duplication_act_f1ap_e* dc_dup_act;

  // Mandatory
  // DL PDCP SN length
//  pdcp_sn_len_e dl_pdcp_sn_len;

  // Optional
  // UL PDCP SN length
  if(src->ul_pdcp_sn_len != NULL)
    free(src->ul_pdcp_sn_len);
//  pdcp_sn_len_e* ul_pdcp_sn_len;

  // Additional PDCP Duplication TNL List
  for(size_t i = 0; src->sz_add_pdcp_dup_tnl ; ++i){
    free_add_pdcp_dup_tnl(&src->add_pdcp_dup_tnl[i]);
  }
  if(src->add_pdcp_dup_tnl != NULL){
    free(src->add_pdcp_dup_tnl);
  }

  // Optional
  // RLC Duplication Information 9.3.1.146
  assert(src->sz_rlc_dup_info == 0 && "Not implemented ");
  assert(src-> rlc_dup_info == NULL && "Not implemented");

}

bool eq_drb_to_be_setup(drb_to_be_setup_t const* m0, drb_to_be_setup_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // DRB ID 9.3.1.8
  assert(m0->drb_id < 33); // INTEGER [0,32]
  assert(m1->drb_id < 33); // INTEGER [0,32]
  if(m0->drb_id != m1->drb_id)
    return false;

  assert(m0->qos_info < END_QOS_INFORMATION);
  assert(m1->qos_info < END_QOS_INFORMATION);
  if(m0->qos_info != m1->qos_info)
    return false;

  if(m0->qos_info == E_UTRAN_QOS_INFORMATION){
    if(eq_e_utran_qos(&m0->e_utran_qos, &m1->e_utran_qos) == false)
      return false;
  } else if (m0->qos_info == DRB_INFO_QOS_INFORMATION) {

    if(eq_drb_info(&m0->drb_info, &m1->drb_info) == false)
      return false;

  } else {
    assert(0!=0 && "Unknown type");
  }

  // UL UP TNL Information to be setup List
  if(m0->sz_ul_up_tnl_info_lst != m1->sz_ul_up_tnl_info_lst)
    return false;

  for(size_t i = 0; i < m0->sz_ul_up_tnl_info_lst; ++i){
      if(eq_ul_up_tnl_info_lst(&m0->ul_up_tnl_info[i], &m1->ul_up_tnl_info[i] ) == false){
        assert(0!=0);
        return false;
      }
  }

  // Mandatory
  // RLC Mode 9.3.1.27
  if(m0->rlc_mode != m1->rlc_mode)
    return false;

  // Optional 
  // UL Configuration 9.3.1.31
  if(m0->ul_conf != m1->ul_conf){
    if(m0->ul_conf == NULL || m1->ul_conf == NULL)
      return false;

    if(*m0->ul_conf != *m1->ul_conf)
      return false;
  }

  // Optional 
  // Duplication Activation 9.3.1.36
  // duplication_act_f1ap_e* dup_act;
  if(m0->dup_act != m1->dup_act ){
    if(m0->dup_act == NULL || m1->dup_act == NULL)
      return false;

    if(*m0->dup_act != *m1->dup_act )
      return false;
  }

  // Optional
  // DC Based Duplication Configured
  // bool* dc_dup_act_conf;
  if(m0->dc_dup_act_conf != m1->dc_dup_act_conf ){
    if(m0->dc_dup_act_conf == NULL || m1->dc_dup_act_conf == NULL)
      return false;

    if(*m0->dc_dup_act_conf != *m1->dc_dup_act_conf )
      return false;
  }

  // Optional
  // DC Based Duplication Activation 9.3.1.36
  //  duplication_act_f1ap_e* dc_dup_act;
  if(m0->dc_dup_act != m1->dc_dup_act ){
    if(m0->dc_dup_act == NULL || m1->dc_dup_act == NULL)
      return false;

    if(*m0->dc_dup_act != *m1->dc_dup_act )
      return false;
  }

  // Mandatory
  // DL PDCP SN length
  // pdcp_sn_len_e dl_pdcp_sn_len;
  if(m0->dl_pdcp_sn_len != m1->dl_pdcp_sn_len)
    return false;

  // Optional
  // UL PDCP SN length
  //  pdcp_sn_len_e* ul_pdcp_sn_len;
  if(m0->ul_pdcp_sn_len != m1->ul_pdcp_sn_len ){
    if(m0->ul_pdcp_sn_len == NULL || m1->ul_pdcp_sn_len == NULL)
      return false;

    if(*m0->ul_pdcp_sn_len != *m1->ul_pdcp_sn_len )
      return false;
  }

  // Additional PDCP Duplication TNL List
  if(m0->sz_add_pdcp_dup_tnl != m1->sz_add_pdcp_dup_tnl)
    return false;

  for(size_t i = 0; i < m0->sz_add_pdcp_dup_tnl; ++i){
    if(eq_add_pdcp_dup_tnl(&m0->add_pdcp_dup_tnl[i], &m1->add_pdcp_dup_tnl[i]) == false)
      return false;
  }

  // Optional
  // RLC Duplication Information 9.3.1.146
  assert(m0->sz_rlc_dup_info == 0 && "Not implemented ");
  assert(m0->rlc_dup_info == NULL && "Not implemented");

  assert(m1->sz_rlc_dup_info == 0 && "Not implemented ");
  assert(m1->rlc_dup_info == NULL && "Not implemented");

  return true;
}

