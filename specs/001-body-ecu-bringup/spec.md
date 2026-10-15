# Feature Specification: Body ECU bring-up

**Feature Branch**: `001-body-ecu-bringup`

**Created**: 2026-10-14

**Status**: Implemented — awaiting verification on the board

**Input**: `docs/spec-inputs/01-body-ecu-bringup.md`, Part A — prove the toolchain and the firmware skeleton (layers, scheduler, board support) of the body zone ECU on the real STM32F407G-DISC1 board, before any CAN.

**Implements / traces to**: SW-ZCS-023 (deterministic start-up and operation), SW-ZCS-040 (generated code kept separate), SW-ZCS-041 (layered architecture), SW-ZCS-022 (heartbeat indication, Normal state only). Foundation for the light functions SW-ZCS-010 ff.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - The ECU shows it started and is alive (Priority: P1) 🎯 MVP

The maintainer powers or resets the board and sees a lamp test (all four LEDs on for 1 s, then off), like a real instrument cluster check. Afterwards the blue LED blinks at 1 Hz as an "ECU alive" heartbeat.

**Why this priority**: Proves start-up, the time base and LED control at once. Every later feature relies on it.

**Independent Test**: Press the black reset button and watch the LEDs.

**Acceptance Scenarios**:

1. **Given** the flashed board, **When** it is reset, **Then** all four LEDs are on for 1000 ms ± 50 ms and then all turn off.
2. **Given** the lamp test has finished, **When** the ECU runs, **Then** the blue LED blinks at 1 Hz with 500 ms on and 500 ms off, and keeps doing so.

---

### User Story 2 - A button press is detected exactly once (Priority: P2)

The maintainer presses the blue user button. Each physical press is detected exactly once, even though the contact bounces, and toggles the orange LED so the result is visible.

**Why this priority**: Debounced input handling is needed for every later switch input (e.g. the turn-signal stalk), but it builds on the time base from story 1.

**Independent Test**: Press the button 10 times at different speeds; count orange LED toggles.

**Acceptance Scenarios**:

1. **Given** state Normal, **When** the button is pressed and held for 50 ms or longer, **Then** exactly one press event is reported and the orange LED toggles once.
2. **Given** state Normal, **When** the button bounces or is pressed for less than 50 ms, **Then** no press event is reported.
3. **Given** state Normal, **When** the button is held down for several seconds, **Then** only one event is reported; the next event requires a release first.

---

### User Story 3 - The time base can be checked in the debugger (Priority: P3)

The maintainer opens live expressions in the debugger and sees a millisecond uptime counter, a counter of executed 100 ms tasks and the current ECU state, so the time base can be checked without an oscilloscope.

**Why this priority**: Diagnostic convenience; the feature works without it.

**Independent Test**: Run for about 60 s with live expressions open; compare the uptime counter with a stopwatch.

**Acceptance Scenarios**:

1. **Given** the ECU has run for about 60 s, **When** the maintainer reads the uptime counter, **Then** it shows about 60000 (± 1 %).
2. **Given** the ECU runs, **When** the maintainer reads the 100 ms task counter, **Then** it equals the uptime divided by 100 (± 1).
3. **Given** the lamp test is running or finished, **When** the maintainer reads the ECU state, **Then** it shows Init or Normal respectively.

### Edge Cases

- The button is pressed during the lamp test: the press is ignored (the lamp test owns the LEDs), and the orange LED starts from "off" in state Normal.
- The main loop is late by several milliseconds (e.g. a breakpoint was hit): the scheduler catches up tick by tick, so no period is skipped and no drift accumulates.
- The millisecond counter wraps after about 49.7 days: elapsed-time calculations stay correct across the wrap.
- The button is held down at reset: no event until it has been released and pressed again.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: On start-up the ECU shall perform a lamp test: all four LEDs on for 1000 ms ± 50 ms, then all off.
- **FR-002**: After the lamp test the ECU shall enter state Normal and blink the blue LED at 1 Hz with 50 % duty cycle. *(SW-ZCS-022, Normal part)*
- **FR-003**: The ECU shall provide periodic task slots of 1 ms, 10 ms and 100 ms. Every slot shall run at its period without drift accumulating over time. *(SW-ZCS-023)*
- **FR-004**: The ECU shall detect a button press only after the button has been stably pressed for at least 50 ms, and report exactly one press event per physical press.
- **FR-005**: In this bring-up feature, each button press event shall toggle the orange LED.
- **FR-006**: The ECU shall expose an uptime counter in milliseconds and a 100 ms task execution counter as debugger-visible variables.
- **FR-007**: The ECU shall have explicit states Init (during start-up and lamp test) and Normal; the current state shall be readable in the debugger.
- **FR-008**: Generated code (CubeMX) shall stay unchanged except for calling `app_init()` once and `app_run()` in the main loop, and application code shall follow the layers functions → bsp → HAL. *(SW-ZCS-040, SW-ZCS-041)*

### Key Entities

- **ECU state**: Init or Normal (Degraded and Fault follow with spec 002).
- **Task slot**: a fixed period (1, 10, 100 ms) with the functions that run in it.
- **Button press event**: a one-shot flag, set once per debounced press, cleared when read.

### Out of scope

CAN communication, servo, diagnostics, low-power modes, watchdog (parking lot), joystick turn-signal stalk and buzzer (later light feature).

## Success Criteria *(mandatory)*

- **SC-001**: After reset: 1 s all LEDs on → all off → blue blinks at 1 Hz (observed on the board).
- **SC-002**: 10 button presses → orange LED toggles exactly 10 times.
- **SC-003**: Uptime counter after about 60 s matches a stopwatch within 1 %.
- **SC-004**: Host unit tests for debounce, scheduler timing and lamp-test/heartbeat timing pass on a PC without hardware.
- **SC-005**: The firmware builds for the target from a clean checkout with the CMake Debug preset.

## Assumptions

- The board's user button has an external pull-down and reads high when pressed (UM1472).
- The ST HAL's 1 ms SysTick (`HAL_GetTick`) is the time base; no extra timer is needed.
