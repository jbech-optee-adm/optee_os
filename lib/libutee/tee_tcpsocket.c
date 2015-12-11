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
#include <stdint.h>

#include <tee_internal_api.h>
#include <tee_tcpsocket.h>
#include <utee_syscalls.h>

static TEE_Result tcp_open(TEE_iSocketHandle *ctx,
			   void *setup,
			   uint32_t *protocolError)
{
	struct tcp_socket_context *tcp_ctx;
	TEE_tcpSocket_Setup *tcp_setup;
	/* FIXME: Just stubbed so far */
	TEE_Result res = TEE_ERROR_BAD_PARAMETERS;

	(void)protocolError;
	DMSG("Entered");

	if (!ctx || !setup || !protocolError)
		/*
		 * FIXME: Should be updated to return the correct panic
		 * identifier, in this case:
		 * Specification Number: 103, Function Number: 0x101
		 */
		TEE_Panic(TEE_ERROR_BAD_PARAMETERS);

	tcp_setup = (TEE_tcpSocket_Setup *)setup;
	DMSG("tcp_setup->ipVersion: 0x%08x\n"
	     "tcp_setup->server_addr: %s\n"
	     "tcp_setup->port: 0x%08x",
	     tcp_setup->ipVersion,
	     tcp_setup->server_addr,
	     tcp_setup->server_port);

	if (tcp_setup->ipVersion != TEE_IP_VERSION_4 &&
	    tcp_setup->ipVersion != TEE_IP_VERSION_6 &&
	    tcp_setup->ipVersion != TEE_IP_VERSION_DC)
		TEE_Panic(TEE_ERROR_BAD_PARAMETERS); /* MAY panic */

	if (!tcp_setup->server_addr ||
	    tcp_setup->server_port == 0)
		TEE_Panic(TEE_ERROR_BAD_PARAMETERS); /* MAY panic */

	tcp_ctx = TEE_Malloc(sizeof(struct tcp_socket_context),
			     TEE_MALLOC_FILL_ZERO);
	if (!tcp_ctx) {
		*ctx = TEE_HANDLE_NULL;
		return TEE_ERROR_OUT_OF_MEMORY;
	}

	*ctx = (TEE_iSocketHandle)tcp_ctx;

	tcp_ctx->protocol_error = protocolError;

	/* syscall not implemented yet */
	res = utee_socket_open(tcp_setup);

	return res;
}

static TEE_Result tcp_close(TEE_iSocketHandle ctx)
{
	/* FIXME: Just stubbed so far */
	(void)ctx;
	return TEE_SUCCESS;
}

static TEE_Result tcp_send(TEE_iSocketHandle ctx,
			   const void *buf,
			   uint32_t *length,
			   uint32_t timeout)
{
	/* FIXME: Just stubbed so far */
	(void)ctx;
	(void)buf;
	(void)length;
	(void)timeout;
	return TEE_SUCCESS;
}

static TEE_Result tcp_recv(TEE_iSocketHandle ctx,
			   void *buf,
			   uint32_t *length,
			   uint32_t timeout)
{
	/* FIXME: Just stubbed so far */
	(void)ctx;
	(void)buf;
	(void)length;
	(void)timeout;
	return TEE_SUCCESS;
}

static uint32_t tcp_error(TEE_iSocketHandle ctx)
{
	/* FIXME: Just stubbed so far */
	(void)ctx;
	return TEE_SUCCESS;
}

static TEE_Result tcp_ioctl(TEE_iSocketHandle ctx,
			    uint32_t commandCode,
			    const void *buf,
			    uint32_t *length)
{
	/* FIXME: Just stubbed so far */
	(void)ctx;
	(void)commandCode;
	(void)buf;
	(void)length;
	return TEE_SUCCESS;
}

const TEE_iSocket tcp_socket = {
	.TEE_iSocketVersion = 0, /* FIXME: Add correct TEE_iSocketVersion */
	.protocolID = TEE_ISOCKET_PROTOCOLID_TCP,
	.open = tcp_open,
	.close = tcp_close,
	.send = tcp_send,
	.recv = tcp_recv,
	.error = tcp_error,
	.ioctl = tcp_ioctl
};

const TEE_iSocket *TEE_tcpSocket = &tcp_socket;
