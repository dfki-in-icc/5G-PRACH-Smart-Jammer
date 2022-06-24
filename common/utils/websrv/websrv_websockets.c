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
 
 #include <jansson.h>
 #include <ulfius.h>
 #include <gnutls/gnutls.h>
 #include <gnutls/x509.h>
 #include "common/utils/LOG/log.h"
 #include "common/utils/websrv/websrv.h"
 #include "executables/softmodem-common.h"
 #include "time.h"
 #include <arpa/inet.h>
 
/* websocket callbacks as set in callback_websocket, the initial url endpoint which triggers the websocket init */
void websrv_websocket_onclose_callback (const struct _u_request * request,
                                struct _websocket_manager * websocket_manager,
                                void * websocket_onclose_user_data) {
  websrv_dump_request("websocket close ",request);
}

void websrv_websocket_manager_callback(const struct _u_request * request,
                               struct _websocket_manager * websocket_manager,
                               void * websocket_manager_user_data) {

  websrv_dump_request("websocket manager ",request);
  
  time_t linuxtime;
  struct tm loctime;
  
  websrv_msg_t msg;
  while(1) {
	linuxtime=time(NULL);	  
    localtime_r(&linuxtime,&loctime);
    msg.src=htonl(1);
    snprintf(msg.data,sizeof(msg.data),"%d/%d/%d %d:%d:%d",loctime.tm_mday,loctime.tm_mon,loctime.tm_year+1900,loctime.tm_hour,loctime.tm_min,loctime.tm_sec);
//    Send text message without fragmentation
    if (ulfius_websocket_wait_close(websocket_manager, 2000) == U_WEBSOCKET_STATUS_OPEN) {
      if (ulfius_websocket_send_message(websocket_manager, U_WEBSOCKET_OPCODE_TEXT,strlen(msg.data), msg.data ) != U_OK) {
        LOG_W(UTIL,"Error sending websocket message\n");
    }
  }
  

  // Send ping message
  if (ulfius_websocket_wait_close(websocket_manager, 2000) == U_WEBSOCKET_STATUS_OPEN) {
    if (ulfius_websocket_send_message(websocket_manager, U_WEBSOCKET_OPCODE_PING, 0, NULL) != U_OK) {
      LOG_W(UTIL, "Error send ping message");
    }
  }
  sleep(1);
  // Send binary message without fragmentation
  if (ulfius_websocket_wait_close(websocket_manager, 2000) == U_WEBSOCKET_STATUS_OPEN) {
   if (ulfius_websocket_send_message(websocket_manager, U_WEBSOCKET_OPCODE_BINARY,strlen(msg.data)+sizeof(msg.src ), (char *)&msg) != U_OK) {
     LOG_I(UTIL, "Error send binary message without fragmentation");
   }
 }

  // Send JSON message without fragmentation

// if (ulfius_websocket_wait_close(websocket_manager, 2000) == U_WEBSOCKET_STATUS_OPEN) {
//    json_t * message = json_pack("{ss}", "send", json_string(strtime));
//    if (ulfius_websocket_send_json_message(websocket_manager, message) != U_OK) {
//   }
//   json_decref(message);
// }

  }
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
    LOG_I(UTIL, "binary payload from %i: %48s\n",msg->src,msg->data);
  }
}

/**
 * Ulfius main callback function that simply set  the websocket callbacks 
 */
int websrv_callback_websocket (const struct _u_request * request, struct _u_response * response, void * user_data) {
  int ret;
  
  websrv_dump_request("websocket ",request);
  websrv_string_response("softscope", response, 200) ;
  if ((ret = ulfius_set_websocket_response(response, NULL, NULL, websrv_websocket_manager_callback, NULL, websrv_websocket_incoming_message_callback, NULL, websrv_websocket_onclose_callback, NULL)) == U_OK) {
    return U_CALLBACK_COMPLETE;
  } else {
    return U_CALLBACK_ERROR;
  }
} 

int websrv_init_websocket(websrv_params_t *websrvparams,char *module) {
	int status=ulfius_add_endpoint_by_val(&(websrvparams->instance), "GET", NULL, module, 1, &websrv_callback_websocket, NULL);
    return status;
}
