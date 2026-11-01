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

## Resolution of the conflicts and open points (2026-11-01)

| Item | Resolution |
|---|---|
| FUN-SAFE-001 / SW-ZCS-021 | Changed: on loss of `BODY_LightCmd` or `VEH_State` for 500 ms or longer, the low beam goes on, central requests are ignored, and joystick indication and the local hazard switch keep working as in `On`. |
| ECU states (SW-ZCS-020) | Fail-safe reuses the existing ECU state `Degraded`; no new ECU state. Vehicle states (Off / On / Update) are a separate state machine that the ECU follows. |
| Hazard input in fail-safe | The joystick push button (FUN-LIGHT-012, SW-ZCS-052). |
| Cycle time | `VEH_State` every 100 ms; 5 missed cycles = 500 ms (ADR 0007). |
| Indicators in `Update` | Direction indication off, hazard available (FUN-VEH-001, SW-ZCS-054). |
| Leaving fail-safe | On the first valid message, once both `BODY_LightCmd` and `VEH_State` are valid again (as before). |
| Start-up | The vehicle state is treated as `Off` until `VEH_State` is received (SW-ZCS-053). |

New requirements: STK-006, SYS-VEH-001, FUN-VEH-001, SW-ZCS-053, SW-ZCS-054.
