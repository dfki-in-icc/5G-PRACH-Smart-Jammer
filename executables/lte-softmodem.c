/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file lte-softmodem.c
 * \brief Top-level threads for eNodeB
 * \author R. Knopp, F. Kaltenberger, Navid Nikaein
 * \date 2012
 * \version 0.1
 * \company Eurecom
 * \email: knopp@eurecom.fr,florian.kaltenberger@eurecom.fr, navid.nikaein@eurecom.fr
 * \note
 * \warning
 */


#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#undef MALLOC //there are two conflicting definitions, so we better make sure we don't use it at all

#include "assertions.h"

#include "PHY/types.h"

#include "PHY/defs_eNB.h"
#include "common/ran_context.h"
#include "common/config/config_userapi.h"
#include "common/utils/load_module_shlib.h"
#undef MALLOC //there are two conflicting definitions, so we better make sure we don't use it at all
//#undef FRAME_LENGTH_COMPLEX_SAMPLES //there are two conflicting definitions, so we better make sure we don't use it at all

#include "radio/COMMON/common_lib.h"
#include "radio/ETHERNET/USERSPACE/LIB/if_defs.h"

//#undef FRAME_LENGTH_COMPLEX_SAMPLES //there are two conflicting definitions, so we better make sure we don't use it at all

#include <openair1/PHY/phy_extern_ue.h>

#include "PHY/phy_vars.h"
#include "SCHED/sched_common_vars.h"
#include "LAYER2/MAC/mac_vars.h"

#include "LAYER2/MAC/mac.h"
#include "LAYER2/MAC/mac_proto.h"
#include "RRC/LTE/rrc_vars.h"
#include "PHY_INTERFACE/phy_interface_vars.h"
#include "PHY/TOOLS/phy_scope_interface.h"
#include "nfapi/oai_integration/vendor_ext.h"
#ifdef SMBV
#include "PHY/TOOLS/smbv.h"
unsigned short config_frames[4] = {2,9,11,13};
#endif
#include "common/utils/LOG/log.h"
#include "UTIL/OTG/otg_tx.h"
#include "UTIL/OTG/otg_externs.h"
#include "UTIL/MATH/oml.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "UTIL/OPT/opt.h"
#include "enb_config.h"


#include "create_tasks.h"


#include "PHY/INIT/phy_init.h"

#include "system.h"

#include "lte-softmodem.h"
#include "NB_IoT_interface.h"
#include <executables/split_headers.h>

#if USING_GPROF
#  include "sys/gmon.h"
#endif



#ifdef OAI_E2_AGENT

//// Slicing
#include "openair2/LAYER2/MAC/slicing/slicing.h"

//////////////////////////////////
//// E2 Agent headers
//////////////////////////////////

#include "openair2/E2AP/sm/agent_if/read/sm_ag_if_rd.h"
#include "openair2/E2AP/sm/agent_if/write/sm_ag_if_wr.h"
#include "openair2/E2AP/sm/sm_io.h"
#include "openair2/E2AP/agent/e2_agent_api.h"
#include <time.h>

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

#endif // OAI_E2_AGENT

pthread_cond_t nfapi_sync_cond;
pthread_mutex_t nfapi_sync_mutex;
int nfapi_sync_var=-1; //!< protected by mutex \ref nfapi_sync_mutex


pthread_cond_t sync_cond;
pthread_mutex_t sync_mutex;
int sync_var=-1; //!< protected by mutex \ref sync_mutex.
int config_sync_var=-1;

uint16_t runtime_phy_rx[29][6]; // SISO [MCS 0-28][RBs 0-5 : 6, 15, 25, 50, 75, 100]
uint16_t runtime_phy_tx[29][6]; // SISO [MCS 0-28][RBs 0-5 : 6, 15, 25, 50, 75, 100]


int oai_exit = 0;

uint64_t                 downlink_frequency[MAX_NUM_CCs][4];
int32_t                  uplink_frequency_offset[MAX_NUM_CCs][4];

int UE_scan = 1;
int UE_scan_carrier = 0;
runmode_t mode = normal_txrx;

FILE *input_fd=NULL;


#if MAX_NUM_CCs == 1
rx_gain_t rx_gain_mode[MAX_NUM_CCs][4] = {{max_gain,max_gain,max_gain,max_gain}};
double tx_gain[MAX_NUM_CCs][4] = {{20,0,0,0}};
double rx_gain[MAX_NUM_CCs][4] = {{110,0,0,0}};
#else
rx_gain_t                rx_gain_mode[MAX_NUM_CCs][4] = {{max_gain,max_gain,max_gain,max_gain},{max_gain,max_gain,max_gain,max_gain}};
double tx_gain[MAX_NUM_CCs][4] = {{20,0,0,0},{20,0,0,0}};
double rx_gain[MAX_NUM_CCs][4] = {{110,0,0,0},{20,0,0,0}};
#endif

double rx_gain_off = 0.0;

double sample_rate=30.72e6;
double bw = 10.0e6;

uint8_t dci_Format = 0;
uint8_t agregation_Level =0xFF;

uint8_t nb_antenna_tx = 1;
uint8_t nb_antenna_rx = 1;

char ref[128] = "internal";
char channels[128] = "0";

int rx_input_level_dBm;
int otg_enabled;

uint64_t num_missed_slots=0; // counter for the number of missed slots

int split73=0;
void sendFs6Ul(PHY_VARS_eNB *eNB, int UE_id, int harq_pid, int segmentID, int16_t *data, int dataLen, int r_offset) {
  AssertFatal(false, "Must not be called in this context\n");
}
void sendFs6Ulharq(enum pckType type, int UEid, PHY_VARS_eNB *eNB, LTE_eNB_UCI *uci, int frame, int subframe, uint8_t *harq_ack, uint8_t tdd_mapping_mode, uint16_t tdd_multiplexing_mask, uint16_t rnti, int32_t stat) {
  AssertFatal(false, "Must not be called in this context\n");
}

RU_t **RCconfig_RU(int nb_RU,int nb_L1_inst,PHY_VARS_eNB ***eNB,uint64_t *ru_mask,pthread_mutex_t *ru_mutex,pthread_cond_t *ru_cond);

extern void reset_opp_meas(void);
extern void print_opp_meas(void);


RU_t **RCconfig_RU(int nb_RU,int nb_L1_inst,PHY_VARS_eNB ***eNB,uint64_t *ru_mask,pthread_mutex_t *ru_mutex,pthread_cond_t *ru_cond);

int transmission_mode=1;
int emulate_rf = 0;
int numerology = 0;

THREAD_STRUCT thread_struct;
/* struct for ethernet specific parameters given in eNB conf file */
eth_params_t *eth_params;

double cpuf;

int oaisim_flag=0;


/* forward declarations */
void set_default_frame_parms(LTE_DL_FRAME_PARMS *frame_parms[MAX_NUM_CCs]);

/*---------------------BMC: timespec helpers -----------------------------*/

struct timespec min_diff_time = { .tv_sec = 0, .tv_nsec = 0 };
struct timespec max_diff_time = { .tv_sec = 0, .tv_nsec = 0 };

struct timespec clock_difftime(struct timespec start, struct timespec end) {
  struct timespec temp;

  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }

  return temp;
}

void print_difftimes(void) {
#ifdef DEBUG
  printf("difftimes min = %lu ns ; max = %lu ns\n", min_diff_time.tv_nsec, max_diff_time.tv_nsec);
#else
  LOG_I(HW,"difftimes min = %lu ns ; max = %lu ns\n", min_diff_time.tv_nsec, max_diff_time.tv_nsec);
#endif
}

