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

/*! \file PHY/NR_TRANSPORT/nr_ulsch.c
* \brief Top-level routines for the reception of the PUSCH TS 38.211 v 15.4.0
* \author Ahmed Hussein
* \date 2019
* \version 0.1
* \company Fraunhofer IIS
* \email: ahmed.hussein@iis.fraunhofer.de
* \note
* \warning
*/

#include <stdint.h>
#include "PHY/NR_TRANSPORT/nr_transport_common_proto.h"
#include "PHY/NR_TRANSPORT/nr_ulsch.h"

int16_t find_nr_ulsch(PHY_VARS_gNB *gNB, uint16_t rnti, int pid, int frame)
{

  int16_t first_free_index = -1;
  AssertFatal(gNB != NULL,"gNB is null\n");

  for (int i = 0; i < gNB->number_of_nr_ulsch_max; i++) {
    NR_gNB_ULSCH_t *ulsch = gNB->ulsch[i];
    AssertFatal(ulsch != NULL, "gNB->ulsch[%d] is null\n", i);
    if(!ulsch->active) {
      if (first_free_index == -1)
        first_free_index = i;
    }
    else {
      // if there is already an active ULSCH for this RNTI and HARQ_PID
      if ((ulsch->harq_pid == pid) && (ulsch->rnti == rnti))
        return i;
    }
  }
  return first_free_index;
}

void nr_fill_ulsch(PHY_VARS_gNB *gNB,
                   int frame,
                   int slot,
                   nfapi_nr_pusch_pdu_t *ulsch_pdu)
{
 
  int harq_pid = ulsch_pdu->pusch_data.harq_process_id;
  int ulsch_id = find_nr_ulsch(gNB, ulsch_pdu->rnti, harq_pid, frame);
  AssertFatal((ulsch_id >= 0) && (ulsch_id < gNB->number_of_nr_ulsch_max),
              "illegal or no ulsch_id found!!! rnti %04x ulsch_id %d\n",ulsch_pdu->rnti,ulsch_id);

  NR_gNB_ULSCH_t  *ulsch = gNB->ulsch[ulsch_id];

  ulsch->rnti = ulsch_pdu->rnti;
  ulsch->harq_pid = harq_pid;
  ulsch->handled = 0;
  ulsch->active = true;

  NR_UL_gNB_HARQ_t *harq = ulsch->harq_process;
  harq->frame = frame;
  harq->slot = slot;
  harq->new_rx = ulsch_pdu->pusch_data.new_data_indicator;
  LOG_D(PHY,"%d.%d ULSCH ID %d RNTI %x HARQ PID %d new data indicator %d\n",
        frame, slot, ulsch_id, ulsch_pdu->rnti, harq_pid, ulsch_pdu->pusch_data.new_data_indicator);
  if (harq->new_rx)
    harq->round = 0;
  else
    harq->round++;

  memcpy((void*)&ulsch->harq_process->ulsch_pdu, (void*)ulsch_pdu, sizeof(nfapi_nr_pusch_pdu_t));

  LOG_D(PHY,"Initializing nFAPI for ULSCH, UE %d, harq_pid %d\n", ulsch_id, harq_pid);
}

void nr_ulsch_unscrambling(int16_t* llr, uint32_t size, uint32_t Nid, uint32_t n_RNTI)
{
  nr_codeword_unscrambling(llr, size, 0, Nid, n_RNTI);
}

void nr_ulsch_layer_demapping(int16_t *llr_cw,
                              uint8_t Nl,
                              uint8_t mod_order,
                              uint32_t length,
                              int16_t **llr_layers)
{

  switch (Nl) {
    case 1:
      memcpy((void*)llr_cw, (void*)llr_layers[0], (length)*sizeof(int16_t));
      break;
    case 2:
    case 3:
    case 4:
      for (int i=0; i<(length/Nl/mod_order); i++) {
        for (int l=0; l<Nl; l++) {
          for (int m=0; m<mod_order; m++) {
            llr_cw[i*Nl*mod_order+l*mod_order+m] = llr_layers[l][i*mod_order+m];
          }
        }
      }
      break;
  default:
    AssertFatal(0, "Not supported number of layers %d\n", Nl);
  }
}

void dump_pusch_stats(FILE *fd, PHY_VARS_gNB *gNB)
{

  for (int i = 0;i < MAX_MOBILES_PER_GNB; i++) {
    NR_gNB_PHY_STATS_t *stats = &gNB->phy_stats[i];
    if (stats->active && stats->frame != stats->ulsch_stats.dump_frame) {
      stats->ulsch_stats.dump_frame = stats->frame;
      for (int aa = 0; aa < gNB->frame_parms.nb_antennas_rx; aa++)
        if (aa==0)
          fprintf(fd,"ULSCH RNTI %4x, %d: ulsch_power[%d] %d,%d ulsch_noise_power[%d] %d.%d, sync_pos %d\n",
                  stats->rnti, stats->frame,
                  aa, stats->ulsch_stats.power[aa]/10, stats->ulsch_stats.power[aa]%10,
                  aa, stats->ulsch_stats.noise_power[aa]/10, stats->ulsch_stats.noise_power[aa]%10,
                  stats->ulsch_stats.sync_pos);
        else
          fprintf(fd,"                  ulsch_power[%d] %d.%d, ulsch_noise_power[%d] %d.%d\n",
                  aa, stats->ulsch_stats.power[aa]/10, stats->ulsch_stats.power[aa]%10,
                  aa, stats->ulsch_stats.noise_power[aa]/10, stats->ulsch_stats.noise_power[aa]%10);


      fprintf(fd,"                 round_trials %d(%1.1e):%d(%1.1e):%d(%1.1e):%d, DTX %d, current_Qm %d, current_RI %d, total_bytes RX/SCHED %d/%d\n",
              stats->ulsch_stats.round_trials[0],
              (double)stats->ulsch_stats.round_trials[1]/stats->ulsch_stats.round_trials[0],
              stats->ulsch_stats.round_trials[1],
              (double)stats->ulsch_stats.round_trials[2]/stats->ulsch_stats.round_trials[0],
              stats->ulsch_stats.round_trials[2],
              (double)stats->ulsch_stats.round_trials[3]/stats->ulsch_stats.round_trials[0],
              stats->ulsch_stats.round_trials[3],
              stats->ulsch_stats.DTX,
              stats->ulsch_stats.current_Qm,
              stats->ulsch_stats.current_RI,
              stats->ulsch_stats.total_bytes_rx,
              stats->ulsch_stats.total_bytes_tx);
    }
  }
}

void clear_pusch_stats(PHY_VARS_gNB *gNB)
{
  for (int i = 0; i < MAX_MOBILES_PER_GNB; i++)
    memset((void*)&gNB->phy_stats[i].ulsch_stats,0,sizeof(gNB->phy_stats[i].ulsch_stats));
}


