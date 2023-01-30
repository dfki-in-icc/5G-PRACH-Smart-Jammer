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

/*! \file rrc_gNB_UE_context.h
 * \brief rrc procedures for UE context
 * \author Lionel GAUTHIER
 * \date 2015
 * \version 1.0
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "common/utils/LOG/log.h"
#include "rrc_gNB_UE_context.h"

RB_HEAD(rrc_nr_ue_tree_s, rrc_gNB_ue_context_s) rrc_ue_head = RB_INITIALIZER(NoTUsed); // ue_context tree key search by rnti

RB_PROTOTYPE(rrc_nr_ue_tree_s, rrc_gNB_ue_context_s, entries, rrc_gNB_compare_ue_rnti_id);

//------------------------------------------------------------------------------
static int rrc_gNB_compare_ue_rnti_id(
                                      struct rrc_gNB_ue_context_s *c1_pP, struct rrc_gNB_ue_context_s *c2_pP)
//------------------------------------------------------------------------------
{
  if (c1_pP->ue_id_rnti > c2_pP->ue_id_rnti) {
    return 1;
  }

  if (c1_pP->ue_id_rnti < c2_pP->ue_id_rnti) {
    return -1;
  }

  return 0;
}

/* Generate the tree management functions */
RB_GENERATE(rrc_nr_ue_tree_s, rrc_gNB_ue_context_s, entries,
            rrc_gNB_compare_ue_rnti_id);



//------------------------------------------------------------------------------
struct rrc_gNB_ue_context_s *
rrc_gNB_allocate_new_UE_context(
                                gNB_RRC_INST *rrc_instance_pP
                                )
//------------------------------------------------------------------------------
{
  struct rrc_gNB_ue_context_s *new_p;
  new_p = (struct rrc_gNB_ue_context_s *)calloc(1, sizeof(*new_p));

  if (new_p == NULL) {
    LOG_E(RRC, "Cannot allocate new ue context\n");
    return NULL;
  }

  new_p->ue_id_rnti = uid_linear_allocator_new(&rrc_instance_pP->uid_allocator);
  new_p->local_uid =  new_p->ue_id_rnti;

  for (int i = 0; i < sizeofArray(new_p->ue_context.pduSession); i++) {
    new_p->ue_context.e_rab[i].xid = -1;
    new_p->ue_context.pduSession[i].xid = -1;
  }

  LOG_D(NR_RRC,"Returning new UE context at %p\n",new_p);
  return(new_p);
}


//------------------------------------------------------------------------------
struct rrc_gNB_ue_context_s *rrc_gNB_get_ue_context(const gNB_RRC_INST *rrc_instance_pP, const ue_id_t UEid)
//------------------------------------------------------------------------------
{
  rrc_gNB_ue_context_t temp = {.ue_id_rnti = UEid};
  struct rrc_gNB_ue_context_s   *ue_context_p = NULL;
  ue_context_p = RB_FIND(rrc_nr_ue_tree_s, &rrc_ue_head, &temp);

  if ( ue_context_p != NULL) {
    return ue_context_p;
  } else {
    /* FIxme: what is this mess !!! */
    RB_FOREACH(ue_context_p, rrc_nr_ue_tree_s, &rrc_ue_head)
      {
        if (ue_context_p->ue_context.rnti == UEid) {
          return ue_context_p;
        }
      }
    return NULL;
  }
}

void rrc_gNB_free_mem_UE_context(struct rrc_gNB_ue_context_s *const ue_context_pP)
//-----------------------------------------------------------------------------
{

  LOG_E(NR_RRC,"Free UE to implement, mem leak here\n");
  free(ue_context_pP);
  //ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_LTE_SCellToAddMod_r10, &ue_context_pP->ue_context.sCell_config[0]);
  //ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_LTE_SCellToAddMod_r10, &ue_context_pP->ue_context.sCell_config[1]);

  // empty the internal fields of the UE context here
}

//------------------------------------------------------------------------------
void rrc_gNB_remove_ue_context(gNB_RRC_INST                *rrc_instance_pP,
                               struct rrc_gNB_ue_context_s *ue_context_pP)
