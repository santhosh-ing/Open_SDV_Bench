/**
 * @file    bsp_led.c
 * @brief   Board support: user LEDs on GPIOD (implementation).
 *
 * Layer:   bsp (calls the ST HAL)
 * Spec:    001-body-ecu-bringup
 * Req:     SW-ZCS-041
 *
 * The GPIO pins are configured as push-pull outputs by the CubeMX-generated
 * MX_GPIO_Init(); this module only writes them.
 */
#include "bsp_led.h"

#include <stdint.h>

#include "stm32f4xx_hal.h"

/* Index = bsp_led_id_t. All four LEDs sit on port D. */
static const uint16_t led_pin[BSP_LED_COUNT] = {
    GPIO_PIN_12, /* green  */
    GPIO_PIN_13, /* orange */
    GPIO_PIN_14, /* red    */
    GPIO_PIN_15, /* blue   */
};

void bsp_led_set(bsp_led_id_t led, bool on)
{
    if (led < BSP_LED_COUNT)
    {
        HAL_GPIO_WritePin(GPIOD, led_pin[led], on ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void bsp_led_toggle(bsp_led_id_t led)
{
    if (led < BSP_LED_COUNT)
    {
        HAL_GPIO_TogglePin(GPIOD, led_pin[led]);
    }
}

void bsp_led_set_all(bool on)
{
    for (uint32_t i = 0U; i < (uint32_t)BSP_LED_COUNT; i++)
    {
        bsp_led_set((bsp_led_id_t)i, on);
    }
}
