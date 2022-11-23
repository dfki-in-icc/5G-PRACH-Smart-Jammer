#ifndef F1AP_GNB_RRC_VERSION_MIR_H
#define F1AP_GNB_RRC_VERSION_MIR_H 

#include "../byte_array.h"

#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

#include <stdbool.h>

typedef struct{
 BIT_STRING_t latest_rrc_version;
 byte_array_t* latest_rrc_version_enh;
} gnb_rrc_ver_f1ap_t;

void free_gnb_rrc_ver_f1ap(gnb_rrc_ver_f1ap_t* src);

bool eq_gnb_rrc_ver_f1ap(gnb_rrc_ver_f1ap_t const* m0, gnb_rrc_ver_f1ap_t const* m1);


#endif

