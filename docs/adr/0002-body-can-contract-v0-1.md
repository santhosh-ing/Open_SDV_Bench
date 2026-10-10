# ADR 0002: Body CAN contract v0.1 (command/status split)

## Status

Accepted

## Context

The placeholder `interfaces/can/body.dbc` (v1.0) could not be parsed by DBC tools (invalid signal syntax, value table attached to the wrong message). It also had no command direction: both messages were sent by the body ECU, so nothing on the bus could request a light function. Signals used whole bytes for booleans, and no cycle times were defined, so timeout supervision was impossible.

The body ECU needs a contract that:

- separates **requests** (from the central computer) from **actual state** (from the ECU), as in production vehicles,
- defines cycle times so command loss can be detected and a fail-safe state entered,
- carries alive counters so stale or repeated frames can be detected,
- can be turned into C and Python code automatically (cantools), so no CAN layout is hand-coded.

## Decision

Replace the placeholder with contract **v0.1.0**:

| ID | Message | Sender → Receiver | Cycle |
|---|---|---|---|
| 0x200 | BODY_LightCmd | HPC → BODY_ECU | 100 ms |
| 0x210 | BODY_LightStatus | BODY_ECU → HPC, TESTER | 100 ms |

- CAN classic, 11-bit IDs, 500 kbit/s.
- Booleans are 1-bit signals; each message has a 4-bit rolling alive counter in byte 7.
- `BODY_LightCmd` missing for ≥ 500 ms = command timeout (fail-safe defined in spec 002).
- Mirror fold signals are reserved now and used in the servo milestone. The earlier "mirror heat" signal is dropped (no heater on the bench).
- The file is validated with `python3 -m cantools dump interfaces/can/body.dbc`.

## Consequences

- Firmware and the future virtual twin generate their pack/unpack code from this file.
- The placeholder `app/com/can_signals.c` (pack/unpack of the old BODY_STATUS layout) no longer matches and will be replaced by generated code in spec 002.
- Any further change to this contract needs a new ADR and a version bump in the DBC.
