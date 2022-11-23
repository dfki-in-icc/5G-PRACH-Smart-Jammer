#ifndef F1AP_TRANSPORT_BANDWIDTH_MIR_H
#define F1AP_TRANSPORT_BANDWIDTH_MIR_H 

#include <stdbool.h>

typedef enum {
	F1AP_NR_NRB_nrb11	= 0,
	F1AP_NR_NRB_nrb18	= 1,
	F1AP_NR_NRB_nrb24	= 2,
	F1AP_NR_NRB_nrb25	= 3,
	F1AP_NR_NRB_nrb31	= 4,
	F1AP_NR_NRB_nrb32	= 5,
	F1AP_NR_NRB_nrb38	= 6,
	F1AP_NR_NRB_nrb51	= 7,
	F1AP_NR_NRB_nrb52	= 8,
	F1AP_NR_NRB_nrb65	= 9,
	F1AP_NR_NRB_nrb66	= 10,
	F1AP_NR_NRB_nrb78	= 11,
	F1AP_NR_NRB_nrb79	= 12,
	F1AP_NR_NRB_nrb93	= 13,
	F1AP_NR_NRB_nrb106	= 14,
	F1AP_NR_NRB_nrb107	= 15,
	F1AP_NR_NRB_nrb121	= 16,
	F1AP_NR_NRB_nrb132	= 17,
	F1AP_NR_NRB_nrb133	= 18,
	F1AP_NR_NRB_nrb135	= 19,
	F1AP_NR_NRB_nrb160	= 20,
	F1AP_NR_NRB_nrb162	= 21,
	F1AP_NR_NRB_nrb189	= 22,
	F1AP_NR_NRB_nrb216	= 23,
	F1AP_NR_NRB_nrb217	= 24,
	F1AP_NR_NRB_nrb245	= 25,
	F1AP_NR_NRB_nrb264	= 26,
	F1AP_NR_NRB_nrb270	= 27,
	F1AP_NR_NRB_nrb273	= 28
	/*
	 * Enumeration is extensible
	 */
}  NR_NRB_F1AP_e;

typedef enum {
	F1AP_NR_SCS_scs15	= 0,
	F1AP_NR_SCS_scs30	= 1,
	F1AP_NR_SCS_scs60	= 2,
	F1AP_NR_SCS_scs120	= 3
	/*
	 * Enumeration is extensible
	 */
} F1AP_NR_SCS_e;

typedef struct{
// 9.3.1.15
// Transmission Bandwidth

   F1AP_NR_SCS_e nr_scs;
   NR_NRB_F1AP_e n_rb;

} trans_bw_f1ap_t ;

void free_trans_bw_f1ap(trans_bw_f1ap_t* src);

bool eq_trans_bw_f1ap(trans_bw_f1ap_t const* m0, trans_bw_f1ap_t const* m1);

#endif

