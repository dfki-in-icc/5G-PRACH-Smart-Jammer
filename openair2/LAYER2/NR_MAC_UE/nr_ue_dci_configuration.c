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

/* \file nr_ue_dci_configuration.c
 * \brief functions for generating dci search procedures based on RRC Serving Cell Group Configuration
 * \author R. Knopp, G. Casati
 * \date 2020
 * \version 0.2
 * \company Eurecom, Fraunhofer IIS
 * \email: knopp@eurecom.fr, guido.casati@iis.fraunhofer.de
 * \note
 * \warning
 */

#include "mac_proto.h"
#include "mac_defs.h"
#include "assertions.h"
#include "LAYER2/NR_MAC_UE/mac_extern.h"
#include "mac_defs.h"
#include "common/utils/nr/nr_common.h"
#include "executables/softmodem-common.h"
#include <stdio.h>
#include "nfapi_nr_interface.h"

#ifdef NR_PDCCH_DCI_TOOLS_DEBUG
#define LOG_DCI_D(a...) printf("\t\t<-NR_PDCCH_DCI_TOOLS_DEBUG (nr_extract_dci_info) ->" a)
#else 
#define LOG_DCI_D(a...)
#endif
#define LOG_DCI_PARM(a...) LOG_D(PHY,"\t<-NR_PDCCH_DCI_TOOLS_DEBUG (nr_generate_ue_ul_dlsch_params_from_dci)" a)

//#define DEBUG_DCI

void fill_dci_search_candidates(NR_SearchSpace_t *ss,
                                fapi_nr_dl_config_dci_dl_pdu_rel15_t *rel15,
                                int slot, int rnti) {

  LOG_D(NR_MAC,"Filling search candidates for DCI\n");

  uint8_t aggregation;
  uint8_t number_of_candidates=0;
  rel15->number_of_candidates=0;
  int i=0;
  uint32_t Y = 0;
  if (slot >= 0)
    Y = get_Y(ss, slot, rnti);
  for (int maxL=16;maxL>0;maxL>>=1) {
    find_aggregation_candidates(&aggregation,
                                &number_of_candidates,
                                ss,maxL);

    if (number_of_candidates>0) {
      LOG_D(NR_MAC,"L %d, number of candidates %d, aggregation %d\n",maxL,number_of_candidates,aggregation);
      rel15->number_of_candidates += number_of_candidates;
      int N_cce_sym = 0; // nb of rbs of coreset per symbol
      for (int i=0;i<6;i++) {
        for (int t=0;t<8;t++) {
          N_cce_sym+=((rel15->coreset.frequency_domain_resource[i]>>t)&1);
        }
      }
      int N_cces = N_cce_sym*rel15->coreset.duration;
      for (int j=0; j<number_of_candidates; i++,j++) {
        int first_cce = aggregation * (( Y + CEILIDIV((j*N_cces),(aggregation*number_of_candidates)) + 0 ) % CEILIDIV(N_cces,aggregation));
        LOG_D(NR_MAC,"Candidate %d of %d first_cce %d (L %d N_cces %d Y %d)\n", j, number_of_candidates, first_cce, aggregation, N_cces, Y);
        rel15->CCE[i] = first_cce;
        rel15->L[i] = aggregation;
      }
    }
  }
}

