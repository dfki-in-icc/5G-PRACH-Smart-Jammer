#ifndef F1AP_FILL_RANDOM_DATA_MIR_H
#define F1AP_FILL_RANDOM_DATA_MIR_H 

#include "f1ap_types/f1_setup.h"
#include "f1ap_types/f1_setup_response.h"
#include "f1ap_types/f1_setup_failure.h"
#include "f1ap_types/ue_ctx_setup_request.h"
#include "f1ap_types/ue_ctx_setup_response.h"
#include "f1ap_types/gnb_cu_conf_update.h"

f1_setup_t gen_rnd_f1_setup(void);

f1_setup_response_t gen_rnd_f1_setup_response(void);

f1_setup_failure_t gen_rnd_f1_setup_failure(void);

ue_ctx_setup_request_t gen_rnd_ue_ctx_setup_request(void);

ue_ctx_setup_response_t gen_rnd_ue_ctx_setup_response(void);

gnb_cu_conf_update_t gen_rnd_gnb_cu_conf_update(void);

#endif

