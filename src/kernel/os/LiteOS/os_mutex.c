#include <stdint.h>

#include <kernel/os/os_mutex.h>
#include "os_util.h"

#include <los_mux.h>

#include <los_mux.ph>
#include <los_task.ph>

#define LOS_MUX_MAGIC                        0x20

/**
 * @brief Create and initialize a mutex object
 * @note A mutex can only be locked by a single thread at any given time.
 * @param[in] mutex Pointer to the mutex object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_MutexCreate(OS_Mutex_t *mutex)
{
    UINT32 muxId;
    UINT32 ret;

    ret = LOS_MuxCreate(&muxId);

    if (ret != LOS_OK) {
        printf("Failed to create mux!\n");

        if (ret == LOS_ERRNO_MUX_ALL_BUSY) {
            printf("LOSCFG_BASE_IPC_MUX_LIMIT in target_config.h too small!");
        }

        return OS_FAIL;
    }

    mutex->handle = (OS_MutexHandle_t)(uintptr_t)(muxId + LOS_MUX_MAGIC);

    return OS_OK;
}

/**
 * @brief Delete the mutex object
 * @param[in] mutex Pointer to the mutex object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_MutexDelete(OS_Mutex_t *mutex)
{
    UINT32 muxId = (UINT32)mutex->handle - LOS_MUX_MAGIC;

    if (LOS_MuxDelete(muxId) != LOS_OK) {
        return OS_FAIL;
    }

    OS_MutexSetInvalid(mutex);

    return OS_OK;
}

/**
 * @brief Lock the mutex object
 * @note A mutex can only be locked by a single thread at any given time. If
 *       the mutex is already locked, the caller will be blocked for the
 *       specified time duration.
 * @param[in] mutex Pointer to the mutex object
 * @param[in] waitMS The maximum amount of time (in millisecond) the thread
 *                   should remain in the blocked state to wait for the mutex
 *                   to become unlocked.
 *                   OS_WAIT_FOREVER for waiting forever, zero for no waiting.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_MutexLock(OS_Mutex_t *mutex, OS_Time_t waitMS)
{
    UINT32 muxId = (UINT32)mutex->handle - LOS_MUX_MAGIC;
    UINT32 ret;

    ret = LOS_MuxPend(muxId, OS_CalcWaitTicks(waitMS));

    return ret == LOS_OK ? OS_OK : OS_FAIL;
}

/**
 * @brief Unlock the mutex object previously locked using OS_MutexLock()
 * @note The mutex should be unlocked from the same thread context from which
 *       it was locked.
 * @param[in] mutex Pointer to the mutex object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_MutexUnlock(OS_Mutex_t *mutex)
{
    UINT32 muxId = (UINT32)mutex->handle - LOS_MUX_MAGIC;

    return LOS_MuxPost(muxId) == LOS_OK ? OS_OK : OS_FAIL;
}

/**
 * @brief Create and initialize a recursive mutex object
 * @note A recursive mutex can be locked repeatedly by one single thread.
 *       The mutex doesn't become available again until the owner has called
 *       OS_RecursiveMutexUnlock() for each successful OS_RecursiveMutexLock().
 * @param[in] mutex Pointer to the recursive mutex object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_RecursiveMutexCreate(OS_Mutex_t *mutex)
{
    return OS_MutexCreate(mutex);
}

OS_Status OS_RecursiveMutexDelete(OS_Mutex_t *mutex)
{
    return OS_MutexDelete(mutex);
}

/**
 * @brief Lock the recursive mutex object
 * @note A recursive mutex can be locked repeatedly by one single thread.
 *       If the recursive mutex is already locked by other thread, the caller
 *       will be blocked for the specified time duration.
 * @param[in] mutex Pointer to the recursive mutex object
 * @param[in] waitMS The maximum amount of time (in millisecond) the thread
 *                   should remain in the blocked state to wait for the
 *                   recursive mutex to become unlocked.
 *                   OS_WAIT_FOREVER for waiting forever, zero for no waiting.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_RecursiveMutexLock(OS_Mutex_t *mutex, OS_Time_t waitMS)
{
    return OS_MutexLock (mutex, waitMS);
}

/**
 * @brief Unlock the recursive mutex object previously locked using
 *        OS_RecursiveMutexLock()
 * @note The recursive mutex should be unlocked from the same thread context
 *       from which it was locked.
 * @param[in] mutex Pointer to the mutex object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_RecursiveMutexUnlock(OS_Mutex_t *mutex)
{
    return OS_MutexUnlock(mutex);
}

OS_ThreadHandle_t OS_MutexGetOwner(OS_Mutex_t *mutex)
{
    UINT32 muxId = (UINT32)mutex->handle - LOS_MUX_MAGIC;

    if (!OS_MutexIsValid(mutex)) {
        return OS_INVALID_HANDLE;
    }

    return (OS_ThreadHandle_t)(uintptr_t)g_pstAllMux[muxId].pstOwner->uwTaskID;
}
