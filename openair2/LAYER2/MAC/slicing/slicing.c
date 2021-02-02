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

/*!
 * \file   slicing.c
 * \brief  Generic slicing helper functions and Static Slicing Implementation
 * \author Robert Schmidt
 * \date   2020
 * \email  robert.schmidt@eurecom.fr
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>

#include "assertions.h"
#include "common/utils/LOG/log.h"

#include "slicing.h"
#include "slicing_internal.h"

#include "common/ran_context.h"
extern RAN_CONTEXT_t RC;

#define RET_FAIL(ret, x...) do { LOG_E(MAC, x); return ret; } while (0)

int slicing_get_UE_slice_idx(slice_info_t *si, int UE_id) {
  return si->UE_assoc_slice[UE_id];
}

void slicing_add_UE(slice_info_t *si, int UE_id) {
  add_ue_list(&si->s[0]->UEs, UE_id);
  si->UE_assoc_slice[UE_id] = 0;
}

void _remove_UE(slice_t **s, uint8_t *assoc, int UE_id) {
  const uint8_t i = assoc[UE_id];
  DevAssert(remove_ue_list(&s[i]->UEs, UE_id));
  assoc[UE_id] = -1;
}

void slicing_remove_UE(slice_info_t *si, int UE_id) {
  _remove_UE(si->s, si->UE_assoc_slice, UE_id);
}

void _move_UE(slice_t **s, uint8_t *assoc, int UE_id, int to) {
  const uint8_t i = assoc[UE_id];
  const int ri = remove_ue_list(&s[i]->UEs, UE_id);
  if (!ri)
    LOG_W(MAC, "did not find UE %d in DL slice index %d\n", UE_id, i);
  add_ue_list(&s[to]->UEs, UE_id);
  assoc[UE_id] = to;
}

void slicing_move_UE(slice_info_t *si, int UE_id, int idx) {
  DevAssert(idx >= -1 && idx < si->num);
  if (idx >= 0)
    _move_UE(si->s, si->UE_assoc_slice, UE_id, idx);
}

int _exists_slice(uint8_t n, slice_t **s, int id) {
  for (int i = 0; i < n; ++i)
    if (s[i]->id == id)
      return i;
  return -1;
}

slice_t *_add_slice(uint8_t *n, slice_t **s) {
  s[*n] = calloc(1, sizeof(slice_t));
  if (!s[*n])
    return NULL;
  init_ue_list(&s[*n]->UEs);
  *n += 1;
  return s[*n - 1];
}

slice_t *_remove_slice(uint8_t *n, slice_t **s, uint8_t *assoc, int idx) {
  if (idx >= *n)
    return NULL;

  slice_t *sr = s[idx];
  while (sr->UEs.head >= 0)
    _move_UE(s, assoc, sr->UEs.head, 0);

  for (int i = idx + 1; i < *n; ++i)
    s[i - 1] = s[i];
  *n -= 1;
  s[*n] = NULL;

  for (int i = 0; i < MAX_MOBILES_PER_ENB; ++i)
    if (assoc[i] > idx)
      assoc[i] -= 1;

  if (sr->label)
    free(sr->label);

  return sr;
}

/************************ Static Slicing Implementation ************************/

int addmod_static_slice_dl(slice_info_t *si,
                           int id,
                           char *label,
                           void *algo,
                           void *slice_params_dl) {
  static_slice_param_t *dl = slice_params_dl;
  if (dl && dl->posLow > dl->posHigh)
    RET_FAIL(-1, "%s(): slice id %d posLow > posHigh\n", __func__, id);

  uint8_t rbgMap[25] = { 0 };
  int index = _exists_slice(si->num, si->s, id);
  if (index >= 0) {
    for (int s = 0; s < si->num; ++s) {
      static_slice_param_t *sd = dl && si->s[s]->id == id ? dl : si->s[s]->algo_data;
      for (int i = sd->posLow; i <= sd->posHigh; ++i) {
        if (rbgMap[i])
          RET_FAIL(-33, "%s(): overlap of slices detected at RBG %d\n", __func__, i);
        rbgMap[i] = 1;
      }
    }
    /* no problem, can allocate */
    slice_t *s = si->s[index];
    if (label) {
      if (s->label) free(s->label);
      s->label = label;
    }
    if (algo) {
      s->dl_algo.unset(&s->dl_algo.data);
      s->dl_algo = *(default_sched_dl_algo_t *) algo;
      if (!s->dl_algo.data)
        s->dl_algo.data = s->dl_algo.setup();
    }
    if (dl) {
      free(s->algo_data);
      s->algo_data = dl;
    }
    return index;
  }

  if (!dl)
    RET_FAIL(-100, "%s(): no parameters for new slice %d, aborting\n", __func__, id);

  if (si->num >= MAX_STATIC_SLICES)
    RET_FAIL(-2, "%s(): cannot have more than %d slices\n", __func__, MAX_STATIC_SLICES);
  for (int s = 0; s < si->num; ++s) {
    static_slice_param_t *sd = si->s[s]->algo_data;
    for (int i = sd->posLow; i <= sd->posHigh; ++i)
      rbgMap[i] = 1;
  }

  for (int i = dl->posLow; i <= dl->posHigh; ++i)
    if (rbgMap[i])
      RET_FAIL(-3, "%s(): overlap of slices detected at RBG %d\n", __func__, i);

  if (!algo)
    RET_FAIL(-14, "%s(): no scheduler algorithm provided\n", __func__);

  slice_t *ns = _add_slice(&si->num, si->s);
  if (!ns)
    RET_FAIL(-4, "%s(): could not create new slice\n", __func__);
  ns->id = id;
  ns->label = label;
  ns->dl_algo = *(default_sched_dl_algo_t *) algo;
  if (!ns->dl_algo.data)
    ns->dl_algo.data = ns->dl_algo.setup();
  ns->algo_data = dl;

  return si->num - 1;
}

