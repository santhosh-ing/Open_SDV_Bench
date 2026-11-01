# ADR 0006: Vehicle state management by the HPC, and the body zone fail-safe

## Status

Proposed (2026-11-01). Not accepted until the conflicts listed below are resolved.

## Context

Zone functions, starting with the local turn indication (ADR 0005), must behave according to a vehicle-level state similar to a key or ignition. For demos the state must be changeable, and parameter updates (OTA, later SOVD) need a safe state to be applied in. Until now the bench has only ECU states (Init, Normal, Degraded, Fault; SW-ZCS-020), not vehicle states.

## Decision

1. The **central gateway (CGW) role** is the **vehicle state manager** and the **single master** of the vehicle state. It runs on the **HPC (Raspberry Pi 5)**.
2. A **dashboard** on the HPC sets the vehicle state through the CGW (simulated ignition). It is an input to the CGW, not a second mode source.
3. Vehicle states: **`Off`**, **`On`**, **`Update`**. Parameter changes are applied only in `Update`.
4. The ZC-STM32 **follows** the vehicle state. There is **no manual mode switch** on the ZC-STM32; the joystick only controls the indicators.
5. **Loss of the vehicle state signal**: if it is missing for **5 cycles**, the ZC-STM32 enters its fail-safe state.
6. **Hazard warning follows the production-vehicle concept**: it is available in every vehicle state including `Off` and fail-safe, it is **not** switched on automatically on signal loss, and it takes priority over the indicators (SYS-LIGHT-002, FUN-LIGHT-002).
7. **Turn indicators** are active in `On`, and in fail-safe they keep working locally on the ZC-STM32 (ADR 0005).

### Arbitration options considered

- A: the CGW is the only master. **Chosen**: without a mode switch on the ZC-STM32 there is only one source.
- B: local override switch on the ZC-STM32. Rejected: it creates two masters.
- C: CGW master, with local authority in fail-safe. Not needed without a mode switch.

## Consequences

- One source of truth for the vehicle state, which is easy to test and explain.
- The ZC-STM32 needs a timeout monitor on the vehicle state message and a vehicle-state-following state machine, on top of its ECU states.
- The CGW is a single point of failure for the vehicle state. This is accepted because the ZC-STM32 has a defined fail-safe.
- The vehicle state needs a CAN signal from the HPC, which is a contract change (`body.dbc`, new version and ADR).

## Conflicts with the current baseline (to be resolved before acceptance)

- **FUN-SAFE-001 / SW-ZCS-021**: on loss of `BODY_LightCmd` (500 ms = 5 × 100 ms), the indicators are switched **off** and the low beam **on**. Decision 7 keeps the indicators working locally instead. Decide what fail-safe means now: indicators off or local, low beam on or unchanged, and which message loss triggers it (`BODY_LightCmd`, the vehicle state, or both).
- **ECU states (SW-ZCS-020)** and **vehicle states** are two different state machines. Decide whether fail-safe on vehicle-state loss is the existing `Degraded` state or a new one.

## Open points

- **Hazard input in fail-safe**: the dashboard and the HPC cannot be reached then, so hazard must be triggerable **locally on the ZC-STM32**. Which input (a joystick button, user button B1, a separate switch)?
- Cycle time of the vehicle state message (5 cycles = ? ms; the light command uses 100 ms).
- Indicator behaviour in `Update`.
- Leaving fail-safe: immediately on the first valid message, or after N valid messages?
