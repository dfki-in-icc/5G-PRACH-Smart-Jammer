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
/*
 * This file implements the cpu statistics for Level 1 in gnb and in UE. 
 * Caveats: Note that we did not merge this development into file telnetsrv_gnb_measurements.c and telnetsrv_5Gue_measurements.c 
 * belonging to telnetsrv shared library because we needed to provide an API for main programs exectuable without inserting a new 
 * dependency on the telnetsrv shared library.
 */
#define _GNU_SOURCE 
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <linux/sched.h>

#include "common/utils/LOG/log.h"
#include "common/utils/threadPool/thread-pool.h"
#include "cpustats.h"
#include "common/utils/time_meas.h"

#define UEL1_THREADNAME   "L1_UE_stats_act"
#define GNBL1_THREADNAME  "L1_stats_act"

/* ====  Global variables ==== */

// IPC communication between main thread and the statistics one
static notifiedFIFO_t GNBL1cpustats_fifo, UEL1cpustats_fifo; 

static pthread_t UEL1cpustats_threadid  = 0; 
static pthread_t gNBL1cpustats_threadid = 0; 

// data structure for communication between main thread and the statistics one 
typedef struct {
  unsigned int msgid;
  telnet_printfunc_t prnt;
} proto_stats_msg_t; 

// PHY_vars_UE_g: declared in openair1/PHY/phy_vars_nr_ue.h and defined in nr-uesoftmodem.c 
extern PHY_VARS_NR_UE *** PHY_vars_UE_g;

/* ====  Static functions  ==== */

/* return pointer to the global structure that contains the UE data including statistics*/
static PHY_VARS_NR_UE * getglobal_uephy(void) {   return PHY_vars_UE_g[0][0]; }

// RC.nb_nr_macrlc_inst in openair2/LAYER2/NR_MAC_gNB/main.c defines the RAN_CONTEXT RC variable that defines how many instances you will have
// ru stands for 'radio unit' data structure
static RU_t *getglobal_ru(void) { return RC.ru[0];}
static void UEL1cpustats_display(telnet_printfunc_t prnt);
static void UEL1cpustats_thread_main(void);
static void gNBL1cpustats_display(telnet_printfunc_t prnt, PHY_VARS_gNB *param);
static void gNBL1cpustats_thread_main (void * param);

/* xxx: these are the same statistics but with different formatting and list of content of those displayed by 
 * telnetsrv_measurements.c:measurcmd_display_cpumeasures(). you could think about unifying the 2 functions. 
 * XXX: this implementation should be fixed using a dynamically allocated buffer instead of a plain one 
 * it is prone to sigSEGV
 */
static void 
UEL1cpustats_display(telnet_printfunc_t prnt)
{
  /* PHY_vars_UE_g: 
   * this global variable is allocated by nr-uesoftmodem.c and shared among threads makes the trick of passing statistics data to statistic thread
   * that will consume it. There is just one instance of UE created in nr-ue.c
   */
  PHY_VARS_NR_UE *ue = getglobal_uephy();
  
  prnt("Display CPU statistics for UE Level 1\n");

  char output[TELNET_MAX_MSGLENGTH];
  int stroff = 0;
  stroff += print_meas_log(&ue->phy_proc_tx                 , "L1 TX processing"    , NULL, NULL, output         , TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->ulsch_encoding_stats        , "ULSCH encoding"      , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff );
  stroff += print_meas_log(&ue->phy_proc_rx[0]              , "L1 RX processing t0" , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->phy_proc_rx[1]              , "L1 RX processing t1" , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->ue_ul_indication_stats      , "UL Indication"       , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->rx_pdsch_stats              , "PDSCH receiver"      , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_decoding_stats[0]     , "PDSCH decoding t0"   , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_decoding_stats[1]     , "PDSCH decoding t1"   , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_deinterleaving_stats  , " -> Deinterleive"    , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_rate_unmatching_stats , " -> Rate Unmatch"    , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_ldpc_decoding_stats   , " ->  LDPC Decode"    , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_unscrambling_stats    , "PDSCH unscrambling"  , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&ue->dlsch_rx_pdcch_stats        , "PDCCH handling"      , NULL, NULL, output + stroff, TELNET_MAX_MSGLENGTH - stroff);
  
  prnt("%s\n", output);
}

