# Feature Specification: Joystick turn indication and hazard lights

**Feature Branch**: `004-joystick-turn-indication`

**Created**: 2026-11-15

**Status**: Draft

**Input**: `docs/spec-inputs/04-body-ecu-joystick-indication.md`, Part A — the driver operates the turn indicators and the hazard lights with the joystick on the body zone ECU; the on-board LEDs act as lamps. Everything runs locally on the body zone ECU: no CAN, no central computer.

**Implements / traces to**: SW-ZCS-050 (joystick direction classification), SW-ZCS-051 (tap and hold), SW-ZCS-052 (local hazard switch), SW-ZCS-011 (blink timing), HW-ZCS-001 (joystick connection). Parents: FUN-LIGHT-001, FUN-LIGHT-002, FUN-LIGHT-010, FUN-LIGHT-011, FUN-LIGHT-012. Source: `docs/requirements/30-zc-stm32-requirements.sdoc`.

**Why**: In the bench architecture, direction indication runs entirely in the body zone controller, so it keeps working even if the central computer or the bus is lost (ADR 0005). The joystick springs back to its centre, so it behaves like a stalk's non-locking "touch" position: a short tap gives a comfort blink, and holding it indicates for as long as it is held. The joystick's push button is the local hazard switch (ADR 0006).

**Lamp mapping on the board**:

| LED    | Role in this feature                          |
|--------|-----------------------------------------------|
| Green  | Left indicator lamp                           |
| Red    | Right indicator lamp                          |
| Blue   | ECU heartbeat, 1 Hz (unchanged from spec 001) |
| Orange | Unchanged from spec 001 (see FR-012)          |

## Clarifications

### Session 2026-11-15

- Q: Does the first flash start immediately on the request, or at the next blink cycle? → A: Immediately (FR-011).
- Q: Does the hazard toggle need a release before it can toggle again when the button is held? → A: Yes, one toggle per press (FR-013).
- Q: Keep or remove the spec 001 demo (blue user button toggles orange)? → A: Keep (FR-012).

## User Scenarios & Testing *(mandatory)*

### User Story 1 - A tap gives a comfort blink (Priority: P1) 🎯 MVP

The driver taps the joystick left or right and lets it spring back in less than 0.5 s. The lamp of that side flashes exactly 3 times and then stays off.

**Why this priority**: It is the smallest complete slice of the feature: it proves reading the joystick, deciding the direction, the flash timing and driving the lamps. Every other story builds on it.

**Independent Test**: Tap the joystick left, count the green flashes; tap right, count the red flashes.

**Acceptance Scenarios**:

1. **Given** no indication is running, **When** the driver taps the joystick left for less than 0.5 s, **Then** the green lamp flashes exactly 3 times and stays off afterwards; the red lamp stays off.
2. **Given** no indication is running, **When** the driver taps the joystick right for less than 0.5 s, **Then** the red lamp flashes exactly 3 times and stays off afterwards; the green lamp stays off.
3. **Given** a lamp is flashing, **When** its on and off times are measured, **Then** each is about 333 ms (1.5 Hz, 50 % duty cycle).
4. **Given** the joystick rests at its centre or is only slightly moved, **When** nothing else happens, **Then** no lamp flashes.
5. **Given** no indication is running, **When** the driver pushes the joystick to one side, **Then** the lamp of that side switches on at once (no noticeable delay), and this first on phase counts as the first of the 3 flashes.

---

### User Story 2 - Holding indicates until release (Priority: P2)

The driver holds the joystick left or right for 0.5 s or longer. That side flashes for as long as the joystick is held. After release, the flash that is in progress is completed and the lamp then stays off.

**Why this priority**: Together with story 1 it makes the joystick a complete turn-signal stalk. It reuses everything from story 1 and only adds the tap/hold distinction.

**Independent Test**: Hold the joystick left for about 3 s, then release; watch the green lamp.

**Acceptance Scenarios**:

1. **Given** no indication is running, **When** the driver holds the joystick left for about 3 s, **Then** the green lamp flashes during the whole time.
2. **Given** the driver holds a direction and the lamp is in its on phase, **When** the joystick is released, **Then** the lamp finishes that on phase and then stays off.
3. **Given** the driver holds a direction and the lamp is in its off phase, **When** the joystick is released, **Then** the lamp stays off (no further flash).

---

### User Story 3 - The joystick button switches the hazard lights (Priority: P2)

The driver presses the joystick's push button: both indicator lamps flash together (hazard lights). The driver presses again: both stop.

**Why this priority**: Hazard is the safety-relevant part of the feature (FUN-LIGHT-002, FUN-LIGHT-012) and must be available locally (ADR 0006). It is independent of the direction logic.

**Independent Test**: Press the joystick button; green and red flash together. Press again; both stop.

**Acceptance Scenarios**:

