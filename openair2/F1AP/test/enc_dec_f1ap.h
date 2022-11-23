#ifndef ENCODE_DECODE_F1AP_MIR_H
#define ENCODE_DECODE_F1AP_MIR_H 

#include "../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/F1AP_F1AP-PDU.h"
#include "byte_array.h"

byte_array_t encode_pdu_f1ap(F1AP_F1AP_PDU_t const* pdu);

F1AP_F1AP_PDU_t decode_pdu_f1ap(byte_array_t ba);

#endif


