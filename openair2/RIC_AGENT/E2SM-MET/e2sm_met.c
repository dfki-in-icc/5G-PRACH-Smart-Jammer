
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include "common/utils/assertions.h"
#include "f1ap_common.h"
#include "ric_agent.h"
#include "e2ap_encoder.h"
#include "e2sm_met.h"
#include "e2ap_generate_messages.h"
#include "INTEGER.h"
#include "OCTET_STRING.h"

//HERE
#include "E2SM_MET_E2SM-MET-RANfunction-Description.h"
#include "E2SM_MET_RIC-ReportStyle-Item.h"
#include "E2SM_MET_RIC-EventTriggerStyle-Item.h"
#include "E2SM_MET_MeasurementInfo-Action-Item.h"

#include "E2SM_MET_SubscriptionID.h"
#include "E2SM_MET_MeasurementData.h"
#include "E2SM_MET_MeasurementRecord.h"
#include "E2SM_MET_MeasurementRecordItem.h"
#include "E2SM_MET_GranularityPeriod.h"
#include "E2SM_MET_E2SM-MET-IndicationMessage.h"
#include "E2SM_MET_E2SM-MET-IndicationHeader.h"
#include "E2SM_MET_E2SM-MET-IndicationHeader-Format1.h"
#include "E2SM_MET_MeasurementInfoList.h"
#include "E2SM_MET_MeasurementInfoItem.h"
#include "E2SM_MET_E2SM-MET-IndicationMessage-Format1.h"
#include "E2SM_MET_GlobalMETnode-ID.h"

#include "E2AP_Cause.h"
#include "E2SM_MET_UETag.h"

#include "met_debug.h"
#include "openair2/LAYER2/NR_MAC_gNB/nr_mac_gNB.h"

extern RAN_CONTEXT_t RC;


// extern f1ap_cudu_inst_t f1ap_cu_inst[MAX_eNB];
// extern int global_e2_node_id(ranid_t ranid, E2AP_GlobalE2node_ID_t* node_id);
// extern RAN_CONTEXT_t RC;
// extern eNB_RRC_KPI_STATS    rrc_kpi_stats;

/**
 ** The main thing with this abstraction is that we need per-SM modules
 ** to handle the details of the function, event trigger, action, etc
 ** definitions... and to actually do all the logic and implement the
 ** inner parts of the message encoding.  generic e2ap handles the rest.
 **/

//Functions of the SM 

// SECTION this section is for Constatnts, global vars definitions and function signatures 

// debug functions 

static int e2sm_met_subscription_add(ric_agent_info_t *ric, ric_subscription_t *sub);
static int e2sm_met_subscription_del(ric_agent_info_t *ric, ric_subscription_t *sub, int force,long *cause,long *cause_detail);
static int e2sm_met_control(ric_agent_info_t *ric,ric_control_t *control);
static char *time_stamp(void);
static int e2sm_met_ricInd_timer_expiry(
        ric_agent_info_t *ric,
        long timer_id,
        ric_ran_function_id_t function_id,
        long request_id,
        long instance_id,
        long action_id,
        uint8_t **outbuf,
        uint32_t *outlen);

static int e2sm_met_gp_timer_expiry(
        ric_agent_info_t *ric,
        long timer_id,
        ric_ran_function_id_t function_id,
        long request_id,
        long instance_id,
        long action_id,
        uint8_t **outbuf,
        uint32_t *outlen);

static E2SM_MET_E2SM_MET_IndicationMessage_t* encode_met_Indication_Msg(ric_agent_info_t* ric, ric_subscription_t *rs);
void encode_e2sm_met_indication_header(ranid_t ranid, E2SM_MET_E2SM_MET_IndicationHeader_t *ihead) ;

//TODONOW set the subID and granularity period somewhere 
// E2SM_MET_SubscriptionID_t    g_subscriptionID;

E2SM_MET_GranularityPeriod_t     *g_granulPeriod;


#define MAX_RECORD_ITEM 2  // here max recordItems is the same as THE FIXED number of the measurments we have 
#define MAX_UE 5

E2SM_MET_MeasurementRecordItem_t *g_indMsgMeasRecItemArr[MAX_RECORD_ITEM];

static ric_service_model_t e2sm_met_model = {
    .name = "e2sm_met",
    .oid = "1.3.6.1.4.1.53148.1.2.2.98",
    .handle_subscription_add = e2sm_met_subscription_add,
    .handle_subscription_del = e2sm_met_subscription_del,
    .handle_control = e2sm_met_control,
    .handle_ricInd_timer_expiry = e2sm_met_ricInd_timer_expiry,
    .handle_gp_timer_expiry = e2sm_met_gp_timer_expiry
};