void config_dci_pdu(NR_UE_MAC_INST_t *mac, fapi_nr_dl_config_dci_dl_pdu_rel15_t *rel15, fapi_nr_dl_config_request_t *dl_config, int rnti_type, int ss_id)
{

  uint16_t monitoringSymbolsWithinSlot = 0;
  int sps = 0;

  AssertFatal(mac->scc == NULL || mac->scc_SIB == NULL, "both scc and scc_SIB cannot be non-null\n");

  NR_UE_DL_BWP_t *current_DL_BWP = &mac->current_DL_BWP;
  NR_UE_UL_BWP_t *current_UL_BWP = &mac->current_UL_BWP;
  NR_BWP_Id_t dl_bwp_id = current_DL_BWP ? current_DL_BWP->bwp_id : 0;
  NR_ServingCellConfigCommon_t *scc = mac->scc;
  NR_ServingCellConfigCommonSIB_t *scc_SIB = mac->scc_SIB;
  NR_BWP_DownlinkCommon_t *initialDownlinkBWP=NULL;
  NR_BWP_UplinkCommon_t *initialUplinkBWP=NULL;

  if (scc!=NULL || scc_SIB != NULL) {
    initialDownlinkBWP =  scc!=NULL ? scc->downlinkConfigCommon->initialDownlinkBWP : &scc_SIB->downlinkConfigCommon.initialDownlinkBWP;
    initialUplinkBWP = scc!=NULL ? scc->uplinkConfigCommon->initialUplinkBWP : &scc_SIB->uplinkConfigCommon->initialUplinkBWP;
  }

  NR_SearchSpace_t *ss;
  NR_ControlResourceSet_t *coreset;
  if(ss_id>=0) {
    if (rnti_type == NR_RNTI_TC || rnti_type == NR_RNTI_RA) {
      ss = mac->ra.ss;
      AssertFatal(mac->ra.ss->searchSpaceId == ss_id,"Search Space id %d does not correspond to the one in ra->ss %ld for RA procedures\n",
                  ss_id,mac->ra.ss->searchSpaceId);
    }
    else
      ss = mac->SSpace[dl_bwp_id][ss_id-1];
  }
  else
    ss = mac->search_space_zero;

  uint8_t coreset_id = *ss->controlResourceSetId;

  if(coreset_id > 0) {
    coreset = mac->coreset[dl_bwp_id][coreset_id - 1];
    rel15->coreset.CoreSetType = NFAPI_NR_CSET_CONFIG_PDCCH_CONFIG;
  } else {
    coreset = mac->coreset0;
    if(rnti_type == NR_RNTI_SI) {
      rel15->coreset.CoreSetType = NFAPI_NR_CSET_CONFIG_MIB_SIB1;
    } else {
      rel15->coreset.CoreSetType = NFAPI_NR_CSET_CONFIG_PDCCH_CONFIG_CSET_0;
    }
  }

  rel15->coreset.duration = coreset->duration;

  for (int i = 0; i < 6; i++)
    rel15->coreset.frequency_domain_resource[i] = coreset->frequencyDomainResources.buf[i];
  rel15->coreset.CceRegMappingType = coreset->cce_REG_MappingType.present ==
    NR_ControlResourceSet__cce_REG_MappingType_PR_interleaved ? FAPI_NR_CCE_REG_MAPPING_TYPE_INTERLEAVED : FAPI_NR_CCE_REG_MAPPING_TYPE_NON_INTERLEAVED;
  if (rel15->coreset.CceRegMappingType == FAPI_NR_CCE_REG_MAPPING_TYPE_INTERLEAVED) {
    struct NR_ControlResourceSet__cce_REG_MappingType__interleaved *interleaved = coreset->cce_REG_MappingType.choice.interleaved;
    rel15->coreset.RegBundleSize = (interleaved->reg_BundleSize == NR_ControlResourceSet__cce_REG_MappingType__interleaved__reg_BundleSize_n6) ? 6 : (2 + interleaved->reg_BundleSize);
    rel15->coreset.InterleaverSize = (interleaved->interleaverSize == NR_ControlResourceSet__cce_REG_MappingType__interleaved__interleaverSize_n6) ? 6 : (2 + interleaved->interleaverSize);
    rel15->coreset.ShiftIndex = interleaved->shiftIndex != NULL ? *interleaved->shiftIndex : mac->physCellId;
  } else {
    rel15->coreset.RegBundleSize = 0;
    rel15->coreset.InterleaverSize = 0;
    rel15->coreset.ShiftIndex = 0;
  }

  rel15->coreset.precoder_granularity = coreset->precoderGranularity;

  // Scrambling RNTI
  if (coreset->pdcch_DMRS_ScramblingID) {
    rel15->coreset.pdcch_dmrs_scrambling_id = *coreset->pdcch_DMRS_ScramblingID;
    rel15->coreset.scrambling_rnti = mac->crnti;
  } else {
    rel15->coreset.pdcch_dmrs_scrambling_id = mac->physCellId;
    rel15->coreset.scrambling_rnti = 0;
  }
  // loop over RNTI type and configure resource allocation for DCI
  for (int i = 0; i < rel15->num_dci_options; i++) {
    rel15->dci_type_options[i] = ss->searchSpaceType->present;
    const int dci_format = rel15->dci_format_options[i];
    uint16_t alt_size = 0;
    if(current_DL_BWP) {

      // computing alternative size for padding
      dci_pdu_rel15_t temp_pdu;
      if(dci_format == NR_DL_DCI_FORMAT_1_0)
        alt_size = nr_dci_size(initialDownlinkBWP,initialUplinkBWP,
                               current_DL_BWP, current_UL_BWP,
                               mac->cg, &temp_pdu,
                               NR_UL_DCI_FORMAT_0_0, rnti_type, coreset_id, dl_bwp_id,
                               ss->searchSpaceType->present, mac->type0_PDCCH_CSS_config.num_rbs, 0);
      if(dci_format == NR_UL_DCI_FORMAT_0_0)
        alt_size = nr_dci_size(initialDownlinkBWP,initialUplinkBWP,
                               current_DL_BWP, current_UL_BWP,
                               mac->cg, &temp_pdu,
                               NR_DL_DCI_FORMAT_1_0, rnti_type, coreset_id, dl_bwp_id,
                               ss->searchSpaceType->present, mac->type0_PDCCH_CSS_config.num_rbs, 0);
    }

    rel15->dci_length_options[i] = nr_dci_size(initialDownlinkBWP,initialUplinkBWP,
                                               current_DL_BWP, current_UL_BWP,
                                               mac->cg, &mac->def_dci_pdu_rel15[dci_format],
                                               dci_format, NR_RNTI_TC, coreset_id, dl_bwp_id, 
                                               ss->searchSpaceType->present, mac->type0_PDCCH_CSS_config.num_rbs, alt_size);

    rel15->BWPStart = coreset_id == 0 ? mac->type0_PDCCH_CSS_config.cset_start_rb : current_DL_BWP->BWPStart;
    rel15->BWPSize = coreset_id == 0 ? mac->type0_PDCCH_CSS_config.num_rbs : current_DL_BWP->BWPSize;

    switch(rnti_type) {
      case NR_RNTI_C:
        // we use DL BWP dedicated
        sps = current_DL_BWP->cyclicprefix ? 12 : 14;
        // for SPS=14 8 MSBs in positions 13 down to 6
        monitoringSymbolsWithinSlot = (ss->monitoringSymbolsWithinSlot->buf[0]<<(sps-8)) | (ss->monitoringSymbolsWithinSlot->buf[1]>>(16-sps));
        rel15->rnti = mac->crnti;
        rel15->SubcarrierSpacing = current_DL_BWP->scs;
        break;
      case NR_RNTI_RA:
        // we use the initial DL BWP
        sps = current_DL_BWP->cyclicprefix == NULL ? 14 : 12;
        monitoringSymbolsWithinSlot = (ss->monitoringSymbolsWithinSlot->buf[0]<<(sps-8)) | (ss->monitoringSymbolsWithinSlot->buf[1]>>(16-sps));
        rel15->rnti = mac->ra.ra_rnti;
        rel15->SubcarrierSpacing = current_DL_BWP->scs;
        break;
      case NR_RNTI_P:
        break;
      case NR_RNTI_CS:
        break;
      case NR_RNTI_TC:
        // we use the initial DL BWP
        sps = current_DL_BWP->cyclicprefix == NULL ? 14 : 12;
        monitoringSymbolsWithinSlot = (ss->monitoringSymbolsWithinSlot->buf[0]<<(sps-8)) | (ss->monitoringSymbolsWithinSlot->buf[1]>>(16-sps));
        rel15->rnti = mac->ra.t_crnti;
        rel15->SubcarrierSpacing = current_DL_BWP->scs;
        break;
      case NR_RNTI_SP_CSI:
        break;
      case NR_RNTI_SI:
        sps=14;
        // for SPS=14 8 MSBs in positions 13 down to 6
        monitoringSymbolsWithinSlot = (ss->monitoringSymbolsWithinSlot->buf[0]<<(sps-8)) | (ss->monitoringSymbolsWithinSlot->buf[1]>>(16-sps));

        rel15->rnti = SI_RNTI; // SI-RNTI - 3GPP TS 38.321 Table 7.1-1: RNTI values

        if(mac->frequency_range == FR1)
          rel15->SubcarrierSpacing = mac->mib->subCarrierSpacingCommon;
        else
          rel15->SubcarrierSpacing = mac->mib->subCarrierSpacingCommon + 2;
        break;
      case NR_RNTI_SFI:
        break;
      case NR_RNTI_INT:
        break;
      case NR_RNTI_TPC_PUSCH:
        break;
      case NR_RNTI_TPC_PUCCH:
        break;
      case NR_RNTI_TPC_SRS:
        break;
      default:
        break;
    }
  }
  for (int i = 0; i < sps; i++) {
    if ((monitoringSymbolsWithinSlot >> (sps - 1 - i)) & 1) {
      rel15->coreset.StartSymbolIndex = i;
      break;
    }
  }
  #ifdef DEBUG_DCI
    LOG_D(MAC, "[DCI_CONFIG] Configure DCI PDU: rnti_type %d BWPSize %d BWPStart %d rel15->SubcarrierSpacing %d rel15->dci_format %d rel15->dci_length %d sps %d monitoringSymbolsWithinSlot %d \n",
      rnti_type,
      rel15->BWPSize,
      rel15->BWPStart,
      rel15->SubcarrierSpacing,
      rel15->dci_format_options[0],
      rel15->dci_length_options[0],
      sps,
      monitoringSymbolsWithinSlot);
  #endif
  // add DCI
  dl_config->dl_config_list[dl_config->number_pdus].pdu_type = FAPI_NR_DL_CONFIG_TYPE_DCI;
  dl_config->number_pdus += 1;
}

