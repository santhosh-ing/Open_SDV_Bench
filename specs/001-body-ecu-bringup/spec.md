# Feature: Body ECU Bring-up

## Summary

Establish the baseline body ECU project so the hardware, project structure, and application layering are ready for future body features.

## Problem statement

The ECU must present a clean, reviewable starting point with hardware configuration, generated code separation, and a basic application layer before feature-specific development begins.

## Functional requirements

- The project must contain a CubeMX-compatible STM32 hardware configuration.
- Generated code must remain isolated from hand-written logic.
- The application must expose a clear BSP, COM, and functions structure.
- The ECU must provide a simple initialization baseline for body outputs.

## Non-functional requirements

- Deterministic startup behavior
- Clear separation of generated and custom code
- Readable layered architecture for future development

## Acceptance criteria

- `ecus/body-zone-stm32/` exists with a hardware configuration and application skeleton.
- `Core/` and `Drivers/` are reserved for generated code.
- Application logic is implemented under `app/`.
- The project is ready for body feature expansion.
