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


#include <stdio.h>
#include "common_lib.h"
#include "ethernet_lib.h"
#include "shared_buffers.h"
#include "low_oran.h"
#include "oran_isolate.h"

#include "common/utils/LOG/log.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "openair1/PHY/defs_gNB.h"

typedef struct {
  eth_state_t           e;
  shared_buffers        buffers;
  rru_config_msg_type_t last_msg;
  int                   capabilities_sent;
  void                  *oran_priv;
} oran_eth_state_t;


int trx_oran_start(openair0_device *device)
{

  printf("ORAN: %s\n", __FUNCTION__);

#if 1

  oran_eth_state_t *s = device->priv;

  // Start ORAN
   if ( start_oran(s->oran_priv) !=0 ){
      printf("%s:%d:%s: Start ORAN failed ... Exit\n",
         __FILE__, __LINE__, __FUNCTION__);
      exit(1); 
   }else{
      printf("Start ORAN. Done\n");
   }
#endif

  return 0;

}


void trx_oran_end(openair0_device *device)
{
  printf("ORAN: %s\n", __FUNCTION__);
  oran_eth_state_t *s = device->priv;
  stop_oran(s->oran_priv);
  close_oran(s->oran_priv);
}


int trx_oran_stop(openair0_device *device)
{
  printf("ORAN: %s\n", __FUNCTION__);
  oran_eth_state_t *s = device->priv;
  stop_oran(s->oran_priv);
  return(0);
}

int trx_oran_set_freq(openair0_device* device,
                         openair0_config_t *openair0_cfg,
                         int exmimo_dump_config)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return(0);
}

int trx_oran_set_gains(openair0_device* device,
                          openair0_config_t *openair0_cfg)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return(0);
}

int trx_oran_get_stats(openair0_device* device)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return(0);
}


int trx_oran_reset_stats(openair0_device* device)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return(0);
}

int ethernet_tune(openair0_device *device,
                  unsigned int option,
                  int value)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return 0;
}

int trx_oran_write_raw(openair0_device *device,
                          openair0_timestamp timestamp,
                          void **buff, int nsamps, int cc, int flags)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return 0;
}

int trx_oran_read_raw(openair0_device *device,
                         openair0_timestamp *timestamp,
                         void **buff, int nsamps, int cc)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return 0;
}

char *msg_type(int t)
{
  static char *s[12] = {
    "RAU_tick",
    "RRU_capabilities",
    "RRU_config",
    "RRU_config_ok",
    "RRU_start",
    "RRU_stop",
    "RRU_sync_ok",
    "RRU_frame_resynch",
    "RRU_MSG_max_num",
    "RRU_check_sync",
    "RRU_config_update",
    "RRU_config_update_ok",
  };

  if (t < 0 || t > 11) return "UNKNOWN";
  return s[t];
}

int trx_oran_ctlsend(openair0_device *device, void *msg, ssize_t msg_len)
{
  RRU_CONFIG_msg_t *rru_config_msg = msg;
  oran_eth_state_t *s = device->priv;

  printf("ORAN: %s\n", __FUNCTION__);

  printf("    rru_config_msg->type %d [%s]\n", rru_config_msg->type,
         msg_type(rru_config_msg->type));

  s->last_msg = rru_config_msg->type;

  return msg_len;
}

