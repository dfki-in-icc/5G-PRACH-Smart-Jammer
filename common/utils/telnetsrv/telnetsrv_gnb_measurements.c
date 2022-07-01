#define TELNETSERVERCODE
#include "telnetsrv.h"
#include "telnetsrv_measurements.h"
#include "telnetsrv_ltemeasur_def.h"
#include "common/utils/cpustats.h"
#include "openair2/LAYER2/NR_MAC_gNB/nr_mac_gNB.h"

static void gNBL2pstats_measurcmd_display(telnet_printfunc_t prnt);
static char *gNBL2pstats_gethdrline1(char * bufhdr, size_t maxlen);
static char *gNBL2pstats_gethdrline2(char * bufhdr, size_t maxlen);
static const char *scale_number (float n, int width);
/* 
 * caveats: 5th fiels of nrgNBmeasurgroups[] entries is NULL as we are not yet defining in this module the statistics content to 
 * be passed from statistics thread. That field was created for future extensibility work on reuniting statistics management.
 */
static telnet_measurgroupdef_t nrgNBmeasurgroups[] = {
  {GNB_STATS_L2_MAC, GROUP_LTESTATS, 0, gNBL2pstats_measurcmd_display  , {NULL}}, 
  {GNB_STATS_L1    , GROUP_CPUSTATS, 0, gNBL1cpustats_measurcmd_display, {NULL}}
};

const char SPACE_LABEL=' ';
const char MINUS_LABEL='-';
const char * ULSCH_LABEL="[UL sch]";
const char * DLSCH_LABEL="[DL sch]";

int get_measurgroups(telnet_measurgroupdef_t **measurgroups) 
{
  *measurgroups = nrgNBmeasurgroups;
  return sizeof(nrgNBmeasurgroups)/sizeof(telnet_measurgroupdef_t);
}

// XXX: table below would need a bit of tuning to understand expected values ranges and avoid massive use of scaling functionality
static const struct itemtable_t{                                              
    int           width;      // field width in ASCII characters
    unsigned int  tobescaled; // 1(true), 0(false) scaling the value to i.e. Kb, Mb, etc..
    char          *itemlabel; // the new libproc item enum identifier
    unsigned int  scheduler;  // none(0), DownLink scheduler(1), UpLink scheduler (2)
} itemtable[] = {
  {2, 0, "UE"      , 0}, // UE number
  {4, 0, "RNTI"    , 0},  
  {2, 0, "PH"      , 0},  
  {5, 0, "PCMax"   , 0},  
  {7, 0, "RSRPavg" , 0}, 
  {8, 0, "RSRPmeas", 0}, 
  {3, 0, "cqi"     , 0}, 
  {2, 0, "ri"      , 0}, 
  {5, 0, "pmix1"   , 0}, // uint8
  {5, 0, "pmix2"   , 0}, // uint8
  {4, 1, "H"       , 1},
  {4, 0, "A"       , 1}, 
  {4, 0, "R"       , 1}, 
  {4, 0, "Q"       , 1}, 
  {4, 1, "E"       , 1}, 
  {3, 0, "dtx"     , 1}, 
  {5, 1, "bler"    , 1}, // float
  {3, 0, "mcs"     , 1},
  {6, 1, "bdlsch"  , 2}, // tot bytes scheduled
  {4, 1, "H"       , 2},
  {4, 0, "A"       , 2}, 
  {4, 0, "R"       , 2}, 
  {4, 0, "Q"       , 2}, 
  {4, 1, "E"       , 2}, 
  {3, 0, "dtx"     , 2}, 
  {5, 1, "bler"    , 2}, // float
  {3, 0, "mcs"     , 2}, 
  {6, 1, "bulsch"  , 2}, // tot bytes scheduled
  {6, 1, "brxtot"  , 2}  // tot bytes received
  
};

