# Body ECU Requirements

System requirements for the body zone ECU. Specs in `specs/` and tests reference these IDs.

**ID scheme:** `REQ-<AREA>-<NNN>`, areas: `SYS` (system-wide), `BODY` (body functions), `DIAG` (diagnostics), `SW` (software architecture).
**Status:** Draft = not yet reviewed · Agreed = baseline for implementation · Verified = test passed.

> These are the initial statements carried over from the first scaffold, now with IDs. Expanding them into the first 10 testable requirements is an M0 issue.

## Functional requirements

| ID | Requirement | Rationale | Verification | Milestone | Status |
|---|---|---|---|---|---|
| REQ-BODY-001 | The ECU shall control the body-zone outputs for hazard lights, left/right indicators and low beam according to requests received on CAN. | Core body function of a zone ECU. | Test on bench (LED observation) + host unit test | M1 | Draft |
| REQ-BODY-002 | The ECU shall accept and transmit only the CAN frames defined in the body DBC contract (`interfaces/can/body.dbc`). | The DBC is the single source of truth for CAN communication. | Test with CAN trace / loopback | M1 | Draft |
| REQ-BODY-003 | The ECU shall expose explicit, observable state transitions for hazard, indicator and mirror behaviour. | Predictable behaviour and diagnosability. | Review + host unit test | M1 | Draft |
| REQ-DIAG-001 | The ECU shall provide a diagnostic interface for status and fault reporting as defined in `interfaces/diag/body_ecu.yaml`. | Diagnosability of the ECU in the vehicle. | Test with UDS tester | M3 | Draft |

## Non-functional requirements

| ID | Requirement | Rationale | Verification | Milestone | Status |
|---|---|---|---|---|---|
| REQ-SYS-001 | The ECU shall behave deterministically during start-up and normal operation. | Repeatable behaviour; basis for timing requirements. | Review + timing measurement | M1 | Draft |
| REQ-SW-001 | The software shall keep generated code (CubeMX, DBC code generation) separate from hand-written application code. | Regeneration must never overwrite project code. | Review | M1 | Draft |
| REQ-SW-002 | The software shall be modular, with layers (functions, com, diag, bsp, sched) that only call downward. | Maintainability; reuse of function logic in virtual twin and unit tests. | Review | M1 | Draft |