void update_difftimes(struct timespec start, struct timespec end) {
  struct timespec diff_time = { .tv_sec = 0, .tv_nsec = 0 };
  int             changed = 0;
  diff_time = clock_difftime(start, end);

  if ((min_diff_time.tv_nsec == 0) || (diff_time.tv_nsec < min_diff_time.tv_nsec)) {
    min_diff_time.tv_nsec = diff_time.tv_nsec;
    changed = 1;
  }

  if ((max_diff_time.tv_nsec == 0) || (diff_time.tv_nsec > max_diff_time.tv_nsec)) {
    max_diff_time.tv_nsec = diff_time.tv_nsec;
    changed = 1;
  }

#if 1

  if (changed) print_difftimes();

#endif
}

/*------------------------------------------------------------------------*/

unsigned int build_rflocal(int txi, int txq, int rxi, int rxq) {
  return (txi + (txq<<6) + (rxi<<12) + (rxq<<18));
}
unsigned int build_rfdc(int dcoff_i_rxfe, int dcoff_q_rxfe) {
  return (dcoff_i_rxfe + (dcoff_q_rxfe<<8));
}


void exit_function(const char *file, const char *function, const int line, const char *s) {
  int ru_id;

  if (s != NULL) {
    printf("%s:%d %s() Exiting OAI softmodem: %s\n",file,line, function, s);
  }
  close_log_mem();
  oai_exit = 1;

  if (RC.ru == NULL)
    exit(-1); // likely init not completed, prevent crash or hang, exit now...

  for (ru_id=0; ru_id<RC.nb_RU; ru_id++) {
    if (RC.ru[ru_id] && RC.ru[ru_id]->rfdevice.trx_end_func) {
      RC.ru[ru_id]->rfdevice.trx_end_func(&RC.ru[ru_id]->rfdevice);
      RC.ru[ru_id]->rfdevice.trx_end_func = NULL;
    }

    if (RC.ru[ru_id] && RC.ru[ru_id]->ifdevice.trx_end_func) {
      RC.ru[ru_id]->ifdevice.trx_end_func(&RC.ru[ru_id]->ifdevice);
      RC.ru[ru_id]->ifdevice.trx_end_func = NULL;
    }
  }

  sleep(1); //allow lte-softmodem threads to exit first
  exit(1);
}


static void get_options(void) {
  CONFIG_SETRTFLAG(CONFIG_NOEXITONHELP);
  get_common_options(SOFTMODEM_ENB_BIT );
  CONFIG_CLEARRTFLAG(CONFIG_NOEXITONHELP);

  if ( !(CONFIG_ISFLAGSET(CONFIG_ABORT)) ) {
    memset((void *)&RC,0,sizeof(RC));
    /* Read RC configuration file */
    RCConfig();
    NB_eNB_INST = RC.nb_inst;
    printf("Configuration: nb_rrc_inst %d, nb_L1_inst %d, nb_ru %d\n",NB_eNB_INST,RC.nb_L1_inst,RC.nb_RU);

    if (!IS_SOFTMODEM_NONBIOT) {
      load_NB_IoT();
      printf("               nb_nbiot_rrc_inst %d, nb_nbiot_L1_inst %d, nb_nbiot_macrlc_inst %d\n",
             RC.nb_nb_iot_rrc_inst, RC.nb_nb_iot_L1_inst, RC.nb_nb_iot_macrlc_inst);
    } else {
      printf("All Nb-IoT instances disabled\n");
      RC.nb_nb_iot_rrc_inst=RC.nb_nb_iot_L1_inst=RC.nb_nb_iot_macrlc_inst=0;
    }
  }
}





void set_default_frame_parms(LTE_DL_FRAME_PARMS *frame_parms[MAX_NUM_CCs]) {
  int CC_id;

  for (CC_id=0; CC_id<MAX_NUM_CCs; CC_id++) {
    frame_parms[CC_id] = (LTE_DL_FRAME_PARMS *) malloc(sizeof(LTE_DL_FRAME_PARMS));
    /* Set some default values that may be overwritten while reading options */
    frame_parms[CC_id]->frame_type          = FDD;
    frame_parms[CC_id]->tdd_config          = 3;
    frame_parms[CC_id]->tdd_config_S        = 0;
    frame_parms[CC_id]->N_RB_DL             = 100;
    frame_parms[CC_id]->N_RB_UL             = 100;
    frame_parms[CC_id]->Ncp                 = NORMAL;
    frame_parms[CC_id]->Ncp_UL              = NORMAL;
    frame_parms[CC_id]->Nid_cell            = 0;
    frame_parms[CC_id]->num_MBSFN_config    = 0;
    frame_parms[CC_id]->nb_antenna_ports_eNB  = 1;
    frame_parms[CC_id]->nb_antennas_tx      = 1;
    frame_parms[CC_id]->nb_antennas_rx      = 1;
    frame_parms[CC_id]->nushift             = 0;
    frame_parms[CC_id]->phich_config_common.phich_resource = oneSixth;
    frame_parms[CC_id]->phich_config_common.phich_duration = normal;
    // UL RS Config
    frame_parms[CC_id]->pusch_config_common.ul_ReferenceSignalsPUSCH.cyclicShift = 0;//n_DMRS1 set to 0
    frame_parms[CC_id]->pusch_config_common.ul_ReferenceSignalsPUSCH.groupHoppingEnabled = 0;
    frame_parms[CC_id]->pusch_config_common.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled = 0;
    frame_parms[CC_id]->pusch_config_common.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH = 0;
    frame_parms[CC_id]->prach_config_common.rootSequenceIndex=22;
    frame_parms[CC_id]->prach_config_common.prach_ConfigInfo.zeroCorrelationZoneConfig=1;
    frame_parms[CC_id]->prach_config_common.prach_ConfigInfo.prach_ConfigIndex=0;
    frame_parms[CC_id]->prach_config_common.prach_ConfigInfo.highSpeedFlag=0;
    frame_parms[CC_id]->prach_config_common.prach_ConfigInfo.prach_FreqOffset=0;
    //    downlink_frequency[CC_id][0] = 2680000000; // Use float to avoid issue with frequency over 2^31.
    //    downlink_frequency[CC_id][1] = downlink_frequency[CC_id][0];
    //    downlink_frequency[CC_id][2] = downlink_frequency[CC_id][0];
    //    downlink_frequency[CC_id][3] = downlink_frequency[CC_id][0];
    //printf("Downlink for CC_id %d frequency set to %u\n", CC_id, downlink_frequency[CC_id][0]);
    frame_parms[CC_id]->dl_CarrierFreq=downlink_frequency[CC_id][0];
  }
}

void wait_RUs(void) {
  /* do not modify the following LOG_UI message, which is used by CI */
  LOG_UI(ENB_APP,"Waiting for RUs to be configured ... RC.ru_mask:%02lx\n", RC.ru_mask);
  // wait for all RUs to be configured over fronthaul
  pthread_mutex_lock(&RC.ru_mutex);

  while (RC.ru_mask>0) {
    pthread_cond_wait(&RC.ru_cond,&RC.ru_mutex);
    printf("RC.ru_mask:%02lx\n", RC.ru_mask);
  }

  pthread_mutex_unlock(&RC.ru_mutex);
  LOG_I(PHY,"RUs configured\n");
}

