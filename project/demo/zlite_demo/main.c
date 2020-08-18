/*
 * Copyright (C) 2017 XRADIO TECHNOLOGY CO., LTD. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the
 *       distribution.
 *    3. Neither the name of XRADIO TECHNOLOGY CO., LTD. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "common/framework/platform_init.h"
#include <stdio.h>
#include "kernel/os/os.h"

#include "los_hwi.h"

extern void OsInterrupt(void);
extern void (*g_pstHwiForm[])(void);
extern void (*m_pstHwiSlaveForm[])(void);
extern void OsInterrupt(void);

static void hack_vtor(int vec, void (*handler)(void))
{
    g_pstHwiForm[16+vec] = OsInterrupt;
    m_pstHwiSlaveForm[16+vec] = handler;
}

static void fixup_vtor(void)
{
    extern void DMA_IRQHandler(void);
    extern void SDC0_IRQHandler(void);
    extern void UART0_IRQHandler(void);
    extern void Wakeup_Source_Handler(void);
    extern void xradio_irq_handler(void);

    hack_vtor(0,  DMA_IRQHandler);
    hack_vtor(2,  SDC0_IRQHandler);
    hack_vtor(4,  UART0_IRQHandler);
    hack_vtor(25, Wakeup_Source_Handler);
    hack_vtor(29, xradio_irq_handler);
}

int main(void)
{
    platform_init();

    fixup_vtor();

    while (1) {
        OS_Sleep(10);
        printf("Hello world! @ %u sec\n", OS_GetTicks());
    }
    return 0;
}