int addmod_static_slice_ul(slice_info_t *si,
                           int id,
                           char *label,
                           void *algo,
                           void *slice_params_ul) {
  static_slice_param_t *ul = slice_params_ul;
  /* Minimum 3RBs, because LTE stack requires this */
  if (ul && ul->posLow + 2 > ul->posHigh)
    RET_FAIL(-1, "%s(): slice id %d posLow + 2 > posHigh\n", __func__, id);

  uint8_t rbMap[110] = { 0 };
  int index = _exists_slice(si->num, si->s, id);
  if (index >= 0) {
    for (int s = 0; s < si->num; ++s) {
      static_slice_param_t *su = ul && si->s[s]->id == id && ul ? ul : si->s[s]->algo_data;
      for (int i = su->posLow; i <= su->posHigh; ++i) {
        if (rbMap[i])
          RET_FAIL(-33, "%s(): overlap of slices detected at RBG %d\n", __func__, i);
        rbMap[i] = 1;
      }
    }
    /* no problem, can allocate */
    slice_t *s = si->s[index];
    if (algo) {
      s->ul_algo.unset(&s->ul_algo.data);
      s->ul_algo = *(default_sched_ul_algo_t *) algo;
      if (!s->ul_algo.data)
        s->ul_algo.data = s->ul_algo.setup();
    }
    if (label) {
      if (s->label) free(s->label);
      s->label = label;
    }
    if (ul) {
      free(s->algo_data);
      s->algo_data = ul;
    }
    return index;
  }

  if (!ul)
    RET_FAIL(-100, "%s(): no parameters for new slice %d, aborting\n", __func__, id);

  if (si->num >= MAX_STATIC_SLICES)
    RET_FAIL(-2, "%s(): cannot have more than %d slices\n", __func__, MAX_STATIC_SLICES);
  for (int s = 0; s < si->num; ++s) {
    static_slice_param_t *sd = si->s[s]->algo_data;
    for (int i = sd->posLow; i <= sd->posHigh; ++i)
      rbMap[i] = 1;
  }

  for (int i = ul->posLow; i <= ul->posHigh; ++i)
    if (rbMap[i])
      RET_FAIL(-3, "%s(): overlap of slices detected at RBG %d\n", __func__, i);

  if (!algo)
    RET_FAIL(-14, "%s(): no scheduler algorithm provided\n", __func__);

  slice_t *ns = _add_slice(&si->num, si->s);
  if (!ns)
    RET_FAIL(-4, "%s(): could not create new slice\n", __func__);
  ns->id = id;
  ns->label = label;
  ns->ul_algo = *(default_sched_ul_algo_t *) algo;
  if (!ns->ul_algo.data)
    ns->ul_algo.data = ns->ul_algo.setup();
  ns->algo_data = ul;

  return si->num - 1;
}

int remove_static_slice_dl(slice_info_t *si, uint8_t slice_idx) {
  if (slice_idx == 0)
    return 0;
  slice_t *sr = _remove_slice(&si->num, si->s, si->UE_assoc_slice, slice_idx);
  if (!sr)
    return 0;
  free(sr->algo_data);
  sr->dl_algo.unset(&sr->dl_algo.data);
  free(sr);
  return 1;
}

int remove_static_slice_ul(slice_info_t *si, uint8_t slice_idx) {
  if (slice_idx == 0)
    return 0;
  slice_t *sr = _remove_slice(&si->num, si->s, si->UE_assoc_slice, slice_idx);
  if (!sr)
    return 0;
  free(sr->algo_data);
  sr->ul_algo.unset(&sr->ul_algo.data);
  free(sr);
  return 1;
}

void static_dl(module_id_t mod_id,
               int CC_id,
               frame_t frame,
               sub_frame_t subframe) {
  UE_info_t *UE_info = &RC.mac[mod_id]->UE_info;

  store_dlsch_buffer(mod_id, CC_id, frame, subframe);

  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    UE_sched_ctrl_t *ue_sched_ctrl = &UE_info->UE_sched_ctrl[UE_id];

    /* initialize per-UE scheduling information */
    ue_sched_ctrl->pre_nb_available_rbs[CC_id] = 0;
    ue_sched_ctrl->dl_pow_off[CC_id] = 2;
    memset(ue_sched_ctrl->rballoc_sub_UE[CC_id], 0, sizeof(ue_sched_ctrl->rballoc_sub_UE[CC_id]));
    ue_sched_ctrl->pre_dci_dl_pdu_idx = -1;
  }

  const int N_RBG = to_rbg(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  const int RBGsize = get_min_rb_unit(mod_id, CC_id);
  uint8_t *vrb_map = RC.mac[mod_id]->common_channels[CC_id].vrb_map;
  uint8_t rbgalloc_mask[N_RBG_MAX];
  for (int i = 0; i < N_RBG; i++) {
    // calculate mask: init to one + "AND" with vrb_map:
    // if any RB in vrb_map is blocked (1), the current RBG will be 0
    rbgalloc_mask[i] = 1;
    for (int j = 0; j < RBGsize; j++)
      rbgalloc_mask[i] &= !vrb_map[RBGsize * i + j];
  }

  slice_info_t *s = RC.mac[mod_id]->pre_processor_dl.slices;
  int max_num_ue;
  switch (s->num) {
    case 1:
      max_num_ue = 4;
      break;
    case 2:
      max_num_ue = 2;
      break;
    default:
      max_num_ue = 1;
      break;
  }
  for (int i = 0; i < s->num; ++i) {
    if (s->s[i]->UEs.head < 0)
      continue;
    uint8_t rbgalloc_slice_mask[N_RBG_MAX];
    memset(rbgalloc_slice_mask, 0, sizeof(rbgalloc_slice_mask));
    static_slice_param_t *p = s->s[i]->algo_data;
    int n_rbg_sched = 0;
    for (int rbg = p->posLow; rbg <= p->posHigh && rbg <= N_RBG; ++rbg) {
      rbgalloc_slice_mask[rbg] = rbgalloc_mask[rbg];
      n_rbg_sched += rbgalloc_mask[rbg];
    }
    if (n_rbg_sched == 0) /* no free RBGs, e.g., taken by RA */
      continue;

    s->s[i]->dl_algo.run(mod_id,
                         CC_id,
                         frame,
                         subframe,
                         &s->s[i]->UEs,
                         max_num_ue, // max_num_ue
                         n_rbg_sched,
                         rbgalloc_slice_mask,
                         s->s[i]->dl_algo.data);
  }

  // the following block is meant for validation of the pre-processor to check
  // whether all UE allocations are non-overlapping and is not necessary for
  // scheduling functionality
  char t[26] = "_________________________";
  t[N_RBG] = 0;
  for (int i = 0; i < N_RBG; i++)
    for (int j = 0; j < RBGsize; j++)
      if (vrb_map[RBGsize*i+j] != 0)
        t[i] = 'x';
  int print = 0;
  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    const UE_sched_ctrl_t *ue_sched_ctrl = &UE_info->UE_sched_ctrl[UE_id];

    if (ue_sched_ctrl->pre_nb_available_rbs[CC_id] == 0)
      continue;

    LOG_D(MAC,
          "%4d.%d UE%d %d RBs allocated, pre MCS %d\n",
          frame,
          subframe,
          UE_id,
          ue_sched_ctrl->pre_nb_available_rbs[CC_id],
          UE_info->eNB_UE_stats[CC_id][UE_id].dlsch_mcs1);

    print = 1;

    for (int i = 0; i < N_RBG; i++) {
      if (!ue_sched_ctrl->rballoc_sub_UE[CC_id][i])
        continue;
      for (int j = 0; j < RBGsize; j++) {
        if (vrb_map[RBGsize*i+j] != 0) {
          LOG_I(MAC, "%4d.%d DL scheduler allocation list: %s\n", frame, subframe, t);
          LOG_E(MAC, "%4d.%d: UE %d allocated at locked RB %d/RBG %d\n", frame,
                subframe, UE_id, RBGsize * i + j, i);
        }
        vrb_map[RBGsize*i+j] = 1;
      }
      t[i] = '0' + UE_id;
    }
  }
  if (print)
    LOG_D(MAC, "%4d.%d DL scheduler allocation list: %s\n", frame, subframe, t);
}

