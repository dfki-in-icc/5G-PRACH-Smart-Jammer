/**
 * protocol.h
 *
 * Copyright (C) 2017 Pantelis A. Frangoudis <pfrag@eurecom.fr>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>

#include "netfunc.h"

#define PEEK_DATA_LEN 30

#define MTYPE_MREG				10
#define MTYPE_MRSP				20

#define MREG_HDR "MREG"
#define MRSP_HDR "MRSP"

#define STATUS_OK				200
#define STATUS_GEN_ERR			444

typedef struct slice_orchestrator_agent_mreg_info_s {
  int eNB_id;
  char ipaddr[32];
  char localaddr[32];
  int sctp_in;
  int sctp_out;
  int plmn_index;
} slice_orchestrator_agent_mreg_info_t;

/**
 * Peek connection and retrieve message info. Then, read the appropriate
 * amount of data and message type return the message string for parsing.
 */
char *recv_protocol_message(struct cnx_info_t *info, int *mtype, int timeout, int from_client);

/**
 * Just call the lower-level write_data method to send a message.
 */
int xmit_protocol_message(struct cnx_info_t *info, char *message, int to_server);

/*************************************/

/**
 * Parses an MREG request
 */
slice_orchestrator_agent_mreg_info_t *parse_mreg_request(char *message);

/**
 * Generate MREG request
 */
char *generate_mreg_request(slice_orchestrator_agent_mreg_info_t *info);

/*************************************/

char *msg_to_lower_case(char *msg, int len);

#endif

