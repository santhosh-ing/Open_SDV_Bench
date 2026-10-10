# AGENTS

This repository follows the Open SDV Bench working model:

- Use Spec Kit for feature definition and approval.
- Keep interfaces and contracts explicit before firmware implementation.
- Separate generated CubeMX code from hand-written application code.
- Keep application logic layered as BSP, COM, DIAG, and FUNCTIONS.
- Prefer small, reviewable increments with feature specs in `specs/`.

## Repository conventions

- `specs/` contains one feature directory per milestone.
- `docs/` contains system requirements, architecture, ADRs, and diagrams.
- `interfaces/` contains DBC and diagnostic contracts written before implementation.
- `ecus/<target>/app/` contains the implementation layers for the board.

## Working expectations

1. Define the contract in `interfaces/` before firmware logic.
2. Capture the feature in `specs/` with a brief spec, plan, and task breakdown.
3. Keep generated files under `ecus/.../Core` and `Drivers` untouched by hand.
4. Implement only inside `app/` for product logic.
5. Validate completeness before shipping changes.