void wait_eNBs(void) {
  int i,j;
  int waiting=1;

  while (waiting==1) {
    printf("Waiting for eNB L1 instances to all get configured ... sleeping 50ms (nb_L1_inst %d)\n",RC.nb_L1_inst);
    usleep(50*1000);
    waiting=0;

    for (i=0; i<RC.nb_L1_inst; i++) {
      printf("RC.nb_L1_CC[%d]:%d\n", i, RC.nb_L1_CC[i]);

      for (j=0; j<RC.nb_L1_CC[i]; j++) {
        if (RC.eNB[i][j]->configured==0) {
          waiting=1;
          break;
        }
      }
    }
  }

  printf("eNB L1 are configured\n");
}


/*
 * helper function to terminate a certain ITTI task
 */
void terminate_task(module_id_t mod_id, task_id_t from, task_id_t to) {
  LOG_I(ENB_APP, "sending TERMINATE_MESSAGE from task %s (%d) to task %s (%d)\n",
        itti_get_task_name(from), from, itti_get_task_name(to), to);
  MessageDef *msg;
  msg = itti_alloc_new_message (from, 0, TERMINATE_MESSAGE);
  itti_send_msg_to_task (to, ENB_MODULE_ID_TO_INSTANCE(mod_id), msg);
}

extern void  free_transport(PHY_VARS_eNB *);
extern void  phy_free_RU(RU_t *);

static void init_pdcp(void) {
  if (!NODE_IS_DU(RC.rrc[0]->node_type)) {
    pdcp_layer_init();
    uint32_t pdcp_initmask = (IS_SOFTMODEM_NOS1) ?
                             (PDCP_USE_NETLINK_BIT | LINK_ENB_PDCP_TO_IP_DRIVER_BIT) : LINK_ENB_PDCP_TO_GTPV1U_BIT;

    if (IS_SOFTMODEM_NOS1)
      pdcp_initmask = pdcp_initmask | ENB_NAS_USE_TUN_BIT | SOFTMODEM_NOKRNMOD_BIT  ;

    pdcp_initmask = pdcp_initmask | ENB_NAS_USE_TUN_W_MBMS_BIT;

    pdcp_module_init(pdcp_initmask, 0);

    if (NODE_IS_CU(RC.rrc[0]->node_type)) {
      pdcp_set_rlc_data_req_func(cu_send_to_du);
    } else {
      pdcp_set_rlc_data_req_func(rlc_data_req);
      pdcp_set_pdcp_data_ind_func(pdcp_data_ind);
    }
  }
}

static  void wait_nfapi_init(char *thread_name) {
  printf( "waiting for NFAPI PNF connection and population of global structure (%s)\n",thread_name);
  pthread_mutex_lock( &nfapi_sync_mutex );

  while (nfapi_sync_var<0)
    pthread_cond_wait( &nfapi_sync_cond, &nfapi_sync_mutex );

  pthread_mutex_unlock(&nfapi_sync_mutex);
  printf( "NFAPI: got sync (%s)\n", thread_name);
}



#ifdef OAI_E2_AGENT
       
static
const int mod_id = 0;
static
const int CC_id = 0;

static
int64_t time_now_us(void)
{
  struct timespec tms;

  /* The C11 way */
  /* if (! timespec_get(&tms, TIME_UTC))  */

  /* POSIX.1-2008 way */
  if (clock_gettime(CLOCK_REALTIME,&tms)) {
    return -1;
  }
  /* seconds, multiplied with 1 million */
  int64_t micros = tms.tv_sec * 1000000;
  /* Add full microseconds */
  micros += tms.tv_nsec/1000;
  /* round up if necessary */
  if (tms.tv_nsec % 1000 >= 500) {
    ++micros;
  }
  return micros;
}

static
void read_mac_sm(mac_ind_msg_t* data)
{
  assert(data != NULL);

  data->tstamp = time_now_us();

  const size_t num_ues = RC.mac[mod_id]->UE_info.num_UEs;

  data->len_ue_stats = num_ues;
  if(data->len_ue_stats > 0){
    data->ue_stats = calloc(data->len_ue_stats, sizeof(mac_ue_stats_impl_t));
    assert( data->ue_stats != NULL && "Memory exhausted" );
  }

  const UE_list_t* ue_list = &RC.mac[mod_id]->UE_info.list;
  size_t i = 0;
  for (int ue_id = ue_list->head; ue_id >= 0; ue_id = ue_list->next[ue_id]) {
    const eNB_UE_STATS* uestats = &RC.mac[mod_id]->UE_info.eNB_UE_stats[CC_id][ue_id];
    const UE_sched_ctrl_t *sched_ctrl = &RC.mac[mod_id]->UE_info.UE_sched_ctrl[ue_id];
    const UE_TEMPLATE *template = &RC.mac[mod_id]->UE_info.UE_template[CC_id][ue_id];
    mac_ue_stats_impl_t* rd = &data->ue_stats[i];

    rd->dl_aggr_tbs = uestats->total_pdu_bytes;
    rd->ul_aggr_tbs = uestats->total_ulsch_TBS;


    rd->rnti = uestats->crnti;
    rd->dl_aggr_prb = uestats->total_rbs_used;
    rd->ul_aggr_prb = uestats->total_rbs_used_rx;
    rd->dl_aggr_retx_prb = uestats->rbs_used_retx;
    rd->ul_aggr_retx_prb = uestats->rbs_used_retx_rx;

    rd->dl_aggr_bytes_sdus = uestats->total_sdu_bytes;
    uint64_t ul_sdu_bytes = 0;
    for (int i = 0; i < NB_RB_MAX; ++i)
      ul_sdu_bytes += uestats->num_bytes_rx[i];
    rd->ul_aggr_bytes_sdus = ul_sdu_bytes;

    rd->dl_aggr_sdus = uestats->num_mac_sdu_tx;
    rd->ul_aggr_sdus = uestats->num_mac_sdu_rx;

    rd->pusch_snr = sched_ctrl->pusch_snr[CC_id];
    rd->pucch_snr = sched_ctrl->pucch1_snr[CC_id];

    rd->wb_cqi = sched_ctrl->dl_cqi[CC_id];
    rd->dl_mcs1 = uestats->dlsch_mcs1;
    rd->ul_mcs1 = uestats->ulsch_mcs1;
    rd->dl_mcs2 = 0;
    rd->ul_mcs2 = 0;
    rd->phr = template->phr_info;

    const uint8_t lcgid = 0; /* below is aggregated value across all LCGIDs */
    const uint32_t bufferSize = template->estimated_ul_buffer - template->scheduled_ul_bytes;
    rd->bsr = bufferSize;

    const size_t numDLHarq = 4;
    rd->dl_num_harq = numDLHarq;
    for (uint8_t j = 0; j < numDLHarq; ++j)
      rd->dl_harq[j] = uestats->dlsch_rounds[j];
    rd->dl_harq[numDLHarq] = uestats->dlsch_errors;

    const size_t numUlHarq = 4;
    rd->ul_num_harq = numUlHarq;
    for (uint8_t j = 0; j < numUlHarq; ++j)
      rd->ul_harq[j] = uestats->ulsch_rounds[j];
    rd->ul_harq[numUlHarq] = uestats->ulsch_errors;

    ++i;
  }
}

