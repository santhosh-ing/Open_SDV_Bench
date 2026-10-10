# Spec 002 – Body ECU light control over CAN (loopback)

Goal of this feature: the body ECU implements the `interfaces/can/body.dbc` v0.1 contract — it receives light commands and reports its light status — tested on the board alone, using CAN silent loopback (no transceiver needed yet).

Prerequisite: Spec 001 done and verified on the board.

---

## Part A — paste after `/speckit-specify`

Body zone ECU light control over CAN for open-sdv-bench, implementing the CAN contract `interfaces/can/body.dbc` version 0.1 (messages BODY_LightCmd 0x200 and BODY_LightStatus 0x210).

**Why:** In a software-defined vehicle, the central computer requests vehicle functions and zone ECUs execute them on the hardware. The body ECU must turn light requests from the CAN bus into real lamp behaviour, behave safely when requests stop arriving, and report its actual state so the rest of the vehicle (and later the Kuksa Databroker) can see it. Until a CAN transceiver is wired, the ECU tests itself: a test stimulus on the same board plays the role of the central computer.

**Lamp mapping on the board** (matches the physical LED layout): green = left indicator, red = right indicator, orange = low beam, blue = ECU heartbeat.

**User stories**

1. As the central computer, I send BODY_LightCmd every 100 ms, and the body ECU switches its lamps accordingly within one 100 ms cycle.
2. As a driver, when hazard lights are requested, both indicator lamps blink together, regardless of any left/right indicator request.
3. As a driver, when the left or right indicator is requested, only that side blinks.
4. As a safety engineer, when light commands stop arriving for 500 ms, the ECU goes to a safe state: indicators off, low beam ON (so the vehicle stays visible), reports CmdTimeout = 1 and EcuState = Degraded, and the blue heartbeat blinks fast. When valid commands return, the ECU resumes normal behaviour automatically.
5. As the central computer and the tester, I receive BODY_LightStatus every 100 ms with the actual lamp states, the current blink phase, the timeout flag, the ECU state and a rolling alive counter.
6. As the maintainer without a CAN transceiver, I press the user button to step through test scenarios that the on-board test stimulus sends over CAN loopback: Off → Low beam → Left indicator → Right indicator → Hazard → Silence (stop sending, to provoke the timeout) → back to Off.

**Functional requirements**

- FR-001: The ECU shall receive BODY_LightCmd (0x200) and use its signals only after validating ID, DLC = 8 and the alive counter.
- FR-002: A frame whose LightCmd_AliveCtr does not equal the previous value + 1 (modulo 16) shall be counted as an alive-counter error; its content shall still be used, and the error count shall be debugger-visible. (The first frame after start-up or after a timeout is accepted as the new reference.)
- FR-003: Indicator lamps shall blink at 1.5 Hz with 50 % duty cycle (≈333 ms on / ≈333 ms off). Left and right lamps blinking at the same time shall be in phase.
- FR-004: HazardReq = 1 shall make both indicator lamps blink and override IndicatorLeftReq and IndicatorRightReq.
- FR-005: IndicatorLeftReq = 1 and IndicatorRightReq = 1 at the same time without HazardReq shall be treated as an invalid combination: no indicator blinks. [clarify: confirm this behaviour]
- FR-006: LowBeamReq shall switch the low beam lamp directly (no blinking).
- FR-007: If no valid BODY_LightCmd is received for ≥ 500 ms, the ECU shall enter the fail-safe state described in user story 4.
- FR-008: The ECU shall transmit BODY_LightStatus (0x210) every 100 ms ± 10 ms, filled with the actual lamp states, BlinkPhase, CmdTimeout, EcuState, MirrorPosition = Unknown (0) and an alive counter incremented by 1 per frame.
- FR-009: The blue heartbeat shall blink at 1 Hz in state Normal and at 4 Hz in state Degraded.
- FR-010: MirrorFoldReq shall be accepted but ignored in this version (reserved for the servo feature).
- FR-011: In loopback builds, a test stimulus shall send BODY_LightCmd every 100 ms according to the selected scenario; each button press shall advance to the next scenario. The test stimulus shall exist only in loopback builds and shall use the CAN interface exactly like an external sender would (no shortcuts past the CAN driver).

