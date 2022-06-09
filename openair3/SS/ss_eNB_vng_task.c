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
 *      
 * File ss_eNB_vng_task.c
 * Brief: Implements the port manager handling for SS mode.
 * Author: Anurag Asokan, Manu Agrawal, Vaibhav Shrivastava, Vijay Chadachan
 * Company: Firecell
 * Email: anurag.ashokan@firecell.io, manu.agrawal@firecell.io, vaibhav.shrivastava@firecell.io,vijay.chadachan@firecell.io
 */

#include "intertask_interface.h"
#include "common/ran_context.h"

#include "acpSys.h"
#include "ss_eNB_vng_task.h"
#include "ss_eNB_context.h"

#include "ss_eNB_proxy_iface.h"
#include "SIDL_EUTRA_VNG_PORT.h"
#include "acpVng.h"

SSConfigContext_t SS_context;
extern RAN_CONTEXT_t RC;

static acpCtx_t ctx_vng_g = NULL;

enum MsgUserId {
    MSG_VngProcess_userId = 1,
};

/*
 * Function : ss_vng_send_cnf
 * Description: Funtion to send the response to the TTCN/SIDL Client
 * In :
 * req  - ITTI message received from the TTCN via PORTMAN
 * Out:
 * newState: No impact on state machine.
 *
 */
void ss_vng_send_cnf(uint8_t status, EUTRA_CellId_Type CellId)
{
    struct EUTRA_VNG_CTRL_CNF cnf;
    const size_t size = 16 * 1024;

    unsigned char *buffer = (unsigned char *)acpMalloc(size);

    size_t msgSize = size;
    memset(&cnf, 0, sizeof(cnf));
    cnf.Common.CellId 			= eutra_Cell1; /** TODO: Change hardcode */
    cnf.Common.RoutingInfo.d 		= RoutingInfo_Type_None;
    cnf.Common.RoutingInfo.v.None 	= true;
    cnf.Common.TimingInfo.d 		= TimingInfo_Type_Now;
    cnf.Common.TimingInfo.v.Now 	= true;
    if (status == 0) {
        cnf.Common.Result.d 		= ConfirmationResult_Type_Success;
        cnf.Common.Result.v.Success 	= true;
        cnf.Confirm 			= true;
    } else {
        cnf.Common.Result.d 		= ConfirmationResult_Type_Error;
        cnf.Common.Result.v.Error 	= 1; /** TODO: A dummy value */
        cnf.Confirm 			= false;
    }
    LOG_A(ENB_SS, "[VNG] VNG CNF received cellId %d result %d \n",
                     cnf.Common.CellId,cnf.Common.Result.d);

    /* Encode message
     */
    if (acpVngProcessEncSrv(ctx_vng_g, buffer, &msgSize, &cnf) != 0)
    {
        acpFree(buffer);
        return;
    }
    /* Send message
     */
    status = acpSendMsg(ctx_vng_g, msgSize, buffer);
    if (status != 0)
    {
        LOG_A(ENB_SS, "[VNG] acpSendMsg failed. Error : %d on fd: %d\n",
              status, acpGetSocketFd(ctx_vng_g));
        acpFree(buffer);
        return;
    }
    else
    {
        LOG_A(ENB_SS, "[VNG] acpSendMsg Success \n");
    }
    // Free allocated buffer
    acpFree(buffer);
}

/*
 * Function : vng_ss_configure_cell
 * Description: Funtion to configured cell and send SS_VNG_PROXY_REQ message to SYS Task
 * In :
 * req :
 * Out:
 * newState: No impact on state machine.
 *
 */
