#ifndef F1AP_SERVED_CELL_INFO_MIR_H
#define F1AP_SERVED_CELL_INFO_MIR_H 

#include "../byte_array.h"
#include "nr_cgi.h"
#include "srv_plmn.h"
#include "nr_mode_info.h"
#include "tdd_info.h"
#include "fdd_info.h"
#include "iab_info_iab_du.h"
#include "ssb_pos_burst.h"
#include "nr_prach_conf.h"
#include "brdcst_PLMN_id_info.h"

//#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_CellSize.h"
//#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_Cell-Direction.h"
//#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_ConfiguredTACIndication.h"


#include <stdint.h>
#include <stdlib.h>

typedef struct{

  // 1 NR CGI
  nr_cgi_t nr_cgi;

  // 2 NR PCI
  uint16_t nr_pci; // [0, 1007]

  // 3 5GS TAC
  uint32_t* five_gs_tac; // len 3

  // 4 Configured EPS TAC
  uint16_t* eps_tac;

  // 5 Served PLMN
  size_t sz_srv_plmn;
  srv_plmn_f1ap_t* srv_plmn;

  // 6 CHOICE NR-Mode-Info
  nr_mode_info_e nr_mode_info;

  union{
      tdd_info_f1ap_t tdd;
      fdd_info_f1ap_t fdd;
  };


  // 7 Measurement Timing Configuration. Defined at 3gpp TS 38.331 (MeasurementTimingConfiguration)
  byte_array_t meas_timing_conf;

  // 8 RANAC 
  uint8_t* ran_ac;

  // 9 Extended Served PLMN
  size_t sz_ext_srv_plmn;
  srv_plmn_f1ap_t* ext_srv_plmn;

  // 10 Cell Direction
  //e_F1AP_Cell_Direction* cell_dir;
  void* cell_dir;

  // 11 Cell Type
  //e_F1AP_CellSize* cell_type;
  void* cell_type;

  // 12 Broadcast PLMN Identity Info
  size_t sz_brdcst_PLMN_id_info;
  brdcst_PLMN_id_info_f1ap_t* brdcst_PLMN_id_info; 

  // 13 Configured TAC Indication
  //e_F1AP_ConfiguredTACIndication* conf_tac_ind; // enum TRUE
  void* conf_tac_ind; // enum TRUE

  // 14 Aggressor gNB Set ID
  byte_array_t* aggr_gnb_set_id;   

  // 15 Victim gNB Set ID
  byte_array_t* victim_gnb_set_id;

  // 16 AB Info IAB-DU
  iab_info_iab_du_f1ap_t* ab_info_iab_du;

  // 17 SSB Positions In Burst
  ssb_pos_burst_t* ssb_pos_burst;

  // 18 NR PRACH Configuration
  nr_prach_conf_t* nr_prach_conf; 
  
  // 19 SFN Offset
  byte_array_t* sfn_offset;

} srv_cell_info_t; 

void free_srv_cell_info(srv_cell_info_t* src);

bool eq_srv_cell_info( srv_cell_info_t const* m0, srv_cell_info_t const* m1); 

#endif

