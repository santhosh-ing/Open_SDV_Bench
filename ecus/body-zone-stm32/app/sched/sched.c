/**
 * @file    sched.c
 * @brief   Cooperative scheduler (implementation). See sched.h.
 *
 * Layer:   sched (pure logic, no HAL)
 * Spec:    001-body-ecu-bringup (FR-003, FR-006)
 * Req:     SW-ZCS-023
 */
#include "sched.h"

#include <stdbool.h>
#include <stddef.h>

volatile uint32_t g_sched_uptime_ms = 0U;
volatile uint32_t g_sched_task100_count = 0U;

static const sched_config_t *sched_cfg = NULL;
static uint32_t last_tick_ms = 0U;  /* last millisecond already processed */
static uint32_t slot_10ms_ticks = 0U;
static uint32_t slot_100ms_ticks = 0U;

static void run_tasks(const sched_task_fn *tasks, uint8_t count)
{
    for (uint8_t i = 0U; i < count; i++)
    {
        if ((tasks != NULL) && (tasks[i] != NULL))
        {
            tasks[i]();
        }
    }
}

/* Process exactly one millisecond tick. */
static void sched_tick(void)
{
    run_tasks(sched_cfg->tasks_1ms, sched_cfg->count_1ms);

    slot_10ms_ticks++;
    if (slot_10ms_ticks >= SCHED_SLOT_10MS_TICKS)
    {
        slot_10ms_ticks = 0U;
        run_tasks(sched_cfg->tasks_10ms, sched_cfg->count_10ms);
    }

    slot_100ms_ticks++;
    if (slot_100ms_ticks >= SCHED_SLOT_100MS_TICKS)
    {
        slot_100ms_ticks = 0U;
        run_tasks(sched_cfg->tasks_100ms, sched_cfg->count_100ms);
        g_sched_task100_count++;
    }
}

void sched_init(const sched_config_t *config, uint32_t now_ms)
{
    sched_cfg = config;
    last_tick_ms = now_ms;
    slot_10ms_ticks = 0U;
    slot_100ms_ticks = 0U;
    g_sched_uptime_ms = now_ms;
    g_sched_task100_count = 0U;
}

void sched_poll(uint32_t now_ms)
{
    if (sched_cfg == NULL)
    {
        return;
    }

    /* Unsigned subtraction stays correct when the ms counter wraps. */
    while ((uint32_t)(now_ms - last_tick_ms) != 0U)
    {
        last_tick_ms++;
        g_sched_uptime_ms = last_tick_ms;
        sched_tick();
    }
}
