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

/*! \file common/utils/websrv/websrv_websockets.c
 * \brief: implementation of web/websockets API
 * \author Francois TABURET
 * \date 2022
 * \version 0.1
 * \company NOKIA BellLabs France
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */

 #include <gnutls/gnutls.h>
 #include <gnutls/x509.h>
 #include <arpa/inet.h>
 #include <jansson.h>
 #include <ulfius.h>
 #include "common/utils/LOG/log.h"
 #include "common/utils/websrv/websrv.h"
 #include "executables/softmodem-common.h"
 #include "time.h"
 #include "common/utils/websrv/websrv_noforms.h"
 #include "common/ran_context.h"
 #include "openair1/PHY/TOOLS/phy_scope.h"
 #include "openair1/PHY/TOOLS/phy_scope_interface.h" 

extern PHY_VARS_NR_UE ***PHY_vars_UE_g;
 
static scopeData_t  scopedata; 
static websrv_scope_params_t scope_params = {0,1000,NULL,&scopedata};
static websrv_params_t *websrvparams_ptr;

void  websrv_scope_sendIQ(int n, websrv_scopedata_msg_t *msg) {
	LOG_I(UTIL,"[websrv] sending %i IQ's \n",n);
/*    int k = (n/MAX_FLOAT_WEBSOCKMSG)  + 1;
    for (int i=0; i<k ; i++)
      for ( int j=0; j<MAX_FLOAT_WEBSOCKMSG && ((i*MAX_FLOAT_WEBSOCKMSG)+j)<n; j++) {
	    spec->buff[id].data_x[j]=x[(i*MAX_FLOAT_WEBSOCKMSG)+j];
	    spec->buff[id].data_y[j]=y[(i*MAX_FLOAT_WEBSOCKMSG)+j];
  }
*/
  msg->src=WEBSOCK_SRC_SCOPE ;
  msg->msgtype=SCOPEMSG_TYPE_DATA ;
  int st = ulfius_websocket_send_message( websrvparams_ptr->wm, U_WEBSOCKET_OPCODE_BINARY,(n*2*sizeof(float))+WEBSOCK_HEADSIZE, (char *)msg);
  if (st != U_OK)
    LOG_I(UTIL, "Error sending scope message, status %i\n",st);   
};


void websrv_websocket_send_scopemessage(char msg_type, char *msg_data, struct _websocket_manager * websocket_manager) {
websrv_msg_t msg;
int st;
  msg.src=WEBSOCK_SRC_SCOPE ;
  msg.msgtype=msg_type;
  sprintf(msg.data,"%s",msg_data);
  st = ulfius_websocket_send_message( websocket_manager, U_WEBSOCKET_OPCODE_BINARY,strlen(msg.data)+WEBSOCK_HEADSIZE, (char *)&msg);
  if (st != U_OK)
    LOG_I(UTIL, "Error sending scope message, status %i\n",st);
}

void websrv_websocket_process_scopemessage(char msg_type, char *msg_data, struct _websocket_manager * websocket_manager) {
  uint32_t *intptr=(uint32_t *)msg_data; 

  LOG_I(UTIL,"[websrv] processing scope message type %i\n", msg_type);
  switch ( msg_type ) {
 
    case SCOPEMSG_TYPE_STATUSUPD:
      if (strncmp(msg_data,"init",4) == 0){
          LOG_I(UTIL,"[websrv] SoftScope init....\n");
		  if (IS_SOFTMODEM_GNB_BIT) {			 
		     scopedata.ru=RC.ru[0];
		     scopedata.gNB=RC.gNB[0];		 
			 scope_params.scopeform = create_phy_scope_gnb();
		  } else if (IS_SOFTMODEM_5GUE_BIT) {
			 scope_params.scopedata = PHY_vars_UE_g[0][0] ;
			 nrUEinitScope(PHY_vars_UE_g[0][0]);
			 scope_params.scopeform = create_phy_scope_nrue(scope_params.selectedData);
		  } else {
            LOG_I(UTIL,"[websrv] SoftScope web interface  not implemented for this softmodem\n");
            websrv_websocket_send_scopemessage(SCOPEMSG_TYPE_STATUSUPD, "disabled", websocket_manager);			  
		  }
	    }
	    scope_params.statusmask |= SCOPE_STATUSMASK_AVAILABLE; 
      if (strncmp(msg_data,"disabled",8) == 0){
		LOG_I(UTIL,"[websrv] SoftScope disabled state client ack  \n");  
        scope_params.statusmask = SCOPE_STATUSMASK_DISABLED;
      }	    
      if (strncmp(msg_data,"start",5) == 0){
        scope_params.statusmask |= SCOPE_STATUSMASK_STARTED;
        scope_params.selectedData=1; // 1 UE to be received from GUI
      }
      if (strncmp(msg_data,"stop",4) == 0){        
        scope_params.statusmask &= ~SCOPE_STATUSMASK_STARTED;
        websrv_websocket_send_scopemessage(SCOPEMSG_TYPE_STATUSUPD, "stopped", websocket_manager);
        }
      break;
    case SCOPEMSG_TYPE_REFRATE:
      scope_params.refrate = (htonl(*intptr))*100;
      break;
    default:
      LOG_W(UTIL,"[websrv] Unknown scope message type: %c /n",msg_type);
      break;
  }
}
/* websocket callbacks as set in callback_websocket, the initial url endpoint which triggers the websocket init */
/* function executed by ulfius when websocket is closed */
void websrv_websocket_onclose_callback (const struct _u_request * request,
                                struct _websocket_manager * websocket_manager,
                                void * websocket_onclose_user_data) {
  websrv_dump_request("websocket close ",request);
}

