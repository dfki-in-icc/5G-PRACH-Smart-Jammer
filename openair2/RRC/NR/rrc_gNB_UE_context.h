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

/*! \file rrc_eNB_UE_context.h
 * \brief rrc procedures for UE context
 * \author Lionel GAUTHIER
 * \date 2015
 * \version 1.0
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */
#ifndef __RRC_GNB_UE_CONTEXT_H__
#define __RRC_GNB_UE_CONTEXT_H__

#include "collection/tree.h"
#include "COMMON/platform_types.h"
#include "nr_rrc_defs.h"
struct rrc_gNB_ue_context_s*
rrc_gNB_allocate_new_UE_context(
  gNB_RRC_INST* rrc_instance_pP
);

struct rrc_gNB_ue_context_s* rrc_gNB_get_ue_context(const gNB_RRC_INST* rrc_instance_pP, const ue_id_t UEid);

void rrc_gNB_free_mem_UE_context(struct rrc_gNB_ue_context_s         *const ue_context_pP
);

void rrc_gNB_remove_ue_context(
  gNB_RRC_INST*                rrc_instance_pP,
  struct rrc_gNB_ue_context_s* ue_context_pP
);

struct rrc_gNB_ue_context_s *
rrc_gNB_ue_context_random_exist(
  gNB_RRC_INST                *rrc_instance_pP,
  const uint64_t               ue_identityP
);

struct rrc_gNB_ue_context_s *
rrc_gNB_ue_context_5g_s_tmsi_exist(
    gNB_RRC_INST                *rrc_instance_pP,
    const uint64_t              s_TMSI
);
void rrc_gNB_insert_ue_context(struct rrc_gNB_ue_context_s* ue_context_p);
struct rrc_gNB_ue_context_s* rrc_gNB_new_ue_context(const ue_id_t, gNB_RRC_INST* rrc_instance_pP, const uint64_t ue_identityP);

struct rrc_gNB_ue_context_s *rrc_gNB_ue_context_iterator_init();

struct rrc_gNB_ue_context_s *rrc_gNB_ue_context_iterator_next(struct rrc_gNB_ue_context_s * it);

#endif
