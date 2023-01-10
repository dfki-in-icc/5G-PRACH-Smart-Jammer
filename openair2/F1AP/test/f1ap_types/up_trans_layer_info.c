#include "up_trans_layer_info.h"

#include <assert.h>
#include <stdlib.h>

void free_up_trans_layer_info( up_trans_layer_info_t* src)
{
  assert(src != NULL);

  assert(src->trans_layer_addr.size > 0);
  free(src->trans_layer_addr.buf);
}