static inline void
vng_ss_configure_cell (EUTRA_CellId_Type CellId, Dl_Bandwidth_Type Bandwidth,
        int32_t NocLevel, VngProxyCmd_e cmd)
{
    MessageDef *message_p = itti_alloc_new_message(TASK_VNG, INSTANCE_DEFAULT, SS_VNG_PROXY_REQ);
    assert(message_p);

    SS_VNG_PROXY_REQ(message_p).cell_id = SS_context.cellId;
    SS_VNG_PROXY_REQ(message_p).bw = Bandwidth;
    SS_VNG_PROXY_REQ(message_p).Noc_level = NocLevel;
    SS_VNG_PROXY_REQ(message_p).cmd = cmd;

    int res = itti_send_msg_to_task(TASK_SYS, INSTANCE_DEFAULT, message_p);
    if (res < 0)
    {
        LOG_A(ENB_SS, "[SS-VNG] Error in itti_send_msg_to_task\n");
    }
    else
    {
        LOG_A(ENB_SS, "[SS-VNG] Send ITTI message to %s\n", ITTI_MSG_DESTINATION_NAME(message_p));
    }
}

/*
 * Function : ss_eNB_read_from_vng_socket
 * Description: Funtion to read from VNG Socket and call the handler function related
 * to particular message
 * In :
 * req : Request received from VNG task
 * Out:
 * newState: No impact on state machine.
 *
 */
static inline void
ss_eNB_read_from_vng_socket(acpCtx_t ctx)
{
    struct EUTRA_VNG_CTRL_REQ* req = NULL;
    const size_t size = 16 * 1024;
    unsigned char *buffer = (unsigned char *)acpMalloc(size);
    assert(buffer);
    size_t msgSize = size; //2

    assert(ctx);

    while (1)
    {
    	int userId = acpRecvMsg(ctx, &msgSize, buffer);
        LOG_A(ENB_SS, "[SS-VNG] Received msgSize=%d, userId=%d\n", (int)msgSize, userId);

    	// Error handling
    	if (userId < 0)
    	{
    	    if (userId == -ACP_ERR_SERVICE_NOT_MAPPED)
    	    {
    	    	// Message not mapped to user id,
    	    	// this error should not appear on server side for the messages received from clients
    	    }
    	    else if (userId == -ACP_ERR_SIDL_FAILURE)
    	    {
    	    	// Server returned service error,
    	    	// this error should not appear on server side for the messages received from clients
    	    	SidlStatus sidlStatus = -1;
    	    	acpGetMsgSidlStatus(msgSize, buffer, &sidlStatus);
    	    }
    	    else
    	    {
                LOG_A(ENB_SS, "[SS-VNG] Invalid userId: %d \n", userId);
    	    	break;
    	    }
    	}

    	if (userId == 0)
    	{
    	    // No message (timeout on socket)
    	    break;
    	}
    	else if (userId == MSG_VngProcess_userId)
    	{
            LOG_A(ENB_SS, "[SS-VNG] Received VNG Control Request\n");

            if (acpVngProcessDecSrv(ctx, buffer, msgSize, &req) != 0)
            {
                LOG_A(ENB_SS, "[SS-VNG] acpVngProcessDecSrv failed\n");
            	break;
            }

            if (RC.ss.State < SS_STATE_CELL_ACTIVE) {
                LOG_A(ENB_SS, "[SS-VNG] Request received in an invalid state: %d \n", RC.ss.State);
            	break;
            }
            /** TODO: Dump message here */
            switch (req->Request.d)
            {
            	case EUTRA_VngConfigRequest_Type_Configure:
                        LOG_A(ENB_SS, "[SS-VNG] Received Configure request\n");
                        vng_ss_configure_cell(req->Common.CellId, req->Request.v.Configure.Bandwidth,
            			req->Request.v.Configure.NocLevel, (VngProxyCmd_e)EUTRA_VngConfigRequest_Type_Configure);
            		break;
            	case EUTRA_VngConfigRequest_Type_Activate:
                        LOG_A(ENB_SS, "[SS-VNG] Received Activate request\n");
                        vng_ss_configure_cell(req->Common.CellId, (0xFF),
            			(0xFFFF), (VngProxyCmd_e)EUTRA_VngConfigRequest_Type_Activate);
            		break;
            	case EUTRA_VngConfigRequest_Type_Deactivate:
                        LOG_A(ENB_SS, "[SS-VNG] Received Deactivate request\n");
                        vng_ss_configure_cell(req->Common.CellId, (0xFF),
            			(0xFFFF), (VngProxyCmd_e)EUTRA_VngConfigRequest_Type_Deactivate);
            		break;
            	case EUTRA_VngConfigRequest_Type_UNBOUND_VALUE:
            	default:
                        LOG_A(ENB_SS, "[SS-VNG] Received unhandled message in VNG Port\n");
            }

            if (req->Request.d == EUTRA_VngConfigRequest_Type_UNBOUND_VALUE || req->Request.d > EUTRA_VngConfigRequest_Type_Deactivate)
    	        break;
    	}
    }
    acpFree(buffer);
  //acpVngProcessFreeSrv(req);
}