static
void read_rlc_sm(rlc_ind_msg_t* data)
{
  assert(data != NULL);

  data->tstamp = time_now_us();

  data->len = 1; // TODO: get the number of active drb
  if(data->len > 0){
    data->rb = calloc(data->len, sizeof(rlc_radio_bearer_stats_t));
    assert(data->rb != NULL && "Memory exhausted");
  }

  const int frame = RC.mac[mod_id]->frame;
  const int subframe = RC.mac[mod_id]->subframe;

  // use MAC structures to get RNTIs
  const UE_info_t* UE_info = &RC.mac[mod_id]->UE_info;
  const UE_list_t* ue_list = &RC.mac[mod_id]->UE_info.list;

  for (int ue_id = ue_list->head; ue_id >= 0; ue_id = ue_list->next[ue_id]) {
    const int lcid = 3;
    const uint16_t rnti = UE_info->eNB_UE_stats[CC_id][ue_id].crnti;
    rlc_radio_bearer_stats_t* sm_rb = &data->rb[0]; // TODO: loop for rb id

    mac_rlc_status_resp_t rlc_stats =
            mac_rlc_status_ind(mod_id, rnti, mod_id, frame, subframe,
                               ENB_FLAG_YES, MBMS_FLAG_NO, lcid, 0, 0);

    sm_rb->txbuf_occ_bytes = rlc_stats.bytes_in_buffer;
    sm_rb->txbuf_occ_pkts = rlc_stats.pdus_in_buffer;

    sm_rb->rnti = rnti;
    sm_rb->rbid = lcid;
  }
}

static
void read_pdcp_sm(pdcp_ind_msg_t* data)
{
  assert(data != NULL);

  data->tstamp = time_now_us();

  data->len = 1; // TODO: get the number of active drb
  if(data->len > 0){
    data->rb = calloc(data->len, sizeof(rlc_radio_bearer_stats_t));
    assert(data->rb != NULL && "Memory exhausted");
  }

  for (int pdcp_uid = 0; pdcp_uid < MAX_MOBILES_PER_ENB; ++pdcp_uid) {
    if (pdcp_enb[0].rnti[pdcp_uid] == 0)
      continue;

    pdcp_radio_bearer_stats_t* rd = &data->rb[0]; // TODO: loop for rb id
    size_t lcid = 3;

    rd->txpdu_pkts = Pdcp_stats_tx[mod_id][pdcp_uid][lcid];
    rd->rxpdu_pkts = Pdcp_stats_rx[mod_id][pdcp_uid][lcid];
    rd->txpdu_bytes = Pdcp_stats_tx_bytes[mod_id][pdcp_uid][lcid];
    rd->rxpdu_bytes = Pdcp_stats_rx_bytes[mod_id][pdcp_uid][lcid];
    rd->txpdu_sn = Pdcp_stats_tx_sn[mod_id][pdcp_uid][lcid];
    rd->rxpdu_sn = Pdcp_stats_rx_sn[mod_id][pdcp_uid][lcid];
    rd->rbid = lcid;
    rd->rnti = pdcp_enb[mod_id].rnti[pdcp_uid];
  }
}

static
void read_slice_conf(slice_conf_t* conf)
{
  assert(conf != NULL);
  eNB_MAC_INST *mac = RC.mac[mod_id];
  assert(mac);

  ///// DL SLICE CONFIG /////
  ul_dl_slice_conf_t* rd_dl = &conf->dl;
  pp_impl_param_t* pp_dl = &RC.mac[mod_id]->pre_processor_dl;

  // Get sched algo
  rd_dl->len_sched_name = strlen(pp_dl->dl_algo.name);
  rd_dl->sched_name = malloc(strlen(pp_dl->dl_algo.name));
  assert(rd_dl->sched_name != NULL && "memory exhausted");
  memcpy(rd_dl->sched_name, pp_dl->dl_algo.name, rd_dl->len_sched_name);

  // Get slice algo
  int algo = mac->pre_processor_dl.algorithm;

  // Get num of slice
  rd_dl->len_slices = pp_dl->slices ? pp_dl->slices->num : 0; // n_dl_slices

  if (rd_dl->len_slices > 0) {
    rd_dl->slices = calloc(rd_dl->len_slices, sizeof(fr_slice_t));
    assert(rd_dl->slices != NULL && "memory exhausted");

    // Get each slice config: id, label, sched algo, slice algo data
    for (uint32_t i = 0; i < rd_dl->len_slices; ++i) {
      slice_t *s = RC.mac[mod_id]->pre_processor_dl.slices->s[i];
      fr_slice_t *rd_slice = &rd_dl->slices[i];

      // id
      rd_dl->slices[i].id = s->id;

      // label
      if (s->label) {
        rd_slice->len_label = strlen(s->label);
        rd_slice->label = malloc(rd_slice->len_label);
        assert(rd_slice->label != NULL && "Memory exhausted");
        memcpy(rd_slice->label, s->label, rd_slice->len_label);
      }

      // sched algo
      rd_slice->len_sched = strlen(s->dl_algo.name);
      rd_slice->sched = malloc(rd_slice->len_sched);
      assert(rd_slice->sched != NULL && "Memory exhausted");
      memcpy(rd_slice->sched, s->dl_algo.name, rd_slice->len_sched);

      // slice algo data
      if (algo == STATIC_SLICING){
        rd_slice->params.type = SLICE_ALG_SM_V0_STATIC;
        static_slice_t* sta = &rd_slice->params.u.sta;
        sta->pos_high = ((static_slice_param_t *)s->algo_data)->posHigh;
        sta->pos_low = ((static_slice_param_t *)s->algo_data)->posLow;
      } else if (algo == NVS_SLICING) {
        rd_slice->params.type = SLICE_ALG_SM_V0_NVS;
        nvs_slice_t* nvs = &rd_slice->params.u.nvs;
        if (((nvs_slice_param_t *)s->algo_data)->type == NVS_RATE) {
          nvs->conf = SLICE_SM_NVS_V0_RATE;
          const float rsvd = ((nvs_slice_param_t *)s->algo_data)->Mbps_reserved;
          const float ref = ((nvs_slice_param_t *)s->algo_data)->Mbps_reference;
          nvs->u.rate.u1.mbps_required = rsvd;
          nvs->u.rate.u2.mbps_reference = ref;
        } else {
          const float rsvd = ((nvs_slice_param_t *)s->algo_data)->pct_reserved;
          nvs->conf = SLICE_SM_NVS_V0_CAPACITY;
          nvs->u.capacity.u.pct_reserved = rsvd;
        }
      } else if (algo == EDF_SLICING) {
        rd_slice->params.type = SLICE_ALG_SM_V0_EDF;
        edf_slice_t* edf = &rd_slice->params.u.edf;
        edf->deadline = ((edf_slice_param_t *)s->algo_data)->deadline;
        edf->guaranteed_prbs = ((edf_slice_param_t *)s->algo_data)->guaranteed_prbs;
        edf->max_replenish = ((edf_slice_param_t *)s->algo_data)->max_replenish;
        edf->len_over = ((edf_slice_param_t *)s->algo_data)->noverride;
        if (edf->len_over > 0) {
          edf->over = calloc(edf->len_over, sizeof(uint32_t));
          assert(edf->over != NULL && "Memory exhausted");
        }
        for (uint32_t j = 0; j < edf->len_over; ++j) {
          edf->over[j] = ((edf_slice_param_t *)s->algo_data)->loverride[j];
        }
      } else if (algo == 0) {
        rd_slice->params.type = SLICE_ALG_SM_V0_NONE;
      } else {
        assert(0 && "Unknow type of DL algo\n");
      }
    }
  }

  ///// TODO: UL SLICE CONFIG /////
  ul_dl_slice_conf_t* read_ul = &conf->ul;
  char const* ulname = "UL SLICE";
  read_ul->len_sched_name = strlen(ulname);
  read_ul->sched_name = malloc(strlen(ulname));
  assert(read_ul->sched_name != NULL && "memory exhausted");
  memcpy(read_ul->sched_name, ulname, strlen(ulname));

}

