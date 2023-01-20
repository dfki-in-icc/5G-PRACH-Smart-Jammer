#include "common/utils/simple_executable.h"
#include "executables/softmodem-common.h"
#include "common/utils/ocp_itti/intertask_interface.h"
#include "openair3/ocp-gtpu/gtp_itf.h"
#include "openair2/E1AP/e1ap.h"
#include "common/ran_context.h"
#include "nfapi/oai_integration/vendor_ext.h"
#include "openair2/F1AP/f1ap_common.h"
#include "openair2/GNB_APP/gnb_config.h"

unsigned char NB_eNB_INST = 1;
RAN_CONTEXT_t RC;
THREAD_STRUCT thread_struct;
uint64_t downlink_frequency[MAX_NUM_CCs][4];
int32_t uplink_frequency_offset[MAX_NUM_CCs][4];
int asn1_xer_print;
int oai_exit = 0;
instance_t CUuniqInstance = 0;
RRC_release_list_t rrc_release_info;

void exit_function(const char *file, const char *function, const int line, const char *s)
{
}

nfapi_mode_t nfapi_mod = -1;

void nfapi_setmode(nfapi_mode_t nfapi_mode)
{
  nfapi_mod = nfapi_mode;
}

nfapi_mode_t nfapi_getmode(void)
{
  return nfapi_mod;
}

ngran_node_t get_node_type()
{
  return ngran_gNB_CUUP;
}

rlc_op_status_t rlc_data_req(const protocol_ctxt_t *const,
                             const srb_flag_t,
                             const MBMS_flag_t,
                             const rb_id_t,
                             const mui_t,
                             const confirm_t,
                             const sdu_size_t,
                             mem_block_t *const,
                             const uint32_t *const,
                             const uint32_t *const)
{
  abort();
  return 0;
}

int nr_rlc_get_available_tx_space(const rnti_t rntiP, const logical_chan_id_t channel_idP)
{
  abort();
  return 0;
}

void nr_rlc_bearer_init(NR_RLC_BearerConfig_t *RLC_BearerConfig, NR_RLC_BearerConfig__servedRadioBearer_PR rb_type)
{
  abort();
}

void rrc_gNB_generate_dedicatedRRCReconfiguration(const protocol_ctxt_t *const ctxt_pP, rrc_gNB_ue_context_t *ue_context_pP, NR_CellGroupConfig_t *cell_groupConfig_from_DU)
{
  abort();
}

void nr_rlc_bearer_init_ul_spec(struct NR_LogicalChannelConfig *mac_LogicalChannelConfig)
{
  abort();
}

rlc_op_status_t nr_rrc_rlc_config_asn1_req(const protocol_ctxt_t *const ctxt_pP,
                                           const NR_SRB_ToAddModList_t *const srb2add_listP,
                                           const NR_DRB_ToAddModList_t *const drb2add_listP,
                                           const NR_DRB_ToReleaseList_t *const drb2release_listP,
                                           const LTE_PMCH_InfoList_r9_t *const pmch_InfoList_r9_pP,
                                           struct NR_CellGroupConfig__rlc_BearerToAddModList *rlc_bearer2add_list)
{
  abort();
  return 0;
}

int nr_rrc_gNB_process_GTPV1U_CREATE_TUNNEL_RESP(const protocol_ctxt_t *const ctxt_pP, const gtpv1u_gnb_create_tunnel_resp_t *const create_tunnel_resp_pP)
{
  abort();
  return 0;
}

void nr_drb_config(struct NR_RLC_Config *rlc_Config, NR_RLC_Config_PR rlc_config_pr)
{
  abort();
}

void prepare_and_send_ue_context_modification_f1(rrc_gNB_ue_context_t *ue_context_p, e1ap_bearer_setup_resp_t *e1ap_resp)
{
  abort();
}

f1ap_cudu_inst_t *getCxt(F1_t isCU, instance_t instanceP)
{
  abort();
  return NULL;
}

void fill_DRB_configList(const protocol_ctxt_t *const ctxt_pP, rrc_gNB_ue_context_t *ue_context_pP)
{
  abort();
}

int main(int argc, char **argv)
{
  /// static configuration for NR at the moment
  if (load_configmodule(argc, argv, CONFIG_ENABLECMDLINEONLY) == NULL) {
    exit_fun("[SOFTMODEM] Error, configuration module init failed\n");
  }
  logInit();
  set_softmodem_sighandler();
  itti_init(TASK_MAX, tasks_info);
  int rc;
  rc = itti_create_task(TASK_SCTP, sctp_eNB_task, NULL);
  AssertFatal(rc >= 0, "Create task for SCTP failed\n");
  rc = itti_create_task(TASK_GTPV1_U, gtpv1uTask, NULL);
  AssertFatal(rc >= 0, "Create task for GTPV1U failed\n");
  rc = itti_create_task(TASK_CUUP_E1, E1AP_CUUP_task, NULL);
  AssertFatal(rc >= 0, "Create task for CUUP E1 failed\n");
  pdcp_layer_init();
  MessageDef *msg = RCconfig_NR_CU_E1(true);
  if (msg)
    itti_send_msg_to_task(TASK_CUUP_E1, 0, msg);
  else
    AssertFatal(false, "Send inti to task for E1AP UP failed\n");
  sleep(3600 * 24 * 1000);
  return 0;
}