/*
 * Function : ss_eNB_vng_process_itti_msg
 * Description: Funtion Handles the ITTI message received from the TTCN on SYS Port
 * In :
 * req : ITTI message received from the TTCN via PORTMAN
 * Out:
 * newState: No impact on state machine.
 *
 */
void *ss_eNB_vng_process_itti_msg(void *notUsed)
{
    MessageDef *received_msg = NULL;

    itti_poll_msg(TASK_VNG, &received_msg);

    /* Check if there is a packet to handle */
    if (received_msg != NULL)
    {
	switch (ITTI_MSG_ID(received_msg))
	{
	    case SS_VNG_PROXY_RESP:
            {
                LOG_A(ENB_SS, "[SS-VNG] Response receieved from %s CellId: %d Status: %d\n",
                    ITTI_MSG_ORIGIN_NAME(received_msg), SS_VNG_PROXY_RESP(received_msg).cell_id,
		    SS_VNG_PROXY_RESP(received_msg).status);

		/** Send response here */
		ss_vng_send_cnf(SS_VNG_PROXY_RESP(received_msg).cell_id, SS_VNG_PROXY_RESP(received_msg).status);
            }
            break;

            case TERMINATE_MESSAGE:
            {
                itti_exit_task();
                break;
            }
            default:
                LOG_A(ENB_SS, "[VNG] Received unhandled message %d:%s\n",
                    ITTI_MSG_ID(received_msg), ITTI_MSG_NAME(received_msg));
	}
    }

    ss_eNB_read_from_vng_socket(ctx_vng_g);

    return NULL;
}

/*
 * Function : ss_eNB_vng_init
 * Description: Funtion Handles the initilization of VNG task
 * In :
 * req :
 * Out:
 * newState: No impact on state machine.
 *
 */
void ss_eNB_vng_init(void)
{
    IpAddress_t ipaddr;

    const char *hostIp;
    hostIp = RC.ss.hostIp;
    acpConvertIp(hostIp, &ipaddr);

    // Port number
    int port = RC.ss.Vngport;

    LOG_A(ENB_SS, "[SS-VNG] Initializing VNG Port %s:%d\n", hostIp, port);

    //acpInit(malloc, free, 1000);

    const struct acpMsgTable msgTable[] = {
        { "VngProcess", MSG_VngProcess_userId },

        // The last element should be NULL
        { NULL, 0 }
    };

    // Arena size to decode received message
    const size_t aSize = 32 * 1024;

    // Start listening server and get ACP context,
    // after the connection is performed, we can use all services
    int ret = acpServerInitWithCtx(ipaddr, port, msgTable, aSize, &ctx_vng_g);
    if (ret < 0)
    {
        LOG_A(ENB_SS, "[SS-VNG] Connection failure err=%d\n", ret);
        return;
    }
#ifdef ACP_DEBUG_DUMP_MSGS /** TODO: Need to verify */
    adbgSetPrintLogFormat(ctx, true);
#endif
    int fd1 = acpGetSocketFd(ctx_vng_g);
    LOG_A(ENB_SS, "[SS-VNG] Connected: %d\n", fd1);

    //itti_subscribe_event_fd(TASK_VNG, fd1);

    itti_mark_task_ready(TASK_VNG);
}

/*
 * Function : ss_eNB_vng_task
 * Description: Funtion Handles the VNG Task
 * In :
 * req :
 * Out:
 * newState: No impact on state machine.
 *
 */
void *ss_eNB_vng_task(void *arg)
{
    ss_eNB_vng_init();

    while (1)
    {
    	(void)ss_eNB_vng_process_itti_msg(NULL);
    }

    return NULL;
}
