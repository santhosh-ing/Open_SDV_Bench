/**
 * @file    app.h
 * @brief   Application entry points — the only symbols called from main.c.
 *
 * Layer:   app (top; wires the layers together)
 * Spec:    001-body-ecu-bringup (FR-008)
 * Req:     SW-ZCS-040, SW-ZCS-041
 */
#ifndef APP_H
#define APP_H

/* Call once after the CubeMX init code (clock, GPIO). */
void app_init(void);

/* Call on every pass of the main loop. Never blocks. */
void app_run(void);

#endif /* APP_H */
