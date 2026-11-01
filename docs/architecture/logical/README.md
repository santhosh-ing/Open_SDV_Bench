# Logical view

Vehicle functions and the information flows between them — **no ECUs, no buses**. Flows are named after VSS signals where possible.

Planned functions: speed sensing, emergency brake detection, hazard activation, indicator control, low beam control, mirror control, fault monitoring, diagnostic access.

## Turn indication, hazard and vehicle state

Diagram: [`turn-indication.mmd`](turn-indication.mmd). Decisions: ADR 0005 (indication local in the body zone), ADR 0006 (vehicle state management).

| Function | Role | Requirements |
|---|---|---|
| F2 Manage vehicle state | CGW (vehicle state manager) | SYS-VEH-001 |
| F4 Read joystick, F5 Decode direction request | ZC01 | FUN-LIGHT-001, FUN-LIGHT-010, FUN-LIGHT-011 |
| F6 Read hazard switch | ZC01 | FUN-LIGHT-012 |
| F7 Supervise vehicle state | ZC01 | FUN-VEH-001, FUN-SAFE-001 |
| F8 Arbitrate indication | ZC01 | FUN-LIGHT-002, FUN-LIGHT-012, FUN-VEH-001 |
| F9 Generate blink, F10 Drive indicator lamps | ZC01 | FUN-LIGHT-001, FUN-LIGHT-002 |
| F11 Report indication status, F12 Display indication status | ZC01, CGW | FUN-DIAG-001 |

The same view is modelled graphically in Capella (Arcadia logical architecture, diagram `LAB_TurnIndication`). The Mermaid file is the text version for review in git.