static char * gNBL2pstats_gethdrline1(char *bufhdr, size_t maxlen) 
{
  unsigned int idx;
  int bufhdr_idx=0;
  
  int empty_spaces = 0;
  int dl_sch = 0;
  int ul_sch = 0;

  for (idx=0; idx < (sizeof(itemtable)/sizeof(struct itemtable_t)); idx++)
  {
    if ((bufhdr_idx + itemtable[idx].width + 1) >  maxlen)
      return NULL;

    switch (itemtable[idx].scheduler) 
    {
    case 0:
      empty_spaces += itemtable[idx].width + 1;
      memset(bufhdr + bufhdr_idx, SPACE_LABEL, itemtable[idx].width + 1);
      break;
    case 1:
      dl_sch += itemtable[idx].width + 1;
      memset(bufhdr + bufhdr_idx, MINUS_LABEL, itemtable[idx].width + 1);
      break;
    case 2:
      ul_sch += itemtable[idx].width + 1;
      memset(bufhdr + bufhdr_idx, MINUS_LABEL, itemtable[idx].width + 1);
      break;
    }
    bufhdr_idx += itemtable[idx].width + 1;
  }
  bufhdr[bufhdr_idx] = '\n';
  bufhdr_idx++;
  bufhdr[bufhdr_idx] = '\0';
  
  // now, overwite some parts if there is enough space
  if (dl_sch > (sizeof(DLSCH_LABEL) + 4 ))
  {
    bufhdr[empty_spaces + 2] = SPACE_LABEL;
    memcpy(bufhdr + empty_spaces + 3, DLSCH_LABEL, strlen(DLSCH_LABEL));
    bufhdr[empty_spaces + 3 + sizeof(DLSCH_LABEL)] = SPACE_LABEL;
    bufhdr[empty_spaces + dl_sch - 1] = SPACE_LABEL;
  }
  if (ul_sch > (sizeof(ULSCH_LABEL) + 4 ))
  {
    bufhdr[empty_spaces + dl_sch + 2] = SPACE_LABEL;
    memcpy(bufhdr + empty_spaces + dl_sch + 3, ULSCH_LABEL, strlen(ULSCH_LABEL));
    bufhdr[empty_spaces +  dl_sch + 3 + sizeof(ULSCH_LABEL)] = SPACE_LABEL;
    bufhdr[empty_spaces + dl_sch + ul_sch - 1] = SPACE_LABEL;
  }

  return bufhdr;
}

static char * gNBL2pstats_gethdrline2(char *bufhdr, size_t maxlen) 
{
  unsigned int idx;
  int printed=0;

  for (idx=0; idx < (sizeof(itemtable)/sizeof(struct itemtable_t)); idx++)
  {
    if ((printed + itemtable[idx].width + 1) >  maxlen)
      return NULL;

    printed += snprintf(bufhdr + printed, maxlen - printed, "%*s ", itemtable[idx].width, itemtable[idx].itemlabel);
  }

  bufhdr[printed] = '\n';
  printed++;
  bufhdr[printed] = '\0';
  
  return bufhdr;
}

static const char *scale_number (float n, int width) 
{
  static char buf[128];
  buf[0] = '\0';
  
  if (width >= snprintf(buf, sizeof(buf), "%*.0f", width, n))
    return buf;

  char *plabel;
  char scale_labels[] =  { 'K', 'M', 'G', 'T', 'P', 'E', 0 };// E=exabytes are enough, no need for zettabytes and over
  for (plabel = scale_labels; *plabel != 0; plabel++) 
  {
    n /= 1024.0;
    if (width >= snprintf(buf, sizeof(buf), "%*.1f%c", width, n, *plabel))
      return buf;
      // let's retry with one decimal less to see if fits.
    if (width >= snprintf(buf, sizeof(buf), "%*.0f%c", width, n, *plabel))
      return buf;
  }
  // worst case...
  snprintf(buf, sizeof(buf), "%s" , "?");
  return buf;
}

/** 
 * Display UE statistics collected at gNB node related to MAC protocol (OSI Layer 2). This is for the moment limited to one instance of gNB
 * This function is meant to be used in combination with 'telnet set loopd 1000' and 'loop' command prepended to the 'measur' one to provide 
 * refresh of 1000ms of the CLI. 
 * 
 * XXX: You should anyway consider about keeping an historic on the screen for these stats, one line per iteration, 
 * not overwriting like loop command does until a specific command is pressed. One idea is to implement that as option of the loop command, 
 * that forces to go in append instead of refreshing the position on the screen.
 *   
 * XXX: note that max line length is TELNET_MAX_MSGLENGTH that is equal to 2048 chars. We do not check it yet in this function.
 */