kmp_meas_info_t e2sm_met_meas_info[MAX_RECORD_ITEM] = {
                                            {1, "RRC.ConnEstabAatt.sum", 0, FALSE},
                                            {2, "RRC.ConnEstabSucc.sum", 0, FALSE},
                                            {3, "RRC.ConnReEstabAtt.sum", 0, FALSE},
                                            {4, "RRC.ConnMean", 0, FALSE},
                                            {5, "RRC.ConnMax", 0, FALSE}
                                        };

//!SECTION

//SECTION  Initializes MET Service model state and registers MET e2ap_ran_function_id_t number(s).


/**
 * Initializes KPM state and registers KPM e2ap_ran_function_id_t number(s).
 */
int e2sm_met_init(void)
{
    ric_ran_function_t *func;
    E2SM_MET_E2SM_MET_RANfunction_Description_t *func_def;
    E2SM_MET_RIC_ReportStyle_Item_t *ric_report_style_item;
    E2SM_MET_RIC_EventTriggerStyle_Item_t *ric_event_trigger_style_item;
    E2SM_MET_MeasurementInfo_Action_Item_t *meas_action_item1;
    E2SM_MET_MeasurementInfo_Action_Item_t *meas_action_item2;
    E2SM_MET_MeasurementInfo_Action_Item_t *meas_action_item3;
    E2SM_MET_MeasurementInfo_Action_Item_t *meas_action_item4;
    E2SM_MET_MeasurementInfo_Action_Item_t *meas_action_item5;

    func = (ric_ran_function_t *)calloc(1, sizeof(*func));
    func->model = &e2sm_met_model;
    func->revision = 1;
    func->name = "Eurecom-MET-SM";
    func->description = "MET monitor";


    func_def = (E2SM_MET_E2SM_MET_RANfunction_Description_t *)calloc(1, sizeof(*func_def));

    /* RAN Function Name */
    func_def->ranFunction_Name.ranFunction_ShortName.buf = (uint8_t *)strdup(func->name);
    func_def->ranFunction_Name.ranFunction_ShortName.size = strlen(func->name);
    func_def->ranFunction_Name.ranFunction_E2SM_OID.buf = (uint8_t *)strdup(func->model->oid);
    func_def->ranFunction_Name.ranFunction_E2SM_OID.size = strlen(func->model->oid);
    func_def->ranFunction_Name.ranFunction_Description.buf = (uint8_t *)strdup(func->description);
    func_def->ranFunction_Name.ranFunction_Description.size = strlen(func->description);
    /* Hack for E2t crash */
    //REVIEW IMPORTANT
    // long *ranFuncInst;
    // ranFuncInst = (long *)calloc(1,sizeof(*func_def->ranFunction_Name.ranFunction_Instance));
    // *ranFuncInst = 0;
    // func_def->ranFunction_Name.ranFunction_Instance = ranFuncInst;


    /* Sequence of Event trigger styles */
    func_def->ric_EventTriggerStyle_List = (struct E2SM_MET_E2SM_MET_RANfunction_Description__ric_EventTriggerStyle_List *)calloc(1, sizeof(*func_def->ric_EventTriggerStyle_List));
    ric_event_trigger_style_item = (E2SM_MET_RIC_EventTriggerStyle_Item_t *)calloc(1, sizeof(*ric_event_trigger_style_item));
    ric_event_trigger_style_item->ric_EventTriggerStyle_Type = 1;
    ric_event_trigger_style_item->ric_EventTriggerStyle_Name.buf = (uint8_t *)strdup("Trigger1");
    ric_event_trigger_style_item->ric_EventTriggerStyle_Name.size = strlen("Trigger1");
    ric_event_trigger_style_item->ric_EventTriggerFormat_Type = 1;
    ASN_SEQUENCE_ADD(&func_def->ric_EventTriggerStyle_List->list, ric_event_trigger_style_item);

    /* Sequence of Report styles */
    func_def->ric_ReportStyle_List = (struct E2SM_MET_E2SM_MET_RANfunction_Description__ric_ReportStyle_List *)calloc(1, sizeof(*func_def->ric_ReportStyle_List));
    ric_report_style_item = (E2SM_MET_RIC_ReportStyle_Item_t *)calloc(1, sizeof(*ric_report_style_item));
    ric_report_style_item->ric_ReportStyle_Type = 6;
    ric_report_style_item->ric_ReportStyle_Name.buf = (uint8_t *)strdup("O-CU-UP Measurement Container for the EPC connected deployment");
    ric_report_style_item->ric_ReportStyle_Name.size = strlen("O-CU-UP Measurement Container for the EPC connected deployment");
    ric_report_style_item->ric_ActionFormat_Type = 6; //pending 
    
    meas_action_item1 = (E2SM_MET_MeasurementInfo_Action_Item_t *)calloc(1, sizeof(*meas_action_item1));
    meas_action_item1->measName.buf = (uint8_t *)strdup(e2sm_met_meas_info[0].meas_type_name);
    meas_action_item1->measName.size = strlen(e2sm_met_meas_info[0].meas_type_name);

    E2SM_MET_MeasurementTypeID_t *measID1;
    measID1 = (E2SM_MET_MeasurementTypeID_t *)calloc(1, sizeof(*measID1));
    *measID1 = e2sm_met_meas_info[0].meas_type_id;

    meas_action_item1->measID = measID1;
    ASN_SEQUENCE_ADD(&ric_report_style_item->measInfo_Action_List.list, meas_action_item1);
    
    meas_action_item2 = (E2SM_MET_MeasurementInfo_Action_Item_t *)calloc(1, sizeof(*meas_action_item2));
    meas_action_item2->measName.buf = (uint8_t *)strdup(e2sm_met_meas_info[1].meas_type_name); //(uint8_t *)strdup("RRC.ConnEstabSucc.sum");
    meas_action_item2->measName.size = strlen(e2sm_met_meas_info[1].meas_type_name);

    E2SM_MET_MeasurementTypeID_t *measID2;
    measID2 = (E2SM_MET_MeasurementTypeID_t *)calloc(1, sizeof(*measID2));
    *measID2 = e2sm_met_meas_info[1].meas_type_id;

    meas_action_item2->measID = measID2;
    ASN_SEQUENCE_ADD(&ric_report_style_item->measInfo_Action_List.list, meas_action_item2);
    
    // meas_action_item3 = (E2SM_MET_MeasurementInfo_Action_Item_t *)calloc(1, sizeof(*meas_action_item3));
    // meas_action_item3->measName.buf = (uint8_t *)strdup(e2sm_met_meas_info[2].meas_type_name);
    // meas_action_item3->measName.size = strlen(e2sm_met_meas_info[2].meas_type_name);

    // E2SM_MET_MeasurementTypeID_t *measID3;
    // measID3 = (E2SM_MET_MeasurementTypeID_t *)calloc(1, sizeof(*measID3));
    // *measID3 = e2sm_met_meas_info[2].meas_type_id;

    // meas_action_item3->measID = measID3;
    // ASN_SEQUENCE_ADD(&ric_report_style_item->measInfo_Action_List.list, meas_action_item3);

    // meas_action_item4 = (E2SM_MET_MeasurementInfo_Action_Item_t *)calloc(1, sizeof(*meas_action_item4));
    // meas_action_item4->measName.buf = (uint8_t *)strdup(e2sm_met_meas_info[3].meas_type_name);
    // meas_action_item4->measName.size = strlen(e2sm_met_meas_info[3].meas_type_name);

    // E2SM_MET_MeasurementTypeID_t *measID4;
    // measID4 = (E2SM_MET_MeasurementTypeID_t *)calloc(1, sizeof(*measID4));
    // *measID4 = e2sm_met_meas_info[3].meas_type_id;

    // meas_action_item4->measID = measID4;
    // ASN_SEQUENCE_ADD(&ric_report_style_item->measInfo_Action_List.list, meas_action_item4);

    // meas_action_item5 = (E2SM_MET_MeasurementInfo_Action_Item_t *)calloc(1, sizeof(*meas_action_item5));
    // meas_action_item5->measName.buf = (uint8_t *)strdup(e2sm_met_meas_info[4].meas_type_name);
    // meas_action_item5->measName.size = strlen(e2sm_met_meas_info[4].meas_type_name);

    // E2SM_MET_MeasurementTypeID_t *measID5;
    // measID5 = (E2SM_MET_MeasurementTypeID_t *)calloc(1, sizeof(*measID5));
    // *measID5 = e2sm_met_meas_info[4].meas_type_id;

    // meas_action_item5->measID = measID5;
    // ASN_SEQUENCE_ADD(&ric_report_style_item->measInfo_Action_List.list, meas_action_item5);

    ric_report_style_item->ric_IndicationHeaderFormat_Type = 1;
    ric_report_style_item->ric_IndicationMessageFormat_Type = 1;
    ASN_SEQUENCE_ADD(&func_def->ric_ReportStyle_List->list, ric_report_style_item);

    //xer_fprint(stderr, &asn_DEF_E2SM_MET_E2SM_MET_RANfunction_Description, func_def);

    RIC_AGENT_INFO("_______\n"); 
    func->enc_definition_len = e2ap_encode(&asn_DEF_E2SM_MET_E2SM_MET_RANfunction_Description,0, func_def,&func->enc_definition);
    RIC_AGENT_INFO("_______\n");

    RIC_AGENT_INFO("------ RAN FUNC DEF ENC Len:%lu-------\n", func->enc_definition_len);


    if (func->enc_definition_len < 0) {
        RIC_AGENT_ERROR("failed to encode RANfunction_List in E2SM MET func description; aborting!");
        ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_E2SM_MET_E2SM_MET_RANfunction_Description, func_def);
        free(func_def);
        free(func);
        return -1;
    }

    func->enabled = 1;
    func->definition = func_def;

