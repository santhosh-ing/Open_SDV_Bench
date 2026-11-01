# Spec 004 – Joystick turn indication and hazard (local, no CAN)

Goal of this feature: the driver operates the turn indicators and the hazard lights with the joystick on the body zone controller, and the board LEDs show the result. Everything runs locally on the STM32; no CAN, no central computer (ADR 0005).

Prerequisite: spec 001 verified on the board; joystick wired as in `docs/hardware/wiring/joystick.yml` (HW-ZCS-001).

Why before spec 002 (CAN): this slice needs no transceiver and no contract change, and it gives the first visible vehicle function on the bench. Spec 002 (CAN loopback, contract v0.2, vehicle state, fail-safe) follows and adds to it.

---

## Part A — paste after `/speckit-specify`

Body zone ECU turn indication and hazard lights operated by a joystick, for open-sdv-bench. The joystick acts as the turn-signal stalk; the on-board LEDs act as lamps.

**Why:** In the bench architecture, direction indication runs entirely in the body zone controller, so it keeps working even if the central computer or the bus is lost (ADR 0005). The joystick springs back to its centre, so it behaves like a stalk's non-locking "touch" position: a short tap gives a comfort blink, and holding it indicates for as long as it is held. The joystick's push button is the local hazard switch (ADR 0006).

**Lamp mapping on the board:** green = left indicator, red = right indicator, blue = ECU heartbeat (unchanged), orange = unchanged from spec 001.

**User stories**

1. As a driver, when I tap the joystick left or right (shorter than 0.5 s), that side flashes exactly 3 times (comfort blink).
2. As a driver, when I hold the joystick left or right for 0.5 s or longer, that side flashes until I let go; the flash that is in progress is completed.
3. As a driver, when I press the joystick button, the hazard lights switch on (both sides flash together); when I press it again, they switch off.
4. As a driver, while the hazard lights are on, moving the joystick does not change the flashing; when I switch hazard off and still hold a direction, that side continues.
5. As a driver, when I tap the other direction during a comfort blink, the running comfort blink stops and the new side starts.
6. As the maintainer, I can see the joystick's raw value, its classified direction and the hazard state in the debugger.

**Functional requirements**

- FR-001: The ECU shall sample the joystick X axis every 10 ms (12-bit) and classify it as left below 1024, right above 3072, neutral between 1536 and 2560, keeping the previous classification in the bands between (hysteresis). A new classification is used only after it has been stable for 50 ms. (SW-ZCS-050)
- FR-002: Which end of the axis means "left" shall be one named constant, so it can be swapped to match how the joystick is mounted. (SW-ZCS-050)
- FR-003: If a direction returns to neutral less than 500 ms after it was classified, the ECU shall flash that side 3 times; if it stays classified for 500 ms or longer, the ECU shall flash that side until neutral, completing the flash in progress. (SW-ZCS-051)
- FR-004: A new direction request shall cancel a running comfort blink and start the new one. (SW-ZCS-051)
- FR-005: The ECU shall debounce the joystick push button (PE7, active low) and toggle the local hazard request on each press. (SW-ZCS-052)
- FR-006: While hazard is on, both indicator lamps shall flash in phase and direction requests shall have no visible effect. (FUN-LIGHT-002)
- FR-007: Indicator lamps shall flash at 1.5 Hz with a 50 % duty cycle (≈ 333 ms on / ≈ 333 ms off). (SW-ZCS-011)
- FR-008: The raw X value, the classified direction, the indication mode (off / comfort / held / hazard) and the hazard request shall be debugger-visible.
- FR-009: Until the vehicle state is received over CAN (spec 002), the ECU shall behave as in vehicle state On.

**Out of scope:** CAN and the contract (spec 002), vehicle state gating and the vehicle-state fail-safe (SW-ZCS-053, SW-ZCS-054), low beam, status reporting, the Y axis, joystick fault detection, parameters / OTA.

**Traces to:** SW-ZCS-011, SW-ZCS-050, SW-ZCS-051, SW-ZCS-052, HW-ZCS-001 (parents FUN-LIGHT-001, -002, -010, -011, -012) — see `docs/requirements/30-zc-stm32-requirements.sdoc`.

**Acceptance (verified by the maintainer on the board):**
- Tap left → green flashes exactly 3 times. Tap right → red flashes exactly 3 times.
- Hold left for ~3 s → green flashes the whole time and stops after release (finishing the current flash).
- Press the joystick button → green and red flash together; press again → both stop.
- Hazard on, push the joystick left and right → no change; hazard off while holding right → red continues.
- Comfort blink left running, tap right → green stops, red flashes 3 times.
- The blue heartbeat keeps blinking at 1 Hz throughout.

**Open questions for `/speckit-clarify`:**
- Does the first flash start immediately on the request (as in real cars), or at the next blink cycle? (Suggested: immediately.)
- Does the hazard toggle need a release before it can toggle again when the button is held? (Suggested: yes, one toggle per press.)
- Keep or remove the spec 001 demo (blue user button toggles orange)? Orange is reserved for the low beam.

---

## Part B — paste after `/speckit-plan`

Technical context for the plan:

- **CubeMX** (maintainer, in `body-zone-stm32.ioc`):
  - ADC1, channel IN1 on PA1, 12-bit, single conversion, software trigger, sampling time ≥ 84 cycles (joystick potentiometers have a high source impedance).
  - PE7 as GPIO input with pull-up.
  - Regenerate the code; edit nothing outside the `USER CODE` blocks.
- **Layering** (ADR 0001, constitution):
  - `app/bsp/bsp_joystick.{h,c}`: starts one ADC conversion and reads the result (polling, no blocking wait longer than one conversion), reads the push-button pin. Only this module touches the ADC and PE7.
  - `app/bsp/debounce.{h,c}`: reused from spec 001 for the push button.
  - `app/functions/joystick_dir.{h,c}`: pure logic. Raw value in, classified direction out, with hysteresis and the 50 ms stability filter.
  - `app/functions/indication.{h,c}`: pure logic. Inputs: direction, hazard press event, vehicle state (fixed to On for now), 10 ms tick. Outputs: left/right lamp on/off, blink phase, mode. It contains the tap/hold timing, comfort blink counting, hazard toggle and priority, and the blink generator, with an explicit input and output struct.
  - `app/app.c`: calls bsp → joystick_dir → indication → bsp_led in the 10 ms slot.
- **Constants**: named in the headers (thresholds, 50 ms, 500 ms, 3 flashes, 333 ms half period, axis orientation). No magic numbers. They are candidates for parameters later (OTA demo).
- **Host unit tests** (Unity + CTest, like spec 001), each with `@verifies <ID>`:
  - `joystick_dir`: thresholds, hysteresis band, 50 ms stability, orientation constant
  - `indication`: tap = 3 flashes, hold = flashing until neutral and finishing the current flash, new direction cancels comfort blink, hazard toggle, hazard priority, resume after hazard off, 1.5 Hz timing, in-phase flashing
- End the task list with "flash and observe" steps for every acceptance criterion and a debugger checklist (raw X value at rest, fully left and fully right; classified direction; mode).
