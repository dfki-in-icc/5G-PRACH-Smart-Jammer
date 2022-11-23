#include "gnb_cu_sys_info.h"

#include <assert.h>
#include <stdlib.h>

void free_gnb_cu_sys_info( gnb_cu_sys_info_t* src)
{
  assert(src != NULL);

  for(size_t i = 0; i < src->len; i++){
    free_sib(&src->sib[i]);
  }
 
  free(src->sib);

  assert(src->id == NULL && "Not implemented");
}

bool eq_gnb_cu_sys_info(gnb_cu_sys_info_t const* m0, gnb_cu_sys_info_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL)
    return false;

  if(m1 == NULL)
    return false;

  if(m0->len != m1->len)
    return false;

  for(size_t i =0; i < m0->len; ++i){
    if(eq_sib(&m0->sib[i], &m1->sib[i]) == false)
      return false;
  }

  assert(m0->id == NULL && "Not implemented");
  assert(m1->id == NULL && "Not implemented");

  return true;
}

