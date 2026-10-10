# CLAUDE

This repository is intended for body ECU and vehicle integration work.

## Mission

- Define vehicle-facing contracts before firmware work begins.
- Keep generated CubeMX output separate from hand-written application logic.
- Use feature folders in `specs/` to drive incremental design work.

## Required structure

- `specs/` — each feature should get a numbered folder.
- `docs/` — system requirements and architecture artifacts.
- `interfaces/` — contracts expressed in DBC and YAML.
- `ecus/<target>/app/` — product code layered by responsibility.

## Guardrails

- Do not hand-edit generated code under `Core/` or `Drivers/`.
- Keep the DBC and diagnostic definitions authoritative sources for CAN and UDS behavior.
- Prefer clear function boundaries in `bsp/`, `com/`, `diag/`, and `functions/`.