#if 0   
    /* Test code */
    E2SM_MET_E2SM_MET_RANfunction_Description_t *func_defi;
    asn_dec_rval_t decode_result;
    decode_result = aper_decode_complete(NULL, &asn_DEF_E2SM_MET_E2SM_MET_RANfunction_Description,
                                               (void **)&func_defi, func->enc_definition, func->enc_definition_len);
    DevAssert(decode_result.code == RC_OK);
#endif
    return ric_agent_register_ran_function(func);
}


//!SECTION



static int e2sm_met_subscription_add(ric_agent_info_t *ric, ric_subscription_t *sub)
{
  /* XXX: process E2SM content. */
  if (LIST_EMPTY(&ric->subscription_list)) {
    LIST_INSERT_HEAD(&ric->subscription_list,sub,subscriptions);
  }
  else {
    LIST_INSERT_BEFORE(LIST_FIRST(&ric->subscription_list),sub,subscriptions);
  }
  return 0;
}

static int e2sm_met_subscription_del(ric_agent_info_t *ric, ric_subscription_t *sub, int force,long *cause,long *cause_detail)
{
    timer_remove(ric->e2sm_met_timer_id);
    LIST_REMOVE(sub, subscriptions);
    ric_free_subscription(sub);
    return 0;
}

static int e2sm_met_control(ric_agent_info_t *ric,ric_control_t *control)
{
    return 0;
}

