#ifndef F1AP_CELLS_TO_BE_ACTIVE_LIST_H
#define F1AP_CELLS_TO_BE_ACTIVE_LIST_H

#include "avail_plmn.h"
#include "av_snpn_id.h"
#include "ext_avail_plmn.h"
#include "gnb_cu_sys_info.h"
#include "iab_info_iab_cu.h"
#include "nr_cgi.h"

typedef struct {
  // NR CGI 9.3.1.12
  // Mandatory
  nr_cgi_t nr_cgi;

  // NR PCI
  // Optional
  uint16_t* nr_pci; // [0,1007]

 // gNB-CU System Information 9.3.1.42
 // Optional
  gnb_cu_sys_info_t* gnb_cu_sys_info; 

  // Available PLMN List 9.3.1.65
  // Optional
  avail_plmn_t* avail_plmn;

  // Extended Available PLMN List 9.3.1.76
  // Optional
  ext_avail_plmn_t* ext_avail_plmn; 

  // IAB Info IAB-donor-CU 9.3.1.105
  // Optional
  iab_info_iab_cu_t* iab_info_iab_cu;

  // Available SNPN ID List 9.3.1.163
  // Optional
  av_snpn_id_t* ava_snpn_id; 

} cells_to_be_act_t;


#endif

