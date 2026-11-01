# bsp – board support

LEDs (PD12–PD15), user button (PA0), joystick (ADC1 on PA1, button on PE7; `bsp_joystick`), servo PWM (later). The only layer besides `com/` that calls the ST HAL. Exposes small APIs such as `bsp_led_set()`; no pin numbers outside this folder.