void static_ul(module_id_t mod_id,
               int CC_id,
               frame_t frame,
               sub_frame_t subframe,
               frame_t sched_frame,
               sub_frame_t sched_subframe) {
  UE_info_t *UE_info = &RC.mac[mod_id]->UE_info;
  const int N_RB_UL = to_prb(RC.mac[mod_id]->common_channels[CC_id].ul_Bandwidth);
  COMMON_channels_t *cc = &RC.mac[mod_id]->common_channels[CC_id];

  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    UE_TEMPLATE *UE_template = &UE_info->UE_template[CC_id][UE_id];
    UE_template->pre_assigned_mcs_ul = 0;
    UE_template->pre_allocated_nb_rb_ul = 0;
    UE_template->pre_allocated_rb_table_index_ul = -1;
    UE_template->pre_first_nb_rb_ul = 0;
    UE_template->pre_dci_ul_pdu_idx = -1;
  }

  slice_info_t *s = RC.mac[mod_id]->pre_processor_ul.slices;
  int max_num_ue;
  switch (s->num) {
    case 1:
      max_num_ue = 4;
      break;
    case 2:
      max_num_ue = 2;
      break;
    default:
      max_num_ue = 1;
      break;
  }
  for (int i = 0; i < s->num; ++i) {
    if (s->s[i]->UEs.head < 0)
      continue;
    int last_rb_blocked = 1;
    int n_contig = 0;
    contig_rbs_t rbs[2]; // up to two contig RBs for PRACH in between
    static_slice_param_t *p = s->s[i]->algo_data;
    for (int rb = p->posLow; rb <= p->posHigh && rb < N_RB_UL; ++rb) {
      if (cc->vrb_map_UL[rb] == 0 && last_rb_blocked) {
        last_rb_blocked = 0;
        n_contig++;
        AssertFatal(n_contig <= 2, "cannot handle more than two contiguous RB regions\n");
        rbs[n_contig - 1].start = rb;
      }
      if (cc->vrb_map_UL[rb] == 1 && !last_rb_blocked) {
        last_rb_blocked = 1;
        rbs[n_contig - 1].length = rb - rbs[n_contig - 1].start;
      }
    }
    if (!last_rb_blocked)
      rbs[n_contig - 1].length = p->posHigh - rbs[n_contig - 1].start + 1;
    if (n_contig == 1 && rbs[0].length == 0) /* no RBs, e.g., taken by RA */
      continue;

    s->s[i]->ul_algo.run(mod_id,
                         CC_id,
                         frame,
                         subframe,
                         sched_frame,
                         sched_subframe,
                         &s->s[i]->UEs,
                         max_num_ue, // max_num_ue
                         n_contig,
                         rbs,
                         s->s[i]->ul_algo.data);
  }

  // the following block is meant for validation of the pre-processor to check
  // whether all UE allocations are non-overlapping and is not necessary for
  // scheduling functionality
  char t[101] = "__________________________________________________"
                "__________________________________________________";
  t[N_RB_UL] = 0;
  for (int j = 0; j < N_RB_UL; j++)
    if (cc->vrb_map_UL[j] != 0)
      t[j] = 'x';
  int print = 0;
  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    UE_TEMPLATE *UE_template = &UE_info->UE_template[CC_id][UE_id];
    if (UE_template->pre_allocated_nb_rb_ul == 0)
      continue;

    print = 1;
    uint8_t harq_pid = subframe2harqpid(&RC.mac[mod_id]->common_channels[CC_id],
                                        sched_frame, sched_subframe);
    LOG_D(MAC, "%4d.%d UE%d %d RBs (index %d) at start %d, pre MCS %d %s\n",
          frame,
          subframe,
          UE_id,
          UE_template->pre_allocated_nb_rb_ul,
          UE_template->pre_allocated_rb_table_index_ul,
          UE_template->pre_first_nb_rb_ul,
          UE_template->pre_assigned_mcs_ul,
          UE_info->UE_sched_ctrl[UE_id].round_UL[CC_id][harq_pid] > 0 ? "(retx)" : "");

    for (int i = 0; i < UE_template->pre_allocated_nb_rb_ul; ++i) {
      /* only check if this is not a retransmission */
      if (UE_info->UE_sched_ctrl[UE_id].round_UL[CC_id][harq_pid] == 0
          && cc->vrb_map_UL[UE_template->pre_first_nb_rb_ul + i] == 1) {

        LOG_I(MAC, "%4d.%d UL scheduler allocation list: %s\n", frame, subframe, t);
        LOG_E(MAC,
              "%4d.%d: UE %d allocated at locked RB %d (is: allocated start "
              "%d/length %d)\n",
              frame, subframe, UE_id, UE_template->pre_first_nb_rb_ul + i,
              UE_template->pre_first_nb_rb_ul,
              UE_template->pre_allocated_nb_rb_ul);
      }
      cc->vrb_map_UL[UE_template->pre_first_nb_rb_ul + i] = 1;
      t[UE_template->pre_first_nb_rb_ul + i] = UE_id + '0';
    }
  }
  if (print)
    LOG_D(MAC,
          "%4d.%d UL scheduler allocation list: %s\n",
          sched_frame,
          sched_subframe,
          t);
}

