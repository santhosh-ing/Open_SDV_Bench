# ADR 0001: Use an AUTOSAR-inspired layered application structure

## Status

Accepted

## Context

The body ECU requires a predictable structure that is maintainable across multiple features and hardware variations. The project also needs a clean separation between generated code and manually implemented logic.

## Decision

We will organize the application into BSP, COM, DIAG, and FUNCTIONS layers, with generated CubeMX files kept separate from custom implementation.

## Consequences

- Improved readability and maintainability
- Clear ownership of each software responsibility
- Easier integration of future vehicle features
