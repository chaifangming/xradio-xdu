#include "errno.h"
#include "los_errno.h"
#include "los_task.h"
#include <los_config.h>

int errno_array[LOSCFG_BASE_CORE_TSK_LIMIT + 1];

/* the specific errno get or set in interrupt service routine */
static int errno_isr;

static int *__errno_location(void) {
  if (OS_INT_INACTIVE)
    return &errno_array[LOS_CurTaskIDGet()];
  else
    return &errno_isr;
}

int OS_GetErrno(void)
{
	return *__errno_location();
}

void OS_SetErrno(int err)
{
	*__errno_location() = err;
}
