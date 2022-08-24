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

/*! \file common/utils/websrv/websrv_scope.c
 * \brief: implementation of web API specific for oai softscope
 * \author Francois TABURET
 * \date 2022
 * \version 0.1
 * \company NOKIA BellLabs France
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */
 #include <libgen.h>
 #include <jansson.h>
 #include <ulfius.h>
 #include <gnutls/gnutls.h>
 #include <gnutls/x509.h>
 #include "common/config/config_userapi.h"
 #include "common/utils/LOG/log.h"
 #include "common/utils/websrv/websrv.h"
 #define WEBSERVERCODE
 #include "common/utils/telnetsrv/telnetsrv.h"
 #include "common/utils/load_module_shlib.h"
 
 /* default callback tries to find a file in the web server repo (path exctracted from <websrvparams.url>) and if found streams it */
 int websrv_scope_callback_set_params (const struct _u_request * request, struct _u_response * response, void * user_data) {
  websrv_dump_request("scope set params ", request);
  ulfius_set_empty_body_response(response, 200);
  return U_CALLBACK_COMPLETE;
}
 
int websrv_scope_callback_get_desc (const struct _u_request * request, struct _u_response * response, void * user_data) {
  websrv_dump_request("scope get desc ", request);
  ulfius_set_empty_body_response(response, 200);
  return U_CALLBACK_COMPLETE;
}
 
void websrv_init_scope(websrv_params_t *websrvparams) {
  int (* callback_functions_scope[3])(const struct _u_request * request, 
                                      struct _u_response * response,
                                      void * user_data) ={websrv_callback_okset_softmodem_cmdvar,websrv_scope_callback_set_params,websrv_scope_callback_get_desc};
  char *http_methods[3]={"OPTIONS","POST","GET"};

  websrv_add_endpoint(http_methods,3,"oaisoftmodem","scopectrl" ,callback_functions_scope, websrvparams);
 }
