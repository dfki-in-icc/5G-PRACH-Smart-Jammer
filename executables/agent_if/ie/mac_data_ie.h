#ifndef MAC_DATA_INFORMATION_ELEMENTS_H
#define MAC_DATA_INFORMATION_ELEMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 9 Information Elements (IE) , RIC Event Trigger Definition, RIC Action Definition, RIC Indication Header, RIC Indication Message, RIC Call Process ID, RIC Control Header, RIC Control Message, RIC Control Outcome and RAN Function Definition defined by ORAN-WG3.E2SM-v01.00.00 at Section 5
 */


#include <stdbool.h>
#include <stdint.h>

//////////////////////////////////////
// RIC Event Trigger Definition
/////////////////////////////////////

typedef struct {
  uint32_t ms;
} mac_event_trigger_t;

void free_mac_event_trigger(mac_event_trigger_t* src); 

mac_event_trigger_t cp_mac_event_trigger( mac_event_trigger_t* src);

bool eq_mac_event_trigger(mac_event_trigger_t* m0, mac_event_trigger_t* m1);



//////////////////////////////////////
// RIC Action Definition 
/////////////////////////////////////


typedef struct {
  uint32_t dummy;  
} mac_action_def_t;

void free_mac_action_def(mac_action_def_t* src); 

mac_action_def_t cp_mac_action_def(mac_action_def_t* src);

bool eq_mac_action_def(mac_event_trigger_t* m0,  mac_event_trigger_t* m1);



//////////////////////////////////////
// RIC Indication Header 
/////////////////////////////////////


typedef struct{
  uint32_t dummy;  
} mac_ind_hdr_t;

void free_mac_ind_hdr(mac_ind_hdr_t* src); 

mac_ind_hdr_t cp_mac_ind_hdr(mac_ind_hdr_t const* src);

bool eq_mac_ind_hdr(mac_ind_hdr_t* m0, mac_ind_hdr_t* m1);

//////////////////////////////////////
// RIC Indication Message 
/////////////////////////////////////

typedef struct
{
  uint64_t dl_aggr_tbs;
  uint64_t ul_aggr_tbs;
  uint64_t dl_aggr_bytes_sdus;
  uint64_t ul_aggr_bytes_sdus;
  uint64_t dl_curr_tbs;
  uint64_t ul_curr_tbs;
  uint64_t dl_sched_rb;
  uint64_t ul_sched_rb;
 
  float pusch_snr; //: float = -64;
  float pucch_snr; //: float = -64;

  uint32_t rnti;
  uint32_t dl_aggr_prb; 
  uint32_t ul_aggr_prb;
  uint32_t dl_aggr_sdus;
  uint32_t ul_aggr_sdus;
  uint32_t dl_aggr_retx_prb;
  uint32_t ul_aggr_retx_prb;

  uint8_t wb_cqi; 
  uint8_t dl_mcs1;
  uint8_t ul_mcs1;
  uint8_t dl_mcs2; 
  uint8_t ul_mcs2; 
  int8_t phr; 
  uint32_t bsr;
  float dl_bler;
  float ul_bler;

  int dl_num_harq;
  int ul_num_harq;
  uint32_t dl_harq[5];
  uint32_t ul_harq[5];

  int16_t frame;
  int16_t slot;
} mac_ue_stats_impl_t;

mac_ue_stats_impl_t cp_mac_ue_stats_impl(mac_ue_stats_impl_t const* src);

typedef struct {
  uint32_t len_ue_stats;
  mac_ue_stats_impl_t* ue_stats;
  int64_t tstamp;
} mac_ind_msg_t;

void free_mac_ind_msg(mac_ind_msg_t* src); 

mac_ind_msg_t cp_mac_ind_msg(mac_ind_msg_t const* src);

bool eq_mac_ind_msg(mac_ind_msg_t* m0, mac_ind_msg_t* m1);


//////////////////////////////////////
// RIC Call Process ID 
/////////////////////////////////////

typedef struct {
  uint32_t dummy;
} mac_call_proc_id_t;

void free_mac_call_proc_id( mac_call_proc_id_t* src); 

mac_call_proc_id_t cp_mac_call_proc_id( mac_call_proc_id_t* src);

bool eq_mac_call_proc_id(mac_call_proc_id_t* m0, mac_call_proc_id_t* m1);

//////////////////////////////////////
// RIC Control Header 
/////////////////////////////////////

typedef struct {
  uint32_t dummy;
} mac_ctrl_hdr_t;

void free_mac_ctrl_hdr( mac_ctrl_hdr_t* src); 

mac_ctrl_hdr_t cp_mac_ctrl_hdr(mac_ctrl_hdr_t* src);

bool eq_mac_ctrl_hdr(mac_ctrl_hdr_t* m0, mac_ctrl_hdr_t* m1);

//////////////////////////////////////
// RIC Control Message 
/////////////////////////////////////

typedef struct {
  uint32_t action;
} mac_ctrl_msg_t;

void free_mac_ctrl_msg( mac_ctrl_msg_t* src); 

mac_ctrl_msg_t cp_mac_ctrl_msg(mac_ctrl_msg_t* src);

bool eq_mac_ctrl_msg(mac_ctrl_msg_t* m0, mac_ctrl_msg_t* m1);


//////////////////////////////////////
// RIC Control Outcome 
/////////////////////////////////////

typedef enum{
  MAC_CTRL_OUT_OK,


  MAC_CTRL_OUT_END
} mac_ctrl_out_e;

typedef struct {
  mac_ctrl_out_e ans;  
} mac_ctrl_out_t;

void free_mac_ctrl_out(mac_ctrl_out_t* src); 

mac_ctrl_out_t cp_mac_ctrl_out(mac_ctrl_out_t* src);

bool eq_mac_ctrl_out(mac_ctrl_out_t* m0, mac_ctrl_out_t* m1);


//////////////////////////////////////
// RAN Function Definition 
/////////////////////////////////////

typedef struct {
  uint32_t dummy;
} mac_func_def_t;

void free_mac_func_def( mac_func_def_t* src); 

mac_func_def_t cp_mac_func_def(mac_func_def_t* src);

bool eq_mac_func_def(mac_func_def_t* m0, mac_func_def_t* m1);


/////////////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////////


/*
 * O-RAN defined 5 Procedures: RIC Subscription, RIC Indication, RIC Control, E2 Setup and RIC Service Update 
 * */


///////////////
/// RIC Subscription
///////////////

typedef struct{
  mac_event_trigger_t et; 
  mac_action_def_t* ad;
} mac_sub_data_t;

///////////////
// RIC Indication
///////////////

typedef struct{
  mac_ind_hdr_t hdr;
  mac_ind_msg_t msg;
  mac_call_proc_id_t* proc_id;
} mac_ind_data_t;

mac_ind_data_t cp_mac_ind_data( mac_ind_data_t const* src);

void free_mac_ind_data(mac_ind_data_t* ind);

///////////////
// RIC Control
///////////////

typedef struct{
  mac_ctrl_hdr_t hdr;
  mac_ctrl_msg_t msg;
} mac_ctrl_req_data_t;

typedef struct{
  mac_ctrl_out_t* out;
} mac_ctrl_out_data_t;

///////////////
// E2 Setup
///////////////

typedef struct{
  mac_func_def_t func_def;
} mac_e2_setup_data_t;

///////////////
// RIC Service Update
///////////////

typedef struct{
  mac_func_def_t func_def;
} mac_ric_service_update_t;

#ifdef __cplusplus
}
#endif

#endif

