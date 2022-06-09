/*
 * Copyright 2022 Sequans Communications.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// Internal includes
#include "acpSocket.h"
#include "adbg.h"

int acpSocketConnect(IpAddress_t ipaddr, int port)
{
	ACP_DEBUG_ENTER_LOG();

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = ntohl(ipaddr.v.ipv4);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	// Configure KEEPALIVE
	const int keepalive = 1;
	const int keepalive_time = 30;
	const int keepalive_intvl = 2;
	const int keepalive_probes = 5;
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, (socklen_t)sizeof(keepalive)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepalive_time, (socklen_t)sizeof(keepalive_time)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepalive_intvl, (socklen_t)sizeof(keepalive_intvl)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepalive_probes, (socklen_t)sizeof(keepalive_probes)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	int arg = fcntl(sock, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(sock, F_SETFL, arg);

	if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		int myerrno = errno;
		if (myerrno == EINPROGRESS) {
			fd_set fdset;
			FD_ZERO(&fdset);
			FD_SET(sock, &fdset);

			struct timeval tv;
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
				socklen_t optlen = sizeof(int);
				int valopt;
				if ((getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)&valopt, &optlen) == 0) && !valopt) {
					ACP_DEBUG_LOG("Connected to server: %s:%d", inet_ntoa(sin.sin_addr), port);
					ACP_DEBUG_EXIT_LOG(NULL);
					return sock;
				}
			}
		}
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	ACP_DEBUG_LOG("Connected to server: %s:%d", inet_ntoa(sin.sin_addr), port);
	ACP_DEBUG_EXIT_LOG(NULL);
	return sock;
}

int acpSocketListen(IpAddress_t ipaddr, int port)
{
	ACP_DEBUG_ENTER_LOG();

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = ntohl(ipaddr.v.ipv4);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	// Configure KEEPALIVE
	const int keepalive = 1;
	const int keepalive_time = 30;
	const int keepalive_intvl = 2;
	const int keepalive_probes = 5;
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, (socklen_t)sizeof(keepalive)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepalive_time, (socklen_t)sizeof(keepalive_time)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepalive_intvl, (socklen_t)sizeof(keepalive_intvl)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}
	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepalive_probes, (socklen_t)sizeof(keepalive_probes)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	const int reuse = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, (socklen_t)sizeof(reuse)) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	int arg = fcntl(sock, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(sock, F_SETFL, arg);

	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		int myerrno = errno;
		if (myerrno == EINPROGRESS) {
			fd_set fdset;
			FD_ZERO(&fdset);
			FD_SET(sock, &fdset);

			struct timeval tv;
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
				socklen_t optlen = sizeof(int);
				int valopt;
				if ((getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)&valopt, &optlen) == 0) && !valopt) {
					return sock;
				}
			}
		}
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	if (listen(sock, 3) == -1) {
		close(sock);
		ACP_DEBUG_EXIT_LOG(strerror(errno));
		return -1;
	}

	ACP_DEBUG_EXIT_LOG(NULL);
	return sock;
}

int acpSocketSelect(int sock, MSec_t socketTimeout)
{
#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_ENTER_TRACE_LOG();
#endif

	if (sock < 0) {
		ACP_DEBUG_EXIT_TRACE_LOG("invalid socket argument");
		return -1;
	}

	int attemps = 3;
	int available;

	do {
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(sock, &fdset);

		struct timeval tv;
		tv.tv_sec = socketTimeout / 1000;
		tv.tv_usec = (socketTimeout % 1000) * 1000;

		available = select(sock + 1, &fdset, NULL, NULL, &tv);
		if (available < 0) {
			if ((errno == EINTR) || (errno == EAGAIN)) {
				available = 0;
			} else {
				ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
				return -1;
			}
		} else {
			break;
		}

		if (--attemps <= 0) break;

	} while (available == 0);

	if (available == 0) {
#ifdef ACP_DEBUG_TRACE_FLOOD
		ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
		return 0;
	}

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
	return 1;
}

int acpSocketSelectMulti(int* sock, MSec_t socketTimeout, size_t peersSize, int* peers)
{
	SIDL_ASSERT(sock);

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_ENTER_TRACE_LOG();
#endif

	if (*sock < 0) {
		ACP_DEBUG_EXIT_TRACE_LOG("invalid socket argument");
		return -1;
	}

	int attemps = 3;
	int available;

	fd_set fdset;

	do {
		FD_ZERO(&fdset);

		int maxSock = 0;

		if (*sock != -1) {
			FD_SET(*sock, &fdset);
			maxSock = *sock;
		}

		for (size_t i = 0; i < peersSize; i++) {
			if (peers[i] != -1) {
				FD_SET(peers[i], &fdset);
				if (peers[i] > maxSock) {
					maxSock = peers[i];
				}
			}
		}

		struct timeval tv;
		tv.tv_sec = socketTimeout / 1000;
		tv.tv_usec = (socketTimeout % 1000) * 1000;

		available = select(maxSock + 1, &fdset, NULL, NULL, &tv);
		if (available < 0) {
			if ((errno == EINTR) || (errno == EAGAIN)) {
				available = 0;
			} else {
				ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
				return -1;
			}
		} else {
			break;
		}

		if (--attemps <= 0) break;

	} while (available == 0);

	if (available == 0) {
#ifdef ACP_DEBUG_TRACE_FLOOD
		ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
		return 0;
	}

	int retSock = -1;

	if (*sock != -1 && FD_ISSET(*sock, &fdset)) {
		retSock = *sock;
	} else {
		for (size_t i = 0; i < peersSize; i++) {
			if (peers[i] != -1 && FD_ISSET(peers[i], &fdset)) {
				retSock = peers[i];
				break;
			}
		}
	}

	SIDL_ASSERT(retSock != -1);
	*sock = retSock;

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
	return 1;
}

int acpSocketAccept(int sock)
{
	struct sockaddr_in sin;
	int sinSize = sizeof(struct sockaddr_in);
	return accept(sock, (struct sockaddr*)&sin, (socklen_t*)&sinSize);
}

int acpSocketSend(int sock, size_t size, const unsigned char* buffer)
{
#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_ENTER_TRACE_LOG();
#endif

	if (sock < 0) {
		ACP_DEBUG_EXIT_TRACE_LOG("invalid socket argument");
		return -1;
	}

	ssize_t index = 0;
	size_t dataToWrite = size;

	do {
		ssize_t bytes = send(sock, buffer + index, dataToWrite, MSG_DONTWAIT);
		if (bytes <= 0) {
			int err = errno;
			if ((bytes < 0) && (err == EPIPE)) {
				// TODO: fix infinite loop between select() > 0 and send() <= 0
			} else if (bytes < 0) {
				if ((err != EAGAIN) && (err != EPIPE)) {
					ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
					return -1;
				}
			}
			if ((bytes == 0) || (err == EAGAIN)) {
				fd_set fdset;
				FD_ZERO(&fdset);
				FD_SET(sock, &fdset);

				struct timeval tv;
				tv.tv_sec = 1;
				tv.tv_usec = 0;

				int ret = select(sock + 1, NULL, &fdset, NULL, &tv);
				if (ret > 0) {
					continue;
				} else if (ret == 0) {
					continue;
				}
			}
			ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
			return -1;
		}

		index += bytes;
		dataToWrite = size - (size_t)index;

	} while ((size_t)index < size);

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
	return (int)(size - dataToWrite);
}

int acpSocketReceive(int sock, size_t size, unsigned char* buffer, MSec_t socketTimeout, bool* disconnected)
{
#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_ENTER_TRACE_LOG();
#endif

	if (sock < 0) {
		ACP_DEBUG_EXIT_TRACE_LOG("invalid socket argument");
		return -1;
	}

	ssize_t bytes = 0;
	int available = 0;

	if (disconnected) {
		*disconnected = false;
	}

	while ((size_t)bytes < size) {
		int attemps = 3;

		do {
			fd_set fdset;
			FD_ZERO(&fdset);
			FD_SET(sock, &fdset);

			struct timeval tv;
			tv.tv_sec = socketTimeout / 1000;
			tv.tv_usec = (socketTimeout % 1000) * 1000;

			available = select(sock + 1, &fdset, NULL, NULL, &tv);
			if (available < 0) {
				if ((errno == EINTR) || (errno == EAGAIN)) {
					available = 0;
				} else {
					ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
					return -1;
				}
			} else {
				break;
			}

			if (--attemps <= 0) break;

		} while (available == 0);

		if (available == 0) {
#ifdef ACP_DEBUG_TRACE_FLOOD
			ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
			return bytes;
		}

		ssize_t ret = recv(sock, buffer + bytes, size - bytes, 0);
		if (disconnected) {
			if (ret == 0) {
				*disconnected = true;
				ACP_DEBUG_EXIT_TRACE_LOG(NULL);
				return 0;
			} else if (ret < 0) {
				ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
				return -1;
			}
		} else {
			if (ret <= 0) {
				ACP_DEBUG_EXIT_TRACE_LOG(strerror(errno));
				return -1;
			}
		}

		bytes += ret;
	}

#ifdef ACP_DEBUG_TRACE_FLOOD
	ACP_DEBUG_EXIT_TRACE_LOG(NULL);
#endif
	return (int)bytes;
}

int acpSocketClose(int sock)
{
	return sock >= 0 ? close(sock) : -1;
}

bool acpConvertIp(const char* ipstr, IpAddress_t* ipaddr)
{
	unsigned int b0, b1, b2, b3;

	int count = sscanf(ipstr, "%u.%u.%u.%u", &b0, &b1, &b2, &b3);

	if (count == 4) {
		ipaddr->d = IP_V4;
		if (b0 > 255 || b1 > 255 || b2 > 255 || b3 > 255) {
			return false;
		}

		ipaddr->v.ipv4 = ((Ip4Address_t)(b0 & 0xFF) << 24 |
						  (Ip4Address_t)(b1 & 0xFF) << 16 |
						  (Ip4Address_t)(b2 & 0xFF) << 8 |
						  (Ip4Address_t)(b3 & 0xFF) << 0);
		return true;
	}

	return false;
}