static
void read_ue_slice_conf(ue_slice_conf_t* conf)
{
  assert(conf != NULL);

  ///// DL UE ASSOCIATED SLCIE /////
  const UE_info_t* ue_info = &RC.mac[mod_id]->UE_info;
  int algo = RC.mac[mod_id]->pre_processor_dl.algorithm;

  conf->len_ue_slice = ue_info->num_UEs;
  if (conf->len_ue_slice > 0) {
    conf->ues = calloc(conf->len_ue_slice, sizeof(ue_slice_assoc_t));
    assert(conf->ues);
  }

  const UE_list_t* ue_list = &ue_info->list;
  const slice_info_t *dl_slices = RC.mac[mod_id]->pre_processor_dl.slices;
  for (int ue_id = ue_list->head; ue_id >= 0; ue_id = ue_list->next[ue_id]) {
    const uint32_t rnti = ue_info->eNB_UE_stats[CC_id][ue_id].crnti;
    conf->ues[ue_id].rnti = rnti;
    if (algo != 0) {
      const int dl_slice_idx = dl_slices->UE_assoc_slice[ue_id];
      if (dl_slice_idx >= 0) {
        const uint32_t dlslice = dl_slices->s[dl_slice_idx]->id;
        conf->ues[ue_id].dl_id = dlslice;
      }
    } else {
      conf->ues[ue_id].dl_id = - 1;
    }
  }

  ///// TODO: UL UE ASSOCIATED SLICE /////

}

static
void read_slice_sm(slice_ind_msg_t* data)
{
  assert(data != NULL);

  data->tstamp = time_now_us();

  read_slice_conf(&data->slice_conf);
  read_ue_slice_conf(&data->ue_slice_conf);
}

static
void set_new_dl_slice_algo(slice_algorithm_e algo)
{
  eNB_MAC_INST *mac = RC.mac[mod_id];
  assert(mac);

  pp_impl_param_t dl = mac->pre_processor_dl;
  switch (algo) {
    case SLICE_ALG_SM_V0_STATIC:
      mac->pre_processor_dl = static_dl_init(mod_id, CC_id);
      break;
    case SLICE_ALG_SM_V0_NVS:
      mac->pre_processor_dl = nvs_dl_init(mod_id, CC_id);
      break;
    case SLICE_ALG_SM_V0_EDF:
      mac->pre_processor_dl = edf_dl_init(mod_id, CC_id);
      break;
    default:
      mac->pre_processor_dl.algorithm = 0;
      mac->pre_processor_dl.dl = dlsch_scheduler_pre_processor;
      mac->pre_processor_dl.dl_algo.data = mac->pre_processor_dl.dl_algo.setup();
      mac->pre_processor_dl.slices = NULL;
      break;
  }
  if (dl.slices)
    dl.destroy(&dl.slices);
  if (dl.dl_algo.data)
    dl.dl_algo.unset(&dl.dl_algo.data);
}

static
int add_mod_dl_slice(slice_algorithm_e current_algo, fr_slice_t const* slice)
{
  void *params = NULL;
  char *slice_algo = NULL;
  if (current_algo == SLICE_ALG_SM_V0_STATIC) {
    assert(current_algo == SLICE_ALG_SM_V0_STATIC);
    slice_algo = strdup("STATIC");
    // TODO: this should be copied inside addmod_slice() to avoid unnecessary
    // copies, but reuse the old code for the moment
    params = malloc(sizeof(static_slice_param_t));
    if (!params) return -1;
    ((static_slice_param_t *)params)->posLow = slice->params.u.sta.pos_low;
    ((static_slice_param_t *)params)->posHigh = slice->params.u.sta.pos_high;
  } else if (current_algo == SLICE_ALG_SM_V0_NVS) {
    assert(current_algo == SLICE_ALG_SM_V0_NVS);
    params = malloc(sizeof(nvs_slice_param_t));
    if (!params) return -1;
    if (slice->params.u.nvs.conf == SLICE_SM_NVS_V0_RATE) {
      slice_algo = strdup("NVS_RATE");
      ((nvs_slice_param_t *)params)->type = NVS_RATE;
      ((nvs_slice_param_t *)params)->Mbps_reserved = slice->params.u.nvs.u.rate.u1.mbps_required;
      ((nvs_slice_param_t *)params)->Mbps_reference = slice->params.u.nvs.u.rate.u2.mbps_reference;
    } else {
      assert(slice->params.u.nvs.conf == SLICE_SM_NVS_V0_CAPACITY);
      slice_algo = strdup("NVS_CAPACITY");
      ((nvs_slice_param_t *)params)->type = NVS_RES;
      ((nvs_slice_param_t *)params)->pct_reserved = slice->params.u.nvs.u.capacity.u.pct_reserved;
    }
  } else if (current_algo == SLICE_ALG_SM_V0_EDF) {
    assert(current_algo == SLICE_ALG_SM_V0_EDF);
    slice_algo = strdup("EDF");
    params = malloc(sizeof(edf_slice_param_t));
    if (!params) return -1;
    ((edf_slice_param_t *)params)->deadline = slice->params.u.edf.deadline;
    ((edf_slice_param_t *)params)->guaranteed_prbs = slice->params.u.edf.guaranteed_prbs;
    ((edf_slice_param_t *)params)->max_replenish = slice->params.u.edf.max_replenish;
    ((edf_slice_param_t *)params)->noverride = slice->params.u.edf.len_over;
    for (int i = 0; i < ((edf_slice_param_t *)params)->noverride; ++i)
      ((edf_slice_param_t *)params)->loverride[i] = slice->params.u.edf.over[i];
  } else {
    assert(0 != 0 && "Unknow current_algo");
  }

  pp_impl_param_t *dl = &RC.mac[mod_id]->pre_processor_dl;
  void *algo = &dl->dl_algo;
  char *ue_algo = NULL;
  if (slice->sched) {
    if (!strcmp(slice->sched, "RR"))
      ue_algo = strdup("round_robin_dl");
    else if (!strcmp(slice->sched, "PF"))
      ue_algo = strdup("proportional_fair_wbcqi_dl");
    else if (!strcmp(slice->sched, "MT"))
      ue_algo = strdup("maximum_throughput_wbcqi_dl");
    else
      LOG_E(MAC, "unknow scheduler '%s'\n", slice->sched);

    algo = dlsym(NULL, ue_algo);

    if (!algo) {
      free(params);
      LOG_E(MAC, "cannot locate scheduler '%s'\n", slice->sched);
      return -15;
    }
  }

  char *l = NULL;
  if (slice->label)
    l = strdup(slice->label);
  uint8_t sid = slice->id;
  LOG_W(MAC, "add DL slice id %d, label %s, slice sched algo %s, ue sched algo %s\n", sid, l, slice_algo, ue_algo);
  return dl->addmod_slice(dl->slices, sid, l, algo, params);
}