**Out of scope:** real CAN bus and transceiver wiring, mirror servo, diagnostics (UDS), CAN FD, bus-off recovery strategy (parking lot), E2E CRC protection (parking lot).

**Traces to:** REQ-BODY-001 (hazard lights), REQ-BODY-002 (indicators), REQ-BODY-003 (low beam), REQ-BODY-004 (command loss fail-safe) — to be written in `docs/requirements/`.

**Acceptance (verified by the maintainer on the board):**
- Each button press steps through the scenarios, and the LEDs show exactly the expected pattern for each.
- In "Silence", after ~0.5 s: indicators off, orange (low beam) on, blue blinks fast. Next press ("Off") → blue back to 1 Hz.
- In the debugger, the received-frame counter increases by ~10 per second, the alive-counter error count stays 0, and the last transmitted status frame matches the LED states.

**Open questions for `/speckit-clarify`:**
- Is "low beam ON" the right fail-safe, or should lamps keep their last commanded state?
- Should the first indicator on-phase start immediately on request (as in real cars) or wait for the next blink cycle?
- FR-005 behaviour for the invalid left+right combination.

---

## Part B — paste after `/speckit-plan`

Technical context for the plan:

- Contract: `interfaces/can/body.dbc` v0.1. Generate C pack/unpack code with `python3 -m cantools generate_c_source interfaces/can/body.dbc` into `ecus/body-zone-stm32/app/com/generated/` (committed, never edited by hand). Add a script `tools/gen_can.sh` so regeneration is one command.
- CAN peripheral: CAN1 (bxCAN), 500 kbit/s, mode **silent loopback** (`CAN_MODE_SILENT_LOOPBACK`), so nothing is driven onto the pins. APB1 = 42 MHz → prescaler 6, BS1 = 11 tq, BS2 = 2 tq, SJW = 1 tq (14 tq per bit, sample point ≈ 85.7 %). Planned pins PD0 (CAN1_RX) / PD1 (CAN1_TX) — confirm against UM1472 before the transceiver milestone.
- Acceptance filter: accept only IDs 0x200 and 0x210 (list mode); receive via RX FIFO0 interrupt into a small ring buffer; frames are processed in the 10 ms task, never inside the interrupt.
- Layering (constitution principle III):
  - `app/com/can_drv.{h,c}` — HAL wrapper: init, start, send, rx ring buffer. Mode (loopback/normal) selected by a build option `BODY_CAN_LOOPBACK`.
  - `app/com/body_com.{h,c}` — uses generated code; validates frames, alive counters, 500 ms timeout supervision, 100 ms status transmit; exposes `body_com_get_light_cmd()` and `body_com_set_light_status()`.
  - `app/functions/light_ctrl.{h,c}` — hazard/indicator/low beam logic and blink timing; pure logic with an explicit input struct (requests + timeout flag) and output struct (lamp states + blink phase).
  - `app/functions/ecu_state.{h,c}` — extended with Degraded state and heartbeat rates.
  - `app/test_stim/test_stim.{h,c}` — loopback-only test stimulus (scenario state machine, uses `can_drv` to send). Compiled only when `BODY_CAN_LOOPBACK` is on.
  - `app/bsp/` — unchanged LED and button API from spec 001.
- Host unit tests (Unity + CTest) for `light_ctrl` (all scenarios, hazard override, invalid combination, blink timing) and for `body_com` validation logic (alive counter, timeout), using the generated pack/unpack code with faked CAN frames.
- End the task list with "flash and observe" steps for every acceptance criterion and a debugger checklist (counters to watch).