void static_destroy(slice_info_t **si) {
  const int n = (*si)->num;
  (*si)->num = 0;
  for (int i = 0; i < n; ++i) {
    slice_t *s = (*si)->s[i];
    if (s->label)
      free(s->label);
    free(s->algo_data);
    free(s);
  }
  free((*si)->s);
  free(*si);
}

pp_impl_param_t static_dl_init(module_id_t mod_id, int CC_id) {
  slice_info_t *si = calloc(1, sizeof(slice_info_t));
  DevAssert(si);

  si->num = 0;
  si->s = calloc(MAX_STATIC_SLICES, sizeof(slice_t));
  DevAssert(si->s);
  for (int i = 0; i < MAX_MOBILES_PER_ENB; ++i)
    si->UE_assoc_slice[i] = -1;

  /* insert default slice, all resources */
  static_slice_param_t *dlp = malloc(sizeof(static_slice_param_t));
  dlp->posLow = 0;
  dlp->posHigh = to_rbg(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth) - 1;
  default_sched_dl_algo_t *algo = &RC.mac[mod_id]->pre_processor_dl.dl_algo;
  algo->data = NULL;
  DevAssert(0 == addmod_static_slice_dl(si, 0, strdup("default"), algo, dlp));
  const UE_list_t *UE_list = &RC.mac[mod_id]->UE_info.list;
  for (int UE_id = UE_list->head; UE_id >= 0; UE_id = UE_list->next[UE_id])
    slicing_add_UE(si, UE_id);

  pp_impl_param_t sttc;
  sttc.algorithm = STATIC_SLICING;
  sttc.add_UE = slicing_add_UE;
  sttc.remove_UE = slicing_remove_UE;
  sttc.move_UE = slicing_move_UE;
  sttc.addmod_slice = addmod_static_slice_dl;
  sttc.remove_slice = remove_static_slice_dl;
  sttc.dl = static_dl;
  // current DL algo becomes default scheduler
  sttc.dl_algo = *algo;
  sttc.destroy = static_destroy;
  sttc.slices = si;

  return sttc;
}

pp_impl_param_t static_ul_init(module_id_t mod_id, int CC_id) {
  slice_info_t *si = calloc(1, sizeof(slice_info_t));
  DevAssert(si);

  si->num = 0;
  si->s = calloc(MAX_STATIC_SLICES, sizeof(slice_t));
  DevAssert(si->s);
  for (int i = 0; i < MAX_MOBILES_PER_ENB; ++i)
    si->UE_assoc_slice[i] = -1;

  /* insert default slice, all resources */
  static_slice_param_t *ulp = malloc(sizeof(static_slice_param_t));
  ulp->posLow = 0;
  ulp->posHigh = to_prb(RC.mac[mod_id]->common_channels[CC_id].ul_Bandwidth) - 1;
  default_sched_ul_algo_t *algo = &RC.mac[mod_id]->pre_processor_ul.ul_algo;
  algo->data = NULL;
  const int rc = addmod_static_slice_ul(si, 0, strdup("default"), algo, ulp);
  DevAssert(0 == rc);
  const UE_list_t *UE_list = &RC.mac[mod_id]->UE_info.list;
  for (int UE_id = UE_list->head; UE_id >= 0; UE_id = UE_list->next[UE_id])
    slicing_add_UE(si, UE_id);

  pp_impl_param_t sttc;
  sttc.algorithm = STATIC_SLICING;
  sttc.add_UE = slicing_add_UE;
  sttc.remove_UE = slicing_remove_UE;
  sttc.move_UE = slicing_move_UE;
  sttc.addmod_slice = addmod_static_slice_ul;
  sttc.remove_slice = remove_static_slice_ul;
  sttc.ul = static_ul;
  // current DL algo becomes default scheduler
  sttc.ul_algo = *algo;
  sttc.destroy = static_destroy;
  sttc.slices = si;

  return sttc;
}

/************************* NVS Slicing Implementation **************************/

typedef struct {
  float exp; // exponential weight. mov. avg for weight calc
  int   rb;  // number of RBs this slice has been scheduled in last round
  float eff; // effective rate for rate slices
  float beta_eff; // averaging coeff so we average over roughly one second
  int   active;   // activity state for rate slices
} _nvs_int_t;

int _nvs_admission_control(const slice_info_t *si,
                           const nvs_slice_param_t *p,
                           int idx) {
  if (p->type != NVS_RATE && p->type != NVS_RES)
    RET_FAIL(-1, "%s(): invalid slice type %d\n", __func__, p->type);
  if (p->type == NVS_RATE && p->Mbps_reserved > p->Mbps_reference)
    RET_FAIL(-1,
             "%s(): a rate slice cannot reserve more than the reference rate\n",
             __func__);
  if (p->type == NVS_RES && p->pct_reserved > 1.0f)
    RET_FAIL(-1, "%s(): cannot reserve more than 1.0\n", __func__);
  float sum_req = 0.0f;
  for (int i = 0; i < si->num; ++i) {
    const nvs_slice_param_t *sp = i == idx ? p : si->s[i]->algo_data;
    if (sp->type == NVS_RATE)
      sum_req += sp->Mbps_reserved / sp->Mbps_reference;
    else
      sum_req += sp->pct_reserved;
  }
  if (idx < 0) { /* not an existing slice */
    if (p->type == NVS_RATE)
      sum_req += p->Mbps_reserved / p->Mbps_reference;
    else
      sum_req += p->pct_reserved;
  }
  if (sum_req > 1.0)
    RET_FAIL(-3,
             "%s(): admission control failed: sum of resources is %f > 1.0\n",
             __func__, sum_req);
  return 0;
}

