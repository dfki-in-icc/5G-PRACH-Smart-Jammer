#ifndef UL_UP_TNL_INFORMATION_LIST_F1AP_MIR_H
#define UL_UP_TNL_INFORMATION_LIST_F1AP_MIR_H 

#include <stdbool.h>

#include "bh_info.h"
#include "ul_up_tnl_info.h"

typedef struct{

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  ul_up_tnl_info_t tnl_info;

  // Optional
  // BH Information 9.3.1.114
  bh_info_t* bh_info; 

} ul_up_tnl_info_lst_t;

void free_ul_up_tnl_info_lst(ul_up_tnl_info_lst_t* src);

bool eq_ul_up_tnl_info_lst(ul_up_tnl_info_lst_t const* m0, ul_up_tnl_info_lst_t const* m1);

#endif

