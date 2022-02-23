/**
 * protocol.c
 *
 * Copyright (C) 2017 Pantelis Frangoudis <pfrag@eurecom.fr>
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


#include "protocol.h"
#include "netfunc.h"
#include <string.h>
#include <stdio.h>
/**
 * Converts the message body to lower case
 */
char *msg_to_lower_case(char *msg, int len) {
	char *retval;
	int i;

	retval = (char *)malloc(len + 1);
	memset(retval, 0, len + 1);
	
	memcpy(retval, msg, 4);

	for (i = 4; i < len; i++) {
		retval[i] = (char)tolower((unsigned char)msg[i]);
	}
	return retval;
}

/**
 * Peek connection and retrieve message info. Then, read the appropriate
 * amount of data and message type and return the message string for parsing.
 */
char *recv_protocol_message(struct cnx_info_t *info, int *mtype, int timeout, int from_client) {
	int clen;
	int msize;
	char data[PEEK_DATA_LEN];
	char *ldata;
	memset(data, 0, PEEK_DATA_LEN);

	char *message;

	int hdr_len = peek_data(info, data, PEEK_DATA_LEN, timeout, from_client);
	printf("hdr_len %d \n",hdr_len);
	if (hdr_len < 4) {
		return NULL;
	}

	ldata = msg_to_lower_case(data, hdr_len);
	if (!ldata) {
		return NULL;
	}
	printf("msg_to_lower_case\n");

	/* read message type */
	if (!strncasecmp(data, MREG_HDR, strlen(MREG_HDR))) {
		*mtype = MTYPE_MREG;
		if (sscanf(ldata, MREG_HDR"\r\ncontent-length: %d", &clen) != 1) {
			free(ldata);
			return NULL;
		}
	printf("read message type\n");

		msize = strlen(MREG_HDR"\r\ncontent-length: ") + (clen?(int)log10(clen):0) + 3 + clen;
	}
	else
	if (!strncasecmp(data, MRSP_HDR, strlen(MRSP_HDR))) {
		*mtype = MTYPE_MRSP;
		if (sscanf(ldata, MRSP_HDR"\r\ncontent-length: %d", &clen) != 1) {
			free(ldata);
			return NULL;
		}
		
		msize = strlen(MRSP_HDR"\r\ncontent-length: ") + (clen?(int)log10(clen):0) + 3 + clen;
	}
	else {
		free(ldata);
		return NULL;
	}
	printf("read message type done \n");

	if (ldata) free(ldata);
	printf("read message \n");

	message = (char*)malloc(msize + 1);
	memset(message, 0, msize + 1);
	/* read message */

	if (read_data(info, message, msize, from_client) < 1) {
		if (message) free(message);
		return NULL;
	}
	printf("read message  done\n");

	return message;
}

/**
 * Just call the lower-level write_data method to send a message.
 */
int xmit_protocol_message(struct cnx_info_t *info, char *message, int to_server) {
	return write_data(info, (void *)message, strlen(message), to_server);
}


/**
 * Parse an MREG message and return the IPv4 address of the MME.
 */
slice_orchestrator_agent_mreg_info_t *parse_mreg_request(char *message) {
	int clen;
	char *lmessage;
	char *ipaddr;
  slice_orchestrator_agent_mreg_info_t *retval;

	if (!message || strlen(message) < strlen(MRSP_HDR"\r\nContent-Length: ")) {
		return NULL;
	}

	if (strncasecmp(message, MREG_HDR"\r\nContent-Length: ", strlen(MREG_HDR"\r\nContent-Length: "))) {
		return NULL;
	}

	/* convert message to lowercase */
	lmessage = msg_to_lower_case(message, strlen(message));
	if (!lmessage) {
		return NULL;
	}

	retval = (slice_orchestrator_agent_mreg_info_t*)calloc(sizeof(slice_orchestrator_agent_mreg_info_t), 1); /* some overbooking here...*/
  printf("%s\n", lmessage);
	/* parse */
	if (
		sscanf(
			lmessage, 
			MREG_HDR"\r\ncontent-length: %d\r\nenbid: %d\r\nmme: %s\r\nlocal: %s\r\nsctp-in: %d\r\nsctp-out: %d\r\nplmn_index: %d\r\n",
			&clen, &retval->eNB_id, retval->ipaddr, retval->localaddr, &retval->sctp_in, &retval->sctp_out,&retval->plmn_index) != 7 ) {
		free(lmessage);
		return NULL;
	}

	return retval;
}

char *generate_mreg_request(slice_orchestrator_agent_mreg_info_t *info) {
	int mlen;
	int clen;
	int pos;
	char *retval;

	mlen = 0;
	clen = 0;

	mlen += strlen(MREG_HDR"\r\nContent-length: \r\n"); /* clen will be added at the end */
	mlen += strlen("enbid: \r\nmme: \r\nlocal: \r\nsctp-in: \r\nsctp-out: \r\n") + strlen(info->ipaddr) + strlen(info->localaddr) + (int)log10(info->eNB_id) + 1 + (int)log10(info->sctp_in) + 1 + (int)log10(info->sctp_out) + 1;

	clen = strlen("enbid: \r\nmme: \r\nlocal: \r\nsctp-in: \r\nsctp-out: \r\n") + strlen(info->ipaddr) + strlen(info->localaddr) + (int)log10(info->eNB_id) + 1 + (int)log10(info->sctp_in) + 1 + (int)log10(info->sctp_out) + 1;
	mlen += (int)log10(clen) + 1; /* content-length digits */

	retval = (char *)malloc(mlen + 1);
	memset(retval, 0, mlen + 1);

	sprintf(
		retval, 
		MREG_HDR"\r\nContent-Length: %d\r\nenbid: %d\r\nmme: %s\r\nlocal: %s\r\nsctp-in: %d\r\nsctp-out: %d\r\n",
		clen, info->eNB_id, info->ipaddr, info->localaddr, info->sctp_in, info->sctp_out);

	return retval;
}