static void 
UEL1cpustats_thread_main(void)
{
  if (pthread_setname_np(pthread_self(), UEL1_THREADNAME))
    LOG_E(UTIL, "Error setting name on thread %s: %s\n", UEL1_THREADNAME, strerror(errno));
  
  struct sched_param schedp;
  schedp.sched_priority = SCHED_NORMAL;
  if (pthread_setschedparam(pthread_self(), SCHED_IDLE, &schedp))
    LOG_E(UTIL, "Error setting priority in thread %s: %s\n", UEL1_THREADNAME, strerror(errno));
    
  initNotifiedFIFO(&UEL1cpustats_fifo);
  
  cpumeas(CPUMEAS_ENABLE);
  
  while (1){
    /* 
     * Infinite loop that waits for a message on the FIFO. 
     * Behind the scenes it uses blocking pthread.h:pthread_cond_wait()
     */
    notifiedFIFO_elt_t *msg = pullNotifiedFIFO(&UEL1cpustats_fifo); 

    proto_stats_msg_t *tsm = (proto_stats_msg_t *)NotifiedFifoData(msg);
    switch (tsm->msgid)
    {
      case TIMESTAT_MSGID_DISPLAY:
        
        if (cpumeas(CPUMEAS_GETSTATE))
          UEL1cpustats_display(tsm->prnt);
        else
          tsm->prnt("ERR: before displaying meaningful stats, you need to activate their generation\n");
        break;
      case TIMESTAT_MSGID_DISABLE:
        cpumeas(CPUMEAS_DISABLE);
        UEL1cpustats_threadid = 0;
        pthread_exit(NULL);
        break;
    }
    delNotifiedFIFO_elt(msg);
  }
  LOG_I(UTIL, "Exiting thread %s\n", UEL1_THREADNAME);
}

bool 
UEL1cpustats_enable(void)
{
  if (UEL1cpustats_threadid){
    LOG_E(UTIL, "Thread '%s' is already running. Aborting your request to create a new one\n", UEL1_THREADNAME);
    return false;
  }
  
  int rt = pthread_create(&UEL1cpustats_threadid, NULL, (void *(*)(void *))UEL1cpustats_thread_main, NULL);
  if (rt != 0)
    LOG_E(UTIL, "Error creating thread %s: %s\n",UEL1_THREADNAME, strerror(errno));

  return (rt != 0) ? false : true;
}

void 
UEL1cpustats_disable(void)
{
  if (!UEL1cpustats_threadid) {
    LOG_W(UTIL, "Thread '%s' is not running. Discarding your request to disable stats\n", UEL1_THREADNAME);
    return;
  }

  notifiedFIFO_elt_t *nfe = newNotifiedFIFO_elt(sizeof(time_stats_msg_t), 0, NULL, NULL);
	proto_stats_msg_t *msg = (proto_stats_msg_t *)NotifiedFifoData(nfe);
  msg->msgid = TIMESTAT_MSGID_DISABLE;
  pushNotifiedFIFO(&UEL1cpustats_fifo, nfe);
}

void 
UEL1cpustats_measurcmd_display (telnet_printfunc_t prnt)
{
  if (!UEL1cpustats_threadid) {
    prnt("Thread '%s' is not running. Aborting your request to display stats, you need first to enable statistics\n", UEL1_THREADNAME);
    return;
  }
  
  notifiedFIFO_elt_t *nfe = newNotifiedFIFO_elt(sizeof(time_stats_msg_t), 0, NULL, NULL);
	proto_stats_msg_t *msg = (proto_stats_msg_t *)NotifiedFifoData(nfe);
  msg->prnt  = prnt;
  msg->msgid = TIMESTAT_MSGID_DISPLAY;
  pushNotifiedFIFO(&UEL1cpustats_fifo, nfe);
}

