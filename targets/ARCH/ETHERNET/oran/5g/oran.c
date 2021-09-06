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
#include "openair1/PHY/defs_nr_common.h"


typedef struct {
  eth_state_t           e;
  shared_buffers        buffers;
  rru_config_msg_type_t last_msg;
  int                   capabilities_sent;
  void                  *oran_priv;
} oran_eth_state_t;



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



int trx_oran_start(openair0_device *device)
{
  xranLibWraper *xranlib; 
  xranlib = new xranLibWraper;
  
  if(xranlib->SetUp() < 0) {
     return (-1);
  }
  xranlib->Init();
  xranlib->Open(nullptr, 
            nullptr, 
            (void *)xran_fh_rx_callback, 
            (void *)xran_fh_rx_prach_callback, 
            (void *)xran_fh_srs_callback);
  xranlib->Start();
  
  printf("ORAN 5G:  %s\n", __FUNCTION__);
  return 0;
}


void trx_oran_end(openair0_device *device)
{
  printf("ORAN: %s\n", __FUNCTION__);
}


int trx_oran_stop(openair0_device *device)
{
  printf("ORAN: %s\n", __FUNCTION__);
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
  return nsamps*4;
}

int trx_oran_read_raw(openair0_device *device,
                         openair0_timestamp *timestamp,
                         void **buff, int nsamps, int cc)
{
  printf("ORAN: %s\n", __FUNCTION__);
  return nsamps*4;
}

int trx_oran_ctlsend(openair0_device *device, void *msg, ssize_t msg_len)
{
  RRU_CONFIG_msg_t *rru_config_msg = msg;
  oran_eth_state_t *s = device->priv;

  printf("ORAN 5G: %s\n", __FUNCTION__);

  printf("    rru_config_msg->type %d [%s]\n", rru_config_msg->type,
         msg_type(rru_config_msg->type));

  s->last_msg = rru_config_msg->type;

  return msg_len;
}


int trx_oran_ctlrecv(openair0_device *device, void *msg, ssize_t msg_len)
{
  RRU_CONFIG_msg_t *rru_config_msg = msg;
 oran_eth_state_t *s = device->priv;

  printf("ORAN 5G: %s\n", __FUNCTION__);

  if (s->last_msg == RAU_tick && s->capabilities_sent == 0) {
    RRU_capabilities_t *cap;
    rru_config_msg->type = RRU_capabilities;
    rru_config_msg->len  = sizeof(RRU_CONFIG_msg_t)-MAX_RRU_CONFIG_SIZE+sizeof(RRU_capabilities_t);
    cap = (RRU_capabilities_t*)&rru_config_msg->msg[0];
    cap->FH_fmt                           = ORAN_only;
    cap->num_bands                        = 1;
    cap->band_list[0]                     = 78;
    cap->nb_rx[0]                         = 1;
    cap->nb_tx[0]                         = 1;
    cap->max_pdschReferenceSignalPower[0] = -27;
    cap->max_rxgain[0]                    = 90;

    s->capabilities_sent = 1;

    return rru_config_msg->len;

  }
 // if (s->last_msg == RRU_config) {
 //   rru_config_msg->type = RRU_config_ok;
 //   return 0;
 // }
  printf("---------------    rru_config_msg->type %d [%s]\n", rru_config_msg->type,
         msg_type(rru_config_msg->type));
 if (s->last_msg == RRU_config) {
 rru_config_msg->type = RRU_config_ok;
printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    s->oran_priv = oran_start(s->e.if_name, &s->buffers);
  }
  return 0;
}




/*This function reads the IQ samples from OAI, symbol by symbol.
 It also handles the shared buffers.                          */

