#ifndef PHY_PROCEDURES_NR_GNB_MIR_H
#define PHY_PROCEDURES_NR_GNB_MIR_H 


#include "PHY/defs_gNB.h"
#include "sched_nr.h"
#include "PHY/NR_TRANSPORT/nr_transport_proto.h"
#include "PHY/NR_TRANSPORT/nr_dlsch.h"
#include "PHY/NR_TRANSPORT/nr_ulsch.h"
#include "PHY/NR_TRANSPORT/nr_dci.h"
#include "PHY/NR_ESTIMATION/nr_ul_estimation.h"
#include "nfapi/open-nFAPI/nfapi/public_inc/nfapi_interface.h"
#include "nfapi/open-nFAPI/nfapi/public_inc/nfapi_nr_interface.h"
#include "fapi_nr_l1.h"
#include "common/utils/LOG/log.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "PHY/INIT/phy_init.h"
#include "PHY/MODULATION/nr_modulation.h"
#include "PHY/NR_UE_TRANSPORT/srs_modulation_nr.h"
#

#ifdef TASK_MANAGER
void nr_postDecode(PHY_VARS_gNB *gNB, ldpcDecode_t *rdata); 
#else
void nr_postDecode(PHY_VARS_gNB *gNB, notifiedFIFO_elt_t *req);
#endif


#endif
