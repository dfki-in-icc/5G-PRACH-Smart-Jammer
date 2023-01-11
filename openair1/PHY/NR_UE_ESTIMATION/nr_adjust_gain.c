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

#include "PHY/types.h"
#include "PHY/defs_nr_UE.h"
#include "PHY/phy_extern_nr_ue.h"
/*  Adjust the RX Gain based upon NF based upon SS block measurements */
void phy_adjust_gain_nr(PHY_VARS_NR_UE *ue) {
  /* Average received noise power */
  const int n0_dB = 10 * log10(ue->measurements.n0_power_avg);
  /* Taget noise power to be achieved */
  const int n0_dB_target = 20;
  /* Margin to be used for decision is 3 dB */
  const int margin = 3;
  /* If noise power is higher than target reduce the gain */
  if (n0_dB > (n0_dB_target + margin)) {
    ue->measurements.rx_gain_update = 1;
    ue->rfdevice.openair0_cfg[0].rx_gain[0] = ue->rfdevice.app_rx_gain[0] - 3;
  } else if (n0_dB < (n0_dB_target - margin)) {
    /* If noise power is lower than the target increase the gain */
    ue->measurements.rx_gain_update = 1;
    ue->rfdevice.openair0_cfg[0].rx_gain[0] = ue->rfdevice.app_rx_gain[0] + 3;
  }
  /* Upper-limit for the Rx gain, below 10 dB from max possible USRP Gain */
  if (ue->rfdevice.openair0_cfg[0].rx_gain[0] > (ue->rfdevice.max_rx_gain[0] - 10)) {
    ue->rfdevice.openair0_cfg[0].rx_gain[0] = (ue->rfdevice.max_rx_gain[0] - 10);
    ue->measurements.rx_gain_update = 1;
  }
 /* lower limit for Rx gain is set to 20 dB with 20 dB attenuator inbetween Tx/Rx */
  if (ue->rfdevice.openair0_cfg[0].rx_gain[0] < 20) {
    ue->rfdevice.openair0_cfg[0].rx_gain[0] = 20;
    ue->measurements.rx_gain_update = 1;
  }

  LOG_D(PHY,
        "Gain Control: Flag %d, n0_dB : %3d (dB) Old Rx Gain: %3.2f (dB) New Rx Gain %3.2f\n",
        ue->measurements.rx_gain_update, n0_dB,
        ue->rfdevice.app_rx_gain[0], ue->rfdevice.openair0_cfg[0].rx_gain[0]);
}

/*  Adjust the TX Gain based upon gain calculated from power control */
void phy_adjust_txgain_nr(PHY_VARS_NR_UE *ue, int16_t gain) {

  int16_t app_tx_gain = ue->rfdevice.app_tx_gain[0];

  /* Margin to be used for decision is 3 dB */
  const int margin = 3;
  /* If Gain change needed is more than margin then update the USRP TX gain */
  if ((app_tx_gain - gain) > margin) {
    ue->measurements.tx_gain_update = 1;
    int16_t tx_gain = 0;
    tx_gain = ue->rfdevice.openair0_cfg[0].tx_gain[0] - gain;
    if (tx_gain > ue->rfdevice.max_tx_gain[0])
      tx_gain = ue->rfdevice.max_tx_gain[0];
    else if (tx_gain < ue->rfdevice.min_tx_gain[0])
      tx_gain = ue->rfdevice.min_tx_gain[0];
    ue->rfdevice.openair0_cfg[0].tx_gain[0] = tx_gain;
  } 

  LOG_I(PHY,
        "TX Gain Control: Flag %d, Margin:%d gain change needed:%d ,applied Tx Gain: %3.2f (dB) New Tx Gain ATT value %3.2f, Max:%3.2f\n",
        ue->measurements.tx_gain_update, margin, gain, ue->rfdevice.app_tx_gain[0], ue->rfdevice.openair0_cfg[0].tx_gain[0],
        ue->rfdevice.max_tx_gain[0]);
}