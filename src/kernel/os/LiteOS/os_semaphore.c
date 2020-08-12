#include "kernel/os/os_semaphore.h"
#include "os_util.h"

#include <los_sem.h>

#define LOS_SEM_MAGIC                           0x20

extern UINT32 osSemCreate (UINT16 usCount, UINT16 usMaxCount, UINT32 *puwSemHandle);

/**
 * @brief Create and initialize a counting semaphore object
 * @param[in] sem Pointer to the semaphore object
 * @param[in] initCount The count value assigned to the semaphore when it is
 *                      created.
 * @param[in] maxCount The maximum count value that can be reached. When the
 *                     semaphore reaches this value it can no longer be
 *                     released.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_SemaphoreCreate(OS_Semaphore_t *sem, uint32_t initCount, uint32_t maxCount)
{
    UINT32 semId;
    UINT32 ret;

    if (initCount > UINT16_MAX) {
        initCount = UINT16_MAX;
    }

    if (maxCount > UINT16_MAX) {
        maxCount = UINT16_MAX;
    }

    ret = osSemCreate((UINT16)initCount, (UINT16)maxCount, &semId);

    if (ret != LOS_OK) {
        printf("Fail to create semaphore!\n");

        if (ret == LOS_ERRNO_SEM_ALL_BUSY) {
            printf("LOSCFG_BASE_IPC_SEM_LIMIT in target_config.h too small!");
        }

        return OS_FAIL;
    }

    sem->handle = (OS_SemaphoreHandle_t)(uintptr_t)(semId + LOS_SEM_MAGIC);

    return OS_OK;
}

/**
 * @brief Create and initialize a binary semaphore object
 * @note A binary semaphore is equal to a counting semaphore created by calling
         OS_SemaphoreCreate(sem, 0, 1).
 * @param[in] sem Pointer to the semaphore object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_SemaphoreCreateBinary(OS_Semaphore_t *sem)
{
    return OS_SemaphoreCreate(sem, 1, 1);
}

/**
 * @brief Delete the semaphore object
 * @param[in] sem Pointer to the semaphore object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_SemaphoreDelete(OS_Semaphore_t *sem)
{
    UINT32 semId = (UINT32)sem->handle - LOS_SEM_MAGIC;

    if (LOS_SemDelete(semId) != LOS_OK) {
        return OS_FAIL;
    }

    OS_SemaphoreSetInvalid(sem);

    return OS_OK;
}

/**
 * @brief Wait until the semaphore object becomes available
 * @param[in] sem Pointer to the semaphore object
 * @param[in] waitMS The maximum amount of time (in millisecond) the thread
 *                   should remain in the blocked state to wait for the
 *                   semaphore to become available.
 *                   OS_WAIT_FOREVER for waiting forever, zero for no waiting.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_SemaphoreWait(OS_Semaphore_t *sem, OS_Time_t waitMS)
{
    UINT32 semId = (UINT32)sem->handle - LOS_SEM_MAGIC;

    if (OS_IsISRContext()) {
        waitMS = 0;
    }

    return LOS_SemPend(semId, OS_CalcWaitTicks(waitMS)) == LOS_OK ? OS_OK : OS_FAIL;
}

/**
 * @brief Release the semaphore object
 * @param[in] sem Pointer to the semaphore object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_SemaphoreRelease(OS_Semaphore_t *sem)
{
    UINT32 semId = (UINT32)sem->handle - LOS_SEM_MAGIC;

    return LOS_SemPost(semId) == LOS_OK ? OS_OK : OS_FAIL;
}