//------------------------------------------------------------------------------
{
  if (rrc_instance_pP == NULL) {
    LOG_E(NR_RRC," Bad RRC instance\n");
    return;
  }

  if (ue_context_pP == NULL) {
    LOG_E(RRC, " Trying to free a NULL UE context\n");
    return;
  }

  RB_REMOVE(rrc_nr_ue_tree_s, &rrc_ue_head, ue_context_pP);
  // what is this mess
  uid_linear_allocator_free(&rrc_instance_pP->uid_allocator, ue_context_pP->local_uid);

}

//-----------------------------------------------------------------------------
// return the ue context if there is already an UE with ue_identityP, NULL otherwise
struct rrc_gNB_ue_context_s *
rrc_gNB_ue_context_random_exist(
                                gNB_RRC_INST                *rrc_instance_pP,
                                const uint64_t               ue_identityP
                                )
//-----------------------------------------------------------------------------
{
  struct rrc_gNB_ue_context_s        *ue_context_p = NULL;
  RB_FOREACH(ue_context_p, rrc_nr_ue_tree_s, &rrc_ue_head)
    {
      if (ue_context_p->ue_context.random_ue_identity == ue_identityP)
        return ue_context_p;
    }
  return NULL;
}

//-----------------------------------------------------------------------------
// return the ue context if there is already an UE with the same S-TMSI, NULL otherwise
struct rrc_gNB_ue_context_s *
rrc_gNB_ue_context_5g_s_tmsi_exist(
                                   gNB_RRC_INST                *rrc_instance_pP,
                                   const uint64_t              s_TMSI
                                   )
//-----------------------------------------------------------------------------
{
  struct rrc_gNB_ue_context_s        *ue_context_p = NULL;
  RB_FOREACH(ue_context_p, rrc_nr_ue_tree_s, &rrc_ue_head)
    {
      LOG_I(NR_RRC, "checking for UE 5G S-TMSI %ld: rnti %d \n", s_TMSI, ue_context_p->ue_context.rnti);

      if (ue_context_p->ue_context.ng_5G_S_TMSI_Part1 == s_TMSI) {
        return ue_context_p;
      }
    }
  return NULL;
}

void rrc_gNB_insert_ue_context(struct rrc_gNB_ue_context_s *ue_context_p)
{
  if (RB_INSERT(rrc_nr_ue_tree_s, &rrc_ue_head, ue_context_p))
    LOG_E(NR_RRC, "try to insert a existing UE %lu\n", ue_context_p->ue_id_rnti);
  else
    LOG_D(NR_RRC, " Created new UE context uid %lu\n", ue_context_p->ue_id_rnti);
}

//-----------------------------------------------------------------------------
// return a new ue context structure if ue_identityP, ctxt_pP->rnti not found in collection
struct rrc_gNB_ue_context_s *rrc_gNB_new_ue_context(const ue_id_t newUEid, gNB_RRC_INST *rrc_instance_pP, const uint64_t ue_identityP)
//-----------------------------------------------------------------------------
{
  struct rrc_gNB_ue_context_s        *ue_context_p = NULL;
  ue_context_p = rrc_gNB_get_ue_context(rrc_instance_pP, newUEid);

  if (ue_context_p) {
    LOG_E(NR_RRC, "try to create a exitsing ue context %lu\n", newUEid);
    return ue_context_p;
  }

  ue_context_p = rrc_gNB_allocate_new_UE_context(rrc_instance_pP);

  if (ue_context_p == NULL) {
    LOG_E(NR_RRC, " Cannot create new UE context, no memory\n");
    return NULL;
  }

  ue_context_p->ue_id_rnti = newUEid; // here ue_id_rnti is just a key, may be something else
  ue_context_p->ue_context.rnti = ue_identityP; // yes duplicate, 1 may be removed ?????????????? what this means ????
  ue_context_p->ue_context.random_ue_identity = newUEid;
  rrc_gNB_insert_ue_context(ue_context_p);
  return ue_context_p;
}

struct rrc_gNB_ue_context_s *rrc_gNB_ue_context_iterator_init() {
  return RB_MIN(rrc_nr_ue_tree_s, &rrc_ue_head);
}

struct rrc_gNB_ue_context_s *rrc_gNB_ue_context_iterator_next(struct rrc_gNB_ue_context_s * it) {
  return RB_NEXT(rrc_nr_ue_tree_s, &rrc_ue_head, it);
}
