#include "ul_up_tnl_info_lst.h"

#include <assert.h>
#include <stdlib.h>

void free_ul_up_tnl_info_lst(ul_up_tnl_info_lst_t* src)
{
  assert(src != NULL);

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  free_ul_up_tnl_info(&src->tnl_info);

  // Optional
  // BH Information 9.3.1.114
  assert(src->bh_info == NULL && "Not implemented" );
}

bool eq_ul_up_tnl_info_lst(ul_up_tnl_info_lst_t const* m0, ul_up_tnl_info_lst_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // Mandatory
  // UL UP TNL Information 9.3.2.1
  if(eq_ul_up_tnl_info(&m0->tnl_info, &m1->tnl_info ) == false)
    return false;

  // Optional
  // BH Information 9.3.1.114
  assert(m0->bh_info == NULL && "Not implemented" );
  assert(m1->bh_info == NULL && "Not implemented" );

  return true;
}