static
int find_dl_slice(uint32_t id)
{
  slice_info_t *si = RC.mac[mod_id]->pre_processor_dl.slices;
  for (int i = 0; i < si->num; ++i)
    if (si->s[i]->id == id)
      return i;
  return -1;
}

static
int del_dl_slice(uint32_t id)
{
  const int idx = find_dl_slice(id);
  if (idx < 0)
    return -1;
  pp_impl_param_t *dl = &RC.mac[mod_id]->pre_processor_dl;
  return dl->remove_slice(dl->slices, idx);
}

static
int get_ue_id(uint16_t rnti)
{
  const UE_info_t* ue_info = &RC.mac[mod_id]->UE_info;
  const UE_list_t* ue_list = &ue_info->list;
  for (int ue_id = ue_list->head; ue_id >= 0; ue_id = ue_list->next[ue_id]) {
    if (ue_info->UE_template[CC_id][ue_id].rnti == rnti)
      return ue_id;
  }
  return -1;
}

static
int assoc_ue_to_dl_slice(uint16_t rnti, uint32_t id)
{
  int idx = find_dl_slice(id);
  if (idx < 0)
    return -100;
  int ue_id = get_ue_id(rnti);
  if (ue_id < 0)
    return -101;

  pp_impl_param_t *dl = &RC.mac[mod_id]->pre_processor_dl;
  LOG_W(MAC, "associate UE RNTI 0x%04x from slice ID %d idx %d to slice ID %d idx %d\n",
        rnti, dl->slices->UE_assoc_slice[ue_id], find_dl_slice(dl->slices->UE_assoc_slice[ue_id]), id, idx);
  dl->move_UE(dl->slices, ue_id, idx);
  return 0;
}

static
sm_ag_if_ans_e write_slice_sm(slice_ctrl_msg_t const* data)
{
  assert(data != NULL);

  /// ADD MOD ///
  if (data->type == SLICE_CTRL_SM_V0_ADD) {
    eNB_MAC_INST *mac = RC.mac[mod_id];
    assert(mac);
    /// GET DL SLICE CONTROL INFO ///
    slice_conf_t const* wr = &data->u.add_mod_slice;
    size_t slices_len = wr->dl.len_slices;

    if (slices_len >= 0) {
      int current_algo = mac->pre_processor_dl.algorithm;
      int new_algo = -1;
      if (slices_len > 0)
        new_algo = wr->dl.slices[0].params.type;
      pthread_mutex_lock(&mac->pp_dl_mutex);
      if (current_algo != new_algo) {
        set_new_dl_slice_algo(new_algo);
        current_algo = new_algo;
        if (new_algo > 0)
          LOG_D(NR_MAC, "set new algorithm %d\n", current_algo);
        else
          LOG_W(NR_MAC, "reset slicing algorithm as NONE\n");
      }

      for (size_t i = 0; i < slices_len; ++i) {
        const int rc = add_mod_dl_slice(current_algo, &wr->dl.slices[i]);
        if (rc < 0) {
          pthread_mutex_unlock(&mac->pp_dl_mutex);
          LOG_E(MAC, "error code %d while updating slices\n", rc);
          return SLICE_CTRL_OUT_ERROR;
        }
      }
      pthread_mutex_unlock(&mac->pp_dl_mutex);
      return SLICE_CTRL_OUT_OK;
    } else {
      assert(0 != 0 && "slices_len <= 0");
      return SLICE_CTRL_OUT_ERROR;
    }
  } else if (data->type == SLICE_CTRL_SM_V0_DEL) {
    /// DEL ///
    eNB_MAC_INST *mac = RC.mac[mod_id];
    assert(mac);
    if (mac->pre_processor_dl.algorithm <= 0) {
      LOG_E(MAC, "current slice algo is NONE, no slice can be deleted\n");
      return SLICE_CTRL_OUT_ERROR;
    }
    /// GET DL SLICE CONTROL INFO ///
    del_slice_conf_t const* wr = &data->u.del_slice;
    uint32_t* dl_ids = wr->dl;
    size_t n_dl_ids = wr->len_dl;
    assert(n_dl_ids != 0 && "del_slice->len_dl == 0");

    pthread_mutex_lock(&mac->pp_dl_mutex);
    for (size_t i = 0; i < n_dl_ids; ++i) {
      LOG_W(MAC, "attempt to delete slice ID %d\n", dl_ids[i]);
      const int rc = del_dl_slice(dl_ids[i]);
      if (rc < 0) {
        pthread_mutex_unlock(&mac->pp_dl_mutex);
        LOG_E(MAC, "error code %d while deleting slice ID %d\n", rc, dl_ids[i]);
        return SLICE_CTRL_OUT_ERROR;
      }
    }
    pthread_mutex_unlock(&mac->pp_dl_mutex);
    return SLICE_CTRL_OUT_OK;
  } else if (data->type == SLICE_CTRL_SM_V0_UE_SLICE_ASSOC) {
    /// ASSOC SLICE ///
    eNB_MAC_INST *mac = RC.mac[mod_id];
    assert(mac);
    if (mac->pre_processor_dl.algorithm <= 0) {
      LOG_E(NR_MAC, "current slice algo is NONE, no UE can be associated\n");
      return SLICE_CTRL_OUT_ERROR;
    }
    if (mac->pre_processor_dl.slices->num <= 0) {
      LOG_E(NR_MAC, "no UE connected\n");
      return SLICE_CTRL_OUT_ERROR;
    }
    /// GET DL SLICE CONTROL INFO ///
    ue_slice_conf_t const* wr = &data->u.ue_slice;
    ue_slice_assoc_t* new_ues = wr->ues;
    size_t n_new_ues = wr->len_ue_slice;
    assert(n_new_ues != 0 && "ue_slice->len_ue_slice == 0");

    pthread_mutex_lock(&mac->pp_dl_mutex);
    for (size_t i = 0; i < n_new_ues; ++i) {
      // uint16_t rnti = new_ues[i].rnti;
      uint16_t rnti = mac->UE_info.eNB_UE_stats[CC_id][0].crnti; // current FlexRIC can not pass the rnti info by ctrl req
      if (new_ues[i].ul_id)
        LOG_W(MAC, "ignoring UL slice association for RNTI %04x\n", rnti);
      uint32_t new_idx = new_ues[i].dl_id;
      int ue_id = get_ue_id(rnti);
      if (ue_id < 0) {
        pthread_mutex_unlock(&mac->pp_dl_mutex);
        LOG_E(MAC, "no ue connected, ue_id %d\n", ue_id);
        return SLICE_CTRL_OUT_ERROR;
      }
      int cur_idx = slicing_get_UE_slice_idx(mac->pre_processor_dl.slices, ue_id);
      if (new_idx == cur_idx) {
        pthread_mutex_unlock(&mac->pp_dl_mutex);
        LOG_E(MAC, "expected DL slice association for UE RNTI %04x\n", rnti);
        return SLICE_CTRL_OUT_ERROR;
      }
      int rc = assoc_ue_to_dl_slice(rnti, new_idx);
      if (rc < 0) {
        pthread_mutex_unlock(&mac->pp_dl_mutex);
        LOG_E(MAC, "error code %d while associating RNTI %04x\n", rc, rnti);
        return SLICE_CTRL_OUT_ERROR;
      }
    }
    pthread_mutex_unlock(&mac->pp_dl_mutex);
    return SLICE_CTRL_OUT_OK;
  } else {
    assert(0 != 0 && "Unknow slice ctrl type");
  }
}