static char *time_stamp(void)
{
    char *timestamp = (char *)malloc(sizeof(char) * 128);
    time_t ltime;
    ltime=time(NULL);
    struct tm *tm;
    tm=localtime(&ltime);

    sprintf(timestamp,"%d/%d/%d | %d:%d:%d", tm->tm_year+1900, tm->tm_mon,
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
    return timestamp;
}


static int e2sm_met_ricInd_timer_expiry(
        ric_agent_info_t *ric,
        long timer_id,
        ric_ran_function_id_t function_id,
        long request_id,
        long instance_id,
        long action_id,
        uint8_t **outbuf,
        uint32_t *outlen)
{
    //call of the debug function 
    // test_met();

    E2SM_MET_E2SM_MET_IndicationMessage_t* indicationmessage;
    ric_subscription_t *rs;

    DevAssert(timer_id == ric->e2sm_met_timer_id);

    char *time = time_stamp();
    RIC_AGENT_INFO("[%s] ----  Sending MET RIC Indication, timer_id %ld function_id %ld---------\n", 
                   time, timer_id, function_id);
    free(time);

    /* Fetch the RIC Subscription */
    rs = ric_agent_lookup_subscription(ric,request_id,instance_id,function_id);
    if (!rs) {
        RIC_AGENT_ERROR("failed to find subscription %ld/%ld/%ld\n", request_id,instance_id,function_id);
    }

    //REVIEW update the name and the params if necessar
    indicationmessage = encode_met_Indication_Msg(ric, rs);

    {
        char *error_buf = (char*)calloc(300, sizeof(char));
        size_t errlen = 0;
        asn_check_constraints(&asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage, indicationmessage, error_buf, &errlen);
        // fprintf(stderr,"MET IND error length %zu\n", errlen);
        // fprintf(stderr,"MET IND error buf %s\n", error_buf);
        // free(error_buf);
        // xer_fprint(stderr, &asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage, indicationmessage);
    }
    // g_granularityIndx = 0; // Resetting

    // xer_fprint(stderr, &asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage, indicationmessage);
    uint8_t e2smbuffer[8192];
    size_t e2smbuffer_size = 8192;

    asn_enc_rval_t er = asn_encode_to_buffer(NULL,
            ATS_ALIGNED_BASIC_PER,
            &asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage,
            indicationmessage, e2smbuffer, e2smbuffer_size);

    fprintf(stderr, "er encded is %zu\n", er.encoded);
    fprintf(stderr, "after encoding MET IND message\n");

#if 0

    fprintf(stderr, "Here is the message:n\n");
    for (int i = 0; i < er.encoded; i++)
    {
        fprintf(stderr, "0x%02x,", e2smbuffer[i]);
    }
    if (er.encoded < 0) {
        fprintf(stderr, "ERROR encoding indication header, name=%s, tag=%s", er.failed_type->name, er.failed_type->xml_tag);
    }

    DevAssert(er.encoded >= 0);

#endif

    E2AP_E2AP_PDU_t *e2ap_pdu = (E2AP_E2AP_PDU_t*)calloc(1, sizeof(E2AP_E2AP_PDU_t));

    E2SM_MET_E2SM_MET_IndicationHeader_t* ind_header_style1 =
        (E2SM_MET_E2SM_MET_IndicationHeader_t*)calloc(1,sizeof(E2SM_MET_E2SM_MET_IndicationHeader_t));
        //REVIEW update the name and the params if necessar
    encode_e2sm_met_indication_header(ric->ranid, ind_header_style1);

    uint8_t e2sm_header_buf_style1[8192];
    size_t e2sm_header_buf_size_style1 = 8192;
    asn_enc_rval_t er_header_style1 = asn_encode_to_buffer(
            NULL,
            ATS_ALIGNED_BASIC_PER,
            &asn_DEF_E2SM_MET_E2SM_MET_IndicationHeader,
            ind_header_style1,
            e2sm_header_buf_style1,
            e2sm_header_buf_size_style1);

    if (er_header_style1.encoded < 0) {
        fprintf(stderr, "ERROR encoding indication header, name=%s, tag=%s", er_header_style1.failed_type->name, er_header_style1.failed_type->xml_tag);
    }

    DevAssert(er_header_style1.encoded >= 0);

#if 0

    fprintf(stderr, "Here is the ind header:  len = %d \n",er_header_style1.encoded);
    for (int i = 0; i < er_header_style1.encoded; i++)
    {
        fprintf(stderr, "0x%02x,", e2sm_header_buf_style1[i]);
    }
    fprintf(stderr,"\n\n");
    if (er.encoded < 0) {
        fprintf(stderr, "ERROR encoding indication header, name=%s, tag=%s", er.failed_type->name, er.failed_type->xml_tag);
    }

    DevAssert(er.encoded >= 0);

#endif

        // TODO - remove hardcoded values
    generate_e2apv1_indication_request_parameterized(
            e2ap_pdu, request_id, instance_id, function_id, action_id,
            0, e2sm_header_buf_style1, er_header_style1.encoded,
            e2smbuffer, er.encoded);

    *outlen = e2ap_asn1c_encode_pdu(e2ap_pdu, outbuf);

    return 0;
}

    // ANCHOR  primary check of the function passed 
    // TODO this is a requirements (aka used by handle time expircy function) so it is imported here to be later modified to meet MET SM defs 
    // REVIEW this contains the dummy data, it is not real ones + it is duplicated aka RecordItems are creaated once and duplicated in all Records in measData
    // the thing that made this the the use of a table aka array instead of a matrix for troring the measurments : g_indMsgMeasRecItemArr
static E2SM_MET_E2SM_MET_IndicationMessage_t*
encode_met_Indication_Msg(ric_agent_info_t* ric, ric_subscription_t *rs)
{
    int ret, ret2;
    uint64_t i;

    E2SM_MET_MeasurementData_t* meas_data;
    E2SM_MET_MeasurementRecord_t* meas_rec[MAX_UE];

    //REVIEW this local variable can be used later on to hold the real data then extra saves my be added 
    // E2SM_MET_MeasurementRecordItem_t* meas_data_item[MAX_RECORD_ITEM];

    

    // if (action_def_missing == TRUE)
    // if (1)
    // { 
    //     for (i = 0; i < MAX_RECORD_ITEM ; i++)
    //     {
    //         g_indMsgMeasRecItemArr[i] = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
    //         g_indMsgMeasRecItemArr[i]->present = E2SM_MET_MeasurementRecordItem_PR_integer;
    //         g_indMsgMeasRecItemArr[i]->choice.integer = ;

    //         // switch(i)
    //         // {
    //         //     case 0:/*RRC.ConnEstabAtt.sum*/
    //         //         g_indMsgMeasRecItemArr[i]->choice.integer = 21;
    //         //         break;
    //         //     case 1:/*RRC.ConnEstabSucc.sum*/
    //         //         g_indMsgMeasRecItemArr[i]->choice.integer = 10; 
    //         //         break;
    //         //     case 2:/*RRC.ConnReEstabAtt.sum*/
    //         //         g_indMsgMeasRecItemArr[i]->choice.integer = 10;
    //         //         break;
    //         //     case 3:/*RRC.ConnMean*/
    //         //         g_indMsgMeasRecItemArr[i]->choice.integer = 10;
    //         //         break;
    //         //     case 4:/*RRC.ConnMax*/
    //         //         g_indMsgMeasRecItemArr[i]->choice.integer = 10;
    //         //         break;

    //         //     default:
    //         //         break;
    //         // }
    //     }
    //     // g_granularityIndx = 1;
    // } 

    //RIC_AGENT_INFO("Granularity Idx=:%d\n",g_granularityIndx);

    /*
     * measData->measurementRecord (List)
     */
    meas_data = (E2SM_MET_MeasurementData_t*)calloc(1, sizeof(E2SM_MET_MeasurementData_t));
    DevAssert(meas_data!=NULL);
#if 0
        int64_t tmp_id = 11;
        E2SM_MET_MeasurementRecord_t *meas_rec_debug = (E2SM_MET_MeasurementRecord_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecord_t));
        // meas_rec[k]->ueID = asn_int642INTEGER(meas_rec[k]->ueID,k);
        // ret = asn_uint642INTEGER(&meas_rec_debug->ueID,tmp_id);

        meas_rec_debug->ueID = 10;

        // char *name = "AAAA";
        // meas_rec_debug->ueTag.buf = (uint8_t *)strdup(name);
        // meas_rec_debug->ueTag.size = strlen(name);

        // meas_rec[k]->ueTag = "ABC";
        // ret2 = OCTET_STRING_fromString(&meas_rec_debug->ueTag,"ABC");


        E2SM_MET_MeasurementRecordItem_t *t = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
        t->present = E2SM_MET_MeasurementRecordItem_PR_integer;
        t->choice.integer = 10;

        E2SM_MET_MeasurementRecordItem_t *t2 = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
        t2->present = E2SM_MET_MeasurementRecordItem_PR_integer;
        t2->choice.integer = 20;

        ret = ASN_SEQUENCE_ADD(&meas_rec_debug->measRecordItem.list, t);
        DevAssert(ret == 0);

        ret = ASN_SEQUENCE_ADD(&meas_rec_debug->measRecordItem.list, t2);
        DevAssert(ret == 0);    



        // xer_fprint(stderr, &asn_DEF_E2SM_MET_MeasurementRecord,meas_rec_debug);

        encode_decode(meas_rec_debug);

        // log_buf_content(meas_rec_debug,&asn_DEF_E2SM_MET_MeasurementRecord,"helllooo measRec--" );

        


#endif 

    
   
     gNB_MAC_INST *gNB_mac = RC.nrmac[0];
     NR_UE_info_t *UE_info = &gNB_mac->UE_info;
     //int nbue = UE_info->num_UEs;
     //printf("UEs number at MAC: %d\n",nbue);
     //if (nbue <=0) continue;

     NR_list_t *UE_list = &UE_info->list;
     for (int k = UE_list->head; k >= 0; k = UE_list->next[k]) {
        NR_UE_sched_ctrl_t *sched_ctrl = &UE_info->UE_sched_ctrl[k];    /*
         * Measurement Record->MeasurementRecordItem (List)
         */
        const rnti_t rnti = UE_info->rnti[k];
        NR_mac_stats_t *stats = &UE_info->mac_stats[k];
        meas_rec[k] = (E2SM_MET_MeasurementRecord_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecord_t));
        // meas_rec[k]->ueID = asn_int642INTEGER(meas_rec[k]->ueID,k);
        // int ret1 = asn_uint642INTEGER(&meas_rec[k]->ueID,tmp_id);
        meas_rec[k]->ueID = k;
        // meas_rec[k]->ueTag = "AAA";
        // int ret22 = OCTET_STRING_fromString(&meas_rec[k]->ueTag,"ABC");
        // E2SM_MET_UETag_t	*ueTag = (E2SM_MET_UETag_t *)calloc(1, sizeof(E2SM_MET_UETag_t));
        // int ret22 = OCTET_STRING_fromString(ueTag,"AAAA");
        meas_rec[k]->ueTag.buf  = (uint8_t *)strdup("AAA");
        meas_rec[k]->ueTag.size = strlen("AAA");


            E2SM_MET_MeasurementRecordItem_t *g_indMsgMeasRecItem = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
            g_indMsgMeasRecItem->present = E2SM_MET_MeasurementRecordItem_PR_integer;
            g_indMsgMeasRecItem->choice.integer = sched_ctrl->ph;
            ret = ASN_SEQUENCE_ADD(&meas_rec[k]->measRecordItem.list, g_indMsgMeasRecItem);
            DevAssert(ret == 0);
            E2SM_MET_MeasurementRecordItem_t *g_indMsgMeasRecItem1 = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
            g_indMsgMeasRecItem1->present = E2SM_MET_MeasurementRecordItem_PR_integer;
            g_indMsgMeasRecItem1->choice.integer = sched_ctrl->dl_bler_stats.mcs;
            ret = ASN_SEQUENCE_ADD(&meas_rec[k]->measRecordItem.list, g_indMsgMeasRecItem1);
            DevAssert(ret == 0);
        // for(i=0; i < MAX_RECORD_ITEM; i++)
        // { 
        //     /* Meas Records meas_rec[]  have to be prepared for each Meas data item */
        //     E2SM_MET_MeasurementRecordItem_t g_indMsgMeasRecItem = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
        //     g_indMsgMeasRecItem->present = E2SM_MET_MeasurementRecordItem_PR_integer;
        //     g_indMsgMeasRecItem->choice.integer = ;
        //     ret = ASN_SEQUENCE_ADD(&meas_rec[k]->measRecordItem.list, g_indMsgMeasRecItem);
        //     DevAssert(ret == 0);
        // }
        
        //this section is commmented because unlike the original KPM asn1 def the MET does not use DataItem but the structure is 
            // directly Data -> Record -> RecordItem

        // /* MeasDataItem*/
        // meas_data_item[k] = (E2SM_KPM_MeasurementDataItem_KPMv2_t*)calloc(1, sizeof(E2SM_KPM_MeasurementDataItem_KPMv2_t));
        // meas_data_item[k]->measRecord = *meas_rec[k];

        /* Enqueue Meas data items */
        ret = ASN_SEQUENCE_ADD(&meas_data->list, meas_rec[k]);
        DevAssert(ret == 0);
    }

   /*
    * measInfoList
    */
    E2SM_MET_E2SM_MET_IndicationMessage_Format1_t* format = 
                        (E2SM_MET_E2SM_MET_IndicationMessage_Format1_t*)calloc(1, sizeof(E2SM_MET_E2SM_MET_IndicationMessage_Format1_t));
    ASN_STRUCT_RESET(asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage_Format1, format);

   	// format->measInfoList = (E2SM_MET_MeasurementInfoList_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoList_t));


    /*
     * IndicationMessage_Format1 -> measInfoList
     * IndicationMessage_Format1 -> measData
     */

    // format->subscriptID.size = g_subscriptionID.size;
    // format->subscriptID.buf = g_subscriptionID.buf;
    uint16_t tmpp = 10;
    // format->subscriptID = tmpp;
    //ANCHOR This sub Id is important so avoid e2t crash 
    uint64_t subsId = 10;//hack
    // ret = asn_uint642INTEGER(&g_subscriptionID,subsId);
    format->subscriptID =10;

    format->measData = *meas_data;
    // format->granulPeriod = g_granulPeriod;

    /*
     * IndicationMessage -> IndicationMessage_Format1
     */

    
    E2SM_MET_E2SM_MET_IndicationMessage_t* indicationmessage = 
                                (E2SM_MET_E2SM_MET_IndicationMessage_t*)calloc(1, sizeof(E2SM_MET_E2SM_MET_IndicationMessage_t));
    indicationmessage->indicationMessage_formats.present = 
                                    E2SM_MET_E2SM_MET_IndicationMessage__indicationMessage_formats_PR_indicationMessage_Format1;
    indicationmessage->indicationMessage_formats.choice.indicationMessage_Format1 = *format;

    // E2SM_MET_E2SM_MET_IndicationMessage_t* tmp_ind = 
    //                         (E2SM_MET_E2SM_MET_IndicationMessage_t*)calloc(1, sizeof(E2SM_MET_E2SM_MET_IndicationMessage_t));

    // // encode_decode(&asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage,indicationmessage,tmp_ind);  

    // E2SM_MET_E2SM_MET_IndicationMessage_t* tmp_indicationmessage = 
    //                         (E2SM_MET_E2SM_MET_IndicationMessage_t*)calloc(1, sizeof(E2SM_MET_E2SM_MET_IndicationMessage_t));

    // encode_decode(&asn_DEF_E2SM_MET_E2SM_MET_IndicationMessage,indicationmessage,tmp_indicationmessage);

    return indicationmessage;


}
//REVIEW AT the end of the function what known isses : 
    //  the granurality period and subId are not set by nay other function 


