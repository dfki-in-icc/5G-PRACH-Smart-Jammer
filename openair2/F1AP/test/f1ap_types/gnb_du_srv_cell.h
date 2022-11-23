#ifndef F1AP_GNB_DU_SRV_CELL_MIR_H
#define F1AP_GNB_DU_SRV_CELL_MIR_H 

#include "srv_cell_info.h"
#include "gnb_du_sys_info.h"

typedef struct{

  srv_cell_info_t srv_cell_info;
  gnb_du_sys_info_t* info; 

} gnb_du_srv_cell_f1ap_t;

void free_gnb_du_srv_cell_f1ap( gnb_du_srv_cell_f1ap_t* src);

bool eq_gnb_du_srv_cell_f1ap(gnb_du_srv_cell_f1ap_t const* m0, gnb_du_srv_cell_f1ap_t const* m1);


#endif

