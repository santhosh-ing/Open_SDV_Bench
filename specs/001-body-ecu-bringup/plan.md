# Plan: Body ECU Bring-up

## Objective

Create the initial project skeleton for the body-zone STM32 ECU and set the standards for future feature work.

## Context

This is the baseline project for body-related ECU development. It sets the architecture, repository conventions, and interface-first approach required for automotive embedded work.

## Workstreams

### 1. Repository and spec scaffolding

- Create the initial repo structure.
- Capture the feature using the project template.

### 2. Hardware project structure

- Add the STM32 project folder and CubeMX-style configuration.
- Reserve generated source folders for generated code.

### 3. Application structure

- Introduce BSP, COM, DIAG, and functions layers.
- Provide placeholder modules that reflect the intended architecture.

### 4. Verification

- Confirm the folder layout matches the intended repo contract.
- Ensure the project is ready for further feature development.

## Risks and mitigations

- Risk: generated code and app code become mixed.
- Mitigation: keep generated folders isolated and clearly documented.

## Open questions

- Which exact MCU and board variant will be used for the final vehicle build?
- Will the first production feature focus on lighting, mirrors, or broader body control?
