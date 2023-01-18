#ifndef F1AP_ACTIVATE_CELL_MIR_H
#define F1AP_ACTIVATE_CELL_MIR_H

#include "nr_cgi.h"
#include "gnb_cu_sys_info.h"
#include "avail_plmn.h"
#include "ext_avail_plmn.h"
#include "ab_info_iab_donor_cu.h"
#include "av_snpn_id.h"

#include <stdlib.h>

// 9.2.1.5 ETSI TS 138 473 V16.5.0 (2021-04) 
// F1 SETUP RESPONSE && GNB CU CONF UPDATE

typedef struct{

  // mandatory
  // NR CGI 9.3.1.12
  nr_cgi_t nr_cgi;

  // optional
  // NR PCI
  uint16_t* nr_pci; // (0..1007) 

  // optional
  // gNB-CU System Information 9.3.1.42
  gnb_cu_sys_info_t* sys_info;

  // optional
  // Available PLMN List 9.3.1.65 
  size_t sz_avail_plmn_lst; // [0,6]
  avail_plmn_t* avail_plmn;

  // optional
  // Extended Available PLMN List 9.3.1.76 
  size_t sz_ext_avail_plmn_lst; // [0,6] 
  ext_avail_plmn_t* ext_avail_plmn;

  // optional AB Info IAB-donor-CU 9.3.1.105
  ab_info_iab_donor_cu_t* ab_info; 

  // Available SNPN ID List 9.3.1.163
  size_t sz_av_snpn_id_lst; // [0,12]
  av_snpn_id_t* av_snpn;

} activate_cell_t;

void free_activate_cell(activate_cell_t* cell);

bool eq_activate_cell(activate_cell_t const* m0,  activate_cell_t const* m1);

#endif

