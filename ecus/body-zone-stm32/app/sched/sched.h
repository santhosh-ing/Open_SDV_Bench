/**
 * @file    sched.h
 * @brief   Cooperative scheduler with fixed 1 ms, 10 ms and 100 ms task slots.
 *
 * Layer:   sched (pure logic, no HAL — the time is passed in)
 * Spec:    001-body-ecu-bringup (FR-003, FR-006)
 * Req:     SW-ZCS-023
 *
 * How it works: the SysTick interrupt only counts milliseconds. The main loop
 * calls sched_poll(now) as often as it can. For every millisecond that has
 * passed since the last call, the scheduler runs the 1 ms tasks, every 10th
 * millisecond the 10 ms tasks and every 100th the 100 ms tasks. Because the
 * slots are counted in whole ticks (not "now + period" after each run), a late
 * main loop never shifts later periods: the scheduler catches up tick by tick
 * and no drift accumulates. Tasks run in the main loop, not in the interrupt,
 * so they may take a little time without blocking other interrupts.
 * "Cooperative" means every task must return quickly; nothing preempts it.
 */
#ifndef SCHED_H
#define SCHED_H

#include <stdint.h>

#define SCHED_SLOT_10MS_TICKS  10U
#define SCHED_SLOT_100MS_TICKS 100U

typedef void (*sched_task_fn)(void);

typedef struct
{
    const sched_task_fn *tasks_1ms;   /* may be NULL when count is 0 */
    uint8_t count_1ms;
    const sched_task_fn *tasks_10ms;
    uint8_t count_10ms;
    const sched_task_fn *tasks_100ms;
    uint8_t count_100ms;
} sched_config_t;

/* Debugger-visible counters (FR-006). volatile so the debugger always reads
 * the current value and the compiler never optimises them away. */
extern volatile uint32_t g_sched_uptime_ms;     /* ms since reset           */
extern volatile uint32_t g_sched_task100_count; /* executed 100 ms slots    */

void sched_init(const sched_config_t *config, uint32_t now_ms);

/* Run all task slots that became due up to now_ms. */
void sched_poll(uint32_t now_ms);

#endif /* SCHED_H */
