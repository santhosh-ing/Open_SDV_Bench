/**
 * @file    bsp_button.h
 * @brief   Board support: user button B1 (PA0), debounced.
 *
 * Layer:   bsp (may call the ST HAL)
 * Spec:    001-body-ecu-bringup (FR-004)
 */
#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include <stdbool.h>

/* Sample period of bsp_button_task_10ms() and the required stable time. */
#define BSP_BUTTON_SAMPLE_PERIOD_MS 10U
#define BSP_BUTTON_STABLE_TIME_MS   50U

void bsp_button_init(void);

/* Call every 10 ms from the scheduler. */
void bsp_button_task_10ms(void);

/* True exactly once per debounced press; reading clears the event. */
bool bsp_button_pressed_event(void);

#endif /* BSP_BUTTON_H */
