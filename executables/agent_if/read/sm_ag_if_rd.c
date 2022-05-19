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


#include "sm_ag_if_rd.h"
#include "../../mac_sm/ie/mac_data_ie.h"
#include "../../rlc_sm/ie/rlc_data_ie.h"
#include "../../pdcp_sm/ie/pdcp_data_ie.h"
#include "../../slice_sm/ie/slice_data_ie.h"

#include <assert.h>
#include <stdlib.h>

void free_sm_ag_if_rd(sm_ag_if_rd_t* d)
{
  assert(d != NULL);

  if(d->type == MAC_STATS_V0){
    free_mac_ind_data(&d->mac_stats);
    //mac_ind_data_t* ind = &d->mac_stats;
    //free_mac_ind_hdr(&ind->hdr);
    //free_mac_ind_msg(&ind->msg);
    //free_mac_call_proc_id(ind->proc_id);
  } else if(d->type == RLC_STATS_V0){
    free_rlc_ind_data(&d->rlc_stats);
    //rlc_ind_data_t* ind = &d->rlc_stats;
    //free_rlc_ind_hdr(&ind->hdr);
    //free_rlc_ind_msg(&ind->msg);
    //free_rlc_call_proc_id(ind->proc_id);
  } else if(d->type == PDCP_STATS_V0){
    free_pdcp_ind_data(&d->pdcp_stats);
//    pdcp_ind_data_t* ind = &d->pdcp_stats;
//    free_pdcp_ind_hdr(&ind->hdr);
//    free_pdcp_ind_msg(&ind->msg);
//    free_pdcp_call_proc_id(ind->proc_id);
  } else if(d->type == SLICE_STATS_V0) {
    free_slice_ind_data(&d->slice_stats);
  } else {
    assert(0!=0 && "Unforeseen case");
  }

}

sm_ag_if_rd_t cp_sm_ag_if_rd(sm_ag_if_rd_t const* d)
{
  assert(d != NULL);

  sm_ag_if_rd_t ans = {.type = d->type};

  if(ans.type == MAC_STATS_V0){
    ans.mac_stats = cp_mac_ind_data(&d->mac_stats);
  } else if(ans.type == RLC_STATS_V0 ){
    ans.rlc_stats = cp_rlc_ind_data(&d->rlc_stats);
  } else if(ans.type == PDCP_STATS_V0) {
    ans.pdcp_stats = cp_pdcp_ind_data(&d->pdcp_stats);
  } else if(ans.type == SLICE_STATS_V0) {
    ans.slice_stats = cp_slice_ind_data(&d->slice_stats);
  } else {
    assert("Unknown type or not implemented");
  }

  return ans;
}
