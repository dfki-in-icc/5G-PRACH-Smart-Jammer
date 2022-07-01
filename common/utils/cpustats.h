#ifndef _CPUSTATS_H
#define _CPUSTATS_H
/* 
 * Activation, deactivation and printing functionalities of cpu statistics for UE and gNB Level 1/PHY indicators
 */
#include "common/utils/telnetsrv/telnetsrv.h"
#include "openair1/PHY/defs_nr_UE.h"
#include "openair1/PHY/defs_gNB.h"

#define UE_STATS_L1       "phycpunew"
void UEL1cpustats_measurcmd_display(telnet_printfunc_t prnt);
bool UEL1cpustats_enable(void);
void UEL1cpustats_disable(void);

#define GNB_STATS_L1      "gnb_L1"
bool gNBL1cpustats_enable(PHY_VARS_gNB *gNB);
void gNBL1cpustats_disable(void);
void gNBL1cpustats_measurcmd_display (telnet_printfunc_t prnt);

#endif