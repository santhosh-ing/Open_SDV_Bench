/**
 * @file    fake_bsp_led.c
 * @brief   Host fake of bsp_led (see fake_bsp_led.h).
 */
#include "fake_bsp_led.h"

static bool led_on[BSP_LED_COUNT];
static uint32_t toggles[BSP_LED_COUNT];

void fake_bsp_led_reset(void)
{
    for (uint32_t i = 0U; i < (uint32_t)BSP_LED_COUNT; i++)
    {
        led_on[i] = false;
        toggles[i] = 0U;
    }
}

bool fake_bsp_led_is_on(bsp_led_id_t led) { return led_on[led]; }
uint32_t fake_bsp_led_toggle_count(bsp_led_id_t led) { return toggles[led]; }

void bsp_led_set(bsp_led_id_t led, bool on) { led_on[led] = on; }

void bsp_led_toggle(bsp_led_id_t led)
{
    led_on[led] = !led_on[led];
    toggles[led]++;
}

void bsp_led_set_all(bool on)
{
    for (uint32_t i = 0U; i < (uint32_t)BSP_LED_COUNT; i++)
    {
        led_on[i] = on;
    }
}
