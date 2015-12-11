/*
 * Copyright (c) 2016, Linaro Limited
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
#include <stdio.h>
#include <kernel/tee_rpc.h>
#include <kernel/tee_ta_manager.h>
#include <kernel/thread.h>
#include <mm/core_mmu.h>
#include <tee_api_types.h>
#include <tee/tee_svc.h>
#include <tee/tee_svc_socket.h>
#include <trace.h>
#include <util.h>

/* FIXME: Eventually this shouldn't be needed here */
#include <tee_tcpsocket.h>

static TEE_Result copy_setup(struct tee_ta_session *sess,
			     struct socket_payload *spl,
			     TEE_tcpSocket_Setup *setup)
{
	TEE_tcpSocket_Setup s;

	TEE_Result res = tee_svc_copy_from_user(sess, &s, setup,
				     sizeof(TEE_tcpSocket_Setup));
	if (res != TEE_SUCCESS)
		goto exit;

	spl->ip_version = s.ipVersion;
	spl->server_addr_len = s.server_addr_len;
	spl->server_port = s.server_port;

	res = tee_svc_copy_from_user(sess, spl->server_addr, setup->server_addr,
				     s.server_addr_len);
exit:
	return res;
}

static TEE_Result copy_ctx(struct tee_ta_session *sess,
			   struct socket_payload *spl,
			   struct tcp_socket_context *ctx)
{
	struct tcp_socket_context c;
	TEE_Result res = tee_svc_copy_from_user(sess, &c, ctx,
					sizeof(struct tcp_socket_context));
	if (res != TEE_SUCCESS)
		goto exit;

	spl->sockfd = c.sockfd;
	spl->protocol_error = c.protocol_error;
exit:
	return res;
}

TEE_Result syscall_socket_open(void *setup, void *ctx)
{
	TEE_Result res = TEE_SUCCESS;
	struct tee_ta_session *sess = NULL;
	struct socket_payload *spl = NULL;
	struct teesmc32_param params;
	paddr_t pa_payload = 0;
	paddr_t cookie = 0;

	thread_optee_rpc_alloc_payload(sizeof(struct socket_payload),
				       &pa_payload, &cookie);
	if (!pa_payload)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!ALIGNMENT_IS_OK(pa_payload, struct socket_payload))
		goto exit;

	if (core_pa2va(pa_payload, &spl))
		goto exit;

	memset(&params, 0, sizeof(params));
	params.attr = TEESMC_ATTR_TYPE_MEMREF_INOUT |
			(TEESMC_ATTR_CACHE_I_WRITE_THR |
			 TEESMC_ATTR_CACHE_O_WRITE_THR) <<
				TEESMC_ATTR_CACHE_SHIFT;

	params.u.memref.buf_ptr = pa_payload;
	params.u.memref.size = sizeof(spl);

	res = copy_setup(sess, spl, (TEE_tcpSocket_Setup *)setup);
	if (res != TEE_SUCCESS)
		goto exit;

	res = copy_ctx(sess, spl, (struct tcp_socket_context *)ctx);
	if (res != TEE_SUCCESS)
		goto exit;

	res = thread_rpc_cmd(TEE_SOCKET_TCP_OPEN, 1, &params);
	/* FIXME: Write back sockfd */
exit:
	thread_optee_rpc_free_payload(cookie);
	return res;
}

