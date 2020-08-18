/**
 * @defgroup los_config System configuration items
 * @ingroup kernel
 */

#ifndef _TARGET_CONFIG_H
#define _TARGET_CONFIG_H

#include "los_typedef.h"
#include <driver/chip/chip.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOSCFG_CORTEX_M4                                    YES

/*=============================================================================
                                        System clock module configuration
=============================================================================*/

extern uint32_t SystemCoreClock;

/**
 * @ingroup los_config
 * System clock (unit: HZ)
 */
#define OS_SYS_CLOCK                                        (SystemCoreClock)

/**
 * @ingroup los_config
 * Number of Ticks in one second
 */
#define LOSCFG_BASE_CORE_TICK_PER_SECOND                    (1000UL)

/**
 * @ingroup los_config
 * External configuration item for timer tailoring
 */
#define LOSCFG_BASE_CORE_TICK_HW_TIME                       NO

/**
 * @ingroup los_config
 * Configuration liteos kernel tickless
 */
#define LOSCFG_KERNEL_TICKLESS                              NO

/*=============================================================================
                                        Hardware interrupt module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for hardware interrupt tailoring
 */
#define LOSCFG_PLATFORM_HWI                                 YES

/**
 * @ingroup los_config
 * Maximum number of used hardware interrupts, including Tick timer interrupts.
 */
#define LOSCFG_PLATFORM_HWI_LIMIT                           48

#define OS_HWI_WITH_ARG                                     NO


/*=============================================================================
                                       Task module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Default task priority
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO                   10

/**
 * @ingroup los_config
 * Maximum supported number of tasks except the idle task rather than the number of usable tasks
 */
#define LOSCFG_BASE_CORE_TSK_LIMIT                          31              // max num task

/**
 * @ingroup los_config
 * Size of the idle task stack
 */
#define LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE                (0x180U)        // IDLE task stack

/**
 * @ingroup los_config
 * Default task stack size
 */
#define LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE             (0x300U)        // default stack

/**
 * @ingroup los_config
 * Minimum stack size.
 */
#define LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE                 (0x180U)

/**
 * @ingroup los_config
 * Configuration item for task Robin tailoring
 */
#define LOSCFG_BASE_CORE_TIMESLICE                          YES

/**
 * @ingroup los_config
 * Longest execution time of tasks with the same priorities
 */
#define LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT                  10

/**
 * @ingroup los_config
 * Configuration item for task (stack) monitoring module tailoring
 */
#define LOSCFG_BASE_CORE_TSK_MONITOR                        YES

/**
 * @ingroup los_config
 * Configuration item for task perf task filter hook
 */
#define LOSCFG_BASE_CORE_EXC_TSK_SWITCH                     NO

/**
 * @ingroup los_config
 * Configuration item for performance moniter unit
 */
#define OS_INCLUDE_PERF                                     NO

/**
 * @ingroup los_config
 * Define a usable task priority.Highest task priority.
 */
#define LOS_TASK_PRIORITY_HIGHEST                           0

/**
 * @ingroup los_config
 * Define a usable task priority.Lowest task priority.
 */
#define LOS_TASK_PRIORITY_LOWEST                            31


/*=============================================================================
                                       Semaphore module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for semaphore module tailoring
 */
#define LOSCFG_BASE_IPC_SEM                                 YES

/**
 * @ingroup los_config
 * Maximum supported number of semaphores
 */
#define LOSCFG_BASE_IPC_SEM_LIMIT                           63              // the max sem-numb


/*=============================================================================
                                       Mutex module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for mutex module tailoring
 */
#define LOSCFG_BASE_IPC_MUX                                 YES

/**
 * @ingroup los_config
 * Maximum supported number of mutexes
 */
#define LOSCFG_BASE_IPC_MUX_LIMIT                           32              // the max mutex-num

/*=============================================================================
                                       Queue module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for queue module tailoring
 */
#define LOSCFG_BASE_IPC_QUEUE                               YES

/**
 * @ingroup los_config
 * Maximum supported number of queues rather than the number of usable queues
 */
#define LOSCFG_BASE_IPC_QUEUE_LIMIT                         16              //the max queue-numb


/*=============================================================================
                                       Software timer module configuration
=============================================================================*/

#if (LOSCFG_BASE_IPC_QUEUE == YES)
/**
 * @ingroup los_config
 * Configuration item for software timer module tailoring
 */
#define LOSCFG_BASE_CORE_SWTMR                              YES

#define LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE               LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE

#define LOSCFG_BASE_CORE_SWTMR_TASK                         YES

#define LOSCFG_BASE_CORE_SWTMR_ALIGN                        NO
#if(LOSCFG_BASE_CORE_SWTMR == NO && LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    #error "swtmr align first need support swmtr, should make LOSCFG_BASE_CORE_SWTMR = YES"
#endif

/**
 * @ingroup los_config
 * Maximum supported number of software timers rather than the number of usable software timers
 */
#define LOSCFG_BASE_CORE_SWTMR_LIMIT                        32             // the max SWTMR numb

/**
 * @ingroup los_config
 * Max number of software timers ID
 */
#define OS_SWTMR_MAX_TIMERID                                ((65535/LOSCFG_BASE_CORE_SWTMR_LIMIT) * LOSCFG_BASE_CORE_SWTMR_LIMIT)

/**
 * @ingroup los_config
 * Maximum size of a software timer queue
 */
#define OS_SWTMR_HANDLE_QUEUE_SIZE                          (LOSCFG_BASE_CORE_SWTMR_LIMIT + 0)

/**
 * @ingroup los_config
 * Minimum divisor of software timer multiple alignment
 */
#define LOS_COMMON_DIVISOR                                  10
#endif


/*=============================================================================
                                       Memory module configuration
=============================================================================*/

extern unsigned char __end__[];
extern unsigned char __StackTop[];

#define OS_SYS_MEM_ADDR                                     (__end__)

#define OS_SYS_MEM_SIZE                                     (__StackTop - 0x1000 - OS_SYS_MEM_ADDR)

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node integrity checking
 */
#define LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK                YES

/**
 * @ingroup los_config
 * Configuration module tailoring of mem node size checking
 */
#define LOSCFG_BASE_MEM_NODE_SIZE_CHECK                     YES

#define LOSCFG_MEMORY_BESTFIT                               YES

/**
 * @ingroup los_config
 * Configuration module tailoring of more mempry pool checking
 */
#define LOSCFG_MEM_MUL_POOL                                 NO

/**
 * @ingroup los_config
 * Configuration module tailoring of slab memory
 */
#define LOSCFG_KERNEL_MEM_SLAB                              YES

/*=============================================================================
                                        Exception module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for exception tailoring
 */
#define LOSCFG_PLATFORM_EXC                                 NO


/*=============================================================================
                                       Runstop module configuration
=============================================================================*/

/**
 * @ingroup los_config
 * Configuration item for runstop module tailoring
 */
#define LOSCFG_KERNEL_RUNSTOP                               NO

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* _TARGET_CONFIG_H */
