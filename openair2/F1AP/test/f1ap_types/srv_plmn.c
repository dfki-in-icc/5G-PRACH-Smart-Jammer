#include "srv_plmn.h"

#include <assert.h>

void free_srv_plmn_f1ap( srv_plmn_f1ap_t* src)
{
  assert(src != NULL);

  for(size_t i =0; i < src->sz_tai_slice_sup_lst; ++i ){
    free_s_nssai(&src->tai_slice_sup_lst[i]);  
  }

  free(src->tai_slice_sup_lst);

  free(src->npn_sup_info_nid);

  for(size_t i = 0; i < src->sz_ext_slice_sup_lst; ++i){
    free_s_nssai(&src->ext_tai_slice_sup_lst[i]);
  }
  free(src->ext_tai_slice_sup_lst);
}

bool eq_srv_plmn_f1ap( srv_plmn_f1ap_t const* m0, srv_plmn_f1ap_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(eq_plmn_f1ap(&m0->plmn_id, &m1->plmn_id) == false )
    return false;

  if(m0->sz_tai_slice_sup_lst != m1->sz_tai_slice_sup_lst){
    return false;
  }
  for(size_t i =0 ; i < m0->sz_tai_slice_sup_lst; ++i){
    if(eq_s_nssai(&m0->tai_slice_sup_lst[i], &m1->tai_slice_sup_lst[i]) == false )
      return false;
  } 

  if(m0->npn_sup_info_nid != m1->npn_sup_info_nid){
    if(m0->npn_sup_info_nid == NULL)
      return false;
    if(m1->npn_sup_info_nid == NULL)
      return false;
    if(*m0->npn_sup_info_nid != *m1->npn_sup_info_nid)
      return false;
  }

  if(m0->sz_ext_slice_sup_lst != m1->sz_ext_slice_sup_lst)
    return false;

  for(size_t i =0 ; i < m0->sz_ext_slice_sup_lst; ++i){
    if(eq_s_nssai(&m0->ext_tai_slice_sup_lst[i], &m1->ext_tai_slice_sup_lst[i]) == false )
      return false;
  }

  return true;
}