int addmod_nvs_slice_dl(slice_info_t *si,
                        int id,
                        char *label,
                        void *algo,
                        void *slice_params_dl) {
  nvs_slice_param_t *dl = slice_params_dl;
  int index = _exists_slice(si->num, si->s, id);
  if (index < 0 && si->num >= MAX_NVS_SLICES)
    RET_FAIL(-2, "%s(): cannot handle more than %d slices\n", __func__, MAX_NVS_SLICES);

  if (index < 0 && !dl)
    RET_FAIL(-100, "%s(): no parameters for new slice %d, aborting\n", __func__, id);

  if (dl) {
    int rc = _nvs_admission_control(si, dl, index);
    if (rc < 0)
      return rc;
  }

  slice_t *s = NULL;
  if (index >= 0) {
    s = si->s[index];
    if (label) {
      if (s->label) free(s->label);
      s->label = label;
    }
    if (algo) {
      s->dl_algo.unset(&s->dl_algo.data);
      s->dl_algo = *(default_sched_dl_algo_t *) algo;
      if (!s->dl_algo.data)
        s->dl_algo.data = s->dl_algo.setup();
    }
    if (dl) {
      free(s->algo_data);
      s->algo_data = dl;
    } else { /* we have no parameters: we are done */
      return index;
    }
  } else {
    if (!algo)
      RET_FAIL(-14, "%s(): no scheduler algorithm provided\n", __func__);

    s = _add_slice(&si->num, si->s);
    if (!s)
      RET_FAIL(-4, "%s(): cannot allocate memory for slice\n", __func__);
    s->int_data = malloc(sizeof(_nvs_int_t));
    if (!s->int_data)
      RET_FAIL(-5, "%s(): cannot allocate memory for slice internal data\n", __func__);

    s->id = id;
    s->label = label;
    s->dl_algo = *(default_sched_dl_algo_t *) algo;
    if (!s->dl_algo.data)
      s->dl_algo.data = s->dl_algo.setup();
    s->algo_data = dl;
  }

  _nvs_int_t *nvs_p = s->int_data;
  /* reset all slice-internal parameters */
  nvs_p->rb = 0;
  nvs_p->active = 0;
  if (dl->type == NVS_RATE) {
    nvs_p->exp = dl->Mbps_reserved / dl->Mbps_reference;
    nvs_p->eff = dl->Mbps_reference;
  } else {
    nvs_p->exp = dl->pct_reserved;
    nvs_p->eff = 0; // not used
  }
  // scale beta so we (roughly) average the eff rate over 1s
  nvs_p->beta_eff = BETA / nvs_p->exp;

  return index < 0 ? si->num - 1 : index;
}

//int addmod_nvs_slice_ul(slice_info_t *si,
//                        int id,
//                        char *label,
//                        void *slice_params_ul) {
//  nvs_slice_param_t *sp = slice_params_ul;
//  int index = _exists_slice(si->num, si->s, id);
//  if (index < 0 && si->num >= MAX_NVS_SLICES)
//    RET_FAIL(-2, "%s(): cannot handle more than %d slices\n", __func__, MAX_NVS_SLICES);
//
//  int rc = _nvs_admission_control(si->num, si->s, sp, index);
//  if (rc < 0)
//    return rc;
//
//  slice_t *ns = NULL;
//  if (index < 0) {
//    ns = _add_slice(&si->num, si->s);
//    if (!ns)
//      RET_FAIL(-4, "%s(): cannot allocate memory for slice\n", __func__);
//    ns->id = id;
//    ns->int_data = malloc(sizeof(_nvs_int_t));
//    if (!ns->int_data)
//      RET_FAIL(-5, "%s(): cannot allocate memory for slice internal data\n",
//               __func__);
//  } else {
//    ns = si->s[index];
//    free(ns->algo_data);
//  }
//  if (label) {
//    if (ns->label)
//      free(ns->label);
//    ns->label = label;
//  }
//  ns->algo_data = sp;
//  _nvs_int_t *nvs_p = ns->int_data;
//  nvs_p->rb = 0;
//  nvs_p->active = 0;
//  if (sp->type == NVS_RATE) {
//    nvs_p->exp = sp->Mbps_reserved;
//    nvs_p->eff = sp->Mbps_reference;
//  } else {
//    nvs_p->exp = sp->pct_reserved;
//    nvs_p->eff = 0; // not used
//  }
//
//  return si->num - 1;
//}

int remove_nvs_slice_dl(slice_info_t *si, uint8_t slice_idx) {
  if (slice_idx == 0)
    return 0;
  slice_t *sr = _remove_slice(&si->num, si->s, si->UE_assoc_slice, slice_idx);
  if (!sr)
    return 0;
  free(sr->algo_data);
  free(sr->int_data);
  sr->dl_algo.unset(&sr->dl_algo.data);
  free(sr);
  return 1;
}

//int remove_nvs_slice_ul(slice_info_t *si, uint8_t slice_idx) {
//  if (slice_idx == 0)
//    return 0;
//  slice_t *sr = _remove_slice(&si->num, si->s, si->UE_assoc_slice, slice_idx);
//  if (!sr)
//    return 0;
//  free(sr->algo_data);
//  free(sr->int_data);
//  free(sr);
//  return 1;
//}

