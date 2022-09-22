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

/*! \file common/utils/websrv/websrv.h
 * \brief: implementation of web API
 * \author Francois TABURET
 * \date 2022
 * \version 0.1
 * \company NOKIA BellLabs France
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */
 
#ifndef WEBSRV_H
#define WEBSRV_H

#define WEBSRV_MODNAME  "websrv"

#define WEBSRV_PORT               8090
/* websrv_printf_t is an internal structure storing messages while processing a request */
/* The meaage is used to fill a response body                                           */
typedef struct {
  pthread_mutex_t mutex;          // protect the message betwween the websrv_printf_start and websrv_print_end calls
  struct _u_response * response;  // the ulfius response structure, used to send the message
  char *buff;                     // a buffer to store the message, allocated in websrv_printf_start, free in websrv_print_end
  int  buffsize;                  //
  char *buffptr;                  // pointer to free portion of buff
} websrv_printf_t;

/* websrv_params_t is an internal structure storing all the current parameters and */
/* global variables used by the web server                                        */
typedef struct {
	 struct _u_instance instance;         // ulfius (web server) instance
	 struct _websocket_manager * wm;      // web socket instance
     unsigned int dbglvl;                 // debug level of the server
     int priority;                        // server running priority
     unsigned int   listenport;           // ip port the telnet server is listening on
     unsigned int   listenaddr;           // ip address the telnet server is listening on
     unsigned int   listenstdin;          // enable command input from stdin
     char *url;                           // url for the main (initial) web page 
     char *certfile;                      // cert file
     char *keyfile;                       // key file  
     char *rootcafile;                    // root ca file
     void *telnetparams;                  // pointer to telnet server parameters type is telnetsrv_params_t, don't want to enforce telnet include here
} websrv_params_t;

#define WEBSOCK_SRC_SCOPE                's'
typedef struct {
	 unsigned char src;                       // message source
	 unsigned char msgtype;                   // message type
	 unsigned char hdr_unused[6];             // 6 unused char
     char    data[1408];                      // 64*22
     
} websrv_msg_t;
#define MAX_NIQ_WEBSOCKMSG   180000 //max number of 16 bits iq's in pre-allocated buffer
#define MAX_LLR_WEBSOCKMSG    (MAX_NIQ_WEBSOCKMSG*2) //max number of 8 bits llr's in pre-allocated buffer

typedef struct {
	 unsigned char src;                          // message source
	 unsigned char msgtype;                      // message type
	 unsigned char msgseg;                       // message segment number
	 unsigned char chartid;                      // identify chart (scope window)
	 unsigned char datasetid;                    // identify dataset in chart
	 unsigned char update;                       // should chart be updated
	 unsigned char hdr_unused[2];                // 2 unused char
     int16_t         data_xy[MAX_NIQ_WEBSOCKMSG*2]; // data buffer
} websrv_scopedata_msg_t;
#define WEBSOCK_HEADSIZE (offsetof(websrv_msg_t, data))

#define SCOPE_STATUSMASK_UNKNOWN         0          // websocket initialized, no scope request received
#define SCOPE_STATUSMASK_AVAILABLE       1          // scope can be started (available in exec and not started with other if */
#define SCOPE_STATUSMASK_STARTED         2          // scope running
#define SCOPE_STATUSMASK_DATAACK         4          // wait for data message acknowledge before sending
#define SCOPE_STATUSMASK_DISABLED        (1LL<<63)  // scope disabled (running with xform interface or not implemented in exec)

/* values for websocket message type */
/* websocket softscope message: */
#define SCOPEMSG_TYPE_STATUSUPD          1    // scope status: available, started....
#define SCOPEMSG_TYPE_REFRATE            2    // scope refresh delay
#define SCOPEMSG_TYPE_TIME               3    // time
#define SCOPEMSG_TYPE_DATA               10   // graph data
#define SCOPEMSG_TYPE_DATAACK            11   // graph data reception and processing acknowledge
#define SCOPEMSG_TYPE_DEFINEWINDOW       20   // scope window definition 
typedef struct {
	 uint64_t statusmask;                     // 
	 uint32_t refrate;                        // in 100 ms
	 void  *scopeform;                        // OAI_phy_scope_t pointer returned by create_phy_scope_xxx functions
	 void  *scopedata;                        // scopeData_t pointer, filled at init time, contains pointers and functions to retrieve softmodem data
     uint32_t selectedTarget;                   // index to UE/gNB 
     int   xmin;                              // iq view data limit
     int   xmax;
     int   ymin;
     int   ymax;
     int   llr_ythresh;                       // llrview llr threshold
} websrv_scope_params_t;

extern void websrv_printjson(char * label, json_t *jsonobj);

extern void websrv_printf_start(struct _u_response * response, int buffsize );
extern void websrv_printf( const char *message,  ...);
extern void websrv_printf_end(int httpstatus );
extern void websrv_jbody( struct _u_response * response, json_t *jbody);
extern void websrv_init_scope(websrv_params_t *websrvparams) ;
extern int websrv_init_websocket(websrv_params_t *websrvparams,char *module); 
extern void websrv_websocket_send_message(char msg_src, char msg_type, char *msg_data, struct _websocket_manager * websocket_manager);
extern void websrv_dump_request(char *label, const struct _u_request *request);
extern int websrv_string_response(char *astring, struct _u_response * response, int httpstatus) ;
extern void websrv_scope_ws_close() ;
extern void websrv_scope_ws_cb(void *user_data);
extern void websrv_scope_manager(uint64_t lcount,websrv_params_t *websrvparams);
extern void  websrv_scope_senddata(int numd,int dsize, websrv_scopedata_msg_t *msg);
extern void websrv_websocket_process_scopemessage(char msg_type, char *msg_data, struct _websocket_manager * websocket_manager);
extern int websrv_callback_okset_softmodem_cmdvar(const struct _u_request * request, struct _u_response * response, void * user_data);
extern int websrv_add_endpoint( char **http_method, int num_method, const char * url_prefix,const char * url_format,
                         int (* callback_function[])(const struct _u_request * request, 
                                                   struct _u_response * response,
                                                   void * user_data),
                         void * user_data);
extern websrv_scope_params_t *websrv_scope_getparams(void);
#endif
