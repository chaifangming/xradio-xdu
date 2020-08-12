#include "kernel/os/os_timer.h"
#include "os_util.h"

#include <los_swtmr.h>
#include <los_hwi.h>

#include <los_swtmr.ph>

#define LOS_TIMER_MAGIC         0x20

/**
 * @brief Create and initialize a timer object
 *
 * @note Creating a timer does not start the timer running. The OS_TimerStart()
 *       and OS_TimerChangePeriod() API functions can all be used to start the
 *       timer running.
 *
 * @param[in] timer Pointer to the timer object
 * @param[in] type Timer type
 * @param[in] cb Timer expire callback function
 * @param[in] arg Argument of timer expire callback function
 * @param[in] periodMS Timer period in milliseconds
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_TimerCreate(OS_Timer_t *timer, OS_TimerType type,
                         OS_TimerCallback_t cb, void *arg, uint32_t periodMS)
{
    UINT16 timerId;
    UINT32 interval = OS_MSecsToTicks(periodMS);
    UINT8  mode     = type == OS_TIMER_ONCE ? LOS_SWTMR_MODE_NO_SELFDELETE : LOS_SWTMR_MODE_PERIOD;
    UINT32 ret;

    OS_HANDLE_ASSERT(!OS_TimerIsValid(timer), timer->handle);

    ret = LOS_SwtmrCreate(interval, mode, (SWTMR_PROC_FUNC)cb, &timerId, (UINTPTR)arg);

    if (ret != LOS_OK) {
        printf ("Failed to create timer!\n");

        if (ret == LOS_ERRNO_SWTMR_MAXSIZE) {
            printf ("LOSCFG_BASE_CORE_SWTMR_LIMIT in target_config.h too small!");
        }

        return OS_FAIL;
    }

    timer->handle = (OS_TimerHandle_t)(uintptr_t)(timerId + LOS_TIMER_MAGIC);

    return OS_OK;
}

/**
 * @brief Delete the timer object
 * @param[in] timer Pointer to the timer object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_TimerDelete(OS_Timer_t *timer)
{
    UINT16 timerId = (UINT16)(UINTPTR)timer->handle - LOS_TIMER_MAGIC;

    if (LOS_SwtmrDelete(timerId) != LOS_OK) {
        return OS_FAIL;
    }

    OS_TimerSetInvalid(timer);

    return OS_OK;
}

/**
 * @brief Start a timer running.
 * @note If the timer is already running, this function will re-start the timer.
 * @param[in] timer Pointer to the timer object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_TimerStart(OS_Timer_t *timer)
{
    UINT16 timerId = (UINT16)(UINTPTR)timer->handle - LOS_TIMER_MAGIC;

    return LOS_SwtmrStart(timerId) == LOS_OK ? OS_OK : OS_FAIL;
}

/**
 * @brief Change the period of a timer
 *
 * If OS_TimerChangePeriod() is used to change the period of a timer that is
 * already running, then the timer will use the new period value to recalculate
 * its expiry time. The recalculated expiry time will then be relative to when
 * OS_TimerChangePeriod() was called, and not relative to when the timer was
 * originally started.

 * If OS_TimerChangePeriod() is used to change the period of a timer that is
 * not already running, then the timer will use the new period value to
 * calculate an expiry time, and the timer will start running.
 *
 * @param[in] timer Pointer to the timer object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_TimerChangePeriod(OS_Timer_t *timer, uint32_t periodMS)
{
    UINT16 timerId  = (UINT16)(UINTPTR)timer->handle - LOS_TIMER_MAGIC;

    if (m_pstSwtmrCBArray[timerId].ucState == OS_SWTMR_STATUS_TICKING) {
        UINTPTR flags = LOS_IntLock();
        UINT32  ret;

        ret = LOS_SwtmrStop(timerId);

        LOS_IntRestore(flags);

        if (ret != LOS_OK) {
            return OS_FAIL;
        }
    }

    m_pstSwtmrCBArray[timerId].uwInterval = OS_MSecsToTicks(periodMS);

    return LOS_SwtmrStart(timerId) == LOS_OK ? OS_OK : OS_FAIL;
}

/**
 * @brief Stop a timer running.
 * @param[in] timer Pointer to the timer object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_TimerStop(OS_Timer_t *timer)
{
    UINT16 timerId = (UINT16)(UINTPTR)timer->handle - LOS_TIMER_MAGIC;

    return LOS_SwtmrStop(timerId) == LOS_OK ? OS_OK : OS_FAIL;
}

int OS_TimerIsActive(OS_Timer_t *timer)
{
    UINT16 timerId = (UINT16)(UINTPTR)timer->handle - LOS_TIMER_MAGIC;

    return m_pstSwtmrCBArray[timerId].ucState == OS_SWTMR_STATUS_TICKING;
}