void oran_fh_if4p5_south_in(RU_t *ru,
                               int *frame,
                               int *slot)
{
  oran_eth_state_t *s = ru->ifdevice.priv;
  NR_DL_FRAME_PARMS *fp;
  int symbol;
  int32_t *rxdata;
  int antenna = 0;
printf("Ann in oran.c: nr_oran_fh_if4p5_south_in\n");
  lock_ul_buffers(&s->buffers, *slot);
next:
  while (!(s->buffers.ul_busy[*slot] == 0x3fff ||
           s->buffers.prach_busy[*slot] == 1))
    wait_buffers(&s->buffers, *slot);
  if (s->buffers.prach_busy[*slot] == 1) {
    int i;
    int antenna = 0;
    uint16_t *in;
    uint16_t *out;
    in = (uint16_t *)s->buffers.prach[*slot];
    out = (uint16_t *)ru->prach_rxsigF[antenna];
    for (i = 0; i < 849; i++)
      out[i] = ntohs(in[i]);
    s->buffers.prach_busy[*slot] = 0;
    ru->wakeup_prach_gNB(ru->gNB_list[0], ru, *frame, *slot);
    goto next;
  }
  fp  = ru->nr_frame_parms;
  for (symbol = 0; symbol < 14; symbol++) {
    int i;
    uint16_t *p = (uint16_t *)(&s->buffers.ul[*slot][symbol*1272*4]);
    for (i = 0; i < 1272*2; i++) {
      p[i] = htons(p[i]);
    }
    rxdata = &ru->common.rxdataF[antenna][symbol * fp->ofdm_symbol_size];
#if 1
    memcpy(rxdata + 2048 - 1272/2,
           &s->buffers.ul[*slot][symbol*1272*4],
           (1272/2) * 4);
    memcpy(rxdata,
           &s->buffers.ul[*slot][symbol*1272*4] + (1272/2)*4,
           (1272/2) * 4);
#endif
  }

  s->buffers.ul_busy[*slot] = 0;
  signal_buffers(&s->buffers, *slot);
  unlock_buffers(&s->buffers, *slot);

  //printf("ORAN: %s (f.sf %d.%d)\n", __FUNCTION__, *frame, *subframe);

  RU_proc_t *proc = &ru->proc;
  extern uint16_t sl_ahead;
  int f = *frame;
  int sl = *slot;

  //calculate timestamp_rx, timestamp_tx based on frame and subframe
  proc->tti_rx       = sl;
  proc->frame_rx     = f;
  proc->timestamp_rx = ((proc->frame_rx * 20)  + proc->tti_rx ) * fp->samples_per_tti ;

  if (get_nprocs()<=4) {
    proc->tti_tx   = (sl+sl_ahead)%20;
    proc->frame_tx = (sl>(19-sl_ahead)) ? (f+1)&1023 : f;
  }
}


void oran_fh_if4p5_south_out(RU_t *ru,
                                int frame,
                                int slot,
                                uint64_t timestamp)
{
  oran_eth_state_t *s = ru->ifdevice.priv;
  NR_DL_FRAME_PARMS *fp;
  int symbol;
  int32_t *txdata;
  int aa = 0;
printf("Ann in oran.c: oran_fh_if4p5_south_out\n");
  //printf("ORAN: %s (f.sf %d.%d ts %ld)\n", __FUNCTION__, frame, subframe, timestamp);

  lock_dl_buffers(&s->buffers, slot);

  fp  = ru->nr_frame_parms;
  if (ru->num_gNB != 1 || ru->nb_tx != 1 || fp->ofdm_symbol_size != 2048 ||
      fp->Ncp != NORMAL || fp->symbols_per_slot != 14) {
    printf("%s:%d:%s: unsupported configuration\n",
           __FILE__, __LINE__, __FUNCTION__);
    exit(1);
  }

  for (symbol = 0; symbol < 14; symbol++) {
    txdata = &ru->common.txdataF_BF[aa][symbol * fp->ofdm_symbol_size];
#if 1
    memcpy(&s->buffers.dl[slot][symbol*1272*4],
           txdata + 2048 - (1272/2),
           (1272/2) * 4);
    memcpy(&s->buffers.dl[slot][symbol*1272*4] + (1272/2)*4,
           txdata + 1,
           (1272/2) * 4);
#endif
    int i;
    uint16_t *p = (uint16_t *)(&s->buffers.dl[slot][symbol*1272*4]);
    for (i = 0; i < 1272*2; i++) {
      p[i] = htons(p[i]);
    }
  }

  s->buffers.dl_busy[slot] = 0x3fff;
  unlock_buffers(&s->buffers, slot);
}

void *get_internal_parameter(char *name)
{
  printf("BENETEL 5G: %s\n", __FUNCTION__);

  if (!strcmp(name, "fh_if4p5_south_in"))
    return oran_fh_if4p5_south_in;
  if (!strcmp(name, "fh_if4p5_south_out"))
    return oran_fh_if4p5_south_out;

  return NULL;
}

__attribute__((__visibility__("default")))
int transport_init(openair0_device *device,
                   openair0_config_t *openair0_cfg,
                   eth_params_t * eth_params )
{
  oran_eth_state_t *eth;
printf("Ann: ORANNNi 5g\n");
  printf("ORAN 5g: %s\n", __FUNCTION__);

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

  eth = calloc(1, sizeof(oran_eth_state_t));
  if (eth == NULL) {
    AssertFatal(0==1, "out of memory\n");
  }

  eth->e.flags = ETH_RAW_IF4p5_MODE;
  eth->e.compression = NO_COMPRESS;
  eth->e.if_name = eth_params->local_if_name;
  device->priv = eth;
  device->openair0_cfg=&openair0_cfg[0];

  eth->last_msg = -1;

  init_buffers(&eth->buffers);

  return 0;
}

























