#include "kernel/os/os_queue.h"
#include "os_util.h"

#include <los_queue.h>
#include <los_queue.ph>

#define LOS_QUEUE_MAGIC                         0x20

/**
 * @brief Create and initialize a queue object
 * @param[in] queue Pointer to the queue object
 * @param[in] queueLen The maximum number of items that the queue can hold at
 *                     any one time.
 * @param[in] itemSize The size, in bytes, of each data item that can be stored
 *                     in the queue.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_QueueCreate(OS_Queue_t *queue, uint32_t queueLen, uint32_t itemSize)
{
    UINT32 queueId;
    UINT32 ret;

    if ((queueLen > UINT16_MAX) || (itemSize > UINT16_MAX)) {
        printf ("queueLen or itemSize too large!\n");
        return OS_FAIL;
    }

    ret = LOS_QueueCreate("", (UINT16)queueLen, &queueId, 0, (UINT16)itemSize);

    if (ret != LOS_OK) {
        printf("Failed to create queue!\n");

        if (ret == LOS_ERRNO_QUEUE_CB_UNAVAILABLE) {
            printf("LOSCFG_BASE_IPC_QUEUE_LIMIT in target_config.h too small!");
        }

        return OS_FAIL;
    }

    queue->handle = (OS_QueueHandle_t)(uintptr_t)(queueId + LOS_QUEUE_MAGIC);

    return OS_OK;
}

/**
 * @brief Delete the queue object
 * @param[in] queue Pointer to the queue object
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_QueueDelete(OS_Queue_t *queue)
{
    UINT32 queueId = (UINT32)queue->handle - LOS_QUEUE_MAGIC;

    if (LOS_QueueDelete(queueId) != LOS_OK) {
        return OS_FAIL;
    }

    OS_QueueSetInvalid(queue);

    return OS_OK;
}

/**
 * @brief Send (write) an item to the back of the queue
 * @param[in] queue Pointer to the queue object
 * @param[in] item Pointer to the data to be copied into the queue.
 *                 The size of each item the queue can hold is set when the
 *                 queue is created, and that many bytes will be copied from
 *                 item into the queue storage area.
 * @param[in] waitMS The maximum amount of time the thread should remain in the
 *                   blocked state to wait for space to become available on the
 *                   queue, should the queue already be full.
 *                   OS_WAIT_FOREVER for waiting forever, zero for no waiting.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_QueueSend(OS_Queue_t *queue, const void *item, OS_Time_t waitMS)
{
    UINT32 queueId = (UINT32)queue->handle - LOS_QUEUE_MAGIC;
    UINT32 size    = g_pstAllQueue[queueId].usQueueSize - sizeof(UINT32);

    if (LOS_QueueWriteCopy(queueId, (void *)item, size, OS_CalcWaitTicks(waitMS)) != LOS_OK) {
        return OS_FAIL;
    }

    return OS_OK;
}

/**
 * @brief Receive (read) an item from the queue
 * @param[in] queue Pointer to the queue object
 * @param[in] item Pointer to the memory into which the received data will be
 *                 copied. The length of the buffer must be at least equal to
 *                 the queue item size which is set when the queue is created.
 * @param[in] waitMS The maximum amount of time the thread should remain in the
 *                   blocked state to wait for data to become available on the
 *                   queue, should the queue already be empty.
 *                   OS_WAIT_FOREVER for waiting forever, zero for no waiting.
 * @retval OS_Status, OS_OK on success
 */
OS_Status OS_QueueReceive(OS_Queue_t *queue, void *item, OS_Time_t waitMS)
{
    UINT32 queueId = (UINT32)queue->handle - LOS_QUEUE_MAGIC;
    UINT32 size    = g_pstAllQueue[queueId].usQueueSize - sizeof(UINT32);

    if (LOS_QueueReadCopy(queueId, item, &size, OS_CalcWaitTicks(waitMS)) != LOS_OK) {
        return OS_FAIL;
    }

    return OS_OK;
}
