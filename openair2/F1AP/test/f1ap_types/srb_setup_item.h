#ifndef SRB_SETUP_ITEM_F1AP_MIR_H
#define SRB_SETUP_ITEM_F1AP_MIR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct{
  // 9.3.1.7
  uint8_t srb_id; // [0,3]

  // 9.3.1.35
   uint8_t lc_id; // [1,32]

} srb_setup_item_t;

bool eq_srb_setup_item(srb_setup_item_t const* m0, srb_setup_item_t const* m1);

#endif