1. **Given** hazard is off, **When** the driver presses the joystick button, **Then** green and red flash together, in phase, at 1.5 Hz.
2. **Given** hazard is on, **When** the driver presses the joystick button again, **Then** both lamps stop.
3. **Given** any state, **When** the button contact bounces during one press, **Then** the hazard state changes exactly once.
4. **Given** any state, **When** the driver keeps the button pressed for several seconds, **Then** the hazard state changes exactly once; the next change requires a release first.
5. **Given** hazard is off, **When** the driver presses the joystick button, **Then** both lamps switch on at once (no waiting for a blink cycle).

---

### User Story 4 - Hazard has priority over direction (Priority: P3)

While the hazard lights are on, moving the joystick does not change the flashing. When the driver switches hazard off while still holding a direction, that side continues to flash.

**Why this priority**: Priority handling between the two functions; it needs stories 2 and 3 first.

**Independent Test**: Switch hazard on, push the joystick left and right (no change); hold right and switch hazard off (red continues, green stops).

**Acceptance Scenarios**:

1. **Given** hazard is on, **When** the driver taps or holds the joystick left or right, **Then** both lamps keep flashing together without any visible change.
2. **Given** hazard is on and the driver holds the joystick right, **When** the driver switches hazard off, **Then** the red lamp continues flashing and the green lamp stops.
3. **Given** hazard is on and the joystick is at its centre, **When** the driver switches hazard off, **Then** both lamps stop and no indication follows.
4. **Given** a comfort blink or a held indication is running, **When** the driver switches hazard on, **Then** both lamps flash together.

---

### User Story 5 - A new tap replaces a running comfort blink (Priority: P3)

During a comfort blink the driver taps the other direction. The running comfort blink stops and the new side starts its own 3 flashes.

**Why this priority**: Refinement of story 1 for a quick change of mind.

**Independent Test**: Tap left; while green is flashing, tap right; green stops and red flashes 3 times.

**Acceptance Scenarios**:

1. **Given** a comfort blink left is running, **When** the driver taps right, **Then** the green lamp stops and the red lamp flashes exactly 3 times.
2. **Given** a comfort blink left is running, **When** the driver taps left again, **Then** the green lamp flashes 3 times counted from the new tap.

---

### User Story 6 - The maintainer sees the inputs in the debugger (Priority: P3)

The maintainer opens live expressions in the debugger and sees the joystick's raw value, its classified direction, the indication mode and the hazard request. This shows why the lamps behave as they do and lets the maintainer check the thresholds and the mounting direction.

**Why this priority**: Diagnostic convenience; the feature works without it.

**Independent Test**: With live expressions open, move the joystick slowly from end to end and press the button; compare the values with the lamps.

**Acceptance Scenarios**:

1. **Given** the ECU runs, **When** the maintainer moves the joystick from one end to the other, **Then** the raw value moves across the range 0 to 4095 and the classified direction changes between left, neutral and right at the specified thresholds.
2. **Given** the ECU runs, **When** the driver taps, holds or presses the button, **Then** the indication mode shows comfort, held or hazard respectively, and off when nothing is flashing.
3. **Given** the ECU runs, **When** the driver presses the joystick button, **Then** the hazard request changes between off and on.

### Edge Cases

- **Joystick moved only a little** (raw value stays between 1024 and 3072 coming from neutral): no direction is classified, no lamp flashes.
- **Joystick wiggles around a threshold**: the hysteresis bands and the 50 ms stability time prevent the classification from flickering; one deflection gives one request.
- **Deflection shorter than 50 ms**: it never becomes a classification, so nothing flashes.
- **Joystick moved straight from left to right** without resting at the centre: the new side is a new direction request; the previous side stops at once and the new side starts.
- **Release exactly at 0.5 s**: 0.5 s or longer counts as a hold.
- **Hold released shortly after 0.5 s**: the indication ends with the flash in progress, so a hold just over 0.5 s can show fewer flashes than a tap (requirements as written, FUN-LIGHT-011).
- **Tap while hazard is on**: no visible effect, and no comfort blink follows when hazard is switched off later.
- **Hazard switched on during a comfort blink, then off again**: the interrupted comfort blink is not resumed.
- **Button pressed or joystick moved during the lamp test** (state Init): ignored; indication and hazard start as off in state Normal.
- **Button held down at reset**: no hazard toggle until the button has been released and pressed again.
- **Joystick not connected**: the raw value is undefined and lamps may flash unintentionally; detecting this is out of scope.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The ECU shall sample the joystick X axis every 10 ms with 12-bit resolution (0 to 4095) and classify it as left below 1024, right above 3072 and neutral between 1536 and 2560. In the bands between (1024 to 1536 and 2560 to 3072) it shall keep the previous classification (hysteresis). A new classification shall be used only after it has been stable for 50 ms. *(SW-ZCS-050)*
- **FR-002**: Which end of the axis means "left" shall be defined in exactly one named setting, so it can be swapped to match how the joystick is mounted. *(SW-ZCS-050)*
- **FR-003**: If a direction returns to neutral less than 500 ms after it was classified, the ECU shall flash that side exactly 3 times (comfort blink). If the direction stays classified for 500 ms or longer, the ECU shall flash that side until the classification returns to neutral, and shall complete the flash in progress. *(SW-ZCS-051, FUN-LIGHT-010, FUN-LIGHT-011)*
- **FR-004**: A new direction request (either side) shall cancel a running comfort blink and start the new indication. *(SW-ZCS-051)*
- **FR-005**: The ECU shall debounce the joystick push button (PE7, active low) and toggle the local hazard request on each press. *(SW-ZCS-052, HW-ZCS-001)*
- **FR-006**: While the hazard request is on, both indicator lamps shall flash in phase, and direction requests shall have no visible effect. *(FUN-LIGHT-002, FUN-LIGHT-012)*
- **FR-007**: Indicator lamps shall flash at 1.5 Hz with a 50 % duty cycle (about 333 ms on and 333 ms off, each within ± 10 ms). *(SW-ZCS-011)*
- **FR-008**: The raw X value, the classified direction (left / neutral / right), the indication mode (off / comfort / held / hazard) and the hazard request (off / on) shall be visible in the debugger.
- **FR-009**: Until the vehicle state is received over CAN (spec 002), the ECU shall behave as in vehicle state On: direction requests and hazard are always accepted in state Normal.
- **FR-010**: When the hazard request is switched off while a direction is still classified, that side shall continue flashing as a held indication; without a classified direction, both lamps shall stop.
- **FR-011**: The first flash shall start immediately when a request is accepted (a classified direction, or the hazard request switching on), not at the next cycle of an already running blink rhythm. The lamp is on within 20 ms of the request being accepted.
- **FR-012**: The spec 001 demo (blue user button toggles the orange LED) shall be kept unchanged in this feature. Orange is reserved for the low beam; the demo is removed when the low beam feature is specified.
- **FR-013**: The hazard request shall toggle exactly once per button press. A held button shall not toggle again; the next toggle requires a release first.
- **FR-014**: The heartbeat (blue, 1 Hz) and the start-up lamp test from spec 001 shall be unaffected by this feature.