int trx_oran_ctlrecv(openair0_device *device, void *msg, ssize_t msg_len)
{
  RRU_CONFIG_msg_t *rru_config_msg = msg;
  oran_eth_state_t *s = device->priv;

  printf("ORAN: %s\n", __FUNCTION__);

  if (s->last_msg == RAU_tick && s->capabilities_sent == 0) {
    printf("CCCCC ORAN ctrlrcv RRU_tick received and send capabilities hard coded\n");
    RRU_capabilities_t *cap;
    rru_config_msg->type = RRU_capabilities;
    rru_config_msg->len  = sizeof(RRU_CONFIG_msg_t)-MAX_RRU_CONFIG_SIZE+sizeof(RRU_capabilities_t);
    // Fill RRU capabilities (see openair1/PHY/defs_RU.h)
    // For now they are hard coded - try to retreive the params from openari device
    
    cap = (RRU_capabilities_t*)&rru_config_msg->msg[0];
    cap->FH_fmt                           = OAI_IF4p5_only;
    cap->num_bands                        = 1;
    cap->band_list[0]                     = 78;
    // cap->num_concurrent_bands             = 1; component carriers
    cap->nb_rx[0]                         = 1; //device->openair0_cfg->rx_num_channels;
    cap->nb_tx[0]                         = 1; //device->openair0_cfg->tx_num_channels;
    cap->max_pdschReferenceSignalPower[0] = -27;
    cap->max_rxgain[0]                    = 90;
    cap->N_RB_DL[0]                       = 106;
    cap->N_RB_UL[0]                       = 106;
   

    s->capabilities_sent = 1;

    return rru_config_msg->len;
  }
  if (s->last_msg == RRU_config) {
    printf("Oran RRU_config\n");
    rru_config_msg->type = RRU_config_ok;
    //return 0;
  }
#if 0
  if (s->last_msg == RRU_start) {
     // Folllow the same steps as in the wrapper
     printf("Oran RRU_start\n");
     
     // Check if the machine is PTP sync
     check_xran_ptp_sync();

     // SetUp
     if ( setup_oran(s->oran_priv) !=0 ){ 
        printf("%s:%d:%s: SetUp ORAN failed ... Exit\n",
           __FILE__, __LINE__, __FUNCTION__);
        exit(1);       
     }else{
        printf("SetUp ORAN. Done\n");
     }
     
     // Load the IQ samples from file
     load_iq_from_file(s->oran_priv);
     printf("Load IQ from file. Done\n");

     // Register physide callbacks
     register_physide_callbacks(s->oran_priv);
     printf("Register physide callbacks. Done\n");

     // Open callbacks
     open_oran_callback(s->oran_priv);
     printf("Open Oran callbacks. Done\n");

     // Init ORAN
     initialize_oran(s->oran_priv);
     printf("Init Oran. Done\n");

     // Copy the loaded IQ to the xran buffer fro the tx
     xran_fh_tx_send_buffer(s->oran_priv);
     printf("ORAN FH send tx buffer filled in with loaded IQs. Done\n");

     // Open ORAN
     open_oran(s->oran_priv);
     printf("xran_open. Done\n");

     // Start ORAN
     if ( start_oran(s->oran_priv) !=0 ){
        printf("%s:%d:%s: Start ORAN failed ... Exit\n",
           __FILE__, __LINE__, __FUNCTION__);
        exit(1); 
     }else{
        printf("Start ORAN. Done\n");
     }
    
     #if 0 
     // Once xran library and thread started compute the statistics
     while(1){
        sleep(1);
        compute_xran_statistics(s->oran_priv); 
     }
     #endif

  }
#endif
  return 0;
}

void oran_fh_if4p5_south_in(RU_t *ru,
                               int *frame,
                               int *slot)
{
/*
  if(*frame==0 && *slot==0){
    printf("XXX oran_fh_if4p5_south_in %d %d\n", *frame, *slot);
  //  sleep(1);
  }
*/

  oran_eth_state_t *s = ru->ifdevice.priv;
  NR_DL_FRAME_PARMS *fp;
  int symbol;
  int32_t *rxdata;
  int antenna;
 
  //printf("XXX ORAN_fh_if4p5_south_in %d %d\n", *frame, *slot);

  ru_info_t ru_info;
  ru_info.nb_rx = ru->nb_rx;
  ru_info.rxdataF = ru->common.rxdataF;

  int ret = xran_fh_rx_read_slot(s->oran_priv, &ru_info, *frame, *slot);  

#if 0
  for (antenna = 0; antenna < ru->nb_rx; antenna++) {
    for (symbol = 0; symbol < 14; symbol++) {
      printf("\nantenna:%d\tsymbol=%d\n",antenna,symbol);
      for(int sample = 0; sample < 2048; sample++){
        printf("%x ",ru->common.rxdataF[antenna][symbol*2048+sample]);
      }
    }
  }
#endif

#if 0
next:
     while (!((s->buffers.ul_busy[0][*slot] == 0x3fff &&
            s->buffers.ul_busy[1][*slot] == 0x3fff) ||
           s->buffers.prach_busy[*slot] == 1))
    wait_ul_buffer(&s->buffers, *slot);
  if (s->buffers.prach_busy[*slot] == 1) {
    int i;
    int antenna = 0;
    uint16_t *in;
    uint16_t *out;
    in = (uint16_t *)s->buffers.prach[*slot];
    out = (uint16_t *)ru->prach_rxsigF[0][antenna];
    for (i = 0; i < 839*2; i++)
      out[i] = ntohs(in[i]);
    s->buffers.prach_busy[*slot] = 0;
    //printf("prach for f.sl %d.%d\n", *frame, *slot);
    //ru->wakeup_prach_gNB(ru->gNB_list[0], ru, *frame, *slot);
    goto next;
  }
#endif
#if 1
  fp = ru->nr_frame_parms;
  for (antenna = 0; antenna < ru->nb_rx; antenna++) {
    for (symbol = 0; symbol < 14; symbol++) {
      /*      
      int i;
      int16_t *p = (int16_t *)(&s->buffers.ul[antenna][*slot][symbol*1272*4]);
      for (i = 0; i < 1272*2; i++) {
        p[i] = (int16_t)(ntohs(p[i])) / 16;
      }
      */
      rxdata = &ru->common.rxdataF[antenna][symbol * fp->ofdm_symbol_size]; 
    }
  }
#endif
  RU_proc_t *proc = &ru->proc;
  extern uint16_t sl_ahead;
  int f = *frame;
  int sl = *slot;

  //calculate timestamp_rx, timestamp_tx based on frame and slot
  proc->tti_rx       = sl;
  proc->frame_rx     = f;
  /* TODO: be sure of samples_per_slot0
  FK: should use get_samples_per_slot(slot)
  but for mu=1 its ok
  */
  proc->timestamp_rx = ((proc->frame_rx * 20)  + proc->tti_rx ) * fp->samples_per_slot0;

  if (get_nprocs()<=4) {
    // why? what if there are more?
    proc->tti_tx   = (sl+sl_ahead)%20;
    proc->frame_tx = (sl>(19-sl_ahead)) ? (f+1)&1023 : f;
  }
#if 0
   printf("south_in:\ttimestamp_rx=%d{frame_rx=%d,tti_rx=%d}\ttti_tx=%d\tframe_tx=%d\n\n",proc->timestamp_rx,proc->frame_rx,proc->tti_rx,proc->tti_tx,proc->frame_tx);
   if(proc->frame_rx ==20){
      exit(-1);
   }
#endif
}

