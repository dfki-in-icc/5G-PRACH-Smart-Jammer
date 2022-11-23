#ifndef F1AP_BROADCAST_PLMN_ID_INFO_MIR_H
#define F1AP_BROADCAST_PLMN_ID_INFO_MIR_H 

#include <stdbool.h>

typedef struct{

} brdcst_PLMN_id_info_f1ap_t;

void free_brdcst_PLMN_id_info_f1ap( brdcst_PLMN_id_info_f1ap_t* src);

bool eq_brdcst_PLMN_id_info_f1ap( brdcst_PLMN_id_info_f1ap_t const* m0, brdcst_PLMN_id_info_f1ap_t const* m1);

#endif

