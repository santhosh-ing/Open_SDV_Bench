/**
 * @file    bsp_joystick.h
 * @brief   Board support: KY-023 joystick (turn-signal stalk), raw readings.
 *
 * Layer:   bsp (may call the ST HAL)
 * Spec:    004-joystick-indication (input: docs/spec-inputs/04-body-ecu-joystick-indication.md)
 * Req:     HW-ZCS-001, SW-ZCS-050 (sampling part)
 *
 * Wiring (docs/hardware/wiring/joystick.yml): powered from 3V, X axis on PA1
 * (ADC1_IN1), push button on PE7 (input with internal pull-up, active low).
 * Pin numbers and the ADC handle never leave this module.
 *
 * This module only delivers raw values. Classification (left / neutral /
 * right) and debouncing of the button belong to the functions layer.
 */
#ifndef BSP_JOYSTICK_H
#define BSP_JOYSTICK_H

#include <stdbool.h>
#include <stdint.h>

/* 12-bit ADC: 0 .. 4095. */
#define BSP_JOYSTICK_RAW_MAX 4095U

/* Returned by bsp_joystick_read_x() if the conversion does not finish in
 * time. Mid-scale = neutral, so a failed read never triggers indication. */
#define BSP_JOYSTICK_RAW_NEUTRAL 2048U

void bsp_joystick_init(void);

/* One ADC conversion of the X axis (polling, about 5 us). 0 .. 4095. */
uint16_t bsp_joystick_read_x(void);

/* Raw push-button level: true while pressed. Not debounced. */
bool bsp_joystick_button_raw(void);

#endif /* BSP_JOYSTICK_H */
