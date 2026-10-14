/**
 * @file    ecu_state.c
 * @brief   ECU state handling (implementation). See ecu_state.h.
 *
 * Layer:   functions (pure logic, no HAL)
 * Spec:    001-body-ecu-bringup (FR-001, FR-002, FR-005, FR-007)
 * Req:     SW-ZCS-022 (Normal state part), SW-ZCS-023
 *
 * Timing is counted in 10 ms task cycles, so it is exact to the tick and
 * needs no clock reads here.
 */
#include "ecu_state.h"

#include "bsp_led.h"

#define LAMP_TEST_CYCLES \
    (ECU_LAMP_TEST_DURATION_MS / ECU_STATE_TASK_PERIOD_MS)        /* 100 */
#define HEARTBEAT_HALF_CYCLES \
    (ECU_HEARTBEAT_HALF_PERIOD_MS / ECU_STATE_TASK_PERIOD_MS)     /* 50 */

#define HEARTBEAT_LED BSP_LED_BLUE
#define DEMO_LED      BSP_LED_ORANGE

volatile ecu_state_t g_ecu_state = ECU_STATE_INIT;

static uint32_t cycle_count;   /* 10 ms cycles in the current state/phase */
static bool heartbeat_on;

static void enter_normal(void)
{
    g_ecu_state = ECU_STATE_NORMAL;
    cycle_count = 0U;
    bsp_led_set_all(false);          /* end of lamp test */
    heartbeat_on = true;             /* heartbeat starts with an "on" phase */
    bsp_led_set(HEARTBEAT_LED, true);
}

void ecu_state_init(void)
{
    g_ecu_state = ECU_STATE_INIT;
    cycle_count = 0U;
    heartbeat_on = false;
    bsp_led_set_all(true);           /* lamp test: every LED on */
}

void ecu_state_task_10ms(bool button_pressed)
{
    cycle_count++;

    switch (g_ecu_state)
    {
    case ECU_STATE_INIT:
        /* Button presses during the lamp test are ignored (spec edge case). */
        if (cycle_count >= LAMP_TEST_CYCLES)
        {
            enter_normal();
        }
        break;

    case ECU_STATE_NORMAL:
        if (cycle_count >= HEARTBEAT_HALF_CYCLES)
        {
            cycle_count = 0U;
            heartbeat_on = !heartbeat_on;
            bsp_led_set(HEARTBEAT_LED, heartbeat_on);
        }
        if (button_pressed)
        {
            bsp_led_toggle(DEMO_LED);   /* FR-005, bring-up demo only */
        }
        break;

    default:
        /* Unknown state value (e.g. memory corruption): restart safely. */
        ecu_state_init();
        break;
    }
}

ecu_state_t ecu_state_get(void)
{
    return g_ecu_state;
}
