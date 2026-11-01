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

## Conflicts with the current baseline (to be resolved before acceptance)

- **ADR 0002 / `interfaces/can/body.dbc` v0.1.0**: `IndLeftReq` and `IndRightReq` in `BODY_LightCmd` (HPC → ZC) would no longer be the source of indication. The contract needs a new version and its own ADR (rule 1 in `AGENTS.md`).
- **SW-ZCS-010** (outputs follow the CAN requests) must be split or changed for the indicators.
- **FUN-LIGHT-010** (comfort blink): the comment's open allocation question is answered here (ZC-STM32).
- Is `HazardReq` still sent by the HPC, or does hazard also get a local input? See ADR 0006.

## Open points

- The joystick hardware (model, analogue or digital, pins on the STM32F407G-DISC1) is not in `docs/hardware/bom.md` yet.
- Joystick interpretation: thresholds, debounce, neutral position, fault detection (stuck, out of range, open wire) and the resulting DTCs.
- The lamp mapping stays as in `AGENTS.md` (green = left, red = right) unless decided otherwise.