static void gNBL2pstats_measurcmd_display(telnet_printfunc_t prnt)
{
  const int gnb_inst = 0; 
  gNB_MAC_INST *gNB = RC.nrmac[gnb_inst]; // CAVEATS: using global vars as signature of this function can't be change to fit more params.
  pthread_mutex_lock(&gNB->UE_info.mutex);
  
  static char buf[1024];
  prnt(gNBL2pstats_gethdrline1(buf, sizeof(buf)));
  prnt(gNBL2pstats_gethdrline2(buf, sizeof(buf)));
  
  /* loop over all the UEs registered: to each UE will be given a number that is just a progressive counter*/
  int uenum = 1; 
  UE_iterator(gNB->UE_info.list, UE) 
  {   
    NR_UE_sched_ctrl_t *sched_ctrl = &UE->UE_sched_ctrl;
    NR_mac_stats_t          *stats = &UE->mac_stats;
    const int             avg_rsrp = stats->num_rsrp_meas > 0 ? stats->cumul_rsrp / stats->num_rsrp_meas : 0;

    prnt("%*d"        , itemtable[0].width , uenum++);
    prnt(" %0*x"      , itemtable[1].width , UE->rnti);           
    prnt(" %*d"       , itemtable[2].width , sched_ctrl->ph);
    prnt(" %*d"       , itemtable[3].width , sched_ctrl->pcmax);
    prnt(" %*d"       , itemtable[4].width , avg_rsrp);
    prnt(" %*d"       , itemtable[5].width , stats->num_rsrp_meas);
    prnt(" %*d"       , itemtable[6].width , UE->UE_sched_ctrl.CSI_report.cri_ri_li_pmi_cqi_report.wb_cqi_1tb);
    prnt(" %*d"       , itemtable[7].width , UE->UE_sched_ctrl.CSI_report.cri_ri_li_pmi_cqi_report.ri+1);
    prnt(" %*d"       , itemtable[8].width , UE->UE_sched_ctrl.CSI_report.cri_ri_li_pmi_cqi_report.pmi_x1); 
    prnt(" %*d"       , itemtable[9].width , UE->UE_sched_ctrl.CSI_report.cri_ri_li_pmi_cqi_report.pmi_x2);
    prnt(" ");
    prnt(scale_number(stats->dl.rounds[0], itemtable[10].width));
    prnt(" %*"PRIu64"", itemtable[11].width, stats->dl.rounds[1]);
    prnt(" %*"PRIu64"", itemtable[12].width, stats->dl.rounds[2]);
    prnt(" %*"PRIu64"", itemtable[13].width, stats->dl.rounds[3]);
    prnt(" %*"PRIu64"", itemtable[14].width, stats->dl.errors);
    prnt(" %*d"       , itemtable[15].width, stats->pucch0_DTX);
    prnt(" %*.3f"     , itemtable[16].width, sched_ctrl->dl_bler_stats.bler);
    prnt(" %*d"       , itemtable[17].width, sched_ctrl->dl_bler_stats.mcs);
    prnt(" ");
    prnt(scale_number(stats->dl.total_bytes, itemtable[18].width));
    prnt(" ");
    prnt(scale_number(stats->ul.rounds[0], itemtable[19].width));
    //prnt(" %*"PRIu64"", itemtable[19].width, stats->ul.rounds[0]);
    prnt(" %*"PRIu64"", itemtable[20].width, stats->ul.rounds[1]);
    prnt(" %*"PRIu64"", itemtable[21].width, stats->ul.rounds[2]);
    prnt(" %*"PRIu64"", itemtable[22].width, stats->ul.rounds[3]);
    prnt(" %*"PRIu64"", itemtable[23].width, stats->ul.errors);
    prnt(" %*d"       , itemtable[24].width, stats->ulsch_DTX);
    prnt(" %*.3f"     , itemtable[25].width, sched_ctrl->ul_bler_stats.bler);
    prnt(" %*d"       , itemtable[26].width, sched_ctrl->ul_bler_stats.mcs);
    prnt(" ");
    prnt(scale_number(stats->ulsch_total_bytes_scheduled, itemtable[27].width));
    prnt(" ");
    prnt(scale_number(stats->ul.total_bytes, itemtable[28].width));
    prnt("\n");
    
    /*
    * XXX-TODO: the values below
    *  values: stats->dl.lc_bytes[lc_id] for lc_id = 0 ..63;
    *  values: stats->ul.lc_bytes[lc_id] for lc_id = 0 ..63;
    * were included in the previous version of the stats. We will think about including as they are too many 
    * to be added to one line stats display.
    */
  }

  pthread_mutex_unlock(&gNB->UE_info.mutex);

  /* 
   * XXX-TODO: also the values below
   *  print_meas(&gNB->eNB_scheduler, "DL & UL scheduling timing stats", NULL, NULL);
   *  print_meas(&gNB->schedule_dlsch,"dlsch scheduler",NULL,NULL);
   *  print_meas(&gNB->rlc_data_req, "rlc_data_req",NULL,NULL);
   *  print_meas(&gNB->rlc_status_ind,"rlc_status_ind",NULL,NULL);
   * were included in the previous version of the stats. They were using print_meas() to display to stderr and should be changed to 
   * leverage prnt() function. This can be done via using print_meas_log() that prints to buffer, but unfortunately this function will include
   * the header of the fields you are printing (and we want to have just one line header, see gNBL2pstats_gethdrline2()), so you probably would
   * need to create a new print function i.e. print_meas_lograw().
   */
}
