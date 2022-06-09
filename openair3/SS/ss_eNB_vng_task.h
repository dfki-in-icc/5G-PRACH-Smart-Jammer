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
 * File ss_eNB_vng_task.h
 * Brief: Implements the port manager handling for SS mode.
 * Author: Anurag Asokan, Manu Agrawal, Vaibhav Shrivastava, Vijay Chadachan
 * Company: Firecell
 * Email: anurag.ashokan@firecell.io, manu.agrawal@firecell.io, vaibhav.shrivastava@firecell.io,vijay.chadachan@firecell.io
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#ifndef SS_ENB_VNG_TASK_H_
#define SS_ENB_VNG_TASK_H_

void *ss_eNB_vng_process_itti_msg(void *);
void *ss_eNB_vng_task(void *arg);

typedef enum VngProxyCmd
{
    Vng_Invalid_Resp = 0,
    Vng_Config_Cmd = 1,
    Vng_Activate_Cmd = 2,
    Vng_Deactivate_Cmd = 3,
} VngProxyCmd_e;

#endif /* SS_ENB_VNG_TASK_H_ */
