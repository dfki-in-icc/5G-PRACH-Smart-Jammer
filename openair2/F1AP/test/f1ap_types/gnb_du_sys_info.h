#ifndef GNB_DU_SYS_INFO_MIR_H
#define GNB_DU_SYS_INFO_MIR_H 

#include <stdbool.h>

typedef struct {

} gnb_du_sys_info_t;

void free_gnb_du_sys_info( gnb_du_sys_info_t* src);

bool eq_gnb_du_sys_info( gnb_du_sys_info_t const* m0,  gnb_du_sys_info_t const* m1);

#endif

