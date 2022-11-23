#ifndef GNB_CU_SYS_INFO_MIR_H
#define GNB_CU_SYS_INFO_MIR_H 

#include <stdbool.h>
#include <stdlib.h>

#include "sib.h"
#include "sys_info_area_id.h"

// 9.3.1.42
typedef struct {

  size_t len;
  sib_t* sib;

  // Optional. Bit String of 24
  sys_info_area_id_t* id;

} gnb_cu_sys_info_t;

void free_gnb_cu_sys_info( gnb_cu_sys_info_t* src);

bool eq_gnb_cu_sys_info(gnb_cu_sys_info_t const* m0, gnb_cu_sys_info_t const* m1);

#endif

