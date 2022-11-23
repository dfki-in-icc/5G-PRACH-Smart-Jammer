#ifndef F1AP_NR_CARRIER_LST_MIR_H
#define F1AP_NR_CARRIER_LST_MIR_H 

#include <stdbool.h>

typedef struct{
// NR Carrier List
// 9.3.1.137
} nr_carrier_lst_f1ap_t;


void free_nr_carrier_lst_f1ap( nr_carrier_lst_f1ap_t* src);

bool eq_nr_carrier_lst_f1ap( nr_carrier_lst_f1ap_t const* m0,  nr_carrier_lst_f1ap_t const* m1);

#endif

