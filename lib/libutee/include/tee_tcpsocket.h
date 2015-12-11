/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TEE_TCPSOCKET_H
#define TEE_TCPSOCKET_H
#include <tee_isocket.h>

/* Protocol identifier */
#define TEE_ISOCKET_PROTOCOLID_TCP 0x65

/* Instance specific errors */
#define TEE_ISOCKET_TCP_ERROR_HOSTNAME 0xF1010001

typedef enum TEE_ipSocket_ipVersion_e {
	TEE_IP_VERSION_DC = 0, /* don’t care */
	TEE_IP_VERSION_4 = 1,
	TEE_IP_VERSION_6 = 2
} TEE_ipSocket_ipVersion;

/* declare the client setup struct variables */
typedef struct TEE_tcpSocket_Setup_s {
	TEE_ipSocket_ipVersion ipVersion;
	char *server_addr;
	int server_port;
} TEE_tcpSocket_Setup;

struct tcp_socket_context {
	int sockfd;
	uint32_t *protocol_error;
};

/* declare the function pointer handle */
extern const TEE_iSocket *TEE_tcpSocket;

#endif
