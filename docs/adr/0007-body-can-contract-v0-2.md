# ADR 0007: Body CAN contract v0.2 (vehicle state, local direction indication)

## Status

Proposed (2026-11-01). `interfaces/can/body.dbc` changes only after this ADR is accepted.

## Context

ADR 0005 moves direction indication into the body zone controller (joystick on the ZC-STM32). ADR 0006 introduces vehicle states (Off / On / Update) managed by the CGW role on the HPC, which the zone controllers must follow and supervise. Contract v0.1.0 (ADR 0002) has neither a vehicle state message nor a way to report its loss, and it still carries direction requests from the HPC.

## Decision

Contract **v0.2.0**, all other rules of ADR 0002 unchanged (CAN classic, 11-bit IDs, 500 kbit/s, 1-bit booleans, 4-bit alive counter in byte 7):

| ID | Message | Sender → Receiver | Cycle | Change |
|---|---|---|---|---|
| 0x100 | **VEH_State** | HPC → BODY_ECU (later all zones) | 100 ms | **New** |
| 0x200 | BODY_LightCmd | HPC → BODY_ECU | 100 ms | `IndicatorLeftReq` and `IndicatorRightReq` removed; bits 1–2 reserved |
| 0x210 | BODY_LightStatus | BODY_ECU → HPC, TESTER | 100 ms | New signals `VehStateTimeout` and `VehicleStateApplied` |

**VEH_State (0x100), DLC 8**

| Signal | Start bit / length | Values |
|---|---|---|
| `VehicleState` | 0 / 2 | 0 = Off, 1 = On, 2 = Update, 3 = reserved (treated as invalid) |
| `VehState_AliveCtr` | 56 / 4 | 0–15, rolling |

**BODY_LightStatus (0x210), additions**

| Signal | Start bit / length | Values |
|---|---|---|
| `VehStateTimeout` | 6 / 1 | 1 = no valid VEH_State for 500 ms or longer |
| `VehicleStateApplied` | 10 / 2 | The vehicle state the ECU currently follows: 0 = Off, 1 = On, 2 = Update, 3 = fail-safe (acting as On) |

- `VEH_State` missing for 500 ms or longer (five cycles) = vehicle state timeout (FUN-SAFE-001, SW-ZCS-021).
- `IndicatorLeftActive` and `IndicatorRightActive` in `BODY_LightStatus` keep their meaning: they now report the result of local indication.
- `VEH_State` gets the lowest ID of the three messages, so it wins arbitration on the bus.

## Consequences

- `body.dbc` gets `VERSION "0.2.0"`, and the generated pack/unpack code is regenerated with `tools/gen_can.sh`.
- The loopback test stimulus (spec 002) also sends `VEH_State` and can stop it on its own, so the vehicle state timeout can be tested on the board without the HPC.
- The HPC-side CAN provider and the Kuksa / VSS mapping must publish the vehicle state and read `VehicleStateApplied` (milestone M2).
- Spec input 002 (`docs/spec-inputs/02-body-ecu-can-lights.md`) still describes v0.1.0 and must be updated before `/speckit-specify`.

## Open points

- VSS path for the vehicle state (standard VSS has no direct ignition-state signal; a custom branch may be needed).
- Whether other zones (ZC-Arduino over Ethernet / MQTT) receive the vehicle state through a different contract.
