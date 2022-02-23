/**
 * netfunc.h -- A basic networking library for implementing UDP and TCP 
 * client-server applications.
 *
 * Copyright (C) 2011-2013 Pantelis A. Frangoudis <pfrag@aueb.gr>
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

#ifndef _NETFUNC_H_
#define _NETFUNC_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define _PROTO_UDP_	1
#define _PROTO_TCP_	2

#define FROM_CLIENT	1
#define TO_SERVER	1
#define TO_CLIENT	0
#define FROM_SERVER	0

#define ERR_ACCEPT_ON_UDP	-10

struct cnx_info_t {
	int proto;
	char host[80];
	short port;
	int sockfd;
	int cli_sockfd;
	struct sockaddr_in addr;
	struct sockaddr_in cliaddr;
};

/**
 * Initializes a server based on connection information.
 * For a TCP server, it calls socket(), bind() and listen().
 * For a UDP server, it calls socket() and bind().
 * Sets the appropriate info fields and returns the socket descriptor.
 */
int init_server(struct cnx_info_t* info);

/**
 * Sets up a connection to a server. For a TCP client,
 * it calls socket() and connect(). For a UDP one, it just calls socket().
 * Updates the state of info and returns the socket descriptor.
 */
int init_client_connection(struct cnx_info_t* info);

/**
 * Calls accept() on a TCP server socket and returns the client socket
 * or returns ERR_ACCEPT_ON_UPD
 * it case info->proto == _PROTO_UDP_
 */
struct cnx_info_t* accept_client_connection(struct cnx_info_t* info);

/**
 * Sends data.
 */
int write_data(struct cnx_info_t* info, void *data, int datalen, int to_server);

/**
 * Receives data.
 */
int read_data(struct cnx_info_t* info, void *data, int maxlen, int from_client);

/**
 * Peeks data. It should be used to retrieve the first message characters to
 * determine the type of message and content-length, prior to calling the read
 * routine and handing the message for parsing.
 */
int peek_data(struct cnx_info_t *info, void *data, int peek_data_len, int timeout, int from_client);

/**
 * Timeout function. Returns 0 when data are available or -1 on timeout.
 */
int wait_for_data(struct cnx_info_t* info, int msec, int from_client);

#endif

