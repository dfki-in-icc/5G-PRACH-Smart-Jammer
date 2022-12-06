#ifndef ADD_PDCP_DUPLICATION_TNL_F1AP_H
#define ADD_PDCP_DUPLICATION_TNL_F1AP_H 

#include <stdbool.h>

#include "../byte_array.h"
#include "../../../../cmake_targets/ran_build/build/CMakeFiles/F1AP_R16.3.1/BIT_STRING.h"

// 9.3.2.1
typedef struct{
  // Mandatory
  // Transport Layer Address 9.3.2.3
  BIT_STRING_t	trans_layer_add; // BIT STRING(SIZE(1..160, ...))
 // byte_array_t trans_layer_add; // BIT STRING(SIZE(1..160, ...))

  // Mandatory
  // GTP-TEID 9.3.2.2
  char gtp_teid[4];

} add_pdcp_dup_tnl_t;

void free_add_pdcp_dup_tnl( add_pdcp_dup_tnl_t* src);

bool eq_add_pdcp_dup_tnl( add_pdcp_dup_tnl_t const* m0, add_pdcp_dup_tnl_t const* m1);

#endif
