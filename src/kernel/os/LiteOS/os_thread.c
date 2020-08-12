#include <kernel/os/os_thread.h>
#include "os_util.h"

#include <los_task.h>
#include <los_task.ph>

/**
 * @brief Create and start a thread
 *
 * This function starts a new thread. The new thread starts execution by
 * invoking entry(). The argument arg is passed as the sole argument of entry().
 *
 * @note After finishing execution, the new thread should call OS_ThreadDelete()
 *       to delete itself. Failing to do this and just returning from entry()
 *       will result in undefined behavior.
 *
 * @param[in] thread Pointer to the thread object
 * @param[in] name A descriptive name for the thread. This is mainly used to
 *                 facilitate debugging.
 * @param[in] entry Entry, which is a function pointer, to the thread function
 * @param[in] arg The sole argument passed to entry()
 * @param[in] priority The priority at which the thread will execute
 * @param[in] stackSize The number of bytes the thread stack can hold
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_ThreadCreate(OS_Thread_t *thread, const char *name,
                          OS_ThreadEntry_t entry, void *arg,
                          OS_Priority priority, uint32_t stackSize)
{
    TSK_INIT_PARAM_S param;
    UINT32           taskId;
    UINT32           ret;

    if (priority >= 31) {
        return OS_FAIL;
    }

    OS_HANDLE_ASSERT(!OS_ThreadIsValid(thread), thread->handle);

    param.pfnTaskEntry = (TSK_ENTRY_FUNC)entry;
    param.usTaskPrio   = 31 - priority;
    param.uwArg        = (UINT32)arg;
    param.uwStackSize  = stackSize;
    param.pcName       = (char *)name;

    ret = LOS_TaskCreate(&taskId, &param);

    if (ret != LOS_OK) {
        printf("Failed to create thread!\n");

        if (ret == LOS_ERRNO_TSK_TCB_UNAVAILABLE) {
            printf("LOSCFG_BASE_CORE_TSK_LIMIT in target_config.h too small!");
        }

        return OS_FAIL;
    }

    thread->handle = (OS_ThreadHandle_t)(uintptr_t)taskId;

    return OS_OK;
}

/**
 * @brief Terminate the thread
 * @note Only memory that is allocated to a thread by the kernel itself is
 *       automatically freed when a thread is deleted. Memory, or any other
 *       resource, that the application (rather than the kernel) allocates
 *       to a thread must be explicitly freed by the application when the task
 *       is deleted.
 * @param[in] thread Pointer to the thread object to be deleted.
 *                   A thread can delete itself by passing NULL in place of a
 *                   valid thread object.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_ThreadDelete(OS_Thread_t *thread)
{
    UINT32 taskId = (UINT32)thread->handle;

    OS_HANDLE_ASSERT(OS_ThreadIsValid(thread), thread->handle);

    if (LOS_TaskDelete(taskId) != LOS_OK) {
        return OS_FAIL;
    }

    OS_ThreadSetInvalid(thread);

    return OS_OK;
}

void OS_ThreadSleep(OS_Time_t msec)
{
    UINT32 ticks = OS_CalcWaitTicks(msec);

    LOS_TaskDelay(ticks);
}

void OS_ThreadYield(void)
{
    LOS_TaskYield();
}

OS_ThreadHandle_t OS_ThreadGetCurrentHandle(void)
{
    return (OS_ThreadHandle_t)LOS_CurTaskIDGet();
}

void OS_ThreadStartScheduler(void)
{
    (void)LOS_Start();
}

void OS_ThreadSuspendScheduler(void)
{
    LOS_TaskLock();
}

void OS_ThreadResumeScheduler(void)
{
    LOS_TaskUnlock();
}

int OS_ThreadIsSchedulerRunning(void)
{
    return g_usLosTaskLock == 0;
}

#if INCLUDE_uxTaskGetStackHighWaterMark
/**
 * @brief Get the minimum amount of free stack space that has been available
 *        since the thread started executing.
 * @param[in] thread Pointer to the thread object
 * @return The minimum amount of free stack space that has been available since
 *         the thread started executing. This is the amount of stack that
 *         remained unused when stack usage was at its greatest (or deepest)
 *         value.
 */
uint32_t OS_ThreadGetStackMinFreeSize(OS_Thread_t *thread)
{
    UINT32 taskId = (UINT32)thread->handle;

    return osGetTaskWaterLine(taskId);
}
#endif

void OS_ThreadList(void)
{
    return; //only shown by shell cmd
}

void OS_ThreadStackOverflowHook(OS_ThreadHandle_t thread, char *pcTaskName)
{
    printf("%s [id = %d] stack overflow!\n", pcTaskName, (UINT32)thread);
}

