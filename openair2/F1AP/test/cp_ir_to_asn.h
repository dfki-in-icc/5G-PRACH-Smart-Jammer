#ifndef COPY_FROM_INTERMIDIATE_REPRESENTATION_TO_ASN_MIR_H
#define COPY_FROM_INTERMIDIATE_REPRESENTATION_TO_ASN_MIR_H 

#include "f1ap_types/f1_setup.h"
#include "f1ap_types/f1_setup_response.h"
#include "f1ap_types/f1_setup_failure.h"
#include "f1ap_types/ue_ctx_setup_request.h"
#include "f1ap_types/ue_ctx_setup_response.h"

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_F1AP-PDU.h"


F1AP_F1AP_PDU_t cp_f1_setup_asn(f1_setup_t const* src);

F1AP_F1AP_PDU_t cp_f1_setup_response_asn(f1_setup_response_t const* src);

F1AP_F1AP_PDU_t cp_f1_setup_failure_asn(f1_setup_failure_t const* src);

F1AP_F1AP_PDU_t cp_ue_ctx_setup_request_asn(ue_ctx_setup_request_t const* src);

F1AP_F1AP_PDU_t cp_ue_ctx_setup_response_asn(ue_ctx_setup_response_t const* src);

#endif

