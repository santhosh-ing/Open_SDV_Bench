/**
 * @file    debounce.c
 * @brief   Switch debouncing (implementation). See debounce.h.
 *
 * Layer:   bsp (pure logic, no HAL)
 * Spec:    001-body-ecu-bringup (FR-004)
 */
#include "debounce.h"

#include <stddef.h>

void debounce_init(debounce_t *db, uint8_t threshold, bool initial_level)
{
    if (db != NULL)
    {
        db->threshold = threshold;
        db->count = 0U;
        db->stable_level = initial_level;
        db->press_event = false;
    }
}

void debounce_sample(debounce_t *db, bool raw_pressed)
{
    if (db == NULL)
    {
        return;
    }

    if (raw_pressed == db->stable_level)
    {
        /* Same as the accepted level: any bounce so far is forgotten. */
        db->count = 0U;
        return;
    }

    db->count++;
    if (db->count >= db->threshold)
    {
        db->count = 0U;
        db->stable_level = raw_pressed;
        if (raw_pressed)
        {
            db->press_event = true; /* edge released -> pressed */
        }
    }
}

bool debounce_take_press_event(debounce_t *db)
{
    bool event = false;
    if (db != NULL)
    {
        event = db->press_event;
        db->press_event = false;
    }
    return event;
}

bool debounce_is_pressed(const debounce_t *db)
{
    return (db != NULL) && db->stable_level;
}