void nvs_dl(module_id_t mod_id,
               int CC_id,
               frame_t frame,
               sub_frame_t subframe) {
  UE_info_t *UE_info = &RC.mac[mod_id]->UE_info;

  store_dlsch_buffer(mod_id, CC_id, frame, subframe);

  slice_info_t *si = RC.mac[mod_id]->pre_processor_dl.slices;
  const COMMON_channels_t *cc = &RC.mac[mod_id]->common_channels[CC_id];
  const uint8_t harq_pid = frame_subframe2_dl_harq_pid(cc->tdd_Config, frame, subframe);
  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    UE_sched_ctrl_t *ue_sched_ctrl = &UE_info->UE_sched_ctrl[UE_id];

    /* initialize per-UE scheduling information */
    ue_sched_ctrl->pre_nb_available_rbs[CC_id] = 0;
    ue_sched_ctrl->dl_pow_off[CC_id] = 2;
    memset(ue_sched_ctrl->rballoc_sub_UE[CC_id], 0, sizeof(ue_sched_ctrl->rballoc_sub_UE[CC_id]));
    ue_sched_ctrl->pre_dci_dl_pdu_idx = -1;

    const int idx = si->UE_assoc_slice[UE_id];
    DevAssert(idx >= 0);
    const UE_TEMPLATE *UE_template = &UE_info->UE_template[CC_id][UE_id];
    const uint8_t round = UE_info->UE_sched_ctrl[UE_id].round[CC_id][harq_pid];
    /* if UE has data or retransmission, mark respective slice as active */
    const bool active = UE_template->dl_buffer_total > 0 || round != 8;
    ((_nvs_int_t *)si->s[idx]->int_data)->active |= active;
  }

  const int N_RBG = to_rbg(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  const int RBGsize = get_min_rb_unit(mod_id, CC_id);
  const int N_RB_DL = to_prb(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  uint8_t *vrb_map = RC.mac[mod_id]->common_channels[CC_id].vrb_map;
  uint8_t rbgalloc_mask[N_RBG_MAX];
  int n_rbg_sched = 0;
  for (int i = 0; i < N_RBG; i++) {
    // calculate mask: init to one + "AND" with vrb_map:
    // if any RB in vrb_map is blocked (1), the current RBG will be 0
    rbgalloc_mask[i] = 1;
    for (int j = 0; j < RBGsize && RBGsize * i + j < N_RB_DL; j++)
      rbgalloc_mask[i] &= !vrb_map[RBGsize * i + j];
    n_rbg_sched += rbgalloc_mask[i];
  }

  /* todo: schedule retransmission first */

  float maxw = 0.0f;
  int maxidx = -1;
  for (int i = 0; i < si->num; ++i) {
    slice_t *s = si->s[i];
    nvs_slice_param_t *p = s->algo_data;
    _nvs_int_t *ip = s->int_data;
    /* if this slice has been marked as inactive, disable to prevent that
     * it's exp rate is uselessly driven down */
    if (!ip->active)
      continue;

    float w = 0.0f;
    if (p->type == NVS_RATE) {
      float inst = 0.0f;
      if (ip->rb > 0) { /* it was scheduled last round */
        /* inst rate: B in last round * 8(bit) / 1000000 (Mbps) * 1000 (1ms) */
        inst = (float) RC.mac[mod_id]->eNB_stats[CC_id].dlsch_bytes_tx * 8 / 1000;
        ip->eff = (1.0f - ip->beta_eff) * ip->eff + ip->beta_eff * inst;
        //LOG_W(MAC, "i %d slice %d ip->rb %d inst %f ip->eff %f\n", i, s->id, ip->rb, inst, ip->eff);
        ip->rb = 0;
      }
      ip->exp = (1 - BETA) * ip->exp + BETA * inst;
      const float rsv = p->Mbps_reserved * min(1.0f, ip->eff / p->Mbps_reference);
      w = rsv / ip->exp;
    } else {
      float inst = (float)ip->rb / N_RB_DL;
      ip->exp = (1.0f - BETA) * ip->exp + BETA * inst;
      w = p->pct_reserved / ip->exp;
    }
    //LOG_I(MAC, "i %d slice %d type %d ip->exp %f w %f\n", i, s->id, p->type, ip->exp, w);
    ip->rb = 0;
    if (w > maxw + 0.001f) {
      maxw = w;
      maxidx = i;
    }
  }

  if (maxidx < 0)
    return;

  int nb_rb = n_rbg_sched * RBGsize;
  if (rbgalloc_mask[N_RBG - 1]
      && (N_RB_DL == 15 || N_RB_DL == 25 || N_RB_DL == 50 || N_RB_DL == 75))
    nb_rb -= 1;
  ((_nvs_int_t *)si->s[maxidx]->int_data)->rb = nb_rb;

  int rbg_rem = n_rbg_sched;
  if (si->s[maxidx]->UEs.head >= 0) {
    rbg_rem = si->s[maxidx]->dl_algo.run(mod_id,
                                         CC_id,
                                         frame,
                                         subframe,
                                         &si->s[maxidx]->UEs,
                                         4, // max_num_ue
                                         n_rbg_sched,
                                         rbgalloc_mask,
                                         si->s[maxidx]->dl_algo.data);
  }
  if (rbg_rem == n_rbg_sched) // if no RBGs have been used mark as inactive
    ((_nvs_int_t *)si->s[maxidx]->int_data)->active = 0;

  // the following block is meant for validation of the pre-processor to check
  // whether all UE allocations are non-overlapping and is not necessary for
  // scheduling functionality
  char t[26] = "_________________________";
  t[N_RBG] = 0;
  for (int i = 0; i < N_RBG; i++)
    for (int j = 0; j < RBGsize; j++)
      if (vrb_map[RBGsize*i+j] != 0)
        t[i] = 'x';
  int print = 0;
  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    const UE_sched_ctrl_t *ue_sched_ctrl = &UE_info->UE_sched_ctrl[UE_id];

    if (ue_sched_ctrl->pre_nb_available_rbs[CC_id] == 0)
      continue;

    LOG_D(MAC,
          "%4d.%d UE%d %d RBs allocated, pre MCS %d\n",
          frame,
          subframe,
          UE_id,
          ue_sched_ctrl->pre_nb_available_rbs[CC_id],
          UE_info->eNB_UE_stats[CC_id][UE_id].dlsch_mcs1);

    print = 1;

    for (int i = 0; i < N_RBG; i++) {
      if (!ue_sched_ctrl->rballoc_sub_UE[CC_id][i])
        continue;
      for (int j = 0; j < RBGsize; j++) {
        if (vrb_map[RBGsize*i+j] != 0) {
          LOG_I(MAC, "%4d.%d DL scheduler allocation list: %s\n", frame, subframe, t);
          LOG_E(MAC, "%4d.%d: UE %d allocated at locked RB %d/RBG %d\n", frame,
                subframe, UE_id, RBGsize * i + j, i);
        }
        vrb_map[RBGsize*i+j] = 1;
      }
      t[i] = '0' + UE_id;
    }
  }
  if (print)
    LOG_D(MAC, "%4d.%d DL scheduler allocation list: %s\n", frame, subframe, t);
}

void nvs_ul(module_id_t mod_id,
               int CC_id,
               frame_t frame,
               sub_frame_t subframe,
               frame_t sched_frame,
               sub_frame_t sched_subframe) {
  ulsch_scheduler_pre_processor(mod_id, CC_id, frame, subframe, sched_frame, sched_subframe);
}

void nvs_destroy(slice_info_t **si) {
  const int n_dl = (*si)->num;
  (*si)->num = 0;
  for (int i = 0; i < n_dl; ++i) {
    slice_t *s = (*si)->s[i];
    if (s->label)
      free(s->label);
    free(s->algo_data);
    free(s->int_data);
    free(s);
  }
  free((*si)->s);
}

