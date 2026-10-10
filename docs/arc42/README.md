# Arc42 Architecture

This architecture baseline documents the body-zone ECU at a high level.

## Context

The system is a body control ECU connected through CAN and a local microcontroller environment. It manages body outputs, status reporting, and feature logic driven by vehicle signals.

## Building blocks

- Board support layer: board-level hardware access
- Communication layer: CAN message handling and packing
- Diagnostic layer: UDS-style service access
- Functions layer: feature-specific logic such as hazard, indicator, and mirror behavior
