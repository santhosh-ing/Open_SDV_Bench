/**
 * @file    bsp_button.c
 * @brief   Board support: user button B1 on PA0 (implementation).
 *
 * Layer:   bsp (calls the ST HAL)
 * Spec:    001-body-ecu-bringup (FR-004)
 *
 * B1 is active high: the board has an external pull-down, the pin reads 1
 * while the button is pressed (UM1472). PA0 is configured as input by the
 * CubeMX-generated MX_GPIO_Init().
 */
#include "bsp_button.h"

#include <stdint.h>

#include "debounce.h"
#include "stm32f4xx_hal.h"

#define BUTTON_DEBOUNCE_SAMPLES \
    ((uint8_t)(BSP_BUTTON_STABLE_TIME_MS / BSP_BUTTON_SAMPLE_PERIOD_MS))

static debounce_t button_db;

static bool button_read_raw(void)
{
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET;
}

void bsp_button_init(void)
{
    debounce_init(&button_db, BUTTON_DEBOUNCE_SAMPLES, button_read_raw());
}

void bsp_button_task_10ms(void)
{
    debounce_sample(&button_db, button_read_raw());
}

bool bsp_button_pressed_event(void)
{
    return debounce_take_press_event(&button_db);
}
