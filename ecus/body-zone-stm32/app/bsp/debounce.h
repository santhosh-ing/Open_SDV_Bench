/**
 * @file    debounce.h
 * @brief   Switch debouncing: turns a bouncing raw input into clean press events.
 *
 * Layer:   bsp (pure logic, no HAL — so it can be unit-tested on a PC)
 * Spec:    001-body-ecu-bringup (FR-004)
 *
 * Why debounce? A mechanical contact does not close cleanly: for a few
 * milliseconds it bounces between open and closed. Reading it once would
 * report several presses. Here the input is sampled at a fixed period and
 * only accepted as "pressed" (or "released") after it has kept the same
 * level for N consecutive samples. With a 10 ms sample period and N = 5
 * that is 50 ms of stable level.
 */
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t threshold;   /* samples of stable level needed to accept a change */
    uint8_t count;       /* consecutive samples differing from stable_level  */
    bool stable_level;   /* debounced level: true = pressed                  */
    bool press_event;    /* set once per debounced press, cleared when read  */
} debounce_t;

/* initial_level: the raw level at start-up. If the button is already held at
 * reset, it starts as "pressed" and no event is reported until it is released
 * and pressed again. */
void debounce_init(debounce_t *db, uint8_t threshold, bool initial_level);

/* Feed one raw sample; call at a fixed period. */
void debounce_sample(debounce_t *db, bool raw_pressed);

/* Returns true once per debounced press, then clears the event. */
bool debounce_take_press_event(debounce_t *db);

bool debounce_is_pressed(const debounce_t *db);

#endif /* DEBOUNCE_H */
