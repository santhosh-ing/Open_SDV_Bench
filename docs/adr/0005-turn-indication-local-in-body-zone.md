# ADR 0005: Turn indication runs locally in the body zone controller

## Status

Proposed (2026-11-01). Not accepted until the conflicts listed below are resolved.

## Context

The turn-signal joystick (stalk) is to be connected to the body zone controller (ZC-STM32). Until now, the indicator and hazard logic was effectively central: the HPC sends `BODY_LightCmd` with `HazardReq`, `IndLeftReq` and `IndRightReq`, and the ZC-STM32 switches its lamps accordingly (ADR 0002, SW-ZCS-010). The node allocation of the stalk input was still open (comment on FUN-LIGHT-010).

Three allocations were considered:

1. **Fully local**: the ZC-STM32 reads the joystick, decides, generates the blink rhythm and drives the lamps.
2. **Central decision**: the ZC-STM32 forwards the joystick request, the HPC decides and commands the lamps over CAN.
3. **Split**: central decision, blink rhythm generated in the zone.

## Decision

**Option 1.** The turn indication function (joystick acquisition, request interpretation, blink generation, lamp control) is allocated completely to the ZC-STM32. The function is gated by the vehicle state set by the HPC (ADR 0006).

## Consequences

- ➕ Indication keeps working when the HPC or the CAN bus is lost, and the bus adds no blink jitter.
- ➕ A small, self-contained slice that can be verified on the board.
- ➖ For this feature the bench shows less of the "zones do I/O, central computer holds the logic" SDV pattern. A logic change needs a ZC-STM32 update (parameters or firmware), not just an HPC app update.
- ➖ The ZC-STM32 needs parameter storage and an update path (OTA demo, later SOVD → UDS).
- The ZC-STM32 reports the indicator and hazard state to the HPC (Kuksa / VSS, dashboard).

## Resolution of the conflicts and open points (2026-11-01)

| Item | Resolution |
|---|---|
| ADR 0002 / `body.dbc` v0.1.0 | `IndicatorLeftReq` / `IndicatorRightReq` are dropped from `BODY_LightCmd` in contract v0.2.0, proposed in ADR 0007. `HazardReq` stays as the central hazard request (FUN-LIGHT-012). |
| SW-ZCS-010 | Changed: low beam and central hazard from CAN, direction only from the joystick. |
| FUN-LIGHT-010 (comfort blink) | Allocated to the ZC-STM32; refined by SW-ZCS-050 and SW-ZCS-051. |
| Joystick hardware | KY-023-type analogue joystick, X axis on PA1, push button on PE7, powered from 3V (HW-ZCS-001, `docs/hardware/wiring/joystick.yml`). |
| Joystick interpretation | Thresholds with hysteresis and a 50 ms stability time (SW-ZCS-050). Tap = comfort blink, hold = indication while held (FUN-LIGHT-011, SW-ZCS-051). |
| Hazard input | The joystick push button toggles the local hazard request (FUN-LIGHT-012, SW-ZCS-052). |
| Lamps | On-board LEDs as in `AGENTS.md`: green = left, red = right. |

Still open: joystick fault detection (an open wire on the X axis cannot be told apart from a full deflection without extra hardware).