static
void read_RAN(sm_ag_if_rd_t* data)
{
  assert(data != NULL);
  assert(data->type == MAC_STATS_V0
         || data->type == RLC_STATS_V0
         || data->type == PDCP_STATS_V0
         || data->type == SLICE_STATS_V0
  );

  if(data->type == MAC_STATS_V0){
    read_mac_sm(&data->mac_stats.msg);
    //  printf("Calling READ MAC\n");
  }else if(data->type == RLC_STATS_V0) {
    read_rlc_sm(&data->rlc_stats.msg);
    //  printf("Calling READ RLC\n");
  } else if(data->type == PDCP_STATS_V0) {
    read_pdcp_sm(&data->pdcp_stats.msg);
    //  printf("Calling READ PDCP\n");
  } else if(data->type == SLICE_STATS_V0) {
    read_slice_sm(&data->slice_stats.msg);
    //  printf("Calling READ SLICE\n");
  } else {
    assert(0!=0 && "Unknown data type!");
  }
}

static
sm_ag_if_ans_t write_RAN(sm_ag_if_wr_t const* data)
{
  assert(data != NULL);
  if(data->type == SLICE_CTRL_REQ_V0){
    sm_ag_if_ans_t ans = {.type = SLICE_AGENT_IF_CTRL_ANS_V0};
    ans.slice.ans = write_slice_sm(&data->slice_req_ctrl.msg);
    return ans;
  } else {
    assert(0 != 0 && "Not supported function ");
  }

  sm_ag_if_ans_t ans = {0};
  return ans;
}

#endif // OAI_E2_AGENT

