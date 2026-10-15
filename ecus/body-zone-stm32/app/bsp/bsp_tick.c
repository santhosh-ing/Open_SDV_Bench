/**
 * @file    bsp_tick.c
 * @brief   Board support: millisecond time base (implementation).
 *
 * Layer:   bsp (calls the ST HAL)
 * Spec:    001-body-ecu-bringup (FR-003)
 * Req:     SW-ZCS-023
 *
 * The ST HAL already runs SysTick as a 1 ms interrupt (HAL_Init) and counts
 * it in HAL_IncTick(). We only read that counter, so no generated interrupt
 * code has to change.
 */
#include "bsp_tick.h"

#include "stm32f4xx_hal.h"

uint32_t bsp_tick_ms(void)
{
    return HAL_GetTick();
}