void oran_fh_if4p5_south_out(RU_t *ru,
                                int frame,
                                int slot,
                                uint64_t timestamp)
{
  openair0_device *device = &ru->ifdevice;
  oran_eth_state_t *s = device->priv;

  ru_info_t ru_info;
  ru_info.nb_tx = ru->nb_tx;
  ru_info.txdataF_BF = ru->common.txdataF_BF;
//printf("south_out:\tframe=%d\tslot=%d\ttimestamp=%ld\n",frame,slot,timestamp);
#if 0
//printf("\n ORAN south out frame:%d, slot:%d. ru->common.txdataF_BF and  ru_info.txdataF_BF \n",frame,slot);
for(int hhh=0; hhh<(14); hhh++ ){
   for(int jjj=0; jjj<2048; jjj++){
      /// Try to set all the values of the buffer to 1
      ru->common.txdataF_BF[0][hhh*2048+jjj] = 256;
/*      if(ru->common.txdataF_BF[0][hhh*2048+jjj] > 0){      
          printf(" %d:%d:%d ",hhh*2048+jjj,ru->common.txdataF_BF[0][hhh*2048+jjj],ru_info.txdataF_BF[0][hhh*2048+jjj]);
       }
*/
   }
}
//printf(" \n");
//exit(-1);
#endif

  int ret = xran_fh_tx_send_slot(s->oran_priv, &ru_info, frame, slot, timestamp);

#if 0
  if(frame==0 && slot<100){
    printf("XXX oran_fh_if4p5_south_out %d %d %ld\n", frame, slot, timestamp);
  }
#endif
#if 0
//printf("Sofia-Romain exit south out\n");
//exit(-1);
  oran_eth_state_t *s = ru->ifdevice.priv;
  NR_DL_FRAME_PARMS *fp;
  int symbol;
  int32_t *txdata;
  int aa;

  //printf("BENETEL: %s (f.sf %d.%d ts %ld)\n", __FUNCTION__, frame, slot, timestamp);

  lock_dl_buffer(&s->buffers, slot);
  if (s->buffers.dl_busy[0][slot] ||
      s->buffers.dl_busy[1][slot]) {
    printf("%s: fatal: DL buffer busy for subframe %d\n", __FUNCTION__, slot);
    unlock_dl_buffer(&s->buffers, slot);
    return;
  }

  fp = ru->nr_frame_parms;
  if (ru->num_gNB != 1 || fp->ofdm_symbol_size != 2048 ||
      fp->Ncp != NORMAL || fp->symbols_per_slot != 14) {
    printf("%s:%d:%s: unsupported configuration\n",
           __FILE__, __LINE__, __FUNCTION__);
    exit(1);
  }

  for (aa = 0; aa < ru->nb_tx; aa++) {
    for (symbol = 0; symbol < 14; symbol++) {
      txdata = &ru->common.txdataF_BF[aa][symbol * fp->ofdm_symbol_size];
#if 1
      memcpy(&s->buffers.dl[aa][slot][symbol*1272*4],
             txdata + 2048 - (1272/2),
             (1272/2) * 4);
      memcpy(&s->buffers.dl[aa][slot][symbol*1272*4] + (1272/2)*4,
             txdata,
             (1272/2) * 4);
#endif
      int i;
      uint16_t *p = (uint16_t *)(&s->buffers.dl[aa][slot][symbol*1272*4]);
      for (i = 0; i < 1272*2; i++) {
        p[i] = htons(p[i]);
      }
    }
  }

  s->buffers.dl_busy[0][slot] = 0x3fff;
  s->buffers.dl_busy[1][slot] = 0x3fff;
  unlock_dl_buffer(&s->buffers, slot);
#endif
}

