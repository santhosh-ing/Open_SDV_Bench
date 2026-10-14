/**
 * @file    bsp_led.h
 * @brief   Board support: the four user LEDs of the STM32F407G-DISC1.
 *
 * Layer:   bsp (may call the ST HAL)
 * Spec:    001-body-ecu-bringup (FR-001, FR-002, FR-005)
 * Req:     SW-ZCS-041
 *
 * Callers use LED IDs only; pin numbers never leave this module.
 * Bench lamp mapping (AGENTS.md): green = left indicator, orange = low beam,
 * red = right indicator, blue = ECU heartbeat.
 */
#ifndef BSP_LED_H
#define BSP_LED_H

#include <stdbool.h>

typedef enum
{
    BSP_LED_GREEN = 0, /* LD4, PD12 */
    BSP_LED_ORANGE,    /* LD3, PD13 */
    BSP_LED_RED,       /* LD5, PD14 */
    BSP_LED_BLUE,      /* LD6, PD15 */
    BSP_LED_COUNT
} bsp_led_id_t;

void bsp_led_set(bsp_led_id_t led, bool on);
void bsp_led_toggle(bsp_led_id_t led);
void bsp_led_set_all(bool on);

#endif /* BSP_LED_H */