/* Display info from specific instance of gNB and from radio unit */
static void 
gNBL1cpustats_display(telnet_printfunc_t prnt, PHY_VARS_gNB *gNB)
{
  RU_t *ru = getglobal_ru();

  char output[TELNET_MAX_MSGLENGTH]; 
  int stroff = 0;
  prnt("%s\n");
  // XXX: header will be printed automatically by print_meas_log() just the first time it is called in the program. It means that if you
  // use a mechanism like 'loop measur show gnb_L1', you will lose the header after the first iteration.
  // You should fix the logic of 'print_meas_log() to avoid it printing any header.
  stroff += print_meas_log(&gNB->phy_proc_tx            , "L1 Tx processing"    , NULL, NULL, output       , TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->dlsch_encoding_stats   , "DLSCH encoding"      , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->phy_proc_rx            , "L1 Rx processing"    , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->ul_indication_stats    , "UL Indication"       , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->rx_pusch_stats         , "PUSCH inner-receiver", NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->ulsch_decoding_stats   , "PUSCH decoding"      , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  stroff += print_meas_log(&gNB->schedule_response_stats, "Schedule Response"   , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  if (ru->feprx) 
    stroff += print_meas_log(&ru->ofdm_demod_stats      , "feprx"               , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);

  if (ru->feptx_ofdm) {
    stroff += print_meas_log(&ru->precoding_stats       , "feptx_prec"          , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
    stroff += print_meas_log(&ru->txdataF_copy_stats    , "txdataF_copy"        , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
    stroff += print_meas_log(&ru->ofdm_mod_stats        , "feptx_ofdm"          , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
    stroff += print_meas_log(&ru->ofdm_total_stats      , "feptx_total"         , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  }

  if (ru->fh_north_asynch_in) 
    stroff += print_meas_log(&ru->rx_fhaul              , "rx_fhaul"            , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);

  stroff += print_meas_log(&ru->tx_fhaul                , "tx_fhaul"            , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);

  if (ru->fh_north_out) {
    stroff += print_meas_log(&ru->compression           , "compression"         , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
    stroff += print_meas_log(&ru->transport             , "transport"           , NULL, NULL, output+stroff, TELNET_MAX_MSGLENGTH - stroff);
  }

  prnt("%s\n", output);
}

static void 
gNBL1cpustats_thread_main(void * param)
{
  if (pthread_setname_np(pthread_self(), GNBL1_THREADNAME))
    LOG_E(UTIL, "Error setting name on thread %s: %s\n", GNBL1_THREADNAME, strerror(errno));

  struct sched_param schedp;
  schedp.sched_priority = SCHED_NORMAL;  
  if (pthread_setschedparam(pthread_self(), SCHED_IDLE, &schedp))
    LOG_E(UTIL, "Error setting priority in thread %s: %s\n", GNBL1_THREADNAME, strerror(errno));
    
  initNotifiedFIFO(&GNBL1cpustats_fifo);
  
  cpumeas(CPUMEAS_ENABLE);
  
  while (1){
    /* 
     * Infinite loop that waits for a message on the FIFO. 
     * Behind the scenes it uses blocking pthread.h:pthread_cond_wait()
     */
    notifiedFIFO_elt_t *msg = pullNotifiedFIFO(&GNBL1cpustats_fifo); 

    proto_stats_msg_t *tsm = (proto_stats_msg_t *)NotifiedFifoData(msg);
    switch (tsm->msgid)
    {
      case TIMESTAT_MSGID_DISPLAY:
        if (cpumeas(CPUMEAS_GETSTATE))
          gNBL1cpustats_display(tsm->prnt, (PHY_VARS_gNB *)param);
        else
          tsm->prnt("ERR: before displaying meaningful stats, you need to activate their generation\n");
        break;
      case TIMESTAT_MSGID_DISABLE:
        cpumeas(CPUMEAS_DISABLE);
        gNBL1cpustats_threadid = 0;
        pthread_exit(NULL);
        break;
    }
    delNotifiedFIFO_elt(msg);
  }
  LOG_I(UTIL, "Exiting thread '%s'\n", GNBL1_THREADNAME);
}

void 
gNBL1cpustats_measurcmd_display (telnet_printfunc_t prnt)
{
  if (!gNBL1cpustats_threadid) {
    prnt("gNB thread %s is not running. Aborting your request to display stats as you need first to enable statistics\n", GNBL1_THREADNAME);
    return;
  }
  
  notifiedFIFO_elt_t *nfe = newNotifiedFIFO_elt(sizeof(time_stats_msg_t), 0, NULL, NULL);
	proto_stats_msg_t  *msg = (proto_stats_msg_t *)NotifiedFifoData(nfe);
  msg->prnt  = prnt;
  msg->msgid = TIMESTAT_MSGID_DISPLAY;
  pushNotifiedFIFO(&GNBL1cpustats_fifo, nfe);
}

bool 
gNBL1cpustats_enable(PHY_VARS_gNB *gNB)
{
  if (gNBL1cpustats_threadid) {
    LOG_E(UTIL, "Thread '%s' already running, aborting your request to create a new one\n", GNBL1_THREADNAME);  
    return false;
  }

  int rt = pthread_create(&gNBL1cpustats_threadid, NULL, (void *(*)(void *))gNBL1cpustats_thread_main, gNB);
  if (rt != 0)
    LOG_E(UTIL, "Error creating thread '%s': %s\n", GNBL1_THREADNAME, strerror(errno));

  return (rt != 0) ? false : true;
}

void 
gNBL1cpustats_disable(void)
{
  if (!gNBL1cpustats_threadid) {
    LOG_W(UTIL, "Thread '%s' is not running. Discarding your request to disable stats\n", GNBL1_THREADNAME);
    return;
  }

  notifiedFIFO_elt_t *nfe = newNotifiedFIFO_elt(sizeof(time_stats_msg_t), 0, NULL, NULL);
	proto_stats_msg_t  *msg = (proto_stats_msg_t *)NotifiedFifoData(nfe);
  msg->msgid = TIMESTAT_MSGID_DISABLE;
  pushNotifiedFIFO(&GNBL1cpustats_fifo, nfe);
}