pp_impl_param_t nvs_dl_init(module_id_t mod_id, int CC_id) {
  slice_info_t *si = calloc(1, sizeof(slice_info_t));
  DevAssert(si);

  si->num = 0;
  si->s = calloc(MAX_NVS_SLICES, sizeof(slice_t));
  DevAssert(si->s);
  for (int i = 0; i < MAX_MOBILES_PER_ENB; ++i)
    si->UE_assoc_slice[i] = -1;

  /* insert default slice, all resources */
  nvs_slice_param_t *dlp = malloc(sizeof(nvs_slice_param_t));
  DevAssert(dlp);
  dlp->type = NVS_RES;
  dlp->pct_reserved = 1.0f;
  default_sched_dl_algo_t *algo = &RC.mac[mod_id]->pre_processor_dl.dl_algo;
  algo->data = NULL;
  const int rc = addmod_nvs_slice_dl(si, 0, strdup("default"), algo, dlp);
  DevAssert(0 == rc);
  const UE_list_t *UE_list = &RC.mac[mod_id]->UE_info.list;
  for (int UE_id = UE_list->head; UE_id >= 0; UE_id = UE_list->next[UE_id])
    slicing_add_UE(si, UE_id);

  pp_impl_param_t nvs;
  nvs.algorithm = NVS_SLICING;
  nvs.add_UE = slicing_add_UE;
  nvs.remove_UE = slicing_remove_UE;
  nvs.move_UE = slicing_move_UE;
  nvs.addmod_slice = addmod_nvs_slice_dl;
  nvs.remove_slice = remove_nvs_slice_dl;
  nvs.dl = nvs_dl;
  // current DL algo becomes default scheduler
  nvs.dl_algo = *algo;
  nvs.destroy = nvs_destroy;
  nvs.slices = si;

  return nvs;
}

/************************* EDF Slicing Implementation **************************/

typedef struct {
  uint32_t Q;
  uint32_t n;
  bool suspended;
#define d_INF 0xffffffff
  uint32_t d;
} _edf_int_t;

int _edf_admission_control(const slice_info_t *si, edf_slice_param_t *p, int id, int idx, int N_RB_DL)
{
  float sum_req = 0.0f;
  for (int i = 0; i < si->num; ++i) {
    edf_slice_param_t *sp = i == idx ? p : si->s[i]->algo_data;
    sum_req += (float) sp->guaranteed_prbs / sp->deadline;
  }
  if (idx < 0) { /* not an existing slice */
    sum_req += (float) p->guaranteed_prbs / p->deadline;
  }
  /* check if all slices in override are actually present and emit warning if
   * not (do not fail, since then it is not possible to create multiple slices
   * that mutually use override */
  for (int i = 0; i < p->noverride; ++i) {
    bool found = id == p->loverride[i];
    for (int j = 0; !found && j < si->num; ++j)
      found = si->s[j]->id == p->loverride[i];
    if (!found)
      LOG_W(FLEXRAN_AGENT,
            "%s(): slice ID %d references slice ID %d in override, but it is not present (yet)\n",
            __func__, id, p->loverride[i]);
    if (i == p->noverride - 1 && p->loverride[i] != id)
      RET_FAIL(-110,
               "%s(): last override slice of slice index %d should be itself (ID %d)\n",
               __func__, idx, id);
  }
  if (sum_req > 1.0f * N_RB_DL)
    RET_FAIL(-103,
             "%s(): admission control failed: sum of resources %f > 1.0\n",
             __func__, sum_req);
  return 0;
}

