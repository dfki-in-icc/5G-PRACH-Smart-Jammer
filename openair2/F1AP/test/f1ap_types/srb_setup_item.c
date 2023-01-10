#include "srb_setup_item.h"

#include <assert.h>
#include <stdlib.h>

bool eq_srb_setup_item(srb_setup_item_t const* m0, srb_setup_item_t const* m1)
{
  if(m0 == m1)
    return true;

  if(m0 == NULL || m1 == NULL)
    return false;

  // 9.3.1.7 // [0,3]
  assert(m0->srb_id < 4 );
  assert(m1->srb_id < 4 );
  if(m0->srb_id  != m1->srb_id)
    return false;

  // 9.3.1.35 // [1,32]
  assert(m0->lc_id > 0 && m0->lc_id < 33);
  assert(m1->lc_id > 0 && m1->lc_id < 33);
  if(m0->lc_id != m1->lc_id)
    return false;

  return true;
}