/* function executed by ulfius in a dedicated thread, should not terminate while client connection is up */
void websrv_websocket_manager_callback(const struct _u_request * request,
                               struct _websocket_manager * websocket_manager,
                               void * websocket_manager_user_data) {

  websrv_dump_request("websocket manager ",request);
  websrv_params_t *websrvparams = (websrv_params_t *)websocket_manager_user_data;
  websrvparams->wm = websocket_manager;
  time_t linuxtime;
  struct tm loctime;
  
  while(1) {
	char strtime[64];
	linuxtime=time(NULL);	  
    localtime_r(&linuxtime,&loctime);
    snprintf(strtime,sizeof(strtime),"%d/%d/%d %d:%d:%d",loctime.tm_mday,loctime.tm_mon,loctime.tm_year+1900,loctime.tm_hour,loctime.tm_min,loctime.tm_sec);
    if( (scope_params.statusmask & SCOPE_STATUSMASK_STARTED) ) {
      if (ulfius_websocket_wait_close(websocket_manager, scope_params.refrate) == U_WEBSOCKET_STATUS_OPEN) {
        websrv_websocket_send_scopemessage(SCOPEMSG_TYPE_TIME, strtime, websocket_manager);
		  if (IS_SOFTMODEM_GNB_BIT) {			 
            phy_scope_gNB(scope_params.scopeform,  scope_params.scopedata, scope_params.selectedData);
		  } 
		  if (IS_SOFTMODEM_5GUE_BIT) {
            phy_scope_nrUE(scope_params.scopeform, (PHY_VARS_NR_UE *)scope_params.scopedata,  0, scope_params.selectedData);
		  }      
      }      
    }
    else if( (scope_params.statusmask == SCOPE_STATUSMASK_UNKNOWN) ) {
	  if (ulfius_websocket_wait_close(websocket_manager, 2000) == U_WEBSOCKET_STATUS_OPEN) {
        if (IS_SOFTMODEM_DOSCOPE | IS_SOFTMODEM_ENB_BIT | IS_SOFTMODEM_4GUE_BIT) {
	      websrv_websocket_send_scopemessage(SCOPEMSG_TYPE_STATUSUPD, "disabled", websocket_manager);
	    } else {     
          websrv_websocket_send_scopemessage(SCOPEMSG_TYPE_STATUSUPD, "enabled", websocket_manager);
        }
      }
    }

    else if( (scope_params.statusmask == SCOPE_STATUSMASK_DISABLED) ) {
      sleep(10);
    }  else {
	  sleep(1);
	}    
  }
  usleep(scope_params.refrate);
  LOG_I(UTIL, "Closing websocket_manager_callback...\n");
}

void websrv_websocket_incoming_message_callback (const struct _u_request * request,
                                         struct _websocket_manager * websocket_manager,
                                         const struct _websocket_message * last_message,
                                         void * websocket_incoming_message_user_data) {


  LOG_I(UTIL, "Incoming message,  opcode: 0x%02x, mask: %d, len: %zu\n",  last_message->opcode, last_message->has_mask, last_message->data_len);
  
  if (last_message->opcode == U_WEBSOCKET_OPCODE_TEXT) {
    LOG_I(UTIL, "text payload '%.*s'", (int)last_message->data_len, last_message->data);
  } else if (last_message->opcode == U_WEBSOCKET_OPCODE_BINARY) {
	websrv_msg_t *msg = (websrv_msg_t *)last_message->data;
    LOG_I(UTIL, "binary payload from %c type %i\n",msg->src, (int)msg->msgtype);
    switch(msg->src) {
		case 's':
          websrv_websocket_process_scopemessage(msg->msgtype, msg->data,websocket_manager);
          break;
        default:
          LOG_W(UTIL, "[websrv] Unknown message source: %c\n",msg->src);
          break;
     }
  }
}

/**
 * callback function, called when the url corresponding to the endpoint set in
 * websrv_init_websocket is requested. that simply set  the websocket callbacks 
 */
int websrv_callback_websocket (const struct _u_request * request, struct _u_response * response, void * user_data) {
  int ret;
  
  websrv_dump_request("websocket ",request);

  if ((ret = ulfius_set_websocket_response(response, NULL, NULL, websrv_websocket_manager_callback, user_data, websrv_websocket_incoming_message_callback,user_data, websrv_websocket_onclose_callback, user_data)) == U_OK) {
    return U_CALLBACK_COMPLETE;
  } else {
    return U_CALLBACK_ERROR;
  }
} 

int websrv_init_websocket(websrv_params_t *websrvparams,char *module) {
	int status=ulfius_add_endpoint_by_val(&(websrvparams->instance), "GET", NULL, module, 1, &websrv_callback_websocket, websrvparams);
	websrvparams_ptr=websrvparams;
    return status;
}
