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
#include <tee_api_types.h>
#include <tee/tee_svc_socket.h>
#include <kernel/thread.h>
#include <kernel/tee_rpc.h>
#include <mm/core_mmu.h>
#include <util.h>
#include <trace.h>

/* FIXME: Eventually this shouldn't be needed here */
#include <tee_tcpsocket.h>

TEE_Result syscall_socket_open(void *setup)
{
	paddr_t cookie = 0;
	paddr_t phpayload = 0;
	struct teesmc32_param params;
	TEE_Result res = TEE_ERROR_BAD_PARAMETERS;
	void *payload = NULL;

	/*
	 * FIXME: Add parameter for different socket types instead of hardcode
	 * for TCP
	 */
	thread_optee_rpc_alloc_payload(sizeof(struct TEE_tcpSocket_Setup_s),
				       &phpayload, &cookie);
	if (!phpayload)
		return TEE_ERROR_OUT_OF_MEMORY;

	if (!ALIGNMENT_IS_OK(phpayload, struct TEE_tcpSocket_Setup_s)) {
		res = TEE_ERROR_GENERIC;
		goto exit;
	}

	if (core_pa2va(phpayload, &payload)) {
		thread_optee_rpc_free_payload(cookie);
		res = TEE_ERROR_GENERIC;
		goto exit;
	}

	payload = setup;

	memset(&params, 0, sizeof(params));
	params.attr = TEESMC_ATTR_TYPE_MEMREF_INOUT |
			 (TEESMC_ATTR_CACHE_I_WRITE_THR |
				TEESMC_ATTR_CACHE_O_WRITE_THR) <<
					TEESMC_ATTR_CACHE_SHIFT;

	params.u.memref.buf_ptr = phpayload;
	params.u.memref.size = sizeof(struct TEE_tcpSocket_Setup_s);

	DMSG("Calling tee-supplicant with (TEE_SOCKET_TCP_OPEN)");
	res = thread_rpc_cmd(TEE_SOCKET_TCP_OPEN, 1, &params);
exit:
	thread_optee_rpc_free_payload(cookie);
	return res;
}
