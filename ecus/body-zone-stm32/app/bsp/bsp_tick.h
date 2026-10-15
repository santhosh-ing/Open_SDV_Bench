/**
 * @file    bsp_tick.h
 * @brief   Board support: millisecond time base.
 *
 * Layer:   bsp (may call the ST HAL)
 * Spec:    001-body-ecu-bringup (FR-003)
 * Req:     SW-ZCS-023
 */
#ifndef BSP_TICK_H
#define BSP_TICK_H

#include <stdint.h>

/* Milliseconds since reset. Wraps after about 49.7 days; callers must use
 * unsigned subtraction (now - then) for elapsed time. */
uint32_t bsp_tick_ms(void);

#endif /* BSP_TICK_H */