int addmod_edf_slice_dl(slice_info_t *si, int id, char *label, void *algo, void *slice_params_dl)
{
  edf_slice_param_t *dl = slice_params_dl;
  int index = _exists_slice(si->num, si->s, id);
  if (index < 0 && si->num >= MAX_EDF_SLICES)
    RET_FAIL(-2, "%s(): cannot handle more than %d slices\n", __func__, MAX_EDF_SLICES);

  const module_id_t mod_id = 0;
  const int CC_id = 0;
  const int N_RB_DL = to_prb(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  const int rc = _edf_admission_control(si, dl, id, index, N_RB_DL);
  if (rc < 0)
    return rc;

  slice_t *ns = NULL;
  if (index >= 0) {
    ns = si->s[index];
    if (label) {
      if (ns->label) free(ns->label);
      ns->label = label;
    }
    if (algo) {
      ns->dl_algo.unset(&ns->dl_algo.data);
      ns->dl_algo = *(default_sched_dl_algo_t *) algo;
      if (!ns->dl_algo.data)
        ns->dl_algo.data = ns->dl_algo.setup();
    }
    if (dl) {
      free(ns->algo_data);
      ns->algo_data = dl;
    } else { /* we have no parameters: we are done */
      return index;
    }
  } else {
    if (!algo)
      RET_FAIL(-14, "%s(): no scheduler algorithm provided\n", __func__);

    ns = _add_slice(&si->num, si->s);
    if (!ns)
      RET_FAIL(-4, "%s(): cannot allocate memory for slice\n", __func__);
    ns->int_data = malloc(sizeof(_edf_int_t));
    if (!ns->int_data)
      RET_FAIL(-5, "%s(): cannot allocate memory for slice internal data\n",
               __func__);

    ns->id = id;
    ns->label = label;
    ns->dl_algo = *(default_sched_dl_algo_t *) algo;
    if (!ns->dl_algo.data)
      ns->dl_algo.data = ns->dl_algo.setup();
    ns->algo_data = dl;
  }

  _edf_int_t *edf_p = ns->int_data;
  /* reset all slice-internal parameters */
  edf_p->Q = 0;
  edf_p->n = 0;
  edf_p->suspended = false;
  edf_p->d = d_INF;
  return index < 0 ? si->num - 1 : index;
}

void edf_destroy(slice_info_t **si)
{
  const int n_dl = (*si)->num;
  (*si)->num = 0;
  for (int i = 0; i < n_dl; ++i) {
    slice_t *s = (*si)->s[i];
    if (s->label)
      free(s->label);
    free(s->algo_data);
    free(s->int_data);
    free(s);
  }
  free((*si)->s);
}

void edf_dl(module_id_t mod_id, int CC_id, frame_t frame, sub_frame_t subframe)
{
  UE_info_t *UE_info = &RC.mac[mod_id]->UE_info;

  store_dlsch_buffer(mod_id, CC_id, frame, subframe);

  slice_info_t *si = RC.mac[mod_id]->pre_processor_dl.slices;
  const COMMON_channels_t *cc = &RC.mac[mod_id]->common_channels[CC_id];
  const uint8_t harq_pid = frame_subframe2_dl_harq_pid(cc->tdd_Config, frame, subframe);
  uint64_t slice_buffered_data[MAX_EDF_SLICES] = {0};
  bool slice_retransmission_ue[MAX_EDF_SLICES] = {0};
  for (int UE_id = UE_info->list.head; UE_id >= 0; UE_id = UE_info->list.next[UE_id]) {
    UE_sched_ctrl_t *ue_sched_ctrl = &UE_info->UE_sched_ctrl[UE_id];

    /* initialize per-UE scheduling information */
    ue_sched_ctrl->pre_nb_available_rbs[CC_id] = 0;
    ue_sched_ctrl->dl_pow_off[CC_id] = 2;
    memset(ue_sched_ctrl->rballoc_sub_UE[CC_id], 0, sizeof(ue_sched_ctrl->rballoc_sub_UE[CC_id]));
    ue_sched_ctrl->pre_dci_dl_pdu_idx = -1;

    const UE_TEMPLATE *UE_template = &UE_info->UE_template[CC_id][UE_id];
    const uint8_t round = UE_info->UE_sched_ctrl[UE_id].round[CC_id][harq_pid];
    const int idx = si->UE_assoc_slice[UE_id];
    DevAssert(idx >= 0);
    /* if UE has data or retransmission, mark respective slice as active */
    slice_buffered_data[idx] += UE_template->dl_buffer_total;
    slice_retransmission_ue[idx] |= (round != 8); // round != 8 -> UE has retx

    eNB_UE_STATS *eNB_UE_stats = &UE_info->eNB_UE_stats[CC_id][UE_id];
    slice_t *s = si->s[idx];
    _edf_int_t *ip = s->int_data;
    ip->n -= eNB_UE_stats->rbs_used;
  }

  /* EDF algorithm */
  uint32_t q[MAX_EDF_SLICES] = {0};
  UE_list_t L = {.head = -1, .next = {0}};
  for (int i = 0; i < si->num; ++i) {
    slice_t *s = si->s[i];
    const edf_slice_param_t *p = s->algo_data;
    _edf_int_t *ip = s->int_data;

    if (slice_buffered_data[i] > 0) {
      if (ip->d == d_INF || ip->d == 0) {
        ip->d = p->deadline;
        ip->n = 0;
        ip->suspended = false;
        ip->Q = p->guaranteed_prbs;
        q[i] = ip->Q;
      } else {
        if (!ip->suspended) {
          q[i] = max(0, ip->Q - ip->n);
        } else {
          ip->Q = min(p->guaranteed_prbs, ip->Q - ip->n + p->guaranteed_prbs * (p->deadline - ip->d) / p->deadline);
          q[i] = ip->Q;
          ip->d = p->deadline;
          ip->n = 0;
          ip->suspended = false;
        }
      }
    } else {
      if (ip->d == 0) {
        ip->d = d_INF;
      } else if (ip->d < p->deadline && ip->Q - ip->n > 0) {
        q[i] = ip->Q - ip->n;
        ip->suspended = true;
      }
    }

    if (ip->d < d_INF)
      ip->d -= 1;

    /* no data, so it does not make sense to schedule this slice */
    if (slice_buffered_data[i] == 0 && !slice_retransmission_ue[i])
      continue;

    /* insertion sort: sort slices by ascending parameter d */
    int *cur = &L.head;
    while (*cur != -1 && ip->d > ((_edf_int_t *)si->s[*cur]->int_data)->d)
      cur = &L.next[*cur];
    const int next = *cur;
    *cur = i;
    L.next[*cur] = next;
  }

  const int N_RBG = to_rbg(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  const int N_RB_DL = to_prb(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  const int RBGsize = get_min_rb_unit(mod_id, CC_id);
  uint8_t *vrb_map = RC.mac[mod_id]->common_channels[CC_id].vrb_map;
  uint8_t rbgalloc_mask[N_RBG_MAX];
  int n_rbg_sched = 0;
  for (int i = 0; i < N_RBG; i++) {
    // calculate mask: init to one + "AND" with vrb_map:
    // if any RB in vrb_map is blocked (1), the current RBG will be 0
    rbgalloc_mask[i] = 1;
    for (int j = 0; j < RBGsize && RBGsize * i + j < N_RB_DL; j++)
      rbgalloc_mask[i] &= !vrb_map[RBGsize * i + j];
    n_rbg_sched += rbgalloc_mask[i];
  }

  extern int get_rbg_size_last(module_id_t mod_id, int CC_id); // in pre_processor.c
  const int RBGlastsize = get_rbg_size_last(mod_id, CC_id);
  for (int i = L.head; i >= 0; i = L.next[i]) {
    uint32_t quota = q[i]; /* can use this number of RBs! */

    /* TODO get correct mask for up to 'quota' RBs, then call scheduler */
  }
}

pp_impl_param_t edf_dl_init(module_id_t mod_id, int CC_id)
{
  slice_info_t *si = calloc(1, sizeof(slice_info_t));
  DevAssert(si);

  si->num = 0;
  si->s = calloc(MAX_EDF_SLICES, sizeof(*si->s));
  DevAssert(si->s);
  for (int i = 0; i < MAX_MOBILES_PER_ENB; ++i)
    si->UE_assoc_slice[i] = -1;

  /* insert default slice, all resources */
  edf_slice_param_t *dlp = malloc(sizeof(*dlp));
  DevAssert(dlp);
  dlp->deadline = 1;
  dlp->guaranteed_prbs = to_prb(RC.mac[mod_id]->common_channels[CC_id].mib->message.dl_Bandwidth);
  dlp->max_replenish = 0;
  dlp->noverride = 1;
  dlp->loverride[0] = 0;
  DevAssert(dlp->loverride);
  default_sched_dl_algo_t *algo = &RC.mac[mod_id]->pre_processor_dl.dl_algo;
  algo->data = NULL;
  const int rc = addmod_edf_slice_dl(si, 0, strdup("common (all UEs)"), algo, dlp);
  DevAssert(0 == rc);
  const UE_list_t *UE_list = &RC.mac[mod_id]->UE_info.list;
  for (int UE_id = UE_list->head; UE_id >= 0; UE_id = UE_list->next[UE_id])
    slicing_add_UE(si, UE_id);

  pp_impl_param_t edf;
  edf.algorithm = EDF_SLICING;
#pragma warning "EDF scheduler: UE operations need to consider common slice"
  edf.add_UE = slicing_add_UE;
  edf.remove_UE = slicing_remove_UE;
  edf.move_UE = slicing_move_UE;
  edf.addmod_slice = addmod_edf_slice_dl;
  edf.remove_slice = remove_nvs_slice_dl;
  edf.dl = dlsch_scheduler_pre_processor;
  // current DL algo becomes default scheduler
  edf.dl_algo = *algo;
  edf.destroy = edf_destroy;
  edf.slices = si;

  return edf;
}