void *get_internal_parameter(char *name)
{
  printf("ORAN: %s\n", __FUNCTION__);

  if (!strcmp(name, "fh_if4p5_south_in"))
    return (void *) oran_fh_if4p5_south_in;
  if (!strcmp(name, "fh_if4p5_south_out"))
    return (void *) oran_fh_if4p5_south_out;

  return NULL;
}


__attribute__((__visibility__("default")))
int transport_init(openair0_device *device,
                   openair0_config_t *openair0_cfg,
                   eth_params_t * eth_params )
{
  oran_eth_state_t *eth;

  
  device->Mod_id               = 0;
  device->transp_type          = ETHERNET_TP;
  device->trx_start_func       = trx_oran_start;
  device->trx_get_stats_func   = trx_oran_get_stats;
  device->trx_reset_stats_func = trx_oran_reset_stats;
  device->trx_end_func         = trx_oran_end;
  device->trx_stop_func        = trx_oran_stop;
  device->trx_set_freq_func    = trx_oran_set_freq;
  device->trx_set_gains_func   = trx_oran_set_gains;

  device->trx_write_func       = trx_oran_write_raw;
  device->trx_read_func        = trx_oran_read_raw;

  device->trx_ctlsend_func     = trx_oran_ctlsend;
  device->trx_ctlrecv_func     = trx_oran_ctlrecv;


  device->get_internal_parameter = get_internal_parameter;

  eth = (oran_eth_state_t *)calloc(1, sizeof(oran_eth_state_t));
  if (eth == NULL) {
    AssertFatal(0==1, "out of memory\n");
  }

  eth->e.flags = ETH_RAW_IF4p5_MODE;
  eth->e.compression = NO_COMPRESS;
  eth->e.if_name = eth_params->local_if_name;
  eth->oran_priv = define_oran_pointer(); 
  device->priv = eth;
  device->openair0_cfg=&openair0_cfg[0];

  eth->last_msg = (rru_config_msg_type_t)-1;

#if 0
  init_buffers(&eth->buffers);
#endif

#if 1
  oran_eth_state_t *s = eth;

  printf("ORAN: %s\n", __FUNCTION__);

  // Check if the machine is PTP sync
   check_xran_ptp_sync();

   // SetUp
   if ( setup_oran(s->oran_priv) !=0 ){ 
      printf("%s:%d:%s: SetUp ORAN failed ... Exit\n",
         __FILE__, __LINE__, __FUNCTION__);
      exit(1);       
   }else{
      printf("SetUp ORAN. Done\n");
   }

   // Dump ORAN config
   dump_oran_config(s->oran_priv);
   
   // Load the IQ samples from file
//   load_iq_from_file(s->oran_priv);
//   printf("Load IQ from file. Done\n");

   // Register physide callbacks
   register_physide_callbacks(s->oran_priv);
   printf("Register physide callbacks. Done\n");

   // Open callbacks
   open_oran_callback(s->oran_priv);
   printf("Open Oran callbacks. Done\n");

   // Init ORAN
   initialize_oran(s->oran_priv);
   printf("Init Oran. Done\n");

   // Copy the loaded IQ to the xran buffer fro the tx
  // xran_fh_tx_send_buffer(s->oran_priv);
  // printf("ORAN FH send tx buffer filled in with loaded IQs. Done\n");

   // Open ORAN
   open_oran(s->oran_priv);
   printf("xran_open. Done\n");
   

#endif 

  return 0;

}
