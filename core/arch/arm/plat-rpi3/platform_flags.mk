 #
 # Copyright (c) 2016, Sequitur Labs Inc. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions are met:
 #
 # Redistributions of source code must retain the above copyright notice, this
 # list of conditions and the following disclaimer.
 #
 # Redistributions in binary form must reproduce the above copyright notice,
 # this list of conditions and the following disclaimer in the documentation
 # and/or other materials provided with the distribution.
 #
 # Neither the name of ARM nor the names of its contributors may be used
 # to endorse or promote products derived from this software without specific
 # prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 # AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 # ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 # LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 # CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 # SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 # INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 # CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 # ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 # POSSIBILITY OF SUCH DAMAGE.
 #

# 32-bit flags
arm32-platform-cpuarch	:= cortex-a15
arm32-platform-cflags	+= -mcpu=$(arm32-platform-cpuarch) -mthumb
arm32-platform-cflags	+= -pipe -mthumb-interwork -mlong-calls
arm32-platform-cflags	+= -fno-short-enums -mno-apcs-float -fno-common
arm32-platform-cflags	+= -mfloat-abi=soft
arm32-platform-cflags	+= -mno-unaligned-access
arm32-platform-aflags	+= -mcpu=$(arm32-platform-cpuarch)
arm32-platform-aflags	+= -mfpu=neon

# 64-bit flags
arm64-platform-cflags	+= -mgeneral-regs-only
arm64-platform-cflags	+= -mstrict-align

platform-cflags += -ffunction-sections -fdata-sections

DEBUG		?= 1
ifeq ($(DEBUG),1)
platform-cflags += -O0
else
platform-cflags += -Os
endif

platform-cflags += -g
platform-aflags += -g

CFG_ARM32_user_ta := y
user_ta-platform-cflags += $(arm32-platform-cflags)
user_ta-platform-cflags += -fpie
user_ta-platform-cppflags += $(arm32-platform-cppflags)
user_ta-platform-aflags += $(arm32-platform-aflags)
