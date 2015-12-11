/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 // * Redistribution and use in source and binary forms, with or without
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
#ifndef TEE_ISOCKET_H
#define TEE_ISOCKET_H

#define TEE_ISOCKET_ERROR_PROTOCOL 		0xF1007001
#define TEE_ISOCKET_ERROR_REMOTE_CLOSED 	0xF1007002
#define TEE_ISOCKET_ERROR_TIMEOUT 		0xF1007003
#define TEE_ISOCKET_ERROR_OUT_OF_RESOURCES	0xF1007004
#define TEE_ISOCKET_ERROR_LARGE_BUFFER		0xF1007005

typedef struct __TEE_iSocketHandle *TEE_iSocketHandle;

typedef const struct TEE_iSocket_s
{
	uint32_t TEE_iSocketVersion;
	uint8_t protocolID;
	TEE_Result (* open)(TEE_iSocketHandle *ctx,
			    void *setup,
			    uint32_t *protocolError);

	TEE_Result (* close)(TEE_iSocketHandle ctx);

	TEE_Result (* send)(TEE_iSocketHandle ctx,
			    const void *buf,
			    uint32_t *length,
			    uint32_t timeout);

	TEE_Result (* recv)(TEE_iSocketHandle ctx,
			    void *buf,
			    uint32_t *length,
			    uint32_t timeout);

	uint32_t (* error)(TEE_iSocketHandle ctx);

	TEE_Result (* ioctl)(TEE_iSocketHandle ctx,
			     uint32_t commandCode,
			     const void *buf,
			     uint32_t *length);
} TEE_iSocket;

#endif
