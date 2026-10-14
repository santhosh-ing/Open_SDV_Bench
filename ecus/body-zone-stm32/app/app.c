/**
 * @file    app.c
 * @brief   Application wiring: which task runs in which scheduler slot.
 *
 * Layer:   app (top; wires the layers together, no HAL calls)
 * Spec:    001-body-ecu-bringup (FR-003, FR-008)
 * Req:     SW-ZCS-023, SW-ZCS-040, SW-ZCS-041
 */
#include "app.h"

#include <stddef.h>

#include "bsp_button.h"
#include "bsp_tick.h"
#include "ecu_state.h"
#include "sched.h"

/* 10 ms slot: read the button first, then let the ECU logic use the result. */
static void task_ecu_10ms(void)
{
    bsp_button_task_10ms();
    ecu_state_task_10ms(bsp_button_pressed_event());
}

static const sched_task_fn tasks_10ms[] = { task_ecu_10ms };

/* The 1 ms and 100 ms slots exist (FR-003) but have no tasks yet;
 * CAN communication (spec 002) will use them. */
static const sched_config_t sched_config = {
    .tasks_1ms = NULL,
    .count_1ms = 0U,
    .tasks_10ms = tasks_10ms,
    .count_10ms = (uint8_t)(sizeof(tasks_10ms) / sizeof(tasks_10ms[0])),
    .tasks_100ms = NULL,
    .count_100ms = 0U,
};

void app_init(void)
{
    bsp_button_init();
    ecu_state_init();
    sched_init(&sched_config, bsp_tick_ms());
}

void app_run(void)
{
    sched_poll(bsp_tick_ms());
}
