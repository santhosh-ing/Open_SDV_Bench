# Body ECU Requirements

## Functional requirements

1. The ECU shall provide body-zone output control for warning lights and indicator states.
2. The ECU shall accept CAN frames defined in the body DBC contract.
3. The ECU shall support a minimal diagnostic interface for status and fault reporting.
4. The ECU shall preserve a clear separation between generated code and application code.

## Non-functional requirements

1. The system shall remain deterministic during startup and normal operation.
2. The design shall favor modularity and maintainability.
3. The software shall expose explicit state transitions for hazard, indicator, and mirror behavior.
