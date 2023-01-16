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
#include "oran_isolate.h"

#include "common/utils/LOG/log.h"
#include "common/utils/LOG/vcd_signal_dumper.h"
#include "openair1/PHY/defs_gNB.h"

typedef struct {
  eth_state_t           e;
  rru_config_msg_type_t last_msg;
  int                   capabilities_sent;
  void                  *oran_priv;
} oran_eth_state_t;


int trx_oran_start(openair0_device *device)
{

  printf("ORAN: %s\n", __FUNCTION__);

  oran_eth_state_t *s = device->priv;

  // Start ORAN
   if ( start_oran(s->oran_priv) !=0 ){
      printf("%s:%d:%s: Start ORAN failed ... Exit\n",
         __FILE__, __LINE__, __FUNCTION__);
      exit(1); 
   }else{
      printf("Start ORAN. Done\n");
   }

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
  }
  return 0;
}

void oran_fh_if4p5_south_in(RU_t *ru,
                               int *frame,
                               int *slot)
{
  oran_eth_state_t *s = ru->ifdevice.priv;
  NR_DL_FRAME_PARMS *fp;
  int symbol;
  int32_t *rxdata;
  int antenna;
  static uint8_t sync = 0;

  ru_info_t ru_info;
  ru_info.nb_rx = ru->nb_rx;
  ru_info.rxdataF = ru->common.rxdataF;
  ru_info.prach_buf = (int *)ru->prach_rxsigF[0][0];//index: [prach_oca][ant_id]

  RU_proc_t *proc = &ru->proc;
  extern uint16_t sl_ahead;
  int f, sl;

  int ret = xran_fh_rx_read_slot(s->oran_priv, &ru_info, &f, &sl, *frame, *slot, sync);

  if (ret != 0){
     printf("ORAN: ORAN_fh_if4p5_south_in ERROR in RX function \n");
  }

  fp = ru->nr_frame_parms;
  for (antenna = 0; antenna < ru->nb_rx; antenna++) {
    for (symbol = 0; symbol < 14; symbol++) {
      rxdata = &ru->common.rxdataF[antenna][symbol * fp->ofdm_symbol_size]; 
    }
  }

  proc->tti_rx       = sl;
  proc->frame_rx     = f;
  proc->tti_tx       = (sl+sl_ahead)%20;
  proc->frame_tx     = (sl>(19-sl_ahead)) ? (f+1)&1023 : f;

  if (proc->first_rx == 0) {
    if (proc->tti_rx != *slot) {
      LOG_E(PHY,"Received Timestamp doesn't correspond to the time we think it is (proc->tti_rx %d, slot %d)\n",proc->tti_rx,*slot);
    }

    if (proc->frame_rx != *frame) {
      LOG_E(PHY,"Received Timestamp doesn't correspond to the time we think it is (proc->frame_rx %d frame %d proc->tti_rx %d tti %d)\n",proc->frame_rx,*frame,proc->tti_rx,*slot);
    }
  } else {
    proc->first_rx = 0;
    LOG_I(PHY, "before adjusting, OAI: frame=%d slot=%d, XRAN: frame=%d slot=%d\n",*frame,*slot,proc->frame_rx,proc->tti_rx);
    *frame = proc->frame_rx;
    *slot = proc->tti_rx;
    LOG_I(PHY, "After adjusting, OAI: frame=%d slot=%d, XRAN: frame=%d slot=%d\n",*frame,*slot,proc->frame_rx,proc->tti_rx);
  }

  sync = 1;
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

  int ret = xran_fh_tx_send_slot(s->oran_priv, &ru_info, frame, slot, timestamp);
  if (ret != 0){
     printf("ORAN: ORAN_fh_if4p5_south_out ERROR in TX function \n");
  }
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
   
   // Register physide callbacks
   register_physide_callbacks(s->oran_priv);
   printf("Register physide callbacks. Done\n");

   // Open callbacks
   open_oran_callback(s->oran_priv);
   printf("Open Oran callbacks. Done\n");

   // Init ORAN
   initialize_oran(s->oran_priv);
   printf("Init Oran. Done\n");

   // Open ORAN
   open_oran(s->oran_priv);
   printf("xran_open. Done\n");

  return 0;
}
