#ifndef _OS_UTIL_H_
#define _OS_UTIL_H_

#include <string.h>
#include <stdlib.h>
#include "driver/chip/hal_cmsis.h"
#include "kernel/os/os_time.h"
#include "os_debug.h"

#include <los_memory.h>

/* check if in ISR context or not */
static int OS_IsISRContext(void)
{
    return __get_IPSR();
}

static UINT32 OS_CalcWaitTicks(OS_Time_t msec)
{
    if ((msec == OS_WAIT_FOREVER) || (msec == 0))
    {
        return (UINT32) msec;
    }

    return OS_MSecsToTicks(msec);
}

/* memory */
#define OS_Malloc(l)        LOS_MemAlloc(m_aucSysMem0, l)
#define OS_Free(p)          free(m_aucSysMem0, p)
#define OS_Memcpy(d, s, l)  memcpy(d, s, l)
#define OS_Memset(d, c, l)  memset(d, c, l)
#define OS_Memcmp(a, b, l)  memcmp(a, b, l)
#define OS_Memmove(d, s, n) memmove(d, s, n)

#endif /* _OS_UTIL_H_ */