int main ( int argc, char **argv )
{
  int CC_id = 0;
  int ru_id;
  int node_type = ngran_eNB;

  start_background_system();

  if ( load_configmodule(argc,argv,0) == NULL) {
    exit_fun("[SOFTMODEM] Error, configuration module init failed\n");
  }

  mode = normal_txrx;
  logInit();
  set_latency_target();
  printf("Reading in command-line options\n");
  get_options ();

  EPC_MODE_ENABLED = !IS_SOFTMODEM_NOS1;

  if (CONFIG_ISFLAGSET(CONFIG_ABORT) ) {
    fprintf(stderr,"Getting configuration failed\n");
    exit(-1);
  }

#if T_TRACER
  T_Config_Init();
#endif
  //randominit (0);
  set_taus_seed (0);
  printf("configuring for RAU/RRU\n");

  if (opp_enabled ==1) {
    reset_opp_meas();
  }

  cpuf=get_cpu_freq_GHz();
  printf("ITTI init, useMME: %i\n",EPC_MODE_ENABLED);
  itti_init(TASK_MAX, tasks_info);

  init_opt();
  // to make a graceful exit when ctrl-c is pressed
  set_softmodem_sighandler();
#ifndef PACKAGE_VERSION
#  define PACKAGE_VERSION "UNKNOWN-EXPERIMENTAL"
#endif
  LOG_I(HW, "Version: %s\n", PACKAGE_VERSION);

  /* Read configuration */
  if (RC.nb_inst > 0) {
    read_config_and_init();
  } else {
    printf("RC.nb_inst = 0, Initializing L1\n");
    RCconfig_L1();
  }

  if (RC.nb_inst > 0) {

#ifdef OAI_E2_AGENT

//////////////////////////////////
//////////////////////////////////
//// Init the E2 Agent

    sleep(2);
    const eNB_RRC_INST* rrc = RC.rrc[mod_id];
    assert(rrc != NULL && "rrc cannot be NULL");

    const int mcc = rrc->configuration.mcc[0]; // 208;
    const int mnc = rrc->configuration.mnc[0]; // 94;
    const int mnc_digit_len = rrc->configuration.mnc_digit_length[0]; // 2;
    const int nb_id = rrc->configuration.cell_identity; //42;
    // TODO: node_type = 0 // ngran_eNB
    const int cu_du_id = 0;
    sm_io_ag_t io = {.read = read_RAN, .write = write_RAN};
    printf("[E2 NODE]: mcc = %d mnc = %d mnc_digit = %d nd_id = %d \n", mcc, mnc, mnc_digit_len, nb_id);

    int const agent_argc = 1;
    char** agent_argv = NULL;
    fr_args_t ric_args = init_fr_args(agent_argc, agent_argv);
    // TODO: integrate with oai config
    char* conf_dir = getenv("FLEXRIC_CONF");
    char* lib_dir = getenv("FLEXRIC_LIB_DIR");

    if (conf_dir != NULL)
      strcpy(ric_args.conf_file, conf_dir);
    else
      strcpy(ric_args.conf_file, "/usr/local/etc/flexric/flexric.conf");
    if (lib_dir != NULL)
      strcpy(ric_args.libs_dir, lib_dir);
    else
      strcpy(ric_args.libs_dir, "/usr/local/lib/flexric/");

    init_agent_api( mcc, mnc, mnc_digit_len, nb_id, cu_du_id, 0, io, &ric_args);
//////////////////////////////////
//////////////////////////////////
#endif //  OAI_E2_AGENT 
       
    /* initializes PDCP and sets correct RLC Request/PDCP Indication callbacks
     * for monolithic/F1 modes */
   init_pdcp();
    
    if (create_tasks(1) < 0) {
      printf("cannot create ITTI tasks\n");
      exit(-1);
    }

    for (int enb_id = 0; enb_id < RC.nb_inst; enb_id++) {
      MessageDef *msg_p = itti_alloc_new_message (TASK_ENB_APP, 0, RRC_CONFIGURATION_REQ);
      RRC_CONFIGURATION_REQ(msg_p) = RC.rrc[enb_id]->configuration;
      itti_send_msg_to_task (TASK_RRC_ENB, ENB_MODULE_ID_TO_INSTANCE(enb_id), msg_p);
    }
    node_type = RC.rrc[0]->node_type;
  }

  if (RC.nb_inst > 0 && NODE_IS_CU(node_type)) {
    protocol_ctxt_t ctxt;
    ctxt.module_id = 0 ;
    ctxt.instance = 0;
    ctxt.rntiMaybeUEid = 0;
    ctxt.enb_flag = 1;
    ctxt.frame = 0;
    ctxt.subframe = 0;
    pdcp_run(&ctxt);
  }

  /* start threads if only L1 or not a CU */
  if (RC.nb_inst == 0 || !NODE_IS_CU(node_type) || NFAPI_MODE == NFAPI_MODE_PNF || NFAPI_MODE == NFAPI_MODE_VNF) {
    // init UE_PF_PO and mutex lock
    pthread_mutex_init(&ue_pf_po_mutex, NULL);
    memset (&UE_PF_PO[0][0], 0, sizeof(UE_PF_PO_t)*MAX_MOBILES_PER_ENB*MAX_NUM_CCs);
    mlockall(MCL_CURRENT | MCL_FUTURE);
    pthread_cond_init(&sync_cond,NULL);
    pthread_mutex_init(&sync_mutex, NULL);

    rt_sleep_ns(10*100000000ULL);

    if (NFAPI_MODE!=NFAPI_MONOLITHIC) {
      LOG_I(ENB_APP,"NFAPI*** - mutex and cond created - will block shortly for completion of PNF connection\n");
      pthread_cond_init(&sync_cond,NULL);
      pthread_mutex_init(&sync_mutex, NULL);
    }

    if (NFAPI_MODE==NFAPI_MODE_VNF) {// VNF
#if defined(PRE_SCD_THREAD)
      init_ru_vnf();  // ru pointer is necessary for pre_scd.
#endif
      wait_nfapi_init("main?");
    }

    LOG_I(ENB_APP,"START MAIN THREADS\n");
    // start the main threads
    number_of_cards = 1;
    printf("RC.nb_L1_inst:%d\n", RC.nb_L1_inst);

    if (RC.nb_L1_inst > 0) {
      printf("Initializing eNB threads single_thread_flag:%d wait_for_sync:%d\n", get_softmodem_params()->single_thread_flag,get_softmodem_params()->wait_for_sync);
      init_eNB(get_softmodem_params()->single_thread_flag,get_softmodem_params()->wait_for_sync);
    }
    for (int x=0; x < RC.nb_L1_inst; x++) 
      for (int CC_id=0; CC_id<RC.nb_L1_CC[x]; CC_id++) {
        L1_rxtx_proc_t *L1proc= &RC.eNB[x][CC_id]->proc.L1_proc;
        L1_rxtx_proc_t *L1proctx= &RC.eNB[x][CC_id]->proc.L1_proc_tx;
        L1proc->threadPool = (tpool_t *)malloc(sizeof(tpool_t));
        L1proc->respDecode=(notifiedFIFO_t*) malloc(sizeof(notifiedFIFO_t));
        if ( strlen(get_softmodem_params()->threadPoolConfig) > 0 )
         initTpool(get_softmodem_params()->threadPoolConfig, L1proc->threadPool, true);
        else
          initTpool("n", L1proc->threadPool, true);
        initNotifiedFIFO(L1proc->respDecode);
        L1proctx->threadPool = L1proc->threadPool;
    }


  }

  printf("wait_eNBs()\n");
  wait_eNBs();
  printf("About to Init RU threads RC.nb_RU:%d\n", RC.nb_RU);
  
  // RU thread and some L1 procedure aren't necessary in VNF or L2 FAPI simulator.
  // but RU thread deals with pre_scd and this is necessary in VNF and simulator.
  // some initialization is necessary and init_ru_vnf do this.
  if (RC.nb_RU > 0 && NFAPI_MODE != NFAPI_MODE_VNF) {
    RC.ru = RCconfig_RU(RC.nb_RU, RC.nb_L1_inst, RC.eNB, &RC.ru_mask, &RC.ru_mutex, &RC.ru_cond);
    LOG_I(PHY,
          "number of L1 instances %d, number of RU %d, number of CPU cores %d: Initializing RU threads\n",
          RC.nb_L1_inst, RC.nb_RU, get_nprocs());
    init_RU(RC.ru,RC.nb_RU,RC.eNB,RC.nb_L1_inst,RC.nb_L1_CC,get_softmodem_params()->rf_config_file,get_softmodem_params()->send_dmrs_sync);
    
    for (ru_id=0; ru_id<RC.nb_RU; ru_id++) {
      RC.ru[ru_id]->rf_map.card=0;
      RC.ru[ru_id]->rf_map.chain=CC_id+(get_softmodem_params()->chain_offset);
    }

    config_sync_var=0;

    if (NFAPI_MODE==NFAPI_MODE_PNF) { // PNF
      wait_nfapi_init("main?");
    }

    printf("wait RUs\n");
    // end of CI modifications
    fflush(stdout);
    fflush(stderr);
    // wait_RUs() is wrong and over complex!
    wait_RUs();
    LOG_I(ENB_APP,"RC.nb_RU:%d\n", RC.nb_RU);
    // once all RUs are ready intiailize the rest of the eNBs ((dependence on final RU parameters after configuration)
    printf("ALL RUs ready - init eNBs\n");
    
    if (NFAPI_MODE!=NFAPI_MODE_PNF && NFAPI_MODE!=NFAPI_MODE_VNF) {
      LOG_I(ENB_APP,"Not NFAPI mode - call init_eNB_afterRU()\n");
      init_eNB_afterRU();
    } else {
      LOG_I(ENB_APP,"NFAPI mode - DO NOT call init_eNB_afterRU()\n");
    }

    LOG_UI(ENB_APP,"ALL RUs ready - ALL eNBs ready\n");
    // connect the TX/RX buffers
    sleep(1); /* wait for thread activation */
    LOG_I(ENB_APP,"Sending sync to all threads\n");
    pthread_mutex_lock(&sync_mutex);
    sync_var=0;
    pthread_cond_broadcast(&sync_cond);
    pthread_mutex_unlock(&sync_mutex);
    config_check_unknown_cmdlineopt(CONFIG_CHECKALLSECTIONS);
  }

  create_tasks_mbms(1);

  // wait for end of program
  LOG_UI(ENB_APP,"TYPE <CTRL-C> TO TERMINATE\n");
  // CI -- Flushing the std outputs for the previous marker to show on the eNB / DU / CU log file
  fflush(stdout);
  fflush(stderr);
  // end of CI modifications
  //getchar();
  if(IS_SOFTMODEM_DOSCOPE)
     load_softscope("enb",NULL);
  itti_wait_tasks_end();

#if USING_GPROF
  // Save the gprof data now (rather than via atexit) in case we crash while shutting down
  fprintf(stderr, "Recording gprof data...\n");
  _mcleanup();
  fprintf(stderr, "Recording gprof data...done\n");
#endif // USING_GPROF

  oai_exit=1;
  LOG_I(ENB_APP,"oai_exit=%d\n",oai_exit);
  // stop threads

  #if 0 //Disable clean up because this tends to crash (and unnecessary)
  if (RC.nb_inst == 0 || !NODE_IS_CU(node_type)) {
    if(IS_SOFTMODEM_DOSCOPE)
      end_forms();

    LOG_I(ENB_APP,"stopping MODEM threads\n");
    stop_eNB(NB_eNB_INST);
    stop_RU(RC.nb_RU);

    /* release memory used by the RU/eNB threads (incomplete), after all
     * threads have been stopped (they partially use the same memory) */
    for (int inst = 0; inst < NB_eNB_INST; inst++) {
      for (int cc_id = 0; cc_id < RC.nb_CC[inst]; cc_id++) {
	free_transport(RC.eNB[inst][cc_id]);
        phy_free_lte_eNB(RC.eNB[inst][cc_id]);
      }
    }

    for (int inst = 0; inst < RC.nb_RU; inst++) {
      phy_free_RU(RC.ru[inst]);
    }

    free_lte_top();
    end_configmodule();
    pthread_cond_destroy(&sync_cond);
    pthread_mutex_destroy(&sync_mutex);
    pthread_cond_destroy(&nfapi_sync_cond);
    pthread_mutex_destroy(&nfapi_sync_mutex);
    pthread_mutex_destroy(&ue_pf_po_mutex);

    for(ru_id=0; ru_id<RC.nb_RU; ru_id++) {
      if (RC.ru[ru_id]->rfdevice.trx_end_func) {
        RC.ru[ru_id]->rfdevice.trx_end_func(&RC.ru[ru_id]->rfdevice);
        RC.ru[ru_id]->rfdevice.trx_end_func = NULL;
      }

      if (RC.ru[ru_id]->ifdevice.trx_end_func) {
        RC.ru[ru_id]->ifdevice.trx_end_func(&RC.ru[ru_id]->ifdevice);
        RC.ru[ru_id]->ifdevice.trx_end_func = NULL;
      }
    }
  }
  #endif

  pdcp_module_cleanup();
  terminate_opt();
  logClean();
  printf("Bye.\n");
  return 0;
}