### Key Entities

- **Raw X value**: the latest joystick X-axis sample, 0 to 4095.
- **Classified direction**: left, neutral or right — the debounced result of the raw X value with hysteresis.
- **Direction request**: one period during which the classified direction is left or right; either a tap (shorter than 500 ms) or a hold (500 ms or longer).
- **Hazard request**: off or on; toggled by the joystick push button; off after start-up.
- **Indication mode**: off, comfort, held or hazard — what the indicator function is doing right now.
- **Flash**: one on phase of a lamp (about 333 ms), followed by an equally long off phase.

### Out of scope

CAN and the contract (spec 002), vehicle state gating and the vehicle-state fail-safe (SW-ZCS-053, SW-ZCS-054), low beam, status reporting, the joystick Y axis, joystick fault detection, parameters / OTA.

## Success Criteria *(mandatory)*

Verified by the maintainer on the board, unless stated otherwise.

- **SC-001**: Tap left → the green lamp flashes exactly 3 times. Tap right → the red lamp flashes exactly 3 times. Result is the same in 10 out of 10 taps per side.
- **SC-002**: Hold left for about 3 s → the green lamp flashes the whole time and stops after release, finishing the flash in progress.
- **SC-003**: Press the joystick button → green and red flash together; press again → both stop. 10 presses give exactly 10 changes of the hazard state.
- **SC-004**: With hazard on, pushing the joystick left and right changes nothing; switching hazard off while holding right → red continues, green stops.
- **SC-005**: While a comfort blink left is running, tap right → green stops and red flashes exactly 3 times.
- **SC-006**: A flashing lamp completes 15 flashes in 10 s ± 0.5 s (1.5 Hz, checked with a stopwatch).
- **SC-007**: The blue heartbeat keeps blinking at 1 Hz throughout all of the above.
- **SC-008**: The raw value, classified direction, indication mode and hazard request shown in the debugger match what the driver does and what the lamps show.
- **SC-009**: The decision logic (classification, tap/hold, comfort blink, hazard priority, flash timing) is also proven by automated tests that run on a PC without the board, each tagged with the requirement ID it verifies.

## Assumptions

- Spec 001 is verified on the board, and the joystick is wired as in `docs/hardware/wiring/joystick.yml` (HW-ZCS-001): X axis on PA1, push button on PE7 with pull-up, powered from 3 V.
- The joystick button uses the same debounce rule as the user button in spec 001: a press counts after the button has been stably pressed for 50 ms.
- A "flash" is one on phase. A comfort blink is 3 on phases; the lamp is off after the third.
- Switching hazard off, or a new direction request replacing a comfort blink, switches the affected lamps off at once. Only the release of a held indication completes the flash in progress (FR-003).
- After hazard was on, a direction that is still classified continues as a held indication, regardless of how long it has been held.
- Hazard and indication are off after every reset; nothing is stored across power cycles.
- Inputs are evaluated only in state Normal (after the lamp test).
- FR-009 is a temporary rule for this feature. SW-ZCS-053 states that the vehicle state is treated as Off until it is first received; that behaviour arrives with spec 002 and will replace FR-009.
- No requirement text in `docs/requirements/` and no contract in `interfaces/` changes with this feature.