// SECTION 
//TODO  this is also a requirement of the function e2sm_met_ricInd_timer_expiry imported here to be set to hundle MET SM
//ANCHOR primary check passed
void encode_e2sm_met_indication_header(ranid_t ranid, E2SM_MET_E2SM_MET_IndicationHeader_t *ihead) 
{
    ihead->indicationHeader_formats.present = E2SM_MET_E2SM_MET_IndicationHeader__indicationHeader_formats_PR_indicationHeader_Format1;

    E2SM_MET_E2SM_MET_IndicationHeader_Format1_t* ind_header = &ihead->indicationHeader_formats.choice.indicationHeader_Format1;

    /* MET Node ID */
    ind_header->metNodeID = (E2SM_MET_GlobalMETnode_ID_t *)calloc(1,sizeof(E2SM_MET_GlobalMETnode_ID_t));
    //REVIEW is the init of an int done correctly
    *ind_header->metNodeID = 10; // hack 

    // *(ind_header->metNodeID) = 10; //REVIEW  // hack 


    /* Collect Start Time Stamp */
    /* Encoded in the same format as the first four octets of the 64-bit timestamp format as defined in section 6 of IETF RFC 5905 */
    ind_header->colletStartTime.buf = (uint8_t *)calloc(1, 4);
    ind_header->colletStartTime.size = 4;
    *((uint32_t *)(ind_header->colletStartTime.buf)) = htonl((uint32_t)time(NULL));

    // E2SM_MET_MeasurementInfoList_t* meas_info_list = (E2SM_MET_MeasurementInfoList_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoList_t));
    
    E2SM_MET_MeasurementInfoItem_t* meas_info_item1 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item1->buf = (uint8_t *)strdup("mcs");
    meas_info_item1->size = strlen("mcs");
    int ret = ASN_SEQUENCE_ADD( &ind_header->measInfoList.list, meas_info_item1);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item2 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item2->buf = (uint8_t *)strdup("phr");
    meas_info_item2->size = strlen("phr");
    ret = ASN_SEQUENCE_ADD( &ind_header->measInfoList.list, meas_info_item2);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item3 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item3->buf = (uint8_t *)strdup("bler");
    meas_info_item3->size = strlen("bler");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item3);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item4 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item4->buf = (uint8_t *)strdup("errors");
    meas_info_item4->size = strlen("errors");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item4);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item5 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item5->buf = (uint8_t *)strdup("throughput");
    meas_info_item5->size = strlen("throughput");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item5);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item6 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item6->buf = (uint8_t *)strdup("snr");
    meas_info_item6->size = strlen("snr");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item6);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item7 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item7->buf = (uint8_t *)strdup("rsrp");
    meas_info_item7->size = strlen("rsrp");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item7);
    DevAssert(ret == 0);

    E2SM_MET_MeasurementInfoItem_t* meas_info_item8 = (E2SM_MET_MeasurementInfoItem_t*)calloc(1, sizeof(E2SM_MET_MeasurementInfoItem_t));
    meas_info_item8->buf = (uint8_t *)strdup("cqi");
    meas_info_item8->size = strlen("cqi");
    ret = ASN_SEQUENCE_ADD(&ind_header->measInfoList.list, meas_info_item8);
    DevAssert(ret == 0);
    

    //debug
    E2SM_MET_E2SM_MET_IndicationHeader_t* tmp_ind_header =
        (E2SM_MET_E2SM_MET_IndicationHeader_t*)calloc(1,sizeof(E2SM_MET_E2SM_MET_IndicationHeader_t));
    encode_decode(&asn_DEF_E2SM_MET_E2SM_MET_IndicationHeader,ihead,tmp_ind_header);

    //xer_fprint(stderr, &asn_DEF_E2SM_KPM_E2SM_KPMv2_IndicationHeader, ihead);
}
//REVIEW AT the end of the function what known isses : 
    //  the ranid arg is not used and we are not sure yet whether the IE affectations are correct or not 
