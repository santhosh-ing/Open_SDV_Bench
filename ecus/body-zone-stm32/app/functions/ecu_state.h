/**
 * @file    ecu_state.h
 * @brief   ECU state handling: start-up lamp test, heartbeat, bring-up button demo.
 *
 * Layer:   functions (pure logic, no HAL; uses bsp interfaces only)
 * Spec:    001-body-ecu-bringup (FR-001, FR-002, FR-005, FR-007)
 * Req:     SW-ZCS-022 (Normal state part), SW-ZCS-023
 *
 * States: Init (start-up and lamp test) -> Normal. Degraded and Fault are
 * added by spec 002 (SW-ZCS-020, SW-ZCS-021).
 */
#ifndef ECU_STATE_H
#define ECU_STATE_H

#include <stdbool.h>
#include <stdint.h>

#define ECU_STATE_TASK_PERIOD_MS     10U
#define ECU_LAMP_TEST_DURATION_MS    1000U
#define ECU_HEARTBEAT_HALF_PERIOD_MS 500U  /* 1 Hz, 50 % duty cycle */

typedef enum
{
    ECU_STATE_INIT = 0,
    ECU_STATE_NORMAL = 1
} ecu_state_t;

/* Debugger-visible current state (FR-007). */
extern volatile ecu_state_t g_ecu_state;

/* Enter Init and start the lamp test (all LEDs on). */
void ecu_state_init(void);

/* Call every 10 ms. button_pressed: one debounced press event since the
 * last call. */
void ecu_state_task_10ms(bool button_pressed);

ecu_state_t ecu_state_get(void);

#endif /* ECU_STATE_H */
