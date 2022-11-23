#ifndef F1AP_SERVED_PLMN_MIR_H
#define F1AP_SERVED_PLMN_MIR_H 

#include "plmn.h"
#include "s_nssai.h"

#include <stdbool.h>
#include <stdlib.h>


typedef struct{
  plmn_f1ap_t plmn_id;
  
  size_t sz_tai_slice_sup_lst;
  s_nssai_t* tai_slice_sup_lst;

  uint64_t* npn_sup_info_nid; // SIZE(44) bits 

  size_t sz_ext_slice_sup_lst;
  s_nssai_t* ext_tai_slice_sup_lst;

} srv_plmn_f1ap_t;

void free_srv_plmn_f1ap( srv_plmn_f1ap_t* src);

bool eq_srv_plmn_f1ap( srv_plmn_f1ap_t const* m0, srv_plmn_f1ap_t const* m1);

#endif

