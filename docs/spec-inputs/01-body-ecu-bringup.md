# Spec 001 – Body ECU bring-up

Goal of this feature: prove the toolchain and the firmware skeleton (layers, scheduler, BSP) on the real board — before any CAN.

Spec Kit separates **what/why** (`/speckit-specify`) from **how** (`/speckit-plan`).
That's why this file has two parts. Don't paste the technical part into `specify`.

---

## Part A — paste after `/speckit-specify`

Body zone ECU bring-up for the open-sdv-bench body ECU (STM32F407G-DISC1 board).

**Why:** Before the ECU can implement vehicle functions, we need a proven firmware skeleton: the board starts reliably, has a time base for periodic tasks, can drive its four LEDs and read its button, and shows the maintainer that it is alive. Every later feature (CAN light control, diagnostics, servo) builds on this.

**User stories**

1. As the maintainer, when I power or reset the board, I see a lamp test: all four LEDs light for 1 second and then turn off, like a real car's instrument cluster check. This tells me the ECU started and every LED works.
2. As the maintainer, after the lamp test I see the blue LED blinking at 1 Hz (500 ms on, 500 ms off) as an "ECU alive" heartbeat for as long as the ECU runs normally.
3. As the maintainer, when I press the user button, the press is detected exactly once per press, even if the button contact bounces (debounced, minimum stable time 50 ms). For this bring-up, each press toggles the orange LED so I can verify it.
4. As the maintainer, I can see in the debugger (live expressions) a millisecond uptime counter and a counter of executed scheduler cycles, so I can check the time base without an oscilloscope.

**Functional requirements**

- FR-001: On start-up the ECU shall perform a lamp test: all four LEDs on for 1000 ms ± 50 ms, then all off.
- FR-002: After the lamp test the ECU shall enter state Normal and blink the blue LED at 1 Hz with 50 % duty cycle.
- FR-003: The ECU shall provide periodic task slots of 1 ms, 10 ms and 100 ms. Every slot shall run at its period without drift accumulating over time.
- FR-004: The ECU shall detect a button press only after the button has been stably pressed for at least 50 ms, and report exactly one press event per physical press.
- FR-005: In this bring-up feature, each button press event shall toggle the orange LED.
- FR-006: The ECU shall expose an uptime counter in milliseconds and a 100 ms task execution counter as debugger-visible variables.
- FR-007: The ECU shall have explicit states Init (during start-up and lamp test) and Normal; the current state shall be readable in the debugger.

**Out of scope:** CAN communication, servo, diagnostics, low-power modes, watchdog (parking lot).

**Traces to:** foundation for REQ-BODY-* (light functions); no direct system requirement.

**Acceptance (verified by the maintainer on the board):**
- After reset: 1 s all LEDs on → all off → blue blinks at 1 Hz.
- 10 button presses → orange LED toggles exactly 10 times.
- Uptime counter after ~60 s of runtime shows ~60000 (±1 %), compared to a stopwatch.

---

## Part B — paste after `/speckit-plan`

Technical context for the plan:

- Board STM32F407G-DISC1, MCU STM32F407VGT6. System clock 168 MHz from the 8 MHz HSE crystal via PLL; APB1 42 MHz, APB2 84 MHz.
- LEDs: PD12 green, PD13 orange, PD14 red, PD15 blue (push-pull outputs). Button: PA0, input, active high (external pull-down on the board).
- Project generated with STM32CubeMX (Board Selector: STM32F407G-DISC1, toolchain CMake) into `ecus/body-zone-stm32/`. Use the ST HAL. Disable CubeMX's own BSP/LED demo code if it gets in the way of our `app/bsp/` layer.
- Firmware layering as defined in the constitution:
  - `app/sched/` — cooperative scheduler driven by the 1 ms SysTick (`HAL_IncTick` / a 1 ms tick hook); tasks run in `app_run()` from the main loop, not inside the interrupt.
  - `app/bsp/led.{h,c}` — `bsp_led_set(led_id, on)`, `bsp_led_toggle(led_id)`; LED IDs as an enum, no pin numbers outside bsp.
  - `app/bsp/button.{h,c}` — sampled every 10 ms, debounce counter, event flag API `bsp_button_pressed_event()`.
  - `app/functions/ecu_state.{h,c}` — Init/Normal state, lamp test sequence, heartbeat logic. No HAL calls in this layer.
  - `app/app.{h,c}` — `app_init()`, `app_run()`; the only symbols called from `main.c` (inside USER CODE blocks).
- Debugger-visible counters as `volatile` globals in their owning module.
- Host unit tests (CMake + CTest + Unity) for the button debounce logic and lamp-test/heartbeat timing logic, with the HAL calls behind the bsp interface so they can be faked on the PC.
- Build: CMake presets for Debug and Release for the target; a separate CMake project or preset for host tests.
- End the task list with explicit "flash and observe" steps for each acceptance criterion.
