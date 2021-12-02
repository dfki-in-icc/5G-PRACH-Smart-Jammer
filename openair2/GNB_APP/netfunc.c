/**
 * netfunc.c -- A basic networking library for implementing UDP and TCP 
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

#include "netfunc.h"

/**
 * Initializes a server based on connection information.
 * For a TCP server, it calls socket(), bind() and listen().
 * For a UDP server, it calls socket() and bind().
 * Sets the appropriate info fields and returns the socket descriptor.
 */
int init_server(struct cnx_info_t* info) {
	int server_sockfd;

	memset(&(info->addr), 0, sizeof(info->addr));
	info->addr.sin_family = AF_INET;
	if (!inet_aton(info->host, &(info->addr.sin_addr))) {
		info->addr.sin_addr.s_addr = INADDR_ANY;
	}
	info->addr.sin_port = htons(info->port);

	if (info->proto == _PROTO_TCP_) {
		server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	}
	else {
		server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	}

	if (server_sockfd < 0) {
		return -1;
	}

	if (bind(server_sockfd, (struct sockaddr *)&(info->addr), sizeof(info->addr)) < 0) {
		return -2;
	}

	if (info->proto == _PROTO_TCP_) {
		if (listen(server_sockfd, 20) < 0) {
			return -3;
		}
	}

	info->sockfd = server_sockfd;
	return server_sockfd;	
}

/**
 * Sets up a connection to a server. For a TCP client,
 * it calls socket() and connect(). For a UDP one, it just calls socket().
 * Updates the state of info and returns the socket descriptor.
 */
int init_client_connection(struct cnx_info_t* info) {
	int sockfd;

	info->addr.sin_family = AF_INET;
	if (!inet_aton(info->host, &(info->addr.sin_addr))) {
		return -1;
	}
	info->addr.sin_port = htons(info->port);

	if (info->proto == _PROTO_TCP_) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) return -2;
		if (connect(sockfd, (struct sockaddr *)&(info->addr), sizeof(info->addr)) < 0) {
			return -3;		
		}
	}
	else {
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd < 0) return -2;
	}

	info->sockfd = sockfd;
	return sockfd;
}

/**
 * Calls accept() on a TCP server socket and returns the client socket
 * or returns ERR_ACCEPT_ON_UPD
 * it case info->proto == _PROTO_UDP_
 */
struct cnx_info_t* accept_client_connection(struct cnx_info_t* info) {
	int len = sizeof(info->cliaddr);
	struct cnx_info_t* retval;

	if (info->proto == _PROTO_UDP_) {
		return NULL;
	}
	else {
		retval = (struct cnx_info_t*)malloc(sizeof(struct cnx_info_t));
		memcpy(retval, info, sizeof(struct cnx_info_t));
		retval->cli_sockfd = accept(retval->sockfd, (struct sockaddr *)&(retval->cliaddr), (socklen_t*)&len);
		if (retval->cli_sockfd < 0) {
			free(retval);
			return NULL;
		}
		return retval;
	}
}

/**
 * Sends data.
 */
int write_data(struct cnx_info_t* info, void *data, int datalen, int to_server) {
	int len;
	int retval = 0;
	int sent;
	int i;
	void *offset = data;

	struct sockaddr *sa;
	int outsock = info->sockfd;
	if (to_server) {
		len = sizeof(info->addr);
		sa = (struct sockaddr *)&info->addr;
	}
	else {
		len = sizeof(info->cliaddr);
		sa = (struct sockaddr *)&info->cliaddr;
		if (info->proto == _PROTO_TCP_) {
			outsock = info->cli_sockfd;
		}
	}

	int remaining = datalen;

	for (i = 0; i < datalen / 4092; i++) {
		sent = sendto(outsock, offset, 4092, 0, sa, len);
		remaining -= sent;
		retval += sent;
		offset += sent;
	}
	if (remaining > 0) {
		sent = sendto(outsock, offset, remaining, 0, sa, len);
		if (sent >= 0) {
			retval += sent;
		}
	}
	return retval;
}

/**
 * Receives data.
 */
int read_data(struct cnx_info_t* info, void *data, int maxlen, int from_client) {
	int len;
	int bytes_read = 0;
	void *offset = data;
	int remaining = maxlen;
	int retval = 0;
	int i = 0;
	struct sockaddr *sa;

	int insock = info->sockfd;
	if (from_client) {
		len = sizeof(info->cliaddr);
		sa = (struct sockaddr *)&info->cliaddr;
		if (info->proto == _PROTO_TCP_) {
			insock = info->cli_sockfd;
		}
	}
	else {
		len = sizeof(info->addr);
		sa = (struct sockaddr *)&info->addr;
	}

	while (remaining) {
		bytes_read = recvfrom(insock, offset, 1024, 0, sa, (socklen_t*)&len);
		if (bytes_read < 0) break;
		offset += bytes_read;
		retval += bytes_read;
		remaining -= bytes_read;

	}
	return retval;
}

/**
 * Peeks data. It should be used to retrieve the first message characters to
 * determine the type of message and content-length, prior to calling the read
 * routine and handing the message for parsing.
 */
int peek_data(struct cnx_info_t *info, void *data, int peek_data_len, int timeout, int from_client) {
	/* if timeout == 0, skip the waiting step */
	if (timeout > 0) {
		if (wait_for_data(info, timeout, from_client) < 0) return -1;
	}

	if (from_client) {
		if (info->proto == _PROTO_TCP_) {
			return recv(info->cli_sockfd, data, peek_data_len, MSG_PEEK);
		}
		else {
			return recv(info->sockfd, data, peek_data_len, MSG_PEEK);
		}
	}
	else {
		return recv(info->sockfd, data, peek_data_len, MSG_PEEK);
	}
}

/**
 * Timeout function. Returns 0 when data are available or -1 on timeout.
 */
int wait_for_data(struct cnx_info_t* info, int msec, int from_client) {
	fd_set fds;
	struct timeval tv;
	int sockfd;

	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;

	FD_ZERO(&fds);
	if (from_client && _PROTO_TCP_) {
		sockfd = info->cli_sockfd;
	}
	else {
		sockfd = info->sockfd;
	}

	FD_SET(sockfd, &fds);
	select(sockfd + 1, &fds, NULL, NULL, &tv);
	if (FD_ISSET(sockfd, &fds)) {
		return 0;
	}
	else {
		return -1;
	}
}