// !SECTION 



//SECTION  this function is triggered when the granularity period timer is expired
    // unlike KPM, MET service model does not need this function at the present version 
    // because infos are gathered only when the Indication message is expired 
    
static int e2sm_met_gp_timer_expiry(
        ric_agent_info_t *ric,
        long timer_id,
        ric_ran_function_id_t function_id,
        long request_id,
        long instance_id,
        long action_id,
        uint8_t **outbuf,
        uint32_t *outlen)

{
   // this functions fundles the expiration of the granularity period timer
   // in the normal use case this will trigger a data gathering loop that will save 
   // the needed data in the global structure (matrix) of DataInfoItem
    return 0;
}
// !SECTION 


// SECTION  decode_and_handle_action_def function is responsable for getting the details of the sub and 
    // the infos needed for the RAN function to work 

int 
e2sm_met_decode_and_handle_action_def(uint8_t *def_buf, 
                                          size_t def_size, 
                                          ric_ran_function_t *func,
                                          uint32_t      interval_ms,
                                          ric_subscription_t* rs,
                                          ric_agent_info_t *ric)
{
   
    uint64_t subsId = 10;//hack

    // g_indMsgMeasInfoCnt = 0; // resetting
   
    //REVIEW here since our SM is a very basic one with one default action Def and report style 
    // we  didn't respect the ORAN structure and we took a shortcut by ignoring the actionDefinitions
    // here in the code bellow we just assume that there is no action def 


    // if (def_size == 0)
    {
        /* In case of missing action list, all Meas Info should be reported to RIC */
        // RIC_AGENT_INFO("ACTION Def missing, populating all KPM Data\n");

        // for (i = 0; i < MAX_KPM_MEAS; i++)
        // {
        //     g_indMsgMeasInfoItemArr[g_indMsgMeasInfoCnt] =
        //                              (E2SM_KPM_MeasurementInfoItem_KPMv2_t *)calloc(1,sizeof(E2SM_KPM_MeasurementInfoItem_KPMv2_t));
        //     g_indMsgMeasInfoItemArr[g_indMsgMeasInfoCnt].present = E2SM_KPM_MeasurementType_KPMv2_PR_measName;
        //     g_indMsgMeasInfoItemArr[g_indMsgMeasInfoCnt].choice.measName.buf =
        //                                              (uint8_t *)strdup(e2sm_met_meas_info[i].meas_type_name);
        //     g_indMsgMeasInfoItemArr[g_indMsgMeasInfoCnt].choice.measName.size =
        //                                                         strlen(e2sm_met_meas_info[i].meas_type_name);
        //     e2sm_met_meas_info[i].subscription_status = TRUE;
        //     g_indMsgMeasInfoCnt++;
        // }
        // *g_granulPeriod = 10; //Hack

        /* Hack - Subscription ID */
        //g_subscriptionID.size = sizeof(subsId);
        //g_subscriptionID.buf = (uint8_t *)calloc(1,sizeof(subsId));
        //*g_subscriptionID.buf = subsId;


		// g_subscriptionID = subsId; // this is a wrong assignment 

        // asn_uint642INTEGER(&g_subscriptionID,subsId);

        // action_def_missing = TRUE; /* Granularity Timer will not start */
        return 0;
    }
 
}

// !SECTION 