void ue_dci_configuration(NR_UE_MAC_INST_t *mac, fapi_nr_dl_config_request_t *dl_config, frame_t frame, int slot) {

  RA_config_t *ra = &mac->ra;
  int ss_id;

  uint8_t bwp_id = mac->current_DL_BWP.bwp_id;
  //NR_ServingCellConfig_t *scd = mac->scg->spCellConfig->spCellConfigDedicated;
  NR_BWP_DownlinkDedicated_t *bwpd  = (bwp_id>0) ? mac->DLbwp[bwp_id-1]->bwp_Dedicated : mac->cg->spCellConfig->spCellConfigDedicated->initialDownlinkBWP;
  NR_BWP_DownlinkCommon_t *bwp_Common = (bwp_id>0) ? mac->DLbwp[bwp_id-1]->bwp_Common : &mac->scc_SIB->downlinkConfigCommon.initialDownlinkBWP;

  LOG_D(NR_MAC, "[DCI_CONFIG] ra_rnti %p (%x) crnti %p (%x) t_crnti %p (%x)\n", &ra->ra_rnti, ra->ra_rnti, &mac->crnti, mac->crnti, &ra->t_crnti, ra->t_crnti);

  // loop over all available SS for bwp_id
  if (bwpd) {
      for (ss_id = 1; ss_id <= FAPI_NR_MAX_SS; ss_id++){

        if(mac->SSpace[bwp_id][ss_id-1]==NULL) {
          continue;
        }

	LOG_D(NR_MAC, "[DCI_CONFIG] ss_id %d\n",ss_id);
	NR_SearchSpace_t *ss = mac->SSpace[bwp_id][ss_id-1];
        AssertFatal(ss_id == ss->searchSpaceId,"SS IDs don't correspond\n");
	fapi_nr_dl_config_dci_dl_pdu_rel15_t *rel15 = &dl_config->dl_config_list[dl_config->number_pdus].dci_config_pdu.dci_config_rel15;
	NR_SetupRelease_PDCCH_ConfigCommon_t *pdcch_ConfigCommon = bwp_Common->pdcch_ConfigCommon;
	struct NR_PhysicalCellGroupConfig *phy_cgc = mac->cg->physicalCellGroupConfig;
	switch (ss->searchSpaceType->present){
	case NR_SearchSpace__searchSpaceType_PR_common:
	  // this is for CSSs, we use BWP common and pdcch_ConfigCommon

	  if (ss->searchSpaceType->choice.common->dci_Format0_0_AndFormat1_0){
	    // check available SS IDs
	    if (pdcch_ConfigCommon->choice.setup->ra_SearchSpace){
	      if (ss->searchSpaceId == *pdcch_ConfigCommon->choice.setup->ra_SearchSpace){
		switch(ra->ra_state){
		case WAIT_RAR:
		  LOG_D(NR_MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type1-PDCCH common random access search space (RA-Msg2)\n");
		  rel15->num_dci_options = 1;
		  rel15->dci_format_options[0] = NR_DL_DCI_FORMAT_1_0;
		  if (get_softmodem_params()->sa) {
		    config_dci_pdu(mac, rel15, dl_config, NR_RNTI_RA, -1);
		  } else {
		    config_dci_pdu(mac, rel15, dl_config, NR_RNTI_RA, ss_id);
		  }
		  fill_dci_search_candidates(ss, rel15, -1, -1);
		  break;
		case WAIT_CONTENTION_RESOLUTION:
		  LOG_D(NR_MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type1-PDCCH common random access search space (RA-Msg4)\n");
		  rel15->num_dci_options = 1;
		  rel15->dci_format_options[0] = NR_DL_DCI_FORMAT_1_0;
		  config_dci_pdu(mac, rel15, dl_config, NR_RNTI_TC, -1);
		  fill_dci_search_candidates(ss, rel15, -1, -1);
		  break;
		default:
		  break;
		}
	      }
	    }
	    if (pdcch_ConfigCommon->choice.setup->searchSpaceSIB1){
	      if (ss->searchSpaceId == *pdcch_ConfigCommon->choice.setup->searchSpaceSIB1){
		// Configure monitoring of PDCCH candidates in Type0-PDCCH common search space on the MCG
		//LOG_W(MAC, "[DCI_CONFIG] This seach space should not be configured yet...");
	      }
	    }
	    if (pdcch_ConfigCommon->choice.setup->searchSpaceOtherSystemInformation){
	      if (ss->searchSpaceId == *pdcch_ConfigCommon->choice.setup->searchSpaceOtherSystemInformation){
		// Configure monitoring of PDCCH candidates in Type0-PDCCH common search space on the MCG
		//LOG_W(MAC, "[DCI_CONFIG] This seach space should not be configured yet...");
	      }
	    }
	    if (pdcch_ConfigCommon->choice.setup->pagingSearchSpace){
	      if (ss->searchSpaceId == *pdcch_ConfigCommon->choice.setup->pagingSearchSpace){
		// Configure monitoring of PDCCH candidates in Type2-PDCCH common search space on the MCG
		//LOG_W(MAC, "[DCI_CONFIG] This seach space should not be configured yet...");
	      }
	    }
	    if (phy_cgc){
	      if (phy_cgc->cs_RNTI){
		LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type3-PDCCH common search space for dci_Format0_0_AndFormat1_0 with CRC scrambled by CS-RNTI...\n");
		LOG_W(MAC, "[DCI_CONFIG] This RNTI should not be configured yet...\n");
	      }
	      if (phy_cgc->ext1){
		if (phy_cgc->ext1->mcs_C_RNTI){
		  LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in user specific search space for dci_Format0_0_AndFormat1_0 with CRC scrambled by MCS-C-RNTI...\n");
		  LOG_W(MAC, "[DCI_CONFIG] This RNTI should not be configured yet...\n");
		}
	      }
	    }
	  } // end DCI 00 and 01
	  // DCI 2_0
	  if (ss->searchSpaceType->choice.common->dci_Format2_0){
	    LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type3-PDCCH common search space for DCI format 2_0 with CRC scrambled by SFI-RNTI \n");
	    LOG_W(MAC, "[DCI_CONFIG] This format should not be configured yet...\n");
	  }
	  // DCI 2_1
	  if (ss->searchSpaceType->choice.common->dci_Format2_1){
	    LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type3-PDCCH common search space for DCI format 2_1 with CRC scrambled by INT-RNTI \n");
	    LOG_W(MAC, "[DCI_CONFIG] This format should not be configured yet...\n");
	  }
	  // DCI 2_2
	  if (ss->searchSpaceType->choice.common->dci_Format2_2){
	    LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type3-PDCCH common search space for DCI format 2_2 with CRC scrambled by TPC-RNTI \n");
	    LOG_W(MAC, "[DCI_CONFIG] This format should not be configured yet...\n");
	  }
	  // DCI 2_3
	  if (ss->searchSpaceType->choice.common->dci_Format2_3){
	    LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in Type3-PDCCH common search space for DCI format 2_3 with CRC scrambled by TPC-SRS-RNTI \n");
	    LOG_W(MAC, "[DCI_CONFIG] This format should not be configured yet...\n");
	  }

	  break;
	case NR_SearchSpace__searchSpaceType_PR_ue_Specific:
	  // this is an USS
	  if (ss->searchSpaceType->choice.ue_Specific &&
	      ss->searchSpaceType->choice.ue_Specific->dci_Formats == NR_SearchSpace__searchSpaceType__ue_Specific__dci_Formats_formats0_1_And_1_1 &&
	      (ra->ra_state == RA_SUCCEEDED || get_softmodem_params()->phy_test) &&
              mac->crnti > 0) {
	      // Monitors DCI 01 and 11 scrambled with C-RNTI, or CS-RNTI(s), or SP-CSI-RNTI
            LOG_D(NR_MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in the user specific search space\n");
            rel15->num_dci_options = 2;
            rel15->dci_format_options[0] = NR_DL_DCI_FORMAT_1_1;
            rel15->dci_format_options[1] = NR_UL_DCI_FORMAT_0_1;
            config_dci_pdu(mac, rel15, dl_config, NR_RNTI_C, ss_id);
            fill_dci_search_candidates(ss, rel15, slot, mac->crnti);

//#ifdef DEBUG_DCI
		LOG_D(NR_MAC, "[DCI_CONFIG] ss %d ue_Specific %p searchSpaceType->present %d dci_Formats %d\n",
		      ss_id,
		      ss->searchSpaceType->choice.ue_Specific,
		      (int)ss->searchSpaceType->present,
		      (int)ss->searchSpaceType->choice.ue_Specific->dci_Formats);
//#endif
	  }
	  if (phy_cgc){
            if (phy_cgc->cs_RNTI){
		  LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in user specific search space for dci_Format0_0_AndFormat1_0 with CRC scrambled by CS-RNTI...\n");
		  LOG_W(MAC, "[DCI_CONFIG] This RNTI should not be configured yet...");
            }
            if (phy_cgc->sp_CSI_RNTI){
		  LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in user specific search space for dci_Format0_0_AndFormat1_0 with CRC scrambled by SP-CSI-RNTI...\n");
		  LOG_W(MAC, "[DCI_CONFIG] This RNTI should not be configured yet...");
            }
	    if (phy_cgc->ext1 &&
		phy_cgc->ext1->mcs_C_RNTI){
		    LOG_D(MAC, "[DCI_CONFIG] Configure monitoring of PDCCH candidates in user specific search space for dci_Format0_0_AndFormat1_0 with CRC scrambled by MCS-C-RNTI...\n");
		    LOG_W(MAC, "[DCI_CONFIG] This RNTI should not be configured yet...");
            }
	  }
	  break;
	default:
	  AssertFatal(1 == 0, "[DCI_CONFIG] Unrecognized search space type...");
	  break;
	} // switch searchspace
      } // for ss_id
  }
  else {
    AssertFatal(1==0,"Handle DCI searching when CellGroup without dedicated BWP\n");
  }
}
