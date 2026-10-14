/**
 * @file    fake_bsp_led.h
 * @brief   Host fake of bsp_led: records LED states instead of driving GPIOs.
 */
#ifndef FAKE_BSP_LED_H
#define FAKE_BSP_LED_H

#include <stdbool.h>
#include <stdint.h>

#include "bsp_led.h"

void fake_bsp_led_reset(void);
bool fake_bsp_led_is_on(bsp_led_id_t led);
uint32_t fake_bsp_led_toggle_count(bsp_led_id_t led);

#endif
