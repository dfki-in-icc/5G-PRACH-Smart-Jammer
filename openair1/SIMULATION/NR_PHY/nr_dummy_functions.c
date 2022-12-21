#include "nfapi/oai_integration/vendor_ext.h"


int oai_nfapi_hi_dci0_req(nfapi_hi_dci0_request_t *hi_dci0_req)             { return(0);  }
int oai_nfapi_tx_req(nfapi_tx_request_t *tx_req)                            { return(0);  }
int oai_nfapi_dl_config_req(nfapi_dl_config_request_t *dl_config_req)       { return(0);  }
//int oai_nfapi_ul_config_req(nfapi_ul_config_request_t *ul_config_req)       { return(0);  }
int oai_nfapi_dl_tti_req(nfapi_nr_dl_tti_request_t *dl_config_req)
{
  return (0);
}
int oai_nfapi_tx_data_req(nfapi_nr_tx_data_request_t *tx_data_req)
{
  return (0);
}
int oai_nfapi_ul_dci_req(nfapi_nr_ul_dci_request_t *ul_dci_req)
{
  return (0);
}
int oai_nfapi_ul_tti_req(nfapi_nr_ul_tti_request_t *ul_tti_req)
{
  return (0);
}
int oai_nfapi_nr_rx_data_indication(nfapi_nr_rx_data_indication_t *ind)
{
  return (0);
}
int oai_nfapi_nr_crc_indication(nfapi_nr_crc_indication_t *ind)
{
  return (0);
}
int oai_nfapi_nr_srs_indication(nfapi_nr_srs_indication_t *ind)
{
  return (0);
}
int oai_nfapi_nr_uci_indication(nfapi_nr_uci_indication_t *ind)
{
  return (0);
}
int oai_nfapi_nr_rach_indication(nfapi_nr_rach_indication_t *ind)
{
  return (0);
}

int32_t get_uldl_offset(int nr_bandP)
{
  return (0);
}
nfapi_mode_t nfapi_mod;

int dummy_nr_ue_dl_indication(nr_downlink_indication_t *dl_info)
{
  return (0);
}
int dummy_nr_ue_ul_indication(nr_uplink_indication_t *ul_info)              { return(0);  }
void cucp_cuup_message_transfer_e1ap_init(gNB_RRC_INST *rrc) {}
void cucp_cuup_message_transfer_direct_init(gNB_RRC_INST *rrc) {}
teid_t newGtpuCreateTunnel(instance_t instance,
                           ue_id_t ue_id,
                           int incoming_bearer_id,
                           int outgoing_bearer_id,
                           teid_t outgoing_teid,
                           int qfi,
                           transport_layer_addr_t remoteAddr,
                           int port,
                           gtpCallback callBack,
                           gtpCallbackSDAP callBackSDAP) {
  return 0;
}

int newGtpuDeleteAllTunnels(instance_t instance, ue_id_t ue_id) {
  return 0;
}

instance_t gtpv1Init(openAddr_t context) {return 0;}

void GtpuUpdateTunnelOutgoingPair(instance_t instance,
                                  ue_id_t ue_id,
                                  ebi_t bearer_id,
                                  teid_t newOutgoingTeid,
                                  transport_layer_addr_t newRemoteAddr) {}

int
gtpv1u_create_ngu_tunnel(
  const instance_t instanceP,
  const gtpv1u_gnb_create_tunnel_req_t *  const create_tunnel_req_pP,
        gtpv1u_gnb_create_tunnel_resp_t * const create_tunnel_resp_pP){
  return 0;
}

int
gtpv1u_update_ngu_tunnel(
  const instance_t                              instanceP,
  const gtpv1u_gnb_create_tunnel_req_t *const  create_tunnel_req_pP,
  const ue_id_t                                 ue_id 
){
  return 0;
}

int gtpv1u_delete_s1u_tunnel(const instance_t instance, const gtpv1u_enb_delete_tunnel_req_t *const req_pP) {
  return 0;
}

int gtpv1u_delete_ngu_tunnel( const instance_t instance,
			      gtpv1u_gnb_delete_tunnel_req_t *req) {
  return 0;
}

int
gtpv1u_create_s1u_tunnel(
  const instance_t                              instanceP,
  const gtpv1u_enb_create_tunnel_req_t *const  create_tunnel_req_pP,
  gtpv1u_enb_create_tunnel_resp_t *const create_tunnel_resp_pP
) {
  return 0;
}

