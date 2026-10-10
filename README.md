# Open SDV Bench

This repository is a working bench for body ECU development and system-level vehicle interface definition. It separates contracts, architecture documentation, and generated firmware scaffolding from hand-written application code.

## Repository layout

- `specs/` — one feature folder per implementation milestone.
- `.specify/` — Spec Kit metadata and templates.
- `docs/` — requirements, architecture, ADRs, and diagrams.
- `interfaces/` — CAN and diagnostic contracts.
- `ecus/body-zone-stm32/` — MCU project and application layers.

## Current baseline

The workspace currently includes the scaffold for:

- Body ECU bring-up
- Body ECU CAN light control
- Body-zone STM32 hardware project structure

## Recommended workflow

1. Define vehicle contracts in `interfaces/`.
2. Capture feature intent in `specs/<feature>/`.
3. Implement the board support and communications layers.
4. Validate behavior through the app layer before expanding the ECU feature set.
