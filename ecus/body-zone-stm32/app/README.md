# Application Layer

This folder contains hand-written application code for the body ECU.

## Layered responsibilities

- `bsp/` — board-level drivers for LEDs, buttons, and servos.
- `com/` — CAN wrapper and message packing logic.
- `diag/` — diagnostic and UDS-related handling.
- `functions/` — domain logic such as hazard and indicator behavior.
